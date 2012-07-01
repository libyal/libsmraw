AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	-I$(top_srcdir)/libsmraw \
	@LIBHMAC_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@ \
	@LIBSMRAW_DLL_IMPORT@

TESTS = \
	test_glob.sh \
	test_seek.sh \
	test_read.sh \
	test_write.sh \
	test_compare.sh

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
	digest_hash.c digest_hash.h \
	smraw_test_libcerror.h \
	smraw_test_libcstring.h \
	smraw_test_libhmac.h \
	smraw_test_libsmraw.h \
	smraw_test_compare.c

smraw_test_compare_LDADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
        @MEMWATCH_LIBADD@

smraw_test_glob_SOURCES = \
	smraw_test_libcstring.h \
	smraw_test_libsmraw.h \
	smraw_test_glob.c

smraw_test_glob_LDADD = \
	@LIBCSTRING_LIBADD@ \
	../libsmraw/libsmraw.la \
        @MEMWATCH_LIBADD@

smraw_test_read_SOURCES = \
	smraw_test_libcerror.h \
	smraw_test_libcstring.h \
	smraw_test_libsmraw.h \
	smraw_test_read.c

smraw_test_read_LDADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libsmraw/libsmraw.la \
        @MEMWATCH_LIBADD@

smraw_test_read_loop_SOURCES = \
	smraw_test_libcstring.h \
	smraw_test_libsmraw.h \
	smraw_test_read_loop.c

smraw_test_read_loop_LDADD = \
	@LIBCSTRING_LIBADD@ \
	../libsmraw/libsmraw.la \
        @MEMWATCH_LIBADD@

smraw_test_seek_SOURCES = \
	smraw_test_libcstring.h \
	smraw_test_libsmraw.h \
	smraw_test_seek.c

smraw_test_seek_LDADD = \
	@LIBCSTRING_LIBADD@ \
	../libsmraw/libsmraw.la \
        @MEMWATCH_LIBADD@

smraw_test_write_SOURCES = \
	smraw_test_libcstring.h \
	smraw_test_libhmac.h \
	smraw_test_libsmraw.h \
	smraw_test_write.c

smraw_test_write_LDADD = \
	@LIBCSTRING_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
        @MEMWATCH_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

