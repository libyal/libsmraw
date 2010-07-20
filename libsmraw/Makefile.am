AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libnotify \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
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
	libsmraw_libfvalue.h \
	libsmraw_libuna.h \
	libsmraw_metadata.c libsmraw_metadata.h \
	libsmraw_notify.c libsmraw_notify.h \
	libsmraw_string.c libsmraw_string.h \
	libsmraw_support.c libsmraw_support.h \
	libsmraw_types.h

libsmraw_la_LIBADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libnotify/libnotify.la \
	@LIBUNA_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFVALUE_LIBADD@

libsmraw_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libsmraw_definitions.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libsmraw_definitions.h
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libsmraw ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libsmraw_la_SOURCES) 

