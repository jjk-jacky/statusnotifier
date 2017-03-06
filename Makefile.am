
CLEANFILES =

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
include_HEADERS = src/statusnotifier.h src/statusnotifier-compat.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = statusnotifier.pc

libstatusnotifier_la_LDFLAGS = -version-info $(LIB_VERSION_INFO)
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
	src/mkenums \
	m4/introspection.m4

src/enums.h: src/statusnotifier.h
	$(AM_V_GEN) cd $(top_srcdir)/src && ./mkenums

src/enums.c: src/statusnotifier.h
	$(AM_V_GEN) cd $(top_srcdir)/src && ./mkenums


if HAVE_INTROSPECTION
BUILT_GIRSOURCES = StatusNotifier-$(GIR_VERSION).gir

GIR_EXTRA =
if USE_DBUSMENU
GIR_EXTRA += -DUSE_DBUSMENU=1 --include=Gtk-3.0
endif
StatusNotifier-$(GIR_VERSION).gir: $(INTROSPECTION_SCANNER) libstatusnotifier.la
	$(AM_V_GEN) $(INTROSPECTION_SCANNER) \
		-v --warn-all -n StatusNotifier --nsversion=$(GIR_VERSION) \
		--include=GObject-2.0 --include=GdkPixbuf-2.0 \
		$(INCLUDES) $(GIR_EXTRA) \
		--library=statusnotifier -o $@ \
		src/statusnotifier.[ch] src/enums.[ch]

girdir = $(datadir)/gir-1.0
gir_DATA = $(BUILT_GIRSOURCES)

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = $(BUILT_GIRSOURCES:.gir=.typelib)

%.typelib: %.gir $(INTROSPECTION_COMPILER)
	$(AM_V_GEN) $(INTROSPECTION_COMPILER) \
		--includedir=$(srcdir) --includedir=. $(INTROSPECTION_COMPILER_OPTS) \
		$< -o $(@F)

CLEANFILES += $(BUILT_GIRSOURCES) $(typelibs_DATA)
endif
