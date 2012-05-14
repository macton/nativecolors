# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libcurses
DEPS     := 
SOURCES  := $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/addch.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/addchstr.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/addstr.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/attr.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/beep.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/bkgd.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/border.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/clear.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/color.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/debug.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/delch.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/deleteln.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/deprec.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/getch.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/getstr.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/getyx.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/inch.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/inchstr.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/initscr.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/inopts.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/insch.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/insstr.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/instr.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/kernel.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/keyname.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/mouse.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/move.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/outopts.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/overlay.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/pad.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/panel.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/printw.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/refresh.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/scanw.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/scr_dump.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/scroll.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/slk.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/termattr.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/terminfo.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/touch.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/util.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/pdcurses/window.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/ecma-48/pdcclip.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/ecma-48/pdcdisp.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/ecma-48/pdcgetsc.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/ecma-48/pdckbd.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/ecma-48/pdcscrn.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/ecma-48/pdcsetsc.c \
            $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/ecma-48/pdcutil.c

export PROJECT
export SOURCES
export DEPS

all: publish devel devel-debug gcc

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

install:
	cp *.a $(NATIVECOLORS_ROOT)/usr/lib/
	cp $(NATIVECOLORS_ROOT)/usr/packages/pdcurses/curses.h $(NATIVECOLORS_ROOT)/usr/include/

.PHONY: clean
clean:
	make -f MakeLib.mak clean x86_32 
	make -f MakeLib.mak clean x86_64 
	make -f MakeLib.mak clean x86_32 devel
	make -f MakeLib.mak clean x86_64 devel
	make -f MakeLib.mak clean x86_32 devel debug
	make -f MakeLib.mak clean x86_64 devel debug

