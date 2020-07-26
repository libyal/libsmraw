dnl Checks for required headers and functions
dnl
dnl Version: 20200713

dnl Function to detect if ctime_r or ctime is available
dnl Also checks how to use ctime_r
AC_DEFUN([AX_SMRAWTOOLS_CHECK_FUNC_CTIME],
 [AC_CHECK_FUNCS([ctime_r])

 AS_IF(
   [test "x$ac_cv_func_ctime_r" = xyes],
   [AC_MSG_CHECKING(
     [how to use ctime_r])

  AC_LANG_PUSH(C)

  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [[#include <time.h>]],
      [[ctime_r( NULL, NULL, 0 )]] )],
      [AC_MSG_RESULT(
        [with additional size argument])
      ac_cv_cv_ctime_r_size=yes],
      [ac_cv_cv_ctime_r_size=no])

  AS_IF(
    [test "x$ac_cv_cv_ctime_r_size" = xno],
    [AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <time.h>]],
        [[ctime_r( NULL, NULL )]] )],
      [AC_MSG_RESULT(
        [with two arguments])
      ac_cv_cv_ctime_r_posix=yes],
      [ac_cv_cv_ctime_r_posix=no])
  ])

  AS_IF(
    [test "x$ac_cv_cv_ctime_r_posix" = xno],
    [CPPFLAGS="$CPPFLAGS -D_POSIX_PTHREAD_SEMANTICS"
    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <time.h>]],
        [[ctime_r( NULL, NULL )]] )],
      [AC_MSG_RESULT(
        [with two arguments and definition _POSIX_PTHREAD_SEMANTICS])
        ac_cv_cv_ctime_r_posix=yes],
       [ac_cv_cv_ctime_r_posix=no])
  ])

  AC_LANG_POP(C)

  AS_IF(
    [test "x$ac_cv_cv_ctime_r_size" = xno && test "x$ac_cv_cv_ctime_r_posix" = xno],
    [AC_MSG_WARN(
      [unknown])
    ac_cv_func_ctime_r=no])

  AS_IF(
    [test "x$ac_cv_func_ctime_r" = xyes],
    [AC_DEFINE(
      [HAVE_CTIME_R],
      [1],
      [Define to 1 if you have the ctime_r function.])
  ])

  AS_IF(
    [test "x$ac_cv_cv_ctime_r_size" = xyes],
    [AC_DEFINE(
      [HAVE_CTIME_R_SIZE],
      [1],
      [Define to 1 if you have the ctime_r function with a third size argument.])
    ])
  ])

 AS_IF(
   [test "x$ac_cv_func_ctime_r" = xno],
   [AC_CHECK_FUNCS([ctime])

   AS_IF(
     [test "x$ac_cv_func_ctime" = xno],
     [AC_MSG_FAILURE(
       [Missing function: ctime_r and ctime],
       [1])
    ])
  ])
])

dnl Function to detect if libsmraw dependencies are available
AC_DEFUN([AX_LIBSMRAW_CHECK_LOCAL],
  [dnl Check for internationalization functions in libsmraw/libsmraw_i18n.c
  AC_CHECK_FUNCS([bindtextdomain])
])

dnl Function to detect if smrawtools dependencies are available
AC_DEFUN([AX_SMRAWTOOLS_CHECK_LOCAL],
  [dnl Headers used in smrawtools
  AC_CHECK_HEADERS([signal.h sys/signal.h unistd.h])

  dnl Headers included in smrawtools/smrawtools_glob.h
  AC_CHECK_HEADERS([errno.h glob.h])

  AS_IF(
    [test "x$ac_cv_header_glob_h" = xno],
    [AC_CHECK_HEADERS([io.h])
  ])

  dnl Functions used in smrawtools
  AC_CHECK_FUNCS([close getopt setvbuf])

  AS_IF(
   [test "x$ac_cv_func_close" != xyes],
   [AC_MSG_FAILURE(
     [Missing function: close],
     [1])
  ])

  dnl Headers included in smrawtools/log_handle.c
  AC_CHECK_HEADERS([stdarg.h varargs.h])

  AS_IF(
    [test "x$ac_cv_header_stdarg_h" != xyes && test "x$ac_cv_header_varargs_h" != xyes],
    [AC_MSG_FAILURE(
      [Missing headers: stdarg.h and varargs.h],
      [1])
  ])

  dnl Functions used in smrawtools/smrawmount.c
  AS_IF(
   [test "x$ac_cv_enable_winapi" = xno],
   [AC_HEADER_TIME
   AC_CHECK_FUNCS([clock_gettime getegid geteuid time])
   ])

  dnl Date and time functions used in smrawtools/process_status.c
  AX_SMRAWTOOLS_CHECK_FUNC_CTIME
 
  AC_CHECK_FUNCS([gmtime gmtime_r])

  AS_IF(
   [test "x$ac_cv_func_gmtime" != xyes && test "x$ac_cv_func_gmtime_r" != xyes],
   [AC_MSG_FAILURE(
    [Missing functions: gmtime_r and gmtime],
    [1])
   ])
 
  AS_IF(
   [test "x$ac_cv_func_time" != xyes],
   [AC_MSG_FAILURE(
    [Missing function: time],
    [1])
   ])
])

dnl Function to check if DLL support is needed
AC_DEFUN([AX_LIBSMRAW_CHECK_DLL_SUPPORT],
  [AS_IF(
    [test "x$enable_shared" = xyes && test "x$ac_cv_enable_static_executables" = xno],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw* | *msys*],
      [AC_DEFINE(
        [HAVE_DLLMAIN],
        [1],
        [Define to 1 to enable the DllMain function.])
      AC_SUBST(
        [HAVE_DLLMAIN],
        [1])

      AC_SUBST(
        [LIBSMRAW_DLL_EXPORT],
        ["-DLIBSMRAW_DLL_EXPORT"])

      AC_SUBST(
        [LIBSMRAW_DLL_IMPORT],
        ["-DLIBSMRAW_DLL_IMPORT"])
      ])
    ])
  ])

