# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libnativeyellow
DEPS     := nativeyellow.h
SOURCES  := nativeyellow.c pthread_rwlock.c writev.c chmod.c unlink.c lstat.c utime.c chown.c ioctl.c

export PROJECT
export SOURCES
export DEPS

all: publish devel devel-debug

publish:
	make -f ../common/MakeLib.mak x86_32 
	make -f ../common/MakeLib.mak x86_64 
	make -f ../common/MakeLib.mak arm

devel:
	make -f ../common/MakeLib.mak x86_32 devel
	make -f ../common/MakeLib.mak x86_64 devel
	make -f ../common/MakeLib.mak arm    devel

devel-debug:
	make -f ../common/MakeLib.mak x86_32 devel debug
	make -f ../common/MakeLib.mak x86_64 devel debug
	make -f ../common/MakeLib.mak arm    devel debug

.PHONY: clean
clean:
	make -f ../common/MakeLib.mak clean x86_32 devel debug
	make -f ../common/MakeLib.mak clean x86_64 devel debug
	make -f ../common/MakeLib.mak clean arm    devel debug
	make -f ../common/MakeLib.mak clean x86_32 devel
	make -f ../common/MakeLib.mak clean x86_64 devel
	make -f ../common/MakeLib.mak clean arm    devel
	make -f ../common/MakeLib.mak clean x86_32
	make -f ../common/MakeLib.mak clean x86_64
	make -f ../common/MakeLib.mak clean arm

install:
	cp -R lib/* $(NATIVECOLORS_ROOT)/host/lib
	cp nativeyellow.h $(NATIVECOLORS_ROOT)/host/include/
