# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libcurses
DEPS     := 
SOURCES  := ../pdcurses/addch.c \
            ../pdcurses/addchstr.c \
            ../pdcurses/addstr.c \
            ../pdcurses/attr.c \
            ../pdcurses/beep.c \
            ../pdcurses/bkgd.c \
            ../pdcurses/border.c \
            ../pdcurses/clear.c \
            ../pdcurses/color.c \
            ../pdcurses/debug.c \
            ../pdcurses/delch.c \
            ../pdcurses/deleteln.c \
            ../pdcurses/deprec.c \
            ../pdcurses/getch.c \
            ../pdcurses/getstr.c \
            ../pdcurses/getyx.c \
            ../pdcurses/inch.c \
            ../pdcurses/inchstr.c \
            ../pdcurses/initscr.c \
            ../pdcurses/inopts.c \
            ../pdcurses/insch.c \
            ../pdcurses/insstr.c \
            ../pdcurses/instr.c \
            ../pdcurses/kernel.c \
            ../pdcurses/keyname.c \
            ../pdcurses/mouse.c \
            ../pdcurses/move.c \
            ../pdcurses/outopts.c \
            ../pdcurses/overlay.c \
            ../pdcurses/pad.c \
            ../pdcurses/panel.c \
            ../pdcurses/printw.c \
            ../pdcurses/refresh.c \
            ../pdcurses/scanw.c \
            ../pdcurses/scr_dump.c \
            ../pdcurses/scroll.c \
            ../pdcurses/slk.c \
            ../pdcurses/termattr.c \
            ../pdcurses/terminfo.c \
            ../pdcurses/touch.c \
            ../pdcurses/util.c \
            ../pdcurses/window.c \
            ../ecma-48/pdcclip.c \
            ../ecma-48/pdcdisp.c \
            ../ecma-48/pdcgetsc.c \
            ../ecma-48/pdckbd.c \
            ../ecma-48/pdcscrn.c \
            ../ecma-48/pdcsetsc.c \
            ../ecma-48/pdcutil.c

export PROJECT
export SOURCES
export DEPS

all: publish devel devel-debug gcc

publish:
	make -f MakeLib.mak x86_32 
	make -f MakeLib.mak x86_64 
	make -f xmas.mak publish
	make -f rain.mak publish
	make -f worm.mak publish

devel:
	make -f MakeLib.mak x86_32 devel
	make -f MakeLib.mak x86_64 devel
	make -f xmas.mak devel
	make -f rain.mak devel
	make -f worm.mak devel

devel-debug:
	make -f MakeLib.mak x86_32 devel debug
	make -f MakeLib.mak x86_64 devel debug
	make -f xmas.mak devel-debug
	make -f rain.mak devel-debug
	make -f worm.mak devel-debug

# Just a local target for testing
gcc:
	make -f MakeLib.mak gcc
	make -f MakeLib.mak gcc
	make -f xmas.mak gcc
	make -f rain.mak gcc
	make -f worm.mak gcc

install:
	cp *.a $(NATIVECOLORS_ROOT)/lib/
	cp ../curses.h $(NATIVECOLORS_ROOT)/include/

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.a
	rm -f *.nexe
	rm -f rain-gcc
	rm -f xmas-gcc
	rm -f worm-gcc

