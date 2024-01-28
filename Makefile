ROOTDIR = $(CURDIR)

ifndef config
ifneq ("$(wildcard ./config.mk)","")
	config ?= config.mk
else
	config ?= make/config.mk
endif
endif

include $(config)


# specify tensor path
.PHONY: clean install installdev all test doc pylint cpplint lint verilog cython2 web runtime runtime

ifndef DMLC_CORE_PATH
	DMLC_CORE_PATH = $(ROOTDIR)/dmlc-core
endif

ifndef DLPACK_PATH
	DLPACK_PATH = $(ROOTDIR)/dlpack
endif

# THe source code dependencies
LIB_HALIDEIR = HalideIR/lib/libHalideIR.a

CC_SRC = $(filter-out src/contrib/%.cc src/runtime/%.cc,\
			$(wildcard src/*/*.cc src/*/*/*.cc))
METAL_SRC = $(wildcard src/runtime/metal/*.mm)
CUDA_SRC = $(wildcard src/runtime/cuda/*.cc)
OPENCL_SRC = $(wildcard src/runtime/opencl/*.cc)
RPC_SRC = $(wildcard src/runtime/rpc/*.cc)
RUNTIME_SRC = $(widlcard src//runtime/*.cc)

# Objectives
METAL_OBJ = $(patsubst src/%.mm, build/%.o, $(METAL_SRC))
CUDA_OBJ = $(patsubst src/%.cc, build/%.o, $(CUDA_SRC))
OPENCL_OBJ = $(patsubst src/%.cc, build/%.o, $(OPENCL_SRC))
RPC_OBJ = $(patsubst src/%.cc, build/%.o, $(RPC_SRC))
CC_OBJ = $(patsubst src/%.cc, build/%.o, $(CC_SRC))
RUNTIME_OBJ = $(patsubst src/%.cc, build/%.o, $(RUNTIME_SRC))
CONTRIB_OBJ = 

UNAME_S := $(shell uname -s)

# Deps
ALL_DEP = $(CC_OBJ) $(CONTRIB_OBJ) $(LIB_HALIDEIR)
RUNTIME_DEP = $(RUNTIME_OBJ)

# The flags
LDFLAGS = -pthread -lm -ldl
INCLUDE_FLAGS = -Iinclude -I$(DLPACK_PATH)/include -I$(DMLC_CORE_PATH)/include -IHalideIR/src -Itopi/include
CFLAGS = -std=c++11 -Wall -O0 -g $(INCLUDE_FLAGS) -fPIC
LLVM_CFLAGS = -fno-rtti -DDMLC_ENABLE_RTTI=0
FRAMEWORKS = 
OBJCFLAGS = -fno-objc-arc
EMCC_FLAGS = -s RESERVED_FUNCTION_POINTERS=2 -s NO_EXIT_RUNTIME=1 -s MAIN_MODULE=1 -DDMLC_LOG_STACK_TRACK=0 \
			 -std=c++11 -Oz $(INCLUDE_FLAGS)


# Dependency specific rules
ifdef CUDA_PATH
	NVCC=$(CUDA_PATH)/bin/nvcc
	CFLAGS += -I$(CUDA_PATH)/include
	LDFLAGS += -L$(CUDA_PATH)/lib64
endif


SRC = $(wildcard src/*.cc src/*/*.cc)
ALL_OBJ = $(patsubst src/%.cc, build/%.o, $(SRC))
ALL_DEP = $(ALL_OBJ) $(LIB_HALIDE_IR)

include tests/cpp/unittest.mk

test: $(TEST)

build/%.o: src/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -MM -MT build/$*.o $< >build/$*.d
	$(CXX) -c $(CFLAGS) -c $< -o $@

lib/libtvm.a: $(ALL_DEP)
	@mkdir -p $(@D)
	ar crv $@ $(filter %.o, $?)

lib/libtvm.so: $(ALL_DEP)
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -shared -o $@ $(filter %.o %.a, $^) $(LDFLAGS)

lint:
	python dmlc-core/scripts/lint.py txtvm cpp include src

clean:
	$(RM) -rf build lib bin *~ */*~ */*/*~ */*/*/*~ */*.o */*/*.o */*/*/*.o */*.d */*/*.d */*/*/*.d

-include build/*.d
-include build/*/*.d
