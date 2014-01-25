#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "co.h"
#include "gam.h"
#include "gtm.h"
#include "debug.h"

void pcxops(int iop1, int iop2, float xloc[], float yloc[], double degi)
{
	float deg1, deg2, radius, temp;

	float *const Xloc = &xloc[0] - 1;
	float *const Yloc = &yloc[0] - 1;
  UNUSED(iop1);

	/* Ind
	 *=====================================================================
	 * PURPOSE:
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *=====================================================================
	 * MODULE/LEVEL:
	 *=====================================================================
	 * GLOBAL INPUT:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *=====================================================================
	 * GLOBAL COUPLING:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * ASSUMPTIONS:
	 *=====================================================================
	 * LIMITATIONS:
	 *=====================================================================
	 * KNOWN ERRORS:
	 *=====================================================================
	 * EXTERNAL DEPENDENCIES:
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Compute radius of sector.  This is distance between
	 *   first two data points. */
	radius = sqrt( powi(Xloc[2] - Xloc[1],2) + powi(Yloc[2] - Yloc[1],2) );

	/* - Compute sector angles.  These are the angles between the second
	 *   and third data points and the first. */

	deg1 = TODEG*atan2( Xloc[2] - Xloc[1], Yloc[2] - Yloc[1] );
	deg2 = TODEG*atan2( Xloc[3] - Xloc[1], Yloc[3] - Yloc[1] );

	/* - Swap angles if needed to insure that DEG2 > DEG1. */

	if( deg2 < deg1 ){
		temp = deg1;
		deg1 = deg2;
		deg2 = temp;
		}

	/* - There are two possible cases:
	 *   (1) Difference in two angles is less than 180 degrees.
	 *       (A) Normal arc is requested.
	 *       (B) Complement arc is requested.
	 *   (2) Difference in two angles is greater than 180 degrees.
	 *       (A) Normal arc is requested.
	 *       (B) Complement arc is requested. */

	/* Cases (1.A) and (2.B) are identical.  The arc we want is the one
	 * clockwise from DEG1 to DEG2.
	 * The other two cases are also identical.  The arc we want is the
	 * one clockwise from DEG2 to DEG1.  We must add 360 degres to DEG1
	 * so that it is numerically greater than DEG2. */

	if( ((deg2 - deg1) < 180. && iop2 == 1) || ((deg2 - deg1) >= 180. && 
	 iop2 == 2) ){
		worldsector( Xloc[1], Yloc[1], radius, deg1, deg2, degi );
		}
	else{
		worldsector( Xloc[1], Yloc[1], radius, deg2, deg1 + 360., 
		 degi );
		}

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    821015:  Original version.
	 *===================================================================== */

} /* end of function */

