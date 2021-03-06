
# --------------------------------------------------------------------
# Project config
# --------------------------------------------------------------------

WARNINGS := -Wno-long-long -Wall
CFLAGS   := -std=c99 $(WARNINGS)
ARFLAGS  := rcs
OSNAME   := $(shell python $(NACL_SDK_ROOT)/tools/getos.py)
CFLAGS   += -I$(NATIVECOLORS_ROOT)/host/include
CFLAGS   += -D_GNU_SOURCE

# --------------------------------------------------------------------
# glibc (newlib if not specified)
# --------------------------------------------------------------------

ifeq (glibc,$(filter glibc,$(MAKECMDGOALS)))
LIBDIR   := lib/glibc
OBJEXT   := glibc
LIBCNAME := glibc
else
ifneq (gcc,$(filter gcc,$(MAKECMDGOALS)))
LIBDIR   := lib/newlib
OBJEXT   := newlib
LIBCNAME := newlib
endif
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
# gcc (local fake testing project)
# --------------------------------------------------------------------

ifeq (gcc,$(filter gcc,$(MAKECMDGOALS)))
LIBDIR  := $(LIBDIR)-gcc
OBJEXT   := gcc
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
# arm
# --------------------------------------------------------------------


ifeq (arm,$(filter arm,$(MAKECMDGOALS)))
LIBDIR  := $(LIBDIR)-arm
OBJEXT   := $(OBJEXT)-arm
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
CFLAGS   += -O0 -g
OBJEXT   := $(OBJEXT)-debug
LIBDIR  := $(LIBDIR)-debug
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

.PHONY: arm
ifneq (clean,$(filter clean,$(MAKECMDGOALS)))
arm: $(LIBDIR)/$(PROJECT).a
else
arm:
	@:
endif

.PHONY: gcc
ifneq (clean,$(filter clean,$(MAKECMDGOALS)))
gcc: $(LIBDIR)/$(PROJECT).a
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

OBJS     := $(patsubst %.c, %.$(OBJEXT).o, $(SOURCES))
TC_PATH  := $(abspath $(NACL_SDK_ROOT)/toolchain/$(OSNAME)_x86_$(LIBCNAME))
CC       := $(TC_PATH)/bin/i686-nacl-gcc
LINK     := $(TC_PATH)/bin/i686-nacl-ar
STRIP    := $(TC_PATH)/bin/i686-nacl-strip

ifeq (gcc,$(filter gcc,$(MAKECMDGOALS)))
TC_PATH :=
CC      := gcc
LINK    := ar
STRIP   := strip
endif

ifeq (arm,$(filter arm,$(MAKECMDGOALS)))
TC_PATH  := $(abspath $(NACL_SDK_ROOT)/toolchain/$(OSNAME)_arm_$(LIBCNAME))
CC       := $(TC_PATH)/bin/arm-nacl-gcc
LINK     := $(TC_PATH)/bin/arm-nacl-ar
STRIP    := $(TC_PATH)/bin/arm-nacl-strip
endif


$(LIBDIR)/$(PROJECT).a : $(OBJS)
	mkdir -p $(LIBDIR)
	$(LINK) rcs $@ $^

%.$(OBJEXT).o : %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -rf $(LIBDIR)

