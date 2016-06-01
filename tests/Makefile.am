TESTS = async-timer iodef iodef-criteria iodef-document-helper iodef-path iodef-value libiodef-client libiodef-string libiodef-timer
check_PROGRAMS = $(TESTS)
LDADD = $(top_builddir)/src/libiodef.la ../libmissing/libmissing.la
AM_CPPFLAGS = -I$(top_builddir)/src/include -I$(top_srcdir)/src/include -I$(top_builddir)/src/libiodef-error -I$(top_builddir)/libmissing -I$(top_srcdir)/libmissing

iodef_value_LDADD = $(top_builddir)/src/iodef-value.lo $(LDADD)
async_timer_LDADD = @LTLIBMULTITHREAD@ $(LDADD)

if HAVE_VALGRIND

check-local:
	$(MAKE) valgrind


%.valgrind: %
	$(LIBTOOL) --mode=execute                               \
	$(VALGRIND) -q                                          \
	--tool=memcheck --leak-check=full --trace-children=yes  \
	--leak-resolution=high --num-callers=20                 \
	$* 2>&1 | tee valgrind.log				
	@if grep "==" valgrind.log > /dev/null 2>&1; then       \
		rm valgrind.log;                                \
		exit 1;                                         \
	fi
	@rm valgrind.log

valgrind: $(TESTS)
	@echo "Valgrinding tests ..."
	@failed=0;                                                      \
	for t in $(TESTS); do   					\
		$(MAKE) $$t.valgrind;                                   \
		if test "$$?" -ne 0; then                               \
			echo "Valgrind error for test $$t";             \
			failed=`expr $$failed + 1`;                     \
			whicht="$$whicht $$t";                          \
		fi;                                                     \
	done;                                                           \
	if test "$$failed" -ne 0; then                                  \
		echo "$$failed tests had leaks or errors under valgrind:";      \
		echo "$$whicht";                                        \
		false;                                                  \
	fi
else
valgrind:
endif

-include $(top_srcdir)/git.mk
