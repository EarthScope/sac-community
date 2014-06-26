/** 
 * @file   xenvelope.c
 * 
 * @brief  Envelope Function
 * 
 */

#include "sam.h"
#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "bool.h"


#include "co.h"
#include "msg.h"
#include "ucf.h"
#include "dff.h"

#define	MINDATALEN	201
#define	MLENSCRATCH	4297

/** 
 * 
 *  To parse and execute the action command ENVELOPE.
 *     This command computes the envelope of a function.
 * 
 *  @param nerr
 *     Error return flag
 *     - 0 on Success
 *     - Non-Zero on Error
 *
 * @note Local Variables
 *    - MLENSCRATCH:  Size of scratch space needed for transform. [ip]
 *    - ndxscratch:   Index in SACMEM array for scratch space. [i]
 *    - ndxhilbert:   Index in SACMEM array for Hilbert transform. [i]
 *    - ndxy:         Index in SACMEM array for current signal. [i]
 *
 * @date  890223:  Original version.
 * @date  890223:  Documented/Reviewed
 *
 * 
 */

void 
xenvelope(int *nerr) {
	int j, jdfl, 
    nlenmn, nlenmx;
  sac *s;
  float *hilbert;
	*nerr = 0;

	/* - Test for a non-null data file list. */
	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Make sure each file is an evenly spaced time series file. */
	vfeven( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Determine minimum and maximum signal size.
	 *   Make sure minimum is not too small for fir filter subroutine.
	 *   Use maximum to allocate space for the Hilbert transform. */

	nlenmn = MLARGE;
	nlenmx = 0;
	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
    if(!(s = sacget(jdfl-1, TRUE, nerr))) {
*nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
goto L_8888;
}
		//getfil( jdfl, FALSE, &ntused, &ntused, &ntused, nerr );
		if( *nerr != 0 )
			goto L_8888;
		nlenmn = min( nlenmn, s->h->npts );
		nlenmx = max( nlenmx, s->h->npts );
		}

	if( nlenmn < MINDATALEN ){
		*nerr = 1613;
		setmsg( "ERROR", *nerr );
		apimsg( MINDATALEN );
		//		outmsg();
		goto L_8888;
		}

	/* - EXECUTION PHASE: */

	/* - Allocate temporary blocks for output Hilbert transform. */

  hilbert = (float *) malloc(sizeof(float) * nlenmx);

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){

    if(!(s = sacget(jdfl-1, TRUE, nerr))) {
      goto L_8888;
    }
		//getfil( jdfl, TRUE, &nlnsignal, &ndxsignal, &notused, nerr );

		envelope(s->h->npts,
             s->y,
             hilbert);

		/* Copy output signal back onto input */
		for(j = 0; j < s->h->npts; j++) {
      s->y[j] = hilbert[j];
		}

		/* -- Update any header fields that may have changed. */
		extrma( s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax, &s->h->depmen );

		}

	/* - Release scratch space. */
  FREE(hilbert);
	/* - Calculate and set new range of dependent variable. */
	setrng();

L_8888:
	return;

}

