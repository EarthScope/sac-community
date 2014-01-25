
#include <string.h>

#include "icm.h"
#include "ucf.h"
#include "co.h"

/*                                                              PREDFL
 *
 *  Prediction filter, with options for 
 *    in-place processing.  Buffered for in-place
 *    processing.
 *
 *  Author:  George Randall, from PEF by Dave Harris
 *
 *  Created:  February 9, 1987
 *
 *  Last Modified:  February 9, 1987
 *
 *
 *
 *  Input Arguments:
 *  ----- ----------
 *
 *    DATA                   Real*4 input data array.
 *
 *    #POINTS                Number of data points.
 *
 *    A                      Real*4 array of filter coefficients.
 *
 *    FILTER_SIZE            Number of filter coefficients.
 *
 *
 *  Output Arguments:
 *  ------ ----------
 *
 *    RESULT                 Filtered output placed in this array.
 *                           May be the same array as DATA for in-place
 *                           filtering.
 *
 *    ERROR_MESSAGE          Character*130 error message variable.
 *
 *
 *  Linkage:  COPY, ZERO
 *
 * */
#define	NCMAX	12

struct t_pefcom {
	float inbuf[2000], outbuf[2000];
} pefcom;


void /*FUNCTION*/ predfl( data, npts, a, nc, result, errmsg )
float data[];
int npts;
float a[];
int nc;
float result[];
char *errmsg;
{
	int bufptr, datptr, k, lsamp, ncmp;
	float history[NCMAX-(0)+1];
	double eo;

	float *const A = &a[0] - 1;
	float *const Data = &data[0] - 1;
	float *const Inbuf = &pefcom.inbuf[0] - 1;
	float *const Outbuf = &pefcom.outbuf[0] - 1;
	float *const Result = &result[0] - 1;



	/*  Declarations
	 * */


	/*  Initializations
	 * */
	datptr = 1;

	if( nc > NCMAX ){
		strcpy( errmsg, " PREDFL - Filter too large " ) ;
	}
	else{
		errmsg[ 0 ] = '\0' ;
	}

	for( k = 0 ; k < 2000 ; k++ )
	    pefcom.inbuf[ k ] = pefcom.outbuf[ k ] = 0.0 ;

	for( k = 0 ; k < NCMAX + 1 ; k++ )
	    history[ k ] = 0.0 ;

	/*  Loop
	 * */
L_1:
	;
	if( datptr >= npts )
		goto L_2;

	/*    Calculate start and stop points for buffer index.
	 * */
	bufptr = 1;
	lsamp = min( 2000, npts - datptr + 1 );
	ncmp = lsamp;

	/*    Load new data into input buffer
	 * */
	/* copy( (int*)&Data[datptr], (int*)&Inbuf[1], ncmp ); */
	copy_float( &(Data[datptr]), &(Inbuf[1]), ncmp );

	/*    Filter data.
	 * */
L_3:
	;
	if( bufptr > lsamp )
		goto L_4;

	eo = Inbuf[bufptr];

	for( k = nc; k >= 1; k-- ){
		eo += A[k]*history[k];
		history[k] = history[k - 1];
	}

	Outbuf[bufptr] = eo;
	history[1] = eo;

	bufptr = bufptr + 1;

	goto L_3;
L_4:
	;

	/*    Store newly filtered data.
	 * */
	/* copy( (int*)&Outbuf[1], (int*)&Result[datptr], ncmp ); */
	copy_float( &(Outbuf[1]), &(Result[datptr]), ncmp );

	/*    Increment data pointer.
	 * */
	datptr = datptr + ncmp;

	goto L_1;
L_2:
	;

	/*  Done
	 * */
	return;
} /* end of function */

