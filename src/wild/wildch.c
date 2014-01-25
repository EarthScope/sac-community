
#include <string.h>

#include "wild.h"


#include "co.h"

void /*FUNCTION*/ wildch(xsngl, xmult, xccon)
char xsngl, xmult, *xccon;
{

	/*==============================================================
	 * PURPOSE:  To set the various key characters for pattern matching.
	 *==============================================================
	 * INPUT ARGUMENTS:
	 *     XSNGL:  Single wild character
	 *     XMULT:  Multiple wild character
	 *     XCCON:  Beginning and end characters defining concatenation group
	 *==============================================================
	 * GLOBAL OUTPUT:
	 *    WILD:  SNGL, MULT, CCON
	 *==============================================================
	 * MODIFICATION HISTORY:
	 *    870619:  Deleted iteration option.
	 *    850000:  Original version.
	 *==============================================================
	 * DOCUMENTED/REVIEWED:  870619
	 *============================================================== */
	/* PROCEDURE: */
	kmwild.sngl = xsngl;
	kmwild.mult = xmult;
	fstrncpy( kmwild.ccon, 2, xccon, strlen(xccon));

	cmwild.igcon = 0;

       
	return;

} /* end of function */

