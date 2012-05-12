
# --------------------------------------------------------------------
# Project config
# --------------------------------------------------------------------

WARNINGS := -Wno-long-long -Wall -pedantic
CFLAGS   := -std=c99 $(WARNINGS)
ARFLAGS  := rcs
OSNAME   := $(shell python $(NACL_SDK_ROOT)/tools/getos.py)
LDFLAGS  :=
ifneq (gcc,$(filter gcc,$(MAKECMDGOALS)))
LDFLAGS  += -lppapi_gles2 -lppapi
CFLAGS   += -Xlinker --wrap -Xlinker write
endif

CFLAGS   += -D_GNU_SOURCE
CFLAGS   += -I.. -I../..
CURSES   := curses

# --------------------------------------------------------------------
# glibc (newlib if not specified)
# --------------------------------------------------------------------

ifeq (glibc,$(filter glibc,$(MAKECMDGOALS)))
PROJECT  := $(PROJECT)-glibc
OBJEXT   := glibc
LIBCNAME := glibc
CURSES   := $(CURSES)-glibc
else
ifneq (gcc,$(filter gcc,$(MAKECMDGOALS)))
PROJECT  := $(PROJECT)-newlib
OBJEXT   := newlib
LIBCNAME := newlib
CURSES   := $(CURSES)-newlib
endif
endif

# --------------------------------------------------------------------
# x86_32
# --------------------------------------------------------------------


ifeq (x86_32,$(filter x86_32,$(MAKECMDGOALS)))
PROJECT  := $(PROJECT)-x86-32
OBJEXT   := $(OBJEXT)-x86-32
CFLAGS   += -m32
CURSES   := $(CURSES)-x86-32
endif

# --------------------------------------------------------------------
# x86_64
# --------------------------------------------------------------------


ifeq (x86_64,$(filter x86_64,$(MAKECMDGOALS)))
PROJECT  := $(PROJECT)-x86-64
OBJEXT   := $(OBJEXT)-x86-64
CFLAGS   += -m64
CURSES   := $(CURSES)-x86-64
endif

# --------------------------------------------------------------------
# gcc (local for testing)
# --------------------------------------------------------------------


ifeq (gcc,$(filter gcc,$(MAKECMDGOALS)))
PROJECT  := $(PROJECT)-gcc
OBJEXT   := gcc
CURSES   := curses-gcc
endif

# --------------------------------------------------------------------
# devel
# --------------------------------------------------------------------


ifeq (devel,$(filter devel,$(MAKECMDGOALS)))
CFLAGS   += -DDEVEL
OBJEXT   := $(OBJEXT)-devel
PROJECT  := $(PROJECT)-devel
CURSES   := $(CURSES)-devel
endif

# --------------------------------------------------------------------
# debug (release if not specified)
# --------------------------------------------------------------------

ifeq (debug,$(filter debug,$(MAKECMDGOALS)))
CFLAGS   += -O0 -g
OBJEXT   := $(OBJEXT)-debug
PROJECT  := $(PROJECT)-debug
CURSES   := $(CURSES)-debug
else
CFLAGS   += -O2
endif



# --------------------------------------------------------------------
# MAKECMDGOALS targets
# --------------------------------------------------------------------

.PHONY: x86_32
x86_32: $(PROJECT).nexe

.PHONY: x86_64
x86_64: $(PROJECT).nexe

.PHONY: gcc
gcc: $(PROJECT)

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

LDFLAGS  += -L. -l$(CURSES)

OBJS     := $(patsubst %.c, %.$(OBJEXT).o, $(SOURCES))
TC_PATH  := $(abspath $(NACL_SDK_ROOT)/toolchain/$(OSNAME)_x86_$(LIBCNAME))
CC       := $(TC_PATH)/bin/i686-nacl-gcc
LINK     := $(TC_PATH)/bin/i686-nacl-g++
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
	rm -f *.nexe

# --------------------------------------------------------------------
# Special rules
# --------------------------------------------------------------------

ifneq (gcc,$(filter gcc,$(MAKECMDGOALS)))
../demos/xmas.$(OBJEXT).o : ../demos/xmas.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@.main.o
	$(OBJCOPY) $@.main.o $@ --redefine-sym main=nablue_main

../demos/worm.$(OBJEXT).o : ../demos/worm.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@.main.o
	$(OBJCOPY) $@.main.o $@ --redefine-sym main=nablue_main

../demos/rain.$(OBJEXT).o : ../demos/rain.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@.main.o
	$(OBJCOPY) $@.main.o $@ --redefine-sym main=nablue_main
endif
