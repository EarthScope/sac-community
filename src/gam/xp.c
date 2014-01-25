#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mach.h"
#include "gem.h"
#include "gdm.h"
#include "gam.h"
#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "bool.h"


#include "pl.h"
#include "bot.h"
#include "msg.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

void xp(int *nerr)
{
	char kret[9];
	int lany, lframs, lwait, lxgens, lprint = FALSE ;
	int jdfl, ncret, nlcx, nlcy, nlen, notused;
	static char kwait[9] = "Waiting$";


	/*=====================================================================
	 * PURPOSE:  To execute the action command PLOT.
	 *           This command plots data in memory, one file per frame.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  GAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GEM:     LFRAME, LEVERY
	 *    GAM:     KGDDEF, LWAITR, LWAITE
	 *    DFM:     NDFL
	 *    MEM:     SACMEM
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GEM:     LXGEN, XDELTA, XFIRST, LYLIM, YIMN, YIMX
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  VFLIST, VFTIME, GETSTATUS, BEGINDEVICES, GETFIL, GETXLM,
	 *             GETYLM, INDEXB, BEGINFRAME, PL2D, DISPID, DISPPK, PLHOME,
	 *             ENDFRAME, ZGPMSG, UPCASE
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    KWAIT:   Message sent to terminal when in wait mode.
	 *    KRET:    Message received from terminal when in wait mode.
	 *    NLCX:    Location in SACMEM array of each file's x data.
	 *    NLCY:    Location in SACMEM array of each file's y data.
	 *    NLEN:    Length of each data file.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    970130:  Added arguments to dispid() to plot file number. maf
	 *    910607:  Added call to zgetgd when no graphics device specified.
	 *             Changed begindevice to begindevices. (wct)
	 *    861118:  Added a default graphics device option.
	 *    830114:  Added call to PLHOME.
	 *    821206:  Added call to upcase wait response.
	 *    820430:  Mod due to change in ZGPMSG.
	 *    810120:  Changed to output message retrieval from disk.
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Save current values of several GEM parameters. */

	lxgens = cmgem.xgen.on;
	lframs = cmgem.lframe;

	/* PARSING PHASE: */

	if ( lcmore( nerr ) ){
	    /* -- "PRINT":  print the final product. */
	    if( lckey( "PRINT#$", 8 ) ) {
		if ( cmgdm.lbegf ) {
		    setmsg ( "WARNING" , 2403 ) ;
		    outmsg () ;
		    clrmsg () ;
		}
		else {
		    lprint = TRUE ;
		    lcchar ( MAXPRNTRNAMELEN   , kmgem.kptrName ,
			     MAXPRNTRNAMELEN+1 , &notused ) ;
		    terminate ( kmgem.kptrName ) ;
		}
	    }

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	}

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Check to make sure all files are time series files. */

	vftime( nerr );
	if( *nerr != 0 ){
	    aplmsg( "Use PLOTSP command to plot spectral data.",42 );
	    goto L_8888;
	}

	/* - If no graphics device is open, try to open the default device. */

	getstatus( "ANY", &lany );
	if( !lany ){
	    zgetgd( kmgam.kgddef,9 );
	    begindevices( kmgam.kgddef,9, 1, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

	/* EXECUTION PHASE: */

	/* - Check WAIT option.  This is on when:
	 * -- A wait request has been made.
	 * -- An active device (normally the user's terminal) is on. */

	if( cmgam.lwaitr ){
	    getstatus( "ACTIVE", &lwait );
	}
	else{
	    lwait = FALSE;
	}

	/* - For each file in DFL: */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	    /* -- Get file from memory manager. */
	    getfil( jdfl, TRUE, &nlen, &nlcy, &nlcx, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Set up x axis data generation parameters if evenly spaced. */
	    if( *leven ){
		cmgem.xgen.delta = *delta;
		cmgem.xgen.first = *begin;
		cmgem.xgen.on = TRUE;
	    }
	    else{
		cmgem.xgen.on = FALSE;
	    }

	    /* -- Determine x axis plot limits. */
	    getxlm( &cmgem.lxlim, &cmgem.ximn, &cmgem.ximx );

	    /* -- Determine y axis plot limits. */
	    getylm( &cmgem.lylim, &cmgem.yimn, &cmgem.yimx );

	    /* -- Begin next plot frame if requested. */
	    if( lframs ){
		beginframe( lprint , nerr );
		if( *nerr != 0 )
		    goto L_8888;
		getvspace( &cmgem.view.xmin, &cmgem.view.xmax, 
                           &cmgem.view.ymin, &cmgem.view.ymax );
	    }

	    /* -- Plot the data.  Do not allow PL2D to perform framing. */
	    cmgem.lframe = FALSE;
	    pl2d( cmmem.sacmem[nlcx], cmmem.sacmem[nlcy], nlen, 1, 1, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Plot the frame id, pick display and home cursor. */
	    dispid( cmgam.lfinorq , jdfl, 0, NULL );

	    disppk( 0. );
	    plhome();
	    flushbuffer( nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- End current plot frame if requested. */
	    if( lframs )
		endframe( TRUE , nerr );
            else 
              flushbuffer( nerr );
	    /* -- Wait for user prompt before plotting next frame if appropriate. */
	    if( jdfl == cmdfm.ndfl && !cmgam.lwaite )
		lwait = FALSE;
	    if( lwait ){
		zgpmsg( kwait,9, kret,9 );
		ncret = indexb( kret,9 );
		upcase( kret, ncret, kret,9 );
		if( kret[0] == 'K' )
		    goto L_8888;
		if( kret[0] == 'G' )
		    lwait = FALSE;
	    }
	}

	/* - Restore GEM parameters before returning. */

L_8888:
	cmgem.xgen.on = lxgens;
	cmgem.lframe = lframs;
	return;

} /* end of function */

