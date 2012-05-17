# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libnativeyellow
DEPS     := nativeyellow.h pthread_rwlock.h
SOURCES  := nativeyellow.c pthread_rwlock.c writev.c chmod.c unlink.c lstat.c

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
	@mkdir -p $(NATIVECOLORS_ROOT)/srv/www/nativeyellow
	cp *.a $(NATIVECOLORS_ROOT)/usr/lib/
	cp *.h $(NATIVECOLORS_ROOT)/usr/include/
