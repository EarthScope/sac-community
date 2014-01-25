
#include "gem.h"


#include "cpf.h"

void /*FUNCTION*/ xtsize(nerr)
int *nerr;
{
	int j;

  double tmp;

	/*=====================================================================
	 * PURPOSE:  To execute the parameter-setting command TSIZE.
	 *           This command sets various text size attributes.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL: GEM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GEM:     MTXSIZ, OTXSIZ, DTXSIZ, OTXRAT, DTXRAT
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GEM:     TXSIZ, TXRAT
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCKEY, LKRRC, LKREAL
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "OLD/NEW": set text sizes to old or new values. */
		if( lckey( "OLD$",5 ) ){
			for( j = 1; j <= MTXSIZ; j++ ){
				cmgem.txsiz[j-1] = cmgem.otxsiz[j-1];
				}
			cmgem.txrat = cmgem.otxrat;
			}
		else if( lckey( "NEW$",5 ) ){
			for( j = 1; j <= MTXSIZ; j++ ){
				cmgem.txsiz[j-1] = cmgem.dtxsiz[j-1];
				}
			cmgem.txrat = cmgem.dtxrat;

			/* -- "size v":  define a new value for one of the text sizes. */
			}
		else if( lkrrc( "TINY$",6, 0.0, 1.0, &tmp ) ){ 
      cmgem.txsiz[0] = (float) tmp; 
    }
		else if( lkrrc( "SMALL$",7, 0.0, 1.0, &tmp ) ){
      cmgem.txsiz[1] = (float) tmp; 
    }
		else if( lkrrc( "MEDIUM$",8, 0.0, 1.0, &tmp ) ){
      cmgem.txsiz[2] = (float) tmp; 
    }
		else if( lkrrc( "LARGE$",7, 0.0, 1.0, &tmp ) ){
      cmgem.txsiz[3] = (float) tmp; 
    }
			/* -- "RATIO v":  set height/width character size ratio. */
		else if( lkreal( "RATIO$",7, &tmp ) ){
      cmgem.txrat = (float) tmp;
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

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    841105:  Original version.
	 *===================================================================== */

} /* end of function */

