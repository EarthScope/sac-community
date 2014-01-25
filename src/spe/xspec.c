
#include "spe.h"
#include "bool.h"

#include "msg.h"
#include "dfm.h"
#include "cpf.h"

void /*FUNCTION*/ xspec(index, nerr)
int index, *nerr;
{



	/*=====================================================================
	 * PURPOSE: To execute a Spectral Estimation Subprocess command.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    INDEX:   The index number of the command.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 0910.
	 *=====================================================================
	 * MODULE/LEVEL: SPE/1
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SPE:     LSPEID
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  GTOUTM, XSPE, XCOR, XPCOR, XR, XPPE, XPDS, XMLM,
	 *             XMEM, XPSPE, XCLOG, XWCOR, XWSPE, XRCOR
	 *=====================================================================
	 * MODIFICATION HISTORY:
         *    050322:  Add PREVIOUS to SPEID.  (GRH/UB)
	 *    861203:  Added QUITSPE command.
	 *    850114:  Added READCOR command.
	 *    850108:  Added READ command.
	 *    841227:  Changes due to major rewrite of SPE subprocess.
	 *    821129:  Restructured subprocess execution logic.
	 *    811105:  Fixed bug in setting output argument LFIND.
	 *    810923:  Changed OPEN command to INIG.
	 *    810120:  Changed to output message retrieval from disk.
	 *    801219:  Added WSPE and WCOR commands.
	 *    801024:  Added SPEID command.
	 *    800925:  Moved file checking and memory management from CORCN.
	 *    800905:  Original version [Prime].
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850109
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Jump to correct command based upon its index number. */

	switch( index ){
		case 1: goto L_100;
		case 2: goto L_200;
		case 3: goto L_300;
		case 4: goto L_400;
		case 5: goto L_500;
		case 6: goto L_600;
		case 7: goto L_700;
		case 8: goto L_800;
		case 9: goto L_900;
		case 10: goto L_1000;
		case 11: goto L_1100;
		case 12: goto L_1200;
		case 13: goto L_1300;
		case 14: goto L_1400;
		case 15: goto L_1500;
	}

	/* - Error return if bad index value. */

	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XSPEC",9 );
	goto L_8888;


L_100:
	/* - Command 01: SPE ... initialize subprocess. */
	xspe( TRUE, nerr );
	goto L_8888;

L_200:
	/* - Command 02: COR ... compute cross-correlation function. */
	xcor( nerr );
	goto L_8888;

L_300:
	/* - Command 03: PLOTCOR ... plot cross-correlation function. */
	xpcor( nerr );
	goto L_8888;

L_400:
	/* - Command 04: READ ... read new data file into memory. */
	xr( nerr );
	if( *nerr != 0 )
		goto L_8888;
	xspe( FALSE, nerr );
	goto L_8888;

L_500:
	/* - Command 05: PLOTPE ... plot principle error. */
	xppe( nerr );
	goto L_8888;

L_600:
	/* - Command 06: PDS ... perform Power Density Spectral estimation. */
	xpds( nerr );
	goto L_8888;

L_700:
	/* - Command 07: MLM ... perform Maximum Likelihood Method estimation. */
	xmlm( nerr );
	goto L_8888;

L_800:
	/* - Command 08: MEM ... perform Maximum Entropy Method estimation. */
	xmem( nerr );
	goto L_8888;

L_900:
	/* - Command 09: PLOTSPE ... plot spectral estimate. */
	xpspe( nerr );
	goto L_8888;

L_1000:
	/* - Command 10: SPEID ... set plot id options. */
	xclog( &cmspe.lspeid, nerr );
	goto L_8888;

L_1100:
	/* - Command 11: WRITECOR ... writes autocorrelation function as SAC file. */
	xwcor( nerr );
	goto L_8888;

L_1200:
	/* - Command 12: WRITESPE ... write spectral estimate as SAC file. */
	xwspe( nerr );
	goto L_8888;

L_1300:
	/* - Command 13: READCOR ... reads autocorrelation function. */
	xrcor( nerr );
	goto L_8888;

L_1400:
	/* - Command 14: QUITSUB ... quit (terminate) subprocess. */
	xquitspe( nerr );
	goto L_8888;

L_1500:
	/* - Command 15: WHITEN (PREWHITEN) ... adds white noise to data */
	xwhiten( nerr ) ;
	goto L_8888;

L_8888:
	return;

} /* end of function */

