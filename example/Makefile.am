
bin_PROGRAMS = sn-example

AM_CPPFLAGS = -I$(top_srcdir)/src

sn_example_CFLAGS = ${AM_CFLAGS} @GTK_CFLAGS@
sn_example_LDADD = $(top_builddir)/.libs/libstatusnotifier.la @GTK_LIBS@
sn_example_SOURCES = sn-example.c

