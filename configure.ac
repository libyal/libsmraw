AC_PREREQ( 2.59 )

AC_INIT(
 [libsmraw],
 [20110831],
 [jbmetz@users.sourceforge.net])

AC_CONFIG_SRCDIR(
 [include/libsmraw.h.in])

AM_INIT_AUTOMAKE([gnu 1.6])

AC_CONFIG_MACRO_DIR([m4])

dnl Check for host type
AC_CANONICAL_HOST

dnl Check for large file support
AC_SYS_LARGEFILE

dnl Check for configure arguments
COMMON_ARG_ENABLE(
 [wide-character-type],
 [wide_character_type],
 [enable wide character type support],
 [no])
COMMON_ARG_ENABLE(
 [winapi],
 [winapi],
 [enable WINAPI support for cross-compilation],
 [auto-detect])
COMMON_ARG_ENABLE(
 [verbose-output],
 [verbose_output],
 [enable verbose output],
 [no])
COMMON_ARG_ENABLE(
 [debug-output],
 [debug_output],
 [enable debug output],
 [no])

COMMON_ARG_WITH(
 [libuna],
 [libuna],
 [search for libuna in includedir and libdir or in the specified DIR, or no if to use local version],
 [auto-detect],
 [DIR])
COMMON_ARG_WITH(
 [libbfio],
 [libbfio],
 [search for libbfio in includedir and libdir or in the specified DIR, or no if to use local version],
 [auto-detect],
 [DIR])
COMMON_ARG_WITH(
 [libfvalue],
 [libfvalue],
 [search for libfvalue in includedir and libdir or in the specified DIR, or no if to use local version],
 [auto-detect],
 [DIR])
COMMON_ARG_WITH(
 [libmfdata],
 [libmfdata],
 [search for libmfdata in includedir and libdir or in the specified DIR, or no if to use local version],
 [auto-detect],
 [DIR])

dnl Check for libtool DLL support
AC_LIBTOOL_WIN32_DLL

dnl Checks for programs
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Support of internationalization (i18n)
AM_GNU_GETTEXT([external])

dnl Check for compiler language support
AC_C_CONST
AC_C_VOLATILE

dnl Automatically enable winapi support for mingw
AC_CHECK_WINAPI

dnl Check for type definitions
AC_CHECK_LOCAL_TYPES

dnl Check for headers

dnl Check for functions

dnl Check for printf conversion specifier support
AC_CHECK_FUNC_PRINTF_JD
AC_CHECK_FUNC_PRINTF_ZD

dnl Checks for libraries and library functions
dnl ac_cv_libsmraw_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_CHECK_LOCAL_COMMON
AC_CHECK_LOCAL_LIBCSTRING
AC_CHECK_LOCAL_LIBERROR
AC_CHECK_LOCAL_LIBNOTIFY

dnl Check for enabling libuna for Unicode string support
AC_CHECK_LIBUNA

AS_IF(
 [test "x$ac_cv_libuna" = xyes],
 [AC_SUBST(
  [libewf_pc_libs_private_libuna],
  [-luna])
 ])

AS_IF(
 [test "x$ac_cv_libuna" = xyes],
 [AC_SUBST(
  [libewf_spec_requires_libuna],
  [libuna])
 AC_SUBST(
  [libewf_spec_build_requires_libuna],
  [libuna-devel])
 ])

AS_IF(
 [test "x$ac_cv_libuna" != xyes],
 [AC_DEFINE(
  [HAVE_LOCAL_LIBUNA],
  [1],
  [Define to 1 if the local version of libuna is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBUNA],
  [1])
 AC_SUBST(
  [LIBUNA_CPPFLAGS],
  [-I../libuna])
 AC_SUBST(
  [LIBUNA_LIBADD],
  [../libuna/libuna.la])
 ac_cv_libuna=local
 ])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBUNA],
 [test "x$ac_cv_libuna" = xlocal])

dnl Check for enabling libbfio for basic file IO support
AC_CHECK_LIBBFIO

AS_IF(
 [test "x$ac_cv_libbfio" = xyes],
 [AC_SUBST(
  [libewf_pc_libs_private_libbfio],
  [-lbfio])
 ])

AS_IF(
 [test "x$ac_cv_libbfio" = xyes],
 [AC_SUBST(
  [libewf_spec_requires_libbfio],
  [libbfio])
 AC_SUBST(
  [libewf_spec_build_requires_libbfio],
  [libbfio-devel])
 ])

AS_IF(
 [test "x$ac_cv_libbfio" != xyes],
 [AC_DEFINE(
  [HAVE_LOCAL_LIBBFIO],
  [1],
  [Define to 1 if the local version of libbfio is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBBFIO],
  [1])
 AC_SUBST(
  [LIBBFIO_CPPFLAGS],
  [-I../libbfio])
 AC_SUBST(
  [LIBBFIO_LIBADD],
  [../libbfio/libbfio.la])
 ac_cv_libbfio=local
 ])

AS_IF(
 [test "x$ac_cv_libbfio" = xlocal],
 [AC_CHECK_LOCAL_LIBBFIO])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBBFIO],
 [test "x$ac_cv_libbfio" = xlocal])

dnl Check for enabling libmfdata for multi-file data support
AC_CHECK_LIBMFDATA

AS_IF(
 [test "x$ac_cv_libmfdata" = xyes],
 [AC_SUBST(
  [libewf_pc_libs_private_libmfdata],
  [-lmfdata])
 ])

AS_IF(
 [test "x$ac_cv_libmfdata" = xyes],
 [AC_SUBST(
  [libewf_spec_requires_libmfdata],
  [libmfdata])
 AC_SUBST(
  [libewf_spec_build_requires_libmfdata],
  [libmfdata-devel])
 ])

AS_IF(
 [test "x$ac_cv_libmfdata" != xyes],
 [AC_DEFINE(
  [HAVE_LOCAL_LIBMFDATA],
  [1],
  [Define to 1 if the local version of libmfdata is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBMFDATA],
  [1])
 AC_SUBST(
  [LIBMFDATA_CPPFLAGS],
  [-I../libmfdata])
 AC_SUBST(
  [LIBMFDATA_LIBADD],
  [../libmfdata/libmfdata.la])
 ac_cv_libmfdata=local
 ])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBMFDATA],
 [test "x$ac_cv_libmfdata" = xlocal])

dnl Check for enabling libfvalue for file value support
AC_CHECK_LIBFVALUE

AS_IF(
 [test "x$ac_cv_libfvalue" = xyes],
 [AC_SUBST(
  [libewf_pc_libs_private_libfvalue],
  [-lfvalue])
 ])

AS_IF(
 [test "x$ac_cv_libfvalue" = xyes],
 [AC_SUBST(
  [libewf_spec_requires_libfvalue],
  [libfvalue])
 AC_SUBST(
  [libewf_spec_build_requires_libfvalue],
  [libfvalue-devel])
 ])

AS_IF(
 [test "x$ac_cv_libfvalue" != xyes],
 [AC_DEFINE(
  [HAVE_LOCAL_LIBFVALUE],
  [1],
  [Define to 1 if the local version of libfvalue is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBFVALUE],
  [1])
 AC_SUBST(
  [LIBFVALUE_CPPFLAGS],
  [-I../libfvalue])
 AC_SUBST(
  [LIBFVALUE_LIBADD],
  [../libfvalue/libfvalue.la])
 ac_cv_libfvalue=local
 ])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBFVALUE],
 [test "x$ac_cv_libfvalue" = xlocal])

dnl TODO use libhmac
dnl Check for enabling crypographic digest hash support
ac_cv_libcrypto=no

AS_IF(
 [test "x$ac_cv_enable_winapi" = xyes],
 [AC_SUBST(
  [LIBCRYPTO_LIBADD],
  [-ladvapi32])
  ac_cv_libcrypto=libadvapi32])

dnl Check for libcrypto openssl EVP support
AS_IF(
 [test "x$ac_cv_libcrypto" = xno],
 [AC_CHECK_HEADERS([openssl/evp.h openssl/opensslv.h])

 AS_IF(
  [test "x$ac_cv_header_openssl_evp_h" = xyes],
  [AC_LANG_PUSH(C)
  AC_MSG_CHECKING(
   [if openssl/evp.h can be used in combination with zlib.h])
  AC_LINK_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <zlib.h>
#include <openssl/evp.h>]],
    [[ ]] )],
    [AC_MSG_RESULT(
     [yes])
    AC_DEFINE(
     [HAVE_OPENSSL_EVP_H],
     [1],
     [Define to 1 if you have the <openssl/evp.h> header file.])
    AC_SUBST(
     [HAVE_OPENSSL_EVP_H],
     [1])
    ac_cv_libcrypto=libcrypto_evp],
   [AC_MSG_RESULT(
    [no])
   ac_cv_libcrypto=no])
  AC_LANG_POP(C)],
  [ac_cv_libcrypto=no])
 ])

dnl Check if all required libcrypto openssl EVP functions are available
AS_IF(
 [test "x$ac_cv_libcrypto" = xlibcrypto_evp],
 [AC_CHECK_LIB(
  crypto,
  EVP_md5,
  [AC_SUBST(
   [LIBCRYPTO_LIBADD],
   [-lcrypto])
  AC_DEFINE(
   [HAVE_LIBCRYPTO],
   [1],
   [Define to 1 if you have the 'crypto' library (-lcrypto).]) ],
  [ac_cv_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_MD_CTX_init,
  [ac_cv_libsmraw_dummy=yes],
  [ac_cv_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_MD_CTX_cleanup,
  [ac_cv_libsmraw_dummy=yes],
  [ac_cv_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DigestInit_ex,
  [ac_cv_libsmraw_dummy=yes],
  [ac_cv_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DigestUpdate,
  [ac_cv_libsmraw_dummy=yes],
  [ac_cv_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DigestFinal_ex,
  [ac_cv_libsmraw_dummy=yes],
  [ac_cv_libcrypto=no])
 ])

AS_IF(
 [test "x$ac_cv_libcrypto" = xlibadvapi32],
 [ac_cv_libcrypto_md5=libadvapi32])

AS_IF(
 [test "x$ac_cv_libcrypto" = xlibcrypto_evp],
 [ac_cv_libcrypto_md5=libcrypto_evp],
 [AC_SUBST(
  [HAVE_OPENSSL_EVP_H],
  [0]) ])

AS_IF(
 [test "x$ac_cv_libcrypto" = xno],
 [ac_cv_libcrypto_md5=no])

dnl Check for libcrypto openssl MD5 support
AS_IF(
 [test "x$ac_cv_libcrypto_md5" = xno],
 [AC_CHECK_HEADERS([openssl/md5.h])

 AS_IF(
  [test "x$ac_cv_header_openssl_md5_h" = xyes],
  [ac_cv_libcrypto_md5=libcrypto
  AC_CHECK_LIB(
   crypto,
   MD5_Init,
   [AC_SUBST(
    [LIBCRYPTO_LIBADD],
    [-lcrypto])
   AC_DEFINE(
    [HAVE_LIBCRYPTO],
    [1],
    [Define to 1 if you have the 'crypto' library (-lcrypto).]) ],
   [ac_cv_libcrypto_md5=no])
  AC_CHECK_LIB(
   crypto,
   MD5_Update,
   [ac_cv_libsmraw_dummy=yes],
   [ac_cv_libcrypto_md5=no])
  AC_CHECK_LIB(
   crypto,
   MD5_Final,
   [ac_cv_libsmraw_dummy=yes],
   [ac_cv_libcrypto_md5=no])
  ])
 ])

AS_IF(
 [test "x$ac_cv_libcrypto_md5" = xno],
 [AC_MSG_NOTICE(
  [No support for calculating MD5 hashes, if required install (openssl) libcrypto with MD5 support or Windows Crypto API.])
 ])

dnl Check if libsmraw should be build with verbose output
AS_IF(
 [test "x$ac_cv_enable_verbose_output" != xno],
 [AC_DEFINE(
  [HAVE_VERBOSE_OUTPUT],
  [1],
  [Define to 1 if verbose output should be used] )
 ac_cv_enable_verbose_output=yes])

dnl Check if libsmraw should be build with debug output
AS_IF(
 [test "x$ac_cv_enable_debug_output" != xno],
 [AC_DEFINE(
  [HAVE_DEBUG_OUTPUT],
  [1],
  [Define to 1 if debug output should be used] )
 ac_cv_enable_debug_output=yes])

dnl Check if requires and build requires should be set in spec file
AS_IF(
 [test "x$ac_cv_libuna" = xyes || test "x$ac_cv_libbfio" = xyes || test "x$ac_cv_libfvalue" = xyes || test "x$ac_cv_libmfdata" = xyes],
 [AC_SUBST(
  [libsmraw_spec_requires],
  [Requires:])
 AC_SUBST(
  [libsmraw_spec_build_requires],
  [BuildRequires:]) ],
 [AC_SUBST(
  [libsmraw_spec_requires],
  [])
 AC_SUBST(
  [libsmraw_spec_build_requires],
  [])
 ac_cv_libsmraw_dummy=yes])

dnl Set memwatch support
ac_libsmraw_have_memwatch=no

AS_IF(
 [test "x$ac_libsmraw_have_memwatch" != xno ],
 [AC_SUBST(
  [MEMWATCH_CPPFLAGS],
  ["-DMEMWATCH -DMEMWATCH_STDIO"])
 AC_SUBST(
  [MEMWATCH_LIBADD],
  [../common/libmemwatch.la]) ])

AM_CONDITIONAL(
 [HAVE_MEMWATCH],
 [test "x$ac_libsmraw_have_memwatch" != xno])

dnl Set additional compiler flags
CFLAGS="$CFLAGS -Wall";

dnl Check if DLL support is needed in smrawtools
AS_IF(
 [test x"$enable_shared" = xyes],
 [AS_CASE(
  [$host],
  [*cygwin* | *mingw*],
  [AC_SUBST(
   [LIBSMRAW_DLL_IMPORT],
   ["-DLIBSMRAW_DLL_IMPORT"])
  ])
 ])

dnl Have configure make the Makefiles
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile])
AC_CONFIG_FILES([common/Makefile])
AC_CONFIG_FILES([libcstring/Makefile])
AC_CONFIG_FILES([liberror/Makefile])
AC_CONFIG_FILES([libnotify/Makefile])
AC_CONFIG_FILES([libuna/Makefile])
AC_CONFIG_FILES([libbfio/Makefile])
AC_CONFIG_FILES([libfvalue/Makefile])
AC_CONFIG_FILES([libmfdata/Makefile])
AC_CONFIG_FILES([libsmraw/Makefile])
AC_CONFIG_FILES([smraw.net/Makefile])
AC_CONFIG_FILES([po/Makefile.in])
AC_CONFIG_FILES([po/Makevars])
AC_CONFIG_FILES([manuals/Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([examples/Makefile])
AC_CONFIG_FILES([borlandc/Makefile])
AC_CONFIG_FILES([msvscpp/Makefile])
dnl Have configure make the include files
AC_CONFIG_FILES([include/libsmraw.h])
AC_CONFIG_FILES([include/libsmraw/definitions.h])
AC_CONFIG_FILES([include/libsmraw/features.h])
AC_CONFIG_FILES([include/libsmraw/types.h])
AC_CONFIG_FILES([libsmraw/libsmraw_definitions.h])
dnl Have configure make distribution specific files
AC_CONFIG_FILES([common/types.h])
AC_CONFIG_FILES([libsmraw/libsmraw.rc])
AC_CONFIG_FILES([libsmraw.pc])
AC_CONFIG_FILES([libsmraw.spec])
dnl Have configure write its configuration
AC_CONFIG_HEADERS([common/config.h])

AC_OUTPUT

dnl Print a summary
AC_MSG_NOTICE([
Building:
   libuna support:                                  $ac_cv_libuna
   libbfio support:                                 $ac_cv_libbfio
   libmfdata support:                               $ac_cv_libmfdata
   libfvalue support:                               $ac_cv_libfvalue
   MD5 support:                                     $ac_cv_libcrypto_md5

Features:
   Wide character type support:                     $ac_cv_enable_wide_character_type
   Verbose output:                                  $ac_cv_enable_verbose_output
   Debug output:                                    $ac_cv_enable_debug_output
]);


