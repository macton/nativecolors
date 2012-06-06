# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libz
DEPS     := 

SOURCES  := \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/adler32.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/compress.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/crc32.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/deflate.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/gzclose.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/gzlib.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/gzread.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/gzwrite.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/infback.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/inffast.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/inflate.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/inftrees.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/trees.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/uncompr.c \
            $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/zutil.c

export PROJECT
export SOURCES
export DEPS

CFLAGS   += -DSTDC -DHAVE_UNISTD_H=1 -DHAVE_STDARG_H=1

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
	cp $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/zlib.h $(NATIVECOLORS_ROOT)/host/include/
	cp $(NATIVECOLORS_ROOT)/host/src/packages/zlib-1.2.7/zconf.h $(NATIVECOLORS_ROOT)/host/include/

.PHONY: clean
clean:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_32 
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_64 
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_32 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_64 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_32 devel debug
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_64 devel debug

