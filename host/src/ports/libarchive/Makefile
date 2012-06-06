# --------------------------------------------------------------------
# Project sources
# --------------------------------------------------------------------

PROJECT  := libarchive
DEPS     := config.h

# mtree requires lstat and fstat
# $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_format_mtree.c
# $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_set_format_mtree.c

# requires more sophisticated disk access than we have in either nativeyellow (host) or nativeblue (client)
# $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_disk_entry_from_file.c 
# $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_disk_set_standard_lookup.c
# $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_disk_acl.c
# $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_disk_set_standard_lookup.c

# can read ISO 9660 but not write.
# $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_set_format_iso9660.c

# not windows specific
# $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_windows.c
# $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_disk_windows.c
# $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_disk_windows.c


SOURCES  := \
        archive_read_support_format_all.c\
        archive_read_support_format_by_code.c\
        archive_write_set_format_by_name.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_disk_posix.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_disk_posix.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_acl.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_check_magic.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_crypto.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_entry.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_entry_copy_bhfi.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_entry_copy_stat.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_entry_link_resolver.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_entry_sparse.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_entry_stat.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_entry_strmode.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_entry_xattr.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_getdate.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_match.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_options.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_pathmatch.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_ppmd7.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_rb.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_data_into_fd.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_extract.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_open_fd.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_open_file.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_open_filename.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_open_memory.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_set_options.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_filter_all.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_filter_bzip2.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_filter_compress.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_filter_gzip.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_filter_none.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_filter_program.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_filter_rpm.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_filter_uu.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_filter_xz.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_format_7zip.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_format_ar.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_format_cab.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_format_cpio.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_format_empty.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_format_lha.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_format_rar.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_format_raw.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_format_tar.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_format_xar.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_format_zip.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_read_support_format_iso9660.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_string.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_string_sprintf.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_util.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_virtual.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_add_filter_bzip2.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_add_filter.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_add_filter_compress.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_add_filter_gzip.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_add_filter_none.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_add_filter_program.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_add_filter_xz.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_open_fd.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_open_file.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_open_filename.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_open_memory.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_set_format_7zip.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_set_format_ar.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_set_format.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_set_format_cpio.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_set_format_cpio_newc.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_set_format_gnutar.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_set_format_pax.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_set_format_shar.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_set_format_ustar.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_set_format_xar.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_set_format_zip.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_write_set_options.c\
        $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/filter_fork.c

export PROJECT
export SOURCES
export DEPS

CFLAGS   += -I$(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive -I.
CFLAGS   += -I$(NATIVECOLORS_ROOT)/host/include
CFLAGS   += -D_BSD_SOURCE
CFLAGS   += -DPLATFORM_CONFIG_H="\"$(NATIVECOLORS_ROOT)/host/src/ports/libarchive/config.h\""

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

.PHONY: clean
clean:
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_32 
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_64 
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_32 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_64 devel
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_32 devel debug
	make -f $(NATIVECOLORS_ROOT)/host/src/common/MakeLib.mak clean x86_64 devel debug

install:
	cp -R lib/* $(NATIVECOLORS_ROOT)/host/lib/
	cp $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive.h $(NATIVECOLORS_ROOT)/host/include/
	cp $(NATIVECOLORS_ROOT)/host/src/packages/libarchive/libarchive/archive_entry.h $(NATIVECOLORS_ROOT)/host/include/

