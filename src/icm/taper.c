#include <stdio.h>
#include <math.h>

#include "icm.h"

/* SUBROUTINE TO TAPER SPECTRA BY A COSINE
 *
 * CALLING ARGUMENTS:
 *
 *     FREQ - FREQUENCY IN QUESTION
 *     FQH - FREQUENCY AT WHICH THERE IS A TRANSITION BETWEEN UNITY AND
 *           THE TAPER
 *     FQL - FREQUENCY AT WHICH THERE IS A TRANSITION BETWEEN ZERO AND
 *           THE TAPER
 *     NOTE:  IF FQL>FQH THEN LO-PASS
 *            IF FQH>FQL THEN HI-PASS
 * */
double taper_spectra(double freq, double fqh, double fql)
{
	/* LO-PASS CASE */
	if (fql > fqh) {
		if (freq < fqh)
			return 1.0e0;
		
		if (freq >= fqh && freq <= fql)
			return 0.5e0 * (1.0e0 + cos(M_PI * (freq - fqh) / (fql - fqh)));
		
		if (freq > fql)
			return 0.0e0;
	}
	
	/* HI-PASS CASE */
	if (fqh > fql) {
		if (freq < fql)
			return 0.0e0;
		
		if (freq >= fql && freq <= fqh)
			return 0.5e0 * (1.0e0 - cos(M_PI * (freq - fql) / (fqh - fql)));
		
		if (freq > fqh)
			return 1.0e0;
	}
	
	/* ERROR */
	printf(" INVALID TAPER WINDOW SPECIFIED\n");
	return 1.0e0;
}
