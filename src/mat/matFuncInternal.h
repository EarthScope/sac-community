
    extern char linkedAndRunning ;
    extern Engine * ep ;
    extern void *engHandle[ ENGFUNCS + 1 ] ;
    extern void *mxHandle[ MXFUNCS + 1 ] ;

    extern Engine *	( * EngOpen )		() ;
    extern int		( * EngEvalString )	() ;
    extern int		( * EngPutArray )	() ;
    extern int		( * EngOutputBuffer )	() ;
    extern mxArray *	( * EngGetArray )	() ;
    extern int		( * EngClose )		() ;

    extern mxArray *	( * MxCreateString )	() ;
    extern int		( * MxGetString )	() ;
    extern void		( * MxSetName )		() ;
    extern mxArray *	( * MxCreateStructArray)() ;
    extern double *	( * MxGetPi )		() ;
    extern void		( * MxSetField )	() ;
    extern void		( * MxSetPr )		() ;
    extern mxArray *	( * MxGetField )	() ;
    extern double *	( * MxGetPr )		() ;
    extern bool		( * MxIsDouble )	() ;
    extern int		( * MxGetM )		() ;
    extern int		( * MxGetN )		() ;
    extern bool		( * MxIsComplex )	() ;
    extern bool		( * MxIsNumeric )	() ;
    extern bool		( * MxIsChar )		() ;
    extern void		( * MxDestroyArray )	() ;
    extern mxArray *	( * MxCreateDoubleMatrix)();
    extern bool		( * MxIsEmpty )		() ;
    extern double	( * MxGetScalar )	() ;
    extern mxArray *	( * MxCreateCharMatrixFromStrings ) () ;

