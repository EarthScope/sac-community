/** 
 * @file   xdecimate.c
 * 
 * @brief  Decimate a file
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "mach.h"
#include "scm.h"
#include "hdr.h"
#include "amf.h"
#include "dfm.h"
#include "fir.h"
#include "bool.h"


#include "bot.h"
#include "ucf.h"
#include "sam.h"
#include "msg.h"
#include "dbh.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

void 
xdecimate(int *nerr) {

	char kdecnm[MCPFN+1];
	int idx, jdx, jdfl,
    nlndec, nlnnew, nlnscr;

	static char kint[7-(2)+1]={'2','3','4','5','6','7'};
	static int ndecmn = 2;
	static int ndecmx = 7;
        char *cattemp;

  sac *s;
  float *new, *scr;
	/*=====================================================================
	 * PURPOSE:  To execute the action command DECIMATE.
	 *           This command decimates data in memory.
	 *           An anti-aliasing IIR lowpass filter is also applied.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL:  SCM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL, MEMNOW
	 *    HDR:     IFTYPE, ITIME, IXY, LEVEN, NPTS, DELTA
	 *    SCM:     NDECFC, LDECFI
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     NPTS, DELTA, E
	 *    DFM:     NLNDTA
	 *    FIR:     KIDFIR, CFIR, NCFIR, DTFIR
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    NDECMN:  Minimum decimation factor = 2
	 *    NDECMX:  Maximum decimation factor = 7
	 *    JDFL:    Counter to files in DFL; range is 1 to NDFL.
	 *    NDXSCR:  Index in SACMEM array of scratch space used in decimation.
	 *    NLNSCR:  Length of scratch space.
	 *    NLNNEW:  Computed new length of data after decimation.
	 *    NLNDEC:  New length after decimation returned by DECIM.
	 *    NDXNEW:  New index of data after decimation.
	 *=====================================================================
	 * ASSUMPTIONS:
	 * - Lowpass FIR filters for decimation are in AUX subdirectory FIR.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920227:  Added DATA-SET update logic.
	 *    870923:  Deleted ".saf" from aux file names.
	 *    870318:  Added option to turn anti-aliasing filter off (desamp).
	 *    850801:  Changed to new memory manager.
	 *    820414:  Fixed bug in out of range error reporting.
	 *    820331:  Combined "parse" and "control" modules.
	 *    810205:  Replaced call to ZFILNM with more general ZCHSTR.
	 *    801204:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870318
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

        for( idx = 0 ; idx < MCPFN ; idx++ )
            kdecnm[ idx ] = ' ' ;
        kdecnm[ MCPFN ] = '\0' ;


	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){
	    /* -- "n":  change decimation factor. */
	    if( lcirc( ndecmn, ndecmx, &cmscm.ndecfc ) )
	    { /* do nothing */ }

	    /* -- "FILTER ON|OFF":  set up anti-aliasing filter option. */
	    else if( lklog( "FILTER$",8, &cmscm.ldecfi ) )
	    { /* do nothing */ }

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
	    return ;

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
	    return ;

	/* - Check to make sure all files are evenly spaced time series files. */

	vfeven( nerr );
	if( *nerr != 0 )
	    return ;

	/* EXECUTION PHASE: */

	/* - Read in the proper FIR lowpass filter used in decimation and
	 *   set up scratch space for decimation if necessary. */


	if( cmscm.ldecfi ){
	    zbasename( kdecnm,MCPFN+1 );
	    crname( kdecnm,MCPFN+1, KSUBDL, "fir",4, nerr );
	    if( *nerr != 0 )
		return ;
	    cattemp = malloc(3+1+1);
	    strcpy(cattemp,"dec");
	    cattemp[3] = kint[cmscm.ndecfc-(2)];
	    cattemp[4] = '\0';
	    crname( kdecnm,MCPFN+1, KDIRDL, cattemp, 3+1+1, nerr );
	    free(cattemp);
	    if( *nerr != 0 )
		return ;
	    rfir( kdecnm,MCPFN+1, MFIR, cmfir.cfir, &cmfir.ncfir, &cmfir.dtfir, 
	     kmfir.kidfir,81, nerr );
	    if( *nerr != 0 )
		return ;

	    nlnscr = 2*cmfir.ncfir + 100;
      scr = (float *) malloc(sizeof(float) * nlnscr);
	    //allamb( &cmmem, nlnscr, &ndxscr, nerr );
	    if( *nerr != 0 )
		return ;
	}

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){

	    /* -- Get the next file in DFL, moving header to CMHDR. */
      if(!(s = sacget(jdfl-1, TRUE, nerr))) {
        return;
      }
	    //getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

	    /* -- Get new data block.  */
	    nlnnew = (s->h->npts - 1)/cmscm.ndecfc + 1;
      new = (float *) malloc(sizeof(float) * nlnnew);
	    //allamb( &cmmem, nlnnew, &ndxnew, nerr );
	    if( *nerr != 0 )
		return ;

	    /* -- Perform decimation (with filter) or desampling (without filter.) */
	    if( cmscm.ldecfi ){
        decim( s->y, s->h->npts, scr, nlnscr, cmfir.cfir, 
               cmfir.ncfir, 1, cmscm.ndecfc, new, &nlndec );
		if( nlndec != nlnnew ){
		    *nerr = 901;
		    setmsg( "ERROR", *nerr );
		    apcmsg( "in XDECIMATE. Two new lengths are:",35 );
		    apimsg( nlnnew );
		    apimsg( nlndec );
		    return ;
		}
	    }
	    else{
        for( jdx = 1; jdx <= nlnnew; jdx++ ){
          new[jdx] = s->y[cmscm.ndecfc * (jdx-1)];
        }
	    }

	    /* -- Release old data block and store new index and data length. */
      FREE(s->y);
      s->y = new;

	    /* -- Update any header fields that may have changed. */

	    s->h->npts  = nlnnew;
	    s->h->delta = s->h->delta*(float)( cmscm.ndecfc );
	    s->h->e     = s->h->b + s->h->delta*(float)( s->h->npts - 1 );
	    extrma( s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax, &s->h->depmen );

	}

	/* - Release the scratch space. */
  
	if( cmscm.ldecfi )
    FREE(scr);

	/* - Calculate and set new range of dependent variable. */

	setrng();

} /* end of function */

