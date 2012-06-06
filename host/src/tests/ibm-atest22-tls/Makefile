# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

CWD      := $(shell pwd)
PROJECT  := $(shell basename $(CWD))
DEPS     := 
SOURCES  := hello.c

export PROJECT
export SOURCES
export DEPS

all: publish devel devel-debug

publish:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak x86_32 
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak x86_64 

devel:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak x86_32 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak x86_64 devel

devel-debug:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak x86_32 devel debug
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak x86_64 devel debug

.PHONY: clean
clean:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_32 devel debug
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_64 devel debug
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_32 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_64 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_32
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_64
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean gcc
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean gcc

.PHONY: install
install:
	@mkdir -p $(NATIVECOLORS_ROOT)/host/src/tests/bin/
	cp -R bin/* $(NATIVECOLORS_ROOT)/host/src/tests/bin/
