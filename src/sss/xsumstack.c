
#include "sss.h"
#include "gam.h"
#include "dfm.h"
#include "amf.h"
#include "gem.h"
#include "hdr.h"
#include "bool.h"


#include "pl.h"
#include "ucf.h"
#include "msg.h"
#include "gdm.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

float *sss_sum;

void /*FUNCTION*/ xsumstack(nerr)
int *nerr;
{
	int lany;
	int ioffsetdata, ioffsetsum, j, jdfl,
    nlnsumnew, numintersect;
	float delay, factor, norm, swts, unused;
  sac *s;

	/*=====================================================================
	 * PURPOSE:  To execute the SUMSTACK command.  This command sums the
	 *           files in the signal stack.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 5103, 5105
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    dfm:     ndfl
	 *    gam:     kgddef
	 *    sss:     wt, lpol, dlyt, dlyn, dlyvm
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:     lnorm, twlim, del, ndxsum, nlnsum
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, lklog, cfmt, cresp, setmsg, getstatus, 
	 *             begindevices, allamb, plsave, pl2d, plrest
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    norm:    Normalization factor. [f]
	 *    delay:   Total time delay for a signal. [f]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    881114:  Minor cleanup work and name change.
	 *    850819:  Major rewrite of stacking module.
	 *    850801:  Changes in argument list for RDSAC.
	 *    821201:  Changed to lastest set of parsing and checking functions.
	 *    810120:  Changed to output message retrieval from disk.
	 *    790529:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  881115
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "NORMALIZATION ON/OFF":  sum stack with or without normalization. */
		if( lklog( "NORMALIZATION$",15, &cmsss.lnorm ) ){

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	/* CHECKING PHASE: */

	/* - Check for a defined stack window. */

	if( !cmsss.ltwlim ){
		*nerr = 5103;
		setmsg( "ERROR", *nerr );
		goto L_8888;
		}

	/* - If no graphics device is open, try to open the default graphics device. */

	getstatus( "ANY", &lany );
	if( !lany ){
		begindevices( kmgam.kgddef,9, 1, nerr );
		if( *nerr != 0 )
			goto L_8888;
		}

	/* EXECUTION PHASE: */

	/* - Compute normalization if any. */

	if( cmsss.lnorm ){
		swts = 0.;
		for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
			swts = swts + Wt[jdfl];
			}
		norm = 1./swts;
		}
	else{
		norm = 1.0;
		}

	/* - Compute length of stack window.
	 *   Allocate memory block if needed. */

	nlnsumnew = (int)( (Twlim[2] - Twlim[1])/cmsss.del ) + 1;
	if( cmsss.nlnsum > 0 ){
		if( nlnsumnew != cmsss.nlnsum ){
      FREE(sss_sum);
			//relamb( cmmem.sacmem, cmsss.ndxsum, nerr );
			cmsss.nlnsum = nlnsumnew;
      sss_sum = (float *) malloc(sizeof(float) * cmsss.nlnsum);
			//allamb( &cmmem, cmsss.nlnsum, &cmsss.ndxsum, 
			// nerr );
			if( *nerr != 0 )
				goto L_8888;
			}
		}
	else{
		cmsss.nlnsum = nlnsumnew;
    sss_sum = (float *) malloc(sizeof(float) * cmsss.nlnsum);
		//allamb( &cmmem, cmsss.nlnsum, &cmsss.ndxsum, 
    //nerr );
		if( *nerr != 0 )
			goto L_8888;
		}

	/* - Initialize memory block. */

	fill( sss_sum, cmsss.nlnsum, 0.0 );

	/* - Calculate velocity model delays. */

	if( Lvm[1] ){
		vmcalc( 1, nerr );
		if( *nerr != 0 )
			goto L_8888;
		vmdly( nerr );
		if( *nerr != 0 )
			goto L_8888;
		}

	/* - Loop on number of files in stack list. */

	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){

		/* -- Compute multiplying factor. */
		factor = Wt[jdfl]*norm;

		/* -- Reverse polarity if requested. */
		if( !Lpol[jdfl] )
			factor = -factor;

		/* -- Get the next file. */
    if(!(s = sacget(jdfl-1, TRUE, nerr))) {
      goto L_8888;
    }
		//getfil( jdfl, TRUE, &nlen, &ndxy, &notused, nerr );

		/* -- Set up delay and compute intersection of file's data and the
		 *    summation's time windows.  This determines how much data to sum. */
		delay = Dlyt[jdfl] + Dlyn[jdfl]*cmsss.del + Dlyvm[jdfl];
		definelimits( Twlim[1], Twlim[2], s->h->b + delay, s->h->e + delay, 
		 s->h->delta, &ioffsetsum, &ioffsetdata, &numintersect );

		/* -- Loop on length of sumstack window. */
		for( j = 0; j < numintersect; j++ ){
      if(j+ioffsetsum < cmsss.nlnsum &&
         j+ioffsetdata < s->h->npts) {
        sss_sum[j + ioffsetsum] += factor * s->y[j+ioffsetdata];
			}
    }
  }

	/* - Plot resulting stacked signal. */

	plsave();
	cmgem.axis[LEFT].ticks      = TRUE;
	cmgem.axis[RIGHT].ticks     = TRUE;
	cmgem.axis[TOP].ticks       = TRUE;
	cmgem.axis[BOTTOM].ticks    = TRUE;
	cmgem.axis[LEFT].annotate   = TRUE;
	cmgem.axis[RIGHT].annotate  = FALSE;
	cmgem.axis[TOP].annotate    = FALSE;
	cmgem.axis[BOTTOM].annotate = TRUE;
	cmgem.ximn = Twlim[1];
	cmgem.ximx = Twlim[2];
	cmgem.lframe = TRUE;
	cmgem.xlabel.on = TRUE;
	fstrncpy( kmgem.kxlab, 144, "Delayed time (sec)", 18 );
	cmgem.xlabel.len = 18;
	cmgem.ylabel.on = TRUE;
	fstrncpy( kmgem.kylab, 144, "Stacked Signal ", 15 );
	cmgem.ylabel.len = 14;
	cmgem.title.on = TRUE;
	cmgem.xgen.on = TRUE;
	cmgem.xgen.delta = cmsss.del;
	cmgem.xgen.first = Twlim[1];
	pl2d( (float*)&unused, sss_sum, cmsss.nlnsum, 1, 
	 1, nerr );
	if( *nerr != 0 )
		goto L_8888;
	plrest();

L_8888:
	return;

} /* end of function */

