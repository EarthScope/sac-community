
# Debug
AC_SUBST([DEBUG])
AC_DEFUN([DEBUG_OFF], [ DEBUG="" ])
AC_DEFUN([CHECK_DEBUG], [
AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug],[enable debugging options]), 
        [ AS_IF( [ test x$enableval != xno ], [  DEBUG="-g -Wall -Wextra" ], [  DEBUG_OFF ] ) ], 
        [ DEBUG_OFF ] 
)
])

# Optimization
AC_SUBST([OPTIM])
AC_DEFUN([OPTIM_OFF], [ OPTIM="" ])
AC_DEFUN([CHECK_OPTIMIZATION], [
AC_ARG_ENABLE(optim, AS_HELP_STRING([--enable-optim=level],[enable optimization level]),    
    [ if test x$enableval != xno; then
         OPTIM=-O${enableval}
      else
         OPTIM_OFF
      fi
    ], [ OPTIM_OFF ] 
)
])

