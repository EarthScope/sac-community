/** 
 * @file   afr.c
 * 
 * @brief  Calculate the frequency response of an analog filter.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "complex.h"
#include "dbh.h"
#include "bool.h"

/** 
 *  Calculates the frequency response of an analog filter:  amplitude, 
 *    phase and group delay. Assumes filter is stored as second order 
 *    sections.
 * 
 * @param sn 
 *    Input Array containing numerator polynomial coefficients for 
 *      second order sections.  Packed head-to-tail, Groups of 3
 * @param sd 
 *    Input Array containing denominator polynomial coefficients for  
 *      second order sections.  Packed head-to-tail, Groups of 3
 * @param nsects 
 *    Number of second order sections. Length of arrays \p sn and \p sd
 * @param type 
 *    Type of response desired, (Character, Length 2)
 *     - 'AM' amplitude                                          
 *     - 'PH' phase                                              
 *     - 'GD' group delay                                        
 * @param sampling 
 *    Frequency Domain Sampling (Character, Length 8)
 *     - 'LINEAR'                                                
 *     - 'LOG'                                                    
 * @param fl 
 *    Low Frequency in range, may not be zero for 'LOG' Sampling
 * @param fh 
 *    High Frequency in range, \p fh > \p fl
 * @param nsamps 
 *    Number of Frequency Samples Desired
 * @param response 
 *    Output Array containing desired freqency response samples
 * @param freqs 
 *    Output Array containing frequency sample points
 *
 * @return Nothing
 *
 * @author Dave B. Harris
 *           Lawrence Livermore National Laboratory
 *           L-205
 *           P.O. Box 808
 *           Livermore, CA  94550
 *           USA
 *           (415) 423-0617
 *
 * @copyright 1990  Regents of the University of California 
 *
 *
 * @note 
 *    H(s) = B(s) / A(s) = (b_0 + b_1 * s + b_2 *s^2) / (a_0 + a_1 * s + a_2 *s^2)
 *    where b = \p sn and
 *          a = \p sd
 *          s = 0 + i * 2 pi f_i
 * @see inspect
 *
 * @date 071022 Documented/Reviewed
 *
 */
void 
afr(float    *sn, 
    float    *sd, 
    int       nsects, 
    char     *type, 
    char     *sampling, 
    double    fl, 
    double    fh, 
    int       nsamps, 
    float    *response, 
    float    *freqs)
{
	int logspacing;
	int i, iptr, j;
	float a0, a1, a2, afh, afl, delf, ft, gd, phase, twopi;
	complexf denominator, dent, h, numerator, numt, pd, pdt, s;

	float *const Freqs = &freqs[0] - 1;
	float *const Response = &response[0] - 1;
	float *const Sd = &sd[0] - 1;
	float *const Sn = &sn[0] - 1;

	/*  Frequency sampling initialization                                            
	 * */
	twopi = 2.*3.14159265;
	if( memcmp(sampling,"LOG",3) == 0 ){
		logspacing = TRUE;
		afl = log( fl );
		afh = log( fh );
		}
	else if( memcmp(sampling,"LIN",3) == 0 ){
		logspacing = FALSE;
		afl = fl;
		afh = fh;
    } else {
        fprintf(stderr, "filter: Unknown Sampling in Analog Frequency Response\n");
        fprintf(stderr, "      : Expected LOG or LON\n");
        return;
    }
	delf = (afh - afl)/(float)( nsamps - 1 );

	ft = afl;

	/*  Loop over frequency                                                          
	 * */
	for( j = 1; j <= nsamps; j++ ){

		if( logspacing ){
			Freqs[j] = exp( ft );
			}
		else{
			Freqs[j] = ft;
			}
		s = flttocmplx( 0., twopi*Freqs[j] );
		h = flttocmplx( 1., 0. );
		gd = 0.0;

		iptr = 1;
		for( i = 1; i <= nsects; i++ ){

			a0 = Sn[iptr];
			a1 = Sn[iptr + 1];
			a2 = Sn[iptr + 2];
			numerator = cmplxadd(cmplxmul((cmplxadd(cmplxmul(flttocmplx(a2,0.),s),flttocmplx(a1,0.))),
			 s),flttocmplx(a0,0.));
			pd = cmplxadd(cmplxmul(flttocmplx(2*a2,0.),s),flttocmplx(a1,0.));
			if( fabs( cmplxtof( cmplxmul(numerator,cmplxcj( numerator )) ) ) == 
			 0.0 ){
				numt = pd;
				pdt = flttocmplx(2*a2,0.);
				gd = gd - cmplxtof( cmplxdiv(pdt,numt) );
				}
			else{
				gd = gd - cmplxtof( cmplxdiv(pd,numerator) );
				}

			a0 = Sd[iptr];
			a1 = Sd[iptr + 1];
			a2 = Sd[iptr + 2];
			denominator = cmplxadd(cmplxmul((cmplxadd(cmplxmul(flttocmplx(a2,0.),s),
			 flttocmplx(a1,0.))),s),flttocmplx(a0,0.));
			pd = cmplxadd(cmplxmul(flttocmplx(2*a2,0.),s),flttocmplx(a1,0.));
			if( fabs( cmplxtof( cmplxmul(denominator,cmplxcj( denominator )) ) ) == 
			 0.0 ){
				dent = pd;
				pdt = flttocmplx(2*a2,0.);
				gd = gd + cmplxtof( cmplxdiv(pdt,dent) );
				}
			else{
				gd = gd + cmplxtof( cmplxdiv(pd,denominator) );
				}

			h = cmplxdiv(cmplxmul(h,numerator),denominator);

			iptr = iptr + 3;

			}

		if( memcmp(type,"AM",2) == 0 ){
			Response[j] = sqrt( cmplxtof( cmplxmul(cmplxcj( h ),h) ) );
			}
		else if( memcmp(type,"PH",2) == 0 ){
			phase = atan2( aimag( h ), cmplxtof( h ) );
			Response[j] = phase*360./twopi;
			}
		else if( memcmp(type,"GD",2) == 0 ){
			Response[j] = gd;
			}

		ft = ft + delf;

		}

	return;
}

