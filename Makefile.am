
SUBDIRS = . docs/reference
if EXAMPLE
SUBDIRS += example
endif

ACLOCAL_AMFLAGS = -I m4

if USE_GIT_VERSION
_VERSION = `git describe --abbrev=4 --dirty`
DEFS += -DGIT_VERSION=\"$(_VERSION)\"
else
_VERSION = $(PACKAGE_VERSION)
endif

AM_CFLAGS = \
	-g \
	-DDOCDIR='"$(docdir)"' \
	${WARNING_CFLAGS}

lib_LTLIBRARIES = libstatusnotifier.la
include_HEADERS = src/statusnotifier.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = statusnotifier.pc

libstatusnotifier_la_CFLAGS = ${AM_CFLAGS} @DEP_CFLAGS@
libstatusnotifier_la_LIBADD = @DEP_LIBS@
libstatusnotifier_la_SOURCES = \
	src/enums.h \
	src/enums.c \
	src/statusnotifier.h \
	src/statusnotifier.c \
	src/closures.h \
	src/closures.c \
	src/interfaces.h

EXTRA_DIST = \
	src/closures \
	src/closures.def \
	src/mkenums

src/enums.h: src/statusnotifier.h
	$(AM_V_GEN) cd $(top_srcdir)/src && ./mkenums

src/enums.c: src/statusnotifier.h
	$(AM_V_GEN) cd $(top_srcdir)/src && ./mkenums

