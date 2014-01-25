/** 
 * @file   srtndx.c
 *
 * @brief  Sort a floating point array
 * 
 */

#include "ucf.h"
#include "bool.h"


#include "msg.h"

#define	MAX_	512

/** 
 * Sort a floating point array
 * 
 * @param value 
 *    Array to be sorted on input
 *    Sorted array on output
 * @param num 
 *    Length of \p value
 * @param index 
 *    Sorted indicies
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   800903:  Original version.
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   810416:  Replaced CMWORK with local storage.
 *
 */
void 
srtndx(float *value, 
       int    num, 
       int   *index, 
       int   *nerr) {

	int lagain;
	int i, j, nax;
	float svalue[MAX_], v;

	int *const Index = &index[0] - 1;
	float *const Svalue = &svalue[0] - 1;
	float *const Value = &value[0] - 1;


	/* PROCEDURE: */
	*nerr = 0;

	/* RANGE CHECK ON NUM */

	if( num <= 0 || num > MAX_ ){
		nax = MAX_;
		*nerr = 910;
		setmsg( "ERROR", *nerr );
		apimsg( nax );
		goto L_8888;
		}

	/* SET UP INDEX AND SCRATCH ARRAYS */

	for( j = 1; j <= num; j++ ){
		Svalue[j] = Value[j];
		Index[j] = j;
		}

	/* SORT BOTH SCRATCH AND INDEX ARRAYS BASED ON SCRATCH ARRAY */

L_2000:
	lagain = FALSE;
	for( j = 1; j <= (num - 1); j++ ){

                if ((Svalue[j] - Svalue[j + 1]) <= 0.0 )
                                goto L_4000;


		v = Svalue[j];
		Svalue[j] = Svalue[j + 1];
		Svalue[j + 1] = v;
		i = Index[j];
		Index[j] = Index[j + 1];
		Index[j + 1] = i;
		lagain = TRUE;
L_4000:
		;
		}
	if( lagain )
		goto L_2000;

L_8888:
	return;

}

