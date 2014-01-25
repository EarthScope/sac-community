/** 
 * @file   bilin2.c
 * 
 * @brief  Transform an analog filter to a digital via the
 *         bilinear transformation.
 */

#include "dbh.h"

/** 
 * Transform an analog filter to a digital filter via the bilinear 
 *    transformation.  Assumes both filters are stored as 2nd Order
 *    sections and the transform is done in place.
 * 
 * @param sn 
 *    Array containing numerator polynomial coefficeients. Packed head to
 *      tail and using groups of 3.  Length is 3 * \p nsects
 * @param sd 
 *    Array containing demoninator polynomial coefficeients. Packed head to
 *      tail and using groups of 3.  Length is 3 * \p nsects
 * @param nsects 
 *    Number of 2nd order sections.
 * 
 * @return Nothing
 *
 * @copyright 1990  Regents of the University of California
 *
 * @author  Dave Harris                                                         
 *           Lawrence Livermore National Laboratory
 *           L-205
 *           P.O. Box 808
 *           Livermore, CA  94550
 *           USA
 *           (415) 423-0617
 *
 * \note
 *    - scale = a0 + a1 + a2
 *    - 
 *    - a_0 = 1.0
 *    - a_1 = 2.0 * (a_0 - a_2) / scale
 *    - a_2 = (a_2 - a_1 + a_0) / scale
 *    - 
 *    - b_0 = (b_2 + b_1 + b_0) / scale
 *    - b_1 = 2.0 * (b_0 - b_2) / scale
 *    - b_2 = (b_2 - b_1 + b_0) / scale
 *
 * \todo Further Documentation 
 */
void 
bilin2(float    *sn, 
       float    *sd, 
       int       nsects)
{
	int i, iptr;
	double a0, a1, a2, scale;

	float *const Sd = &sd[0] - 1;
	float *const Sn = &sn[0] - 1;

	iptr = 1;
	for( i = 1; i <= nsects; i++ ){

		a0 = Sd[iptr];
		a1 = Sd[iptr + 1];
		a2 = Sd[iptr + 2];

		scale = a2 + a1 + a0;
		Sd[iptr] = 1.;
		Sd[iptr + 1] = (2.*(a0 - a2))/scale;
		Sd[iptr + 2] = (a2 - a1 + a0)/scale;

		a0 = Sn[iptr];
		a1 = Sn[iptr + 1];
		a2 = Sn[iptr + 2];

		Sn[iptr] = (a2 + a1 + a0)/scale;
		Sn[iptr + 1] = (2.*(a0 - a2))/scale;
		Sn[iptr + 2] = (a2 - a1 + a0)/scale;

		iptr = iptr + 3;

		}

	return;
} 

