
#include <string.h>

#include "sss.h"
#include "hdr.h"
#include "amf.h"
#include "bool.h"

#include "smDataIO.h"


#include "dfm.h"
#include "ucf.h"
extern float *sss_sum;

void /*FUNCTION*/ xzerostack(nerr)
int *nerr;
{
	char kvm[9], *worksetName ;



	/*=====================================================================
	 * PURPOSE: To parse the parameter-setting command ZEROSTACK.
	 *          This command zeroes or reinitializes the signal stack.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag.  Set to 0 if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    hdr:     fundef
	 *    sss:     ndxsum
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:     dlytg, dlytig, dlyng, dlyngi, wtg, dstg, lpolg,
	 *             kvm, dlyvm, ndxsum, nlnsum
	 *    mem:     isacmem
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  fill, cleardfl
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    881130:  Added call to release summation data block if necessary.
	 *    881122:  Changed initial value of dstg.
	 *    870227:  Changed call to INIAM to CLEARDFL.
	 *    860407:  Added call to INIAM to initialize the SACMEM array.
	 *    850809:  Changes due to major rewrite of SSS.
	 *    821207:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850809
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Reset global property values to their default values. */

	cmsss.dlytg = 0.0;
	cmsss.dlytig = 0.0;
	cmsss.dlyng = 0.0;
	cmsss.dlynig = 0.0;
	cmsss.wtg = 1.0;
	cmsss.dstg = SAC_FLOAT_UNDEFINED;
	cmsss.lpolg = TRUE;
	strcpy( kvm, "     DLY" );
	fill( cmsss.dlyvm, MSFL, 0.0 );

	/* - Release summation data block if necessary. */

	if( (cmsss.ndxsum > 0) && sss_sum != NULL) {
    FREE(sss_sum);
		cmsss.ndxsum = 0;
		cmsss.nlnsum = 0;
		if( *nerr != 0 )
			goto L_8888;
		}

	/* - Clear data file list storage. */

	sacclear();

	/* delete files from SeisMgr */
	worksetName = smGetDefaultWorksetName () ;
	if ( worksetName )
	    smDeleteWorksetByName ( worksetName ) ;

L_8888:
	return;

} /* end of function */

