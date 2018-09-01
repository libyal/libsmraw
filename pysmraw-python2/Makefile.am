# Note that we cannot use: AUTOMAKE_OPTIONS = subdir-objects
# subdir-objects will compile the source files to a single version of Python.
# Since subdir-objects is being deprecated we copy the source files instead.
am__installdirs = "$(DESTDIR)$(pyexecdir2)"
pyexecdir = $(pyexecdir2)

if HAVE_PYTHON2
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBSMRAW_DLL_IMPORT@

am_pysmraw_la_rpath = -rpath $(pyexecdir2)

BUILT_SOURCES = \
	pysmraw.c pysmraw.h \
	pysmraw_error.c pysmraw_error.h \
	pysmraw_file_object_io_handle.c pysmraw_file_object_io_handle.h \
	pysmraw_file_objects_io_pool.c pysmraw_file_objects_io_pool.h \
	pysmraw_handle.c pysmraw_handle.h \
	pysmraw_integer.c pysmraw_integer.h \
	pysmraw_libbfio.h \
	pysmraw_libcerror.h \
	pysmraw_libclocale.h \
	pysmraw_libsmraw.h \
	pysmraw_metadata.c pysmraw_metadata.h \
	pysmraw_python.h \
	pysmraw_unused.h

pyexec_LTLIBRARIES = pysmraw.la

nodist_pysmraw_la_SOURCES = $(BUILT_SOURCES)

pysmraw_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libsmraw/libsmraw.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pysmraw_la_CPPFLAGS = $(PYTHON2_CPPFLAGS)
pysmraw_la_LDFLAGS  = -module -avoid-version $(PYTHON2_LDFLAGS)

$(BUILT_SOURCES):
	/bin/cp -f $(top_srcdir)/pysmraw/$@ $@

endif

MAINTAINERCLEANFILES = \
	Makefile.in

clean-local:
	/bin/rm -f pysmraw*.[ch]

distclean: clean
	/bin/rm -f Makefile

