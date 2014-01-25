/** 
 * @file   c1roots.c
 * 
 * @brief  Compute Chebyshev Type 1 Poles for Normalized 
 *         Low Pass (LP) Filter
 */

#include <math.h>

#include "complex.h"
#include "dbh.h"
#include "co.h"

/** 
 * Compute Chebyshev Type I Poles for a Normalized Low Pass (LP) Filter
 * 
 * @param p 
 *    Complex Array containing Poles. Contains only one of from each
 *     - Complex Conjugate Pole-Zero Pair
 *     - Complex Conjugate Pole Pair
 *     - Single Real Pole
 * @param rtype 
 *    Character Array indicating 2nd Order Section Types
 *     - 'CPZ' Complex Conjugate Pole-Zero Pair
 *     - 'CP'  Complex Conjugate Pole Pair
 *     - 'SP'  Single Real Pole
 * @param rtype_s 
 *     Length of string \p rtype
 * @param dcvalue 
 *     Magnitude of the filter at Zero Frequency
 * @param nsects 
 *     Number of 2nd Order Sections
 * @param iord 
 *     Desired Filter Order, Must be between 1 and 8
 * @param eps
 *     Output Chebyshev Parameter Related to Passband Ripple
 *
 * @return Nothing
 *
 * @copyright 1990  Regents of the University of California
 *
 * @author  Dave Harris                                                         
 *          Lawrence Livermore National Laboratory                              
 *          L-205                                                               
 *          P.O. Box 808                                                        
 *          Livermore, CA  94550                                                
 *          USA                                                                 
 *          (415) 423-0617                                                      
 *
 * \todo Documentation for chebyshev Type I Filter.  
 *          Find roots and multiply them together.
 *
 *
 * \date 900907    LAST MODIFIED
 *
 */
void 
c1roots(complexf  *p, 
        char      *rtype, 
        int        rtype_s, 
        float     *dcvalue, 
        int       *nsects, 
        int        iord, 
        double     eps)
{
#define RTYPE(I_,J_)	(rtype+(I_)*(rtype_s)+(J_))
	int half, i, i_;
	float angle, c, gamma, omega, pi, s, sigma;

	complexf *const P = &p[0] - 1;

	pi = 3.14159265;
	half = iord/2;

	/*  INTERMEDIATE DESIGN PARAMETERS                                               
	 * */
	gamma = (1. + sqrt( 1. + eps*eps ))/eps;
	gamma = log( gamma )/(float)( iord );
	gamma = exp( gamma );
	s = .5*(gamma - 1./gamma);
	c = .5*(gamma + 1./gamma);

	/*  CALCULATE POLES                                                              
	 * */
	*nsects = 0;
	for( i = 1; i <= half; i++ ){
		i_ = i - 1;
		fstrncpy( RTYPE(i_,0) , rtype_s - 1 , "CP", 2 );
		angle = (float)( 2*i - 1 )*pi/(float)( 2*iord );
		sigma = -s*sin( angle );
		omega = c*cos( angle );
		P[i] = flttocmplx( sigma, omega );
		*nsects = *nsects + 1;
		}
	if( 2*half < iord ){
		fstrncpy( RTYPE(half,0) , rtype_s - 1 , "SP", 2 );
		P[half + 1] = flttocmplx( -s, 0.0 );
		*nsects = *nsects + 1;
		*dcvalue = 1.0;
		}
	else{
		*dcvalue = 1./sqrt( 1 + powi(eps,2) );
		}

	/*  DONE                                                                         
	 * */
	return;
#undef	RTYPE
}

