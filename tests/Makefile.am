AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/libsmraw \
	@MEMWATCH_CPPFLAGS@ \
	@LIBSMRAW_DLL_IMPORT@

TESTS = \
	test_glob.sh

check_SCRIPTS = \
	test_glob.sh

check_PROGRAMS = \
	smraw_test_glob \
	smraw_test_read \
	smraw_test_seek \
	smraw_test_write

smraw_test_glob_SOURCES = \
	smraw_test_glob.c

smraw_test_glob_LDADD = \
	../libcstring/libcstring.la \
	../libsmraw/libsmraw.la \
        @MEMWATCH_LIBADD@

smraw_test_read_SOURCES = \
	smraw_test_read.c

smraw_test_read_LDADD = \
	../libcstring/libcstring.la \
	../libsmraw/libsmraw.la \
        @MEMWATCH_LIBADD@

smraw_test_seek_SOURCES = \
	smraw_test_seek.c

smraw_test_seek_LDADD = \
	../libcstring/libcstring.la \
	../libsmraw/libsmraw.la \
        @MEMWATCH_LIBADD@

smraw_test_write_SOURCES = \
	smraw_test_write.c

smraw_test_write_LDADD = \
	../libcstring/libcstring.la \
	../libsmraw/libsmraw.la \
        @MEMWATCH_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

