AC_PREREQ( 2.59 )

AC_INIT(
 [libsmraw],
  [20100627],
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
LIBSMRAW_TEST_ENABLE(
 [wide-character-type],
 [wide_character_type],
 [enable wide character type support],
 [no])
LIBSMRAW_TEST_ENABLE(
 [winapi],
 [winapi],
 [enable WINAPI support for cross-compilation],
 [no])
LIBSMRAW_TEST_ENABLE(
 [verbose-output],
 [verbose_output],
 [enable verbose output],
 [no])
LIBSMRAW_TEST_ENABLE(
 [debug-output],
 [debug_output],
 [enable debug output],
 [no])

dnl Check for libtool DLL support
AC_LIBTOOL_WIN32_DLL

dnl Checks for programs
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Check for compiler language support
AC_C_CONST
AC_C_VOLATILE

dnl Check for headers
dnl Headers included in include/libsmraw/types.h
AC_CHECK_HEADER(
 [sys/types.h],
 [AC_DEFINE(
  [HAVE_SYS_TYPES_H],
  [1],
  [Define to 1 if you have the <sys/types.h> header file.])
 AC_SUBST(
  [HAVE_SYS_TYPES_H],
  [1])],
 [AC_SUBST(
  [HAVE_SYS_TYPES_H],
  [0])])

AC_CHECK_HEADER(
 [inttypes.h],
 [AC_DEFINE(
  [HAVE_INTTYPES_H],
  [1],
  [Define to 1 if you have the <inttypes.h> header file.])
 AC_SUBST(
  [HAVE_INTTYPES_H],
  [1])],
 [AC_SUBST(
  [HAVE_INTTYPES_H],
  [0])])

AC_CHECK_HEADER(
 [stdint.h],
 [AC_DEFINE(
  [HAVE_STDINT_H],
  [1],
  [Define to 1 if you have the <stdint.h> header file.])
 AC_SUBST(
  [HAVE_STDINT_H],
  [1])],
 [AC_SUBST(
  [HAVE_STDINT_H],
  [0])])

dnl Headers included in common/common.h
AS_IF(
 [test "x$ac_cv_libsmraw_enable_winapi" = xyes],
 [AC_CHECK_HEADERS([windows.h])

 AS_IF(
  [test "x$ac_cv_header_windows_h" = xno],
  [AC_MSG_FAILURE(
   [Missing header: windows.h header is required to compile with winapi support],
   [1]) ])
 ])

dnl Headers included in common/types.h
AC_CHECK_HEADERS([limits.h])

dnl Headers included in common/memory.h and libcstring/libcstring_narrow_string.h
AC_CHECK_HEADERS([stdlib.h string.h])

dnl Headers included in liberror/liberror_error.c and libnotify/libnotify_print.c
AC_CHECK_HEADERS([stdarg.h])

AS_IF(
 [test "x$ac_cv_header_stdarg_h" != xyes],
 [AC_CHECK_HEADERS([varargs.h]) ])

AS_IF(
 [test "x$ac_cv_header_stdarg_h" != xyes && test "x$ac_cv_header_varargs_h" != xyes],
 [AC_MSG_FAILURE(
  [Missing headers: stdarg.h and varargs.h],
  [1]) ])

dnl Headers included in libnotify/libnotify_stream.c
AC_CHECK_HEADERS([errno.h])

dnl Headers included in libbfio/libbfio_file.c
AC_CHECK_HEADERS([fcntl.h sys/stat.h unistd.h])

dnl Other headers

dnl Check for declarations and type definitions
AC_TYPE_OFF_T
AC_TYPE_SIZE_T

AC_CHECK_TYPE(
 [size32_t],
 [AC_SUBST(
  [HAVE_SIZE32_T],
  [1])],
 [AC_SUBST(
  [HAVE_SIZE32_T],
  [0])])

AC_CHECK_TYPE(
 [ssize32_t],
 [AC_SUBST(
  [HAVE_SSIZE32_T],
  [1])],
 [AC_SUBST(
  [HAVE_SSIZE32_T],
  [0])])

AC_CHECK_TYPE(
 [size64_t],
 [AC_SUBST(
  [HAVE_SIZE64_T],
  [1])],
 [AC_SUBST(
  [HAVE_SIZE64_T],
  [0])])

AC_CHECK_TYPE(
 [ssize64_t],
 [AC_SUBST(
  [HAVE_SSIZE64_T],
  [1])],
 [AC_SUBST(
  [HAVE_SSIZE64_T],
  [0])])

AC_CHECK_TYPE(
 [off64_t],
 [AC_SUBST(
  [HAVE_OFF64_T],
  [1])],
 [AC_SUBST(
  [HAVE_OFF64_T],
  [0])])

AC_CHECK_TYPE([ssize_t])
AC_CHECK_TYPE([u64])

AC_CHECK_SIZEOF([off_t])
AC_CHECK_SIZEOF([size_t])

dnl Check for functions
dnl File stream functions used in common/file_stream.h
AC_CHECK_FUNCS(
 [fclose],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fclose],
  [1]) ])

AC_CHECK_FUNCS(
 [feof],
 [],
 [AC_MSG_FAILURE(
  [Missing function: feof],
  [1]) ])

AC_CHECK_FUNCS(
 [fgets],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fgets],
  [1]) ])

AC_CHECK_FUNCS(
 [fopen],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fopen],
  [1]) ])

AC_CHECK_FUNCS(
 [fread],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fread],
  [1]) ])

AC_CHECK_FUNCS(
 [fseeko],
 [],
 [AC_CHECK_FUNCS(
  [fseeko64],
  [],
  [AC_MSG_FAILURE(
   [Missing function: fseeko and fseeko64],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [fwrite],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fwrite],
  [1]) ])

AC_CHECK_FUNCS(
 [vfprintf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: vfprintf],
  [1]) ])

dnl Memory functions used in common/memory.h
AC_CHECK_FUNCS(
 [free],
 [],
 [AC_MSG_FAILURE(
  [Missing function: free],
  [1]) ])

AC_CHECK_FUNCS(
 [malloc],
 [],
 [AC_MSG_FAILURE(
  [Missing function: malloc],
  [1]) ])

AC_CHECK_FUNCS(
 [memcmp],
 [],
 [AC_MSG_FAILURE(
  [Missing function: memcmp],
  [1]) ])

AC_CHECK_FUNCS(
 [memcpy],
 [],
 [AC_MSG_FAILURE(
  [Missing function: memcpy],
  [1]) ])

AC_CHECK_FUNCS(
 [memset],
 [],
 [AC_MSG_FAILURE(
  [Missing function: memset],
  [1]) ])

AC_CHECK_FUNCS(
 [realloc],
 [],
 [AC_MSG_FAILURE(
  [Missing function: realloc],
  [1]) ])

dnl Narrow character string functions used in libcstring/libcstring_narrow_string.h
AC_CHECK_FUNCS(
 [fgets],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fgets],
  [1]) ])

AC_CHECK_FUNCS(
 [memchr],
 [],
 [AC_CHECK_FUNCS(
  [strchr],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: memchr and strchr],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [memcmp],
 [],
 [AC_CHECK_FUNCS(
  [strncmp],
  [],
  [AC_CHECK_FUNCS(
   [strcmp],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: memcmp, strncmp and strcmp],
    [1]) ]) ]) ])

AC_CHECK_FUNCS(
 [memcpy],
 [],
 [AC_CHECK_FUNCS(
  [strncpy],
  [],
  [AC_CHECK_FUNCS(
   [strcpy],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: memcpy, strncpy and strcpy],
    [1]) ]) ]) ])

AC_CHECK_FUNCS(
 [memrchr],
 [AC_CHECK_DECLS(
  [memrchr],
  [ac_libsmraw_have_memrchr=yes],
  [ac_libsmraw_have_memrchr=no]) ],
 [ac_libsmraw_have_memrchr=no])

AS_IF(
 [test "x$ac_libsmraw_have_memrchr" = xno],
 [AC_CHECK_FUNCS(
 [strrchr],
 [],
 [AC_MSG_FAILURE(
 [Missing functions: strrchr and memrchr],
 [1]) ]) ])

AC_CHECK_FUNCS(
 [snprintf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: snprintf],
  [1]) ])

AC_CHECK_FUNCS(
 [sscanf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: sscanf],
  [1]) ])

AC_CHECK_FUNCS(
 [strlen],
 [],
 [AC_MSG_FAILURE(
  [Missing function: strlen],
  [1]) ])

AC_CHECK_FUNCS(
 [strncasecmp],
 [],
 [AC_CHECK_FUNCS(
  [strcasecmp],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strncasecmp and strcasecmp],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strtoll],
 [],
 [AC_CHECK_FUNCS(
  [atoll],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strtoll and atoll],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strtoull],
 [],
 [AC_CHECK_FUNCS(
  [atoll],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strtoull and atoll],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [vsnprintf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: vsnprintf],
  [1]) ])

dnl File input/output functions used in libbfio/libbfio_file.h
AC_CHECK_FUNCS(
 [close],
 [],
 [AC_MSG_FAILURE(
  [Missing function: close],
  [1]) ])

AC_CHECK_FUNCS(
 [lseek],
 [],
 [AC_MSG_FAILURE(
  [Missing function: lseek],
  [1]) ])

AC_CHECK_FUNCS(
 [open],
 [],
 [AC_MSG_FAILURE(
  [Missing function: open],
  [1]) ])

AC_CHECK_FUNCS(
 [read],
 [],
 [AC_MSG_FAILURE(
  [Missing function: read],
  [1]) ])

AC_CHECK_FUNCS(
 [write],
 [],
 [AC_MSG_FAILURE(
  [Missing function: write],
  [1]) ])

AC_CHECK_FUNCS(
 [fstat],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fstat],
  [1]) ])

dnl Check for error string functions used in libbfio/libbfio_error_string.h
AC_FUNC_STRERROR_R()

AS_IF(
 [test "x$ac_cv_have_decl_strerror_r" = xno],
 [AC_CHECK_FUNCS(
  [strerror],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strerror_r and strerror],
   [1]) ]) ])

dnl Check for printf conversion specifier support
LIBSMRAW_CHECK_FUNC_PRINTF_JD
LIBSMRAW_CHECK_FUNC_PRINTF_ZD

dnl Checks for libraries and library functions
dnl ac_libsmraw_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments

dnl Check for enabling libuna for Unicode string support
LIBSMRAW_CHECK_LIBUNA

AS_IF(
 [test "x$ac_libsmraw_have_libuna" = xyes],
 [dnl libuna is available
 AC_SUBST(
  [libsmraw_pc_libs_private_libuna],
  [-luna])
 AC_SUBST(
  [libsmraw_spec_requires],
  [Requires:])
 AC_SUBST(
  [libsmraw_spec_requires_libuna],
  [libuna])
 AC_SUBST(
  [libsmraw_spec_build_requires],
  [BuildRequires:])
 AC_SUBST(
  [libsmraw_spec_build_requires_libuna],
  [libuna-devel]) ],
 [dnl libuna is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBUNA],
  [1],
  [Define to 1 if the local version of libuna is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBUNA],
  [1])
 AC_SUBST(
  [libsmraw_pc_libs_private_libuna],
  [])
 AC_SUBST(
  [libsmraw_spec_requires],
  [])
 AC_SUBST(
  [libsmraw_spec_requires_libuna],
  [])
 AC_SUBST(
  [libsmraw_spec_build_requires],
  [])
 AC_SUBST(
  [libsmraw_spec_build_requires_libuna],
  [])
 AC_SUBST(
  [LIBUNA_CPPFLAGS],
  [-I../libuna])
 AC_SUBST(
  [LIBUNA_LIBADD],
  [../libuna/libuna.la])
 ac_libsmraw_have_libuna=local])
 
AM_CONDITIONAL(
 [HAVE_LOCAL_LIBUNA],
 [test "x$ac_libsmraw_have_libuna" = xlocal])

dnl Check for enabling libbfio for basic file IO support
LIBSMRAW_CHECK_LIBBFIO

AS_IF(
 [test "x$ac_libsmraw_have_libbfio" = xyes],
 [dnl libbfio is available
 AC_SUBST(
  [libsmraw_pc_libs_private_libbfio],
  [-lbfio])
 AC_SUBST(
  [libsmraw_spec_requires_libbfio],
  [libbfio])
 AC_SUBST(
  [libsmraw_spec_build_requires_libbfio],
  [libbfio-devel])
 AC_SUBST(
  [LIBBFIO_CPPFLAGS],
  [])
 AC_SUBST(
  [LIBBFIO_LIBADD],
  [])
 AC_SUBST(
  [HAVE_LIBBFIO],
  [1]) ],
 [dnl libbfio is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBBFIO],
  [1],
  [Define to 1 if the local version of libbfio is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBBFIO],
  [1])
 AC_SUBST(
  [libsmraw_pc_libs_private_libbfio],
  [])
 AC_SUBST(
  [libsmraw_spec_requires_libbfio],
  [])
 AC_SUBST(
  [libsmraw_spec_build_requires_libbfio],
  [])
 AC_SUBST(
  [LIBBFIO_CPPFLAGS],
  [-I../libbfio])
 AC_SUBST(
  [LIBBFIO_LIBADD],
  [../libbfio/libbfio.la])
 AC_SUBST(
  [HAVE_LIBBFIO],
  [0])
 ac_libsmraw_have_libbfio=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBBFIO],
 [test "x$ac_libsmraw_have_libbfio" = xlocal])

dnl Check for enabling crypographic digest hash support
ac_libsmraw_have_libcrypto=no

AS_IF(
 [test "x$ac_cv_libsmraw_enable_winapi" = xyes],
 [AC_SUBST(
  [LIBCRYPTO_LIBADD],
  [-ladvapi32])
  ac_libsmraw_have_libcrypto=libadvapi32])

dnl Check for libcrypto openssl EVP support
AS_IF(
 [test "x$ac_libsmraw_have_libcrypto" = xno],
 [AC_CHECK_HEADERS([openssl/evp.h openssl/opensslv.h])

 AS_IF(
  [test "x$ac_cv_header_openssl_evp_h" = xyes],
  [AC_LANG_PUSH(C)
  AC_MSG_CHECKING(
   [if openssl/evp.h can be used in combination with zlib.h])
  AC_LINK_IFELSE(
   AC_LANG_PROGRAM(
    [[#include <zlib.h>
#include <openssl/evp.h>]],
    [[ ]]),
    [AC_MSG_RESULT(
     [yes])
    AC_DEFINE(
     [HAVE_OPENSSL_EVP_H],
     [1],
     [Define to 1 if you have the <openssl/evp.h> header file.])
    AC_SUBST(
     [HAVE_OPENSSL_EVP_H],
     [1])
    ac_libsmraw_have_libcrypto=libcrypto_evp],
   [AC_MSG_RESULT(
    [no])
   ac_libsmraw_have_libcrypto=no])
  AC_LANG_POP(C)],
  [ac_libsmraw_have_libcrypto=no])
 ])

dnl Check if all required libcrypto openssl EVP functions are available
AS_IF(
 [test "x$ac_libsmraw_have_libcrypto" = xlibcrypto_evp],
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
  [ac_libsmraw_have_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_MD_CTX_init,
  [ac_libsmraw_dummy=yes],
  [ac_libsmraw_have_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_MD_CTX_cleanup,
  [ac_libsmraw_dummy=yes],
  [ac_libsmraw_have_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DigestInit_ex,
  [ac_libsmraw_dummy=yes],
  [ac_libsmraw_have_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DigestUpdate,
  [ac_libsmraw_dummy=yes],
  [ac_libsmraw_have_libcrypto=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DigestFinal_ex,
  [ac_libsmraw_dummy=yes],
  [ac_libsmraw_have_libcrypto=no])
 ])

AS_IF(
 [test "x$ac_libsmraw_have_libcrypto" = xlibadvapi32],
 [ac_libsmraw_have_libcrypto_md5=libadvapi32])

AS_IF(
 [test "x$ac_libsmraw_have_libcrypto" = xlibcrypto_evp],
 [ac_libsmraw_have_libcrypto_md5=libcrypto_evp],
 [AC_SUBST(
  [HAVE_OPENSSL_EVP_H],
  [0]) ])

AS_IF(
 [test "x$ac_libsmraw_have_libcrypto" = xno],
 [ac_libsmraw_have_libcrypto_md5=no])

dnl Check for libcrypto openssl MD5 support
AS_IF(
 [test "x$ac_libsmraw_have_libcrypto_md5" = xno],
 [AC_CHECK_HEADERS([openssl/md5.h])

 AS_IF(
  [test "x$ac_cv_header_openssl_md5_h" = xyes],
  [ac_libsmraw_have_libcrypto_md5=libcrypto
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
   [ac_libsmraw_have_libcrypto_md5=no])
  AC_CHECK_LIB(
   crypto,
   MD5_Update,
   [ac_libsmraw_dummy=yes],
   [ac_libsmraw_have_libcrypto_md5=no])
  AC_CHECK_LIB(
   crypto,
   MD5_Final,
   [ac_libsmraw_dummy=yes],
   [ac_libsmraw_have_libcrypto_md5=no])
  ])
 ])

AS_IF(
 [test "x$ac_libsmraw_have_libcrypto_md5" = xno],
 [AC_MSG_NOTICE(
  [No support for calculating MD5 hashes, if required install (openssl) libcrypto with MD5 support or Windows Crypto API.])
 ])

dnl Check if libsmraw should be build with wide character type support
AS_IF(
 [test "x$ac_cv_libsmraw_enable_wide_character_type" != xno ],
 [dnl Check for the wide character header
 AC_CHECK_HEADERS(
  [wchar.h],
  [AC_DEFINE(
   [HAVE_WCHAR_H],
   [1],
   [Define to 1 if you have the <wchar.h> header file.])
  AC_SUBST(
   [HAVE_WCHAR_H],
   [1]) ],
  [AC_MSG_FAILURE(
   [Missing header: wchar.h],
   [1]) ])

 dnl Determine wide character byte size
 AC_CHECK_SIZEOF([wchar_t])

 dnl Check for wide character string functions

 dnl Wide character string functions used in common/file_stream.h
 AC_CHECK_FUNCS(
  [fgetws],
  [],
  [AC_MSG_FAILURE(
   [Missing function: fgetws],
   [1]) ])

 dnl Wide character string functions used in libcstring/libcstring_wide_string.h
 AC_CHECK_FUNCS(
  [swprintf],
  [],
  [AC_MSG_FAILURE(
   [Missing function: swprintf],
   [1]) ])

 AC_CHECK_FUNCS(
  [wmemchr],
  [],
  [AC_CHECK_FUNCS(
   [wcschr],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: wmemchr and wcschr],
    [1]) ]) ])

 AC_CHECK_FUNCS(
  [wmemcmp],
  [],
  [AC_CHECK_FUNCS(
   [wcsncmp],
   [],
   [AC_CHECK_FUNCS(
    [wcscmp],
    [],
    [AC_MSG_FAILURE(
     [Missing functions: wmemcmp, wcsncmp and wcscmp],
     [1]) ]) ]) ])

 AC_CHECK_FUNCS(
  [wmemcpy],
  [],
  [AC_CHECK_FUNCS(
   [wcsncpy],
   [],
   [AC_CHECK_FUNCS(
    [wcscpy],
    [],
    [AC_MSG_FAILURE(
     [Missing functions: wmemcpy, wcsncpy and wcscpy],
     [1]) ]) ]) ])

 AC_CHECK_FUNCS(
  [wmemrchr],
  [],
  [AC_CHECK_FUNCS(
   [wcsrchr],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: wmemrchr and wcsrchr],
    [1]) ]) ])

 AC_CHECK_FUNCS(
  [wcslen],
  [],
  [AC_MSG_FAILURE(
   [Missing function: wcslen],
   [1]) ])

 AC_CHECK_FUNCS(
  [wcsncasecmp],
  [],
  [AC_CHECK_FUNCS(
   [wcscasecmp],
   [],
   [AC_CHECK_FUNCS(
    [towlower],
    [],
    [AC_MSG_FAILURE(
     [Missing functions: wcsncasecmp, wcscasecmp and towlower],
     [1]) ]) ]) ])

 AC_CHECK_FUNCS(
  [wcstoll],
  [],
  [AC_MSG_FAILURE(
   [Missing function: wcstoll],
   [1]) ])

 AC_CHECK_FUNCS(
  [wcstoull],
  [],
  [AC_MSG_FAILURE(
   [Missing function: wcstoull],
   [1]) ])

 dnl Make the necessary definitions
 AC_DEFINE(
  [HAVE_WIDE_CHARACTER_TYPE],
  [1],
  [Define to 1 if wide character type should be used.])
 AC_SUBST(
  [HAVE_WIDE_CHARACTER_TYPE],
  [1])
 ac_cv_libsmraw_enable_wide_character_type=yes],

 [AC_SUBST(
  [HAVE_WIDE_CHARACTER_TYPE],
  [0])
 AC_SUBST(
  [HAVE_WCHAR_H],
  [0]) ])

AC_SUBST(
 [HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS],
 [0])

dnl Check if libsmraw should be build with verbose output
AS_IF(
 [test "x$ac_cv_libsmraw_enable_verbose_output" != xno],
 [AC_DEFINE(
  [HAVE_VERBOSE_OUTPUT],
  [1],
  [Define to 1 if verbose output should be used] )
 ac_cv_libsmraw_enable_verbose_output=yes])

dnl Check if libsmraw should be build with debug output
AS_IF(
 [test "x$ac_cv_libsmraw_enable_debug_output" != xno],
 [AC_DEFINE(
  [HAVE_DEBUG_OUTPUT],
  [1],
  [Define to 1 if debug output should be used] )
 ac_cv_libsmraw_enable_debug_output=yes])

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
AC_CONFIG_FILES([libsmraw/Makefile])
AC_CONFIG_FILES([manuals/Makefile])
AC_CONFIG_FILES([tests/Makefile])
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
AC_CONFIG_FILES([libsmraw.pc])
AC_CONFIG_FILES([libsmraw.spec])
dnl Have configure write its configuration
AC_CONFIG_HEADERS([common/config.h])

AC_OUTPUT

dnl Print a summary
AC_MSG_NOTICE([
Building:
   libuna support:                                  $ac_libsmraw_have_libuna
   libbfio support:                                 $ac_libsmraw_have_libbfio
   MD5 support:                                     $ac_libsmraw_have_libcrypto_md5

Features:
   Wide character type support:                     $ac_cv_libsmraw_enable_wide_character_type
   Verbose output:                                  $ac_cv_libsmraw_enable_verbose_output
   Debug output:                                    $ac_cv_libsmraw_enable_debug_output
]);


