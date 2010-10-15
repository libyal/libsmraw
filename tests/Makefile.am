AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libsmraw \
	@MEMWATCH_CPPFLAGS@ \
	@LIBSMRAW_DLL_IMPORT@

TESTS = \
	test_glob.sh \
	test_seek.sh \
	test_read.sh \
	test_compare.sh \
	test_write.sh

check_SCRIPTS = \
	test_compare.sh \
	test_glob.sh \
	test_read.sh \
	test_seek.sh \
	test_write.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	smraw_test_compare \
	smraw_test_glob \
	smraw_test_read \
	smraw_test_read_loop \
	smraw_test_seek \
	smraw_test_write

smraw_test_compare_SOURCES = \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	md5.h \
	smraw_test_compare.c

smraw_test_compare_LDADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libsmraw/libsmraw.la \
	@LIBCRYPTO_LIBADD@ \
        @MEMWATCH_LIBADD@

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

smraw_test_read_loop_SOURCES = \
	smraw_test_read_loop.c

smraw_test_read_loop_LDADD = \
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
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	md5.h \
	smraw_test_write.c

smraw_test_write_LDADD = \
	../libcstring/libcstring.la \
	../libsmraw/libsmraw.la \
	@LIBCRYPTO_LIBADD@ \
        @MEMWATCH_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

