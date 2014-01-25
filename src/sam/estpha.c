
#include <math.h>

#include "sam.h"
#include "bool.h"
#include "co.h"

#define	L	(ipow(2,12))
#define	PI	M_PI
#define	TWOPI	(2.*PI)


struct t_cmunwr {
  float thlinc, thlcon;
  int nfft;
  float con1, dvtmn2;
} cmunwr;

double /*FUNCTION*/ estpha(x, nx, i, pphase, ppdvt, ppv, pdvt, iscons)
float x[];
int nx, i;
float *pphase, *ppdvt;
double ppv, pdvt;
int *iscons;
{
	int k, pindex, sindex[17], sp;
	float delta, estpha_v, freq, phainc, phase, sdvt[17], sppv[17], 
	 xi, xr, yi, yr;
	double d1, d2, d3, d4, d5;

	float *const Sdvt = &sdvt[0] - 1;
	int *const Sindex = &sindex[0] - 1;
	float *const Sppv = &sppv[0] - 1;


	/*=====================================================================
	 * PURPOSE:
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/3
	 *=====================================================================
	 * GLOBAL INPUT:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *=====================================================================
	 * GLOBAL COUPLING:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * ASSUMPTIONS:
	 *=====================================================================
	 * LIMITATIONS:
	 *=====================================================================
	 * KNOWN ERRORS:
	 *=====================================================================
	 * EXTERNAL DEPENDENCIES:
	 *===================================================================== */
	/* PROCEDURE:
	 *
	 *-----------------------------------------------------------------------
	 * FUNCTION: ESTPHA
	 * PHASE UNWRAPPING BASED ON TRIBOLET'S ADAPTIVE INTEGRATION SCHEME.
	 * THE UNWRAPPED PHASE ESTIMATE IS RETURNED IN ESTPHA.
	 *-----------------------------------------------------------------------
	 *
	 *
	 * DESCRIPTION OF ARGUMENTS:
	 *
	 *      X = ARRAY CONTAINING SEQUENCE X(N)
	 *     NX = # POINTS IN SEQUENCE X(N)
	 *   LREV was the third parameter in the call, removed from unwrap, and
	 *   tests for lrev in this procedure were also removed. H. Patton and wct.
	 *   LREV = .TRUE. IF SIGN REVERSAL IS NECESSARY.
	 *      I = INDEX OF PHASE ESTIMATE ON EQUALLY SPACED FFT
	 *         FREQUENCY GRID
	 * PPHASE = PHASE ESTIMATE AT INDEX I-1
	 *  PPDVT = PHASE DERIVATIVE AT INDEX I-1
	 *    PPV = PHASE PRINCIPLE VALUE AT INDEX I
	 *   PDVT = PHASE DERIVATIVE AT INDEX I
	 * ISCONS = .FALSE. IF PHASE ESTIMATION UNSUCCESSFUL
	 *         (STACK DIMENSION EXCEEDED BEFORE CONSISTENT ESTIMATE
	 *         FOUND);.TRUE. OTHERWISE
	 *
	 *
	 * SUBROUTINES CALLED:
	 *
	 * SPCVAL = SUBROUTINE TO COMPUTE SPECTRAL VALUE
	 * CHKPHA = SUBROUTINE TO CHECK PHASE CONSISTENCY
	 *
	 * */

	/* DESCRIPTION OF ARRAYS AND VARIABLES:
	 *
	 * SINDEX = INDEX STACK
	 *   SDVT = PHASE DERIVATIVE STACK
	 *   SPPV = PHASE PRINCIPLE VALUE STACK
	 *     SP = STACK POINTER
	 *
	 *
	 *
	 *
	 *
	 * INITIALIZATION
	 * */
	pindex = 1;
	sp = 1;
	Sppv[sp] = ppv;
	Sdvt[sp] = pdvt;
	Sindex[sp] = L + 1;

	/* ENTER MAJOR LOOP
	 * */
	goto L_40;

	/* IF SOFTWARE STACK DIMENSION DOES NOT ALLOW FURTHER STEP
	 * REDUCTION, RETURN.
	 * */
L_20:
	if( (Sindex[sp] - pindex) <= 1 ) {
	    *iscons = FALSE;
	    estpha_v = 0.;
	    return( estpha_v );
	}

	/* DEFINE INTERMEDIATE FREQUENCY(I.F.):
	 * W = (TWOPI/NFFT)*(I-2+(K-1)/L)
	 * */
	k = (Sindex[sp] + pindex)/2;

	/* CALCULATE I.F.
	 * */
	freq = 2.*cmunwr.con1*((float)( i - 2 )*(float)( L ) + (float)( k - 1 ));
	spcval( nx, x, freq, &xr, &xi, &yr, &yi );

	/* COMPUTE PHASE DERIVATIVE AND PRINCIPLE VALUE OF THE PHASE
	 * AT I.F.;UPDATE STACK
	 * */
	sp = sp + 1;

        if(sp > 17 ) {
	  *iscons = FALSE;
	  estpha_v = 0.;
	  return( estpha_v );
        }

	Sindex[sp] = k;
	Sppv[sp] = atan2( xi, xr );
	d1 = (double)( xr );
	d2 = (double)( xi );
	d3 = (double)( yr );
	d4 = (double)( yi );
	d5 = d1*d1 + d2*d2;
	/* xmag = (float)( d5 ); */
	Sdvt[sp] = -(float)( (d1*d3 + d2*d4)/d5 );

	/* EVALUATE THE PHASE INCREMENT ACROSS SPECTRAL INTERVAL
	 * */
L_40:
	delta = cmunwr.con1*(float)( Sindex[sp] - pindex );
	phainc = delta*(*ppdvt + Sdvt[sp]);

	/* IF PHASE INCREMENT,REDUCED BY EXPECTED LINEAR PHASE INCREMENT,
	 * IS GREATER THAN SPECIFIED THRESHOLD,ADAPT STEP SIZE
	 * */
	if( fabs( phainc - delta*cmunwr.dvtmn2 ) > cmunwr.thlinc )
		goto L_20;

	/* FORM PHASE ESTIMATE;CHECK CONSISTENCY
	 * */
	phase = *pphase + phainc;
	chkpha( &phase, Sppv[sp], iscons );
	if( !*iscons )
		goto L_20;

	/* IF RESULTING PHASE INCREMENT IS GREATER THAN PI,ADAPT STEP SIZE
	 * FOR MORE CONFIDENT ESTIMATE;OTHERWISE UPDATE PREVIOUS ESTIMATE
	 * IF STACK IS NOT EMPTY
	 * */
	if( fabs( phase - *pphase ) > PI )
		goto L_20;

	/* WHEN STACK IS EMPTY,THE UNWRAPPED PHASE AT
	 * W = TWOPI*(I-1)/NFFT IS HELD IN PHASE
	 * */
	if( sp != 1 ) {
            /* Update previous estimate */
            pindex = Sindex[sp];
            *pphase = phase;
            *ppdvt = Sdvt[sp];
            sp = sp - 1;

            goto L_40;
	}



	estpha_v = phase;
	return( estpha_v );
	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    920528:  Removed lrev tests and negation of phase and amplitude
	 *             for files with negative dc off-set. H. Patton & wct.
	 *    810000:  Original version.
	 *===================================================================== */

} /* end of function */

