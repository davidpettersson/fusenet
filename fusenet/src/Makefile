#
# Makefile
#
# David Pettersson <david@shebang.nu>
#

CXX = g++
CXXFLAGS = -pipe -O2 -Wall -W -ansi -pedantic-errors -Wmissing-braces
CXXFLAGS += -Wparentheses -Wold-style-cast -g

program = fusenet
sources = $(wildcard *.cc)
objects = $(sources:.cc=.o)
depends = $(sources:.cc=.d)

all: $(program) 

$(program): $(objects)
	$(CXX) -o $@ $^

%.d: %.cc
	$(CXX) -M $< | sed 's/$*.o/& $@/g' > $@

-include $(depends)

clean:
	rm -f $(objects) $(depends) $(program) *~

.PHONY: all clean

