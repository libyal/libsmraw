AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libsystem \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBMFDATA_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@ \
	@LIBSMRAW_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	smrawmount

smrawmount_SOURCES = \
	smrawmount.c \
	smrawoutput.c smrawoutput.h \
	smrawtools_libsmraw.h \
	smrawtools_libuna.h \
	mount_handle.c mount_handle.h

smrawmount_LDADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libsmraw/libsmraw.la \
	../libsystem/libsystem.la \
	@LIBINTL@ \
	@LIBFUSE_LIBADD@ \
	@MEMWATCH_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on smrawmount ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(smrawmount_SOURCES) 

