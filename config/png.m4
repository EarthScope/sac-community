
png=""
AC_SUBST([PNG])
AC_SUBST([png_CFLAGS])
AC_SUBST([png_LIBS])
AC_DEFUN([PNG_OFF], [ 
             png_CFLAGS="" 
             png_LIBS="" 
             png=""
            ])
AC_DEFUN([PNG_ON_LIBS], [
             png_LIBS="-lpng"
             png_CFLAGS=""
])
AC_DEFUN([PNG_ON], [
             AC_DEFINE([HAVE_PNG], [1], [Compile with PNG File Support])
             png="png "
             ])

AC_DEFUN([PNG_TRY],
        [ 
        PKG_CHECK_MODULES(png, libpng, [ PNG_ON ], [ PNG_OFF ])
        case x"$png" in
        x)
            AC_CHECK_LIB(png, png_write_image, [ 
            PNG_ON 
            PNG_ON_LIBS
            ], [ PNG_OFF ] ) 
        ;;
        esac
        ])

AC_DEFUN([CHECK_PNG],[
AC_ARG_ENABLE(png, AS_HELP_STRING([--enable-png], [enable saving to png files]),
                   [ AS_IF( [test x$enableval != xno ],
                            [ PNG_TRY ],
                            [ PNG_OFF ] ) ],
                            [ PNG_OFF ] )
   ])