
#include "icm.h"
#include "co.h"

void /*FUNCTION*/ dcpft(re, im, nfreq, incp, isignp)
double re[], im[];
int nfreq, incp, isignp;
{
	int ij, inc, is, it, ji, k0, k1, n1, n2, ninc, rc, span;
	double c, c0, i0, i1, r0, r1, s, s0, sgn, t;

        int alloc;

        static double *sines;
        static double *Sines;

        static int first = 1;
        static int mpow2 = 0;


	double *const Im = &im[0] - 1;
	double *const Re = &re[0] - 1;


	if( nfreq == 1 )
	    return;

        alloc = FALSE;

        while ( ipow(2,mpow2) < nfreq ){
            mpow2 += 1;
            alloc = TRUE;
	}

        if( alloc ) {
	    if ( !first ) {
		free(sines);
	    }
	    first = 0;
	    if((sines = (double *)malloc(mpow2*sizeof(double))) == NULL){
		printf("error allocating memory-dcpft\n");
		return;
	    }
	    Sines = sines - 1;
	    Sines[1] = 1.0e0;
	    t = atan( 1.0e0 );
	    for( is = 2; is <= mpow2; is++ ){
		Sines[is] = sin( t );
		t = t/2.0e0;
	    }
	}

	/*  SET UP VARIOUS INDICES.
	 * */
	inc = incp;
	sgn = isignp;
	ninc = nfreq*inc;
	span = ninc;
	it = nfreq/2;
	for( is = 1; is <= mpow2; is++ ){
	    if( it == 1 )
		goto L_12;
	    it = it/2;
	}

L_10:
	t = s + (s0*c - c0*s);
	c = c - (c0*c + s0*s);
	s = t;

	/*  REPLICATION LOOP. */
L_11:
	k1 = k0 + span;
	r0 = Re[k0 + 1];
	r1 = Re[k1 + 1];
	i0 = Im[k0 + 1];
	i1 = Im[k1 + 1];
	Re[k0 + 1] = r0 + r1;
	Im[k0 + 1] = i0 + i1;
	r0 = r0 - r1;
	i0 = i0 - i1;
	Re[k1 + 1] = c*r0 - s*i0;
	Im[k1 + 1] = s*r0 + c*i0;
	k0 = k1 + span;
	if( k0 < ninc )
	    goto L_11;
	k1 = k0 - ninc;
	c = -c;
	k0 = span - k1;
	if( k1 < k0 )
	    goto L_11;
	k0 = k0 + inc;
	if( k0 < k1 )
	    goto L_10;

	/*  RECURSION TO NEXT LEVEL. */
L_12:
	span = span/2;
	k0 = 0;

	/*  ANGLE=0 LOOP. */
L_13:
	k1 = k0 + span;
	r0 = Re[k0 + 1];
	r1 = Re[k1 + 1];
	i0 = Im[k0 + 1];
	i1 = Im[k1 + 1];
	Re[k0 + 1] = r0 + r1;
	Im[k0 + 1] = i0 + i1;
	Re[k1 + 1] = r0 - r1;
	Im[k1 + 1] = i0 - i1;
	k0 = k1 + span;
	if( k0 < ninc )
	    goto L_13;

	/*  ARE WE FINISHED... */
	if( span == inc )
	    goto L_20;
	/*  NO. PREPARE NON-ZERO ANGLES. */
	c0 = 2.0e0*powi(Sines[is],2);
	is = is - 1;
	s = sign( Sines[is], sgn );
	s0 = s;
	c = 1.0e0 - c0;
	k0 = inc;
	goto L_11;

L_20:
	n1 = ninc - inc;
	n2 = ninc/2;
	rc = 0;
	ji = rc;
	ij = ji;
	if( n2 == inc )
	    return;
	goto L_22;

	/*  EVEN. */
L_21:
	ij = n1 - ij;
	ji = n1 - ji;
	t = Re[ij + 1];
	Re[ij + 1] = Re[ji + 1];
	Re[ji + 1] = t;
	t = Im[ij + 1];
	Im[ij + 1] = Im[ji + 1];
	Im[ji + 1] = t;
	if( ij > n2 )
	    goto L_21;

	/*  ODD. */
L_22:
	ij = ij + inc;
	ji = ji + n2;
	t = Re[ij + 1];
	Re[ij + 1] = Re[ji + 1];
	Re[ji + 1] = t;
	t = Im[ij + 1];
	Im[ij + 1] = Im[ji + 1];
	Im[ji + 1] = t;
	it = n2;

	/*  INCREMENT REVERSED COUNTER. */
L_23:
	it = it/2;
	rc = rc - it;
	if( rc >= 0 )
	    goto L_23;
	rc = rc + 2*it;
	ji = rc;
	ij = ij + inc;
	if( ij < ji )
	    goto L_21;
	if( ij < n2 )
	    goto L_22;

	return;
} /* end of function */





