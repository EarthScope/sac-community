
#include <string.h>

#include "sam.h"
#include "gem.h"
#include "gam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "pl.h"
#include "bot.h"
#include "msg.h"
#include "gdm.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

void /*FUNCTION*/ xpsp(nerr)
int *nerr;
{
	char kret[9];
	int lany, lconv, lframs, lwait, ncret;
	int index, jdfl, ndx1, ndx2, nlen, nptspl;
	float xjunk;
	void zgpmsg();
	static char kwait[9] = "Waiting$";


	/*=====================================================================
	 * PURPOSE:  To execute the action command PSP.
	 *           This command plots spectral data files.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     MDFL
	 *    SAM:     KPSPTP, LPAMPH, LPRLIM, LPSPC1, LPSPC2, KPSPL1, KPSPL2,
	 *             IXSPIN, IYSPIN, NSPTPL, KSPTPL
	 *    GAM:     KGDDEF, LWAITR, LWAITE
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SAM:     KPSPTP, LPAMPH, LPRLIM, LPSPC1, LPSPC2, KPSPL1, KPSPL2,
	 *             IXSPIN, IYSPIN
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LCLIST,
	 *             VFLIST, VFSPEC
	 *             GETSTATUS, BEGINDEVICE, PLSAVE, GETFIL, TOAMPH, TORLIM,
	 *             INDEXB, GETYLM, PL2D, ZGPMSG, PLREST
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

	     /* -- "component":  select spectral component(s) to plot. */
	     if( lclist( (char*)kmsam.ksptpl,9, cmsam.nsptpl, &index ) ){
		strcpy( kmsam.kpsptp, kmsam.ksptpl[index - 1] );
		/* Case branch:   ASIS,RLIM,AMPH,RL  ,IM  ,AM  ,PH  ,POWER */
		switch( index ){
		    case 1: cmsam.lpamph = FALSE;
			    cmsam.lprlim = FALSE;
			    cmsam.lpspc1 = TRUE;
			    cmsam.lpspc2 = TRUE;
			    break ;
		    case 2: cmsam.lpamph = FALSE;
			    cmsam.lprlim = TRUE;
			    cmsam.lpspc1 = TRUE;
			    cmsam.lpspc2 = TRUE;
			    strcpy( kmsam.kpspl1, "Real Component  " );
			    strcpy( kmsam.kpspl2, "Imag. Component " );
			    break ;
		    case 3: cmsam.lpamph = TRUE;
			    cmsam.lprlim = FALSE;
			    cmsam.lpspc1 = TRUE;
			    cmsam.lpspc2 = TRUE;
			    strcpy( kmsam.kpspl1, "Amplitude       " );
			    strcpy( kmsam.kpspl2, "Phase (radians) " );
			    break ;
		    case 4: cmsam.lpamph = FALSE;
			    cmsam.lprlim = TRUE;
			    cmsam.lpspc1 = TRUE;
			    cmsam.lpspc2 = FALSE;
			    strcpy( kmsam.kpspl1, "Real Component  " );
			    strcpy( kmsam.kpspl2, "NOT PLOTTED     " );
			    break ;
		    case 5: cmsam.lpamph = FALSE;
			    cmsam.lprlim = TRUE;
			    cmsam.lpspc1 = FALSE;
			    cmsam.lpspc2 = TRUE;
			    strcpy( kmsam.kpspl1, "NOT PLOTTED     " );
			    strcpy( kmsam.kpspl2, "Imag. Component " );
			    break ;
		    case 6: cmsam.lpamph = TRUE;
			    cmsam.lprlim = FALSE;
			    cmsam.lpspc1 = TRUE;
			    cmsam.lpspc2 = FALSE;
			    strcpy( kmsam.kpspl1, "Amplitude       " );
			    strcpy( kmsam.kpspl2, "NOT PLOTTED     " );
			    break ;
		    case 7: cmsam.lpamph = TRUE;
			    cmsam.lprlim = FALSE;
			    cmsam.lpspc1 = FALSE;
			    cmsam.lpspc2 = TRUE;
			    strcpy( kmsam.kpspl1, "NOT PLOTTED     " );
			    strcpy( kmsam.kpspl2, "Phase (radians) " );
			    break ;
		    case 8: *nerr = 1012;			/* TEMP */
			    setmsg( "ERROR", *nerr );
			    apcmsg( kmsam.kpsptp,9 );	/* ENDTEMP */
			    cmsam.lpamph = TRUE;
			    cmsam.lprlim = FALSE;
			    cmsam.lpspc1 = TRUE;
			    cmsam.lpspc2 = FALSE;
			    strcpy( kmsam.kpspl1, "Power           " );
			    strcpy( kmsam.kpspl2, "NOT PLOTTED     " );
			    break ;
		} /* end switch */

	     } /* end if( lclist( (char*)kmsam.ksptpl ... */

	     else if( lckey( "LINLIN$",8 ) ){
		cmsam.ixspin = AXIS_LINEAR;
		cmsam.iyspin = AXIS_LINEAR;
	     }
	     else if( lckey( "LINLOG$",8 ) ){
		cmsam.ixspin = AXIS_LINEAR;
		cmsam.iyspin = AXIS_LOG;
	     }
	     else if( lckey( "LOGLIN$",8 ) ){
		cmsam.ixspin = AXIS_LOG;
		cmsam.iyspin = AXIS_LINEAR;
	     }
	     else if( lckey( "LOGLOG$",8 ) ){
		cmsam.ixspin = AXIS_LOG;
		cmsam.iyspin = AXIS_LOG;
	     }
	     else if( lckey( "XLIN$",6 ) ){
		cmsam.ixspin = AXIS_LINEAR;
	     }
	     else if( lckey( "XLOG$",6 ) ){
		cmsam.ixspin = AXIS_LOG;
	     }
	     else if( lckey( "YLIN$",6 ) ){
		cmsam.iyspin = AXIS_LINEAR;
	     }
	     else if( lckey( "YLOG$",6 ) ){
		cmsam.iyspin = AXIS_LOG;
	     }

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

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Check to make sure all files are spectral files. */

	vfspec( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* EXECUTION PHASE: */

	/* - If no graphics device is open, try to open the default graphics device. */

	getstatus( "ANY", &lany );
	if( !lany ){
	    begindevice( kmgam.kgddef,9, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

	/* - Save current plot environment and define specific
	 *   options that apply to these spectral plots. */

	plsave();
	lframs = cmgem.lframe;
	cmgem.lframe = FALSE;
	cmgem.xgen.on = TRUE;
	cmgem.ygen.on = FALSE;
	cmgem.ylabel.on = TRUE;
	cmgem.xlabel.on = TRUE;
	fstrncpy( kmgem.kxlab, 144, "Frequency (Hz)", 14 );
	cmgem.xlabel.len = 14;

	/* - Check WAIT option.  This is on when:
	 * -- A wait request has been made.
	 * -- An active device (normally the user's terminal) is on. */

	if( cmgam.lwaitr ){
	    getstatus( "ACTIVE", &lwait );
	}
	else{
	    lwait = FALSE;
	}

	/* - For each file in data file list: */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

	    /* -- Get the next file in DFL, moving header to CMHDR. */

	    getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
	    if( *nerr != 0 )
		goto L_7777;

	    /* -- Convert spectral file type if needed. */

	    if( cmsam.lpamph && *iftype == *irlim ){
		toamph( cmmem.sacmem[ndx1], cmmem.sacmem[ndx2], *npts,
		 cmmem.sacmem[ndx1], cmmem.sacmem[ndx2] );
		lconv = TRUE;
		*iftype = *iamph;
	    }
	    else if( cmsam.lprlim && *iftype == *iamph ){
		torlim( cmmem.sacmem[ndx1], cmmem.sacmem[ndx2], *npts,
		 cmmem.sacmem[ndx1], cmmem.sacmem[ndx2] );
		lconv = TRUE;
		*iftype = *irlim;
	    }
	    else{
		lconv = FALSE;
	    }


	    /* -- Set up specific plot options for this data file. */

	    nptspl = *npts/2 - 1;
	    cmgem.xgen.first = *delta;
	    cmgem.xgen.delta = *delta;
	    getxlm( &cmgem.lxlim, &cmgem.ximn, &cmgem.ximx );
	    getylm( &cmgem.lylim, &cmgem.yimn, &cmgem.yimx );

	    /* -- Determine suffixes if KPSPTP is 'ASIS'. */

	    if( strcmp(kmsam.kpsptp,"ASIS    ") == 0 ){
		if( *iftype == *irlim ){
		    strcpy( kmsam.kpspl1, "REAL COMPONENT  " );
		    strcpy( kmsam.kpspl2, "IMAGINARY CMP.  " );
		    cmsam.lprlim = TRUE;
		    cmsam.lpamph = FALSE;
		}
		else{
		    strcpy( kmsam.kpspl1, "AMPLITUDE       " );
		    strcpy( kmsam.kpspl2, "PHASE (RADIANS) " );
		    cmsam.lprlim = FALSE;
		    cmsam.lpamph = TRUE;
		}
	    }

	    /* -- Plot first spectral component if requested. */

	    if( cmsam.lpspc1 ){

		/* --- Set up interpolation mode. */
		cmgem.ixint = cmsam.ixspin;
		cmgem.iyint = cmsam.iyspin;

		/* --- Set up y axis label. */
		fstrncpy( kmgem.kylab, 144, kmsam.kpspl1, strlen(kmsam.kpspl1));
		cmgem.ylabel.len = indexb( kmgem.kylab,145 );

		/* --- Plot data, excluding first data point (dc level). */
		if( lframs ){
		    beginframe( FALSE , nerr );
		    getvspace( &cmgem.view.xmin, &cmgem.view.xmax, 
                               &cmgem.view.ymin, &cmgem.view.ymax );
		}

		pl2d( (float*)&xjunk, cmmem.sacmem[ndx1]+1, nptspl, 1,1, nerr );
		if( *nerr != 0 )
		    goto L_7777;

		dispid( cmgam.lfinorq, jdfl, 0, NULL );

		plhome();
		if( lframs )
		    endframe( FALSE , nerr );
                else
                  flushbuffer(nerr);

		/* --- Wait for user prompt before plotting
			next frame if appropriate. */
		if( ( jdfl == cmdfm.ndfl && !cmsam.lpspc2 ) && !cmgam.lwaite )
		    lwait = FALSE;
		if( lwait ){
		    zgpmsg( kwait,9, kret,9 );
		    ncret = indexb( kret,9 );
		    upcase( kret, ncret, kret,9 );
		    if( kret[0] == 'K' )
			goto L_7777;
		    if( kret[0] == 'G' )
			lwait = FALSE;
		}
	    } /* end if( cmsam.lpspc1 ) */

	    /* -- Plot second spectral component if requested. */

	    if( cmsam.lpspc2 ){
		/* --- Set up y axis label. */
		fstrncpy( kmgem.kylab, 144, kmsam.kpspl2, strlen(kmsam.kpspl2));
		cmgem.ylabel.len = indexb( kmgem.kylab,145 );

		/* --- Set up interpolation mode,
			forcing phase plots to be linlin. */
		if( cmsam.lpamph ){
		    cmgem.ixint = AXIS_LINEAR;
		    cmgem.iyint = AXIS_LINEAR;
		}
		else{
		    cmgem.ixint = cmsam.ixspin;
		    cmgem.iyint = cmsam.iyspin;
		}

		/* --- Plot data, excluding first data point (dc level). */
		if( lframs ){
		    beginframe( FALSE , nerr );
		    getvspace( &cmgem.view.xmin, &cmgem.view.xmax, 
                               &cmgem.view.ymin, &cmgem.view.ymax );
		}
		pl2d( (float*)&xjunk, cmmem.sacmem[ndx2]+1, nptspl, 1,1, nerr );
		if( *nerr != 0 )
		    goto L_8888;
		dispid( cmgam.lfinorq, jdfl, 0, NULL ); 

		plhome();
		if( lframs )
		    endframe( FALSE , nerr );
                else
                  flushbuffer(nerr);

		/* --- Wait for user prompt before plotting
			next frame if appropriate. */
		if( jdfl == cmdfm.ndfl && !cmgam.lwaite )
		    lwait = FALSE;
		if( lwait ){
		    zgpmsg( kwait,9, kret,9 );
		    if( kret[0] == 'K' )
			goto L_7777;
		    if( kret[0] == 'G' )
			lwait = FALSE;
		}

	    } /* end if( cmsam.lpspc2 ) */

	    /* -- Convert file back to original type if necessary. */

	    if( lconv && *iftype == *irlim ){
		toamph( cmmem.sacmem[ndx1], cmmem.sacmem[ndx2], *npts,
			cmmem.sacmem[ndx1], cmmem.sacmem[ndx2] );
		*iftype = *iamph;
	    }
	    else if( lconv && *iftype == *iamph ){
		torlim( cmmem.sacmem[ndx1], cmmem.sacmem[ndx2], *npts,
			cmmem.sacmem[ndx1], cmmem.sacmem[ndx2] );
		*iftype = *irlim;
	    }

	} /* end for ( jdfl ) */

	/* - Restore plot environment and return. */

L_7777:
	plrest();

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
         *    970130:  Added arguments to dispid() to plot file number. maf
	 *    850307:  Deleted y limits on phase plot.
	 *    821206:  Added logic to upcase wait response.
	 *    821122:  Deleted use of temporary arrays.
	 *    820430:  Mod due to change in ZGPMSG.
	 *    820331:  Combined "parse" and "control" modules.
	 *    810401:  Original version.
	 *===================================================================== */

} /* end of function */

