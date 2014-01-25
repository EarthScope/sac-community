
AC_DEFUN([CHECK_WIN_APP],
        [
        AC_ARG_ENABLE(win-app, AS_HELP_STRING([--enable-win-app], [enable build of WIN Applications]),
        [AS_IF( [ test x$enabelval != xno ], [WINAPP_ON ] ) ] )
])
