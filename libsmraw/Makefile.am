AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
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
	@PTHREAD_CPPFLAGS@ \
	@LIBSMRAW_DLL_EXPORT@

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
	libsmraw_io_handle.c libsmraw_io_handle.h \
	libsmraw_libbfio.h \
	libsmraw_libcerror.h \
	libsmraw_libclocale.h \
	libsmraw_libcnotify.h \
	libsmraw_libcthreads.h \
	libsmraw_libfdata.h \
	libsmraw_libfvalue.h \
	libsmraw_libuna.h \
	libsmraw_notify.c libsmraw_notify.h \
	libsmraw_support.c libsmraw_support.h \
	libsmraw_types.h \
	libsmraw_unused.h

libsmraw_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@PTHREAD_LIBADD@

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

