
AC_INIT([fern], [1.0.2], [savage13@gmail.com])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([foreign no-dependencies subdir-objects])
AM_MAINTAINER_MODE
AC_CONFIG_HEADERS([config.h])
AC_PROG_CC
AM_PROG_AR
AC_PROG_RANLIB

# libxml2
AM_PATH_XML2(2.4.0, [], [ AC_MSG_ERROR("Cannot find libxml2 library") ])

# libcurl
LIBCURL_CHECK_CONFIG

AC_SEARCH_LIBS([ms3_readmsr], [mseed], [], [
   AC_MSG_ERROR([unable to find the ms3_readmsr() function])
])
AC_SEARCH_LIBS([sac_read], [sacio_bsd], [], [
         AC_MSG_ERROR([unable to find the sac_read() function])
])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

