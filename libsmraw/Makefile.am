AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBMFCACHE_CPPFLAGS@ \
	@LIBMFDATA_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

lib_LTLIBRARIES = libsmraw.la

libsmraw_la_SOURCES = \
	libsmraw.c \
	libsmraw_codepage.h \
	libsmraw_definitions.h \
	libsmraw_error.c libsmraw_error.h \
	libsmraw_extern.h \
	libsmraw_filename.c libsmraw_filename.h \
	libsmraw_handle.c libsmraw_handle.h \
	libsmraw_information_file.c libsmraw_information_file.h \
	libsmraw_legacy.c libsmraw_legacy.h \
	libsmraw_libbfio.h \
	libsmraw_libcerror.h \
	libsmraw_libclocale.h \
	libsmraw_libcnotify.h \
	libsmraw_libcstring.h \
	libsmraw_libfvalue.h \
	libsmraw_libmfcache.h \
	libsmraw_libmfdata.h \
	libsmraw_libuna.h \
	libsmraw_metadata.c libsmraw_metadata.h \
	libsmraw_notify.c libsmraw_notify.h \
	libsmraw_support.c libsmraw_support.h \
	libsmraw_types.h \
	libsmraw_unused.h

libsmraw_la_LIBADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBMFCACHE_LIBADD@ \
	@LIBMFDATA_LIBADD@

libsmraw_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libsmraw_definitions.h.in \
	libsmraw.rc \
	libsmraw.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libsmraw_definitions.h
	/bin/rm -f libsmraw.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libsmraw ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libsmraw_la_SOURCES) 

