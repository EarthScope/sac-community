
#include "sss.h"
#include "amf.h"


#include "cpf.h"
#include "exm.h"

void /*FUNCTION*/ xquitsss(nerr)
int *nerr;
{



	/*=====================================================================
	 * PURPOSE:  To execute the QUITSUB command in the Signal Stacking
	 *           Subprocess.  This command terminates the subprocess
	 *           and returns to the make SAC program.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL: SSS/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    SSS:     NDXSUM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  RELAMB, SETCOMLIST, SETPROMPT
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861203:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850801
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Change to the standard command list and default prompt. */

	setcomlist( 1 );
	setprompt( "SAC>",5 );

	/* - Release data blocks used in this subprocess. */

	if( cmsss.nlnsum > 0 ){
		relamb( cmmem.sacmem, cmsss.ndxsum, nerr );
		cmsss.nlnsum = 0;
		cmsss.ndxsum = 0;
		if( *nerr != 0 )
			goto L_8888;
		}

L_8888:
	return;

} /* end of function */

