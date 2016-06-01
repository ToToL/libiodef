dnl Autoconf macros for libiodef
dnl $id$

# Modified for LIBIODEF -- Yoann Vandoorselaere
# Modified for LIBGNUTLS -- nmav
# Configure paths for LIBGCRYPT
# Shamelessly stolen from the one of XDELTA by Owen Taylor
# Werner Koch   99-12-09

dnl AM_PATH_LIBIODEF([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND ]]], THREAD_SUPPORT)
dnl Test for libiodef, and define LIBIODEF_PREFIX, LIBIODEF_CFLAGS, LIBIODEF_PTHREAD_CFLAGS,
dnl LIBIODEF_LDFLAGS, and LIBIODEF_LIBS
dnl
AC_DEFUN([AM_PATH_LIBIODEF],
[dnl
dnl Get the cflags and libraries from the libiodef-config script
dnl
AC_ARG_WITH(libiodef-prefix, AC_HELP_STRING(--with-libiodef-prefix=PFX,
            Prefix where libiodef is installed (optional)),
            libiodef_config_prefix="$withval", libiodef_config_prefix="")

  if test x$libiodef_config_prefix != x ; then
     if test x${LIBIODEF_CONFIG+set} != xset ; then
        LIBIODEF_CONFIG=$libiodef_config_prefix/bin/libiodef-config
     fi
  fi

  AC_PATH_PROG(LIBIODEF_CONFIG, libiodef-config, no)
  if test "$LIBIODEF_CONFIG" != "no"; then
  	if $($LIBIODEF_CONFIG --thread > /dev/null 2>&1); then
        	LIBIODEF_PTHREAD_CFLAGS=`$LIBIODEF_CONFIG --thread --cflags`

        	if test x$4 = xtrue || test x$4 = xyes; then
                	libiodef_config_args="--thread"
        	else
                	libiodef_config_args="--no-thread"
        	fi
  	else
        	LIBIODEF_PTHREAD_CFLAGS=`$LIBIODEF_CONFIG --pthread-cflags`
  	fi
  fi

  min_libiodef_version=ifelse([$1], ,0.1.0,$1)
  AC_MSG_CHECKING(for libiodef - version >= $min_libiodef_version)
  no_libiodef=""
  if test "$LIBIODEF_CONFIG" = "no" ; then
    no_libiodef=yes
  else
    LIBIODEF_CFLAGS=`$LIBIODEF_CONFIG $libiodef_config_args --cflags`
    LIBIODEF_LDFLAGS=`$LIBIODEF_CONFIG $libiodef_config_args --ldflags`
    LIBIODEF_LIBS=`$LIBIODEF_CONFIG $libiodef_config_args --libs`
    LIBIODEF_PREFIX=`$LIBIODEF_CONFIG $libiodef_config_args --prefix`
    LIBIODEF_CONFIG_PREFIX=`$LIBIODEF_CONFIG $libiodef_config_args --config-prefix`
    libiodef_config_version=`$LIBIODEF_CONFIG $libiodef_config_args --version`


      ac_save_CFLAGS="$CFLAGS"
      ac_save_LDFLAGS="$LDFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $LIBIODEF_CFLAGS"
      LDFLAGS="$LDFLAGS $LIBIODEF_LDFLAGS"
      LIBS="$LIBS $LIBIODEF_LIBS"
dnl
dnl Now check if the installed libiodef is sufficiently new. Also sanity
dnl checks the results of libiodef-config to some extent
dnl
      rm -f conf.libiodeftest
      AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libiodef/libiodef.h>

int
main ()
{
    system ("touch conf.libiodeftest");

    if( strcmp( libiodef_check_version(NULL), "$libiodef_config_version" ) )
    {
      printf("\n*** 'libiodef-config --version' returned %s, but LIBIODEF (%s)\n",
             "$libiodef_config_version", libiodef_check_version(NULL) );
      printf("*** was found! If libiodef-config was correct, then it is best\n");
      printf("*** to remove the old version of LIBIODEF. You may also be able to fix the error\n");
      printf("*** by modifying your LD_LIBRARY_PATH enviroment variable, or by editing\n");
      printf("*** /etc/ld.so.conf. Make sure you have run ldconfig if that is\n");
      printf("*** required on your system.\n");
      printf("*** If libiodef-config was wrong, set the environment variable LIBIODEF_CONFIG\n");
      printf("*** to point to the correct copy of libiodef-config, and remove the file config.cache\n");
      printf("*** before re-running configure\n");
    }
    else if ( strcmp(libiodef_check_version(NULL), LIBIODEF_VERSION ) ) {
        printf("\n*** LIBIODEF header file (version %s) does not match\n", LIBIODEF_VERSION);
        printf("*** library (version %s)\n", libiodef_check_version(NULL) );
    }
    else {
      if ( libiodef_check_version( "$min_libiodef_version" ) )
        return 0;
      else {
        printf("no\n*** An old version of LIBIODEF (%s) was found.\n",
                libiodef_check_version(NULL) );
        printf("*** You need a version of LIBIODEF newer than %s. The latest version of\n",
               "$min_libiodef_version" );
        printf("*** LIBIODEF is always available from https://www.libiodef-siem.org/project/libiodef/files\n");
        printf("*** \n");
        printf("*** If you have already installed a sufficiently new version, this error\n");
        printf("*** probably means that the wrong copy of the libiodef-config shell script is\n");
        printf("*** being found. The easiest way to fix this is to remove the old version\n");
        printf("*** of LIBIODEF, but you can also set the LIBIODEF_CONFIG environment to point to the\n");
        printf("*** correct copy of libiodef-config. (In this case, you will have to\n");
        printf("*** modify your LD_LIBRARY_PATH enviroment variable, or edit /etc/ld.so.conf\n");
        printf("*** so that the correct libraries are found at run-time))\n");
      }
    }
    return 1;
}
],, no_libiodef=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
       LDFLAGS="$ac_save_LDFLAGS"
  fi

  if test "x$no_libiodef" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])
  else
     if test -f conf.libiodeftest ; then
        :
     else
        AC_MSG_RESULT(no)
     fi
     if test "$LIBIODEF_CONFIG" = "no" ; then
       echo "*** The libiodef-config script installed by LIBIODEF could not be found"
       echo "*** If LIBIODEF was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the LIBIODEF_CONFIG environment variable to the"
       echo "*** full path to libiodef-config."
     else
       if test -f conf.libiodeftest ; then
        :
       else
          echo "*** Could not run libiodef test program, checking why..."
          CFLAGS="$CFLAGS $LIBIODEF_CFLAGS"
          LDFLAGS="$LDFLAGS $LIBIODEF_LDFLAGS"
          LIBS="$LIBS $LIBIODEF_LIBS"
          AC_TRY_LINK([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libiodef/libiodef.h>
],      [ return !!libiodef_check_version(NULL); ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding LIBIODEF or finding the wrong"
          echo "*** version of LIBIODEF. If it is not finding LIBIODEF, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
          echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"
          echo "***" ],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means LIBIODEF was incorrectly installed"
          echo "*** or that you have moved LIBIODEF since it was installed. In the latter case, you"
          echo "*** may want to edit the libiodef-config script: $LIBIODEF_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LDFLAGS="$ac_save_LDFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     LIBIODEF_CFLAGS=""
     LIBIODEF_LDFLAGS=""
     LIBIODEF_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  rm -f conf.libiodeftest
  AC_SUBST(LIBIODEF_CFLAGS)
  AC_SUBST(LIBIODEF_PTHREAD_CFLAGS)
  AC_SUBST(LIBIODEF_LDFLAGS)
  AC_SUBST(LIBIODEF_LIBS)
  AC_SUBST(LIBIODEF_PREFIX)
  AC_SUBST(LIBIODEF_CONFIG_PREFIX)

  m4_ifdef([LT_INIT],
           [AC_DEFINE([LIBIODEF_APPLICATION_USE_LIBTOOL2], [], [Define whether application use libtool >= 2.0])],
           [])

])

dnl *-*wedit:notab*-*  Please keep this as the last line.
