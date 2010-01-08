ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
	include \
	common \
	liberror \
	libnotify \
	libuna \
	libbfio \
	libewf \
	libsystem \
	libsmio \
	smiotools \
	manuals \
	macosx

REDHAT_FILES = \
	libsmio.spec \
	libsmio.spec.in

EXTRA_DIST = \
        $(REDHAT_FILES) \
	libsmio.pc.in

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = \
	libsmio.pc

libtool: @LIBTOOL_DEPS@
	cd $(srcdir) && $(SHELL) ./config.status --recheck

lib: library

library:
	(cd $(srcdir)/common && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/liberror && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libnotify && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libuna && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libbfio && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libewf && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libsmio && $(MAKE) $(AM_MAKEFLAGS))

distclean: clean
	/bin/rm -f Makefile
	/bin/rm -f config.status
	/bin/rm -f config.cache
	/bin/rm -f config.log
	/bin/rm -f libsmio.pc
	/bin/rm -f libsmio.spec
	@for dir in ${subdirs}; do \
		(cd $$dir && $(MAKE) distclean) \
		|| case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"

splint:
	(cd $(srcdir)/liberror && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libnotify && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libuna && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libbfio && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libewf && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libsmio && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libsystem && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/smiotools && $(MAKE) splint $(AM_MAKEFLAGS))

