/** 
 * @file   xsetmacro.c
 * 
 * @brief  SETMACRO command
 * 
 */

#include <string.h>

#include "exm.h"
#include "cpf.h"
#include "bool.h"


#include "bot.h"

/** 
 * Execute the SETMACRO command which sets the macro search path
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   961205:  Added MORE option to allow subsequent additions to list,
 *                  also added functionallity to clean up old values if
 *                  MORE is not specified and if the new number of values
 *                  is less than the previous number of values.  maf
 * @date   870416:  Original version. 
*/
void 
xsetmacro(int *nerr) {

	char ktemp[MCPFN+1];
	int nchar;
        int oldnmcdir = -1 ; /* clean up old values  */
	int lmore = FALSE ;  /* supports MORE option */

	*nerr = 0;

	/* - Look for location-dependent token: MORE.  maf 961205 */
	if ( lcmore ( nerr ) ) {
	    if ( lckey( "MORE #$",8 ) )
		lmore = TRUE ;
	    else {
		oldnmcdir = cmexm.nmcdir ;
		cmexm.nmcdir = 0;
	    }
	} /* end if ( lcmore ) */


	/* - Loop on remaining tokens in command: */

	while( lcmore( nerr ) ){

		/* -- "text":  the name of a directory to search for macros. */
		if( lcchar( MCPFN, ktemp,MCPFN+1, &nchar ) ){
			if( cmexm.nmcdir < MMCDIR ){
				cmexm.nmcdir = cmexm.nmcdir + 1;
				if( MODEFILECASE < 0 ){
				    modcase( FALSE, ktemp, nchar, (char*)kmexm.kmcdir[cmexm.nmcdir - 1] );
				}
				else if( MODEFILECASE > 0 ){
				    modcase( TRUE, ktemp, nchar, (char*)kmexm.kmcdir[cmexm.nmcdir - 1] );
				}
				else{
				    strcpy( kmexm.kmcdir[cmexm.nmcdir - 1], ktemp );
				}
			} /* end if( cmexm.nmcdir < MMCDIR ) */
			else{
				cfmt( "TOO MANY DIRECTORIES:",23 );
				cresp();
			}
		} /* end if( lcchar( MCPFN, ktemp,MCPFN+1, &nchar ) ) */

		/* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();
		}

	} /* end while ( lcmore( nerr ) ) */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	/* If there are previously defined directories, and lmore is FALSE, erase the old ones */
	/*	maf 961205 */
	if ( !lmore ) {
	    while ( oldnmcdir > cmexm.nmcdir )
	    {
		strcpy ( kmexm.kmcdir[oldnmcdir - 1] , "" ) ;
		oldnmcdir-- ;
	    }
	}

	return;

} /* end of function */

