/** 
 * @file   apply.c
 * 
 * @brief  Apply a IIR (Infinite Impluse Reponse) Filter
 * 
 */

#include "dbh.h"

/** 
 * Apply an IIR (Infinite Impulse Response) Filter to a data 
 *   sequence.  The filter is assumed to be stored as second
 *   order sections.  The filtering is done in place.  Zero-phase
 *   (forward plus reverse filtering) is an option.
 * 
 * @param data 
 *    Array containing data on input and filtered data on output
 * @param nsamps 
 *    Length of array \p data
 * @param zp 
 *    If Zero Phase filtering is requested
 *    - TRUE Zero phase Two pass filtering (forward + reverse filters)
 *    - FALSE Single Pass filtering
 * @param sn 
 *    Numerator polynomials for 2nd Order Sections
 * @param sd 
 *    Denominator polynomials for 2nd Order Sections
 * @param nsects 
 *    Number of 2nd Order Sections
 *
 * @return Nothing
 *
 *  @copyright 1990  Regents of the University of California                      
 *
 *
 *  @author  Dave Harris
 *           Lawrence Livermore National Laboratory
 *           L-205
 *           P.O. Box 808
 *           Livermore, CA  94550
 *           USA
 *
 * @note 
 *    y_n = \Sum_1^\p N (b_0 * x_n + b1 * x_{n-1} + b2 * x_{n-2} - 
 *                                   a1 * y_{n-1} + b2 * y_{n-2} )
 *    where 
 *       - N = \p nsamps
 *       - b = \p sn
 *       - a = \p sd
 *     
 */
void 
apply(float    *data, 
      int       nsamps, 
      int       zp, 
      float    *sn, 
      float    *sd, 
      int       nsects)
{
	int i, j, jptr;
	float a1, a2, b0, b1, b2, output, x1, x2, y1, y2;

	float *const Data = &data[0] - 1;
	float *const Sd = &sd[0] - 1;
	float *const Sn = &sn[0] - 1;

	jptr = 1;
	for( j = 1; j <= nsects; j++ ){

		x1 = 0.0;
		x2 = 0.0;
		y1 = 0.0;
		y2 = 0.0;
		b0 = Sn[jptr];
		b1 = Sn[jptr + 1];
		b2 = Sn[jptr + 2];
		a1 = Sd[jptr + 1];
		a2 = Sd[jptr + 2];

		for( i = 1; i <= nsamps; i++ ){

			output = b0*Data[i] + b1*x1 + b2*x2;
			output = output - (a1*y1 + a2*y2);
			y2 = y1;
			y1 = output;
			x2 = x1;
			x1 = Data[i];
			Data[i] = output;

			}

		jptr = jptr + 3;

		}

	if( zp ){

		jptr = 1;
		for( j = 1; j <= nsects; j++ ){

			x1 = 0.0;
			x2 = 0.0;
			y1 = 0.0;
			y2 = 0.0;
			b0 = Sn[jptr];
			b1 = Sn[jptr + 1];
			b2 = Sn[jptr + 2];
			a1 = Sd[jptr + 1];
			a2 = Sd[jptr + 2];

			for( i = nsamps; i >= 1; i-- ){

				output = b0*Data[i] + b1*x1 + b2*x2;
				output = output - (a1*y1 + a2*y2);
				y2 = y1;
				y1 = output;
				x2 = x1;
				x1 = Data[i];
				Data[i] = output;

				}

			jptr = jptr + 3;

			}

		}

	return;
}

