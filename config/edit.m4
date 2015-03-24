
editline=on
readline=on

# Curses Library
AC_SUBST([CURSES_SEARCH_PATH])
curses_library_found=no

# Macro Definitions
AC_DEFUN([NCURSES_ON], [ CURSES_SEARCH_PATH="ncurses curses termcap" ])
AC_DEFUN([NCURSES_OFF], [ CURSES_SEARCH_PATH="curses termcap" ])
AC_DEFUN([EDITLINE_TURN_ON],  [ editline=on ] )
AC_DEFUN([READLINE_TURN_ON],  [ readline=on ] )
AC_DEFUN([EDITING_ON],       [ AC_DEFINE([READLINE],          [1], [Compile with command line capabilities]) ])
AC_DEFUN([EDITLINE_TURN_OFF], [ AC_DEFINE([EDITLINE_DISABLED], [1], [Compile without editline comand line capabilities]) ])

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
    [ AS_IF( [ test x$enableval == xyes ], [ READLINE_TURN_ON ] ) ],
    [ test ] 
)
])

# Editline (ON by Default)
AC_DEFUN([CHECK_EDITLINE], [
AC_ARG_ENABLE(editline, AS_HELP_STRING([--disable-editline],[disable editline command line capabilities]), 
    [ AS_IF( [ test x$enableval == xno ], [ EDITLINE_TURN_OFF ], [ EDITLINE_TURN_ON ] )],
    [ AS_IF( [ test x$readline  != xon ], [ EDITLINE_TURN_ON ] )]
)
])

AC_DEFUN([CHECK_EDITING], [
    echo "Readline: $readline"
    if test x${readline} = xon; then
        CHECK_CURSES
        CHECK_EDITLINE
        CHECK_READLINE
        # Handle the Editline Library
        AS_IF( [ test x$editline = xon ], [
           CURSES
           EDITING_ON
           editline=on
           readline=off
        ])

        # Handle the Readline Library
        AS_IF( [ test x$readline = xon ], [
           CURSES
           EDITING_ON
           AC_CHECK_HEADERS([ readline/readline.h ],
              AC_SEARCH_LIBS([add_history], [readline]),
              [AC_MSG_FAILURE( --enable-readline was given, but test for readline failed)] )
                 EDITLINE_TURN_OFF
           editline=off
           readline=on   
        ])
    fi
    AM_CONDITIONAL([EDITLINE], [ test x$editline = xon ])
    AM_CONDITIONAL([READLINE], [ test x$readline = xon ])
])
