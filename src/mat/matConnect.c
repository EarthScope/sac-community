
#include <config.h>

#ifdef HAVE_MATLAB

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "msg.h"
#include "bool.h"

/* Matlab Specific Header File */
#include <engine.h>

#include "matFuncExternal.h"

/* libmx (Matlab MX) Functions to be Resolved */
#define MX_GET_STRING                      "mxGetString"
#define MX_CREATE_STRING                   "mxCreateString"
#define MX_GET_STRING                      "mxGetString"
#define MX_SET_NAME                        "mxSetName"
#define MX_CREATE_STRUCT_ARRAY             "mxCreateStructArray"
#define MX_GET_PI                          "mxGetPi"
#define MX_SET_FIELD                       "mxSetField"
#define MX_SET_PR                          "mxSetPr"
#define MX_GET_FIELD                       "mxGetField"
#define MX_GET_PR                          "mxGetPr"
#define MX_IS_DOUBLE                       "mxIsDouble"
#define MX_GET_M                           "mxGetM"
#define MX_GET_N                           "mxGetN"
#define MX_IS_COMPLEX                      "mxIsComplex"
#define MX_IS_NUMERIC                      "mxIsNumeric"
#define MX_IS_CHAR                         "mxIsChar"
#define MX_DESTROY_ARRAY                   "mxDestroyArray"
#define MX_CREATE_DOUBLE_MATRIX            "mxCreateDoubleMatrix"
#define MX_IS_EMPTY                        "mxIsEmpty"
#define MX_GET_SCALAR                      "mxGetScalar"
#define MX_CREATE_CHAR_MATRIX_FROM_STRINGS "mxCreateCharMatrixFromStrings"

/* libeng (Matlab Engine) Functions to be Resolved */
#define ENG_OPEN                           "engOpen"
#define ENG_EVAL_STRING                    "engEvalString"
#define ENG_PUT_ARRAY                      "engPutArray"
#define ENG_OUTPUT_BUFFER                  "engOutputBuffer"
#define ENG_GET_ARRAY                      "engGetArray"
#define ENG_CLOSE                          "engClose"

#ifdef OSX

#define MATLAB_LIB_SUFFIX                  ".dylib"

#undef MX_GET_STRING                     
#undef MX_CREATE_STRUCT_ARRAY            
#undef MX_SET_FIELD                      
#undef MX_GET_FIELD                      
#undef MX_CREATE_DOUBLE_MATRIX           
#undef MX_CREATE_CHAR_MATRIX_FROM_STRINGS

#define MX_GET_STRING                      "mxGetString_730"
#define MX_CREATE_STRUCT_ARRAY             "mxCreateStructArray_730"
#define MX_SET_FIELD                       "mxSetField_730"
#define MX_GET_FIELD                       "mxGetField_730"
#define MX_CREATE_DOUBLE_MATRIX            "mxCreateDoubleMatrix_730"
#define MX_CREATE_CHAR_MATRIX_FROM_STRINGS "mxCreateCharMatrixFromStrings_730"

#endif

#ifdef LINUX
#define MATLAB_LIB_SUFFIX ".so"
#endif

#ifdef SOLARIS
#define MATLAB_LIB_SUFFIX ".so"
#endif

#ifdef CGYWIN
#define MATLAB_LIB_SUFFIX ".dll"
#endif

#define MATLAB_PATH_LIBENG MATLAB_LIB "/libeng" MATLAB_LIB_SUFFIX
#define MATLAB_PATH_LIBMX  MATLAB_LIB "/libmx" MATLAB_LIB_SUFFIX

#define SAC_MATLAB_BIN_ENV "SAC_MATLAB_BIN"

static const char engFuncNames[ ENGFUNCS + 1 ][16] = 
  { "" , 
    ENG_OPEN , 
    ENG_EVAL_STRING ,
    ENG_PUT_ARRAY ,
    ENG_OUTPUT_BUFFER ,
    ENG_GET_ARRAY ,
    ENG_CLOSE
  } ;
static const char mxFuncName[ MXFUNCS + 1 ][50] = 
  { 
    "" , 
    MX_CREATE_STRING , 
    MX_GET_STRING , 
    MX_SET_NAME , 
    MX_CREATE_STRUCT_ARRAY , 
    MX_GET_PI ,
    MX_SET_FIELD , 
    MX_SET_PR , 
    MX_GET_FIELD , 
    MX_GET_PR , 
    MX_IS_DOUBLE , 
    MX_GET_M , 
    MX_GET_N , 
    MX_IS_COMPLEX , 
    MX_IS_NUMERIC , 
    MX_IS_CHAR , 
    MX_DESTROY_ARRAY , 
    MX_CREATE_DOUBLE_MATRIX , 
    MX_IS_EMPTY , 
    MX_GET_SCALAR , 
    MX_CREATE_CHAR_MATRIX_FROM_STRINGS
  } ;


char *
env(char *name, char *def) {
  char *out;
  if((out = getenv(name)) != NULL) {
    return out;
  }
  return def;
}

int matConnect ( )
{
    int nerr = 0 , idx ;

    /* these lists of function names are used in loops below to facilitate
	calls to dlsym() */

#   include "matFuncInternal.h"

    /* If the SO files are already linked and the MATLAB is open, skip it */
    if ( linkedAndRunning )
	return 0 ;

    /* open libeng.so */
    *engHandle = dlopen ( MATLAB_PATH_LIBENG , RTLD_LAZY ) ;
    if ( *engHandle == NULL ) {
	/* error handling */
	fprintf(stderr, "Error: %s\n", dlerror());
	nerr = 8002 ;
	setmsg ( "ERROR" , nerr ) ;
	apcmsg ( MATLAB_PATH_LIBENG , strlen(MATLAB_PATH_LIBENG)+1 ) ;
	outmsg ();
	return nerr ;
    }

    /* open libmx.so */
    *mxHandle = dlopen ( MATLAB_PATH_LIBMX , RTLD_LAZY ) ;
    if ( *mxHandle == NULL ) {
        /* error handling */
	fprintf(stderr, "Error: %s\n", dlerror());
        nerr = 8002 ;
        setmsg ( "ERROR" , nerr ) ;
	apcmsg ( MATLAB_PATH_LIBMX , strlen(MATLAB_PATH_LIBMX)+1 ) ;
	outmsg ();
	dlclose ( *engHandle ) ;
	return nerr ;
    }

    /* get specific functions in libeng. */
    for ( idx = 1 ; idx <= ENGFUNCS && nerr == 0 ; idx++ ) {
	engHandle[ idx ] = dlsym ( *engHandle , engFuncNames[ idx ] ) ;
	if ( engHandle[ idx ] == NULL ) {
	    fprintf(stderr, "Error: %s\n", dlerror());
	    nerr = 8003 ;
	    setmsg ( "ERROR" , nerr ) ;
	    apcmsg ( (char*)engFuncNames[ idx ] , strlen ( engFuncNames[ idx ] ) + 1 ) ;
	    outmsg ();
	}
    } /* end for ( idx ) */

    /* get specific functions in libmx. */
    for ( idx = 1 ; idx <= MXFUNCS && nerr == 0 ; idx++ ) {
        mxHandle[ idx ] = dlsym ( *mxHandle , mxFuncName[ idx ] ) ;
        if ( mxHandle[ idx ] == NULL ) {
	    fprintf(stderr, "Error: %s\n", dlerror());
            nerr = 8003 ;
            setmsg ( "ERROR" , nerr ) ;
            apcmsg ( (char*)mxFuncName[ idx ] , strlen ( mxFuncName[ idx ] ) + 1) ;
	    outmsg ();
	}
    } /* end for ( idx ) */

    if ( nerr == 0 ) {
	/* assign a value to EngOpen */
	EngOpen = ( Engine * (*) () ) engHandle[ 1 ] ;

	/* Start up MATLAB */
	printf("Starting Matlab computational engine... \n");
	//	if (!(ep = EngOpen("matlab "))) {
	if (!(ep = EngOpen( env(SAC_MATLAB_BIN_ENV, MATLAB_BIN) ))) {
	    /* error handling */
	    nerr = 8004 ;
	    setmsg ( "ERROR" , nerr ) ;
	}
    }
    
    if ( nerr != 0 ) {		/* If there was an error ... */
	/* close handles */
	dlclose ( *engHandle ) ;
	dlclose ( *mxHandle ) ;
	linkedAndRunning = FALSE ;

	/* NULL out the elements */
	for ( idx = 0 ; idx <= ENGFUNCS ; idx++ ) 
	    engHandle[ idx ] = NULL ;
	for ( idx = 0 ; idx <= MXFUNCS  ; idx++ )
	     mxHandle[ idx ] = NULL ;
    }
    else {
	linkedAndRunning = TRUE ;

	/* assign values to the rest of the pointers to functions. */
	EngEvalString		= ( int      (*) () ) engHandle[ 2 ] ;
	EngPutArray		= ( int      (*) () ) engHandle[ 3 ] ;
	EngOutputBuffer		= ( int      (*) () ) engHandle[ 4 ] ;
	EngGetArray		= (mxArray * (*) () ) engHandle[ 5 ] ;
	EngClose		= ( int      (*) () ) engHandle[ 6 ] ;

	MxCreateString		= ( mxArray * (*) () ) mxHandle[ 1 ] ;
	MxGetString		= ( int       (*) () ) mxHandle[ 2 ] ;
	MxSetName		= ( void      (*) () ) mxHandle[ 3 ] ;
	MxCreateStructArray	= ( mxArray * (*) () ) mxHandle[ 4 ] ;
	MxGetPi			= ( double *  (*) () ) mxHandle[ 5 ] ;
	MxSetField		= ( void      (*) () ) mxHandle[ 6 ] ;
	MxSetPr			= ( void      (*) () ) mxHandle[ 7 ] ;
	MxGetField		= ( mxArray * (*) () ) mxHandle[ 8 ] ;
	MxGetPr			= ( double *  (*) () ) mxHandle[ 9 ] ;
	MxIsDouble		= ( bool      (*) () ) mxHandle[ 10 ] ;
	MxGetM			= ( int       (*) () ) mxHandle[ 11 ] ;
	MxGetN			= ( int       (*) () ) mxHandle[ 12 ] ;
	MxIsComplex		= ( bool      (*) () ) mxHandle[ 13 ] ;
	MxIsNumeric		= ( bool      (*) () ) mxHandle[ 14 ] ;
	MxIsChar		= ( bool      (*) () ) mxHandle[ 15 ] ;
	MxDestroyArray		= ( void      (*) () ) mxHandle[ 16 ] ;
	MxCreateDoubleMatrix	= ( mxArray * (*) () ) mxHandle[ 17 ] ;
	MxIsEmpty		= ( bool      (*) () ) mxHandle[ 18 ] ;
	MxGetScalar		= ( double    (*) () ) mxHandle[ 19 ] ;
	MxCreateCharMatrixFromStrings = (mxArray * (*)()) mxHandle[20];

    }

    return nerr ;

} /* end matConnect */


#endif /* HAVE_MATLAB */

#ifndef HAVE_MATLAB

void __matConnect_undef_symbol() { }

#endif 
