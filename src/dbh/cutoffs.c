/** 
 * @file   cutoffs.c
 * 
 * @brief  Alter a cutoff of a filter.
 */

#include "dbh.h"

/** 
 * Alter the cutoff of a filter.  Assumed that the filter
 * is structured as 2nd order sections.  Changes the cutoffs
 * of a normalized lowpass or highpass filters through a 
 * simple polynomial transformation.
 * 
 * @param sn 
 *   Numerator polynomials for 2nd order sections
 * @param sd 
 *   Denominator polynomials for 2nd order sections
 * @param nsects
 *   Number of 2nd order sections 
 * @param f 
 *   New cutoff frequency
 * 
 * @return Nothing
 *
 * \copyright 1990  Regents of the University of California
 *
 * \author   Dave Harris
 *           Lawrence Livermore National Laboratory
 *           L-205
 *           P.O. Box 808
 *           Livermore, CA  94550
 *           USA
 *           (415) 423-0617
 *
 */
void 
cutoffs(float     *sn, 
        float     *sd, 
        int        nsects, 
        double     f)
{
	int i, iptr;
	float scale;

	float *const Sd = &sd[0] - 1;
	float *const Sn = &sn[0] - 1;

	scale = 2.*3.14159265*f;

	iptr = 1;
	for( i = 1; i <= nsects; i++ ){

		Sn[iptr + 1] = Sn[iptr + 1]/scale;
		Sn[iptr + 2] = Sn[iptr + 2]/(scale*scale);
		Sd[iptr + 1] = Sd[iptr + 1]/scale;
		Sd[iptr + 2] = Sd[iptr + 2]/(scale*scale);
		iptr = iptr + 3;

		}

	return;
} 

