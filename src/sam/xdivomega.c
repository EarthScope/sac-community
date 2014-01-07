
#include <math.h>

#include "sam.h"
#include "amf.h"
#include "dfm.h"
#include "hdr.h"
#include "bool.h"

#include "ucf.h"
#include "dff.h"

#define PI  M_PI

void /*FUNCTION*/ xdivomega(nerr)
int *nerr;
{
	int j, jdfl, jj, nfreq;
	float const_, oldimag, oldreal, value;
  float slope;
  sac *s;


	/*=====================================================================
	 * PURPOSE: To parse and execute the action command DIVOMEGA.
	 *          This command divides a spectral file by a ramp
	 *          function equal to omega.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     IFTYPE, DEPMIN, DEPMAX, DEPMEN
	 *    MEM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870918:  Fixed some major logic flaws in execution phase.
	 *    870811:  Now works directly in the frequency domain rather than
	 *             by faking it by first writing file using writesp.
	 *    830000:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED: 
	 *===================================================================== */

	/* PROCEDURE: */
	*nerr = 0;

	/* - CHECKING PHASE: */

	/* - Test for a non-null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Make sure each file is a spectral file. */

	vfspec( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - EXECUTION PHASE: */

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
    if(!(s = sacget(jdfl-1, TRUE, nerr))) {
      goto L_8888;
    }
		//getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

		/* -- Need to divide spectra by "eye omega". */
		/* -- If real-imaginary this means: (REAL, IMAG) = (+IMAG/omega, -REAL/omega) */
		if( s->h->iftype == IRLIM ){

			nfreq = s->h->npts/2;
			value = 2.*PI*s->h->delta;
			s->y[0] = 0.0;
      s->x[0] = 0.0;
			for( j = 1; j <= (nfreq - 1); j++ ){
				oldreal = s->y[j];
				oldimag = s->x[j];
        s->y[j] = oldimag / value;
        s->x[j] = -oldreal / value;
				jj = s->h->npts - j;
        s->y[jj] =  s->y[j];
        s->x[jj] = -s->x[j];
				value = (2.*PI*s->h->delta) * (j+1);
      }
			oldreal = s->y[nfreq];
			oldimag = s->x[nfreq];
      s->y[nfreq] =  oldimag / value;
      s->x[nfreq] = -oldreal/ value;

			/* -- If amplitude-phase this means: (AMP, PHASE) = (AMP/omega, PHASE-pi/2) */
			}
		else{
			nfreq = s->h->npts/2;
			value = 2.*PI*s->h->delta;
      slope = 2.*PI*s->h->delta;
      s->y[0] = 0;
			const_ = 0.5*PI;
      s->x[0] += const_;

			for( j = 1; j <= (nfreq - 1); j++ ){
        s->y[j] /= value;
        s->x[j] -= const_;
				jj = s->h->npts - j;
        s->y[jj] =  s->y[j];
        s->x[jj] = -s->x[j];
				value += slope;//(2.*PI*s->h->delta);// * (j+1);
      }
      s->y[nfreq] /= value;
      s->x[nfreq] -= const_;
			}

		/* -- Update any header fields that may have changed. */
		extrma( s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax, &s->h->depmen );


		}

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

} /* end of function */

