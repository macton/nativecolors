# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := echo
DEPS     := 
SOURCES  := $(NATIVECOLORS_ROOT)/usr/packages/echo/echo.c writev.c

export PROJECT
export SOURCES
export DEPS

all: publish devel devel-debug

publish:
	make -f MakeNexe.mak x86_32 
	make -f MakeNexe.mak x86_64 

devel:
	make -f MakeNexe.mak x86_32 devel
	make -f MakeNexe.mak x86_64 devel

devel-debug:
	make -f MakeNexe.mak x86_32 devel debug
	make -f MakeNexe.mak x86_64 devel debug

gcc:
	make -f MakeNexe.mak gcc
	make -f MakeNexe.mak gcc

.PHONY: clean
clean:
	make -f MakeNexe.mak clean x86_32 devel debug
	make -f MakeNexe.mak clean x86_64 devel debug
	make -f MakeNexe.mak clean x86_32 devel
	make -f MakeNexe.mak clean x86_64 devel
	make -f MakeNexe.mak clean x86_32
	make -f MakeNexe.mak clean x86_64
	make -f MakeNexe.mak clean gcc
	make -f MakeNexe.mak clean gcc

.PHONY: install
install:
	# Assume host is x86 and can run x86_32
	cp echo-newlib-x86-32.nexe $(NATIVECOLORS_ROOT)/usr/bin/echo.nexe
