AC_INIT(autotool,0.0.1,mailto:herzi@gnome-de.org)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)

dnl skip c++ and f77 checks
CXX=no
F77=no

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

AM_PATH_GLIB_2_0
PKG_CHECK_MODULES(GFC,[glib-2.0 >= 2.16
		       gobject-2.0])
PKG_CHECK_MODULES(GTK,[gtk+-2.0])

AC_CHECK_PROGS(VALAC, valac)
if test "x$VALAC" != "x"; then
	AC_MSG_CHECKING([for vala version >= 0.1.6])
	PKG_CHECK_EXISTS([vala-1.0 >= 0.1.6],
			 [AC_MSG_RESULT([yes])],
			 [VALAC=
			  AC_MSG_RESULT([no])])
fi
AM_CONDITIONAL(HAVE_VALAC, [test "x$VALAC" != "x"])

AC_OUTPUT(Makefile)
