
#include <stdlib.h>

#include "spe.h"
#include "amf.h"


#include "cpf.h"
#include "exm.h"

void /*FUNCTION*/ xquitspe(nerr)
int *nerr;
{
	/*=====================================================================
	 * PURPOSE:  To execute the QUITSUB command in the Spectral Estimation
	 *           Subprocess.  This command terminates the subprocess and
	 *           returns to the main SAC program.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 5002, 1302
	 *=====================================================================
	 * MODULE/LEVEL: SPE/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    SPE:     NDXCOR
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

	/* - Reset firstPowerOf2 for use in SPECTROGRAM */
	cmspe.firstPowerOf2 = MINPOW ;

	/* - Release data blocks used in this subprocess. */

	if( cmmem.sacmem[cmspe.ndxcor] != NULL )
		relamb( cmmem.sacmem, cmspe.ndxcor, nerr );
	if( *nerr != 0 )
		goto L_8888;

	if( cmmem.sacmem[cmspe.ndxpe] != NULL )
		relamb( cmmem.sacmem, cmspe.ndxpe, nerr );
	if( *nerr != 0 )
		goto L_8888;

	if( cmmem.sacmem[cmspe.ndxspe] != NULL )
		relamb( cmmem.sacmem, cmspe.ndxspe, nerr );
	if( *nerr != 0 )
		goto L_8888;

	if( cmmem.sacmem[cmspe.ndxaux] != NULL )
		relamb( cmmem.sacmem, cmspe.ndxaux, nerr );
	if( *nerr != 0 )
		goto L_8888;

L_8888:
	return;

} /* end of function */

