
files := $(wildcard *.cpp)
targets := $(files:%.cpp=%.exe)
dtargets := $(targets:%=d%)

.PHONY : all
.SUFFIXES: 
MAKEFLAGS += -r

OPTS := -D_ISOC99_SOURCE -march=native -ffast-math -fno-rtti -O3 -std=c++14 -Wall -lstdc++

all : $(targets)
debug : $(dtargets)

$(targets) : %.exe : %.cpp lib/*.h makefile
	g++ $(OPTS) -o$@ $<
	clang $(OPTS) -ocl-$@ $<

$(dtargets) : d%.exe : %.cpp lib/*.h makefile
	g++ $(OPTS) -o$@ $<
	clang $(OPTS) -ocl-$@ $<

