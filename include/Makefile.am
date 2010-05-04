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
	/bin/rm -f libsmraw.h
	/bin/rm -f libsmraw/definitions.h
	/bin/rm -f libsmraw/features.h
	/bin/rm -f libsmraw/types.h
	/bin/rm -f Makefile

