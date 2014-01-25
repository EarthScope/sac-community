
xpm_support=""
AC_SUBST([XPM])
AC_DEFUN([XPM_OFF], [
             xpm_LIBS=""
             xpm_CFLAGS=""
             xpm=""
           ])
AC_DEFUN([XPM_ON_LIBS], [ 
         xpm_LIBS="-lXpm"
         xpm_CFLAGS=""
         ])

AC_DEFUN([XPM_ON], [
              AC_DEFINE([HAVE_XPM], [1], [Compile with XPM File Support])
              xpm="xpm "
             ])

AC_DEFUN([XPM_TRY],
     [
     if test x${display} = xx11; then
        PKG_CHECK_MODULES(xpm, xpm, [ XPM_ON ], [ XPM_OFF ])
        case x"$xpm" in
        x)
          AC_CHECK_LIB(Xpm, XpmWriteFileFromPixmap, [ 
            XPM_ON
            XPM_ON_LIBS 
          ], [ XPM_OFF ], [ ${X_LIBS} ] ) 
        ;;
        esac
     fi
     ]
)

AC_DEFUN([CHECK_XPM],[
AC_ARG_ENABLE(xpm, AS_HELP_STRING([--enable-xpm], [enable saving to XPM files]),
                   [ AS_IF( [test x$enableval != xno ],
                            [ XPM_TRY ],
                            [ XPM_OFF ] ) ],
                            [ XPM_TRY ] )
     ])



