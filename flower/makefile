
files := $(wildcard car*.cpp)
targets := $(files:%.cpp=%.exe)

.PHONY : all
.SUFFIXES: 
MAKEFLAGS += -r

all : $(targets)
	echo $(MAKEFLAGS)

$(targets) : %.exe : %.cpp lib/*.h
	g++ -g -O2 -fno-omit-frame-pointer -std=c++14 -Wall -lstdc++ -o$@ $<


