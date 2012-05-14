# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := rain
DEPS     := 
SOURCES  := rain.c
MAIN     := rain.c

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
	@mkdir -p $(NATIVECOLORS_ROOT)/srv/www/curses_rain
	cp *.nexe $(NATIVECOLORS_ROOT)/srv/www/curses_rain
	cp *.html $(NATIVECOLORS_ROOT)/srv/www/curses_rain
	cp *.nmf $(NATIVECOLORS_ROOT)/srv/www/curses_rain
