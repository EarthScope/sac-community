/** 
 * @file   fft.c
 * 
 * @brief  Compute the Fast Fourier Transform of a Sequence
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dbh.h"
#include "co.h"

/** 
 * Compute the Fast Fourier Transform (FFT) of a sequence.
 * 
 * @param xreal 
 *    Real Part of a Sequence
 * @param ximag 
 *    Imaginary Part of a Sequence
 * @param n 
 *    Length of arrays \p xreal and \p ximag
 * @param idir 
 *    Direction of the Transform
 *    - -1 Forward Transform 
 *    -  1 Inverse Transform (Normalization performed here)
 *    
 * @note The transform result is retured in \p xreal and \p ximag
 *       and the initial sequence is destroyed
 *  
 * @note Internal Variables:
 *    - tcos  Cosine Table
 *    - tsin  Sine Table
 *    - logn  Base-2 Logarithm of \p n
 *
 * @author  Dave Harris
 *          L-205
 *          Lawrence Livermore National Laboratory
 *          Livermore, CA  94550
 *
 * @date    771114 Last Modified 
 *
 */
void 
fft(float    *xreal, 
    float    *ximag, 
    int       n, 
    int       idir)
{
        int i, i1, iblock, icount, irel, iti;
	int j, k, logn, nblock, nby2, nm1, nstage;
	float cosine, scale, sine, *tcos, temp, *tsin;
	double dcosn, dsine, fund;

	float *Tcos;
	float *Tsin;
	float *const Ximag = &ximag[0] - 1;
	float *const Xreal = &xreal[0] - 1;

	nby2 = n/2;

        if((tcos = (float *)malloc(nby2*sizeof(float))) == NULL){
	    printf("memory allocation failed in fft\n");
	    return;
	}

        if((tsin = (float *)malloc(nby2*sizeof(float))) == NULL){
	    printf("memory allocation failed in fft\n");
	    free(tcos);
	    return;
	}

        Tcos = tcos - 1;
        Tsin = tsin - 1;

	for ( logn = 1 ; ipow ( 2 , logn ) != n ; logn++ ) ;


	/*     TABLE GENERATOR
	 * */
	fund = 3.141592653589793e0*2.0e0;
	fund = fund/(float)( n );
	dcosn = cos( fund );
	dsine = sin( fund );
	if( idir == -1 )
	    dsine = -dsine;

	Tcos[1] = 1.;
	Tsin[1] = 0.;
	for( i = 2; i <= nby2; i++ ){
	    Tcos[i] = dcosn*Tcos[i - 1] - dsine*Tsin[i - 1];
	    Tsin[i] = dcosn*Tsin[i - 1] + dsine*Tcos[i - 1];
	}

	/*     BIT REVERSE CODE
	 * */
	nm1 = n - 1;
	j = 1;
	for( i = 1; i <= nm1; i++ ){
	    if( i < j ){
		temp = Xreal[i];
		Xreal[i] = Xreal[j];
		Xreal[j] = temp;
		temp = Ximag[i];
		Ximag[i] = Ximag[j];
		Ximag[j] = temp;
	    }

	    for ( k = nby2 ; k < j ; k /= 2 )
		j = j - k;

	    j = j + k;
	}

	/*     INDEXING CODE
	 * */
	nblock = n;
	irel = 1;
	for( nstage = 1; nstage <= logn; nstage++ ){
	    if( nstage > 1 )
			irel = irel*2;

	    i = -irel;
	    nblock = nblock/2;
	    for( iblock = 1; iblock <= nblock; iblock++ ){
		i = i + irel;
		iti = 1 - nblock;
		for( icount = 1; icount <= irel; icount++ ){
		    i = i + 1;
		    iti = iti + nblock;
		    i1 = i + irel;

		    /* BUTTERFLY CODE */
		    if( nstage > 1 ){
			sine = Tsin[iti];
			cosine = Tcos[iti];
			temp = Xreal[i1]*cosine - Ximag[i1]*sine;
			Ximag[i1] = Xreal[i1]*sine + Ximag[i1]*cosine;
			Xreal[i1] = temp;
		    }
		    temp = Xreal[i] + Xreal[i1];
		    Xreal[i1] = Xreal[i] - Xreal[i1];
		    Xreal[i] = temp;
		    temp = Ximag[i] + Ximag[i1];
		    Ximag[i1] = Ximag[i] - Ximag[i1];
		    Ximag[i] = temp;

		} /* end for( icount ) */
	    } /* end for( iblock ) */
	} /* end for( nstage ) */

	/*  IF REVERSE TRANSFORM, DIVIDE THROUGH BY N
	 * */
	if( idir == 1 ){
	    scale = 1./(float)( n );
	    for( i = 1; i <= n; i++ ){
		Xreal[i] = Xreal[i]*scale;
		Ximag[i] = Ximag[i]*scale;
	    }
	}

	/* BYE */

        free(tcos);
        free(tsin);

	return;
}

