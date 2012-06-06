
# --------------------------------------------------------------------
# Project config
# --------------------------------------------------------------------

WARNINGS := -Wno-long-long -Wall
ARFLAGS  := rcs
OSNAME   := $(shell python $(NACL_SDK_ROOT)/tools/getos.py)

# CFLAGS can come fom calling makefile
CFLAGS   += -std=c99 $(WARNINGS)
CFLAGS   += -I$(NATIVECOLORS_ROOT)/client/include
CFLAGS   += -D_GNU_SOURCE

# --------------------------------------------------------------------
# glibc (newlib if not specified)
# --------------------------------------------------------------------

ifeq (glibc,$(filter glibc,$(MAKECMDGOALS)))
LIBDIR   := lib/glibc
OBJEXT   := glibc
LIBCNAME := glibc
else
LIBDIR   := lib/newlib
OBJEXT   := newlib
LIBCNAME := newlib
endif

# --------------------------------------------------------------------
# x86_32
# --------------------------------------------------------------------


ifeq (x86_32,$(filter x86_32,$(MAKECMDGOALS)))
LIBDIR  := $(LIBDIR)-x86-32
OBJEXT   := $(OBJEXT)-x86-32
CFLAGS   += -m32
endif

# --------------------------------------------------------------------
# x86_64
# --------------------------------------------------------------------


ifeq (x86_64,$(filter x86_64,$(MAKECMDGOALS)))
LIBDIR  := $(LIBDIR)-x86-64
OBJEXT   := $(OBJEXT)-x86-64
CFLAGS   += -m64
endif

# --------------------------------------------------------------------
# devel
# --------------------------------------------------------------------


ifeq (devel,$(filter devel,$(MAKECMDGOALS)))
CFLAGS   += -DDEVEL
OBJEXT   := $(OBJEXT)-devel
LIBDIR  := $(LIBDIR)-devel
endif

# --------------------------------------------------------------------
# debug (release if not specified)
# --------------------------------------------------------------------

ifeq (debug,$(filter debug,$(MAKECMDGOALS)))
CFLAGS   += -O0 -g -DDEBUG
OBJEXT   := $(OBJEXT)-debug
LIBDIR  := $(LIBDIR)-debug
# For pdcurses:
CFLAGS   += -DPDCDEBUG
else
CFLAGS   += -O2
endif

# --------------------------------------------------------------------
# MAKECMDGOALS targets
# --------------------------------------------------------------------

.PHONY: x86_32
ifneq (clean,$(filter clean,$(MAKECMDGOALS)))
x86_32: $(LIBDIR)/$(PROJECT).a
else
x86_32:
	@:
endif

.PHONY: x86_64
ifneq (clean,$(filter clean,$(MAKECMDGOALS)))
x86_64: $(LIBDIR)/$(PROJECT).a
else
x86_64:
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

OBJS     := $(patsubst %.c, %.$(OBJEXT).o, $(SOURCES))
TC_PATH  := $(abspath $(NACL_SDK_ROOT)/toolchain/$(OSNAME)_x86_$(LIBCNAME))
CC       := $(TC_PATH)/bin/i686-nacl-gcc
LINK     := $(TC_PATH)/bin/i686-nacl-ar
STRIP    := $(TC_PATH)/bin/i686-nacl-strip

$(LIBDIR)/$(PROJECT).a : $(OBJS)
	mkdir -p $(LIBDIR)
	$(LINK) rcs $@ $^

%.$(OBJEXT).o : %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(LIBDIR)/$(PROJECT).a

