# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libcjson
DEPS     := $(NATIVECOLORS_ROOT)/usr/packages/cJSON/cJSON.h
SOURCES  := $(NATIVECOLORS_ROOT)/usr/packages/cJSON/cJSON.c

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
	make -f MakeLib.mak clean x86_32 
	make -f MakeLib.mak clean x86_64 
	make -f MakeLib.mak clean x86_32 devel
	make -f MakeLib.mak clean x86_64 devel
	make -f MakeLib.mak clean x86_32 devel debug
	make -f MakeLib.mak clean x86_64 devel debug

install:
	cp *.a $(NATIVECOLORS_ROOT)/usr/lib/
	cp $(NATIVECOLORS_ROOT)/usr/packages/cJSON/cJSON.h $(NATIVECOLORS_ROOT)/usr/include/
