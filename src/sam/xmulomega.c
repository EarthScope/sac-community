
#include <math.h>

#include "sam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "ucf.h"
#include "dff.h"

#define PI   M_PI

void /*FUNCTION*/ xmulomega(nerr)
int *nerr;
{
	int j, jdfl, jj, ndx1, ndx2, nfreq, nlen;
	float const_, oldimag, oldreal, slope, value;

        float *Sacmem1, *Sacmem2;

	/*=====================================================================
	 * PURPOSE: To parse and execute the action command MULOMEGA.
	 *          This command multiplies a spectral file by a ramp
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

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get next file from the memory manager.
		 *    (Header is moved into common blocks CMHDR and KMHDR.) */
		getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Need to multiply spectra by "eye omega". */

		/* -- If real-imaginary this means: (REAL, IMAG) = (-IMAG*omega, +REAL*omega) */
		if( *iftype == *irlim ){
			nfreq = *npts/2;
			value = 2.*PI**delta;
			slope = 2.*PI**delta;

                        Sacmem1 = cmmem.sacmem[ndx1];
                        Sacmem2 = cmmem.sacmem[ndx2];
			*Sacmem1 = 0.;
			*Sacmem2 = 0.;
			for( j = 1; j <= (nfreq - 1); j++ ){
				oldreal = *(Sacmem1+j);
				oldimag = *(Sacmem2+j);
				*(Sacmem1+j) = -oldimag*value;
				*(Sacmem2+j) = oldreal*value;
				jj = *npts - j;
				*(Sacmem1+jj) = *(Sacmem1+j);
				*(Sacmem2+jj) = -(*(Sacmem2+j));
				value = value + slope;
				}
			oldreal = *(Sacmem1+nfreq);
			oldimag = *(Sacmem2+nfreq);
			*(Sacmem1+nfreq) = -oldimag*value;
			*(Sacmem2+nfreq) = oldreal*value;

			/* -- If amplitude-phase this means: (AMP, PHASE) = (AMP*omega, PHASE+pi/2) */
			}
		else{
			nfreq = *npts/2;
			value = 2.*PI**delta;
			slope = 2.*PI**delta;

                        Sacmem1 = cmmem.sacmem[ndx1];
                        Sacmem2 = cmmem.sacmem[ndx2];
			*Sacmem1 = 0.;
			const_ = 0.5*PI;
			*Sacmem2 -= const_;
			for( j = 1; j <= (nfreq - 1); j++ ){
				*(Sacmem1+j) *= value;
				*(Sacmem2+j) +=  const_;
				jj = *npts - j;
				*(Sacmem1+jj) = *(Sacmem1+j);
				*(Sacmem2+jj) = -*(Sacmem2+j);
				value = value + slope;
				}
			*(Sacmem1+nfreq) *= value;
			*(Sacmem2+nfreq) += const_;
			}

		/* -- Update any header fields that may have changed. */
		extrma( cmmem.sacmem[ndx1], 1, nlen, depmin, depmax, depmen );

		/* -- Return file to memory manager. */
		putfil( jdfl, nerr );
		if( *nerr != 0 )
			goto L_8888;

		}

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

} /* end of function */

