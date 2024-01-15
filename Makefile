export LDFLAGS = -pthread -lm
export CFLAGS =  -std=c++11 -Wall -O2  -Wno-unknown-pragmas -funroll-loops\
	 -Iinclude -Idmlc-core/include -fPIC

# specify tensor path
.PHONY: clean all test doc

all: lib/libtxtvm.a lib/libtxtvm.so
SRC = $(wildcard src/*.cc src/*/*.cc)
ALL_OBJ = $(patsubst src/%.cc, build/%.o, $(SRC))
ALL_DEP = $(ALL_OBJ)

include tests/cpp/unittest.mk

test: $(TEST)

build/%.o: src/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -MM -MT build/$*.o $< >build/$*.d
	$(CXX) -c $(CFLAGS) -c $< -o $@


lib/libtxtvm.a: $(ALL_DEP)
	@mkdir -p $(@D)
	ar crv $@ $(filter %.o, $?)

lib/libtxtvm.so: $(ALL_DEP)
	@mkdir -p $(@D)
	$(CXX) $(CPLAGS) -shared -o $@ $(filter %.o %.a, $^) $(LDFLAGS)

lint:
	python3 dmlc-core/scripts/lint.py txtvm cpp include src

doc:
	doxygen docs/Doxygen

clean:
	$(RM) -rf build lib bin *~ */*~ */*/*~ */*/*/*~ */*.o */*/*.o */*/*/*.o */*.d */*/*.d */*/*/*.d

-include build/*.d
-include build/*/*.d
