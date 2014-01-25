/** 
 * @file   lp.c
 * 
 * @brief  Generate a Second Order Section for a Low Pass Filter
 * 
 */

#include <string.h>

#include "complex.h"
#include "dbh.h"

/** 
 *  Subroutine to generate second order section parameterization
 *    from an pole-zero description for lowpass filters.
 * 
 * 
 * @param p 
 *    Array of Poles
 * @param z 
 *    Array of Zeros
 * @param rtype 
 *    Character array containing root type information
 *      - "SP"  Single real pole
 *      - "CP"  Complex conjugate pole pair
 *      - "CPZ" Complex conjugate pole and zero pairs
 * @param rtype_s
 *     Length of \p rtype  
 * @param dcvalue 
 *     Zero-frequency value of prototype filter
 * @param nsects 
 *     Number of second-order sections
 * @param sn 
 *     Output Numerator polynomials for second order sections.
 * @param sd 
 *     Output Denominator polynomials for second order sections.
 *
 * @copyright  Copyright 1990  Regents of the University of California
 *
 * @author:  Dave Harris
 *           Lawrence Livermore National Laboratory
 *           L-205
 *           P.O. Box 808
 *           Livermore, CA  94550
 *           USA
 *           (415) 423-0617
 *
 * \date   020416: Changed SN and SD adjustable arrays to use 
 *                 "*" rather than "1". - wct
 */
void 
lp(complexf *p, 
   complexf *z, 
   char     *rtype, 
   int       rtype_s, 
   double    dcvalue, 
   int       nsects, 
   float    *sn, 
   float    *sd)
{

#define RTYPE(I_,J_)	(rtype+(I_)*(rtype_s)+(J_))

        int i, i_, iptr;
	float scale;

	complexf *const P = &p[0] - 1;
	float *const Sd = &sd[0] - 1;
	float *const Sn = &sn[0] - 1;
	complexf *const Z = &z[0] - 1;

	iptr = 1;
	for( i = 1; i <= nsects; i++ ){
		i_ = i - 1;

		if( memcmp(RTYPE(i_,0),"CPZ",3) == 0 ){

			scale = cmplxtof( cmplxmul(P[i],cmplxcj( P[i] )) )/cmplxtof( cmplxmul(Z[i],
			 cmplxcj( Z[i] )) );
			Sn[iptr] = cmplxtof( cmplxmul(Z[i],cmplxcj( Z[i] )) )*scale;
			Sn[iptr + 1] = -2.*cmplxtof( Z[i] )*scale;
			Sn[iptr + 2] = 1.*scale;
			Sd[iptr] = cmplxtof( cmplxmul(P[i],cmplxcj( P[i] )) );
			Sd[iptr + 1] = -2.*cmplxtof( P[i] );
			Sd[iptr + 2] = 1.;
			iptr = iptr + 3;

			}
		else if( memcmp(RTYPE(i_,0),"CP",2) == 0 ){

			scale = cmplxtof( cmplxmul(P[i],cmplxcj( P[i] )) );
			Sn[iptr] = scale;
			Sn[iptr + 1] = 0.;
			Sn[iptr + 2] = 0.;
			Sd[iptr] = cmplxtof( cmplxmul(P[i],cmplxcj( P[i] )) );
			Sd[iptr + 1] = -2.*cmplxtof( P[i] );
			Sd[iptr + 2] = 1.;
			iptr = iptr + 3;

			}
		else if( memcmp(RTYPE(i_,0),"SP",2) == 0 ){

			scale = -cmplxtof( P[i] );
			Sn[iptr] = scale;
			Sn[iptr + 1] = 0.;
			Sn[iptr + 2] = 0.;
			Sd[iptr] = -cmplxtof( P[i] );
			Sd[iptr + 1] = 1.;
			Sd[iptr + 2] = 0.;
			iptr = iptr + 3;

			}

		}

	Sn[1] = dcvalue*Sn[1];
	Sn[2] = dcvalue*Sn[2];
	Sn[3] = dcvalue*Sn[3];


	return;
#undef	RTYPE

}



