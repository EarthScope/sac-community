/** 
 * @file   iirfilter.c
 * 
 * @brief  Apply an IIR Filter
 */

#include "dbh.h"

/** 
 * Apply an IIR Filter to a data sequence.  The filter is assumed to be
 *   stored as 2nd Order Sections and filtering is done in place.
 * 
 * @param data 
 *    Input array to be filtered
 * @param nsamples 
 *    Length of array \p data
 * @param a 
 *    Array of Gains for 2nd Order Sections, length \p nsects
 * @param sn 
 *    Numerator polynomilals for 2nd Order Sections
 *    Length \p nsects * 2
 *    Assumes the 0th numerator coefficient is 1
 * @param sd 
 *    Demoninator polynomials for 2nd Order Sections
 *    Length \p nsects * 2
 *    Assumes the 0th numerator coefficient is 1
 * @param nsects 
 *    Number of 2nd Order Sections Length of arrays \p a
 * @param states 
 *    Internal states that must be saved for blocked data
 *    Length \p nsects * 2
 * @param fdata 
 *    Output filtered array
 *
 * @note
 *                A second order section looks like:                     
 *
 *                                    -1       -2                        
 *                            1 + b1*z   + b2*z            b1 = sn(2*j-1)
 *                                                         b2 = sn(2*j)  
 *               Sj(z) = g * ---------------------          g = a(j)     
 *                                    -1       -2          a1 = sd(2*j-1)
 *                            1 + a1*z   + a2*z            a2 = sd(2*j)  
 *
 *  @copyright   1991  Regents of the University of California
 *
 *
 *  @author  Dave Harris
 *           Lawrence Livermore National Laboratory
 *           L-205
 *           P.O. Box 808
 *           Livermore, CA  94550
 *           USA
 *           (415) 423-0617
 *
 *  @date    910630  Last Modified
 *
 */
void 
iirfilter(float    *data, 
	  int       nsamples, 
	  float    *a, 
	  float    *sn, 
	  float    *sd, 
	  int       nsects, 
	  float    *states, 
	  float    *fdata)
{
	int i, j, jptr;
	float a1, a2, b1, b2, g, s0, s1, s2;

	float *const A = &a[0] - 1;
	float *const Data = &data[0] - 1;
	float *const Fdata = &fdata[0] - 1;
	float *const Sd = &sd[0] - 1;
	float *const Sn = &sn[0] - 1;
	float *const States = &states[0] - 1;

	jptr = 1;
	for( i = 1; i <= nsamples; i++ ){
		Fdata[i] = Data[i];
		}
	for( j = 1; j <= nsects; j++ ){

		s1 = States[jptr];
		s2 = States[jptr + 1];
		g = A[j];
		b1 = Sn[jptr];
		b2 = Sn[jptr + 1];
		a1 = Sd[jptr];
		a2 = Sd[jptr + 1];

		for( i = 1; i <= nsamples; i++ ){

			s0 = g*Fdata[i] - a1*s1 - a2*s2;
			Fdata[i] = s0 + b1*s1 + b2*s2;
			s2 = s1;
			s1 = s0;

			}

		States[jptr] = s1;
		States[jptr + 1] = s2;

		jptr = jptr + 2;

		}


	return;
}

