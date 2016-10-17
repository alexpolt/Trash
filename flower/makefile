
files := $(wildcard *.cpp)
targets := $(files:%.cpp=%.exe)
dtargets := $(targets:%=d%)

.PHONY : all
.SUFFIXES: 
MAKEFLAGS += -r

all : $(targets)
debug : $(dtargets)

$(targets) : %.exe : %.cpp lib/*.h makefile
	g++ -D_ISOC99_SOURCE -msse2 -march=native -ffast-math -O3 -std=c++14 -Wall -lstdc++ -o$@ $<
	clang -D_ISOC99_SOURCE -msse2 -march=native -ffast-math -O3 -std=c++14 -Wall -lstdc++ -ocl-$@ $<

$(dtargets) : d%.exe : %.cpp lib/*.h makefile
	g++ -D_ISOC99_SOURCE -msse2 -march=native -ffast-math -g -O0 -fno-omit-frame-pointer -std=c++14 -Wall -lstdc++ -o$@ $<
	clang -D_ISOC99_SOURCE -msse2 -march=native -ffast-math -g -O0 -fno-omit-frame-pointer -std=c++14 -Wall -lstdc++ -ocl-$@ $<

