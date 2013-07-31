SHELL=/bin/sh

# version of c++ compiler and linker
CXX=	clang++
LINK=	clang++


# location for gtest files
GTEST_DIR = /usr/src/gtest
GTEST_HEADERS = /usr/include/gtest/*.h \
                /usr/include/gtest/internal/*.h
GMOCK_DIR = /usr/src/gmock
GMOCK_HEADERS = /usr/include/gmock/*.h \
                /usr/include/gmock/internal/*.h


# compiler flags
CXXFLAGS += -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual \
	-Wcast-align -Wwrite-strings -fshort-enums -fno-common \
	-O4 -DHAVE_INLINE -stdlib=libc++ -pthread

ifdef DEBUG
CXXFLAGS += -g -O0 -DDEBUG=1
endif #DEBUG

LDLIBS	= -ltcmalloc -lgsl -lcblas -latlas -lm
# additional flags libraries needed for unit tests
CPPFLAGS_TEST = -isystem $(GTEST_DIR)/include -isystem \
	$(GMOCK_DIR)/include
LDLIBS_TEST = -lpthread


# object files to generate, should be named ${foo}.o where source file
# is ${foo}.c
ODIR	= obj
_OBJ	= 
OBJ	= $(patsubst %,$(ODIR)/%,$(_OBJ))
# file containing main() (excluded from test binary, which defines its
# own main() )
_MAIN	= 
MAIN	= $(patsubst %,$(ODIR)/%,$(_OBJ))
# test cases for code in ${foo}.cc should be named ${foo}_test.cc and
# placed in TDIR
TDIR	= tests
_TESTS	= 
TESTS	= $(patsubst %,$(ODIR)/%,$(_TESTS))

# main and unit test binaries
BIN	= tensor
TEST	= tensor_test

GENERATED = $(OBJ) $(BIN) $(TESTS)

#
#	targets
#
.PHONY	:	all
all	:	$(BIN)

.PHONY	:	run
run	:	$(BIN)
	./$^

.PHONY	:	check
check	:	$(TEST)
	./$^

$(BIN)	:	$(OBJ) $(MAIN)
	$(LINK) -o $@ $^ $(CPPFLAGS) $(CXXFLAGS) $(LDLIBS)

$(TEST)	:	$(OBJ) $(TESTS) gmock_main.a
	$(LINK) -o $@ $^ $(CPPFLAGS) $(CPPFLAGS_TEST) $(CXXFLAGS) \
	$(LDLIBS) $(LDLIBS_TEST)

.PHONY	:	objs
objs	:	$(OBJ)

$(ODIR)/%.o : %.cc %.d
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

$(ODIR)/%_test.o : $(TDIR)/%_test.cc $(TDIR)/%_test.d
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CPPFLAGS_TEST) $(CXXFLAGS)

%.d	:	%.cc
	$(SHELL) -ec '$(CXX) -M $(CPPFLAGS) $(CPPFLAGS_TEST) $< | sed '\"s/$*.o/& $@/g'\" > $@'

#debugging and cleaning targets
.PHONY	:	gdb
gdb	:	$(BIN)
	gdb ./$(BIN)

.PHONY	:	valgrind
valgrind :	$(BIN)
	valgrind --trace-children=yes --leak-check=full \
		--show-reachable=yes ./$(BIN)

.PHONY	:	clean
clean	:
	-rm -f $(OBJ)

.PHONY	:	distclean
distclean :
	-rm -f $(GENERATED)

# Building Google Test and Google Mock

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_HEADERS)
GMOCK_SRCS_ = $(GMOCK_DIR)/src/*.cc $(GMOCK_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
$(ODIR)/gtest-all.o : $(GTEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            -o $@ $(GTEST_DIR)/src/gtest-all.cc

$(ODIR)/gtest_main.o : $(GTEST_SRCS_) $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            -o $@ $(GTEST_DIR)/src/gtest_main.cc

$(ODIR)/gmock-all.o : $(GTEST_HEADERS) $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) -c \
            -o $@ $(GMOCK_DIR)/src/gmock-all.cc

$(ODIR)/gmock_main.o : $(GTEST_HEADERS) $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) -c \
            -o $@ $(GMOCK_DIR)/src/gmock_main.cc

gtest.a : $(ODIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : $(ODIR)/gtest-all.o $(ODIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

gmock.a : $(ODIR)/gtest-all.o gmock-all.o
	$(AR) $(ARFLAGS) $@ $^

gmock_main.a : $(ODIR)/gtest-all.o $(ODIR)/gmock-all.o \
		$(ODIR)/gmock_main.o
	$(AR) $(ARFLAGS) $@ $^
