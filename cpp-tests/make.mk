#  Alex Poltavsky, 2013
#  alexpolt@yandex.ru

CC  =  g++

LIBS =
GLLIBS = -lopengl32 -lgdi32
DSLIBS = -ldsound

ifeq ($(target:dsound%=dsound), dsound)
  LIBS := $(LIBS) $(DSLIBS)
endif

ifeq ($(target:opengl%=opengl), opengl)
  LIBS := $(LIBS) $(GLLIBS)
endif

DIRS  =  
OPTS  =  -std=c++11 -Wall -Winline -pedantic
FMATH = -ffast-math -msse2 -mfpmath=sse -O2 -static
DEBUG = -DALXP_DEBUG

.PHONY := all clean test finish phony
.RECIPEPREFIX = + #prefix

ifeq ($(strip $(target)),)
  finish = finish
endif

all: $(target) $(finish)

deps := $(wildcard $(target:.exe=)*) $(target:.exe=rc.o)

$(deps) ::;

$(target):: $(deps)
+$(CC) $(OPTS) $(FMATH) $(DEBUG) -o $@ $(target:.exe=.cpp) $(target:.exe=rc.o) $(DIRS) $(LIBS) $(cmdlibs)

$(target:.exe=rc.o) :: $(target:.exe=rc.rc)
+windres.exe $< $@

finish:
+@echo Nothing to do!

.DEFAULT:
+@echo Wrong target: $@

