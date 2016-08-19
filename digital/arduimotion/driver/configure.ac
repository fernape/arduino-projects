#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.62)
AC_INIT(am_driver, 0.1, fernando.apesteguia@gmail.com)
AM_INIT_AUTOMAKE(am_driver, 0.1)
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_LANG_C

# Checks for libraries.
PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.20.4)

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h libintl.h Xlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([memset])

AC_OUTPUT(Makefile)
