if HAVE_EASY_BINDINGS
if HAVE_RUBY

swigdatadir = @SWIG_DATA_DIR@/ruby
dist_swigdata_DATA = libiodefcpp-ruby.i

EXTRA_DIST = libiodefcpp-ruby.i Prelude.cxx

rbexec_LTLIBRARIES = Prelude.la
Prelude_la_CPPFLAGS = $(RUBY_INCLUDES) $(RUBY_CCFLAGS) -I@top_builddir@ -I$(top_srcdir)/src/include -I$(top_builddir)/src/include -I@top_builddir@/src/libiodef-error -I$(top_srcdir)/bindings/c++/include
Prelude_la_LDFLAGS = -module -avoid-version
Prelude_la_LIBADD = $(top_builddir)/bindings/c++/.libs/libiodefcpp.la $(RUBY_LIBS)
nodist_Prelude_la_SOURCES = Prelude.cxx

Prelude.cxx: $(top_srcdir)/bindings/c++/include/*.hxx $(top_srcdir)/bindings/libiodefcpp.i libiodefcpp-ruby.i 
if HAVE_SWIG
	$(SWIG) -c++ -DSWIG_COMPILE_LIBPRELUDE -I$(top_srcdir)/bindings -I$(top_srcdir)/bindings/c++/include -I$(top_srcdir)/src/include -I$(top_srcdir)/src/libiodef-error -ruby -o $@ $(top_srcdir)/bindings/libiodefcpp.i
endif

endif
endif

-include $(top_srcdir)/git.mk
