/** 
 * @file   wiener.c
 * 
 * @brief  Compute a Wiener error prediction filter
 * 
 */

#include <math.h>

#include "dbh.h"


#include "msg.h"

/** 
 *  Program to design and apply a prediction error filter to a given signal
 *
 * @param data
 *    array containing input data sequence
 * @param nsamps
 *    length of sequence in data, which is assumed to start at data(1)
 * @param start
 *    integer variable containing the starting
 *    index defining the window used to
 *    estimate the noise autocorrelation.
 *    start must be greater than zero
 * @param wlen
 *    integer variable containing the length of the window in samples
 * @param nc
 *    the number of filter coefficients, maximum:  100
 * @param lmu
 *    1 if mu is set, else 0
 * @param mu
 *    real*4 variable containing adaptation parameter
 * @param lepsilon
 *    1 if epsilon is set, else 0
 * @param epsilon
 *    lmu, lepsilon, and epsilon added 960723, maf
 * @param fdata
 *    array containing filtered output sequence, which may be the same array
 *    as data.
 * @param ierr
 *    integer error condition code
 *     = 0    no error
 *     = 1    numerical instability in levinson recursion
 *
 *
 *  @author  Dave Harris
 *
 *  @date April 1, 1997  Last Modified
 *  @date March 6, 1997
 *  @date July 23, 1996
 *  @date February 5, 1981
 *
 * 
 */

void 
wiener(float  *data, 
       int     nsamps, 
       int     start, 
       int     wlen, 
       int     nc, 
       int     lmu, 
       double  mu, 
       int     lepsilon, 
       double  epsilon, 
       float  *fdata, 
       int    *ierr) {

	int i, j, k, point;
	float	a[100],
		buffer[100],
		reflct[100],
		rho[100],
		originalRho;	/* added to allow variable epsilon.  maf 970306 */
	double e1, err ;	/* changed err from float to double, maf 960723 */

	float *const A = &a[0] - 1;
	float *const Buffer = &buffer[0] - 1;
	float *const Data = &data[0] - 1;
	float *const Fdata = &fdata[0] - 1;
	float *const Reflct = &reflct[0] - 1;
	float *const Rho = &rho[0] - 1;




	/*  ESTIMATE AUTOCORRELATION FUNCTION
	 * */
	zero( rho, nc );
	for( i = 1; i <= nc; i++ ){
	    for( j = 0; j <= (wlen - i); j++ ){
		k = j + start;

		if ( k > 0 && k + i - 1 < nsamps ) {	/* check array limits. maf 970401 */
			/* added casting operators, maf 960723 */
		    Rho[i] +=  (double) ( Data[k] ) * (double) ( Data[k + i - 1] ) ;
		}
	    } /* end for ( j ) */
	} /* end for ( i ) */

	/* IF EPSILON IS NOT SET, SET IT TO DEFAULT maf 960723 */
	if ( !lepsilon )
	    epsilon = 0.0 ;

	originalRho = Rho[1] ;	/* added to allow variable epsilon.  maf 970306 */

	do {	/* This do loop allows epsilon to vary from zero to 0.01,  maf 970306 */
	    /*  REGULARIZE DIAGONAL ELEMENT, 
	        RHO[1]==> RHO[1] * (1.0 + EPSILON) maf 960723 */
	    Rho[1] = (double) ( originalRho ) * ( 1.0 + epsilon ) ;

	    /*  CALCULATE PREDICTION FILTER COEFFICIENTS
	     * */
	    levin( rho, a, reflct, nc );

	    /*  CHECK NUMERICAL STABILITY OF LEVINSON RECURSION
	     * */
	    *ierr = 0;
	    for( i = 1; i <= (nc - 1); i++ ){
		if( fabs( Reflct[i] ) > .999 ){
		    *ierr = 1;
		    break ;
		}
	    }

	    if ( *ierr && !lepsilon ) {
		if ( epsilon == 0.0 )
		    epsilon = 0.00001 ;
		else
		    epsilon *= 10. ;

		/* Send a message about increasing epsilon unless epsilon is 
		   too big */
		if ( epsilon < 0.1 ) {
		    setmsg ( "WARNING", 1614 ) ;
		    apfmsg ( epsilon ) ;
		    outmsg () ;
		    clrmsg () ;
		}
	    }
	}while ( !lepsilon && *ierr && epsilon < 0.1 ) ;

	/*  FILTER DATA
	 *
	 *    INITIALIZE BUFFER
	 * */
	zero( buffer, nc );

	/*    INITIALIZE POINTER
	 * */
	point = 1;

	/*    LOOP
	 * */

	while( point <= nsamps ) {
	    /*    FETCH INPUT DATUM
	     * */
	    Buffer[1] = Data[point];

	    /*    CALCULATE NEW ERROR POINT
	     * */
	    e1 = err = Buffer[1];
	    for( i = 2; i <= nc; i++ ){ /* casting operators added. maf 960723 */
		e1 = e1 + (double) ( Buffer[i] ) * (double) ( A[i] ) ;
	    }

	    Fdata[point] = e1;

	    /*    UPDATE FILTER COEFFICIENTS
	     * */		 /* replaced !lmu with lmu maf 960801 */
	    if ( lmu ) {	/* if the user wants mu calculated */
		mu = 0.0 ;  /* figure it out. maf 960723 */
		for ( i = 1 ; i <= nc ; i++ ){
		    mu += fabs ( (double) Rho[i] ) ;
		} /* end for */

		mu = fabs( 1.95 / mu ) ;
	    } /* end if */
 
	    for( i = 2; i <= nc; i++ ){ /* casting operators added. maf 960723 */
		A[i] = (double) ( A[i] ) - mu * err * (double) ( Buffer[i] ) ;
	    }

	    /*    SHIFT BUFFER
	     * */
	    for( i = 2; i <= nc; i++ ){
		k = nc + 2 - i;
		Buffer[k] = Buffer[k - 1];
	    }

	    /*    UPDATE POINTER
	     * */
	    point = point + 1;
	} /* end while */

	/*  DONE
	 * */
	return;
} /* end of function */

