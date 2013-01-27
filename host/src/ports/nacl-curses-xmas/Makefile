# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := nacl-curses-xmas
DEPS     := 
SOURCES  := $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/demos/xmas.c 

export PROJECT
export SOURCES
export DEPS

export LIB_CURSES=1
export LIB_NATIVEYELLOW=1

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

.PHONY: clean
clean:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean arm    
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_32 
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_64 
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean arm    devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_32 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_64 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean arm    devel debug
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_32 devel debug
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeNexe.mak clean x86_64 devel debug


.PHONY: install
install:
	cp -R bin/* $(NATIVECOLORS_ROOT)/host/bin/
