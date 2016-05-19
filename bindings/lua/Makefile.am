if HAVE_EASY_BINDINGS

EXTRA_DIST = libiodefcpp-lua.i prelude.cxx

if HAVE_LUA

swigdatadir = @SWIG_DATA_DIR@/lua
dist_swigdata_DATA = libiodefcpp-lua.i

AM_CPPFLAGS = -I@top_srcdir@ -I@top_builddir@/src/include -I@top_srcdir@/src/include -I@top_builddir@/src/libiodef-error -I@top_srcdir@/bindings/c++/include @LUA_CFLAGS@ -I@top_srcdir@/libmissing -I@top_builddir@/libmissing

prelude_la_LDFLAGS = -module -avoid-version @LUA_LIBS@
prelude_la_LIBADD = $(top_builddir)/bindings/c++/.libs/libiodefcpp.la
prelude_la_SOURCES = prelude.cxx
preludedir = $(libdir)/lua/@LUA_VERSION@
prelude_LTLIBRARIES = prelude.la

prelude.cxx: $(top_srcdir)/bindings/c++/include/*.hxx $(top_srcdir)/bindings/libiodefcpp.i libiodefcpp-lua.i 
if HAVE_SWIG
	$(SWIG) -c++ -DSWIG_COMPILE_LIBPRELUDE -I$(top_srcdir)/bindings -I$(top_srcdir)/bindings/c++/include -I$(top_srcdir)/src/include -I$(top_srcdir)/src/libiodef-error -lua -o $@ $(top_srcdir)/bindings/libiodefcpp.i
endif


endif

endif

-include $(top_srcdir)/git.mk
