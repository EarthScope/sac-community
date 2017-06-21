
# OSX Application

AC_DEFUN([CHECK_FRAMEWORK],
  [AC_CACHE_CHECK([if -framework $1 works],[my_cv_framework_$1],
     [save_LIBS="$LIBS"
     LIBS="$LIBS -framework $1"
     AC_LINK_IFELSE([AC_LANG_PROGRAM([],[])],
             [my_cv_framework_$1=yes],
            [my_cv_framework_$1=no])
     LIBS="$save_LIBS"
    ])
   if test "$my_cv_framework_$1"="yes"; then
       AC_DEFINE(AS_TR_CPP([HAVE_FRAMEWORK_$1]),1,
            [Define if you have the  $1 framework])
   AS_TR_CPP([FRAMEWORK_$1])="-framework $1"
   AC_SUBST(AS_TR_CPP([FRAMEWORK_$1]))
   fi])



AC_DEFUN([OSXAPP_ON], [ AC_DEFINE([OSX_APP], [1], [Create OSX Application]) 
                      display=osx
                      editline=on
                      readline=on
                      case "${host}" in 
                           *darwin10* ) ;; # 10.6
                           *darwin11* ) ;; # 10.7
                           *darwin12* ) ;; # 10.8
                           *darwin13* ) ;; # 10.9
                           *darwin14* ) ;; # 10.10
                           *darwin15* ) ;; # 10.11
                           *darwin16* ) ;; # 10.12
                           *darwin17* ) ;; # 10.13
                           * ) AC_MSG_ERROR("Building OSX App requires OSX version 10.6 or higher")
                      esac
                      DISPATCH_C_BLOCKS
                      CHECK_FRAMEWORK([Foundation])
                      CHECK_FRAMEWORK([Cocoa])
                      AC_SUBST([SACAPP_OSX])
                      AC_SUBST([SACAPP_RSC])
                      SACAPP_OSX="Sac.app/Contents/MacOS/"
                      SACAPP_RSC="Sac.app/Contents/Resources/"
                      AM_CFLAGS="$AM_CFLAGS -isysroot /Developer/SDKs/MacOSX10.6.sdk"
                      AM_CFLAGS="$AM_CFLAGS -mmacosx-version-min=10.6"
                      AM_OBJCFLAGS="$AM_OBJCFLAGS -isysroot /Developer/SDKs/MacOSX10.6.sdk"
                      AM_OBJCFLAGS="$AM_OBJCFLAGS -mmacosx-version-min=10.6"
         ])
AC_DEFUN([CHECK_OSX_APP],
         [
          AC_ARG_ENABLE(osx-app, AS_HELP_STRING([--enable-osx-app], [enable build of OSX Application]),
                      [ AS_IF( [ test x$enableval != xno ], [ OSXAPP_ON ] ) ] )
         ]) 


AC_DEFUN([DISPATCH_C_BLOCKS], [
#
# Detect compiler support for Blocks; perhaps someday -fblocks won't be
# required, in which case we'll need to change this.
#
AC_CACHE_CHECK([for C Blocks support], [dispatch_cv_cblocks], [
  AC_LANG_PUSH([Objective C])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],[(void)^{int i; i = 0; }();])], [
  ], [
    AC_MSG_ERROR([
    ********************************************************
       C Blocks not supported with current compiler: $OBJC
       Try the llvm or clang compiler
       ./configure --enable-osx-app CC=clang OBJC=clang CXX=clang
    ********************************************************
    ])
  ])
  AC_LANG_POP([Objective C])
])
])
