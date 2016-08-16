#  Alex Poltavsky, 2013
#  alexpolt@yandex.ru

CC  :=  g++
LIBS  :=  
LIBSGL  :=  -lopengl32 -lglut32
DIRS  :=  -Llib
OPTS  :=  -c -std=c++11 -pedantic -Wall -O2
FMATH := -ffast-math -msse2 -mfpmath=sse
DEBUG := -DALXP_DEBUG

CPP := $(wildcard *.cpp)
OBJS := $(CPP:%.cpp=%.o)
HEADERS := $(wildcard *.h)

.PHONY = all clean test finish
.RECIPEPREFIX = + #prefix

ifeq ($(strip $(target)),)
  finish = finish
endif

all: $(target) $(finish)

$(target): $(OBJS) $(HEADERS);
#+$(CC) $(OPTS) $(FMATH) $(DEBUG) -o $@ $< $(DIRS)

%.o : %.cpp
+$(CC) $(OPTS) $(FMATH) $(DEBUG) $^ $(DIRS)

%.h: ;


finish:
+@echo Nothing to do!

.DEFAULT:
+@echo Wrong target: $@

