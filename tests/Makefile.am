AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
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
	@LIBCSYSTEM_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBSMRAW_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYSMRAW  = \
	test_pysmraw_open_close.sh \
	test_pysmraw_seek.sh \
	test_pysmraw_read.sh
endif

TESTS = \
	test_glob.sh \
	test_open_close.sh \
	test_seek.sh \
	test_read.sh \
	test_write.sh \
	test_smrawverify.sh \
	$(TESTS_PYSMRAW)

check_SCRIPTS = \
	pysmraw_test_open_close.py \
	pysmraw_test_read.py \
	pysmraw_test_seek.py \
	test_glob.sh \
	test_open_close.sh \
	test_pysmraw_open_close.sh \
	test_pysmraw_read.sh \
	test_pysmraw_seek.sh \
	test_read.sh \
	test_runner.sh \
	test_seek.sh \
	test_smrawverify.sh \
	test_write.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	smraw_test_glob \
	smraw_test_open_close \
	smraw_test_read \
	smraw_test_read_loop \
	smraw_test_seek \
	smraw_test_write

smraw_test_glob_SOURCES = \
	smraw_test_glob.c \
	smraw_test_libcstring.h \
	smraw_test_libsmraw.h \
	smraw_test_unused.h

smraw_test_glob_LDADD = \
	../libsmraw/libsmraw.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

smraw_test_open_close_SOURCES = \
	smraw_test_libcerror.h \
	smraw_test_libcstring.h \
	smraw_test_libsmraw.h \
	smraw_test_open_close.c \
	smraw_test_unused.h

smraw_test_open_close_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

smraw_test_read_SOURCES = \
	smraw_test_libcerror.h \
	smraw_test_libcstring.h \
	smraw_test_libsmraw.h \
	smraw_test_read.c \
	smraw_test_unused.h

smraw_test_read_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

smraw_test_read_loop_SOURCES = \
	smraw_test_libcerror.h \
	smraw_test_libcstring.h \
	smraw_test_libsmraw.h \
	smraw_test_read_loop.c \
	smraw_test_unused.h

smraw_test_read_loop_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

smraw_test_seek_SOURCES = \
	smraw_test_libcstring.h \
	smraw_test_libsmraw.h \
	smraw_test_seek.c \
	smraw_test_unused.h

smraw_test_seek_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

smraw_test_write_SOURCES = \
	smraw_test_libcpath.h \
	smraw_test_libcstring.h \
	smraw_test_libhmac.h \
	smraw_test_libsmraw.h \
	smraw_test_unused.h \
	smraw_test_write.c

smraw_test_write_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@
	@LIBDL_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

