
#include <string.h>

#include "icm.h"
#include "complex.h"

#include "msg.h"

void /*FUNCTION*/ sro(nfreq, delfrq, xre, xim, subtyp, subtyp_s)
int nfreq;
double delfrq, xre[], xim[];
char *subtyp;   int subtyp_s;
{
	int nerr, npole, nzero;
	float const_;
	complexf pole[17], zero[8];

	complexf *const Pole = &pole[0] - 1;
	complexf *const Zero = &zero[0] - 1;

	/*   .....SRO - for an SRO seismometer.....
	 * */


	/*   .....Set poles and zeros.....
	 *
	 *
	 * SRO SEISMOMETER.  THERE ARE THREE SYSTEMS THAT CAN BE COMPUTED
	 * HERE:  BROADBAND (BB), SHORT PERIOD (SP), OR LONG PERIOD (LP).
	 * REFERENCES ARE:
	 * MCCOWAN, D. W. AND R. T. LACOSS (1978).  TRANSFER FUNCTION
	 * FOR THE SEISMIC RESEARCH OBSERVATORY SEISMOGRAPH SYSTEM, BULL. SEIS.
	 * SOC. AM., 68, 501-512.
	 * BERGER, J., D. W. MCCOWAN, W. E. FARRELL, AND R. T. LACOSS (1978).
	 * COMMENTS ON 'TRANSFER FUNCTIONS FOR THE SEISMIC RESEARCH OBSERVATORY
	 * SEISMOGRAPH SYSTEM' BY DOUGLAS W. MCCOWAN AND RICHARD T. LACOSS, BULL.
	 * SEIS. SOC. AM., 68, 1537-1538.
	 *
	 *
	 *  .....SRO BROADBAND     'BB'.....
	 * */
	if( memcmp(subtyp,"BB",2) == 0 ){
		const_ = -394.0;

		nzero = 4;
		Zero[1] = flttocmplx( -0.125, 0.0 );
		Zero[2] = flttocmplx( -50.0, 0.0 );
		Zero[3] = flttocmplx( 0.0, 0.0 );
		Zero[4] = flttocmplx( 0.0, 0.0 );

		npole = 4;
		Pole[1] = flttocmplx( -0.13, 0.0 );
		Pole[2] = flttocmplx( -6.02, 0.0 );
		Pole[3] = flttocmplx( -8.66, 0.0 );
		Pole[4] = flttocmplx( -35.2, 0.0 );

		/*   .....SRO SHORT PERIOD.      'SP'.....
		 * INCLUDES SEISMOMETER AND SHAPING FILTER TRANSFER FUNCTIONS.
		 * */
		}
	else if( memcmp(subtyp,"SP",2) == 0 ){
		const_ = 5.08233208e13;

		nzero = 5;
		Zero[1] = flttocmplx( -50.0, 0.0 );
		Zero[2] = flttocmplx( 0.0, 0.0 );
		Zero[3] = flttocmplx( 0.0, 0.0 );
		Zero[4] = flttocmplx( 0.0, 0.0 );
		Zero[5] = flttocmplx( 0.0, 0.0 );

		npole = 9;
		Pole[1] = flttocmplx( -0.13, 0.0 );
		Pole[2] = flttocmplx( -6.02, 0.0 );
		Pole[3] = flttocmplx( -8.66, 0.0 );
		Pole[4] = flttocmplx( -35.2, 0.0 );
		Pole[5] = flttocmplx( -100.0, 0.0 );
		Pole[6] = flttocmplx( -17.97, 0.0 );
		Pole[7] = flttocmplx( -17.97, 0.0 );
		Pole[8] = flttocmplx( -63.29, 0.0 );
		Pole[9] = flttocmplx( -63.29, 0.0 );

		/*   .....SRO LONG PERIOD         'LP'.....
		 * INCLUDES SEISMOMETER, SHAPING FILTERS, AND ANTI-ALIASING FILTERS.
		 * */
		}
	else if( memcmp(subtyp,"LPDE",4) == 0 ){
		const_ = 16892.61226;

		nzero = 8;
		Zero[1] = flttocmplx( -50.0, 0.0 );
		Zero[2] = flttocmplx( 0.0, 0.0 );
		Zero[3] = flttocmplx( 0.0, 0.0 );
		Zero[4] = flttocmplx( 0.0, 0.0 );
		Zero[5] = flttocmplx( 0.0, 1.05 );
		Zero[6] = flttocmplx( 0.0, -1.05 );
		Zero[7] = flttocmplx( 0.0, 0.0 );
		Zero[8] = flttocmplx( 0.0, 0.0 );

		npole = 17;
		Pole[1] = flttocmplx( -0.13, 0.0 );
		Pole[2] = flttocmplx( -6.02, 0.0 );
		Pole[3] = flttocmplx( -8.66, 0.0 );
		Pole[4] = flttocmplx( -35.2, 0.0 );
		Pole[5] = flttocmplx( -100.0, 0.0 );
		Pole[6] = flttocmplx( -3.93, 0.0 );
		Pole[7] = flttocmplx( -0.282, 0.0 );
		Pole[8] = flttocmplx( -0.201, 0.241 );
		Pole[9] = flttocmplx( -0.201, -0.241 );
		Pole[10] = flttocmplx( -0.134, 0.1 );
		Pole[11] = flttocmplx( -0.134, -0.1 );
		Pole[12] = flttocmplx( -0.0251, 0.0 );
		Pole[13] = flttocmplx( -0.00942, 0.0 );
		Pole[14] = flttocmplx( -0.24, 0.58 );
		Pole[15] = flttocmplx( -0.58, 0.24 );
		Pole[16] = flttocmplx( -0.58, -0.24 );
		Pole[17] = flttocmplx( -0.24, -0.58 );

		}
	else{
		nerr = 2105;
		setmsg( "ERROR", nerr );
		apcmsg( "SRO:",5 );
		apcmsg( subtyp,subtyp_s );
		goto L_8888;
		}

	/*   .....Compute transfer function.....
	 * */
	getran( nfreq, delfrq, const_, nzero, zero, npole, pole, xre, 
	 xim );

L_8888:
	return;
} /* end of function */

