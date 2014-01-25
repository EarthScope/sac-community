#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "co.h"
#include "eam.h"
#include "gam.h"


void /*FUNCTION*/ pkfunc(float fdold, float fdnew, float *chfsta, float *chflta, float *chf) {

	float fdfd;

	/*=====================================================================
	 * PURPOSE: To calculate characteristic function and its averages
	 *          by automatic picker.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    FDOLD:   Old (last) value of filtered trace. [f]
	 *    FDNEW:   New (current) value of filtered trace. [f]
	 *    CHFSTA:  Old short term average of characteristic function. [f]
	 *    CHFLTA:  Old long term average of characteristic function. [f]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    CHFSTA:  New short term average. [f]
	 *    CHFLTA:  New long term average. [f]
	 *    CHF:     New value of characteristic function. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  EAM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    EAM:     C2, C3, C5,
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Compute first difference of filtered data. */
	fdfd = fdnew - fdold;

	/* - Compute characteristic function. */

	*chf = pow(fdnew,2) + cmeam.c2*pow(fdfd,2);

	/* - Compute the short term average. */

	*chfsta = *chfsta + cmeam.c3*(*chf - *chfsta);

	/* - Compute the long term average. */

	*chflta = *chflta + cmeam.c4*(*chf - *chflta);

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    801101:  Factored from PK1.
	 *===================================================================== */

} /* end of function */

