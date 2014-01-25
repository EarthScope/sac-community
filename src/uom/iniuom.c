
#include <string.h>

#include "uom.h"
#include "bool.h"

void /*FUNCTION*/ iniuom()
{
	int j;

	/*=====================================================================
	 * PURPOSE: Variable initialization of common block CMUOM.
	 *=====================================================================
	 * MODULE/LEVEL:  UOM/4
	 *=====================================================================
	 * PARAMETERS:
	 *    MUOCON:  Length of unary constant arrays. [i]
	 *    MDIFTP:  Number of differentiator types in DIF command. [i]
	 *=====================================================================
	 * VARIABLE DEFINITIONS:
	 *    CONADD:  Constants for unary addition command. [f]
	 *    CONSUB:  Constants for unary subtraction command. [f]
	 *    CONMUL:  Constants for unary multiplication commmaand. [f]
	 *    CONDIV:  Constants for unary division command. [f]
	 *    KDIFTP:  Array of differentiator types for DIF command. [k]
	 *    IDIFTP:  Index in KDIFTP array of current DIF type. [i]
	 *    NDIFFC:  Number of coefficients to use in DIFF filter. [i]
	 *             This filter simulates the differential operator.
	 *    LTRAP:   Integration technique flag used in INT command. [l]
	 *===================================================================== */
	/* PROCEDURE: */
	/* - ADD, SUB, MUL, and DIV commands. */
	for( j = 1; j <= MUOCON; j++ ){
        Conadd[j] = 0.;
		Consub[j] = 0.;
		Conmul[j] = 1.;
		Condiv[j] = 1.;
		}

	/* - DIF command. */

	strcpy( kmuom.kdiftp[0], "TWO     " );
	strcpy( kmuom.kdiftp[1], "THREE   " );
	strcpy( kmuom.kdiftp[2], "FIVE    " );
	cmuom.ndiftp = 3;
	cmuom.idiftp = 1;

	/* - INT command. */

	cmuom.ltrap = TRUE;

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    901218:  Added initialization of INT command ltrap variable.
	 *    831024:  Added initialization for DIF command.
	 *    810414:  Original version.
	 *===================================================================== */

} /* end of function */

