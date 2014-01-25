/** 
 * @file  decim.c
 *
 * @brief Efficient Time Domain Filtering Code
 */

#include "dbh.h"

/** 
 * Efficient Time-Domain / Filtering Code
 * 
 * @param x 
 *    Input array to be filtered / decimated
 * @param nx 
 *    Length of array \p x
 * @param buf 
 *    Work Space Array
 * @param nb 
 *    Length of array \p buf, Must be >= NC
 * @param c 
 *    Array of Filter Coefficients. Stored in Symmetric Mode
 * @param nchp1 
 *    Length of array \p c
 * @param isym 
 *    Symmetry Type
 *       - 1 Evenly Symmetric
 *       - 0 Oddly Symmetric
 * @param irate 
 *    Downsampling Rate
 * @param y 
 *    Output array containing decimated / Filtered array.
 *    May be the same array as \p x for in-place compuatations
 * @param ny 
 *    Length of array \p y
 * 
 * @return Nothing
 *    
 * @author  Dave Harris
 *
 * @date  810724  Last Modified
 * @date  071022  Documented/Reviewed
 *
 */
void 
decim(float      *x, 
      int         nx, 
      float      *buf, 
      int         nb, 
      float      *c, 
      int         nchp1, 
      int         isym, 
      int         irate, 
      float      *y, 
      int        *ny)
{
	int i, in, ioff, nbstop, nc, nch, nex, nsave, nxstop, out, ptr;
	float temp;

	float *const Buf = &buf[0] - 1;
	float *const C = &c[0] - 1;
	float *const X = &x[0] - 1;
	float *const Y = &y[0] - 1;


	/*  Initializations */
	nch = nchp1 - 1;
	nc = 2*nch + 1;
	*ny = (nx - 1)/irate + 1;
	nex = nb - nc;
	nex = (nex/irate)*irate;
	nxstop = nx - nch - irate;
	nbstop = nchp1 + nex;
	ptr = nchp1 - irate;
	in = 1 - irate;
	out = 0;

	/*  Initialize buffer, with zeroes where no data available. */
	for( i = 1; i <= nch; i++ )
	    Buf[i] = 0.;

	for( i = nchp1; i <= (nc - irate); i++ )
	    Buf[i] = X[i - nch];

	/*  Main loop - filter until end of data reached. */
	while ( in <= nxstop ) {

	    /*    Shift buffer, if end of buffer reached. */
	    if( ptr == nbstop ){
		ptr = nchp1 - irate;
		nsave = nc - irate;
		ioff = nex + irate;
		for( i = 1; i <= nsave; i++ )
		    Buf[i] = Buf[i + ioff];
	    }

	    /*    Update pointers */
	    out = out + 1;
	    in = in + irate;
	    ptr = ptr + irate;

	    /*    Read in new data */
	    for( i = 1; i <= irate; i++ ){
		ioff = nch + i - irate;
		Buf[ptr + ioff] = X[in + ioff];
	    }

	    /*    Compute output point */
	    temp = C[1]*Buf[ptr];
	    for( i = 1; i <= nch; i++ )
		temp = temp + C[i + 1]*(Buf[ptr + i] + isym*Buf[ptr - i]);

	    Y[out] = temp;
	} /* end while */

	/*  Loop to handle case where operator runs off of the data */
	while ( in <= nx - irate ) {

	    /*    Shift buffer, if end of buffer reached. */
	    if( ptr == nbstop ){
		ptr = nchp1 - irate;
		nsave = nc - irate;
		ioff = nex + irate;
		for( i = 1; i <= nsave; i++ )
		    Buf[i] = Buf[i + ioff];
	    }

	    /*    Update pointers */
	    out = out + 1;
	    in = in + irate;
	    ptr = ptr + irate;

	    /*    Read in new data */
	    for( i = 1; i <= irate; i++ ){
		ioff = nch + i - irate;
		if( in + ioff > nx )
		    Buf[ptr + ioff] = 0.;

		else
		    Buf[ptr + ioff] = X[in + ioff];

	    }

	    /*    Compute output point */
	    temp = C[1]*Buf[ptr];
	    for( i = 1; i <= nch; i++ )
		temp = temp + C[i + 1]*(Buf[ptr + i] + isym*Buf[ptr - i]);

	    Y[out] = temp;
	} /* end while */


	/*  Done */
	return;
}

