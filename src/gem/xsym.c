
#include "gem.h"
#include "bool.h"


#include "gtm.h"
#include "cpf.h"

void /*FUNCTION*/ xsym(nerr)
int *nerr;
{
	int int_;
  double swidth, tmp;

	/*=====================================================================
	 * PURPOSE:  To execute the parameter-setting command SYMBOL.
	 *           This command controls the symbol plotting attributes.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GEM:     CHWID, MSYM, MISYM
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GEM:     LSYM, ISYM, SYMSZ, SYMSP, LISYM, NISYM, IISYM, JISYM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *     SACLIB: LCMORE, CFMT, CRESP, LCLOG, LCINT, LKRRC, LKLOG, LCKEY,
	 *             INISYM, SETSYMBOLNUM
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Parse positional tokens first: */

	/* -- "ON/OFF":  turn on/off symbol plotting. */
	if( lclog( &cmgem.lsym ) ){

		/* "n":  turn on symbol plotting and set symbol number. */
		}
	else if( lcirc( 1, MSYM, &cmgem.isym ) ){
		setsymbolnum( cmgem.isym );
		cmgem.lsym = TRUE;

		}

	/* - Parse position-independent tokens: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "SIZE v":  change the symbol size attribute. */
		if( lkrrc( "SI$",4, 0., 1., &tmp ) ){
      cmgem.symsz = (float) tmp;
			setsymbolsize( cmgem.symsz );
    }
			/* -- "WIDTH v":  change the symbol line width */
		else if( lkrrc( "WI$",4, 0., 10., &swidth ) ){
      cmgem.isymwidth = (int) (swidth + 0.1);
      cmgem.lwidth = TRUE;
    }
			/* -- "SPACING v":  change the symbol gap attribute. */
		else if( lkrrc( "SP$",4, 0., 1., &tmp ) ){
      cmgem.symsp = (float) tmp;
			setsymbolgap( cmgem.symsp );
    }
    /* -- "INCREMENT ON/OFF":  turn symbol incrementing on or off. */
		else if( lklog( "I$",3, &cmgem.lisym ) ){
			cmgem.lsym = TRUE;
			cmgem.jisym = 0;

			/* -- "LIST STANDARD/n ...":  change list of symbol numbers to use. */
			}
		else if( lckey( "L$",3 ) ){
			if( lckey( "S$",3 ) ){
				inisym( cmgem.iisym, &cmgem.nisym );
				}
			else{
				cmgem.nisym = 0;
L_1100:
				if( lcint( &int_ ) ){
					if( cmgem.nisym < MISYM )
						cmgem.nisym = cmgem.nisym + 1;
					cmgem.iisym[cmgem.nisym-1] = int_;
					goto L_1100;
					}
				if( cmgem.nisym <= 0 )
					inisym( cmgem.iisym, &cmgem.nisym );
				cmgem.isym = cmgem.iisym[1-1];
				setsymbolnum( cmgem.isym );
				cmgem.lsym = TRUE;
				cmgem.jisym = 0;
				}

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();
			}

		goto L_1000;
		}

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    850611:  Symbol now set by call to SETSYMBOLNUM.
	 *    830114:  Added LIST option.
	 *    820817:  Changed to newest set of parsing and checking functions.
	 *    811207:  Changed logic to account for positional tokens.
	 *    810723:  Original version.
	 *===================================================================== */

} /* end of function */

