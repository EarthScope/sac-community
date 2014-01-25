
#include <stdio.h>
#include <string.h>

#include "spe.h"
#include "amf.h"
#include "bool.h"


#include "msg.h"
#include "ucf.h"
#include "cpf.h"
#include "dbh.h"

void /*FUNCTION*/ xmem(nerr)
int *nerr;
{
	/*=====================================================================
	 * PURPOSE:  To execute the action command MEM.
	 *           This command calculates a spectral estimate using
	 *           the Maximum Entropy Method.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag.  Set to 0 if no error occurred.
	 *             Potential error numbers:  5005.
	 *=====================================================================
	 * MODULE/LEVEL:  SPE/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     SACMEM
	 *    SPE:     LCOR, NLNCOR, NDXCOR, NLNFFT, firstPowerOf2, CPREWH, NPREWH,
	 *             NDXSPE, NDXAUX
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SPE:     NLGMEM, KERMSG, LSPE, LRESL, LCL,
	 *             KSPEL1,KSPEL2, KSPEL3, NLNSPE
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LKINT, LKIRC, GTOUTM, SPECTR
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

		/* -- "ORDER n": set order of estimate in lags. */
		if( lkint( "ORDER$",7, &cmspe.nlgmem ) )
		{ /* do nothing */ }

		/* -- "NUMBER i":  set number of points in spectral estimate. */
		else if( lkirc( "NUMBER$",8, 512, cmspe.firstPowerOf2, &cmspe.nlnspe ) )
		    cmspe.nlnspe = next2( cmspe.nlnspe );

		/* -- Bad syntax. */
		else{
		    cfmt( "ILLEGAL OPTION:",17 );
		    cresp();
		}
	}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
	    goto L_8888;

	/* CHECKING PHASE: */

	/* - Make sure correlation function has been calculated. */

	if( !cmspe.lcor ){
	    *nerr = 5003;
	    setmsg( "ERROR", *nerr );
	    goto L_8888;
	}

	/* EXECUTION PHASE: */

	/* - Perform MEM function. */

	spectr( cmmem.sacmem[cmspe.ndxcor], cmspe.nlnfft, cmspe.nlncor, "MEM"
	 , &cmspe.nlgmem, cmspe.nlnspe, " ", cmspe.cprewh, cmspe.nprewh, 
	 cmmem.sacmem[cmspe.ndxspe], kmspe.kermsg,131, cmmem.sacmem[cmspe.ndxaux] );

	/* - Check for error. */

	if( memcmp(kmspe.kermsg,"        ",8) != 0 ){
	    *nerr = 5005;
	    setmsg( "ERROR", *nerr );
	    aplmsg( kmspe.kermsg,131 );
	    goto L_8888;
	}

	/* - Define globals pertaining to PSP plot. */

	cmspe.lspe = TRUE;
	cmspe.lresl = FALSE;
	cmspe.lcl = FALSE;
	strcpy( kmspe.kpspl1, "MEM             " );
        sprintf(kmspe.kpspl2,"LAGS: %5d",cmspe.nlgmem );
	strcpy( kmspe.kpspl3, "                " );

L_8888:
	return;


	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    850111:  Changed SPE id logic.
	 *    841227:  Changes due to major rewrite of SPE subprocess.
	 *    840113:  Changed subroutine MEM to MXEM.
	 *    821130:  Changed name from MEMCN and made minor mods.
	 *    810120:  Changed to output message retrieval from disk.
	 *    801211:  Changes and additions for plot id.
	 *    801024:  Modified argument list to MEM call.
	 *    800915:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850109
	 *===================================================================== */

} /* end of function */

