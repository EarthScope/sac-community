
#include "gem.h"
#include "bool.h"


#include "gdm.h"
#include "pl.h"
#include "cpf.h"

void /*FUNCTION*/ xwidth(nerr)
int *nerr;
{
	int inum;

	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command WIDTH.
	 *          WIDTH controls the line-width display attributes.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    GEM:     MWIDTH
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GEM:     LWIDTH, LIWIDTH, IWIDTH, IIWIDTH, ISKWIDTH,
	 *             NIWIDTH, JIWIDTH
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCLOG
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *  920526: Original version - implemented from COLOR command.
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;
    inum = 1;
	/* - Parse position-dependent tokens: */

	/* -- Turning WIDTH ON/OFF toggles current widths with the thin width. */
	if( lclog( &cmgem.lwidth ) ){
		if( cmgem.lwidth ){
			cmgem.iwidth = cmgem.iswidth;
			cmgem.iskwidth = cmgem.isskwidth;
			}
		else{
			cmgem.lwidth = TRUE;
			setlinewidth( LINE_WIDTH_THIN );
			cmgem.lwidth = FALSE;
			cmgem.iswidth = cmgem.iwidth;
			cmgem.isskwidth = cmgem.iskwidth;
			}

		}
	else if( lcint(&inum) ) {
    cmgem.iwidth = inum;
    cmgem.lwidth = TRUE;
  }

	/* - Parse position-independent tokens: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "SKELETON width":  change skeleton width. */
		if( lckey( "SK$",4 ) || lckey( "BO$",4 ) ){
			if( lcint( &inum ) ) {
				cmgem.iskwidth = inum;
				cmgem.lwidth = TRUE;
      }
			else{
				cfmt( "NEED SKELETON WIDTH VALUE:",28 );
				cresp();
      }

			/* -- "LIST STANDARD/widthlist":  change the width list. */
    }
		else if( lckey( "L$",3 ) ){
			if( lckey( "S$",3 ) ){
				iniwidth();
      }
			else{
				cmgem.niwidth = 0;
L_1100:
				if( lcint(&inum) ) {
          if( cmgem.niwidth < MWIDTH )
            cmgem.niwidth = cmgem.niwidth + 1;
          cmgem.iiwidth[cmgem.niwidth-1] = inum;
          goto L_1100;
        }
				if( cmgem.niwidth <= 0 )
					iniwidth();
				cmgem.iwidth = cmgem.iiwidth[1-1];
				cmgem.lwidth = TRUE;
				cmgem.jiwidth = 0;
      }

			/* -- "INCREMENT ON/OFF":  increment width after each file or not. */
    }
		else if( lklog( "I$",3, &cmgem.liwidth ) ){
			cmgem.lwidth = TRUE;
			cmgem.jiwidth = 0;

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

