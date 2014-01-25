
#include <string.h>

#include "spe.h"
#include "dfm.h"
#include "amf.h"
#include "icm.h"
#include "bool.h"

#include "co.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"

void /*FUNCTION*/ xwhiten(nerr)
int *nerr;
{
	int nprerq;
	/* names for the FD option */
	char kname[ MCPFN + 10 ] ;
	char temp[ 131 ] ;
    char *tmp;

	/*=====================================================================
	 * PURPOSE: To parse and execute the action command WHITEN.
	 *          This command adds white noise to the data. 
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001, 5005, 5006.
	 *=====================================================================
	 * MODULE/LEVEL:  SPE/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    SPE:     NDXDAT, NLNDAT
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SPE:     NPRERQ, NPREWH, CPREWH, LCOR, LSPE, KERMSG
	 *    DFM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, PREWIT
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

	    /* -- order  */
	    if ( lcint ( &nprerq ) ) {
		cmspe.nprerq = nprerq ;
	    }

	    /* FILTERDESIGN: pass whiten coefficients into filterdesign */
	    else if ( lckey ( "FILTERDESIGN #$" , 16 ) || lckey ( "FD #$" , 6 ) ) 
		cmicm.lfd = TRUE ;

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	} /* end while ( lcmore ( nerr ) */

	if( *nerr != 0 )
	    goto L_8888;

	/* CHECKING PHASE: */

	if( !cmspe.lfile ){
	    *nerr = 5006;
	    setmsg( "ERROR", *nerr );
	    goto L_8888;
	}

	/* EXECUTION PHASE: */

	/* - Prewhiten data if requested.
	 *   Adjust data array start and length to account for prewhitening. */

	cmspe.nprewh = min( cmspe.nprerq, MPREWH );

	/* if FD option used, get filename */
	if ( cmicm.lfd ) {
        tmp = string_list_get(datafiles, 0);
        strncpy( kname, tmp, strlen(tmp) );
	}

	temp[ 0 ] = '\0' ;
	prewit( cmmem.sacmem[cmspe.ndxdat], cmspe.nlndat, &cmspe.nprewh, 
	  cmspe.cprewh, kname , temp );

	if( temp[ 0 ] )
	    memcpy( kmspe.kermsg, temp, strlen( temp ) ) ;

	/* Turn off cmicm.lfd, it is not sticky */
	cmicm.lfd = FALSE ;

	if( memcmp(kmspe.kermsg,"        ",8) != 0 ){
	    *nerr = 5005;
	    setmsg( "ERROR", *nerr );
	    aplmsg( kmspe.kermsg,131 );
	    goto L_8888;
	}

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    980710:  Original version.  maf
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  
	 *===================================================================== */

} /* end of function */

