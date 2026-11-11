if HAVE_LIB_FUZZING_ENGINE
AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@

bin_PROGRAMS = \
	handle_fuzzer \
	information_value_fuzzer \
	integrity_hash_value_fuzzer \
	media_fuzzer

handle_fuzzer_SOURCES = \
	handle_fuzzer.cc \
	ossfuzz_libbfio.h \
	ossfuzz_libsmraw.h

handle_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

information_value_fuzzer_SOURCES = \
	information_value_fuzzer.cc \
	ossfuzz_libbfio.h \
	ossfuzz_libsmraw.h

information_value_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

integrity_hash_value_fuzzer_SOURCES = \
	integrity_hash_value_fuzzer.cc \
	ossfuzz_libbfio.h \
	ossfuzz_libsmraw.h

integrity_hash_value_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

media_fuzzer_SOURCES = \
	media_fuzzer.cc \
	ossfuzz_libbfio.h \
	ossfuzz_libsmraw.h

media_fuzzer_LDADD = \
	@LIB_FUZZING_ENGINE@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@
endif

DISTCLEANFILES = \
	Makefile \
	Makefile.in

splint-local:
	@echo "Running splint on handle_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(handle_fuzzer_SOURCES)
	@echo "Running splint on information_value_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(information_value_fuzzer_SOURCES)
	@echo "Running splint on integrity_hash_value_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(integrity_hash_value_fuzzer_SOURCES)
	@echo "Running splint on media_fuzzer ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(media_fuzzer_SOURCES)
