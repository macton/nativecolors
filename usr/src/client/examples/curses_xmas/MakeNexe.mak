
# --------------------------------------------------------------------
# Project config
# --------------------------------------------------------------------

WARNINGS    := -Wno-long-long -Wall -pedantic
CFLAGS      := -std=c99 $(WARNINGS)
ARFLAGS     := rcs
OSNAME      := $(shell python $(NACL_SDK_ROOT)/tools/getos.py)
LDFLAGS     :=
ifneq (gcc,$(filter gcc,$(MAKECMDGOALS)))
CFLAGS      += -Xlinker --wrap -Xlinker write
CFLAGS      += -Xlinker --wrap -Xlinker read
CFLAGS      += -Xlinker --wrap -Xlinker open
CFLAGS      += -Xlinker --wrap -Xlinker lseek
CFLAGS      += -Xlinker --wrap -Xlinker isatty
endif

CFLAGS      += -I$(NATIVECOLORS_ROOT)/usr/include
CFLAGS      += -D_GNU_SOURCE
NATIVEBLUE  := nativeblue
NATIVEBLACK := nativeblack
CURSES      := curses
CJSON       := cjson

# --------------------------------------------------------------------
# glibc (newlib if not specified)
# --------------------------------------------------------------------

ifeq (glibc,$(filter glibc,$(MAKECMDGOALS)))
PROJECT     := $(PROJECT)-glibc
OBJEXT      := glibc
LIBCNAME    := glibc
NATIVEBLUE  := $(NATIVEBLUE)-glibc
NATIVEBLACK := $(NATIVEBLACK)-glibc
CURSES      := $(CURSES)-glibc
CJSON       := $(CJSON)-glibc
else
ifneq (gcc,$(filter gcc,$(MAKECMDGOALS)))
PROJECT     := $(PROJECT)-newlib
OBJEXT      := newlib
LIBCNAME    := newlib
NATIVEBLUE  := $(NATIVEBLUE)-newlib
NATIVEBLACK := $(NATIVEBLACK)-newlib
CURSES      := $(CURSES)-newlib
CJSON       := $(CJSON)-newlib
endif
endif

# --------------------------------------------------------------------
# x86_32
# --------------------------------------------------------------------


ifeq (x86_32,$(filter x86_32,$(MAKECMDGOALS)))
PROJECT     := $(PROJECT)-x86-32
OBJEXT      := $(OBJEXT)-x86-32
CFLAGS      += -m32
NATIVEBLUE  := $(NATIVEBLUE)-x86-32
NATIVEBLACK := $(NATIVEBLACK)-x86-32
CURSES      := $(CURSES)-x86-32
CJSON       := $(CJSON)-x86-32
endif

# --------------------------------------------------------------------
# x86_64
# --------------------------------------------------------------------


ifeq (x86_64,$(filter x86_64,$(MAKECMDGOALS)))
PROJECT     := $(PROJECT)-x86-64
OBJEXT      := $(OBJEXT)-x86-64
CFLAGS      += -m64
NATIVEBLUE  := $(NATIVEBLUE)-x86-64
NATIVEBLACK := $(NATIVEBLACK)-x86-64
CURSES      := $(CURSES)-x86-64
CJSON       := $(CJSON)-x86-64
endif

# --------------------------------------------------------------------
# gcc (local for testing)
# --------------------------------------------------------------------


ifeq (gcc,$(filter gcc,$(MAKECMDGOALS)))
PROJECT  := $(PROJECT)-gcc
OBJEXT   := gcc
CURSES   := $(CURSES)-gcc
CJSON    := $(CJSON)-gcc
endif

# --------------------------------------------------------------------
# devel
# --------------------------------------------------------------------


ifeq (devel,$(filter devel,$(MAKECMDGOALS)))
CFLAGS      += -DDEVEL
OBJEXT      := $(OBJEXT)-devel
PROJECT     := $(PROJECT)-devel
NATIVEBLUE  := $(NATIVEBLUE)-devel
NATIVEBLACK := $(NATIVEBLACK)-devel
CURSES      := $(CURSES)-devel
CJSON       := $(CJSON)-devel
endif

# --------------------------------------------------------------------
# debug (release if not specified)
# --------------------------------------------------------------------

ifeq (debug,$(filter debug,$(MAKECMDGOALS)))
CFLAGS      += -O0 -g
OBJEXT      := $(OBJEXT)-debug
PROJECT     := $(PROJECT)-debug
NATIVEBLUE  := $(NATIVEBLUE)-debug
NATIVEBLACK := $(NATIVEBLACK)-debug
CURSES      := $(CURSES)-debug
CJSON       := $(CJSON)-debug
else
CFLAGS   += -O2
endif


# --------------------------------------------------------------------
# MAKECMDGOALS targets
# --------------------------------------------------------------------

.PHONY: x86_32
ifneq (clean,$(filter clean,$(MAKECMDGOALS)))
x86_32: $(PROJECT).nexe 
else
x86_32:
	@:
endif

.PHONY: x86_64
ifneq (clean,$(filter clean,$(MAKECMDGOALS)))
x86_64: $(PROJECT).nexe
else
x86_64:
	@:
endif

.PHONY: gcc
ifneq (clean,$(filter clean,$(MAKECMDGOALS)))
gcc: $(PROJECT)
else
gcc:
	@:
endif

.PHONY: devel
devel:
	@:

.PHONY: debug
debug:
	@:

.PHONY: glibc 
glibc:
	@:

# --------------------------------------------------------------------
# Common (create lib)
# --------------------------------------------------------------------

ifneq (gcc,$(filter gcc,$(MAKECMDGOALS)))
# Seems to be randomly picky about lib include order. 
LDFLAGS  += -lppapi
# Make sure these symbols get pulled in from the library
LDFLAGS  += -Wl,--undefined
LDFLAGS  += -Wl,__wrap_lseek
LDFLAGS  += -Wl,--undefined
LDFLAGS  += -Wl,__wrap_write
LDFLAGS  += -Wl,--undefined
LDFLAGS  += -Wl,__wrap_open
LDFLAGS  += -Wl,--undefined
LDFLAGS  += -Wl,__wrap_close
LDFLAGS  += -Wl,--undefined
LDFLAGS  += -Wl,__wrap_read
LDFLAGS  += -Wl,--undefined
LDFLAGS  += -Wl,__wrap_isatty
LDFLAGS  += -L$(NATIVECOLORS_ROOT)/usr/lib
LDFLAGS  += -l$(CURSES)
LDFLAGS  += -l$(NATIVEBLACK)
LDFLAGS  += -l$(NATIVEBLUE)
LDFLAGS  += -l$(CJSON)
LDFLAGS  += -lppapi_gles2
LDFLAGS  += -lm
else
LDFLAGS  += -L$(NATIVECOLORS_ROOT)/usr/lib
LDFLAGS  += -l$(CURSES)
endif

MAINOBJ  := $(patsubst %.c, %.$(OBJEXT).o, $(MAIN))
OBJS     := $(patsubst %.c, %.$(OBJEXT).o, $(SOURCES))
TC_PATH  := $(abspath $(NACL_SDK_ROOT)/toolchain/$(OSNAME)_x86_$(LIBCNAME))
CC       := $(TC_PATH)/bin/i686-nacl-gcc
LINK     := $(TC_PATH)/bin/i686-nacl-gcc
STRIP    := $(TC_PATH)/bin/i686-nacl-strip
OBJCOPY  := $(TC_PATH)/bin/i686-nacl-objcopy

ifeq (gcc,$(filter gcc,$(MAKECMDGOALS)))
CC      := gcc
LINK    := gcc
STRING  := strip
OBJCOPY := objcopy
endif

$(PROJECT).nexe : $(OBJS)
	$(LINK) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(PROJECT) : $(OBJS) 
	$(LINK) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.$(OBJEXT).o : %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(PROJECT).nexe

# --------------------------------------------------------------------
# Special rules
# --------------------------------------------------------------------

ifneq (gcc,$(filter gcc,$(MAKECMDGOALS)))
xmas.$(OBJEXT).o : xmas.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@.main.o
	$(OBJCOPY) $@.main.o $@ --redefine-sym main=nablue_main
	rm -f $@.main.o
endif
