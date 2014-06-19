AC_INIT([renmod], [20140619])
AM_INIT_AUTOMAKE
AM_PROG_CC_C_O
AC_CHECK_HEADER([mikmod.h], [], [AC_MSG_ERROR([missing header: mikmod.h])])
AC_CHECK_HEADER([string.h], [], [AC_MSG_ERROR([missing header: string.h])])
AC_CHECK_HEADER([stdlib.h], [], [AC_MSG_ERROR([missing header: stdlib.h])])
AC_CHECK_HEADER([stdio.h], [], [AC_MSG_ERROR([missing header: stdio.h])])
AC_CHECK_LIB([mikmod], [MikMod_Init], [], [AC_MSG_ERROR([cannot find the mikmod library, go to http://mikmod.sourceforge.net/])])
AC_CHECK_LIB([gtk-3], [gtk_init], [], [AC_MSG_ERROR([cannot find the gtk-3 library, go to http://www.gtk.org/])])
AC_CHECK_PROG([HAVE_PKG_CONFIG],[pkg-config],[yes])
if test "x$HAVE_PKG_CONFIG" != xyes; then
	AC_MSG_ERROR([cannot find the pkg-config program, go to http://www.freedesktop.org/])
fi
AC_SUBST([GTK_CFLAGS], [`pkg-config gtk+-3.0 --cflags`])
AC_SUBST([GTK_LIBS], [`pkg-config gtk+-3.0 --libs`])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
