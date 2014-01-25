
#include <math.h>

#include "sam.h"

#include "co.h"
#include "ucf.h"

#define	L	(ipow(2,12))
#define	PI	M_PI
#define	TWOPI	(2.*PI)

struct t_cmunwr {
  float thlinc, thlcon;
  int nfft;
  float con1, dvtmn2;
} cmunwr;
void /*FUNCTION*/ unwrap(x, nx, nt, thrcon, thrinc, aux1, aux2, aux3, 
	 am, ph, nok, lok)
float x[];
int nx, nt;
double thrcon, thrinc;
float aux1[], aux2[], aux3[], am[], ph[];
int *nok;
int *lok;
{
	int j1, j2, n;
	float pdvt, phase, ppdvt, pphase, ppv;
	double d1, d2, d3, d4, d5;

	float *const Am = &am[0] - 1;
	float *const Aux1 = &aux1[0] - 1;
	float *const Aux2 = &aux2[0] - 1;
	float *const Aux3 = &aux3[0] - 1;
	float *const Ph = &ph[0] - 1;

	/*=====================================================================
	 * PURPOSE: To compute amplitude and unwrapped phase of a sequence.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    X:       Array containing the sequence X(N).
	 *    NX:      Length of X.
	 *    NT:      Desired length of transformed signal.
	 *    THRCON:  Phase consistency threshold (radians).
	 *    THRINC:  Phase increment threshold (radians).
	 *    AUX1:    First scratch array. 
	 *    AUX2:    Second scratch array.
	 *    AUX3:    Third scratch array.
	 *             Each scratch array must be at least NT words long.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    AM:      Array containing amplitudes.
	 *    PH:      Array containing unwrapped phase.
	 *             AM and PH must be of length NT and may overlap X.
	 *    NOK:     Length of AM and PH output arrays if successful.
	 *             Index to last unwrapped phase if an error occurs.
	 *    LOK:     .TRUE. if unwrap was successful.
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/4
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920601:  Bugfix, cleaning up lrev mess. Moved pphase 
	 *             assignment after ppv = atan2(etc.) wct/hjp
	 *    920528:  Removed logical variable lrev from parameter list
	 *             in call to estpha, and removed tests of lrev in this
	 *             routine that were used to negate amplitude and phase
	 *             for signals that have dc-offset < 0.  Howard Patton
	 *             approved the changes. wct.
	 *    870727:  Scratch arrays now passed in by calling program rather
	 *             than being local arrays of fixed size.
	 *    810806:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Initialization. */
	cmunwr.nfft = nt;
	n = cmunwr.nfft/2 + 1;
	cmunwr.con1 = PI/((float)( L )*(float)( cmunwr.nfft ));
	cmunwr.thlcon = thrcon;
	cmunwr.thlinc = thrinc;

	/* - Save input data in AUX3 array. */

	/* copy( (int*)x, (int*)aux3, nx ); */
    copy_float(x, aux3, nx);

	/* - Transform N*X(N). Store in auxiliary arrays. */

	for( j1 = 1; j1 <= nx; j1++ ){
	    Aux1[j1] = (float)( j1 - 1 )*Aux3[j1];
	}
	fill( &Aux1[nx + 1], cmunwr.nfft - nx, 0. );
	fill( aux2, cmunwr.nfft, 0. );
	cpft( aux1, aux2, cmunwr.nfft, 1, -1 );

	/* Transform X(N).  Store in AM and PH arrays. */

	/* copy( (int*)aux3, (int*)am, nx ); */
    copy_float(aux3, am, nx);
	fill( &Am[nx + 1], cmunwr.nfft - nx, 0. );
	fill( ph, cmunwr.nfft, 0. );
	cpft( am, ph, cmunwr.nfft, 1, -1 );

	/* - Compute the spectral magnitude.  Store in AUX1.
	 * - Compute the spectral phase derivative.  Store in AUX2.
	 * - Compute the linear phase estimate (mean of phase derivative.)
	 *   Store twice the estimate in DVTMN2. */

	cmunwr.dvtmn2 = 0.;
	for( j1 = 1; j1 <= n; j1++ ){
	    d1 = (double)( Am[j1] );
	    d2 = (double)( Ph[j1] );
	    d3 = (double)( Aux1[j1] );
	    d4 = (double)( Aux2[j1] );
	    d5 = d1*d1 + d2*d2;
	    Aux1[j1] = (float)( d5 );
	    Aux2[j1] = -(float)( (d1*d3 + d2*d4)/d5 );
	    cmunwr.dvtmn2 = cmunwr.dvtmn2 + Aux2[j1];
	}
	cmunwr.dvtmn2 = 2.*(2.*cmunwr.dvtmn2 - Aux2[1] - Aux2[n])/(float)( cmunwr.nfft );

	/* - Compute logmagnitude.  Store in AM.
	 * - Compute unwrapped phase.  Store in PH. */

	ppdvt = Aux2[1];
	ppv = atan2( Ph[1], Am[1] );
	pphase = ppv;
	Am[1] = sqrt( Aux1[1] );
	Ph[1] = ppv;
	for( j1 = 2; j1 <= n; j1++ ){
	    pdvt = Aux2[j1];
	    ppv = atan2( Ph[j1], Am[j1] );
	    phase = estpha( aux3, nx, j1, &pphase, &ppdvt, ppv, pdvt, lok );
	    if( *lok ){
		ppdvt = pdvt;
		pphase = phase;
		Am[j1] = sqrt( Aux1[j1] );
		Ph[j1] = phase;
	    }
	    else{
		*nok = j1 - 1;
		for( j2 = j1; j2 <= n; j2++ ){
		    Am[j2] = sqrt( Aux1[j2] );
		    Ph[j2] = Aux2[j2];
		}
		goto L_8888;
	    }
	}

	*nok = n;

L_8888:
	return;

} /* end of function */

