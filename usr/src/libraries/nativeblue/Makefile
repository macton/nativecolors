# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libnativeblue
DEPS     := nativeblue.h nativeblue_private.h
SOURCES  := nativeblue.c pthread_rwlock.c writev.c filesys.c sleep.c write.c ioctl.c urlencode.c pow.c fd.c open.c close.c lseek.c cwd.c read.c

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
	cp nativeblue.h $(NATIVECOLORS_ROOT)/usr/include/
