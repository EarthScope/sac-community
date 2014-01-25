# Check Required Libraries and Files
# http://www.mathworks.com/access/helpdesk/help/techdoc/index.html
#

AC_DEFUN([CHECK_MATLAB],
	[
	have_matlab=off
	AC_ARG_VAR([MATLAB_DIR], [Matlab Base Directory])
	AC_ARG_WITH(matlab,
	[  --with-matlab=DIR  the directory where Matlab is installed ],

	MATLAB_DIR=${withval},
	MATLAB_DIR=)
	
        # Check for the "--with-matlab" Option
	if test -n "${MATLAB_DIR}"
	then
	
	MATLAB_CFLAGS="-I${MATLAB_DIR}/extern/include"
	# Save the LDFLAGS, CFLAGS, and LIBS
	LDFLAGS_SAVE="${LDFLAGS}"
	CFLAGS_SAVE="${CFLAGS}"
	LIBS_SAVE="${LIBS}"
	

	# Modify the CFLAGS to find the include files
        CFLAGS="${MATLAB_CFLAGS} ${CFLAGS}"
        LDFLAGS="-L${MATLAB_DIR}/${MATLAB_BIN_PATH} ${LDFLAGS}"
	
	# Check for the Include and Libraries Files
	AC_CHECK_HEADER([$MATLAB_DIR/extern/include/matrix.h], [
	AC_SEARCH_LIBS([engOpen], [eng],[ 
	AC_SEARCH_LIBS([mexAtExit], [mex], 
	               [have_matlab=on; 
			MATLAB_LIB="${MATLAB_DIR}/${MATLAB_BIN_PATH}"]) ]) ])

	# Check for the Matlab Executable
	MATLAB_BIN_TEST="${MATLAB_DIR}/${MATLAB_EXE_PATH}"
	AC_ARG_VAR([MATLAB_BIN], [Full Path to the Matlab Executable])
	AC_CHECK_FILE(["${MATLAB_BIN_TEST}"], 
                      [ AC_DEFINE_UNQUOTED([MATLAB_BIN], ["${MATLAB_BIN_TEST}"], [Location of Matlab Libraries]) ]
		      [] )

	# Return LDFLAGS, CFLAGS, and LIBS to Saved Value
	LDFLAGS="${LDFLAGS_SAVE}"
	CFLAGS="${CFLAGS_SAVE}"
	LIBS="${LIBS_SAVE}"
	
	# Output the Result
	AC_MSG_CHECKING([for Matlab libraries])

	AS_IF([ test x$have_matlab = "xon" ], 
		[ AC_MSG_RESULT($MATLAB_LIB) 
		  AC_DEFINE([HAVE_MATLAB], [1], [ Matlab Interface Routines ])
		  AC_DEFINE_UNQUOTED([MATLAB_LIB], ["${MATLAB_LIB}"], [Location of Matlab Libraries])
                  CFLAGS="${MATLAB_CFLAGS} ${CFLAGS}"		  
		], 
		[ AC_MSG_RESULT(Not Found) ]
              )
	else
	
	# No Matlab Option Given
	AC_MSG_CHECKING(for Matlab libraries)
	AC_MSG_RESULT(Not checked for)
	have_matlab=off
	fi

		
])
