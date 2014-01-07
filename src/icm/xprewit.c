
#include <string.h>

#include "icm.h"
#include "bool.h"
#include "dfm.h"
#include "hdr.h"
#include "spe.h"
#include "amf.h"

#include "co.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xprewit(nerr)
int *nerr;
{
	int iprew , jdfl;
	float coefficients[MPREWH+1];
	char errmsg[131] ;
    char *tmp;
	char kname[ MCPFN + 10 ] ;
  sac *s;
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
	 *=====================================================================
	 * GLOBAL OUTPUT:
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
	    if ( lcint ( &iprew ) ) {
		cmicm.iprew = min ( iprew , MPREWH ) ;
	    }

	    /* FILTERDESIGN: pass whiten coefficients into filterdesign */
	    else if( lckey( "FILTERDESIGN #$" , 16 ) || lckey( "FD #$" , 6 ) )
		cmicm.lfd = TRUE ;

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	} /* end if ( lcmore ( nerr ) */

	if( *nerr != 0 )
	    goto L_8888;

	/* CHECKING PHASE: */

        /* - Test for a non-null data file list. */

        vflist( nerr );

        /* - Make sure each file is an evenly spaced time series file. */

        vfeven( nerr );
        if( *nerr != 0 )
            goto L_8888;


	/* EXECUTION PHASE: */
	errmsg[ 0 ] = '\0' ;

	for ( jdfl = 1 ; jdfl <= saclen() ; jdfl++ ) {
    if(!(s = sacget(jdfl-1, TRUE, nerr))) {
      goto L_8888;
    }
    //getfil ( jdfl , TRUE , &nlen , &ndxy , &ndxx , nerr ) ;

	    /* if FD option used, get filename */
	    if ( cmicm.lfd ) {
        tmp = s->m->filename;
            strncpy( kname , tmp, strlen(tmp));
            kname[strlen(tmp)+1] = '\0' ;
	    }

	    prewit( s->y, s->h->npts, s->h->delta, &cmicm.iprew, coefficients,
		    kname, errmsg );
	    if( errmsg[ 0 ] ){
		*nerr = 5005;
		setmsg( "ERROR", *nerr );
		aplmsg( errmsg,131 );
		goto L_8888;
	    }
	}

L_8888:
	/* Turn off cmicm.lfd, it is not sticky */
	cmicm.lfd = FALSE ;

	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    980710:  Original version.  maf plagerized from xwhiten.c
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  
	 *===================================================================== */

} /* end of function */

