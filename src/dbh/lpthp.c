/** 
 * @file   lpthp.c
 * 
 * @brief  Convert a Low Pass Filter to a High Pass Filter
 * 
 */

#include <string.h>

#include "complex.h"
#include "dbh.h"

/*
 *  Subroutine to convert a lowpass filter to a highpass filter via
 *    an analog polynomial transformation.  The lowpass filter is
 *    described in terms of its poles and zeroes (as input to this routine).
 *    The output consists of the parameters for second order sections.
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
 *     Number of second-order sections.
 * @param sn 
 *     Output Numerator polynomials for second order sections.
 * @param sd 
 *     Output Denominator polynomials for second order sections.
 *
 * \bug This routine defines PI and TWO PI, which are available in math.h
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
 * 
 */

void 
lpthp(complexf *p, 
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
			Sn[iptr] = 1.*scale;
			Sn[iptr + 1] = -2.*cmplxtof( Z[i] )*scale;
			Sn[iptr + 2] = cmplxtof( cmplxmul(Z[i],cmplxcj( Z[i] )) )*scale;
			Sd[iptr] = 1.;
			Sd[iptr + 1] = -2.*cmplxtof( P[i] );
			Sd[iptr + 2] = cmplxtof( cmplxmul(P[i],cmplxcj( P[i] )) );
			iptr = iptr + 3;

			}
		else if( memcmp(RTYPE(i_,0),"CP",2) == 0 ){

			scale = cmplxtof( cmplxmul(P[i],cmplxcj( P[i] )) );
			Sn[iptr] = 0.;
			Sn[iptr + 1] = 0.;
			Sn[iptr + 2] = scale;
			Sd[iptr] = 1.;
			Sd[iptr + 1] = -2.*cmplxtof( P[i] );
			Sd[iptr + 2] = cmplxtof( cmplxmul(P[i],cmplxcj( P[i] )) );
			iptr = iptr + 3;

			}
		else if( memcmp(RTYPE(i_,0),"SP",2) == 0 ){

			scale = -cmplxtof( P[i] );
			Sn[iptr] = 0.;
			Sn[iptr + 1] = scale;
			Sn[iptr + 2] = 0.;
			Sd[iptr] = 1.;
			Sd[iptr + 1] = -cmplxtof( P[i] );
			Sd[iptr + 2] = 0.;
			iptr = iptr + 3;

			}

		}

	Sn[1] = Sn[1]*dcvalue;
	Sn[2] = Sn[2]*dcvalue;
	Sn[3] = Sn[3]*dcvalue;

	return;
#undef	RTYPE
}

