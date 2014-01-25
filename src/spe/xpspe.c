
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "spe.h"
#include "gam.h"
#include "gem.h"
#include "amf.h"
#include "bool.h"
#include "gtm.h"
#include "pl.h"
#include "msg.h"
#include "gdm.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

void 
xpspe(	int *nerr)
{
        int i, n;
        char **last;
	int lany, lsavpk , lframs ;
	int ifidls, j1, ndxplt, nlnplt;
	float frwid, x1, x2, xjunk, y1, yd;

        float *Sacmem1, *Sacmem2;

	/*=====================================================================
	 * PURPOSE: To parse and execute the action command PLOTSPE.
	 *          This command plots the latest spectral estimate.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL:  SPE/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL, SACMEM
	 *    GAM:     IFIDLC, IUR, XFIDLC, LDSPPK, KGDDEF
	 *    GEM:     LFRAME, LTOPAX, LBOTAX, LRIGAX, LLEFAX, LTOPTC, LBOTTC,
	 *             LRIGTC, LLEFTC, LXLAB, KXLAB,NXLAB, IXLAPB, IBOT,
	 *             IXLABS, ITINY, LYLAB, KYLAB, NYLAB, IYLABP, ILEFT, IYLABS,
	 *             LXGEN, XFIRST, XDELTA, 'RIGHT', 'BOTTOM', ICLINE
	 *    SPE:     KSPSTP, NPSPTP, LSPE, NDXSPE, NLNSPE, LCL, LRESL,
	 *             SAMFRQ, NDXAUX, CLU, CLL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SPE:     IPSPTP, LRQCL
	 *    DFM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCLIST, LKLOG, GTOUTM, GETSTATUS,
	 *             BEGINDEVICES, PLSAVE, BEGINFRAME, PL2D, DISPID, PLTEXT,
	 *             LINE, PLHOME, ENDFRAME, PLREST, SETTEXTJUST
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    NDXPLT:  Index in SACMEM array of data being plotted.
	 *=====================================================================
	 * MODIFICATION HISTORY:
         *    050718:  Use default device. (GRH/UB)
         *    050202:  Honor xlim values in PLOTSPE plot (GRH/UB).
         *    031124:  Bug fix in PLOTSPE AMPLITUDE command (GRH/UB).
         *    970130:  Added arguments to dispid() not to plot file number. maf
	 *    910301:  Changed iline to icline.
	 *    850215:  Fixed bug involving wrong sampling interval for plot.
	 *    841227:  Changes due to major rewrite of SPE subprocess.
	 *    821130:  Combined PSEPS and PSECN.
	 *             Changed to latest command parsing logic.
	 *    801024:  Moved plot id logic to subroutine SEID.
	 *    800925:  Added ability to turn on/off conf. limits and plot id.
	 *    810120:  Changed to output message retrieval from disk.
	 *    800911:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850109
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;
	lframs = cmgem.lframe ;
	/* - PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

		/* -- "POWER/LOG/AMPLITUDE":  select type of plot. */
		if( lclist( (char*)kmspe.kpsptp,9, cmspe.npsptp, &cmspe.ipsptp ) )
		{ /* do nothing */ }

		/* -- "CONFIDENCE [ON/OFF]":  turn confidence limit plotting on or off. */
		else if( lklog( "CONFIDENCE$",12, &cmspe.lrqcl ) )
		{ /* do nothing */ }

		/* -- Bad syntax. */
		else{
		    cfmt( "ILLEGAL OPTION:",17 );
		    cresp();
		}
	} /* end while */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
	    goto L_8888;

	/* - CHECKING PHASE: */

	if( cmspe.lrqcl ){
	    setmsg( "WARNING", 5007 );
	    outmsg();
	}

	/* - Make sure a spectral estimation calculation has been made. */

	if( !cmspe.lspe ){
	    *nerr = 5004;
	    setmsg( "ERROR", *nerr );
	    goto L_8888;
	}

	/* - If no graphics device is open, try to open the default. */

	getstatus( "ANY", &lany );
	if( !lany ){
	    begindevices( kmgam.kgddef,9, 1, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

	/* - EXECUTION PHASE: */

	/* - Save plot environment */

	plsave();
	ifidls = cmgam.ifidlc;

	/* - Set up specific options for this plot */

	cmgam.ifidlc = cmgam.iur;
	cmgam.xfidlc = cmgam.fidbdr;
	lsavpk = cmgam.ldsppk;
	cmgam.ldsppk = FALSE;
	lframs = cmgem.lframe ;
	cmgem.lframe = FALSE;
	cmgem.axis[TOP].annotate    = FALSE;
	cmgem.axis[BOTTOM].annotate = TRUE;
	cmgem.axis[RIGHT].annotate  = FALSE;
	cmgem.axis[LEFT].annotate   = TRUE;
	cmgem.axis[TOP].ticks       = TRUE;
	cmgem.axis[BOTTOM].ticks    = TRUE;
	cmgem.axis[RIGHT].ticks     = TRUE;
	cmgem.axis[LEFT].ticks      = TRUE;
	cmgem.xlabel.on = TRUE;
	fstrncpy( kmgem.kxlab, 144, "Frequency (Hz)", 14 );
	cmgem.xlabel.len = 14;
	cmgem.xlabel.pos = BOTTOM;
	cmgem.ylabel.on = TRUE;
	cmgem.ylabel.pos = LEFT;

	/* - Set up x axis parameters. */

	cmgem.xgen.on = TRUE;
	cmgem.xgen.first = 0.;
	cmgem.xgen.delta = cmspe.samfrq/(float)( cmspe.nlnspe - 1 );
        getxlm( &cmgem.lxlim, &cmgem.ximn, &cmgem.ximx );  /* added 100423 jas/vt as by gbh */
        
	/* - Set up the data array containing the spectral estimation. */

	nlnplt = cmspe.nlnspe/2 + 1;
        if( cmspe.ipsptp == 1 ){
	    cmgem.iyint = AXIS_LINEAR;
	    fstrncpy( kmgem.kylab, 144, "Power", 5 );
	    cmgem.ylabel.len = 5;
	    ndxplt = cmspe.ndxspe;
	}
	else if( cmspe.ipsptp == 2 ){
	    cmgem.iyint = AXIS_LOG;
	    fstrncpy( kmgem.kylab, 144, "Power (loglog scale)", 20 );
	    cmgem.ylabel.len = 17;
	    ndxplt = cmspe.ndxspe;
	}
	else if( cmspe.ipsptp == 3 ){
	    cmgem.iyint = AXIS_LINEAR;
	    fstrncpy( kmgem.kylab, 144, "Amplitude", 9 );
	    cmgem.ylabel.len = 9;
	    ndxplt = cmspe.ndxaux;
            Sacmem1 = cmmem.sacmem[ndxplt];
            Sacmem2 = cmmem.sacmem[cmspe.ndxspe];
	    for( j1 = ndxplt; j1 <= (ndxplt + nlnplt - 1); j1++ )
                *(Sacmem1++) = sqrt(*(Sacmem2++));
	} else {
        fprintf(stderr, "Plot Spectral Estimate: Unknown plot type\n");
        goto L_8888;
    }

	/* - Make the basic plot of spectral estimation vs frequency. */

	if ( lframs )
	    beginframe( FALSE , nerr );
	pl2d( (float*)&xjunk, cmmem.sacmem[ndxplt], nlnplt, 1, 1, nerr );
	if( *nerr != 0 )
	    goto L_7777;

	/* - Plot the file id.  Add the SE id below it. */
        last = NULL;
        n = 0;
	if( cmspe.lspeid ){
          n = 0;
          last = (char **) malloc(sizeof(char *) * 5);
	    /* -- Blank line between file id and SE id. */
          last[n++] = strdup(" ");
          //cmgam.yfidlc = cmgam.yfidlc - cmgem.chht;

                                  
	    /* -- Type of spectral estimate. */
          last[n++] = fstrdup(kmspe.kpspl1, 17);
          //pltext( kmspe.kpspl1,17, cmgam.xfidlc, cmgam.yfidlc );
          //cmgam.yfidlc = cmgam.yfidlc - cmgem.chht;

	    /* -- Window length or order. */
          last[n++] = fstrdup(kmspe.kpspl2, 17);
          //pltext( kmspe.kpspl2,17, cmgam.xfidlc, cmgam.yfidlc );
          //cmgam.yfidlc = cmgam.yfidlc - cmgem.chht;

          /* -- Type of correlation window (if PDS): */
          if( strcmp(kmspe.kpspl3,"                ") != 0 ){
            last[n++] = fstrdup(kmspe.kpspl3, 17);
            //pltext( kmspe.kpspl3,17, cmgam.xfidlc, cmgam.yfidlc );
            //cmgam.yfidlc = cmgam.yfidlc - cmgem.chht;
          }
          asprintf(&last[n++],"Number %5d", cmspe.nlnspe );
          //pltext( kpspl4,17, cmgam.xfidlc, cmgam.yfidlc );
          //cmgam.yfidlc = cmgam.yfidlc - cmgem.chht;
	}
	dispid( 0 , 0, n, last );

        if(n > 0) {
          for(i = 0; i < n; i++) {
            free(last[i]);
            last[i] = NULL;
          }
          free(last);
          last = NULL;
        }


	/* - Add the estimation resolution if known. */

	cmspe.lresl = FALSE;
	if( cmspe.lresl ){
	    pltext( "FR:",4, cmgam.xfidlc, cmgam.yfidlc );
	    frwid = (cmgem.uplot.xmax - cmgem.uplot.xmin)*cmspe.resl/(0.5*cmspe.samfrq);
	    x1 = cmgam.xfidlc + 4.*cmgem.chwid;
	    x2 = x1 + frwid;
	    y1 = cmgam.yfidlc;
	    yd = 0.3*cmgem.chht;
	    line( x1, y1, x1, y1 + 2.*yd );
	    line( x1, y1 + yd, x2, y1 + yd );
	    line( x2, y1, x2, y1 + 2.*yd );
	}

	/* - Add the confidence limits if known. */

	if( cmspe.lcl && cmspe.lrqcl ){
	    cmgem.icline = LINE_STYLE_DOTTED;

	    /* -- Lower confidence band. */
	    ndxplt = cmspe.ndxaux;
            Sacmem1 = cmmem.sacmem[ndxplt];
            Sacmem2 = cmmem.sacmem[cmspe.ndxspe];
	    if( cmspe.ipsptp == 1 || cmspe.ipsptp == 2 ){
		for( j1 = ndxplt; j1 <= (ndxplt + nlnplt - 1); j1++ )
                    *(Sacmem1++) = cmspe.cll**(Sacmem2++);
	    }
	    else if( cmspe.ipsptp == 3 ){
		for( j1 = ndxplt; j1 <= (ndxplt + nlnplt - 1); j1++ )
                    *(Sacmem1++) = sqrt(cmspe.cll**(Sacmem2++));
	    }
	    pldta( (float*)&xjunk, cmmem.sacmem[ndxplt], nlnplt, 1, 1, nerr );
	    if( *nerr != 0 )
			goto L_7777;

	    /* - Upper confidence band. */
	    ndxplt = cmspe.ndxaux;
            Sacmem1 = cmmem.sacmem[ndxplt];
            Sacmem2 = cmmem.sacmem[cmspe.ndxspe];
	    if( cmspe.ipsptp == 1 || cmspe.ipsptp == 2 ){
		for( j1 = ndxplt; j1 <= (ndxplt + nlnplt - 1); j1++ )
                   *(Sacmem1++) = cmspe.clu**(Sacmem2++);
	    }
	    else if( cmspe.ipsptp == 3 ){
		for( j1 = ndxplt; j1 <= (ndxplt + nlnplt - 1); j1++ )
		    *(Sacmem1++) = sqrt(cmspe.clu**(Sacmem2++));
	    }
	    pldta( (float*)&xjunk, cmmem.sacmem[ndxplt], nlnplt, 1, 1, nerr );
	    if( *nerr != 0 )
			goto L_7777;
	} /* end if( cmspe.lcl && cmspe.lrqcl ) */

	/* - End plot and restore plot environment. */

L_7777:
	plhome();
	if ( lframs ) 
          endframe( FALSE , nerr );
        else 
          flushbuffer( nerr );
	plrest();
	cmgam.ldsppk = lsavpk;
	cmgam.ifidlc = ifidls;

L_8888:
	cmgem.lframe = lframs ;
	return;

} /* end of function */

