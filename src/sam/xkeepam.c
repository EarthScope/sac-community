
#include "sam.h"
#include "amf.h"
#include "dfm.h"
#include "hdr.h"
#include "bool.h"



#include "ucf.h"
#include "dff.h"

void /*FUNCTION*/ xkeepam(nerr)
int *nerr;
{
	int jdfl, ndx1, ndx2, nfreq, nlen;



	/*=====================================================================
	 * PURPOSE:  To execute the action command KEEPAM.
	 *           This command deletes the phase component of 
	 *           spectral file(s) in memory; if the ifytpe = irlim, the
	 *           spectral file(s) are converted to iftype = iamph first.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:  (to be updated)
	 *    MACH:
	 *    DFM:     NDFL, KDFL
	 *    HDR:     IFTYPE, IRLIM, IAMPH, NPTS, DELTA, B, E,
	 *             DEPMIN, DEPMAX, DEPMEN
	 *=====================================================================
	 * GLOBAL OUTPUT:  (to be updated)
	 *    SAM:     KWSPS1, KWSPS2
	 *=====================================================================
	 * SUBROUTINES CALLED:  (to be updated)
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCLIST, LCKEY, LCDFL,
	 *             VFLIST, VFSPEC,
	 *             GTOUTM, GETFIL, TOAMPH, TORLIM, INDEXB,
	 *             ZDEST, ZNFILE, ZWABS, ZCLOSE, ZPUTC
	 *=====================================================================
	 * LOCAL VARIABLES:  (to be updated)
	 *    KWSPNM:  Name of disk file being written.
	 *    LCONV:   .TRUE. if spectral file needs to be temporarily
	 *             converted from one type to another.
	 *    TEMP:    Scratch space used to store character information
	 *             before writing it to disk.
	 *=====================================================================
	 * KNOWN ERRORS:
	 *
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	/* 1000 if(lcmore(nerr))then
	 * - This command accepts no parameters
	 *      endif */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	/*      if(nerr.ne.0)go to 8888 */

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Check to make sure all files are spectral files. */

	vfspec( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get the next file in DFL, moving header to CMHDR. */

		getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Convert spectral file type if needed. */

		if( *iftype == *irlim ){
			toamph( cmmem.sacmem[ndx1], cmmem.sacmem[ndx2], *npts, cmmem.sacmem[ndx1], 
			 cmmem.sacmem[ndx2] );
			*iftype = *iamph;
			}

		/* -- Release phase component. */

/*		relamb( cmmem.sacmem, ndx2, nerr ); */
/*		if( *nerr != 0 )
			goto L_8888; */
/*		cmdfm.ndxdta[jdfl_][1] = 0; */
		Ncomp[jdfl] = 1;

		/* -- Adjust header from spectral file to ixy file. */

		nfreq = *npts/2 + 1;
		Nlndta[jdfl] = nfreq;
		*npts = nfreq;
		*b = 0.;
		*e = *delta*(float)( nfreq - 1 );
		*iftype = *ixy;

		/* -- Adjust header for component specific values. */
		extrma( cmmem.sacmem[ndx1], 1, nfreq, depmin, depmax, depmen );

		/* -- Give file back to memory manager. */

		putfil( jdfl, nerr );
		if( *nerr != 0 )
			goto L_8888;

		}

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    910510:  Original version (jjy).
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */

} /* end of function */

