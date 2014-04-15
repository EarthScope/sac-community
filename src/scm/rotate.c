
#include <math.h>

#include "mach.h"
#include "scm.h"

void rotate(float si1[], float si2[], int ns, double angle, int lnpi, int lnpo, float so1[], float so2[])
{
	int js;
	float con11, con12, con21, con22, cosa, sina, tsi1, tsi2;

	float *const Si1 = &si1[0] - 1;
	float *const Si2 = &si2[0] - 1;
	float *const So1 = &so1[0] - 1;
	float *const So2 = &so2[0] - 1;


	/*=====================================================================
	 * PURPOSE: To perform a clockwise rotation of a pair of signals.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    SI1:     First input signal.
	 *    SI2:     Second input signal.
	 *    NS:      Number of points in input signals.
	 *    ANGLE:   Angle of rotation, clockwise from direction of S1.
	 *    LNPI:    .TRUE. if the input signals have "normal" polarity.
	 *    LNPO:    .TRUE. if the output signals are to have "normal" polarity.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    SO1:     First output signal.
	 *    SO2:     Second output signal.
	 *             SO1 and SO2 may be the same arrays as SI1 and SI2.
	 *=====================================================================
	 * MODULE/LEVEL:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * ASSUMPTIONS:
	 * - "Normal" polarity is such that the second component leads
	 *   the first component by 90 degrees in a clockwise rotation.
	 *===================================================================== */
	/* PROCEDURE: */
	cosa = cos( TORAD*angle );
	sina = sin( TORAD*angle );
	con11 = cosa;
	con12 = sina;
	con21 = -sina;
	con22 = cosa;
	if( !lnpi ){
		con12 = -con12;
		con22 = -con22;
		}
	if( !lnpo ){
		con21 = -con21;
		con22 = -con22;
		}

	for( js = 1; js <= ns; js++ ){
		tsi1 = Si1[js];
		tsi2 = Si2[js];
		So1[js] = con11*tsi1 + con12*tsi2;
		So2[js] = con21*tsi1 + con22*tsi2;
		}

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    810325:  Original version.
	 *    810527:  Modifications for normal polarity flags.
	 *===================================================================== */

} /* end of function */

void rotate_(float si1[], float si2[], int *ns, double *angle, int *lnpi, int *lnpo, float so1[], float so2[]) {
  rotate(si1, si2, *ns, *angle, *lnpi, *lnpo, so1, so2);
}
void rotate__(float si1[], float si2[], int *ns, double *angle, int *lnpi, int *lnpo, float so1[], float so2[]) {
  rotate(si1, si2, *ns, *angle, *lnpi, *lnpo, so1, so2);
}
