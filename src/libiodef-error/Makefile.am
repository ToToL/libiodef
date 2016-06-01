includedir = $(prefix)/include/libiodef
AM_CPPFLAGS = @PCFLAGS@ -I$(top_srcdir)/libmissing -I$(top_srcdir)/src/include -I$(top_builddir)/src/include

EXTRA_DIST = mkstrtable.awk err-sources.h.in err-codes.h.in \
	mkerrnos.awk errnos.in mkerrcodes.awk mkerrcodes1.awk mkerrcodes2.awk mkerrcodes.c \
	mkheader.awk libiodef-error.h.in err-sources.h err-codes.h

BUILT_SOURCES = err-sources.h err-codes.h code-from-errno.h code-to-errno.h err-sources-sym.h err-codes-sym.h errnos-sym.h libiodef-error.h 

CLEANFILES = err-sources.h err-codes.h code-to-errno.h code-from-errno.h \
        libiodef-error.h mkerrcodes mkerrcodes.h \
        err-sources-sym.h err-codes-sym.h errnos-sym.h

nodist_include_HEADERS = libiodef-error.h

noinst_LTLIBRARIES = libiodef-error.la
libiodef_error_la_LIBADD  = @LTLIBTHREAD@
libiodef_error_la_LDFLAGS = -no-undefined
libiodef_error_la_SOURCES = libiodef-error.h libiodef-error.c strsource.c strerror.c

err-sources.h: Makefile mkstrtable.awk err-sources.h.in
	LANG="" $(AWK) -f $(srcdir)/mkstrtable.awk -v textidx=3 \
		$(srcdir)/err-sources.h.in >$@

err-sources-sym.h: Makefile mkstrtable.awk err-sources.h.in
	LANG="" $(AWK) -f $(srcdir)/mkstrtable.awk -v textidx=2 -v nogettext=1 \
	$(srcdir)/err-sources.h.in >$@

err-codes.h: Makefile mkstrtable.awk err-codes.h.in
	LANG="" $(AWK) -f $(srcdir)/mkstrtable.awk -v textidx=3 \
	$(srcdir)/err-codes.h.in >$@

err-codes-sym.h: Makefile mkstrtable.awk err-codes.h.in
	LANG="" $(AWK) -f $(srcdir)/mkstrtable.awk -v textidx=2 -v nogettext=1 \
		$(srcdir)/err-codes.h.in >$@

code-to-errno.h: Makefile mkerrnos.awk errnos.in
	LANG="" $(AWK) -f $(srcdir)/mkerrnos.awk $(srcdir)/errnos.in >$@


mkerrcodes.h: Makefile mkerrcodes.awk
	LANG="" $(AWK) -f $(srcdir)/mkerrcodes1.awk $(srcdir)/errnos.in >_$@
	$(CPP) -P _$@ | grep LIBIODEF_ERROR_ | LANG="" $(AWK) -f $(srcdir)/mkerrcodes.awk >$@
	-rm _$@

# It is correct to use $(CC_FOR_BUILD) here.  We want to run the
# program at build time.
mkerrcodes: mkerrcodes.c mkerrcodes.h Makefile
	$(CC_FOR_BUILD) -I. -I$(srcdir) -o $@ $(srcdir)/mkerrcodes.c

code-from-errno.h: mkerrcodes Makefile
	./mkerrcodes | LANG="" $(AWK) -f $(srcdir)/mkerrcodes2.awk >$@

errnos-sym.h: Makefile mkstrtable.awk errnos.in
	LANG="" $(AWK) -f $(srcdir)/mkstrtable.awk -v textidx=2 -v nogettext=1 \
	-v prefix=LIBIODEF_ERROR_ -v namespace=errnos_ \
	$(srcdir)/errnos.in >$@

libiodef-error.h: Makefile mkheader.awk err-sources.h.in err-codes.h.in errnos.in libiodef-error.h.in
	LANG="" $(AWK) -f $(srcdir)/mkheader.awk \
		$(srcdir)/err-sources.h.in \
		$(srcdir)/err-codes.h.in \
		$(srcdir)/errnos.in \
		$(srcdir)/libiodef-error.h.in > $@


-include $(top_srcdir)/git.mk
