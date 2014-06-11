

# Process this file with autoconf to produce a configure script.
AC_INIT(Seismic Analysis Code, 101.6a, sac-help@iris.washington.edu, sac)

# Place configuration files in the config directory
AC_CONFIG_AUX_DIR(config)

# Default install location
AC_PREFIX_DEFAULT(/usr/local/sac)

# Do not re-run autotools 
# Do not recreate the configure script if the time stamps are not correct
AM_MAINTAINER_MODE

# Determine the System Type
AC_CANONICAL_SYSTEM

# Header Values 
AC_CONFIG_HEADER(inc/config.h)

# Initialization of Automake
AM_INIT_AUTOMAKE 

m4_include([config/matlab.m4])
m4_include([config/png.m4])
m4_include([config/xpm.m4])
m4_include([config/zlib.m4])
m4_include([config/xft.m4])
m4_include([config/x11.m4])
m4_include([config/osx.m4])
m4_include([config/xdr.m4])
m4_include([config/db.m4])
m4_include([config/code.m4])
m4_include([config/edit.m4])
m4_include([config/win.m4])

# Check for CFLAGS Variable and set default to ""
AS_IF( [ test x"${CFLAGS}" = x ],  [ CFLAGS="" ] )

# Check for LDFLAGS Variable and set default to ""
AS_IF( [ test x"${LDFLAGS}" = x ], [ LDFLAGS="" ] )

AC_SUBST([BINARY_DIST])
BINARY_DIST=""

# Readline/Editline Handling (Editline On by Default, except for Cygwin)
display=x11
editline=off
readline=off
winaux=off

# Operating System Checks and Sets
case "${host}" in
     i[[3456]]86-*-linux-gnu*)       AC_DEFINE( [LINUX],   [1], [ Linux/x86 Operating System ])
                                     BINARY_DIST="linux_x86"
                                     MATLAB_BIN_PATH="bin/glnx86"
                                     MATLAB_EXE_PATH="bin/matlab"
                                     CFLAGS+=" -D_GNU_SOURCE "
     ;;
     x86_64-*-linux-gnu*)            AC_DEFINE( [LINUX],   [1], [ Linux/x86/64 Operating System ]) 
                                     BINARY_DIST="linux_x86_64"
                                     MATLAB_BIN_PATH="bin/glnxa64"
                                     MATLAB_EXE_PATH="bin/matlab"
                                     CFLAGS+=" -D_GNU_SOURCE "
     ;;
     sparc*-sun-solaris2.[[56789]]*) AC_DEFINE( [SOLARIS], [1], [ SUN/Solaris Operating System ]) 
                                     BINARY_DIST="sun_solaris2"
                                     MATLAB_BIN_PATH="bin/sol64"
                                     MATLAB_EXE_PATH="bin/matlab"
     ;;
     sparc*-sun-solaris*)            AC_DEFINE( [SOLARIS], [1], [ SUN/Solaris Operating System ]) 
                                     BINARY_DIST="sun_solaris"
                                     MATLAB_BIN_PATH="bin/sol64" 
                                     MATLAB_EXE_PATH="bin/matlab"    
     ;;
     *darwin[[789]]* | *darwin[[912]]* ) AC_DEFINE( [OSX],     [1], [ Apple/OSX/Darwin Operating System ]) 
                                     BINARY_DIST="OSX_darwin"    
                                     MATLAB_EXE_PATH="bin/matlab" 
                                     case "${host}" in
                                          *i686*)    MATLAB_BIN_PATH="bin/maci"  ;;
                                          *powerpc*) MATLAB_BIN_PATH="bin/mac"   ;;
                                          *x86_64*)  MATLAB_BIN_PATH="bin/mac"   ;;
                                     esac
     ;;
     *darwin6* )                     AC_DEFINE( [OSX],     [1], [ Apple/OSX/Darwin Operating System ])
                                     BINARY_DIST="OSX_darwin6"     
                                     LDFLAGS="-L/sw/lib $LDFLAGS"
                                     CFLAGS="-I/sw/include $CFLAGS"
                                     MATLAB_EXE_PATH="bin/matlab"
                                     case "${host}" in
                                          *i686*)    MATLAB_BIN_PATH="bin/maci"  ;;
                                          *powerpc*) MATLAB_BIN_PATH="bin/mac"   ;;
                                     esac
     ;;
     *cygwin* )                      AC_DEFINE( [LINUX],   [1], [ Unix Emulation ] )
                                     AC_DEFINE( [CYGWIN],  [1], [ Cygwin32 System ] )
                                     BINARY_DIST="win32"
                                     readline=on
                                     MATLAB_BIN_PATH="bin/win32"
                                     MATLAB_EXE_PATH="bin/matlab"
                                     AC_MSG_CHECKING([for Cygwin version])
                                     OSRELEASE=`uname -r`
                                     case "${OSRELEASE}" in 
                                          *1.[[135]]*) winaux=on
                                          ;;
                                     esac
		                     AC_MSG_RESULT( $OSRELEASE )
     ;;
     *mingw* )                       AC_DEFINE([MINGW], [1], [Minimalist GNU for Windows])
				     BINARY_DIST="mingw"
                                     windows=on
				     readline=off
				     curses=off
				     use_x11=off
				     MATLAB_BIN_PATH=""
				     MATLAB_EXE_PATH=""
                                     AM_CXXFLAGS=-mwindows
			             AC_MSG_CHECKING([for MinGW version])
                                     AC_DEFINE([WIN_APP], [1], [Create Windows Application])
                                     display=win
				     OSRELEASE=`uname -r`
				     case "${OSRELEASE}" in
				       *1.* ) winaux=on
     ;;
     esac
     AC_MSG_RESULT( $OSRELEASE )
     OBJEXT=o
     
     ;;
esac

AM_CONDITIONAL([CYGWIN_SYSTEM], [ test x${BINARY_DIST} = xwin32 ])
AM_CONDITIONAL([WINAUX_NOT_AUX], [ test x${winaux} = on ] )

# C Compiler
AC_PROG_CC

# C++ Compiler for Windows
#if test x$windows == xon; then
AC_PROG_CXX
#fi
AC_PROG_OBJC

# Linking Files
AC_PROG_LN_S

# Sed - Stream Editor, Handles SAC_PREFIX for bin/sacinit.[sh,csh]
AC_PROG_SED

# Source and Objects in the Same Directory
AM_PROG_CC_C_O

# Windows Application Options
CHECK_WIN_APP

# OSX Application Option
CHECK_OSX_APP

AM_CONDITIONAL([X11_APP], [ test x$display = xx11 ])
AM_CONDITIONAL([OSX_APP], [ test x$display = xosx ])
AM_CONDITIONAL([WIN_APP], [ test x$display = xwin ])

# Check for Editline/Readlien
CHECK_EDITING

# XDR Format Option
CHECK_XDR

AC_PROG_F77([f77 gfortran f90 xlf90 ifort])
AM_CONDITIONAL([HAVE_FC], [ test x"$F77" != x"" ] )

# Check for Matlab Options
CHECK_MATLAB

# SeisMgr Database Access
CHECK_DATABASE

# Check Code Generation
CHECK_DEBUG
CHECK_OPTIMIZATION

if [ test x${display} = xx11 ]; then
  CHECK_X11_DISPLAY
  CHECK_XPM
  CHECK_XFT
fi

CHECK_ZLIB
CHECK_PNG

if test x${display} = xx11 ; then
   AC_DEFINE([USE_TERMIOS], [1], [ Program needs the Terminal (TTY) to Function ] )
fi

# Checks for programs.
AC_PROG_RANLIB

AC_CHECK_HEADERS([ inttypes.h limits.h stdint.h ])

if test x$curses == xon ; then
	# Check for Terminal Window Size Location
	AC_HEADER_TIOCGWINSZ
fi

# Math Libary
AC_CHECK_LIB(m, sin)

# Dynamic Loading of Modules
AC_CHECK_LIB(dl, dlopen)

# Check for Specific Functions 
AC_CHECK_FUNC(lround,    [ AC_DEFINE( [HAVE_FUNC_LROUND],        [1], [ System Libraries define lround ]) ],
                         [ AC_DEFINE( [MISSING_FUNC_LROUND],     [1], [ System Libraries missing lround ]) ]  )
AC_CHECK_FUNC(fmin,      [ AC_DEFINE( [HAVE_FUNC_FMIN],          [1], [ System Libraries define fmin ]) ],
                         [ AC_DEFINE( [MISSING_FUNC_FMIN],       [1], [ System Libraries missing fmin ]) ]  )
AC_CHECK_FUNC(fmax,      [ AC_DEFINE( [HAVE_FUNC_FMAX],          [1], [ System Libraries define fmax ]) ],
                         [ AC_DEFINE( [MISSING_FUNC_FMAX],       [1], [ System Libraries missing fmax ]) ]  )
AC_CHECK_FUNC(tolower,   [ AC_DEFINE( [HAVE_FUNC_TOLOWER],       [1], [ System Libraries define tolower ]) ],
                         [ AC_DEFINE( [MISSING_FUNC_TOLOWER],    [1], [ System Libraries missing tolower ]) ]  )
AC_CHECK_FUNC(toupper,   [ AC_DEFINE( [HAVE_FUNC_TOUPPER],       [1], [ System Libraries define toupper ]) ],
                         [ AC_DEFINE( [MISSING_FUNC_TOUPPER],    [1], [ System Libraries missing toupper ]) ]  )
AC_CHECK_FUNC(strtof,    [ AC_DEFINE( [HAVE_FUNC_STRTOF],        [1], [ System Libraries define strtof ]) ] )
AC_DEFUN([SETENV_ON],    [ AC_DEFINE( [HAVE_FUNC_SETENV],        [1], [ System Libraries define setenv ]) ])
AC_DEFUN([PUTENV_ON],    [ AC_DEFINE( [HAVE_FUNC_PUTENV], [1], [ System Libraries define putenv ]) ])
AC_DEFUN([MISSING_SETPUT], [ AC_MSG_FAILURE( [ System does have a working setenv or putenv function ] ) ] )
AC_CHECK_FUNC(setenv, [ SETENV_ON ], [ AC_CHECK_FUNC(putenv,  [ PUTENV_ON ], [ MISSING_SETPUT ]) ] )
AC_CHECK_FUNC(vasprintf, [ AC_DEFINE( [HAVE_FUNC_VASPRINTF],     [1], [ System Libraries define vasprintf ]) ],
                         [ AC_DEFINE( [MISSING_FUNC_VASPRINTF],  [1], [ System Libraries missing vasprintf ]) ]  )
AC_CHECK_FUNC(strsep,    [ AC_DEFINE( [HAVE_FUNC_STRSEP],        [1], [ System Libraries define strsep ]) ],
                         [ AC_DEFINE( [MISSING_FUNC_STRSEP],     [1], [ System Libraries missing strsep ]) ]  )
AC_CHECK_FUNC(mkstemps,  [ AC_DEFINE( [HAVE_FUNC_MKSTEMPS],      [1], [ System Libraries define mkstemps ]) ],
                         [ AC_DEFINE( [MISSING_FUNC_MKSTEMPS],   [1], [ System Libraries missing mkstemps ]) ]  )
AC_CHECK_FUNC(bzero,     [ AC_DEFINE( [HAVE_FUNC_BZERO],         [1], [ System Libraries define bzero ]) ],
						 [ AC_DEFINE( [MISSING_FUNC_BZERO],      [1], [ System Libraries missing bzero ]) ] )
AC_CHECK_FUNC(index,     [ AC_DEFINE( [HAVE_FUNC_INDEX],         [1], [ System Libraries define index ]) ],
						 [ AC_DEFINE( [MISSING_FUNC_INDEX],      [1], [ System Libraries missing index ]) ] )
AC_CHECK_FUNC(rindex,    [ AC_DEFINE( [HAVE_FUNC_RINDEX],        [1], [ System Libraries define rindex ]) ],
						 [ AC_DEFINE( [MISSING_FUNC_RINDEX],     [1], [ System Libraries missing rindex ]) ] )
AC_CHECK_FUNC(sleep,     [ AC_DEFINE( [HAVE_FUNC_SLEEP],         [1], [ System Libraries define sleep ]) ],
						 [ AC_DEFINE( [MISSING_FUNC_SLEEP],      [1], [ System Libraries missing sleep ]) ] )

if test x$use_x11 == xon ; then
   AC_DEFINE([USE_X11],[1],[Use X11 as a Graphics Device])
fi

# Check for typedefs, structures, and compiler characteristics
#AC_C_CONST

# Define the Build Date 
AC_DEFINE_UNQUOTED([BUILD_DATE], ["`date +%m/%d/%Y`"], [Macro which defines the Date of the Current Build])

# Define the Install Directory
if [ test "x$prefix" = xNONE ]; then
   aux="${ac_default_prefix}/aux"
else
   aux="${prefix}/aux"
fi
AC_DEFINE_UNQUOTED([SACAUX], ["${aux}"], [Define the SACAUX directory])

# Output the Makefiles
AC_OUTPUT(src/Makefile utils/Makefile libedit/Makefile t/Makefile Makefile)

DISPLAYSTR=""
if test x${display} = xx11; then
DISPLAYSTR="
X11-buffer     = $x11_buffer
X11-depth      = $x11_depth
X_LIBS         = $X_LIBS 
X_CFLAGS       = $X_CFLAGS  "
fi

AC_MSG_NOTICE([

Compilation Options for $PACKAGE_TARNAME ($PACKAGE_STRING)

CFLAGS         = $AM_CFLAGS $CFLAGS
LDFLAGS        = $LDFLAGS
LIBS           = $LIBS 
Debug          = $DEBUG $COVERAGE
Optimization   = $OPTIM
Database       = $database
Matlab         = $have_matlab  $DISPLAYSTR
XDR Format     = $librpc
Support        = $xpm$png$zlib$xft$freetype$fontconfig
Readline       = $readline
Editline       = $editline
Install Base   = $prefix
System         = $BINARY_DIST ($host) ($display)

To compile $PACKAGE_TARNAME type make ])
