
#include <math.h>

#include "sam.h"
#include "gam.h"
#include "amf.h"
#include "bool.h"
#include "co.h"


#include "gdm.h"
#include "pl.h"
#include "gtm.h"

int /*FUNCTION*/ fdplot ( memptr , lprint , xbeg , nerr )
int memptr[] , *xbeg , *nerr ;
int lprint ;
{
	int lany;
	int npulpts = 0, xend;
	float dx, dy, xdattmp[NDATPTS], ximptmp[NIMPPTS], xos, 
	 xvsmax, xvsmin, xwmax, xwmin, yos, yvsmax, yvsmin, ywmax, ywmax1, 
	 ywmin, ywmin1;

	int *const Memptr = &memptr[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To plot the traces produced by the filterdesign command. 
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    MEMPTR:  Array of pointers (offsets) to data in sacmem. [r]
	 *    NPTRS:   Number of elements in memptr. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  GAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MEM:     SACMEM
	 *    SAM:     NDATPTS, NIMPPTS, FDDELTA
	 *=====================================================================
	 * GLOBAL VARIABLES:
	 *    GAM:     KGDDEF:  Name of the graphics device. [c]
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB: General graphics subroutines.
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    xwmin: X axes world coordinate minimum value. [r]
	 *    xwmax: X axes world coordinate maximum value. [r]
	 *    ywmin: Y axes world coordinate minimum value. [r]
	 *    ywmax: Y axes world coordinate minimum value. [r]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    910213:  Commented out the call to setwindowsize. (wct)
	 *    901016:  Original version
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Save current graphics environment */
	plsave();

	/*     call setwindowsize(1, 0.4, .95, 0.4, .95) */

	/* - Get a default graphics device if one is defined */
	getstatus( "ANY", &lany );
	if( !lany ){
		begindevice( kmgam.kgddef,9, nerr );
		if( *nerr != 0 )
			goto L_8888;
	}

	beginframe( lprint , nerr );
	if( *nerr != 0 )
		goto L_8888;

	getvspace( &xvsmin, &xvsmax, &yvsmin, &yvsmax );

	/* - The view space dimensions */
	dx = xvsmax - xvsmin;
	dy = yvsmax - yvsmin;

	/* - Offsets for the margins in the viewspace. */
	xos = .05;
	yos = .05;

	/*      call settextjust('CENTER', 'CENTER') */

	/* - lower left, Group Delay Plot
	 * -- Set world min & max values for this data set. */
	getlims( cmmem.sacmem[Memptr[5]], NDATPTS, &ywmin, &ywmax );
	getlims( cmmem.sacmem[Memptr[6]], NDATPTS, &xwmin, &xwmax );
	getlims( cmmem.sacmem[Memptr[9]], NDATPTS, &ywmin1, &ywmax1 );

	if( ywmin1 < ywmin )
		ywmin = ywmin1;
	if( ywmax1 > ywmax )
		ywmax = ywmax1;

	setworld( xwmin, xwmax, ywmin, ywmax );
	setvport( xvsmin + 2.*xos, xvsmin + (xvsmax - xvsmin)/2.0 - xos, 
	 yvsmin + yos, yvsmin + (yvsmax - yvsmin)/2.0 - yos );

	setlinestyle( 4 );
	setcolorname( "magenta",8 );
	worldpolyline( cmmem.sacmem[Memptr[6]], cmmem.sacmem[Memptr[5]], NDATPTS );

	setlinestyle( 1 );
	setcolorname( "blue",5 );
	loadxtmp( xdattmp, xwmin, xwmax, NDATPTS );
	worldpolyline( xdattmp, cmmem.sacmem[Memptr[9]], NDATPTS );

	settextfont( 1 );
	setcolorname( "black",6 );
	settextsize( (.667*.02), .02 );

	xaxis( "LINEAR", "BELOW", "BELOW", "Frequency (Hz)",15 );
	yaxis( "LINEAR", "LEFT", "LEFT", "Group Delay (sec)",18 );

	settextfont( 3 );
	settextsize( (.667*.03), .03 );

	move( 0.25*dx - xos, 0.5*dy - yos );
	text( "Group Delay",12, 11 );

	/* - lower right, Impulse Response
	 * -- Set world min & max values by compiting the statistical duration. */

	/* -- Find the begin and end array locations for this computed signal duration. */
	npulpts = NIMPPTS;
	sduration( cmmem.sacmem[Memptr[10]], &npulpts, xbeg, &xend );
	/* -- Load the temp x array with the first npulpts values to be displayed,
	 *    using fddelta so that the plot time in seconds matches the duration. */
	loadxtmp( ximptmp, 0.0, npulpts*cmsam.fddelta, npulpts );
	xwmin = 0.0;
	xwmax = npulpts*cmsam.fddelta;

	getlims( cmmem.sacmem[Memptr[10]], NIMPPTS, &ywmin, &ywmax );

	setworld( xwmin, xwmax, ywmin, ywmax );
	setvport( xvsmin + (xvsmax - xvsmin)/2.0 + xos, xvsmax - xos, 
	 yvsmin + yos, yvsmin + (yvsmax - yvsmin)/2.0 - yos );

	setcolorname( "blue",5 );

	worldpolyline( ximptmp, cmmem.sacmem[Memptr[10]]+(*xbeg)-1, npulpts );

	settextfont( 1 );
	setcolorname( "black",6 );
	settextsize( (.667*.02), .02 );

	xaxis( "LINEAR", "BELOW", "BELOW", "Time (seconds)",15 );

	settextfont( 3 );
	settextsize( (.667*.03), .03 );

	move( 0.75*dx - 1.5*xos, 0.5*dy - yos );
	text( "Impulse Response",17, 16 );

	/* - upper left
	 * -- Set world min & max values for this data set.
	 *    X axis is alway 0 to 20 for data sampled at 40 samples/sec (delta .025)
	 *    so think about taking out the getlims call for x min and max values. */
	getlims( cmmem.sacmem[Memptr[1]], NDATPTS, &ywmin, &ywmax );
	getlims( cmmem.sacmem[Memptr[2]], NDATPTS, &xwmin, &xwmax );

	getlims( cmmem.sacmem[Memptr[7]], NDATPTS, &ywmin1, &ywmax1 );
	if( ywmin1 < ywmin )
		ywmin = ywmin1;
	if( ywmax1 > ywmax )
		ywmax = ywmax1;

	setworld( xwmin, xwmax, ywmin, ywmax );
	setvport( xvsmin + 2.*xos, xvsmin + (xvsmax - xvsmin)/2.0 - xos, 
	 yvsmin + (yvsmax - yvsmin)/2.0 + yos, yvsmax - yos );

	settextfont( 1 );
	setcolorname( "black",6 );
	settextsize( (.667*.02), .02 );

	xaxis( "LINEAR", "BELOW", "BELOW", "Frequency (Hz)",15 );
	yaxis( "LINEAR", "LEFT", "LEFT", "Amplitude",10 );

	settextfont( 3 );
	settextsize( (.667*.03), .03 );

	move( 0.25*dx - 1.25*xos, 0.95*dy );
	text( "Amplitude Response",19, 18 );

	setlinestyle( 4 );
	setcolorname( "magenta",8 );
	worldpolyline( cmmem.sacmem[Memptr[2]], cmmem.sacmem[Memptr[1]], NDATPTS );

	setlinestyle( 1 );
	setcolorname( "blue",5 );
	loadxtmp( xdattmp, xwmin, xwmax, NDATPTS );
	worldpolyline( xdattmp, cmmem.sacmem[Memptr[7]], NDATPTS );

	/* - upper right
	 * -- Set world min & max values for this data set.
	 *    X axis is alway 0 to 20 for data sampled at 40 samples/sec (delta .025)
	 *    so think about taking out the getlims call for x min and max values. */
	getlims( cmmem.sacmem[Memptr[3]], NDATPTS, &ywmin, &ywmax );
	getlims( cmmem.sacmem[Memptr[4]], NDATPTS, &xwmin, &xwmax );

	getlims( cmmem.sacmem[Memptr[8]], NDATPTS, &ywmin1, &ywmax1 );
	if( ywmin1 < ywmin )
		ywmin = ywmin1;
	if( ywmax1 > ywmax )
		ywmax = ywmax1;

	setworld( xwmin, xwmax, ywmin, ywmax );
	setvport( xvsmin + (xvsmax - xvsmin)/2.0 + xos, xvsmax - xos, 
	 yvsmin + (yvsmax - yvsmin)/2.0 + yos, yvsmax - yos );

	setlinestyle( 4 );
	setcolorname( "magenta",8 );
	worldpolyline( cmmem.sacmem[Memptr[4]], cmmem.sacmem[Memptr[3]], NDATPTS );

	setlinestyle( 1 );
	setcolorname( "blue",5 );
	loadxtmp( xdattmp, xwmin, xwmax, NDATPTS );
	worldpolyline( xdattmp, cmmem.sacmem[Memptr[8]], NDATPTS );

	settextfont( 1 );
	setcolorname( "black",6 );
	settextsize( (.667*.02), .02 );

	xaxis( "LINEAR", "BELOW", "BELOW", "Frequency (Hz)",15 );
	yaxis( "LINEAR", "LEFT", "LEFT", "Phase (degrees)",16 );

	settextfont( 3 );
	settextsize( (.667*.03), .03 );

	move( .75*dx - 1.5*xos, .95*dy );
	text( "Phase Response",15, 14 );

	/* - End the frame */
	endframe( FALSE , nerr );

	/*      call enddevice(kgddef,nerr) */

L_8888:
	plrest();
	return npulpts ;

} /* end of function */

/*===================================================================== */
void /*FUNCTION*/ getlims(datary, npts, min_, max_)
float datary[];
int npts;
float *min_, *max_;
{
	int idx ;

	float *const Datary = &datary[0] - 1;


	/* - To determine the min and max values in the data array. */
	/* -- Initialize max to the smallest possible value, min to the largest. */
	*max_ = -VLARGE;
	*min_ =  VLARGE;

	for( idx = 1; idx <= npts; idx++ ){
		if( Datary[idx] > *max_ )
			*max_ = Datary[idx];
		if( Datary[idx] < *min_ )
			*min_ = Datary[idx];
	}

	return;
} /* end of function */
/*===================================================================== */
void /*FUNCTION*/ loadxtmp(xtmp, xwmin, xwmax, npts)
float xtmp[];
double xwmin, xwmax;
int npts;
{
	int idx ;
	float dx;

	float *const Xtmp = &xtmp[0] - 1;


	/* - To assign values to a temporary 'xtmp' array corresponding to
	 *   the min and max window positions for the number of pts requird. */
	dx = (xwmax - xwmin)/(npts - 1);
	Xtmp[1] = xwmin;
	for( idx = 2; idx <= npts; idx++ ){
		Xtmp[idx] = Xtmp[idx - 1] + dx;
	}

	return;
} /* end of function */
/*===================================================================== */
void /*FUNCTION*/ sduration(s, npts, min_, max_)
float s[];
int *npts, *min_, *max_;
{
	int n, ploc;
	float denom, durat, numer;

	float *const S = &s[0] - 1;


	/*   Computes the statistical duration of a signal to return the
	 *   x data array min and max positions for the data to be displayed. */
	numer = 0.0;
	denom = 0.0;

	/* - Find the center of mass of the signal. */

	for( n = 1; n <= *npts; n++ ){
		numer = numer + n*powi(S[n],2);
		denom = denom + powi(S[n],2);
	}
	ploc = numer/denom;

	/* - Find the duration of the signal */

	numer = 0.0;
	denom = 0.0;

	for( n = 1; n <= *npts; n++ ){
		numer = numer + powi(S[n],2)*ipow(n - ploc,2);
		denom = denom + powi(S[n],2);
	}

	/* - I'm using a kludge to get a reasonable duration. The actual 
	 *   algorithm for the signal duration, in number of porints, is:
	 *        sqrt(n/d) * sqrt(12.0)
	 *   Turns out that it is too short for most impulse response
	 *   signals, so we multiply by some constant. */

	durat = sqrt( numer/denom )*sqrt( 12. )*15;

	/* - Find the begin and end locations in the array for the signal
	 *   and the number of points to be plotted.  Move 20% left and 80%
	 *   right of the center of the signals duration. */

	*min_ = ploc - .2*durat;
	if( *min_ < 1 )
		*min_ = 1;

	*max_ = ploc + .8*durat;
	if( *max_ > *npts )
		*max_ = *npts;

	*npts = *max_ - *min_ + 1;

	return;
} /* end of function */

