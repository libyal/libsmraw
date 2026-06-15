AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
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
	@LIBSMRAW_DLL_IMPORT@

check_PYTHONdir = $(abs_top_builddir)

AM_TESTS_ENVIRONMENT = \
	export TESTS_USE_WINAPI="@TESTS_USE_WINAPI@";

BUILT_SOURCES = package.m4

TESTS = \
	$(check_SCRIPTS)

check_AUTOTESTS = \
	test_library \
	test_library_write \
	test_manpages \
	test_tools

if HAVE_PYTHON_TESTS
check_AUTOTESTS += test_python_module
endif

check_SCRIPTS = \
	test_glob.sh

check_PYTHON_SCRIPTS = \
	pysmraw_test_handle.py \
	pysmraw_test_support.py

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
	smraw_test_macros.h \
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
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

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
	smraw_test_getopt.c smraw_test_getopt.h \
	smraw_test_libcpath.h \
	smraw_test_libsmraw.h \
	smraw_test_unused.h \
	smraw_test_write.c

smraw_test_write_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCERROR_LIBADD@

AUTOM4TE = autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest

SUFFIXES = .at

.at:
	$(AM_V_GEN)rm -f $@ $@.tmp; \
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $< && \
	mv $@.tmp $@

atconfig: $(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status tests/$@

check-build: $(check_PROGRAMS)

check-local: $(check_AUTOTESTS)
	@fail=0; \
	for test_suite in $(check_AUTOTESTS); do \
		$(AM_TESTS_ENVIRONMENT) $(SHELL) "./$$test_suite" $(TESTSUITEFLAGS) || fail=1; \
		echo ""; \
	done; \
	exit $$fail

clean-local:
	for test_suite in $(check_AUTOTESTS); do \
		test ! -f "$$test_suite" || $(SHELL) "./$$test_suite" --clean; \
	done

package.m4: $(top_srcdir)/configure.ac
	$(AM_V_GEN):; \
	{ \
	  echo 'm4_define([AT_PACKAGE_NAME], [$(PACKAGE_NAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_TARNAME], [$(PACKAGE_TARNAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_VERSION], [$(PACKAGE_VERSION)])' ; \
	  echo 'm4_define([AT_PACKAGE_STRING], [$(PACKAGE_STRING)])' ; \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT], [$(PACKAGE_BUGREPORT)])' ; \
	} > $@

test_inputs_libsmraw.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) libsmraw

test_inputs_pysmraw.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) pysmraw

test_inputs_smrawverify.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) smrawverify

test_library: \
	package.m4 \
	test_inputs_libsmraw.at \
	test_library.at \
	test_macros.at

test_manpages: \
	package.m4 \
	test_manpages.at

test_python_module: \
	package.m4 \
	test_inputs_pysmraw.at \
	test_python_module.at

test_tools: \
	package.m4 \
	test_inputs_smrawverify.at \
	test_macros.at \
	test_tools.at

EXTRA_DIST = \
	$(check_AUTOTESTS:=.at) \
	$(check_SCRIPTS) \
	$(check_PYTHON_SCRIPTS) \
	generate_test_inputs.sh \
	package.m4 \
	test_macros.at \
	test_runner.sh

CLEANFILES = \
	$(check_AUTOTESTS) \
	*.exe \
	*.tmp \
	notify_stream.log \
	test_inputs_*.at

DISTCLEANFILES = \
	*.log \
	atconfig \
	Makefile \
	Makefile.in

