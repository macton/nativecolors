# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libnativeblack
DEPS     := nativeblack.h tweak.h
SOURCES  := nativeblack.c

export PROJECT
export SOURCES
export DEPS

all: publish devel devel-debug

publish:
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak x86_32 
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak x86_64 

devel:
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak x86_32 devel
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak x86_64 devel

devel-debug:
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak x86_32 devel debug
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak x86_64 devel debug

.PHONY: clean
clean:
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_32 devel debug
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_64 devel debug
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_32 devel
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_64 devel
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_32
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_64

install:
	cp -R lib/* $(NATIVECOLORS_ROOT)/client/lib/
	cp nativeblack.h $(NATIVECOLORS_ROOT)/client/include/
	cp tweak.h $(NATIVECOLORS_ROOT)/client/include/
