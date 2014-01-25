
#include <stdio.h>
#include <string.h>

#include "spe.h"
#include "amf.h"
#include "bool.h"


#include "msg.h"
#include "co.h"
#include "ucf.h"
#include "cpf.h"
#include "dbh.h"

void /*FUNCTION*/ xpds(nerr)
int *nerr;
{
	double secpds;

	/*=====================================================================
	 * PURPOSE: To parse and execute the action command PDS.
	 *          This command calculates a spectral estimate using
	 *          the Power Density Spectrum method.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001, 5005.
	 *=====================================================================
	 * MODULE/LEVEL:  SPE/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *    SPE:     NLNCOR, KWINTP, MWINTP, SAMFRQ, NDXCOR, firstPowerOf2,
	 *             CPREWH, NPREWH, NDXAUX, NLNFFT
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    DFM:     SACMEM
	 *    SPE:     IWNPDS, SECPDS, KERMSG, LSPE, LRESL,
	 *             LCL, KPSPL1, KSPEL2, KSPEL3, NLNSPE
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LKLIST, LKREAL, LCLOG2, LKIRC, SPECTR
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

	    /* -- "SECONDS v": set window length in seconds. */
    if( lkreal( "SECONDS$",9, &secpds ) ) {
      cmspe.nlgpds = (int)( secpds*cmspe.samfrq );
    }
	    /* -- "LAGS n": set window length in lags. */
	    else if( lkint( "LAGS$",6, &cmspe.nlgpds ) )
	    { /* do nothing */ }

	    /* -- "NUMBER i":  set number of points in spectral estimate. */
	    else if( lkirc( "NUMBER$",8, 512, cmspe.firstPowerOf2, &cmspe.nlnspe ) )
		cmspe.nlnspe = next2( cmspe.nlnspe );

	    /* -- "TYPE c":  set window type. */
	    else if( lklist( "TYPE$",6, (char*)kmspe.kwintp,9, MWINTP, 
	     &cmspe.iwnpds ) ) { /* do nothing */ }

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

	/* - Perform PDS function. */

	spectr( cmmem.sacmem[cmspe.ndxcor], cmspe.nlnfft, cmspe.nlncor, "PDS"
	 , &cmspe.nlgpds, cmspe.nlnspe, (char*)kmspe.kwintp[cmspe.iwnpds - 1]
	 , cmspe.cprewh, cmspe.nprewh, cmmem.sacmem[cmspe.ndxspe], kmspe.kermsg
	 ,131, cmmem.sacmem[cmspe.ndxaux] );

	/* - Check for error. */

	if( memcmp(kmspe.kermsg,"        ",8) != 0 ){
	    *nerr = 5005;
	    setmsg( "ERROR", *nerr );
	    aplmsg( kmspe.kermsg,131 );
	    goto L_8888;
	}

	/* - Define globals pertaining to PSP plot. */

	cmspe.lspe = TRUE;
	cmspe.lresl = TRUE;
	cmspe.lcl = TRUE;
	strcpy( kmspe.kpspl1, "PDS             " );
	secpds = (float)( cmspe.nlgpds )/cmspe.samfrq;
  sprintf(kmspe.kpspl2,"SECONDS %5.1lf", secpds );
  fstrncpy( kmspe.kpspl3, 16, "TYPE ", 5);
  fstrncpy( kmspe.kpspl3+5, 16-5, kmspe.kwintp[cmspe.iwnpds - 1],
            strlen(kmspe.kwintp[cmspe.iwnpds - 1]));

L_8888:

	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    850111:  Changed SPE id logic.
	 *    841227:  Changes due to major rewrite of SPE subprocess.
	 *    821130:  Combined PDSPS and PDSCN.
	 *             Changed to new command parsing logic.
	 *    800915:  Original version.
	 *    810120:  Changed to output message retrieval from disk.
	 *    801211:  Changes and additions for plot id.
	 *    801024:  Modified argument list to PDS call.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850109
	 *===================================================================== */


} /* end of function */

