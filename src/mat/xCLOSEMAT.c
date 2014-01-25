
#include <config.h>
#include "debug.h"

#ifdef HAVE_MATLAB

#include <stdio.h>

/* Matlab Specific Header File */
#include <engine.h>

#include "mat.h"
#include "matFuncExternal.h"


/** 
 * Execute the command "closemat" which closes the Matlab Engine
 *
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success ( Always Successful )
 *    - Non-Zero on Error
 *
 * @date 970902:  Original version.
 *
 */
void 
xCLOSEMAT(int *nerr)
{
#	include "matFuncInternal.h" 

	*nerr = 0;

	if ( linkedAndRunning ) {
	   matDisconnect () ;
	}

        return;
        
} 

#endif /* HAVE_MATLAB */

#ifndef HAVE_MATLAB

void matlab_unavailable();

void xCLOSEMAT(int *nerr) { UNUSED(nerr); matlab_unavailable(); }

#endif 
