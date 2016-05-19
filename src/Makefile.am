SUBDIRS = libiodef-error include

AM_CPPFLAGS = @PCFLAGS@ -I$(srcdir)/include -I$(top_builddir) -I$(top_builddir)/src/include -I$(srcdir)/libiodef-error \
-I$(top_builddir)/libmissing -I$(top_srcdir)/libmissing $(LTDLINCL) @LIBIODEF_CFLAGS@ @LIBGCRYPT_CFLAGS@ 

pkgconfigdir = $(libdir)/pkgconfig

libiodefincludedir = $(includedir)/
libiodef_la_LIBADD = @LIBIODEF_LIBS@ $(LIBLTDL) $(top_builddir)/libmissing/libmissing.la libiodef-error/libiodef-error.la
libiodef_la_LDFLAGS = $(GETADDRINFO_LIB) $(GETHOSTNAME_LIB) $(HOSTENT_LIB) $(INET_NTOP_LIB) $(INET_PTON_LIB) $(LIBSOCKET) $(LIB_POLL) $(LIB_SELECT) $(LTLIBINTL) $(LTLIBTHREAD) $(SERVENT_LIB) -no-undefined -version-info @LIBIODEF_SONAME@ -export-symbols-regex "^(iodef|_?prelude)_"
lib_LTLIBRARIES = libiodef.la
BUILT_SOURCES = $(LTDLDEPS) 

AM_YFLAGS = -d
LEX_OUTPUT_ROOT = lex.yy

libiodef_la_SOURCES = 		\
	common.c			\
	ntp.c				\
	variable.c			\
	prelude-hash.c			\
	prelude-ident.c			\
	prelude-io.c			\
	prelude-log.c			\
	prelude-string.c		\
	iodef-additional-data.c		\
	iodef-class.c			\
	iodef-criteria.c		\
	iodef-criteria-string.yac.y	\
	iodef-criteria-string.lex.l	\
	iodef-criterion-value.c		\
	iodef-data.c			\
	iodef-document-helpers.c		\
	iodef-document-read-json.c		\
	iodef-document-print.c		\
	iodef-document-print-json.c		\
	iodef-document-write.c		\
	iodef-object.c			\
	iodef-path.c			\
	iodef-time.c			\
	iodef-tree-wrap.c		\
	iodef-value.c			\
	iodef-value-type.c		

-include $(top_srcdir)/git.mk
