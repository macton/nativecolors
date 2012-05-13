# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libnativeblue
DEPS     := nativeblue.h pthread_rwlock.h
SOURCES  := nativeblue.c pthread_rwlock.c

export PROJECT
export SOURCES
export DEPS

all: publish devel devel-debug

publish:
	make -f MakeLib.mak x86_32 
	make -f MakeLib.mak x86_64 

devel:
	make -f MakeLib.mak x86_32 devel
	make -f MakeLib.mak x86_64 devel

devel-debug:
	make -f MakeLib.mak x86_32 devel debug
	make -f MakeLib.mak x86_64 devel debug

# Just a local target for testing
gcc:
	make -f MakeLib.mak gcc
	make -f MakeLib.mak gcc

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.a
	rm -f *.nexe

install:
	cp *.a $(NATIVECOLORS_ROOT)/lib/
	cp *.h $(NATIVECOLORS_ROOT)/include/
