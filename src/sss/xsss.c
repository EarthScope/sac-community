
#include <math.h>

#include "sss.h"
#include "dfm.h"
#include "hdr.h"
#include "bool.h"


#include "msg.h"
#include "dff.h"
#include "cpf.h"
#include "exm.h"

void /*FUNCTION*/ xsss(nerr)
int *nerr;
{
	char kfile[9];
	int jdfl, notused;



	/*=====================================================================
	 * PURPOSE:  To initialize the Signal Stacking Subprocess.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error return flag.  Set to 0 if no error occurred. [i]
	 *             Possible error values: 1306, 1307, 5109
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    dfm:     ndfl
	 *    hdr:     leven, iftype, itime, ixy, delta, dist, fundef, dist
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:     del, lincl, dlyn, dlyt, dlyni, dlyti, wt, lpol, dst
	 *             beginTime, endTime
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  setmsg, outmsg, getfil, setcomlist, setprompt
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    960701:  Added begin time and end time, maf
	 *    881118:  Added check of headers for files in data file list.
	 *    850812:  Major rewrite of subprocess.
	 *    821207:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850812
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Write a confirming message. */

	setmsg( "OUTPUT", 5101 );
	outmsg();

	/* - Set command list and prompt. */

	setcomlist( 3 );
	setprompt( "SAC/SSS>",9 );

	/* - Check certain header fields for files already in the data file list. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get header from memory manager. */
		getfil( jdfl, FALSE, &notused, &notused, &notused, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Check for evenly spaced time series files. */
		if( !*leven ){
			*nerr = 1306;
			setmsg( "ERROR", *nerr );
			apcmsg( kfile,9 );
			goto L_8888;
			}
		else if( *iftype != *itime && *iftype != *ixy ){
			*nerr = 1307;
			setmsg( "ERROR", *nerr );
			apcmsg( kfile,9 );
			goto L_8888;
			}

		/* -- Make sure the sampling rates match if requested. */
		if( jdfl == 1 ){
			cmsss.del = *delta;
			}
		else if( fabs( *delta - cmsss.del ) > cmsss.srcfac && cmsss.lsrc ){
			*nerr = 5109;
			setmsg( "ERROR", *nerr );
			goto L_8888;
			}

		/* -- Start with global property values. */
		Lincl[jdfl] = TRUE;
		Dlyn[jdfl] = cmsss.dlyng;
		Dlyt[jdfl] = cmsss.dlytg;
		Dlyni[jdfl] = cmsss.dlynig;
		Dlyti[jdfl] = cmsss.dlytig;
		Wt[jdfl] = cmsss.wtg;
		Lpol[jdfl] = cmsss.lpolg;

		/* -- Use global distance or distance from header. */
		if( cmsss.dstg != cmhdr.fundef ){
			Dst[jdfl] = cmsss.dstg;
			}
		else if( *dist != cmhdr.fundef ){
			Dst[jdfl] = *dist;
			}
		else{
			Dst[jdfl] = cmhdr.fundef;
			}

		/* -- Set begin and end time from header. added 960701 maf */
                Tbegin[jdfl] = *begin ;	/* if begin is undefined, Tbegin is too */
		Tend[jdfl] = *ennd ;

		} /* end for */

L_8888:
	return;

} /* end of function */

