
editing=on

AC_DEFUN([EDITING_DISABLED], [
    editing=off
])

AC_DEFUN([CHECK_CMD_EDITING], [
AC_ARG_ENABLE(editing, AS_HELP_STRING([--disable-editing],[disable command line editing]),
    [ AS_IF( [ test x$enableval == xyes ], [
       editing=on
    ],[ EDITING_DISABLED ]) ],
    [ test ]
)
])

AC_DEFUN([CHECK_EDITING], [
    CHECK_CMD_EDITING
    AS_IF([test x${editing} = xon],[
       AC_DEFINE([READLINE], [1], [Compile with command line editing capabilities])
    ])
])
