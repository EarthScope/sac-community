
zlib=""
AC_SUBST([zlib_LIBS])
AC_SUBST([zlib_CFLAGS])

# Function to Turn On/Off Zlib
AC_DEFUN([ZLIB_OFF], [ 
             zlib_LIBS="" 
             zlib_CFLAGS="" 
             zlib=""
            ])

AC_DEFUN([ZLIB_ON_LIBS], [
             zlib_LIBS="-lz"
             zlib_CFLAGS=""
             ])
AC_DEFUN([ZLIB_ON], [
             ZLIB="-lz"
             AC_DEFINE([HAVE_ZLIB], [1], [Compile with zlib compression])
             zlib="zlib "
             ])


# Simple non pkg-config library 
AC_DEFUN([ZLIB_TRY],
         [ 
         PKG_CHECK_MODULES(zlib, zlib, [ZLIB_ON], [ZLIB_OFF])
         case "$zlib_support" in 
         off)
           AC_CHECK_LIB(z, deflate, [ 
           ZLIB_ON 
           ZLIB_ON_LIBS
           ], [ ZLIB_OFF ]) 
         ;;
         esac         
         ])

AC_DEFUN([CHECK_ZLIB],[
AC_ARG_ENABLE(zlib, AS_HELP_STRING([--enable-zlib], [enable compressed pdf files]),
                   [ AS_IF( [test x$enableval != xno ],
                            [ ZLIB_TRY ],
                            [ ZLIB_OFF ] ) ],
                            [ ZLIB_TRY ] )
   ])