# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libcjson
DEPS     := $(NATIVECOLORS_ROOT)/client/src/packages/cJSON/cJSON.h
SOURCES  := $(NATIVECOLORS_ROOT)/client/src/packages/cJSON/cJSON.c

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
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_32 
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_64 
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_32 devel
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_64 devel
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_32 devel debug
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_64 devel debug

install:
	cp -R lib/* $(NATIVECOLORS_ROOT)/client/lib/
	cp $(NATIVECOLORS_ROOT)/client/src/packages/cJSON/cJSON.h $(NATIVECOLORS_ROOT)/client/include/
