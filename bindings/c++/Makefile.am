if HAVE_EASY_BINDINGS

SUBDIRS = include

lib_LTLIBRARIES = libiodefcpp.la
libiodefcpp_ladir = ${prefix}/lib
libiodefcpp_la_SOURCES = \
	prelude-error.cxx \
	iodef.cxx \
	iodef-class.cxx \
	iodef-criteria.cxx \
	iodef-path.cxx \
	iodef-time.cxx \
	iodef-value.cxx

libiodefcpp_la_LDFLAGS = -no-undefined -version-info @LIBIODEFCPP_SONAME@
libiodefcpp_la_LIBADD  = $(top_builddir)/src/.libs/libiodef.la
libiodefcpp_la_CPPFLAGS = -I$(top_builddir) -I$(top_builddir)/src/include -I$(top_srcdir)/src/include -I$(top_builddir)/src/libiodef-error -I$(top_builddir)/bindings/c++/include -I$(top_srcdir)/bindings/c++/include 

endif

-include $(top_srcdir)/git.mk
