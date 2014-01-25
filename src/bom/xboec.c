/** 
 * @file   xboec.c
 * 
 * @brief  Execute BOEC
 * 
 */

#include <string.h>

#include "bom.h"
#include "exm.h"
#include "cpf.h"

/** 
 * Execute the parameter setting command "BOEC".  This command controls
 *   certain error conditions that can occur during binary operations.
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *
 * @date   820817:  Changed to newest set of parsing and checking functions.
 * @date   810723:  Original version.
 *
 */
void 
xboec(int *nerr) {

	int index;

	*nerr = 0;

L_1000:
	if( lcmore( nerr ) ){

		/* -- "NPTS condition":  
		   set control for unequal number of data points. */
		if( lklist( "N$",3, (char*)kmexm.kectp,9, 
			    cmexm.nectp, &index ) ){
			strcpy( kmbom.kecnpt, kmexm.kectp[index - 1] );

		}
		/* "DELTA condition":  
		   set control for sampling interval mismatch. */
		else if( lklist( "D$",3, (char*)kmexm.kectp, 9, 
				 cmexm.nectp, &index ) ){
			strcpy( kmbom.kecdel, kmexm.kectp[index - 1] );

		}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

		}
		goto L_1000;

	}

	return;
}

