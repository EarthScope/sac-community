/** 
 * @file   inspect.c
 * 
 * @brief  Calculate the frequency response IIR filters, digital and analog.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "complex.h"
#include "dbh.h"
#include "bool.h"

/** 
 * Calculate the frequency response of IIR analog and digital
 *   filters.   Choice of analog frequency interval for response
 *   calculation is up to the user.  For digital filters, the interval
 *   is fixed: 0 to the folding frequency.
 * 
 * @param iord 
 *    Filter order, Maximum of 10
 * @param type 
 *    Filter Type, Character *2
 *    - 'LP'  Lowpass
 *    - 'HP'  Highpass
 *    - 'BP'  Bandpass
 *    - 'BR'  Bandreject
 * @param aproto 
 *    Anaalog Prototype
 *    - 'BU'  Butterworth
 *    - 'BE'  Bessel
 *    - 'C1'  Chebyshev Type I
 *    - 'C2'  Chebyshev Type II
 * @param att 
 *    Chebyshev II stopband attenuation factor    
 * @param trbndw 
 *    Transition Bandwidth (fraction of passband width)
 * @param fl 
 *    Low-Frequency cutoff
 * @param fh 
 *    High-Frequency cutoff
 * @param ts 
 *    Sampling Interval
 * @param rtype 
 *    Response Type
 *    - 'DAM'  Digital Amplitude
 *    - 'DPH'  Digital Phase
 *    - 'DGD'  Digital Group Delay
 *    - 'AM'   Analog Amplitude
 *    - 'PH'   Analog Phase
 *    - 'GD'   Analog Group Delay0
 * @param nfreqs 
 *    Number of Frequency samples
 * @param rfl 
 *    Input  Low Sampling Frequency, Input if Response is Analog
 *    Output Low Sampling Frequency, Input if Response is Digital
 * @param rfh 
 *    Input  High Sampling Frequency, Input if Response is Analog
 *    Output High Sampling Frequency, Input if Response is Digital
 * @param sampling 
 *    Frequency domain Sampling, 
 *    - 'LINEAR'
 *    - 'LOG'
 * @param response 
 *    Output frequency response samples
 * @param freqs 
 *    Output sampling frequencies
 *
 *  @copyright  1990  Regents of the University of California                      
 *
 *
 *  @author  Dave Harris
 *           Lawrence Livermore National Laboratory
 *           L-205
 *           P.O. Box 808
 *           Livermore, CA  94550
 *           USA
 *           (415) 423-0617
 *
 */
void 
inspect(int       iord, 
	char     *type, 
	char     *aproto, 
	double    att, 
	double    trbndw, 
	double    fl, 
	double    fh, 
	double    ts, 
	char     *rtype, 
	int       nfreqs, 
	float    *rfl, 
	float    *rfh, 
	char     *sampling, 
	float    *response, 
	float    *freqs)
{
	char stype[10][4];
	int digital;
	int nsects;
	float dcvalue, eps, fhw, flw, omegar, ripple, sd[30], sn[30];
	complexf poles[10], zeros[10];
        char *strtemp;


	if( rtype[0] == 'D' )
	     digital = TRUE;
	else
	     digital = FALSE;

	/*  Analog prototype selection                                                   
	 * */
	if( memcmp(aproto,"BU",2) == 0 ){
	     buroots( poles, (char*)stype,4, &dcvalue, &nsects, iord );
	}
	else if( memcmp(aproto,"BE",2) == 0 ){
	     beroots( poles, (char*)stype,4, &dcvalue, &nsects, iord );
	}
	else if( memcmp(aproto,"C1",2) == 0 ){
	     chebparm( att, trbndw, iord, &eps, &ripple );
	     c1roots( poles, (char*)stype,4, &dcvalue, &nsects, iord, eps );
	}
	else if( memcmp(aproto,"C2",2) == 0 ){
	     omegar = 1. + trbndw;
	     c2roots( poles, zeros, (char*)stype,4, &dcvalue, &nsects, iord,
			att, omegar);
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

	    if( digital ){
		lptbp( poles, zeros, (char*)stype,4, dcvalue, &nsects,
			flw, fhw, sn,sd);
	    }
	    else{
		lptbp( poles, zeros, (char*)stype,4, dcvalue, &nsects,
			fl, fh, sn, sd );
	    }

	}
	else if( memcmp(type,"BR",2) == 0 ){

	    flw = warp( fl*ts/2., 2. );
	    fhw = warp( fh*ts/2., 2. );

	    if( digital ){
		lptbr( poles, zeros, (char*)stype,4, dcvalue, &nsects,
			flw, fhw, sn,sd);
	    }
	    else{
		lptbr( poles, zeros, (char*)stype,4, dcvalue, &nsects,
			fl, fh, sn, sd );
	    }

	}
	else if( memcmp(type,"LP",2) == 0 ){

	    if( digital ){
		fhw = warp( fh*ts/2., 2. );
		lp( poles, zeros, (char*)stype,4, dcvalue, nsects, sn, sd );
		cutoffs( sn, sd, nsects, fhw );
	    }
	    else{
		lp( poles, zeros, (char*)stype,4, dcvalue, nsects, sn, sd );
		cutoffs( sn, sd, nsects, fh );
	    }

	}
	else if( memcmp(type,"HP",2) == 0 ){

	    if( digital ){
		flw = warp( fl*ts/2., 2. );
		lpthp( poles, zeros, (char*)stype,4, dcvalue, nsects, sn, sd );
		cutoffs( sn, sd, nsects, flw );
	    }
	    else{
		lpthp( poles, zeros, (char*)stype,4, dcvalue, nsects, sn, sd );
		cutoffs( sn, sd, nsects, fl );
	    }

	}

	/*  Response calculation                                                         
	 * */
	if( digital ){
	    bilin2( sn, sd, nsects );

            strtemp = malloc(3);
            strncpy(strtemp,rtype+1,2);
            strtemp[2] = '\0';

	    dfr( sn, sd, nsects, strtemp, nfreqs, ts, response );

            free(strtemp);

	    *rfl = 0.0;
	    *rfh = 1/(2.*ts);
	}
	else{
            strtemp = malloc(3);
            strncpy(strtemp,rtype,2);
            strtemp[2] = '\0';

	    afr( sn, sd, nsects, strtemp, sampling, *rfl, *rfh, 
	     nfreqs, response, freqs );

            free(strtemp);
	}

	return;
}

