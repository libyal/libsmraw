AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@LIBSMRAW_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	smrawmount \
	smrawverify

smrawmount_SOURCES = \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h \
	smrawmount.c \
	smrawtools_find.h \
	smrawtools_getopt.c smrawtools_getopt.h \
	smrawtools_glob.c smrawtools_glob.h \
	smrawtools_i18n.h \
	smrawtools_libbfio.h \
	smrawtools_libcdata.h \
	smrawtools_libcerror.h \
	smrawtools_libclocale.h \
	smrawtools_libcnotify.h \
	smrawtools_libcpath.h \
	smrawtools_libsmraw.h \
	smrawtools_output.c smrawtools_output.h \
	smrawtools_signal.c smrawtools_signal.h \
	smrawtools_unused.h

smrawmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

smrawverify_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	log_handle.c log_handle.h \
	process_status.c process_status.h \
	smrawtools_find.h \
	smrawtools_getopt.c smrawtools_getopt.h \
	smrawtools_glob.c smrawtools_glob.h \
	smrawtools_i18n.h \
	smrawtools_libbfio.h \
	smrawtools_libcerror.h \
	smrawtools_libclocale.h \
	smrawtools_libcnotify.h \
	smrawtools_libcsplit.h \
	smrawtools_libfvalue.h \
	smrawtools_libhmac.h \
	smrawtools_libsmraw.h \
	smrawtools_libuna.h \
	smrawtools_system_split_string.h \
	smrawtools_output.c smrawtools_output.h \
	smrawtools_signal.c smrawtools_signal.h \
	smrawtools_unused.h \
	smrawverify.c \
	storage_media_buffer.c storage_media_buffer.h \
	verification_handle.c verification_handle.h

smrawverify_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on smrawmount ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(smrawmount_SOURCES)
	@echo "Running splint on smrawverify ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(smrawverify_SOURCES)

