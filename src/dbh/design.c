/** 
 * @file   design.c
 * 
 * @brief  Design IIR Digital Filter form Analog Prototypes
 *
 */
#include <stdio.h>
#include <string.h>

#include "complex.h"
#include "dbh.h"


/** 
 * Design IIR Digital Filters from Analog Prototypes
 * 
 * @param iord 
 *    Filter Order, Maximum of 10
 * @param type 
 *    Filter Type, Character *2
 *      - 'LP'  Lowpass
 *      - 'HP'  Highpass
 *      - 'BP'  Bandpass
 *      - 'BR'  Bandreject
 * @param aproto 
 *    Analog Prototype, Character *2
 *      - 'BU'  Butterworth
 *      - 'BE'  Bessel
 *      - 'C1'  Cheyshev Type I
 *      - 'C2'  Cheyshev Type II
 * @param a 
 *    Chebyshev stopband Attenuation Factor
 * @param trbndw 
 *    Chebyshev transition bandwidth, fraction of lowpass prototype 
 *    passband width
 * @param fl 
 *    Low Frequency cutoff
 * @param fh 
 *    High Frequency cutoff
 * @param ts 
 *    Sampling Rate / Delta
 * @param sn 
 *    Array containing numerator coefficients of 2nd Order Sections
 *    Packed Head to Tail
 * @param sd 
 *    Array containing denominator coefficients of 2nd Order Sections
 *    Packed Head to Tail
 * @param nsects 
 *    Length of arrays \p sn and \p sd
 *
 *  @copyright 1990  Regents of the University of California                      
 *
 *  @author  Dave Harris
 *           Lawrence Livermore National Laboratory
 *           L-205
 *           P.O. Box 808
 *           Livermore, CA  94550
 *           USA
 * 
 *  @date Documented/Reviewed
 *
 */
void 
design(int        iord, 
       char      *type, 
       char      *aproto, 
       double     a, 
       double     trbndw, 
       double     fl, 
       double     fh, 
       double     ts, 
       float     *sn, 
       float     *sd, 
       int       *nsects)
{
	char stype[10][4];
	float dcvalue, eps, fhw, flw, omegar, ripple;
	complexf p[10], z[10];

	/*  Analog prototype selection                                                   
	 * */
	if( memcmp(aproto,"BU",2) == 0 ){

		buroots( p, (char*)stype,4, &dcvalue, nsects, iord );

		}
	else if( memcmp(aproto,"BE",2) == 0 ){

		beroots( p, (char*)stype,4, &dcvalue, nsects, iord );

		}
	else if( memcmp(aproto,"C1",2) == 0 ){

		chebparm( a, trbndw, iord, &eps, &ripple );
		c1roots( p, (char*)stype,4, &dcvalue, nsects, iord, eps );

		}
	else if( memcmp(aproto,"C2",2) == 0 ){

		omegar = 1. + trbndw;
		c2roots( p, z, (char*)stype,4, &dcvalue, nsects, iord, a, 
		 omegar );

    } else {
        fprintf(stderr, "filter: Unknown Analog filter prototype: '%s'\n", aproto);
        fprintf(stderr, "        Expected: BU, BESSEL, C1, C2\n");
        return;        
    }

	/*  Analog mapping selection                                                     
	 * */
	if( memcmp(type,"BP",2) == 0 ){

		flw = warp( fl*ts/2., 2. );
		fhw = warp( fh*ts/2., 2. );
		lptbp( p, z, (char*)stype,4, dcvalue, nsects, flw, fhw, sn, 
		 sd );

		}
	else if( memcmp(type,"BR",2) == 0 ){

		flw = warp( fl*ts/2., 2. );
		fhw = warp( fh*ts/2., 2. );
		lptbr( p, z, (char*)stype,4, dcvalue, nsects, flw, fhw, sn, 
		 sd );

		}
	else if( memcmp(type,"LP",2) == 0 ){

		fhw = warp( fh*ts/2., 2. );
		lp( p, z, (char*)stype,4, dcvalue, *nsects, sn, sd );
		cutoffs( sn, sd, *nsects, fhw );

		}
	else if( memcmp(type,"HP",2) == 0 ){

		flw = warp( fl*ts/2., 2. );
		lpthp( p, z, (char*)stype,4, dcvalue, *nsects, sn, sd );
		cutoffs( sn, sd, *nsects, flw );

		}

	/*  Bilinear analog to digital transformation                                    
	 * */
	bilin2( sn, sd, *nsects );

	return;
}







