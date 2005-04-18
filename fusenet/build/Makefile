#
# Makefile
#

CXX = g++
CXXFLAGS = -pipe -O2 -Wall -W -ansi -pedantic-errors -Wmissing-braces
CXXFLAGS += -Wparentheses -Wold-style-cast -g
VPATH	= ../src:./
LDFLAGS =

program = fusenet
sources = $(notdir $(wildcard ../src/*.cc))
objects = $(sources:.cc=.o)
depends = $(sources:.cc=.d)

all: $(program) 

test-database: test-database.o memory-database.o database.o
	$(CXX) -lcppunit -ldl $^ -o $@

$(program): $(objects)
	$(CXX) $(LDFLAGS) -o $@ $^

%.d: %.cc
	$(CXX) -M $< | sed 's/$*.o/& $@/g' > $@

-include $(depends)

clean:
	rm -f $(objects) $(depends) $(program) *~

.PHONY: all clean

