# stdio_h.m4 serial 1
dnl Copyright (C) 2007 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([gl_STDIO_H],
[
  AC_REQUIRE([gl_STDIO_H_DEFAULTS])
  gl_ABSOLUTE_HEADER([stdio.h])
  ABSOLUTE_STDIO_H=\"$gl_cv_absolute_stdio_h\"
  AC_SUBST([ABSOLUTE_STDIO_H])
])

AC_DEFUN([gl_STDIO_MODULE_INDICATOR],
[
  dnl Use AC_REQUIRE here, so that the default settings are expanded once only.
  AC_REQUIRE([gl_STDIO_H_DEFAULTS])
  GNULIB_[]m4_translit([$1],[abcdefghijklmnopqrstuvwxyz./-],[ABCDEFGHIJKLMNOPQRSTUVWXYZ___])=1
])

AC_DEFUN([gl_STDIO_H_DEFAULTS],
[
  GNULIB_SNPRINTF=0;     AC_SUBST([GNULIB_SNPRINTF])
  GNULIB_VSNPRINTF=0;    AC_SUBST([GNULIB_VSNPRINTF])
  dnl Assume proper GNU behavior unless another module says otherwise.
  HAVE_DECL_SNPRINTF=1;  AC_SUBST([HAVE_DECL_SNPRINTF])
  HAVE_DECL_VSNPRINTF=1; AC_SUBST([HAVE_DECL_VSNPRINTF])
])
