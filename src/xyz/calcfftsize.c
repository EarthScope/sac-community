
#include <stdio.h>

#include "xyz.h"
#include "spectrogram.h"

/* 
 * 
 *  Name:     CALCFFTSIZE
 *  
 * 
 *  Summmary:  Calculate various storage sizes for
 *             FFT calculations.
 * 
 *  
 * 
 *  Usage/Calling sequence:
 *  
 *     ierr = CALCFFTSIZE ( delta,sliceint,window,iorfft,lfft
 *                          nptswndw,buffersize,windowovrl )
 *  
 * 
 *  Arguments:
 * 
 *     delta       =:    sampling interval - sac/samples(real)
 *     sliceint    =:    image slice interval - seconds (real)
 *     window      =:    data window size in seconds(real)
 *     iorfft       :=   log base 2 of lfft(int)
 *     lfft         :=   size of data for fft calc - must be power of 2(int)
 *     nptswndw     :=   # pts in window
 *     buffersize   :=   size of buffer to read in data(int)
 *     windowovrl   :=   overlap of window(int)
 * 
 *  
 *   Returns:
 *  
 *       calcfftsize = 0 no error
 *       calcfftsize > 0 error
 * 
 *   Notes:
 * 
 *    
 *        By:    T.M.Quinn
 *        On:    12/18/89
 *  
 *        Updates:
 *  
 *             By:
 *             On:
 *             Subj:
 *  
 *             By:
 *             On:
 *             Subj:
 *  
 *   */

int /*FUNCTION*/ calcfftsize(delta, sliceint, window, iorfft, lfft, 
	 nptswndw, buffersize, windowovrl)
double delta, sliceint, window;
int *iorfft, *lfft, *nptswndw, *buffersize, *windowovrl;
{
	int calcfftsize_v, n;

	/*     * Include Files: */
	/*     * Local Variables: */
	/*     * Code Implementation: */
	calcfftsize_v = 1;

	/*         Convert window from seconds to number of points */
	*nptswndw = window/delta;

	/*         Calculate FFT size */
	*lfft = 2;
	*iorfft = 1;

	/*          FFT size must be at least twice the size of the window */
L_1:
	;
	if( *lfft < 2**nptswndw ){
		*lfft = *lfft*2;
		*iorfft = *iorfft + 1;
		goto L_1;
		}

	/*         Calculate window overlap from 'sliceint' */
	*windowovrl = *nptswndw - sliceint/(delta + .0000001);

	/*         Calculate optimum buffersize for reading in data */
	n = 1;
L_3:
	;
	if( (MAXBUFSIZE - *windowovrl)/(*nptswndw - *windowovrl) > n ){
		n = n + 1;
		goto L_3;
		}
	*buffersize = n**nptswndw - (n - 1)**windowovrl;

	/*         Check if FFT size is 'legal' */
	if( *lfft > MAXLFFT ){
		fprintf( stdout, "Error fft size greater than the largest allowed of %d .\n", 
		 MAXLFFT );
		}
	else{
		fprintf( stdout, "Window size: %d  Overlap: %d  FFT size: %d \n", 
		 *nptswndw, *windowovrl, *lfft );
		calcfftsize_v = 0;
		}


	return( calcfftsize_v );
} /* end of function */

