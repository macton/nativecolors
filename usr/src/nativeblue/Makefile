# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libnativeblue
DEPS     := nativeblue.h pthread_rwlock.h
SOURCES  := nativeblue.c pthread_rwlock.c writev.c

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

.PHONY: clean
clean:
	make -f MakeLib.mak clean x86_32 devel debug
	make -f MakeLib.mak clean x86_64 devel debug
	make -f MakeLib.mak clean x86_32 devel
	make -f MakeLib.mak clean x86_64 devel
	make -f MakeLib.mak clean x86_32
	make -f MakeLib.mak clean x86_64

install:
	@mkdir -p $(NATIVECOLORS_ROOT)/srv/www/nativeblue
	cp nativeblueterm.js $(NATIVECOLORS_ROOT)/srv/www/nativeblue
	cp *.a $(NATIVECOLORS_ROOT)/usr/lib/
	cp *.h $(NATIVECOLORS_ROOT)/usr/include/
