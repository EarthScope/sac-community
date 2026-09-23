
# Debug
AC_SUBST([DEBUG])
AC_DEFUN([DEBUG_OFF], [ DEBUG="" ])
AC_DEFUN([CHECK_DEBUG], [
AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug],[enable debugging options]), 
        [ AS_IF( [ test x$enableval != xno ], [  DEBUG=" -g " ], [  DEBUG_OFF ] ) ], 
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

# Disable FP contraction (e.g. FMA) so optimized builds round the same as
# unoptimized ones; filter and FFT results depend on this.
AC_SUBST([FP_CONTRACT])
AC_DEFUN([CHECK_FP_CONTRACT], [
AC_MSG_CHECKING([whether $CC accepts -ffp-contract=off])
save_CFLAGS=$CFLAGS
CFLAGS="$CFLAGS -ffp-contract=off"
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[int main(void) { return 0; }]])],
    [ FP_CONTRACT=-ffp-contract=off; AC_MSG_RESULT([yes]) ],
    [ FP_CONTRACT=""; AC_MSG_RESULT([no]) ]
)
CFLAGS=$save_CFLAGS
])

