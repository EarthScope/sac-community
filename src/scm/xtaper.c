
#include <math.h>

#include "scm.h"
#include "sam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "co.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"

#define PI  M_PI
void /*FUNCTION*/ xtaper(nerr)
int *nerr;
{
	int ipts, j, jdfl, ndx1, ndx2, nlen;
	float f0, f1, omega, tpts, value;



	/*=====================================================================
	 * PURPOSE:  To execute the action command TAPER.
	 *           This command tapers each end of all data files.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    SAM:     KTAPTP, MTAPTP
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SAM:     ITAPTP
	 *    HDR:     DEPMIN, DEPMAX, DEPMEN
	 *    MEM:     NDFL, SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, LCKEY, LKLIST, LKRRC, CFMT, CRESP,
	 *             GETFIL, EXTRMA, PUTFIL
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    TPTS:    Length of taper (at each end) in seconds. [f]
	 *    IPTS:    Length of taper in data points. [i]
	 *    OMEGA:
	 *    NDX1:    Index to first data point in file. [i]
	 *    NDXL:    Index to last data point in file. [i]
	 *    VALUE:   Value of taper at each taper point. [f]
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- Set type of taper. */
		if( lklist( "TYPE$",6, (char*)kmsam.ktaptp,9, cmsam.ntaptp, 
		 &cmsam.itaptp ) ){

			/* -- Set taper width as fraction of total signal width. */
			}
		else if( lkrrc( "WIDTH$",7, 0., 1., &cmsam.widtap ) ){

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
		goto L_8888;

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Check for evenly spaced time series files. */

	vfeven( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - Perform taper on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get next file from memory manager. */
		getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Determine number of points for taper. */
    taper_width_to_points(cmsam.widtap, nlen, &ipts);

    /* -- Taper */
    taper(cmmem.sacmem[ndx1], nlen, cmsam.itaptp, ipts);

		/* -- Compute new extrema. */
		extrma( cmmem.sacmem[ndx1], 1, *npts, depmin, depmax, depmen );

		/* -- Give current file back to memory manager. */
		putfil( jdfl, nerr );
		if( *nerr != 0 )
			goto L_8888;

		}

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    850118:  Fixed bug in computing cosine taper.
	 *             Added HANNING and HAMMING tapers.
	 *             Deleted automatic removal of mean.
	 *    830118:  Now automatically removes mean before tapering.
	 *    820621:  Changed to newest set of parsing functions.
	 *    810414:  Fixed indexing bug involving right side taper.
	 *    810422:  Fixed bug when length of taper was only 1 point.
	 *    810414:  Minor changes relating to new CMSAM.
	 *    810120:  Changed to output message retrieval from disk.
	 *    800203:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850118
	 *===================================================================== */

} /* end of function */

