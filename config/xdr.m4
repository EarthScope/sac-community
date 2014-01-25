
# XDR File Format, Sun-RPC, librpc
librpc=off
AC_DEFUN([CHECK_LIBRPC],
        [
           CFLAGS="$CFLAGS -I/usr/include/tirpc"
           CPPFLAGS="$CFLGAS -I/usr/include/tirpc"
           AC_CHECK_HEADERS([ rpc/rpc.h ] )
           AC_SEARCH_LIBS(xdr_bool, rpc, [], [ 
              AC_MSG_ERROR([cannot find function xdr_bool(), missing Sun-RPC functions])])
        ])
AC_DEFUN([LIBRPC_ON],
         [ AC_DEFINE([HAVE_LIBRPC], [1], [Compile with XDR file format]) librpc=on ])
AC_DEFUN([LIBRPC_OFF],
         [ AC_DEFINE([HAVE_LIBRPC_DISABLED], [1], [Compile without XDR file format]) librpc=off])

AC_DEFUN([CHECK_XDR], [
     AC_ARG_ENABLE(xdr, AS_HELP_STRING([--enable-xdr],[Enable XDR File Format]),
                     [ AS_IF( [ test x$enableval != xyes ], [ LIBRPC_OFF ], [ LIBRPC_ON ] ) ],
                     [ LIBRPC_OFF ])
     AS_IF( [ test x${librpc} = xon ], [ CHECK_LIBRPC ] )
])



