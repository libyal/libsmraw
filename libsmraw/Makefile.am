AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libnotify \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

lib_LTLIBRARIES = libsmraw.la

libsmraw_la_SOURCES = \
	libsmraw_codepage.h \
	libsmraw_definitions.h \
	libsmraw_error.c libsmraw_error.h \
	libsmraw_extern.h \
	libsmraw_handle.c libsmraw_handle.h \
	libsmraw_information_file.c libsmraw_information_file.h \
	libsmraw_libbfio.h \
	libsmraw_libuna.h \
	libsmraw_list_type.c libsmraw_list_type.h \
	libsmraw_metadata.c libsmraw_metadata.h \
	libsmraw_notify.c libsmraw_notify.h \
	libsmraw_string.h \
	libsmraw_support.c libsmraw_support.h \
	libsmraw_system_string.h \
	libsmraw_types.h \
	libsmraw_values_table.c libsmraw_values_table.h

libsmraw_la_LIBADD = \
	../liberror/liberror.la \
	../libnotify/libnotify.la \
	@LIBUNA_LIBADD@ \
	@LIBBFIO_LIBADD@

libsmraw_la_LDFLAGS = -no-undefined -version-info 1:0:0

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libsmraw ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libsmraw_la_SOURCES) 

