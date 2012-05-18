# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libzlib-1.2.7
DEPS     := 

SOURCES  := \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/adler32.c \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/compress.c \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/crc32.c \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/deflate.c \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/gzclose.c \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/gzlib.c \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/gzread.c \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/gzwrite.c \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/infback.c \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/inffast.c \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/inflate.c \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/inftrees.c \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/trees.c \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/uncompr.c \
            $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/zutil.c

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

# Just a local target for testing
gcc:
	make -f MakeLib.mak gcc
	make -f MakeLib.mak gcc

.PHONY: clean
clean:
	make -f MakeLib.mak clean x86_32 
	make -f MakeLib.mak clean x86_64 
	make -f MakeLib.mak clean x86_32 devel
	make -f MakeLib.mak clean x86_64 devel
	make -f MakeLib.mak clean x86_32 devel debug
	make -f MakeLib.mak clean x86_64 devel debug

install:
	cp *.a $(NATIVECOLORS_ROOT)/usr/lib/
	cp $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/zlib.h $(NATIVECOLORS_ROOT)/usr/include/
	cp $(NATIVECOLORS_ROOT)/usr/packages/zlib-1.2.7/zconf.h $(NATIVECOLORS_ROOT)/usr/include/
