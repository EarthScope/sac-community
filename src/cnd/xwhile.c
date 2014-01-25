/** 
 * @file   xwhile.c
 * 
 * @brief  Parse "WHILE"
 * 
 */

#include <string.h>

#include "cnd.h"
#include "cpf.h"
#include "vars.h"
#include "msg.h"

#include "errors.h"


#include "ucf.h"

/** 
 * Parse the action command "WHILE"
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - ERROR_EVALUATING_LOGICAL_EXPRESSION
 *
 * @note Global Coupling
 *    ndolevel: Incremented by one.
 *    lifresp:  Set to value of if test.
 *
 * @date   970128:  Fixed a bug in nesting while loops.  maf
 * @date   870817:  Original version.
 *
 */
void 
xwhile(int *nerr) {

	char kcond[MCMSG+1], kmacroname[MCPFN+1], kresult[9];
	int nc, numlines, nverr;

	*nerr = 0;
	numlines = 0;

	/* -- Get length of do loop */
	getdolen( &numlines, nerr );
	if( *nerr != 0 ){
		apcmsg( "in macro file",14 );
		getvvstring( kmcpf.kvarsname,9, "macroname",10, 
			     &nc, kmacroname, MCPFN+1, &nverr );
		apcmsg( kmacroname,MCPFN+1 );
		goto L_8888;
	}

	/* - Copy rest of command to condition string. */
        memset(kcond,(int)' ',MCMSG);
        kcond[MCMSG] = '\0';

	/* - Evaluate condition */
	evallogical( kcond,MCMSG+1, kresult,9 );
	if( memcmp(kresult,"ERROR",5) == 0 )
		goto L_9000;

	if( memcmp(kresult,"TRUE",4) == 0 ){
		cnd.ndolevel = cnd.ndolevel + 1;
		Ndotype[cnd.ndolevel] = 1;
		Ndolines[cnd.ndolevel] = numlines;
	}
	else if( memcmp(kresult,"FALSE",5) == 0 ){
	        /* added 970128 to fix a bug in nesting. */
		cnd.ndolevel++ ;	
		skipdo( nerr );
	}
	else{
		*nerr = 1;
	}
	
L_8888:
	return;

L_9000:
	*nerr = ERROR_EVALUATING_LOGICAL_EXPRESSION;
	setmsg( "ERROR", *nerr );
	goto L_8888;

}

