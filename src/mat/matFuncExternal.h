#ifndef _MATFUNCEXTERNAL_H
#   define _MATFUNCEXTERNAL_H

#   define ENGFUNCS 6
#   define MXFUNCS 20

#   ifdef _ENGINECALL
	char linkedAndRunning ;
	Engine * ep ;
	void *engHandle[ ENGFUNCS + 1 ] ;
	void *mxHandle[ MXFUNCS + 1 ] ;

	Engine *    ( * EngOpen )           () ;
	int         ( * EngEvalString )     () ;
	int         ( * EngPutArray )       () ;
	int         ( * EngOutputBuffer )   () ;
	mxArray *   ( * EngGetArray )       () ;
	int         ( * EngClose )          () ;

	mxArray *   ( * MxCreateString )    () ;
	int         ( * MxGetString )       () ;
	void        ( * MxSetName )         () ;
	mxArray *   ( * MxCreateStructArray)() ;
	double *    ( * MxGetPi )           () ;
	void        ( * MxSetField )        () ;
	void        ( * MxSetPr )           () ;
	mxArray *   ( * MxGetField )        () ;
	double *    ( * MxGetPr )           () ;
	bool        ( * MxIsDouble )        () ;
	int         ( * MxGetM )            () ;
	int         ( * MxGetN )            () ;
	bool        ( * MxIsComplex )       () ;
	bool        ( * MxIsNumeric )       () ;
	bool        ( * MxIsChar )          () ;
	void        ( * MxDestroyArray )    () ;
	mxArray *   ( * MxCreateDoubleMatrix)() ;
	bool        ( * MxIsEmpty )         () ;
	double      ( * MxGetScalar )       () ;
	mxArray *   ( * MxCreateCharMatrixFromStrings ) () ;
#   endif
#endif
