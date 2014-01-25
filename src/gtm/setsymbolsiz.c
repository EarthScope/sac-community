
#include "gtm.h"
#include "co.h"


#include "gdm.h"

void /*FUNCTION*/ setsymbolsize(size)
double size;
{
	float chht, chwid;



	/*=====================================================================
	 * PURPOSE: To set the symbol size attribute.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    size:    The new symbol size attribute. [f]
	 *             A non-positive value sets size to current character size.
	 *             Normal range is 0. to 1.
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gtm:     symsz
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  gettextsize
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861022:  Moved from gem to gtm.
	 *    810721:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861022
	 *===================================================================== */
	/* PROCEDURE: */
	if( size <= 0. ){
		gettextsize( &chwid, &chht );
		cmgtm.symsz = chht;
		}
	else{
		cmgtm.symsz = fmin( size, 1.0 );
		}

       
	return;

} /* end of function */

