# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := nacl-env
DEPS     := 
SOURCES  := env.c

export PROJECT
export SOURCES
export DEPS

all: publish devel devel-debug

publish:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak x86_32 
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak x86_64 
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak arm

devel:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak x86_32 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak x86_64 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak arm    devel

devel-debug:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak x86_32 devel debug
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak x86_64 devel debug
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak arm    devel debug

gcc:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak gcc

.PHONY: clean
clean:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_32 devel debug
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_64 devel debug
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean arm    devel debug
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_32 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_64 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean arm    devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_32
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_64
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean arm
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean gcc

.PHONY: install
install:
	cp -R bin/* $(NATIVECOLORS_ROOT)/host/bin/
