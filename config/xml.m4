
XML=""
AC_SUBST([XML])

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
