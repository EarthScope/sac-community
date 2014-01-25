
#include <stdio.h>

#include "sam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "ucf.h"
#include "icm.h"
#include "dff.h"

void /*FUNCTION*/ xidft(nerr)
int *nerr;
{
	int jdx, jdfl, ndx1, ndx2, nlen, ndxscr1, ndxscr2;

        float *Sacmem, *Sacmem2;

        double *re, *im;

	/*=====================================================================
	 * PURPOSE:  To execute the action command IDFT.
	 *           This command takes the inverse discrete fourier transform
	 *           of data in memory.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *=====================================================================
	 * GLOBAL COUPLING:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * ASSUMPTIONS:
	 *=====================================================================
	 * LIMITATIONS:
	 *=====================================================================
	 * KNOWN ERRORS:
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

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

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	    getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	    if( *iftype == *iamph ){
		torlim( cmmem.sacmem[ndx1], cmmem.sacmem[ndx2], *npts,
			cmmem.sacmem[ndx1], cmmem.sacmem[ndx2] );
		*iftype = *irlim;
	    }

	    allamb(&cmmem, 2*nlen, &ndxscr1, nerr);
	    if( *nerr != 0){
		printf("error allocating memory-xidft\n");
		goto L_8888;
	    }                

	    allamb(&cmmem, 2*nlen, &ndxscr2, nerr);
	    if( *nerr != 0){
		printf("error allocating memory-xidft\n");
		goto L_8888;
	    }                

	    re = (double *)cmmem.sacmem[ndxscr1];
	    im = (double *)cmmem.sacmem[ndxscr2];

	    Sacmem = cmmem.sacmem[ndx1];
	    Sacmem2 = cmmem.sacmem[ndx2];

	    for (jdx=0; jdx<*npts; jdx++){
		*re++ = (double)*Sacmem++;
		*im++ = (double)*Sacmem2++;
	    }

	    re = (double *)cmmem.sacmem[ndxscr1];
	    im = (double *)cmmem.sacmem[ndxscr2];

/*	    cpft(cmmem.sacmem[ndx1], cmmem.sacmem[ndx2], *npts, 1, cmsam.ibwd);
*/
	    dcpft(re, im, *npts, 1, cmsam.ibwd);

	    re = (double *)cmmem.sacmem[ndxscr1];
	    im = (double *)cmmem.sacmem[ndxscr2];

	    Sacmem = cmmem.sacmem[ndx1];
	    Sacmem2 = cmmem.sacmem[ndx2];

	    for (jdx=0; jdx<*npts; jdx++){
		*Sacmem++ = (float)*re++;
		*Sacmem2++ = (float)*im++;
	    }

	    relamb(cmmem.sacmem, ndxscr1, nerr);
	    if(*nerr != 0){
		printf("error releasing memory-xidft\n");
		goto L_8888;
	    }

	    relamb(cmmem.sacmem, ndxscr2, nerr);
	    if(*nerr != 0){
		printf("error releasing memory-xidft\n");
		goto L_8888;
	    }

	    *delta = *sdelta;
	    *scale = 1./((float)( *npts )**delta);
	    Sacmem = cmmem.sacmem[ndx1];
	    for( jdx = 0; jdx <= (*npts - 1); jdx++ ){
		*(Sacmem++) *= *scale;
	    }
	    *iftype = *itime;
	    *begin = *sb;
	    *npts = *nsnpts;
	    Nlndta[jdfl] = *npts;
	    *ennd = *begin + (float)( *npts - 1 )**delta;
	    extrma( cmmem.sacmem[ndx1], 1, *npts, depmin, depmax, depmen );
	    putfil( jdfl, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	} /* end for( jdfl ) */

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;
	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    910501:  Rid bug stmt, which scaled sacmem(ndx1) an extra time.
	 *    821122:  Changed maximum IDFT to 65536.
	 *             Deleted double precision IDFT option.
	 *    820621:  Changed to newest set of parsing and checking functions.
	 *    810414:  Minor changes relating to new CMSAM.
	 *    810120:  Changed to output message retrieval from disk.
	 *    800630:  Original version.
	 *===================================================================== */

} /* end of function */

