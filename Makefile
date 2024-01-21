export LDFLAGS = -pthread -lm
export CFLAGS = -std=c++11 -Wall -O2 -Wno-unknown-pargmas -funroll-loops \
	-Iinclude -Idmlc-core/include -fPIC

# specify tensor path
.PHONY: clean all test

all: lib/libtvm.a

SRC = $(wildcard src/*.cc src/*/*.cc)
ALL_OBJ = $(patsubst src/%.cc, build/%.o, $(SRC))
ALL_DEP = $(ALL_OBJ)

$(info --------------)
include tests/cpp/unittest.mk

test: $(TEST)

$(info $(GTEST_LIB))

build/%.o: src/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -MM -MT build/$*.o $< >build/$*.d
	$(CXX) -c $(CFLAGS) -c $< -o $@

lib/libtvm.a: $(ALL_DEP)
	@mkdir -p $(@D)
	ar crv $@ $(filter %.o, $?)

lint:
	python dmlc-core/scripts/lint.py tvm cpp include src

clean:
	$(RM) -rf build lib bin *~ */*~ */*/*~ */*/*/*~ */*.o */*/*.o */*/*/*.o */*.d */*/*.d */*/*/*.d

-include build/*.o
-include build/*/*.d
