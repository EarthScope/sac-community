
editing=on
editline=off
readline=off
syseditline=on

# Curses Library
AC_SUBST([CURSES_SEARCH_PATH])
curses_library_found=no

# Macro Definitions
AC_DEFUN([NCURSES_ON], [ CURSES_SEARCH_PATH="ncurses curses termcap" ])
AC_DEFUN([NCURSES_OFF], [ CURSES_SEARCH_PATH="curses termcap" ])

AC_DEFUN([DEFINE_READLINE],         [
  AC_DEFINE([READLINE],          [1], [Compile with command line capabilities])
])

AC_DEFUN([DEFINE_READLINE_EDITLINE], [
  AC_DEFINE([READLINE_EDITLINE], [1], [Compile with editline comand line capabilities])
  DEFINE_READLINE
])
AC_DEFUN([DEFINE_READLINE_READLINE], [
  AC_DEFINE([READLINE_READLINE], [1], [Compile with readline comand line capabilities])
  DEFINE_READLINE
])

AC_DEFUN([CHECK_CURSES],[
AC_ARG_ENABLE(ncurses, AS_HELP_STRING([--disable-ncurses],[avoid the ncurses library]),
                       [ AS_IF( [ test x$enableval != xyes ], [ NCURSES_OFF ], [ NCURSES_ON ]) ],
                       [ NCURSES_ON ]
)
])

AC_DEFUN([CURSES], [
    for termlib in ${CURSES_SEARCH_PATH} ; do
        AC_CHECK_LIB(${termlib}, tgoto, [LIBS="-l${termlib} $LIBS"; curses_library_found=yes; break], [ test ])
    done
    if test "$curses_library_found" = no ; then
        AC_MSG_ERROR([Developer Library ncurses, curses, or termcap not found, please install them])
    fi
    AC_DEFINE([USE_TERMIOS],[1],[Compile with termios support])
    ]
)

# Readline (OFF by Default)
AC_DEFUN([CHECK_READLINE], [
AC_ARG_ENABLE(readline, AS_HELP_STRING([--enable-readline],[enable readline command line capabilities]), 
    [ AS_IF( [ test x$enableval == xyes ], [
        readline=on
        editline=off
        syseditline=off
    ] ) ],
    [ test ] 
)
])

# Editline (OFF by Default)
AC_DEFUN([CHECK_EDITLINE], [
AC_ARG_ENABLE(editline, AS_HELP_STRING([--enable-editline],[enable local editline command line capabilities]), 
    [ AS_IF( [ test x$enableval == xyes ], [
      readline=off
      editline=on
      syseditline=off
    ])],
    [ test ]
)
])

AC_DEFUN([CHECK_CMD_EDITING], [
AC_ARG_ENABLE(editline, AS_HELP_STRING([--enable-editing],[enable command line capabilities]), 
    [ AS_IF( [ test x$enableval == xyes ], [
       editing=on
    ],[editing=off]) ],
    [ test ]
)
])

AC_DEFUN([SYSTEM_LIBEDIT], [
   syseditline=off
   AC_CHECK_HEADER([editline/readline.h], [
      AC_SEARCH_LIBS(el_init, [edit], [syseditline=on])
   ])
])

AC_DEFUN([CHECK_EDITING], [
    CHECK_CMD_EDITING
    AS_IF([test x${editing} = xon],[
       CHECK_CURSES
       CURSES
       CHECK_EDITLINE
       CHECK_READLINE
       # Handle the System Editline Library
       AS_IF(
         [test x${syseditline} = xon], [
           SYSTEM_LIBEDIT
           CURSES
           AM_CONDITIONAL([EDITLINE], [ false ])
           AM_CONDITIONAL([READLINE], [ false ])
           AM_CONDITIONAL([SYSEDITLINE], [ true ])
           DEFINE_READLINE_EDITLINE
         ],
       # Handle the Local Editline Library
         [test x${editline} = xon], [
           DEFINE_READLINE_EDITLINE
           AM_CONDITIONAL([EDITLINE], [ true ])
           AM_CONDITIONAL([READLINE], [ false ])
           AM_CONDITIONAL([SYSEDITLINE], [ false ])
         ],
       # Handle the System Readline Library
         [test x${readline} = xon], [
           AC_CHECK_HEADERS([ readline/readline.h ],
                AC_SEARCH_LIBS([add_history], [readline]),
                [AC_MSG_FAILURE( --enable-readline was given, but test for readline failed)] )
           DEFINE_READLINE_READLINE
           AM_CONDITIONAL([EDITLINE], [ false ])
           AM_CONDITIONAL([READLINE], [ true ])
           AM_CONDITIONAL([SYSEDITLINE], [ false ])

         ],
         [ test ]
       )
       #
    ])
])
