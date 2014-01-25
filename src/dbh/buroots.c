/** 
 * @file   buroots.c
 * 
 * @brief  Compute Butterworth Poles for a Noramlized Low Pass (LP) Filter
 */

#include <math.h>

#include "complex.h"
#include "dbh.h"
#include "co.h"

/** 
 * Compute the Butterworth Poles for a Normalized Low Pass (LP) Filter
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
 *
 * @return Nothing
 *
 * @copyright 1990  Regents of the University of California
 *
 *
 * @author  Dave Harris                                                         
 *          Lawrence Livermore National Laboratory                              
 *          L-205                                                               
 *          P.O. Box 808                                                        
 *          Livermore, CA  94550                                                
 *          USA                                                                 
 *          (415) 423-0617                                                      
 *
 * \note
 *   \f[
 *     \begin{array}{rcl}
 *        n &=& iord \\
 *     H(s) &=& k_0 / \displaystyle\prod_{k=1}^n  s - s_k \\
 *      s_k &=& exp ( i \pi [0.5 + 2(2 k - 1)/2n ] ); k = 1, 2, ..., n \\
 *      k_0 &=& 1.0
 *     \end{array}
 *   \f]
 *
 * \date 900907   LAST MODIFIED
 * 
 */
void 
buroots(complexf *p, 
        char     *rtype, 
        int       rtype_s, 
        float    *dcvalue, 
        int      *nsects, 
        int       iord)
{
#define RTYPE(I_,J_)	(rtype+(I_)*(rtype_s)+(J_))
	int half, k;
	float angle, pi;

	complexf *const P = &p[0] - 1;

	pi = 3.14159265;

	half = iord/2;

	/* TEST FOR ODD ORDER, AND ADD POLE AT -1                                        
	 * */
	*nsects = 0;
	if( 2*half < iord ){
		P[1] = flttocmplx( -1., 0. );
		fstrncpy( RTYPE(0,0) , rtype_s - 1 , "SP", 2 );
		*nsects = 1;
		}

	for( k = 1; k <= half; k++ ){
		angle = pi*(.5 + (float)( 2*k - 1 )/(float)( 2*iord ));
		*nsects = *nsects + 1;
		P[*nsects] = flttocmplx( cos( angle ), sin( angle ) );
		fstrncpy( RTYPE(*nsects - 1,0) , rtype_s - 1 , "CP", 2 );
		}

	*dcvalue = 1.0;

	return;
#undef	RTYPE
}






