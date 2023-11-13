include_HEADERS = \
	libsmraw.h

pkginclude_HEADERS = \
	libsmraw/codepage.h \
	libsmraw/definitions.h \
	libsmraw/error.h \
	libsmraw/extern.h \
	libsmraw/features.h \
	libsmraw/types.h

EXTRA_DIST = \
	libsmraw.h.in \
	libsmraw/definitions.h.in \
	libsmraw/features.h.in \
	libsmraw/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libsmraw.h
	-rm -f libsmraw/definitions.h
	-rm -f libsmraw/features.h
	-rm -f libsmraw/types.h
	-rm -f Makefile

