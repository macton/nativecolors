
# --------------------------------------------------------------------
# Project config
# --------------------------------------------------------------------

WARNINGS := -Wno-long-long -Wall -pedantic
CFLAGS   := -std=c99 $(WARNINGS)
ARFLAGS  := rcs
OSNAME   := $(shell python $(NACL_SDK_ROOT)/tools/getos.py)

CFLAGS   += -Xlinker --wrap -Xlinker write
CFLAGS   += -I..

# --------------------------------------------------------------------
# glibc (newlib if not specified)
# --------------------------------------------------------------------

ifeq (glibc,$(filter glibc,$(MAKECMDGOALS)))
PROJECT  := $(PROJECT)-glibc
OBJEXT   := glibc
LIBCNAME := glibc
else
PROJECT  := $(PROJECT)-newlib
OBJEXT   := newlib
LIBCNAME := newlib
endif

# --------------------------------------------------------------------
# x86_32
# --------------------------------------------------------------------


ifeq (x86_32,$(filter x86_32,$(MAKECMDGOALS)))
PROJECT  := $(PROJECT)-x86-32
OBJEXT   := $(OBJEXT)-x86-32
endif

# --------------------------------------------------------------------
# x86_64
# --------------------------------------------------------------------


ifeq (x86_64,$(filter x86_64,$(MAKECMDGOALS)))
PROJECT  := $(PROJECT)-x86-64
OBJEXT   := $(OBJEXT)-x86-64
endif

# --------------------------------------------------------------------
# devel
# --------------------------------------------------------------------


ifeq (devel,$(filter devel,$(MAKECMDGOALS)))
CFLAGS   += -DDEVEL
OBJEXT   := $(OBJEXT)-devel
PROJECT  := $(PROJECT)-devel
endif

# --------------------------------------------------------------------
# debug (release if not specified)
# --------------------------------------------------------------------

ifeq (debug,$(filter debug,$(MAKECMDGOALS)))
CFLAGS   += -O0 -g
OBJEXT   := $(OBJEXT)-debug
PROJECT  := $(PROJECT)-debug
else
CFLAGS   += -O2
endif

# --------------------------------------------------------------------
# MAKECMDGOALS targets
# --------------------------------------------------------------------

.PHONY: x86_32
x86_32: $(PROJECT).a

.PHONY: x86_64
x86_64: $(PROJECT).a

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

$(PROJECT).a : $(OBJS)
	$(LINK) rcs $@ $^

%.$(OBJEXT).o : %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f *.a
