
XML=""
AC_SUBST([XML])
AC_SUBST([XML_CFLAGS])
AC_SUBST([XML_LIBS])

AC_DEFUN([XML_ON], [
             AC_DEFINE([HAVE_XML], [1], [Compile with XML-Station Support])
             XML="XML "
             ])

AC_DEFUN([XML_OFF], [
             AC_DEFINE([HAVE_XML], [1], [Compile with XML-Station Support])
             XML=" "
             ])

AC_DEFUN([XML_TRY],
        [
        PKG_CHECK_MODULES(XML, libxml-2.0 >= 2.4, [XML_ON], [XML_OFF])
        case x"$XML" in
        x)
            AM_PATH_XML2(2.4.0)
        ;;
        esac
        ])

AC_DEFUN([CHECK_XML],[
AC_ARG_ENABLE(XML, AS_HELP_STRING([--enable-xml], [enable xml-station files]),
                   [ AS_IF( [test x$enableval != xno ],
                            [ XML_TRY ],
                            [ XML_OFF ] ) ],
                            [ XML_TRY ] )
   ])
