
xft=""
AC_DEFUN([XFT_ON],
         AC_DEFINE([HAVE_XFT], [1], [Compile with XFT File Support])
         xft="xft "
)
AC_DEFUN([XFT_OFF],
        xft=""
        )

AC_DEFUN([XFT_TRY], 
    [ 
    if test x${display} = xx11; then
       PKG_CHECK_MODULES(xft, xrender freetype2 fontconfig xft, [ XFT_ON ], [XFT_OFF])
    fi
    ]
)


AC_DEFUN([CHECK_XFT], [
      AC_ARG_ENABLE(xft, AS_HELP_STRING([--enable-xft], [enable XFT Fonts for X11]),
                   [ AS_IF( [test x$enableval != xno ],
                            [ XFT_TRY ],
                            [ XFT_OFF ] ) ],
                            [ XFT_OFF ] )
       ])


