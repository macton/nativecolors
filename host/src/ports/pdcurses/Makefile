# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libcurses
DEPS     := 
SOURCES  := $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/addch.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/addchstr.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/addstr.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/attr.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/beep.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/bkgd.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/border.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/clear.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/color.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/debug.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/delch.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/deleteln.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/deprec.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/getch.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/getstr.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/getyx.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/inch.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/inchstr.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/initscr.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/inopts.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/insch.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/insstr.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/instr.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/kernel.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/keyname.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/mouse.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/move.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/outopts.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/overlay.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/pad.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/panel.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/printw.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/refresh.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/scanw.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/scr_dump.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/scroll.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/slk.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/termattr.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/terminfo.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/touch.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/util.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/pdcurses/window.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/ecma-48/pdcclip.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/ecma-48/pdcdisp.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/ecma-48/pdcgetsc.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/ecma-48/pdckbd.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/ecma-48/pdcscrn.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/ecma-48/pdcsetsc.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/ecma-48/pdcutil.c

export PROJECT
export SOURCES
export DEPS

CFLAGS  += -I$(NATIVECOLORS_ROOT)/host/src/packages/pdcurses
CFLAGS  += -I$(NATIVECOLORS_ROOT)/host/include
CFLAGS  += -DPDC_PLATFORM="\"nacl-host\""

export CFLAGS

all: publish devel devel-debug

publish:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak x86_32 
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak x86_64 

devel:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak x86_32 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak x86_64 devel

devel-debug:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak x86_32 devel debug
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak x86_64 devel debug

install:
	cp -R lib/* $(NATIVECOLORS_ROOT)/host/lib/
	cp $(NATIVECOLORS_ROOT)/host/src/packages/pdcurses/curses.h $(NATIVECOLORS_ROOT)/host/include/

.PHONY: clean
clean:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_32 
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_64 
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_32 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_64 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_32 devel debug
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_64 devel debug

