#  Alex Poltavsky, 2013
#  alexpolt@yandex.ru

CC  =  g++
LIBS  =  -lgdi32
GLLIBS  =  -lopengl32 -lglut32
DIRS  =  
OPTS  =  -std=c++11 -Wall
FMATH = -ffast-math -msse2 -mfpmath=sse -O2
DEBUG = -DALXP_DEBUG

.PHONY = all clean test finish phony
.RECIPEPREFIX = + #prefix

ifeq ($(strip $(target)),)
  finish = finish
endif

all: $(target) $(finish)

$(target)::
+make -C .. target=dummy

deps := $(wildcard ../*.o)

$(target)::
+$(CC) $(OPTS) $(FMATH) $(DEBUG) -o $@ $(target:%.exe=%.cpp) $< $(deps) $(DIRS) $(LIBS) 

finish:
+@echo Nothing to do!

.DEFAULT:
+@echo Wrong target: $@

