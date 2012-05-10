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
            pdcclip.c \
            pdcdisp.c \
            pdcgetsc.c \
            pdckbd.c \
            pdcscrn.c \
            pdcsetsc.c \
            pdcutil.c

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

gcc:
	make -f MakeLib.mak gcc
	make -f MakeLib.mak gcc

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.a
	rm -f *.nexe
	rm -f ../pdcurses/*.o

