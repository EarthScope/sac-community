#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gam.h"


#include "cpf.h"

void xpicks(int *nerr)
{
	int index, ipknam;
  double tmp;

	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command PICKS.
	 *          This command controls the display of time picks on each plot.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  GAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GAM:     KPKNAM(), MTMDUP, KPKTYP(), MPKTYP
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GAM:     LDSPPK, IPKTYP(), PKWDTH, PKHGTH
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCLOG, LKRRC, LCLIST
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    871015:  Fixed bug in reporting bad pick type error.
	 *    820000:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  871015
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "ON/OFF":  turn pick display on or off. */
		if( lclog( &cmgam.ldsppk ) ){

			/* -- "WIDTH v":  define new pick width. */
			}
		else if( lkrrc( "W$",3, 0., 1., &tmp ) ){
      cmgam.pkwdth = (float) tmp;
    }
    /* -- "HEIGHT v":  define new pick height. */
		else if( lkrrc( "H$",3, 0., 1., &tmp ) ){
      cmgam.pkhgth = (float) tmp;
    }
    /* -- "pick type":  define new display type for a given time pick. */
		else if( lclist( (char*)kmgam.kpknam,9, MPKNAM, &ipknam ) ){
			if( lclist( (char*)kmgam.kpktyp,9, MPKTYP, &index ) ){
				Ipktyp[ipknam] = index;
				}
			else{
				cfmt( "ILLEGAL PICK TYPE:",20 );
				cresp();
				}

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

       
	return;

} /* end of function */

