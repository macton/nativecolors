# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libnativeblue
DEPS     := nativeblue.h nativeblue_private.h
SOURCES  := close.c \
            cwd.c \
            creat.c \
            fd.c \
            filesys.c \
            fsync.c \
            ioctl.c \
            lseek.c \
            nativeblue.c \
            open.c \
            pow.c \
            pread.c \
            pthread_rwlock.c \
            pwrite.c \
            read.c \
            readv.c \
            sleep.c \
            urlencode.c \
            write.c \
            writev.c \
            mkdir.c  \
            commandqueue.c  \
            tty.c  \
            stat.c  

export PROJECT
export SOURCES
export DEPS

all: publish devel devel-debug

publish:
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak x86_32 
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak x86_64 

devel:
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak x86_32 devel
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak x86_64 devel

devel-debug:
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak x86_32 devel debug
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak x86_64 devel debug

.PHONY: clean
clean:
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_32 devel debug
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_64 devel debug
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_32 devel
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_64 devel
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_32
	make -f $(NATIVECOLORS_ROOT)/client/src/common/MakeLib.mak clean x86_64

install:
	cp -R lib/* $(NATIVECOLORS_ROOT)/client/lib/
	cp nativeblue.h $(NATIVECOLORS_ROOT)/client/include/
	@mkdir -p $(NATIVECOLORS_ROOT)/client/srv/www/nativeblue
	cp nativeblueterm.js $(NATIVECOLORS_ROOT)/client/srv/www/nativeblue
