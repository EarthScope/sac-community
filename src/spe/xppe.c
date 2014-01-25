
#include "spe.h"
#include "gam.h"
#include "gem.h"
#include "amf.h"
#include "bool.h"


#include "msg.h"
#include "gdm.h"
#include "pl.h"
#include "dbh.h"
#include "co.h"

void /*FUNCTION*/ xppe(nerr)
int *nerr;
{
	int lany , lframs ;
	int ifidls;
	float xjunk;

	/*=====================================================================
	 * PURPOSE:  To execute the action command PPE.
	 *           This command plots the prediction error vs frequency.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag.  Set to 0 if no error occurred.
	 *             Potential error numbers:  5003
	 *=====================================================================
	 * MODULE/LEVEL:  SPE/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    SPE:     LCOR, NLNCOR, NDXCOR, NDXPE, MLNPE
	 *    GAM:     IFIDLC, IUR, KGDDEF
	 *    GEM:     LFRAME, LTOPAX, LBOTAX, LRIGAX, LLEFAX, LTOPTC, LBOTTC,
	 *             LRIGTC, LLEFTC, LTITLE, LXLAB, KXLAB,NXLAB, IXLAPB, IBOT,
	 *             IXLABS, ITINY, LYLAB, KYLAB, NYLAB, IYLABP, ILEFT, IYLABS,
	 *             LXGEN, XFIRST, XDELTA
	 *    DFM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  GTOUTM, GETSTATUS, BEGINDEVICES, PLSAVE, CRIT,
	 *             BEGINFRAME(NERR), PL2D, DISPID, ENDFRAME(NERR), PLREST
	 *=====================================================================
	 * LOCAL VARIABLES
	 *    IFIDLS:  Used to save and restore fileid attribute. [i]
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;
	lframs = cmgem.lframe ;
	/* CHECKING PHASE: */

	/* - Make sure correlation function has been calculated. */

	if( !cmspe.lcor ){
	    *nerr = 5003;
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

	/* EXECUTION PHASE: */

	/* - Save plot environment */

	plsave();
	ifidls = cmgam.ifidlc;

	/* - Set up specific options for this plot */

	cmgam.ifidlc = cmgam.iur;
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
	cmgem.title.on = FALSE;
	cmgem.xlabel.on = TRUE;
	fstrncpy( kmgem.kxlab, 144, "Lag Number", 10 );
	cmgem.xlabel.len = 10;
	cmgem.xlabel.pos = BOTTOM;
	cmgem.ylabel.on = TRUE;
	fstrncpy( kmgem.kylab, 144, "RMS Prediction Error", 20 );
	cmgem.ylabel.len = 20;
	cmgem.ylabel.pos = LEFT;

	/* - Set up x axis parameters. */

	cmgem.xgen.on = TRUE;
	cmgem.xgen.first = 1.;
	cmgem.xgen.delta = 1.;

	/* - Calculate prediciton error. */

	crit( cmmem.sacmem[cmspe.ndxcor], cmspe.nlncor, cmmem.sacmem[cmspe.ndxpe] );

	/* - Plot prediction error vs lag number. */

	if ( lframs )
	    beginframe( FALSE , nerr );
	pl2d( (float*)&xjunk, cmmem.sacmem[cmspe.ndxpe], MLNPE, 1, 1, nerr );
	if( *nerr != 0 )
	    goto L_7777;

	/* - Plot the file id. */

	dispid( 0 , 0, 0, NULL );

	/* - End plot and restore plot environment. */

L_7777:
	plhome();
	if ( lframs )
          endframe( FALSE , nerr );
        else
          flushbuffer( nerr );
	plrest();
	cmgam.ifidlc = ifidls;

L_8888:
	cmgem.lframe = lframs ;
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
         *    970130:  Added arguments to dispid() not to plot file number. maf
	 *    050718:  Use default device. (GRH/UB)
         *    841227:  Changes due to major rewrite of SPE subprocess.
	 *    821130:  Changed name from PPECN and made minor mods.
	 *    810120:  Changed to output message retrieval from disk.
	 *    801024:  Added a plot id.
	 *    800925:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850109
	 *===================================================================== */

} /* end of function */

