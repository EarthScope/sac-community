/** 
 * @file   crscor.c
 * 
 * @brief  Compute Cross-Correlation Function
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbh.h"
#include "co.h"
#include "ucf.h"

struct t_big {
  float *caux;
  float *w;
  float *workr; 
  float *worki;
} big;

/** 
 * Compute the Cross-Correlation Function
 * 
 * @param data1 
 *    Array containing the first data sequence
 * @param data2 
 *    Array containing the second data sequence
 * @param nsamps 
 *    Number of samples in data sequence
 * @param nwin 
 *    Requested Number of windows
 * @param wlen 
 *    Requested number of samples in each window.  The 
 *    subroutine will calculate the window overlap. 
 *    Maximum value is 2048
 * @param type 
 *    Type of data analysis window to use.  Valid values
 *    are:
 *     -   <HAM>MING
 *     -   <HAN>NING
 *     -   <C>OSINE
 *     -   <R>ECTAN
 *     -   <T>RIANG
 * @param c 
 *    Output Array containing resulting 2*\p wlen -1 length 
 *    correlation coefficients.  The correlation sequence is 
 *    circularly rotated in the array so that the zeroth lag 
 *    is at the beginning.  Array dimensions 0:4095
 * @param nfft 
 *    Number of samples in the correlation sequence.  
 *    May be padded with zeros.
 * @param err 
 *    Error Message
 * @param err_s 
 *    Length of string \p err
 *
 * @return Nothing
 *
 * \author   Dave Harris
 *           L-205
 *           Lawrence Livermore National Laboratory
 *           Livermore, Ca  94550
 *
 * \date 800130  Created
 * \date 840621  Last Modified
 *
 *
 */
void 
crscor(float     *data1, 
       float     *data2, 
       int        nsamps, 
       int        nwin, 
       int        wlen, 
       char      *type, 
       float     *c, 
       int       *nfft, 
       char      *err, 
       int        err_s)
{
	char temp[131];
	int half, i, j, k, lsamp, nlags, nverlp, point;
	float scale, scale1, scale2, xi, xr, yi, yr;

	float *const Data1 = &data1[0] - 1;
	float *const Data2 = &data2[0] - 1;

	/*  Initializations
	 * */
	fstrncpy( err, err_s-1,  " ", 1 );


	/*  Check for legal window length and compute overlap
	 * */
	nlags = 2*wlen - 1;
	if( nwin < 1 ){

		fstrncpy( err, err_s-1, " CRSCOR - too few windows ", 26 );
		return;

		}
	else if( wlen < 1 || wlen > nsamps ){

		fstrncpy( err, err_s-1," CRSCOR - illegal window length " , 32 );
		return;

		}
	else{

		/*                                               Everything OK */

		if( nwin*wlen <= nsamps ){
			nverlp = 0;
			}
		else{
			nverlp = (nwin*wlen - nsamps)/(nwin - 1);
			if( nwin*wlen - nverlp*(nwin - 1) > nsamps ){
				nverlp = nverlp + 1;
				}
			}
		lsamp = wlen - 1;

		}


	/*  Find first power of two >= #LAGS
	 * */
	*nfft = 8;
L_2:
	;
	if( *nfft >= nlags )
		goto L_3;
	*nfft = *nfft*2;
	goto L_2;
L_3:
	;
	half = *nfft/2;


        if ((big.w = (float *)malloc(wlen*sizeof(float))) == NULL) {
          printf("memory allocation failed in crscor\n");
          goto L_8892;
	}

        if ((big.caux = (float *)malloc(*nfft*sizeof(float))) == NULL) {
          printf("memory allocation failed in crscor\n");
          goto L_8891;
	}

        if ((big.workr = (float *)malloc(*nfft*sizeof(float))) == NULL) {
          printf("memory allocation failed in crscor\n");
          goto L_8890;
	}

        if ((big.worki = (float *)malloc(*nfft*sizeof(float))) == NULL) {
          printf("memory allocation failed in crscor\n");
          goto L_8889;
	}

	/*  Generate window
	 * */
	for( i = 0; i <= lsamp; i++ ){
		big.w[i] = 1.;
		/*             I */
		}
	window( &big.w[0], wlen, type, 1, wlen, &big.w[0], err,err_s );

	/*  Check validity of window calculation
	 * */
	if( memcmp(err,"        ",8) != 0 ){
                fstrncpy(temp, 130, err, strlen(err));
                fstrncpy(temp+strlen(err),130-strlen(err), " (from CROSS)", 13);
                fstrncpy(err,err_s-1,temp,strlen(temp));
                goto L_8888;
		}

	/*  Compute cross-correlation function
	 *
	 *
	 *    Initialize window pointer
	 * */
	point = 1;

	/*    Initialize correlation arrays
	 * */
	zero( &c[0], *nfft );
	zero( &big.caux[0],*nfft );

	/*    Compute cross-spectrum for each window,  then average
	 * */
	for( i = 1; i <= nwin; i++ ){

		/*    Zero work arrays
		 * */
		zero( &big.workr[0], *nfft );
		zero( &big.worki[0], *nfft );

		/*    Load data into arrays
		 * */
        /* copy( (int*)&Data1[point], (int*)&big.workr[0], wlen ); */
        /* copy( (int*)&Data2[point], (int*)&big.worki[0], wlen ); */

		copy_float( &(Data1[point]), big.workr, wlen );
		copy_float( &(Data2[point]), big.worki, wlen );

		/*    Compute scale factors
		 * */
		scale1 = rms( &big.workr[0], wlen );
		scale2 = rms( &big.worki[0], wlen );
		scale = scale1*scale2;

		/*    Window and scale data
		 * */
		for( j = 0; j <= lsamp; j++ ){
			big.workr[j] = big.workr[j]*big.w[j]/scale1;
			big.worki[j] = big.worki[j]*big.w[j]/scale2;
			/*               J */
			}

		/*    Compute and average cross spectra
		 * */
		fft( &big.workr[0], &big.worki[0], *nfft, -1 );

		/*      Special case for point at 0
		 * */
		c[0] = c[0] + big.workr[0]*big.worki[0]*scale;

		/*      All other points
		 * */
		for( j = 1; j <= half; j++ ){

			k = *nfft - j;

			xr = (big.workr[j] + big.workr[k])*.5;
			xi = (big.worki[j] - big.worki[k])*.5;
			yr = (big.worki[j] + big.worki[k])*.5;
			yi = (big.workr[k] - big.workr[j])*.5;

			c[j] = c[j] + (xr*yr + xi*yi)*scale;
			big.caux[j] = big.caux[j] + (xr*yi - xi*yr)*scale;
			c[k] = c[j];
			big.caux[k] = -big.caux[j];

			}

		/*    Update window pointer
		 * */
		point = point + wlen - nverlp;

		}

	/*    Inverse fft for correlation computation
	 * */
	fft( &c[0], &big.caux[0], *nfft, 1 );

	/*  Bye
	 * */

L_8888:
        free(big.worki);

L_8889:
        free(big.workr);

L_8890:
        free(big.caux);

L_8891:
        free(big.w);

L_8892:
	return;
} 

void
cross_correlation_normalized(float *data1,
                             float *data2,
                             int   *npts_data,
                             float *xcorr,
                             int   *npts_xcorr) {
  int j;
  float squared_sum_master;
  float squared_sum_slave;
  float demon;
  
  squared_sum_master = 0.0;
  squared_sum_slave  = 0.0;
  for(j = 0; j < *npts_data; j++) {
    squared_sum_master += data1[j] * data1[j];
    squared_sum_slave  += data2[j] * data2[j];
  }
  demon = sqrt(squared_sum_master * squared_sum_slave);

  for(j = 0; j < *npts_xcorr; j++) {
    xcorr[j] = xcorr[j] / demon;
  }
}

void 
crscor_(float     *data1, 
        float     *data2, 
        int       *nsamps, 
        int       *nwin, 
        int       *wlen, 
        char      *type, 
        float     *c, 
        int       *nfft, 
        char      *err, 
        int        err_s) {
  crscor(data1, data2, *nsamps, *nwin, *wlen, type, c, nfft, err, err_s);
}

void 
crscor__(float     *data1, 
         float     *data2, 
         int       *nsamps, 
         int       *nwin, 
         int       *wlen, 
         char      *type, 
         float     *c, 
         int       *nfft, 
         char      *err, 
         int        err_s) {
  crscor(data1, data2, *nsamps, *nwin, *wlen, type, c, nfft, err, err_s);  
}








