#
# Makefile
#
# David Pettersson <david@shebang.nu>
#

CXX = g++
CXXFLAGS = -pipe -Wall -W -ansi -pedantic-errors -Wmissing-braces
CXXFLAGS += -Wparentheses -Wold-style-cast -ggdb -I../src
CXXFLAGS += -ftest-coverage -fprofile-arcs
VPATH	= ../src:.
LDFLAGS = -lcppunit -ldl

sources = $(notdir $(wildcard ../src/*.cc))
objects = $(sources:.cc=.o)
depends = $(sources:.cc=.d)

all: test-database

test-database: test-database.o memory-database.o filesystem-database.o database.o
	$(CXX) $(LDFLAGS) -o $@ $^

%.d: %.cc
	$(CXX) -M $< | sed 's/$*.o/& $@/g' > $@

-include $(depends)

clean:
	rm test-database
	rm -f $(depends)
	rm -f *.o 
	rm -f *~

.PHONY: all clean

