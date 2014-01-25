
#include "gtm.h"
#include "bool.h"
#include "co.h"

void /*FUNCTION*/ setsymbolnum(number)
int number;
{
	int ilast;



	/*=====================================================================
	 * PURPOSE: To set the symbol number attribute.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    number:  The new symbol number attribute. [i]
	 *             There are 16 available symbols.
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gtm:     msym, munsym, isyml1, nsymlc, msisym
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gtm:     isym, lscsym, jsyml1, jsym1b, jsym1e,
	 *             ldbsym, jsyml2, jsym2b, jsym2e
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861022:  Moved from gem to gtm.
	 *    850610:  Moved new symbol logic from psym.
	 *    850328:  Fixed bug involving zero or negative argument.
	 *    811117:  Removed on/off logic and put it in ssymst.
	 *    810721:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861022
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Temporarily save previous value of symbol number. */
	ilast = cmgtm.isym;

	/* - Make sure new symbol number is positive. */

	cmgtm.isym = max( 1, number );

	/* - Calculate new table indices if symbol has changed since last call. */

	if( cmgtm.isym != ilast ){
		cmgtm.isym = number%MSYM;
		if( cmgtm.isym == 0 )
			cmgtm.isym = MSYM;

		/* -- Flag for scaled/unscaled symbol. */
		if( cmgtm.isym <= MUNSYM ){
			cmgtm.lscsym = FALSE;
			}
		else{
			cmgtm.lscsym = TRUE;
			}

		/* -- Indices for simple symbol. */
		cmgtm.jsyml1 = cmgtm.isyml1[cmgtm.isym - 1];
		cmgtm.jsym1b = cmgtm.nsymlc[cmgtm.jsyml1 - 1];
		cmgtm.jsym1e = cmgtm.nsymlc[cmgtm.jsyml1] - 1;

		/* -- Flag and indices for a single/double symbol. */
		if( cmgtm.isym <= MSISYM ){
			cmgtm.ldbsym = FALSE;
			}
		else{
			cmgtm.ldbsym = TRUE;
			cmgtm.jsyml2 = cmgtm.isyml2[cmgtm.isym - 1];
			cmgtm.jsym2b = cmgtm.nsymlc[cmgtm.jsyml2 - 1];
			cmgtm.jsym2e = cmgtm.nsymlc[cmgtm.jsyml2] - 1;
			}
		}

       
	return;

} /* end of function */

