#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../inc/complex.h"
#include "../../inc/proto.h"
#include <string.h>
#include "../../inc/mach.h"
#include "../../inc/gam.h"

#include "cpf.h"

void /*FUNCTION*/ xfid(nerr)
int *nerr;
{
	char ktok[9];
	int lnumbr;
	int j, j_;

	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command FID.
	 *          This command controls the file id display on each plot.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  GAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GAM:     KFIDTP(), NFIDTP, KFIDLC(), NFIDLC
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GAM:     LFIDRQ, IFIDTP, IFIDLC
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCLOG, LKLIST
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

		/* -- "ON/OFF":  turn file id display on or off. */
		if( lclog( &cmgam.lfidrq ) )
		{ /* do nothing */ }

		/* -- "TYPE DEFAULT/STD/NAME/LIST list":  change type of file id. */
		else if( lklist( "TYPE$",6, (char*)kmgam.kfidtp,9, cmgam.nfidtp, 
		 &cmgam.ifidtp ) ){
			if( cmgam.ifidtp == 1 || cmgam.ifidtp == 2 ){
				cmgam.nfidnm = cmgam.nfidst;
				for( j = 1; j <= cmgam.nfidst; j++ ){
					j_ = j - 1;
					strcpy( kmgam.kfidnm[j_], kmgam.kfidst[j_] );
				}
			} /* end if( cmgam.ifidtp == 1 || cmgam.ifidtp == 2 ) */
			else if( cmgam.ifidtp == 3 ){
				cmgam.nfidnm = 1;
				strcpy( kmgam.kfidnm[0], "FILENAME" );
			}
			else if( cmgam.ifidtp == 4 ){
				cmgam.nfidnm = 0;

				while ( lcchar( 9,ktok,9, &lnumbr ) ){
					if( cmgam.nfidnm < MFIDNM ){
						cmgam.nfidnm = cmgam.nfidnm + 1;
						strcpy( kmgam.kfidnm[cmgam.nfidnm - 1], ktok);
					}
					else{
						cfmt( "TOO MANY FIELDS IN LIST:",26 );
						cresp();
					}
				} 
			} /* end else if( cmgam.ifidtp == 4 ) */
		} /* end else if( lklist( "TYPE$", ... ) */

		/* -- "LOCATION UL/UR/LL/LR":  change location of file id display. */
		else if( lklist( "LOCATION$",10, (char*)kmgam.kfidlc,9, cmgam.nfidlc, 
		 &cmgam.ifidlc ) )
		{ /* do nothing */ }

		/* -- "FORMAT EQUALS/COLONS/NONAMES":  change format of display. */
		else if( lklist( "FORMAT$",8, (char*)kmgam.kfidfm,9, cmgam.nfidfm, 
		 &cmgam.ifidfm ) )
		{ /* do nothing */ }

		/* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();
		}

	} /* end while ( lcmore( nerr ) ) */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    841026:  Added LIST and FORMAT options.
	 *    820818:  Original version (from XDISPL).
	 *===================================================================== */

} /* end of function */

