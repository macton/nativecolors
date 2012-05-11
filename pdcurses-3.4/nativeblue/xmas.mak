# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := xmas
DEPS     := 
SOURCES  := xmas.c 

ifneq (gcc,$(filter gcc,$(MAKECMDGOALS)))
SOURCES  += ../../nativeblack/nativeblack.c ../../nativeblue/nativeblue.c ../../nativeblue/pthread_rwlock.c
endif

export PROJECT
export SOURCES
export DEPS

all: publish devel devel-debug

publish:
	make -f MakeNexe.mak x86_32 
	make -f MakeNexe.mak x86_64 

devel:
	make -f MakeNexe.mak x86_32 devel
	make -f MakeNexe.mak x86_64 devel

devel-debug:
	make -f MakeNexe.mak x86_32 devel debug
	make -f MakeNexe.mak x86_64 devel debug

gcc:
	make -f MakeNexe.mak gcc
	make -f MakeNexe.mak gcc

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.a
	rm -f *.nexe
	rm -f ../pdcurses/*.o

