
XML=""
AC_SUBST([XML])

AC_DEFUN([AM_PATH_XML2],[
  # m4_warn([obsolete], [AM_PATH_XML2 is deprecated, use PKG_CHECK_MODULES instead])
  AC_REQUIRE([PKG_PROG_PKG_CONFIG])

  verdep=ifelse([$1], [], [], [">= $1"])
  PKG_CHECK_MODULES(XML, [libxml-2.0 $verdep], [$2], [$3])

  XML_CPPFLAGS=$XML_CFLAGS
  AC_SUBST(XML_CPPFLAGS)
  AC_SUBST(XML_LIBS)
])

AC_DEFUN([XML_ON], [
             AC_DEFINE([HAVE_XML], [1], [Compile with XML-Station Support])
             XML="XML "
             ])

AC_DEFUN([XML_TRY], [
        AM_PATH_XML2(2.4.0, [XML_ON], [ AC_MSG_ERROR("Cannot find libxml2 library") ])
      ])

AC_DEFUN([CHECK_XML],[
AC_ARG_ENABLE(XML, AS_HELP_STRING([--enable-xml], [enable xml-station files]),
                   [ AS_IF( [test x$enableval != xno ],
                            [ XML_TRY ],
                            [ XML_OFF ] ) ],
                            [ XML_TRY ] )
   ])
