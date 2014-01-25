
database=off
AC_DEFUN([DATABASE_ON], 
     [ AC_DEFINE([DATABASE_ENABLED],  
                 [1], 
                 [Compile with SeisMgr Database Capabilities]) 
                 database=on ])
AC_DEFUN([DATABASE_OFF], 
     [ AC_DEFINE([DATABASE_DISABLED], 
                 [1], 
                 [Compile without SeisMgr Database Capabilities]) 
                 database=off ])
AC_DEFUN([CHECK_DATABASE], [
    AC_ARG_ENABLE(database, AS_HELP_STRING([--enable-database],[enable SeisMgr database capabilities]), 
       [ AS_IF( [ test x$enableval != xno ], [ DATABASE_ON ], [ DATABASE_OFF ] ) ], 
       [ DATABASE_OFF ]
    )
    AM_CONDITIONAL([DATABASE], [ test x$database = xon ])
])

