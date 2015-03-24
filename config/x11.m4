
# X11 Path and Libaries

AC_DEFUN([CHECK_X11_DISPLAY],
    [
    if test x${display} = xx11; then
       no_x=
       AC_PATH_XTRA
       if test x"$no_x" == xyes ; then
           AC_MSG_ERROR([Cannot find X11 Developer Libraries and Include files, please install them])
       fi
       CHECK_X11_COLOR_DEPTH
       AC_DEFINE([X11_APP], [1], [ Create X11 Application ])
    fi
    ])


# X11 Multiple Color Depths
x11_depth=off
AC_DEFUN([X11_DEPTH_ON], 
         [ AC_DEFINE([USE_X11_MULTIPLE_DEPTHS], 
                     [1], 
                     [Compile with X11 multiple color depths]) 
             x11_depth=on ] )
AC_DEFUN([X11_DEPTH_OFF], 
         [ AC_DEFINE([USE_X11_MULTIPLE_DEPTHS_DISABLED], 
                     [1], 
                     [Compile without X11 multiple color depth])
             x11_depth=off ] )
AC_DEFUN([CHECK_X11_COLOR_DEPTH],
        [
        AC_ARG_ENABLE(x11_depth, AS_HELP_STRING([--disable-x11-depth],[disable X11 multiple color depth]),  
            [ AS_IF( [ test x$enableval != xyes ], [ X11_DEPTH_OFF ], [ X11_DEPTH_ON ] ) ],
            [ X11_DEPTH_ON ] 
        )
        ])


