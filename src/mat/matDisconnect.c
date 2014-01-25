
#include <config.h>

#ifdef HAVE_MATLAB 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>

/* Matlab Specific Header File */
#include <engine.h>

#include "matFuncExternal.h"

#define  TRUE 1
#define FALSE 0

void matDisconnect () 
{
    int idx ;

#   include "matFuncInternal.h"

    if ( ! linkedAndRunning ) 
	return ;

    fprintf (stderr, "Closing Matlab engine ... \n" ) ;
    EngClose ( ep ) ;

    dlclose ( * engHandle ) ;
    dlclose ( * mxHandle ) ;

    for ( idx = 0 ; idx <= ENGFUNCS ; idx++ )
	engHandle[ idx ] = NULL ;
    for ( idx = 0 ; idx <= MXFUNCS ; idx++ )
	mxHandle [ idx ] = NULL ;

    linkedAndRunning = FALSE ;
}

#endif /* HAVE_MATLAB */


#ifndef HAVE_MATLAB

void __matDisconnect_undef_symbol() { }

#endif 
