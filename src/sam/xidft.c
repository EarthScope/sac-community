
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
	int jdx, jdfl;

        double *re, *im;
        sac *s;
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

	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
    if(!(s = sacget(jdfl-1, TRUE, nerr))) {
      goto L_8888;
    }
    //getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

	    if( s->h->iftype == IAMPH ){
        torlim( s->y, s->x, s->h->npts, s->y, s->x);
		s->h->iftype = IRLIM;
	    }

      re = (double *) malloc(sizeof(double) * s->h->npts);
      im = (double *) malloc(sizeof(double) * s->h->npts);
      if(!re || !im) {
        printf("error allocating memory-xidft\n");
        goto L_8888;
	    }

	    for (jdx=0; jdx<s->h->npts; jdx++){
        re[jdx] = (double) s->y[jdx];
        im[jdx] = (double) s->x[jdx];
        DEBUG("%d %e %e %e %e\n",jdx, re[jdx],im[jdx],s->y[jdx],s->x[jdx]);
	    }
	    dcpft(re, im, s->h->npts, 1, cmsam.ibwd);

	    for (jdx=0; jdx<s->h->npts; jdx++){
        s->y[jdx] = (float)re[jdx];
        s->x[jdx] = (float)im[jdx];
	    }
      //DEBUG("AFTER\n");
      //DEBUG("%d %e %e %e %e\n",0, re[0],im[0],s->y[0],s->x[0]);
      //DEBUG("%d %e %e %e %e\n",1, re[1],im[1],s->y[1],s->x[1]);
      
      FREE(re);
      FREE(im);
      
	    s->h->delta = s->h->sdelta;
	    s->h->scale = 1./((float)( s->h->npts )*s->h->delta);
	    for( jdx = 0; jdx <= (s->h->npts - 1); jdx++ ){
        s->y[jdx] *= s->h->scale;
      }
	    s->h->iftype = ITIME;
	    s->h->b      = s->h->sb;
	    s->h->npts   = s->h->nsnpts;
	    s->h->e      = s->h->b + (float)( s->h->npts - 1 )*s->h->delta;
	    extrma( s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax, &s->h->depmen );
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

