
#include <math.h>

#include "scm.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "ucf.h"
#include "sam.h"
#include "cpf.h"
#include "dff.h"

#define PI  M_PI

void /*FUNCTION*/ xrq(nerr)
int *nerr;
{
	int j, jdfl, jj, ndx1, ndx2, nfreq, num;
	float dfreq, fac, freq, recqf;

        float *Sacmem;

	/*=====================================================================
	 * PURPOSE:  To execute the action command RQ.
	 *           This command removes the seismic Q factor from spectral files.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  SCM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    SCM:     RQQCON, RQRCON, RQCCON
	 *    HDR:     IFTYPE, IRLIM
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SCM:     RQQCON, RQRCON, RQCCON
	 *    HDR:     IFTYPE, DEPMIN, DEPMAX, DEPMEN
	 *    MEM:     SACMEM()
	 *    DFM:     NDFL
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, LCREAL, CFMT, CRESP
	 *             VFLIST, VFSPEC, GETFIL, TOAMPH, EXTRMA, PUTFIL
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    FREQ:    Frequency at a given data point.
	 *    DFREQ:   Delta frequency.
	 *    NFREQ:   Number of unique freqencies.
	 *    FAC:     Constant part of Seismic Q correction term.
	 *    RECQF:   Reciprocal of Seismic Q correction factor.
	 *=====================================================================
	 * LIMITATIONS:
	 * - Q and C are not functions of frequency.
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "Q v":  change seismic Q value. */
		if( lkreal( "Q$",3, &cmscm.rqqcon ) ){

			/* -- "R v":  change distance parameter. */
			}
		else if( lkreal( "R$",3, &cmscm.rqrcon ) ){

			/* -- "C v":  change velocity parameter. */
			}
		else if( lkreal( "C$",3, &cmscm.rqccon ) ){

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

	/* - Check to make sure all files are spectral files. */

	vfspec( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - For each file in DFL: */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get the next file in DFL from the memory manager. */
		getfil( jdfl, TRUE, &num, &ndx1, &ndx2, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Convert the file to amplitude-phase format if necessary. */
		if( *iftype == *irlim ){
			toamph( cmmem.sacmem[ndx1], cmmem.sacmem[ndx2], *npts, cmmem.sacmem[ndx1], 
			 cmmem.sacmem[ndx2] );
			*iftype = *iamph;
			}

		/* -- Apply seismic Q correction to each amplitude data point. */
		freq = *b;
		dfreq = *delta;
		nfreq = *npts/2;
		fac = PI*cmscm.rqrcon/(cmscm.rqqcon*cmscm.rqccon);
                Sacmem = cmmem.sacmem[ndx1];
		for( j = 1; j <= (nfreq - 1); j++ ){
			freq = freq + dfreq;
			recqf = exp( fac*freq );
			*(Sacmem+j) *= recqf;
			jj = *npts - j;
			*(Sacmem+jj) = *(Sacmem+j);
			}

		/* -- Recompute extrema. */
		extrma( cmmem.sacmem[ndx1], 1, *npts, depmin, depmax, depmen );
		*depmen = 0.;

		/* -- Give file back to memory manager. */
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
	 *    820621:  Changed to newest set of parsing and checking functions.
	 *    810414:  Minor changes relating to new CMSCM.
	 *    810120:  Changed to output message retrieval from disk.
	 *    800320:  Original version.
	 *=====================================================================
	 * DOCUMENTED:  820624
	 *===================================================================== */

} /* end of function */

