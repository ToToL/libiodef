if HAVE_EASY_BINDINGS
if HAVE_PYTHON

swigdatadir = @SWIG_DATA_DIR@/python
dist_swigdata_DATA = libiodefcpp-python.i

EXTRA_DIST = libiodefcpp-python.i iodef.py _iodef.cxx pystrings.swg

all-am: python-build

python-build: _iodef.cxx
if HAVE_PYTHON2
	CC="$(CXX)" $(PYTHON2) setup.py build
endif
if HAVE_PYTHON3
	CC="$(CXX)" $(PYTHON3) setup.py build
endif
install-exec-hook:
if HAVE_PYTHON2
	$(PYTHON2) setup.py install `test -n "$(DESTDIR)" && echo --root $(DESTDIR)`
endif
if HAVE_PYTHON3
	$(PYTHON3) setup.py install `test -n "$(DESTDIR)" && echo --root $(DESTDIR)`
endif

uninstall-hook:
if HAVE_PYTHON2
	$(PYTHON2) setup.py uninstall `test -n "$(DESTDIR)" && echo --root $(DESTDIR)`
endif
if HAVE_PYTHON3
	$(PYTHON3) setup.py uninstall `test -n "$(DESTDIR)" && echo --root $(DESTDIR)`
endif

clean-local:
if HAVE_PYTHON2
	$(PYTHON2) setup.py clean -a
endif
if HAVE_PYTHON3
	$(PYTHON3) setup.py clean -a
endif

_iodef.cxx iodef.py: $(top_srcdir)/bindings/c++/include/*.hxx $(top_srcdir)/bindings/libiodefcpp.i libiodefcpp-python.i 
if HAVE_SWIG
	$(SWIG) -c++ -DSWIG_COMPILE_LIBIODEF -I$(top_srcdir)/bindings -I$(top_srcdir)/bindings/c++/include -I$(top_srcdir)/src/include -I$(top_srcdir)/src/libiodef-error -python -threads -builtin -O -o $@ $(top_srcdir)/bindings/libiodefcpp.i
endif

endif
endif

-include $(top_srcdir)/git.mk
