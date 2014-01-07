
#include <string.h>

#include "smm.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "eam.h"
#include "bool.h"



#include "bot.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xmarkptp(nerr)
int *nerr;
{
	int ifpick, ikpick, ipick, jdfl, jpmax, jpmin, 
	 nlnatw, nofatw, nwin;
	double tmax, tmin;
  float ptpamp;

  sac *s;

	/*=====================================================================
	 * PURPOSE: To parse and execute the action command MARKPTP.
	 *          This command measures the maximum peak to peak amplitude
	 *          and period in the current measurement time window.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *    SMM:     LMTW, KMTW, OMTW
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SMM:     WINLEN
	 *    HDR:     DEPMIN, DEPMAX, DEPMEN
	 *    MEM:     SACMEM
	 *    EAM:     LAPFOP, KPKID, PKSECI, PKAMPL, KPKSRC, KPKRID, DTWF, AWF
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    860228:  Original version (based upon Dave Harris's PTP XSC.)
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861128
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

	    /* -- tok: "LENGTH v":  set length of moving window in seconds. */
	    if( lkreal( "LENGTH$",8, &cmsmm.winlen ) )
	    { /* do nothing */ }

	    /* -- "TO hdr":  set starting header marker field. */
	    else if( lklist( "TO$",4, (char*)kmdfm.kpick[7],9, MPICK - 7,
		     &ipick ) ){
		strcpy( kmsmm.kpmark, kmdfm.kpick[ipick + 6] );
	    }

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

	/* - Test for a non-null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
	    return ;

	/* - Make sure each file is an evenly spaced time series file. */

	vfeven( nerr );
	if( *nerr != 0 )
	    return ;

	/* EXECUTION PHASE: */

	/* - Convert the header marker field to starting locations in the
	 *   floating and character arrays. */

	ipick = nequal( kmsmm.kpmark, (char*)kmdfm.kpick,9, MPICK );
	ifpick = ipick + 10 - 7;
	ikpick = ipick + 6 - 7;

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
	    /* -- Get next file from the memory manager.
	     *    (Header is moved into common blocks CMHDR and KMHDR.) */
    if(!(s = sacget(jdfl-1, TRUE, nerr))) {
      return;
    }
    //getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );

	    /* -- Determine measurement window. */
	    if( cmsmm.lmtw ){
		getatw( (char*)kmsmm.kmtw,9, cmsmm.omtw, &tmin, &tmax, 
		 	&nofatw, &nlnatw, nerr );
		if( *nerr != 0 )
		    return ;
	    }
	    else{
		nofatw = 0;
		nlnatw = s->h->npts;
	    }
	    /* -- Measure maximum peak to peak amplitude and period in window */
	    nwin = (int)( cmsmm.winlen/ s->h->delta );
	    ptp( s->y + nofatw, nlnatw, &nwin, &ptpamp, &jpmin, 
		 &jpmax );
	    tmin = s->h->b + (float)( nofatw + jpmin - 1 )*s->h->delta;
	    tmax = s->h->b + (float)( nofatw + jpmax - 1 )*s->h->delta;

	    /* -- Update any header fields that may have changed. */
	    VALUE(fhdr(s,ifpick)) = tmin;
	    strcpy( kmhdr.khdr[ikpick - 1], "PTPMIN  " );
	    VALUE(fhdr(s,ifpick + 1)) = tmax;
	    strcpy( kmhdr.khdr[ikpick], "PTPMAX  " );
	    s->h->user0 = ptpamp;
	    strcpy( s->h->kuser0, "PTPAMP  " );

	    /* -- Write results to alphanumeric pick file if open.
	     *    TMIN is the time of minimum (valley) of the waveform.
	     *    TMAX is the time of the maximum (peak) of the waveform.
	     *    PKSECI is the earliest of the two pick times, TMIN and TMAX */
	    if( cmeam.lapfop ){
		strcpy( kmeam.kpkid, "PTP     " );
		if( tmin <= tmax ){
		    cmeam.pkseci = tmin;
		    cmeam.pkampl = s->y[nofatw + jpmin - 1];
		    Awf[4] = s->y[nofatw + jpmax - 1];
		    Dtwf[4] = tmax - tmin;
		}
		else{
		    cmeam.pkseci = tmax;
		    cmeam.pkampl = s->y[ nofatw + jpmax - 1];
		    Awf[4] = s->y[nofatw + jpmin - 1];
		    Dtwf[4] = tmin - tmax;
		}
		strcpy( kmeam.kpksrc, "A       " );
		strcpy( kmeam.kpkrid, "PTP     " );
		wapf();
	    }

	}

} /* end of function */

