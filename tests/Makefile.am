AUTOMAKE_OPTIONS = subdir-objects

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
	@LIBHMAC_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBSMRAW_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYSMRAW = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_glob.sh \
	test_write_functions.sh \
	test_tools.sh \
	test_smrawverify.sh \
	$(TESTS_PYSMRAW)

check_SCRIPTS = \
	pysmraw_test_handle.py \
	pysmraw_test_support.py \
	test_glob.sh \
	test_library.sh \
	test_manpage.sh \
	test_python_module.sh \
	test_runner.sh \
	test_smrawverify.sh \
	test_tools.sh \
	test_write_functions.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	smraw_test_error \
	smraw_test_filename \
	smraw_test_glob \
	smraw_test_handle \
	smraw_test_information_file \
	smraw_test_io_handle \
	smraw_test_notify \
	smraw_test_support \
	smraw_test_tools_output \
	smraw_test_tools_signal \
	smraw_test_write

smraw_test_error_SOURCES = \
	smraw_test_error.c \
	smraw_test_libsmraw.h \
	smraw_test_macros.h \
	smraw_test_unused.h

smraw_test_error_LDADD = \
	../libsmraw/libsmraw.la

smraw_test_filename_SOURCES = \
	smraw_test_filename.c \
	smraw_test_libcerror.h \
	smraw_test_libsmraw.h \
	smraw_test_macros.h \
	smraw_test_memory.c smraw_test_memory.h \
	smraw_test_unused.h

smraw_test_filename_LDADD = \
	../libsmraw/libsmraw.la \
	@LIBCERROR_LIBADD@

smraw_test_glob_SOURCES = \
	smraw_test_glob.c \
	smraw_test_libsmraw.h \
	smraw_test_unused.h

smraw_test_glob_LDADD = \
	../libsmraw/libsmraw.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@

smraw_test_handle_SOURCES = \
	smraw_test_functions.c smraw_test_functions.h \
	smraw_test_getopt.c smraw_test_getopt.h \
	smraw_test_handle.c \
	smraw_test_libbfio.h \
	smraw_test_libcerror.h \
	smraw_test_libclocale.h \
	smraw_test_libcnotify.h \
	smraw_test_libsmraw.h \
	smraw_test_libuna.h \
	smraw_test_macros.h \
	smraw_test_memory.c smraw_test_memory.h \
	smraw_test_unused.h

smraw_test_handle_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

smraw_test_information_file_SOURCES = \
	smraw_test_functions.c smraw_test_functions.h \
	smraw_test_information_file.c \
	smraw_test_libbfio.h \
	smraw_test_libcerror.h \
	smraw_test_libclocale.h \
	smraw_test_libsmraw.h \
	smraw_test_libuna.h \
	smraw_test_macros.h \
	smraw_test_memory.c smraw_test_memory.h \
	smraw_test_unused.h

smraw_test_information_file_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

smraw_test_io_handle_SOURCES = \
	smraw_test_io_handle.c \
	smraw_test_libcerror.h \
	smraw_test_libsmraw.h \
	smraw_test_macros.h \
	smraw_test_memory.c smraw_test_memory.h \
	smraw_test_unused.h

smraw_test_io_handle_LDADD = \
	../libsmraw/libsmraw.la \
	@LIBCERROR_LIBADD@

smraw_test_notify_SOURCES = \
	smraw_test_libcerror.h \
	smraw_test_libsmraw.h \
	smraw_test_macros.h \
	smraw_test_notify.c \
	smraw_test_unused.h

smraw_test_notify_LDADD = \
	../libsmraw/libsmraw.la \
	@LIBCERROR_LIBADD@

smraw_test_support_SOURCES = \
	smraw_test_libcerror.h \
	smraw_test_libsmraw.h \
	smraw_test_macros.h \
	smraw_test_memory.c smraw_test_memory.h \
	smraw_test_support.c \
	smraw_test_unused.h

smraw_test_support_LDADD = \
	../libsmraw/libsmraw.la \
	@LIBCERROR_LIBADD@

smraw_test_tools_output_SOURCES = \
	../smrawtools/smrawtools_output.c ../smrawtools/smrawtools_output.h \
	smraw_test_libcerror.h \
	smraw_test_macros.h \
	smraw_test_tools_output.c \
	smraw_test_unused.h

smraw_test_tools_output_LDADD = \
	../libsmraw/libsmraw.la \
	@LIBCERROR_LIBADD@

smraw_test_tools_signal_SOURCES = \
	../smrawtools/smrawtools_signal.c ../smrawtools/smrawtools_signal.h \
	smraw_test_libcerror.h \
	smraw_test_macros.h \
	smraw_test_tools_signal.c \
	smraw_test_unused.h

smraw_test_tools_signal_LDADD = \
	../libsmraw/libsmraw.la \
	@LIBCERROR_LIBADD@

smraw_test_write_SOURCES = \
	smraw_test_libcpath.h \
	smraw_test_libhmac.h \
	smraw_test_libsmraw.h \
	smraw_test_unused.h \
	smraw_test_write.c

smraw_test_write_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@
	@PTHREAD_LIBADD@
	@LIBDL_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

