#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mach.h"
#include "gam.h"
#include "hdr.h"

#include "bool.h"


#include "dfm.h"

void getxlm(lxlm, xmin, xmax)
int *lxlm;
float *xmin, *xmax;
{
	int nerr, nlnatw, nofmin;
  double tmin, tmax;


	/*=====================================================================
	 * PURPOSE:  To return x axis plot limit attributes for current file.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    LXLM:    Set to .TRUE. if fixed x limits option is on. [l]
	 *    XMIN:    Minimum x limit if LXLM is .TRUE. [f]
	 *             The file's begin time is returned if LXLM is .FALSE.
	 *    XMAX:    Maximum x limit if LXLM is .TRUE. [f]
	 *             The file's end time is returned if LXLM is .FALSE.
	 *=====================================================================
	 * MODULE/LEVEL:  GAM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GAM:     LRTWXL, KRTWXL, ORTWXL
	 *    HDR:     B, E
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - XLIM command and SETXLM subroutine defines LRTWXL, KRTWXL, and ORTWXL.
	 * - INQXLM returns current values for above variables.
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  GETATW
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Set x limit option flag. */
	*lxlm = cmgam.lrtwxl;

	/* - If option is on, determine x limits. */

	if( *lxlm ){
		getatw( (char*)kmgam.krtwxl,9, cmgam.ortwxl, &tmin, &tmax, &nofmin, 
		 &nlnatw, &nerr );
    *xmin = (float) tmin;
    *xmax = (float) tmax;
		if( nerr != 0 )
			*lxlm = FALSE;
		}

	/* - Return begin and end times if option is off or an error occurred. */

	if( !*lxlm ){
		*xmin = *b;
		*xmax = *e;
		}

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    860304:  Changes in argument list of GETATW.
	 *    820623:  Now uses GETATW to perform calculation.
	 *    810120:  Removed check for logic error.
	 *    800723:  Original version.
	 *===================================================================== */

} /* end of function */

