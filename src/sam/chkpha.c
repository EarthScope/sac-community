
#include <math.h>

#include "sam.h"
#include "bool.h"
#include "co.h"

#define	L	(ipow(2,12))

#define	PI	M_PI

#define	TWOPI	(2.*PI)

struct t_cmunwr {
  float thlinc, thlcon;
  int nfft;
  float con1, dvtmn2;
} cmunwr;

void /*FUNCTION*/ chkpha(ph, pv, iscons)
float *ph;
double pv;
int *iscons;
{
	float a0, a1, a2, a3, a4;

	/*=====================================================================
	 * PURPOSE:
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/3
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
	/* PROCEDURE:
	 *
	 *-----------------------------------------------------------------------
	 * SUBROUTINE: CHKPHA
	 * SUBROUTINE TO CHECK CONSISTENCY OF A PHASE ESTIMATE
	 *-----------------------------------------------------------------------
	 *
	 *
	 * DESCRIPTION OF ARGUMENTS:
	 *
	 *     PH = PHASE ESTIMATE
	 *     PV = PRINCIPLE VALUE OF PHASE AT FREQUENCY
	 *         OF PHASE ESTIMATE
	 * ISCONS = .FALSE. IF PHASE ESTIMATE NOT CONSISTENT;
	 *         .TRUE. IF PHASE ESTIMATE CONSISTENT,
	 *         PHASE RETURNED IN PH
	 * */

	/* FIND THE TWO ADMISSIBLE PHASE VALUES CLOSEST TO PH
	 * */
	a0 = (*ph - pv)/TWOPI;
	a1 = (float)( (int)( a0 ) )*TWOPI + pv;
	a2 = a1 + sign( TWOPI, a0 );
	a3 = fabs( a1 - *ph );
	a4 = fabs( a2 - *ph );

	/* CHECK CONSISTENCY
	 * */
	*iscons = FALSE;
	if( a3 > cmunwr.thlcon && a4 > cmunwr.thlcon )
		return;
	*iscons = TRUE;

	/* FIND THE CLOSEST UNWRAPPED PHASE ESTIMATE
	 * */
	*ph = a1;
	if( a3 > a4 )
		*ph = a2;
	return;
	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    810000:  Original version.
	 *===================================================================== */

} /* end of function */

