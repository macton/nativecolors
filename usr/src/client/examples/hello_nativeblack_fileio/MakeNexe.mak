
# --------------------------------------------------------------------
# Project config
# --------------------------------------------------------------------

WARNINGS    := -Wno-long-long -Wall
CFLAGS      := -std=c99 $(WARNINGS)
ARFLAGS     := rcs
OSNAME      := $(shell python $(NACL_SDK_ROOT)/tools/getos.py)
LDFLAGS     :=
CFLAGS      += -I$(NATIVECOLORS_ROOT)/usr/include
CFLAGS      += -D_GNU_SOURCE
NATIVEBLACK := nativeblack

# --------------------------------------------------------------------
# glibc (newlib if not specified)
# --------------------------------------------------------------------

ifeq (glibc,$(filter glibc,$(MAKECMDGOALS)))
PROJECT     := $(PROJECT)-glibc
OBJEXT      := glibc
LIBCNAME    := glibc
NATIVEBLACK := $(NATIVEBLACK)-glibc
else
ifneq (gcc,$(filter gcc,$(MAKECMDGOALS)))
PROJECT     := $(PROJECT)-newlib
OBJEXT      := newlib
LIBCNAME    := newlib
NATIVEBLACK := $(NATIVEBLACK)-newlib
endif
endif

# --------------------------------------------------------------------
# x86_32
# --------------------------------------------------------------------


ifeq (x86_32,$(filter x86_32,$(MAKECMDGOALS)))
PROJECT     := $(PROJECT)-x86-32
OBJEXT      := $(OBJEXT)-x86-32
CFLAGS      += -m32
NATIVEBLACK := $(NATIVEBLACK)-x86-32
endif

# --------------------------------------------------------------------
# x86_64
# --------------------------------------------------------------------


ifeq (x86_64,$(filter x86_64,$(MAKECMDGOALS)))
PROJECT     := $(PROJECT)-x86-64
OBJEXT      := $(OBJEXT)-x86-64
CFLAGS      += -m64
NATIVEBLACK := $(NATIVEBLACK)-x86-64
endif

# --------------------------------------------------------------------
# gcc (local for testing)
# --------------------------------------------------------------------


ifeq (gcc,$(filter gcc,$(MAKECMDGOALS)))
PROJECT  := $(PROJECT)-gcc
OBJEXT   := gcc
endif

# --------------------------------------------------------------------
# devel
# --------------------------------------------------------------------


ifeq (devel,$(filter devel,$(MAKECMDGOALS)))
CFLAGS      += -DDEVEL
OBJEXT      := $(OBJEXT)-devel
PROJECT     := $(PROJECT)-devel
NATIVEBLACK := $(NATIVEBLACK)-devel
endif

# --------------------------------------------------------------------
# debug (release if not specified)
# --------------------------------------------------------------------

ifeq (debug,$(filter debug,$(MAKECMDGOALS)))
CFLAGS      += -O0 -g
OBJEXT      := $(OBJEXT)-debug
PROJECT     := $(PROJECT)-debug
NATIVEBLACK := $(NATIVEBLACK)-debug
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
LDFLAGS  += -L$(NATIVECOLORS_ROOT)/usr/lib
LDFLAGS  += -l$(NATIVEBLACK)
LDFLAGS  += -lppapi_gles2
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

