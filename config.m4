dnl $Id$
dnl config.m4 for extension instache

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(instache, for instache support,
dnl Make sure that the comment is aligned:
dnl [  --with-instache             Include instache support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(instache, whether to enable instache support,
[  --enable-instache           Enable instache support])

if test "$PHP_INSTACHE" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-instache -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/instache.h"  # you most likely want to change this
  dnl if test -r $PHP_INSTACHE/$SEARCH_FOR; then # path given as parameter
  dnl   INSTACHE_DIR=$PHP_INSTACHE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for instache files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       INSTACHE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$INSTACHE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the instache distribution])
  dnl fi

  dnl # --with-instache -> add include path
  dnl PHP_ADD_INCLUDE($INSTACHE_DIR/include)

  dnl # --with-instache -> check for lib and symbol presence
  dnl LIBNAME=instache # you may want to change this
  dnl LIBSYMBOL=instache # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $INSTACHE_DIR/lib, INSTACHE_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_INSTACHELIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong instache lib version or lib not found])
  dnl ],[
  dnl   -L$INSTACHE_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(INSTACHE_SHARED_LIBADD)

  PHP_NEW_EXTENSION(instache, instache.c liteutil.c hashfunc.c,$ext_shared)
fi
