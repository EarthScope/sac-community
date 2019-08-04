
#include <stdlib.h>

#include "spe.h"
#include "amf.h"

#include "cpf.h"
#include "exm.h"
#include "defs.h"

SPE_EXTERN

extern float *specor;
extern float *spepe;
extern float *spespe;
extern float *speaux;

void /*FUNCTION*/
xquitspe(nerr)
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

    setcomlist(1);
    setprompt("SAC>", 5);

    /* - Reset firstPowerOf2 for use in SPECTROGRAM */
    cmspe.firstPowerOf2 = MINPOW;

    /* - Release data blocks used in this subprocess. */

    FREE(specor);
    FREE(spepe);
    FREE(spespe);
    FREE(speaux);

    return;

}                               /* end of function */
