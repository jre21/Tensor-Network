SHELL = /bin/sh

# version of c++ utilities
CXX = clang++
LINK = clang++
DB = lldb-3.3

ifneq "$(target)" "release"
ifneq "$(target)" "debug"
override target = testing
endif # target != debug
endif # target != release

# compiler flags
CPPFLAGS = -std=c++11 -pthread -DHAVE_INLINE
CXXFLAGS = -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual \
           -Wcast-align -Wwrite-strings -fshort-enums -fno-common \
           -pedantic -stdlib=libc++ -march=native
LDFLAGS = -stdlib=libc++ -fuse-ld=gold
LDLIBS = -ltcmalloc -lgsl -lcblas -latlas -lm -lpthread

ifeq "$(target)" "release"
# add -DNO_ERROR_CHECKING to remove internal error checks
CXXFLAGS += -O3
endif # target == release
ifeq "$(target)" "testing"
CXXFLAGS += -O2 -fvectorize
endif # target == testing
ifeq "$(target)" "debug"
CXXFLAGS += -g -O0 -DDEBUG=1

endif # target == debug
targets = release testing debug

# object files to generate, should be named ${foo}.o where source file
# is ${foo}.c
_OBJ = tensor utils log_msg
OBJ = $(patsubst %,$(target)/%.o,$(_OBJ))
ALL_OBJ = $(foreach foo,$(targets),$(patsubst %,$(foo)/%.o,$(_OBJ)))
# file containing main() (excluded from test binary, which defines its
# own main() )
_MAIN = main
MAIN = $(patsubst %,$(target)/%.o,$(_MAIN))
ALL_MAIN = $(foreach foo,$(targets),$(patsubst %,$(foo)/%.o,$(_MAIN)))
# test cases for code in ${foo}.cc should be named ${foo}$(TSUF).cc
# and placed in TDIR
TDIR = test
TSUF = _test
_TESTS = tensor utils
TESTS = $(patsubst %,$(target)/%$(TSUF).o,$(_TESTS))
ALL_TESTS = $(foreach foo,$(targets),$(patsubst %,$(foo)/%$(TSUF).o,$(_TESTS)))
MPRE = mock_
_MOCKS = tensor
MOCKS = $(patsubst %,$(target)/$(MPRE)%.o,$(_MOCKS))
ALL_MOCKS = $(foreach foo,$(targets),$(patsubst %,$(foo)/$(MPRE)%.o,$(_MOCKS)))

# main and unit test binaries
_BIN = tensor
_TEST = tensor$(TSUF)
BIN = $(_BIN)_$(target).bin
TEST = $(_TEST)_$(target).bin

# library files that shouldn't normally need to be rebuilt
_LIBS = .a -all.o _main.a _main.o
_LIB_OBJS = $(foreach lib,$(_LIBS),gmock$(lib) gtest$(lib))
LIB_OBJS = $(foreach foo,$(targets),$(patsubst %,$(foo)/%,$(_LIB_OBJS)))

# dependency files
DEPS = $(patsubst %,%.d,$(_OBJ)) \
       $(patsubst %,$(TDIR)/%$(TSUF).d,$(_TESTS)) \
       $(patsubst %,$(TDIR)/$(MPRE)%.d,$(_MOCKS))

OBJECTS = $(ALL_OBJ) $(ALL_MAIN) $(ALL_TESTS) $(ALL_MOCKS)
GENERATED = $(OBJECTS) $(LIB_OBJS) $(DEPS)

# targets
.PHONY	:	all
all	:	$(BIN)

.PHONY	:	run
run	:	$(BIN)
	./$<

.PHONY	:	test
test	:	$(TEST)
	./$<

.PHONY	:	check
check	:	$(TEST)
	./$<

$(BIN)	:	$(OBJ) $(MAIN)
	$(LINK) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(TEST)	:	$(OBJ) $(TESTS) $(MOCKS) $(target)/gmock_main.a
	$(LINK) $(LDFLAGS) -o $@ $^ $(LDLIBS)

.PHONY	:	objs
objs	:	$(OBJ)

$(target)/%.o : %.cc %.d
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

$(target)/%$(TSUF).o : $(TDIR)/%$(TSUF).cc $(TDIR)/%$(TSUF).d
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

$(target)/$(MPRE)%.o : $(TDIR)/$(MPRE)%.cc $(TDIR)/$(MPRE)%.d
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

# include dependency information
include $(patsubst %,%.d,$(_OBJ)) $(patsubst %,%.d,$(_MAIN)) \
	$(patsubst %,$(TDIR)/%$(TSUF).d,$(_TESTS)) \
	$(patsubst %,$(TDIR)/$(MPRE)%.d,$(_MOCKS))

# generate dependency information
%.d	:	%.cc
	@set -e; rm -f $@; \
         $(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($(*F)\)\.o[ :]*,$(target)/\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

# debugging and cleaning targets
.PHONY	:	debug
debug	:	$(BIN)
	$(DB) $<

.PHONY	:	debug_test
debug_test :	$(TEST)
	$(DB) $<

.PHONY	:	valgrind
valgrind :	$(BIN)
	valgrind --trace-children=yes --leak-check=full \
		--show-reachable=yes ./$<

.PHONY	:	clean
clean	:
	-rm -f $(OBJECTS)

.PHONY	:	distclean
distclean :
	-rm -f $(GENERATED)

# Building Google Test and Google Mock

# location for gtest files
GTEST_DIR = /usr/src/gtest
GTEST_HEADERS = /usr/include/gtest/*.h \
                /usr/include/gtest/internal/*.h
GMOCK_DIR = /usr/src/gmock
GMOCK_HEADERS = /usr/include/gmock/*.h \
                /usr/include/gmock/internal/*.h

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_HEADERS)
GMOCK_SRCS_ = $(GMOCK_DIR)/src/*.cc $(GMOCK_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
$(target)/gtest-all.o : $(GTEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) \
            -c -w -o $@ $(GTEST_DIR)/src/gtest-all.cc

$(target)/gtest_main.o : $(GTEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) \
            -c -w -o $@ $(GTEST_DIR)/src/gtest_main.cc

$(target)/gmock-all.o : $(GTEST_HEADERS) $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
            -c -w -o $@ $(GMOCK_DIR)/src/gmock-all.cc

$(target)/gmock_main.o : $(GTEST_HEADERS) $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
            -c -w -o $@ $(GMOCK_DIR)/src/gmock_main.cc

$(target)/gtest.a : $(target)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(target)/gtest_main.a : $(target)/gtest-all.o $(target)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

$(target)/gmock.a : $(target)/gtest-all.o gmock-all.o
	$(AR) $(ARFLAGS) $@ $^

$(target)/gmock_main.a : $(target)/gtest-all.o $(target)/gmock-all.o \
		$(target)/gmock_main.o
	$(AR) $(ARFLAGS) $@ $^
