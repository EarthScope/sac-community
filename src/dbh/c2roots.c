/** 
 * @file   c2roots.c
 * 
 * @brief  Compute roots for normalized lowpass (LP) Chebyshev II 
 *         Filter
 */

#include <math.h>

#include "complex.h"
#include "dbh.h"
#include "co.h"

/** 
 * Compute root for normailzed Low Pass Chebyshev Type II Filter
 * 
 * @param p 
 *    Complex Array containing Poles. Contains only one of from each
 *     - Complex Conjugate Pole-Zero Pair
 *     - Complex Conjugate Pole Pair
 *     - Single Real Pole
 * @param z 
 *    Complex Array containing Zeros Contains only one of from each
 *     - Complex Conjugate Pole-Zero Pair
 *     - Complex Conjugate Pole Pair
 *     - Single Real Pole
 * @param rtype 
 *    Character Array indicating 2nd Order Section Types
 *     - 'CPZ' Complex Conjugate Pole-Zero Pair
 *     - 'CP'  Complex Conjugate Pole Pair
 *     - 'SP'  Single Real Pole
 * @param rtype_s 
 *    Length of string \p rtype
 * @param dcvalue 
 *    Magnitude of filter at Zero Frequency
 * @param nsects 
 *    Number of 2nd order sections
 * @param iord 
 *   Input Desired Filter Order
 * @param a 
 *   Input Stopband attenuation factor
 * @param omegar 
 *   Input Cutoff frequency of stopband passband cutoff is 1.0 Hz
 *
 * @return Nothing
 *
 * \copyright Copyright 1990  Regents of the University of California
 *
 * \author   Dave Harris
 *           Lawrence Livermore National Laboratory
 *           L-205
 *           P.O. Box 808
 *           Livermore, CA  94550
 *           USA
 *           (415) 423-0617
 *
 * \date 900907:  LAST MODIFIED
 *
 */
void
c2roots(complexf  *p, 
        complexf  *z, 
        char      *rtype, 
        int        rtype_s, 
        float     *dcvalue, 
        int       *nsects, 
        int        iord, 
        double     a, 
        double     omegar)
{
#define RTYPE(I_,J_)	(rtype+(I_)*(rtype_s)+(J_))
	int half, i, i_;
	float alpha, angle, beta, c, denom, gamma, omega, pi, s, sigma;

	complexf *const P = &p[0] - 1;
	complexf *const Z = &z[0] - 1;

	pi = 3.14159265;
	half = iord/2;

	/*  INTERMEDIATE DESIGN PARAMETERS                                               
	 * */
	gamma = a + sqrt( a*a - 1. );
	gamma = log( gamma )/(float)( iord );
	gamma = exp( gamma );
	s = .5*(gamma - 1./gamma);
	c = .5*(gamma + 1./gamma);

	*nsects = 0;
	for( i = 1; i <= half; i++ ){
		i_ = i - 1;

		/*  CALCULATE POLES                                                              
		 * */
		fstrncpy( RTYPE(i_,0) , rtype_s - 1 , "CPZ", 2 );

		angle = (float)( 2*i - 1 )*pi/(float)( 2*iord );
		alpha = -s*sin( angle );
		beta = c*cos( angle );
		denom = alpha*alpha + beta*beta;
		sigma = omegar*alpha/denom;
		omega = -omegar*beta/denom;
		P[i] = flttocmplx( sigma, omega );

		/*  CALCULATE ZEROS                                                              
		 * */
		omega = omegar/cos( angle );
		Z[i] = flttocmplx( 0.0, omega );

		*nsects = *nsects + 1;

		}

	/*  ODD-ORDER FILTERS                                                            
	 * */
	if( 2*half < iord ){
		fstrncpy( RTYPE(half,0) , rtype_s - 1 , "SP", 2 );
		P[half + 1] = flttocmplx( -omegar/s, 0.0 );
		*nsects = *nsects + 1;
		}

	/*  DC VALUE                                                                     
	 * */
	*dcvalue = 1.0;

	/*  DONE                                                                         
	 * */
	return;
#undef	RTYPE
}

