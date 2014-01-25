
#include <stdio.h>
#include <string.h>

#include "sss.h"
#include "gam.h"
#include "gem.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "pl.h"
#include "bot.h"
#include "ucf.h"
#include "msg.h"
#include "gdm.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

void /*FUNCTION*/ xplotstack(nerr)
int *nerr;
{
	char kptext[MCMSG+1], kret[9];
	int lactive, lany, lwait , lframs ;
	int ioffsetdta, ioffsettw, jdfl, jdfl1, jdfl2, 
	 jfr, jloc, ncret, ndx1, ndx2, nfr, 
	 nlen, nperfr, numplot;
	float delay, factor, unused, unused_, xwloc, ypdel, ypmxsv, ywloc;
	void zgpmsg();
	static char kwait[9] = "Waiting$";
    char *tmp;
        float *Sacmem;

	/*=====================================================================
	 * PURPOSE:  To execute the PLOTST command.  This command plots the
	 *           files in the signal stack.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    dfm:     ndfl, kdfl
	 *    sss:     ltwlim, lvm, twlim
	 *    gem:     ypmn, ypmx, yvspmx
	 *    gam:     kgddef
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:     lpswt, lpspl, lpssum, lpsper, npsper
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, lclog, cfmt, cresp, getstatus, begindevices, 
	 *             vflist, setms, plsave, vmcalc, vmdly, beginframe, pl2d, 
	 *             pltext, getxlm, getylm, settextsize, centxt, 
	 *             plhome, endframe, plrest
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    960710:  Allows it to determine a time window from context.
	 *    881122:  Fixed some bugs with viewspace/viewport sizing.
	 *    850819:  Major rewrite of subprocess.
	 *    850801:  Changes in argument list for RDSAC.
	 *    811228:  Deleted call to ZCLIP.
	 *    810120:  Changed to output message retrieval from disk.
	 *    800515:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850819
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

		/* -- "WEIGHTS ON|OFF":  plot files with or without weights. */
		if( lklog( "WEIGHTS$",9, &cmsss.lpswt ) )
		{ /* do nothing */ }

		/* -- "POLARITY ON|OFF":  plot files with or without polarity. */
		else if( lklog( "POLARITY$",10, &cmsss.lpspl ) )
		{ /* do nothing */ }

		/* -- "SUM ON|OFF":  plot files with or without summed signal. */
		else if( lklog( "SUM$",5, &cmsss.lpssum ) )
		{ /* do nothing */ }

		/* -- "PERPLOT ON|OFF|n":  set number of plots per frame. */
		else if( lklogi( "PERPLOT$",9, &cmsss.lpsper, &cmsss.npsper ) )
		{ /* do nothing */ }

		/* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();
		}
	}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	/* CHECKING PHASE: */

	/* - If no graphics device is open, try to open the default graphics device. */

	getstatus( "ANY", &lany );
	if( !lany ){
		begindevices( kmgam.kgddef,9, 1, nerr );
		if( *nerr != 0 )
			goto L_8888;
	}

	/* - Test for non-null DFL. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Check for a defined time window. */

	if( !cmsss.ltwlim ){
		/* if the user didn't define a time window, figure one out. maf 960710 */
		float       hedgeSize ;       /* the amount by which to pad the data in the plot */

		/* find the earliest begin time and the latest end time */
		extrma( cmsss.beginTime, 1, cmdfm.ndfl, &Twlim[1], &unused, &unused_ );
		extrma( cmsss.endTime, 1, cmdfm.ndfl, &unused, &Twlim[2], &unused_ );

		/* hedge the data by a small amount on either side */
		hedgeSize = 0.15*(Twlim[2] - Twlim[1]);
		Twlim[1] = Twlim[1] - hedgeSize;
		Twlim[2] = Twlim[2] + hedgeSize;
	}

	/* - Check for a defined summation. */

	if( cmsss.lpssum && (cmsss.nlnsum <= 0) ){
		setmsg( "WARNING", 5113 );
		outmsg();
	}

	/* EXECUTION PHASE: */

	/* - Save current values in common block cmpl2d and reset them after plot. */

	plsave();

	/* - Set up specific options for this plot only. */

	cmgem.axis[LEFT].annotate   = TRUE;
	cmgem.axis[RIGHT].annotate  = FALSE;
	cmgem.axis[TOP].annotate    = FALSE;
	cmgem.axis[BOTTOM].annotate = FALSE;
	cmgem.axis[LEFT].ticks      = TRUE;
	cmgem.axis[RIGHT].ticks     = TRUE;
	cmgem.axis[TOP].ticks       = TRUE;
	cmgem.axis[BOTTOM].ticks    = FALSE;
	cmgem.title.on = FALSE;

	/* - Calculate velocity model delays. */

	if( Lvm[1] ){
		vmcalc( 1, nerr );
		if( *nerr != 0 )
			goto L_8888;
		vmdly( nerr );
		if( *nerr != 0 )
			goto L_8888;
	}

	/* - Set up y window for each subplot. */

	if( cmsss.lpsper ){
		nfr = (cmdfm.ndfl - 1)/cmsss.npsper + 1;
		nperfr = cmsss.npsper;
	}
	else{
		nfr = 1;
		nperfr = cmdfm.ndfl;
	}
	if( cmsss.lpssum && (cmsss.nlnsum > 0) )
		nperfr = nperfr + 1;
	ypdel = (cmgem.plot.ymax - cmgem.plot.ymin)/(float)( nperfr );

	/* - Check WAIT option.  This is on when:
	 * -- An active device (normally the user's terminal) is on.
	 * -- There is more than one frame to plot. */

	getstatus( "ACTIVE", &lactive );
	lwait = lactive && nfr > 1;

	/* - Set up x plot limits. */

	cmgem.ximn = Twlim[1];
	cmgem.ximx = Twlim[2];

	/* - Plot each file in DFL in its own subplot region with framing off.
	 *   Previous contents of the DFL are destroyed. */

	lframs = cmgem.lframe ;
	cmgem.lframe = FALSE;
	cmgem.xgen.on = TRUE;
	cmgem.xgen.delta = cmsss.del;
	cmgem.xgen.first = Twlim[1];

	/* - Loop on number of frames. */

	jdfl1 = 1;
	ypmxsv = cmgem.plot.ymax;
	for( jfr = 1; jfr <= nfr; jfr++ ){
		if ( lframs )
			beginframe( FALSE , nerr );

		/* -- Plot sum if requested. */
		if( cmsss.lpssum && (cmsss.nlnsum > 0) ){
			cmgem.plot.ymin = cmgem.plot.ymax - ypdel;
			pl2d( (float*)&unused, cmmem.sacmem[cmsss.ndxsum], cmsss.nlnsum, 
			 1, 1, nerr );
			if( *nerr != 0 )
				goto L_7777;
			fstrncpy( kptext, MCMSG, kmsss.knmsum, strlen(kmsss.knmsum));
			cmgem.chht = cmgem.tsdef;
			cmgem.chwid = cmgem.txrat*cmgem.chht;
			settextsize( cmgem.chwid, cmgem.chht );
			xwloc = cmgem.plot.xmin + 3.*cmgem.chwid;
			ywloc = cmgem.plot.ymax*cmgem.view.ymax - cmgem.chht;
			pltext( kptext,MCMSG+1, xwloc, ywloc );
			cmgem.plot.ymax = cmgem.plot.ymin;
			cmgem.axis[TOP].ticks = FALSE;
		}

		/* -- Plot files in stack file list. */
		jdfl2 = min( cmdfm.ndfl, jdfl1 + nperfr - 1 );
		for( jdfl = jdfl1; jdfl <= jdfl2; jdfl++ ){
			/* --- Adjust some plot parameters if necessary. */
			if( jdfl == jdfl2 )
				cmgem.axis[BOTTOM].ticks = TRUE;
			cmgem.plot.ymin = cmgem.plot.ymax - ypdel;
			/* --- Get file from memory manager. */
			getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
			if( *nerr != 0 )
				goto L_7777;
			/* --- Set up delay and compute intersection of file's data and plot's 
			 *     time windows.  This determines how many data points to plot. */
			delay = Dlyt[jdfl] + Dlyn[jdfl]*cmsss.del + Dlyvm[jdfl];
			definelimits( Twlim[1], Twlim[2], *b + delay, *e + delay, 
			 *delta, &ioffsettw, &ioffsetdta, &numplot );
			/* -- Set up plot parameters and plot. */
			factor = 1.;
			if( cmsss.lpswt )
				factor = Wt[jdfl];
			if( cmsss.lpspl && !Lpol[jdfl] )
				factor = -factor;
			if( factor != 1. ){
                                Sacmem = cmmem.sacmem[ndx1];
				for( jloc = ndx1; jloc <= (ndx1 - 1); jloc++ ){
                                        *(Sacmem++) *= factor;
				}
			}
			getxlm( &cmgem.lxlim, &cmgem.ximn, &cmgem.ximx );
			getylm( &cmgem.lylim, &cmgem.yimn, &cmgem.yimx );
			cmgem.xgen.first = *b + delay + *delta*(float)( ioffsetdta );
			pl2d( (float*)&unused, cmmem.sacmem[ndx1]+ioffsetdta, numplot, 
			 1, 1, nerr );
			if( *nerr != 0 )
				goto L_7777;
			cmgem.chht = cmgem.tsdef;
			cmgem.chwid = cmgem.txrat*cmgem.chht;
			settextsize( cmgem.chwid, cmgem.chht );
            tmp = string_list_get(datafiles, jdfl-1);
            fstrncpy(kptext, MCMSG, tmp, strlen(tmp)+1);
			xwloc = cmgem.plot.xmin + 3.*cmgem.chwid;
			ywloc = cmgem.plot.ymax*cmgem.view.ymax - cmgem.chht;
			pltext( kptext,MCMSG+1, xwloc, ywloc );
			if( delay != 0. ){
                                sprintf(kptext,"%s%16.5g", "DLY:", delay );
				ywloc = ywloc - cmgem.chht;
				pltext( kptext,MCMSG+1, xwloc, ywloc );
			}
			if( Wt[jdfl] != 1. && cmsss.lpswt ){
                                sprintf(kptext,"%s%16.5g", "WT:", Wt[jdfl] );
				ywloc = ywloc - cmgem.chht;
				pltext( kptext,MCMSG+1, xwloc, ywloc );
			}
			if( cmsss.lpspl && !Lpol[jdfl] ){
                                sprintf(kptext,"%s", "Polarity reversed");
				ywloc = ywloc - cmgem.chht;
				pltext( kptext,MCMSG+1, xwloc, ywloc );
			}
			cmgem.axis[TOP].ticks = FALSE;
			cmgem.plot.ymax = cmgem.plot.ymin;
		} /* end for ( jdfl ) */

		/* -- Put time axes at bottom of plot and call frame. */
		cmgem.chht = cmgem.tsaxis;
		cmgem.chwid = cmgem.txrat*cmgem.chht;
		settextsize( cmgem.chwid, cmgem.chht );
		cmgem.axis[BOTTOM].annotate = TRUE;
		xlinax();
		centxt( "Time (sec)  [with individual delays]",37, 34, BOTTOM, cmgem.xlabel.text_size );
		plhome();
		endframe( FALSE , nerr );
		cmgem.axis[BOTTOM].annotate = FALSE;
		cmgem.axis[TOP].ticks       = TRUE;
		cmgem.axis[BOTTOM].ticks    = FALSE;
		cmgem.plot.ymax = ypmxsv;

		/* -- Wait for user prompt before plotting next frame if appropriate. */
		if( lwait && jfr < nfr ){
			zgpmsg( kwait,9, kret,9 );
			ncret = indexb( kret,9 );
			upcase( kret, ncret, kret,9 );
			if( kret[0] == 'K' )
				goto L_7777;
			if( kret[0] == 'G' )
				lwait = FALSE;
		}

		jdfl1 = jdfl2 + 1;
	} /* end for ( jfr ) */

	/* - Restore graphics environment before returning. */

L_7777:
	plrest();

L_8888:
	cmgem.lframe = lframs ;
	return;

} /* end of function */

