
files := $(wildcard car*.cpp)
targets := $(files:%.cpp=%)

.PHONY : all

all : $(targets)

$(targets) : % : %.cpp
	g++ -g -O2 -fno-omit-frame-pointer -std=c++14 -Wall -lstdc++ -o$@ $<


