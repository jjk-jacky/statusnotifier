#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([statusnotifier], [0.1.0], [jjk@jjacky.com])
AC_CONFIG_SRCDIR([src/statusnotifier.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([-Wall -Werror -Wno-portability foreign silent-rules subdir-objects])
AM_SILENT_RULES([yes])

# Option to make doc
m4_ifdef([GTK_DOC_CHECK], [
GTK_DOC_CHECK([1.19],[--flavour no-tmpl])
AC_PATH_PROG([XSLTPROC], [xsltproc])
  if test -z "$XSLTPROC"; then
    if test "$enable_man" = yes ; then
        AC_MSG_ERROR([xsltproc is required for --enable-gtk-doc])
    fi
  fi
],[
AM_CONDITIONAL([ENABLE_GTK_DOC], false)
])

# Checks for programs.
AC_PROG_CC
AM_PROG_AR

LT_INIT

# Option for example
AC_ARG_ENABLE([example],
	AS_HELP_STRING([--enable-example], [enable the example]),
	[wantexample=$enableval], [wantexample=no])

# Option to use git version
AC_ARG_ENABLE([git-version],
	AS_HELP_STRING([--enable-git-version], [enable the use of git version]),
	[wantgitver=$enableval], [wantgitver=no])

# Enable extra compiler warning flags
AC_ARG_ENABLE([warning-flags],
	AS_HELP_STRING([--enable-warning-flags], [enable extra compiler warning flags]),
	[warningflags=$enableval], [warningflags=no])

AC_ARG_ENABLE([dbusmenu],
	AS_HELP_STRING([--enable-dbusmenu], [enable extra dbusmenu functionality via libdbusmenu]),
	[dbusmenu=$enableval], [dbusmenu=no])

# Checks for libraries.
PKG_CHECK_MODULES(GOBJECT, [gobject-2.0], , AC_MSG_ERROR([GLib/GObject is required]))
PKG_CHECK_MODULES(GIO, [gio-2.0], , AC_MSG_ERROR([GLib/GIO is required]))
PKG_CHECK_MODULES(GDK, [gdk-3.0], , AC_MSG_ERROR([GDK 3 is required]))
PKG_CHECK_MODULES(GDK_PIXBUF, [gdk-pixbuf-2.0], , AC_MSG_ERROR([gdk-pixbuf is required]))
if test "x$wantexample" = "xyes"; then
    PKG_CHECK_MODULES(GTK, [gtk+-3.0],
                      AC_DEFINE([EXAMPLE], , [Enable example]),
                      AC_MSG_ERROR([GTK+3 is required for the example]))
else
    enable_example=no
fi
AM_CONDITIONAL(EXAMPLE, test "x$wantexample" = "xyes")

DEP_PACKAGES="gobject-2.0 gio-2.0 gdk-3.0 gdk-pixbuf-2.0"
DEP_CFLAGS="$GOBJECT_CFLAGS $GIO_CFLAGS $GDK_CFLAGS $GDK_PIXBUF_CFLAGS"
DEP_LIBS="$GOBJECT_LIBS $GIO_LIBS $GDK_LIBS $GDK_PIXBUF_LIBS"

# dbusmenu support
if test "x$dbusmenu" = "xyes"; then
    # we require GTK to deal with GtkWidget-s (menu to export)
    PKG_CHECK_MODULES(GTK, [gtk+-3.0], ,
        AC_MSG_ERROR([GTK+3 is required for dbusmenu support]))
    DEP_PACKAGES="$DEP_PACKAGES gtk+-3.0"
    DEP_CFLAGS="$DEP_CFLAGS $GTK_CFLAGS"
    DEP_LIBS="$DEP_LIBS $GTK_LIBS"

    PKG_CHECK_MODULES(DBUSMENU, [dbusmenu-glib-0.4 dbusmenu-gtk3-0.4], ,
        AC_MSG_ERROR([dbusmenu-glib and dbusmenu-gtk3 are required for dbusmenu support])
    ])
    DEP_PACKAGES="$DEP_PACKAGES dbusmenu-glib-0.4 dbusmenu-gtk3-0.4"
    DEP_CFLAGS="$DEP_CFLAGS $DBUSMENU_CFLAGS"
    DEP_LIBS="$DEP_LIBS $DBUSMENU_LIBS"

    AC_DEFINE([USE_DBUSMENU], 1, [Use dbusmenu])
    dbusmenu=yes
else
    dbusmenu=no
fi
AM_CONDITIONAL(USE_DBUSMENU, test "x$dbusmenu" = "xyes")

# introspection
GOBJECT_INTROSPECTION_CHECK([0.6.3])

AC_SUBST(DEP_PACKAGES)
AC_SUBST(DEP_CFLAGS)
AC_SUBST(DEP_LIBS)

# Checks for header files.
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# git version
AC_MSG_CHECKING([if git version must be used])
if test "x$wantgitver" = "xyes"; then
    AC_MSG_RESULT([yes])
    AC_CHECK_PROGS([GIT], [git])
    if test "x$GIT" = "x"; then
        AC_MSG_ERROR([Cannot use git version: git not found])
    fi
    AC_CHECK_FILE([.git/], hasgitdir=yes)
    if test "x$hasgitdir" = "xyes"; then
        usegitver=yes
        gitver=-git
        AC_DEFINE([USE_GIT_VERSION], , [Use GIT version])
    else
        AC_MSG_ERROR([Cannot use git version: .git not found])
    fi
else
    AC_MSG_RESULT([no])
    usegitver=no
    gitver=
fi
AM_CONDITIONAL(USE_GIT_VERSION, test "x$usegitver" = "xyes")

# warning flags
WARNING_CFLAGS="-Wall"
AC_MSG_CHECKING([for extra compiler warning flags])
if test "x$warningflags" = "xyes"; then
    AC_MSG_RESULT([yes])
    CFLAGS_ADD([-Wextra], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wshadow], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wpointer-arith], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wcast-align], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wwrite-strings], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wmissing-prototypes], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wmissing-declarations], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wredundant-decls], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wnested-externs], [WARNING_CFLAGS])
    CFLAGS_ADD([-Winline], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wno-long-long], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wuninitialized], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wconversion], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wstrict-prototypes], [WARNING_CFLAGS])

    CFLAGS_ADD([-Wclobbered], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wempty-body], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wfloat-equal], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wformat-nonliteral], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wformat-security], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wignored-qualifiers], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wlogical-op], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wmissing-field-initializers], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wmissing-parameter-type], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wold-style-declaration], [WARNING_CFLAGS])
    CFLAGS_ADD([-Woverride-init], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wsign-compare], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wstrict-aliasing], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wstrict-overflow=5], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wtype-limits], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wunused-but-set-parameter], [WARNING_CFLAGS])
    CFLAGS_ADD([-Wunused-parameter], [WARNING_CFLAGS])
else
    AC_MSG_RESULT([no])
fi

AC_CONFIG_FILES([Makefile statusnotifier.pc example/Makefile
                 docs/reference/Makefile docs/reference/version.xml])
AC_OUTPUT
echo "
    ${PACKAGE} version ${PACKAGE_VERSION}${gitver}

 Build information:
   source code location     : ${srcdir}
   prefix                   : ${prefix}
   compiler warning flags   : ${WARNING_CFLAGS}

   build html documentation : ${enable_gtk_doc}
   example                  : ${enable_example}
   dbusmenu                 : ${dbusmenu}
   introspection            : ${enable_introspection}

 Install paths:
   binaries                 : $(eval echo $(eval echo ${bindir}))
   libraries                : $(eval echo $(eval echo ${libdir}))
   documentation            : $(eval echo $(eval echo ${docdir}))
   man pages                : $(eval echo $(eval echo ${mandir}))
"

