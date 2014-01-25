/** 
 * @file   chebparm.c
 * 
 * @brief  Calculate Chebyshev Type I and II design parameters
 */

#include <math.h>

#include "dbh.h"
#include "co.h"

/** 
 * Calculate Chebyshev Type I and II Design Parameters
 * 
 * @param a 
 *    Desired Stopband Attenuation, i.e. max stopband 
 *    amplitude is 1/ATTEN
 * @param trbndw 
 *    Transition bandwidth between stop and passband as a 
 *    fraction of the passband width
 * @param iord 
 *    Filter Order (number of Poles)
 * @param eps 
 *    Output Chebyshev passband parameter
 * @param ripple 
 *    Passband ripple
 *
 * @return Nothing
 *
 * \note
 *   \f[
 *     \begin{array}{rcl}
 *  \omega &=& 1.0 + trbndw \\
 *  \alpha &=& (\omega + \sqrt{\omega^2 - 1.0} )^{iord}\\
 *       g &=& \alpha^2 + 1 / 2\alpha \\
 *     eps &=& \sqrt{a^2 - 1.0 } / g \\
 *  ripple &=& 1 / \sqrt{ 1.0 + eps^2 } \\
 *     \end{array}
 *   \f]
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
 */
void 
chebparm(double    a, 
         double    trbndw, 
         int       iord, 
         float    *eps, 
         float    *ripple)
{
	float alpha, g, omegar;

	omegar = 1. + trbndw;
	alpha = powi(omegar + sqrt( powi(omegar,2) - 1. ),iord);
	g = (powi(alpha,2) + 1.)/(2.*alpha);
	*eps = sqrt( powi(a,2) - 1. )/g;
	*ripple = 1./sqrt( 1. + powi(*eps,2) );

	return;
}

