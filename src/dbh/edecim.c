/** 
 * @file   edecim.c 
 * 
 * @brief  
 *
 */

#include "dbh.h"

/** 
 * Efficient Decimator to filter and/or decimate using a FIR Filter
 * 
 * @param data 
 *    Array containing real signal
 * @param ndata 
 *    Length of array \p data
 * @param irate 
 *    Integer decimation (resampling) rate.  If \p irate = 1, then
 *    the data is filtered but not decimated.
 * @param ddata 
 *    Array containing the filtered / decimated data
 * @param nddata 
 *    Length of array \p ddata
 * @param c 
 *    Array of FIR filter Coefficients
 * @param nc 
 *    Number of Filter Coefficients and Length of array \p c
 * @param isym 
 *    Filter Symmetry
 *    - 1  Evenly Symmetric Filter
 *    - -1 Oddly Symmetric Filter
 *
 * @return Nothing
 *
 * @note:  The filter coefficient sequency \p c is assumed to be 
 *         symmetric, so only half (+1) of the coeffients are assumed
 *         to be stored in \p c.  The "Symmetry Point" coefficient is 
 *         stored in \p c(1) and the coefficients to the right of the 
 *         "Symmetry Point" are stored in ascending order, \p c(2) ...
 *         \p c(nc)
 *
 *  @author  Dave Harris
 *
 *  @date    801117   Last Modified
 * 
 */
void 
edecim(float     *data, 
       int        ndata, 
       int        irate, 
       float     *ddata, 
       int       *nddata, 
       float     *c, 
       int        nc, 
       int        isym)
{
	int in, j, k, ncm1, out;
	float add, temp;

	float *const C = &c[0] - 1;
	float *const Data = &data[0] - 1;
	float *const Ddata = &ddata[0] - 1;

	/*  CALCULATING NEW DATA LENGTH
	 * */
	*nddata = ndata/irate;

	/*  LOOP TO FILTER/DECIMATE DATA
	 *
	 *    INITIALIZE INPUT AND OUTPUT POINTERS
	 * */
	in = 1;
	out = 1;

	/*    CALCULATE OUTPUT POINTS UNTIL OUT EXCEEDS THE NUMBER OF NEW POINTS
	 * */
	ncm1 = nc - 1;
L_1:
	;
	if( out > *nddata )
		goto L_2;
	temp = C[1]*Data[in];
	for( j = 1; j <= ncm1; j++ ){
		add = 0.;
		k = in + j;
		if( k <= ndata ){
			add = Data[k];
			}
		k = in - j;
		if( k > 0 ){
			add = add + Data[k]*isym;
			}
		temp = temp + C[j + 1]*add;
		}
	;
	Ddata[out] = temp;

	/*    UPDATE DATA POINTERS
	 * */
	in = in + irate;
	out = out + 1;

	/*  DONE LOOP
	 * */
	goto L_1;
L_2:
	;

	/*  BYE
	 * */
	return;
}

