
#include <string.h>

#include "spe.h"
#include "amf.h"
#include "bool.h"

#include "co.h"
#include "msg.h"
#include "icm.h"
#include "cpf.h"
#include "dbh.h"

void /*FUNCTION*/ xcor(nerr)
int *nerr;
{
	char temp[ 131 ] ;
	int nlnuse;
	float samint;

  float ridge_fac = .00001;
  double tmp;
      
	/*=====================================================================
	 * PURPOSE: To parse and execute the action command COR.
	 *          This command computes the auto-correlation function.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001, 5005, 5006.
	 *=====================================================================
	 * MODULE/LEVEL:  SPE/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    SPE:     SAMFRQ, NDXDAT, NLNDAT, KWINTP, MWINTP, NDXCOR, NDXAUX,
	 *             NLNCOR, NLNFFT
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SPE:     LSNUMW, NUMWIN, WINLEN, IWNCOR, LPREWH, NPRERQ,
	 *             NPREWH, CPREWH, KSCALE, LCOR, NWINLN, LSPE, KERMSG
	 *    DFM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LKLIST, LKREAL, LKLOGI, LCKEY,
	 *             PREWIT, AUTCOR, INDEXB
	 *=====================================================================
	 * LOCAL VARIABES:
	 *    NLNUSE:  Number of data points to use in correlation. [i]
	 *             This is less than the number of points in the file by
	 *             the number of coefficients in the prewhitening filter.
	 *             This is because prewhitening corrupts these data points.
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

	    /* -- "NUMBER n":  set number of correlation windows. */
	    if( lklogi( "NUMBER$",8, &cmspe.lsnumw, &cmspe.numwin ) )
	    { /* do nothing */ }

	    /* -- "LENGTH v":  set length of correlation window (in seconds.) */
	    else if( lkreal( "LENGTH$",8, &tmp ) ) {
        cmspe.winlen = (float) tmp;
      }
	    /* -- "TYPE c":  set window type. */
	    else if( lklist( "TYPE$",6, (char*)kmspe.kwintp,9, MWINTP, 
	     &cmspe.iwncor ) )
	    { /* do nothing */ }

	    /* -- "PREWHITEN ON/OFF/n":  turn prewhitening on or off. */
	    else if( lklogi( "PREWHITEN$",11, &cmspe.lprewh, &cmspe.nprerq ) )
	    { /* do nothing */ }

	    /* -- "STOCHASTIC/TRANSIENT":  set type of scaling. */
	    else if( lckey( "STOCHASTIC$",12 ) )
		strcpy( kmspe.kscale, "STOCHASTIC" );

	    else if( lckey( "TRANSIENT$",11 ) )
		strcpy( kmspe.kscale, "TRANSIENT " );

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	} /* end while */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
	    goto L_8888;

	/* CHECKING PHASE: */

	if( !cmspe.lfile ){
	    *nerr = 5006;
	    setmsg( "ERROR", *nerr );
	    goto L_8888;
	}

	/* EXECUTION PHASE: */

	/* - Turn correlation flag off until signal has been processed. */

	cmspe.lcor = FALSE;

	/* - Prewhiten data if requested.
	 *   Adjust data array start and length to account for prewhitening. */

	if( cmspe.lprewh ){
	    cmspe.nprewh = min( cmspe.nprerq, MPREWH );
	    temp[ 0 ] = '\0' ;
	    prewit( cmmem.sacmem[cmspe.ndxdat], cmspe.nlndat, &cmspe.nprewh, 
	     cmspe.cprewh, NULL, temp );

	    if( temp[ 0 ] )
		memcpy( kmspe.kermsg, temp, strlen( temp ) ) ;

	    if( memcmp(kmspe.kermsg,"        ",8) != 0 ){
		*nerr = 5005;
		setmsg( "ERROR", *nerr );
		aplmsg( kmspe.kermsg,131 );
		goto L_8888;
	    }
	}
	else{
	    cmspe.nprewh = 0;
	}

	/* - Compute the number of points to use in the correlation. */

	nlnuse = cmspe.nlndat - cmspe.nprewh;

	/* - Compute number of samples in a correlation window. */

	cmspe.nwinln = min( (int)( cmspe.samfrq*cmspe.winlen + 0.5 ), 
	 nlnuse );

	/* - Compute the number of windows if user did not specify. */

	if( !cmspe.lsnumw )
	    cmspe.numwin = nlnuse/cmspe.nwinln;

	/* - Perform correlation function. */

	samint = 1./cmspe.samfrq;
	autcor( cmmem.sacmem[cmspe.ndxdat]+cmspe.nprewh, samint, nlnuse, 
	 cmspe.numwin, cmspe.nwinln, (char*)kmspe.kwintp[cmspe.iwncor - 1]
	 , kmspe.kscale, cmmem.sacmem[cmspe.ndxcor], &cmspe.nlnfft, &cmspe.nlncor, 
	 kmspe.kermsg,131, cmmem.sacmem[cmspe.ndxaux] , ridge_fac);

	/* - Check for error in autocorrelation. */

	if( memcmp(kmspe.kermsg,"        ",8) != 0 ){
	    *nerr = 5005;
	    setmsg( "ERROR", *nerr );
	    aplmsg( kmspe.kermsg,131 );
	    goto L_8888;
	}

	/* - Turn correlation flag on and spectral estimation flag off. */

	cmspe.lcor = TRUE;
	cmspe.lspe = FALSE;

	/* set default window length for PDS.  maf 980522 */
	cmspe.nlgpds = (int)( cmspe.winlen * cmspe.samfrq ) ;

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    850215:  Fixed bug involving window length and prewhitening.
	 *    841227:  Changes due to major rewrite of SPE subprocess.
	 *    821129:  Combined CORPS and CORCN.
	 *    810120:  Changed to output message retrieval from disk.
	 *    801211:  Changes and additions for plot id.
	 *    800925:  Moved logic for file checking and memory allocation
	 *             to SPEPRO subroutine.
	 *    800911:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850215
	 *===================================================================== */

} /* end of function */

