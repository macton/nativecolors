
# --------------------------------------------------------------------
# Project config
# --------------------------------------------------------------------

WARNINGS    := -Wno-long-long -Wall
ARFLAGS     := rcs
OSNAME      := $(shell python $(NACL_SDK_ROOT)/tools/getos.py)

# LDFLAGS can come from calling makefile
LDFLAGS     += -lpthread

# CFLAGS can come from calling makefile
CFLAGS      += -std=c99 $(WARNINGS)
CFLAGS      += -I$(NATIVECOLORS_ROOT)/host/include
CFLAGS      += -D_GNU_SOURCE
CFLAGS      += -DMAKE_DATE="\"$(shell date)\""

# --------------------------------------------------------------------
# glibc (newlib if not specified)
# --------------------------------------------------------------------

ifeq (glibc,$(filter glibc,$(MAKECMDGOALS)))
TARGET      := glibc
OBJEXT      := glibc
LIBCNAME    := glibc
else
TARGET      := newlib
OBJEXT      := newlib
LIBCNAME    := newlib
endif

# --------------------------------------------------------------------
# x86_32
# --------------------------------------------------------------------


ifeq (x86_32,$(filter x86_32,$(MAKECMDGOALS)))
TARGET     := $(TARGET)-x86-32
OBJEXT      := $(OBJEXT)-x86-32
CFLAGS      += -m32
endif

# --------------------------------------------------------------------
# x86_64
# --------------------------------------------------------------------


ifeq (x86_64,$(filter x86_64,$(MAKECMDGOALS)))
TARGET     := $(TARGET)-x86-64
OBJEXT      := $(OBJEXT)-x86-64
CFLAGS      += -m64
endif

# --------------------------------------------------------------------
# devel
# --------------------------------------------------------------------


ifeq (devel,$(filter devel,$(MAKECMDGOALS)))
CFLAGS      += -DDEVEL
OBJEXT      := $(OBJEXT)-devel
TARGET     := $(TARGET)-devel
endif

# --------------------------------------------------------------------
# debug (release if not specified)
# --------------------------------------------------------------------

ifeq (debug,$(filter debug,$(MAKECMDGOALS)))
CFLAGS      += -O0 -g
OBJEXT      := $(OBJEXT)-debug
TARGET     := $(TARGET)-debug
else
CFLAGS   += -O2
endif


# --------------------------------------------------------------------
# MAKECMDGOALS targets
# --------------------------------------------------------------------

.PHONY: x86_32
ifneq (clean,$(filter clean,$(MAKECMDGOALS)))
x86_32: bin/$(TARGET)/$(PROJECT) 
else
x86_32:
	@:
endif

.PHONY: x86_64
ifneq (clean,$(filter clean,$(MAKECMDGOALS)))
x86_64: bin/$(TARGET)/$(PROJECT)
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

LDFLAGS += -L$(NATIVECOLORS_ROOT)/host/lib/$(TARGET)

ifdef LIB_Z
LDFLAGS += -lz
endif

ifdef LIB_ARCHIVE
LDFLAGS += -larchive
endif

ifdef LIB_CURSES
LDFLAGS += -lcurses
endif

ifdef LIB_NATIVEYELLOW
LDFLAGS += -lnativeyellow
endif

ifdef LIB_M
LDFLAGS += -lm
endif

MAINOBJ  := $(patsubst %.c, %.$(OBJEXT).o, $(MAIN))
OBJS     := $(patsubst %.c, %.$(OBJEXT).o, $(SOURCES))
TC_PATH  := $(abspath $(NACL_SDK_ROOT)/toolchain/$(OSNAME)_x86_$(LIBCNAME))
CC       := $(TC_PATH)/bin/i686-nacl-gcc
LINK     := $(TC_PATH)/bin/i686-nacl-gcc
STRIP    := $(TC_PATH)/bin/i686-nacl-strip
OBJCOPY  := $(TC_PATH)/bin/i686-nacl-objcopy

bin/$(TARGET)/$(PROJECT) : $(OBJS)
	@mkdir -p bin/$(TARGET)
	$(LINK) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.$(OBJEXT).o : %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -rf bin/$(TARGET)
