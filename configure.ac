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
		       gobject-2.0
		       gdk-2.0])
PKG_CHECK_MODULES(GTK,[gtk+-2.0])

gfc_has_gconf=
PKG_CHECK_MODULES(GCONF,[gconf-2.0],[gfc_has_gconf=yes],[gfc_has_gconf=no])
AM_CONDITIONAL(WITH_GNOME,[test "x$gfc_has_gconf" = "xyes"])
AM_CONDITIONAL(WITH_OSX,[test "x$gfc_has_gconf" != "xyes"])

if test "x${gfc_has_gconf}" != "xyes"; then
	AC_PROG_OBJC

	PLATFORM_CFLAGS=
	PLATFORM_LIBS="-framework AppKit"
	AC_SUBST(PLATFORM_CFLAGS)
	AC_SUBST(PLATFORM_LIBS)
else
	GFC_CFLAGS="$GFC_CFLAGS $GCONF_CFLAGS"
	GFC_LIBS="$GFC_LIBS $GCONF_LIBS"
	AC_SUBST(GFC_CFLAGS)
	AC_SUBST(GFC_LIBS)

	OBJC='$(CC)'
	AC_SUBST(OBJC)
	AM_CONDITIONAL(am__fastdepOBJC,true)
fi

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
