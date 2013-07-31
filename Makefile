SHELL=/bin/sh

#
#	version of c++ compiler and linker
#
CXX=	clang++
LINK=	clang++

#
#	compiler flags
#
CPPFLAGS += -isystem $(GTEST_DIR)/include
CXXFLAGS += -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual \
	-Wcast-align -Wwrite-strings -fshort-enums -fno-common \
	-O4 -DHAVE_INLINE -stdlib=libc++ -pthread

ifdef DEBUG
CXXFLAGS += -g -O0 -DDEBUG=1

LDLIBS	= -ltcmalloc -lgsl -lcblas -latlas -lm
# additional libraries needed for unit tests
LDLIBS_TEST = -lpthread

#
#	location for gtest files
#
GTEST_DIR = /usr/src/gtest
GTEST_HEADERS = /usr/include/gtest/*.h \
                /usr/include/gtest/internal/*.h

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
_TESTS	= main_test.o
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
	$(LINK) -o $@ $^ $(CXXFLAGS) $(LDLIBS)

$(TEST)	:	$(OBJ) $(TESTS) gtest.a
	$(LINK) -o $@ $^ $(CXXFLAGS) $(LDLIBS) $(LDLIBS_TEST)

.PHONY	:	objs
objs	:	$(OBJ)

$(ODIR)/%.o : %.cc %.d
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(ODIR)/%_test.o : $(TDIR)/%_test.cc $(TDIR)/%_test.d
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.d	:	%.cc
	$(SHELL) -ec '$(CXX) -M $(CPPFLAGS) $< | sed '\"s/$*.o/& $@/g'\" > $@'

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

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
