# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := hello
SOURCES  := hello.c ../../nativeblack/nativeblack.c ../../nativeblue/nativeblue.c ../../nativeblue/pthread_rwlock.c
DEPS     := 

export PROJECT
export SOURCES
export DEPS

all: publish devel devel-debug glibc-publish glibc-devel glibc-devel-debug

publish:
	make -f MakeNexe.mak x86_32 
	make -f MakeNexe.mak x86_64 

devel:
	make -f MakeNexe.mak x86_32 devel
	make -f MakeNexe.mak x86_64 devel

devel-debug:
	make -f MakeNexe.mak x86_32 devel debug
	make -f MakeNexe.mak x86_64 devel debug

glibc-publish:
	make -f MakeNexe.mak x86_32 glibc  
	make -f MakeNexe.mak x86_64 glibc  

glibc-devel:
	make -f MakeNexe.mak x86_32 glibc devel
	make -f MakeNexe.mak x86_64 glibc devel

glibc-devel-debug:
	make -f MakeNexe.mak x86_32 glibc devel debug
	make -f MakeNexe.mak x86_64 glibc devel debug

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.nexe
	rm -f ../../nativeblack/*.o
	rm -f ../../nativeblue/*.o

