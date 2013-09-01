MSVSCPP_FILES = \
	libbfio/libbfio.vcproj \
	libcdata/libcdata.vcproj \
	libcerror/libcerror.vcproj \
	libcfile/libcfile.vcproj \
	libclocale/libclocale.vcproj \
	libcnotify/libcnotify.vcproj \
	libcpath/libcpath.vcproj \
	libcstring/libcstring.vcproj \
	libcsplit/libcsplit.vcproj \
	libcsystem/libcsystem.vcproj \
	libcthreads/libcthreads.vcproj \
	libfcache/libfcache.vcproj \
	libfdata/libfdata.vcproj \
	libfvalue/libfvalue.vcproj \
	libhmac/libhmac.vcproj \
	libsmraw_dll/libsmraw_dll.vcproj \
	libuna/libuna.vcproj \
	libsmraw.sln \
	smraw.net_dll/smraw.net_dll.vcproj \
	smraw_test_read/smraw_test_read.vcproj \
	smraw_test_seek/smraw_test_seek.vcproj \
	smraw_test_write/smraw_test_write.vcproj \
	smrawverify/smrawverify.vcproj

SCRIPT_FILES = \
	scripts/vs2008_x64.sh \
	scripts/vs2008_x64_sln.sed \
	scripts/vs2008_x64_vcproj.sed \
	scripts/vs2010_x64.sh \
	scripts/vs2010_x64_sln.sed \
	scripts/vs2010_x64_vcxproj.sed

EXTRA_DIST = \
	$(MSVSCPP_FILES) \
	$(SCRIPT_FILES)

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

