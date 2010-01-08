AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/common -I$(top_srcdir)/liberror -I$(top_srcdir)/libsystem @LIBUNA_CPPFLAGS@ @LIBBFIO_CPPFLAGS@ @LIBEWF_CPPFLAGS@ @MEMWATCH_CPPFLAGS@
AM_LDFLAGS  = @STATIC_LDFLAGS@

bin_PROGRAMS = smioexport smioimage smioinfo smioverify

smioexport_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	export_handle.c export_handle.h \
	md5.h \
	platform.c platform.h \
	process_status.c process_status.h \
	sha1.h \
	smiocommon.c smiocommon.h \
	smioexport.c \
	smioinput.c smioinput.h \
	smiooutput.c smiooutput.h

smioexport_LDADD  = ../liberror/liberror.la ../libsmio/libsmio.la ../libsystem/libsystem.la @MEMWATCH_LIBADD@

smioimage_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	imaging_handle.c imaging_handle.h \
	md5.h \
	platform.c platform.h \
	process_status.c process_status.h \
	sha1.h \
	smioimage.c \
	smiocommon.c smiocommon.h \
	smioinput.c smioinput.h \
	smiooutput.c smiooutput.h

smioimage_LDADD  = ../liberror/liberror.la ../libsmio/libsmio.la ../libsystem/libsystem.la @MEMWATCH_LIBADD@

smioinfo_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_hash.c digest_hash.h \
	guid.c guid.h \
	info_handle.c info_handle.h \
	smioinfo.c \
	smioinput.c smioinput.h \
	smiooutput.c smiooutput.h

smioinfo_LDADD  = ../liberror/liberror.la ../libsmio/libsmio.la ../libsystem/libsystem.la @MEMWATCH_LIBADD@

smioverify_SOURCES = \
	byte_size_string.c byte_size_string.h \
	digest_context.c digest_context.h \
	digest_hash.c digest_hash.h \
	md5.h \
	process_status.c process_status.h \
	sha1.h \
	smioinput.c smioinput.h \
	smiooutput.c smiooutput.h \
	smioverify.c \
	verification_handle.c verification_handle.h

smioverify_LDADD  = ../liberror/liberror.la ../libsmio/libsmio.la ../libsystem/libsystem.la @MEMWATCH_LIBADD@

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on smioexport ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(smioexport_SOURCES) 
	@echo "Running splint on smioimage ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(smioimage_SOURCES) 
	@echo "Running splint on smioinfo ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(smioinfo_SOURCES) 
	@echo "Running splint on smioverify ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(smioverify_SOURCES) 

