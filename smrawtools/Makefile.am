AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
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
	@LIBCSYSTEM_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@LIBSMRAW_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	smrawmount \
	smrawverify

smrawmount_SOURCES = \
	smrawmount.c \
	smrawoutput.c smrawoutput.h \
	smrawtools_libcerror.h \
	smrawtools_libclocale.h \
	smrawtools_libcnotify.h \
	smrawtools_libcstring.h \
	smrawtools_libcsystem.h \
	smrawtools_libsmraw.h \
	smrawtools_libuna.h \
	mount_handle.c mount_handle.h

smrawmount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

smrawverify_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	log_handle.c log_handle.h \
	process_status.c process_status.h \
	smrawverify.c \
	smrawoutput.c smrawoutput.h \
	smrawtools_libcerror.h \
	smrawtools_libclocale.h \
	smrawtools_libcnotify.h \
	smrawtools_libcsplit.h \
	smrawtools_libcstring.h \
	smrawtools_libcsystem.h \
	smrawtools_libhmac.h \
	smrawtools_libsmraw.h \
	smrawtools_libuna.h \
	storage_media_buffer.c storage_media_buffer.h \
	verification_handle.c verification_handle.h

smrawverify_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
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

