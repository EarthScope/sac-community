
#include "gem.h"


#include "msg.h"
#include "gtm.h"
#include "cpf.h"

void /*FUNCTION*/ xgemc(index, nerr)
int index, *nerr;
{



	/*=====================================================================
	 * PURPOSE: To execute a GEM command given its index number.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    index:   The commands' index number.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 0901.
	 *=====================================================================
	 * GEMULE/LEVEL: gem/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gem:     xvspmn, xvspmx, yvspmn, yvspmx, xpmn, xpmx, ypmn, ypmx
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  gtoutm, xclog, xcrtw, xylim, xcrrcp,
	 *             xxdiv, xydiv, xgrid, xaxes, xticks, xclogr,
	 *             xtitle, xxlab, xylab, xclogi, xwait, xline,
	 *             xpen, xsym, xgt, beginframe, endframe, xplab, xtsize,
	 *             setvport
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920526:  Added WIDTH command.
	 *    920224:  Added NULL command.
	 *    900310:  Added calls to setvport.
	 *    890328:  Added xreverse and yreverse commands.
	 *    870728:  Moved xlim and ylim commands to gam.
	 *    820503:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870728
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Jump to correct command based upon its index number. */

	switch( index ){
		case 1: goto L_100;
		case 2: goto L_200;
		case 3: goto L_300;
		case 4: goto L_400;
		case 5: goto L_500;
		case 6: goto L_600;
		case 7: goto L_700;
		case 8: goto L_800;
		case 9: goto L_900;
		case 10: goto L_1000;
		case 11: goto L_1100;
		case 12: goto L_1200;
		case 13: goto L_1300;
		case 14: goto L_1400;
		case 15: goto L_1500;
		case 16: goto L_1600;
		case 17: goto L_1700;
		case 18: goto L_1800;
		case 19: goto L_1900;
		case 20: goto L_2000;
		case 21: goto L_2100;
		case 22: goto L_2200;
		case 23: goto L_2300;
		case 24: goto L_2400;
		case 25: goto L_2500;
		case 26: goto L_2600;
		case 27: goto L_2700;
		case 28: goto L_2800;
		case 29: goto L_2900;
		case 30: goto L_3000;
		case 31: goto L_3100;
		case 32: goto L_3200;
		case 33: goto L_3300;
		case 34: goto L_3400;
		case 35: goto L_3500;
		case 36: goto L_3600;
		case 37: goto L_3700;
		case 38: goto L_3800;
		case 39: goto L_3900;
		case 40: goto L_4000;
		case 41: goto L_4100;
		case 42: goto L_4200;
		case 43: goto L_4300;
		case 44: goto L_4400;
		case 45: goto L_4500;
		case 46: goto L_4600;
		case 47: goto L_4700;
		case 48: goto L_4800;
		}

	/* - Error return if bad index value. */

	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XGEMC",9 );
	goto L_8888;

	/* - Command 01: XLIN */

L_100:
	cmgem.ixint = AXIS_LINEAR;
	goto L_8888;

	/* - Command 02: XLOG */

L_200:
	cmgem.ixint = AXIS_LOG;
	goto L_8888;

	/* - Command 03: YLIN */

L_300:
	cmgem.iyint = AXIS_LINEAR;
	goto L_8888;

	/* - Command 04: YLOG */

L_400:
	cmgem.iyint = AXIS_LOG;
	goto L_8888;

	/* - Command 05: LINLIN */

L_500:
	cmgem.ixint = AXIS_LINEAR;
	cmgem.iyint = AXIS_LINEAR;
	goto L_8888;

	/* - Command 06: LINLOG */

L_600:
	cmgem.ixint = AXIS_LINEAR;
	cmgem.iyint = AXIS_LOG;
	goto L_8888;

	/* - Command 07: LOGLIN */

L_700:
	cmgem.ixint = AXIS_LOG;
	cmgem.iyint = AXIS_LINEAR;
	goto L_8888;

	/* - Command 08: LOGLOG */

L_800:
	cmgem.ixint = AXIS_LOG;
	cmgem.iyint = AXIS_LOG;
	goto L_8888;

	/* - Command 09: XFULL */

L_900:
	xclog( &cmgem.lxfull, nerr );
	goto L_8888;

	/* - Command 10: YFULL */

L_1000:
	xclog( &cmgem.lyfull, nerr );
	goto L_8888;

	/* - Command 11: not being used. */

L_1100:
	;
	goto L_8888;

	/* - Command 12: not being used */

L_1200:
	;
	goto L_8888;

	/* - Command 13: XVP */

L_1300:
  {
    double xmin, xmax;
    xmin = (double) cmgem.plot.xmin;
    xmax = (double) cmgem.plot.xmax;
    xcrrcp( 0., 1., &xmin, &xmax, nerr );
    cmgem.plot.xmin = (float) xmin;
    cmgem.plot.xmax = (float) xmax;
    setvport( cmgem.plot.xmin, cmgem.plot.xmax, cmgem.plot.ymin, cmgem.plot.ymax );
  }
	goto L_8888;

	/* - Command 14: YVP */

L_1400:
  {
    double ymin, ymax;
    ymin = (double) cmgem.plot.ymin;
    ymax = (double) cmgem.plot.ymax;
    xcrrcp( 0., 1., &ymin, &ymax, nerr );
    cmgem.plot.ymin = (float) ymin;
    cmgem.plot.ymax = (float) ymax;
    setvport( cmgem.plot.xmin, cmgem.plot.xmax, cmgem.plot.ymin, cmgem.plot.ymax );
  }
	goto L_8888;

	/* - Command 15: XDIV */

L_1500:
	xxdiv( nerr );
	goto L_8888;

	/* - Command 16: YDIV */

L_1600:
	xydiv( nerr );
	goto L_8888;

	/* - Command 17: GRID */

L_1700:
	xgrid( nerr );
	goto L_8888;

	/* - Command 18: BORDER */

L_1800:
	xclog( &cmgem.lbdr, nerr );
	goto L_8888;

	/* - Command 19: AXES */

L_1900:
	xaxes( nerr );
	goto L_8888;

	/* - Command 20: TICKS */

L_2000:
	xticks( nerr );
	goto L_8888;

	/* - Command 21: LOGLAB */

L_2100:
	xclog( &cmgem.lloglb, nerr );
	goto L_8888;

	/* - Command 22: XFUDGE */

L_2200:
  {
    double fudge;
    fudge = (double) cmgem.xfudg;
    xclogr( &cmgem.lxfudg, &fudge, nerr );
    cmgem.xfudg = (float) fudge;
  }
	goto L_8888;

	/* - Command 23: YFUDGE */

L_2300:
  {
    double fudge;
    fudge = (double) cmgem.yfudg;
    xclogr( &cmgem.lyfudg, &fudge, nerr );
    cmgem.yfudg = (float) fudge;
  }
	goto L_8888;

	/* - Command 24: TITLE */

L_2400:
	xtitle( nerr );
	goto L_8888;

	/* - Command 25: XLABEL */

L_2500:
	xxlab( nerr );
	goto L_8888;

	/* - Command 26: YLABEL */

L_2600:
	xylab( nerr );
	goto L_8888;

	/* - Command 27: QDP */

L_2700:
	xqdp( nerr );
	goto L_8888;

	/* - Command 28: FLOOR */

L_2800:
  {
    double v;
    v = (double) cmgem.floor;
    xclogr( &cmgem.lfloor, &v, nerr );
    cmgem.floor = (float) v;
  }
	goto L_8888;

	/* - Command 29: WAIT */

L_2900:
	xwait( nerr );
	goto L_8888;

	/* - Command 30: LINE */

L_3000:
	xline( nerr );
	goto L_8888;

	/* - Command 31: NO-OP */

L_3100:
	;
	goto L_8888;

	/* - Command 32: SYMBOL */

L_3200:
	xsym( nerr );
	goto L_8888;

	/* - Command 33: BEGFR */

L_3300:
	xbeginframe( nerr );
	goto L_8888;

	/* - Command 34: ENDFR */

L_3400:
	xendframe( nerr );
	goto L_8888;

	/* - Command 35:  GTEXT */

L_3500:
	xgt( nerr );
	goto L_8888;

	/* - Command 36:  COLOR */

L_3600:
	xcolor( nerr );
	goto L_8888;

	/* - Command 37:  XGRID */

L_3700:
	xxgrid( nerr );
	goto L_8888;

	/* - Command 38:  YGRID */

L_3800:
	xygrid( nerr );
	goto L_8888;

	/* - Command 39:  PLABEL */

L_3900:
	xplab( nerr );
	goto L_8888;

	/* - Command 40:  TSIZE */

L_4000:
	xtsize( nerr );
	goto L_8888;

	/* - Command 41:  WINDOW */

L_4100:
	xwindow( nerr );
	goto L_8888;

	/* -- Command 42: BEGINWINDOW */

L_4200:
	xbeginwindow( nerr );
	goto L_8888;

	/* - Command 43: XREVERSE */

L_4300:
	xclog( &cmgem.lxrev, nerr );
	goto L_8888;

	/* - Command 44: YREVERSE */

L_4400:
	xclog( &cmgem.lyrev, nerr );
	goto L_8888;

	/* - Command 45: NULL */

L_4500:
  {
    double v;
    v = (double) cmgem.vnull;
    xclogr( &cmgem.lnull, &v, nerr );
    cmgem.vnull = (float) v;
  }
	goto L_8888;

	/* - Command 46: WIDTH */

L_4600:
	xwidth( nerr );
	goto L_8888;

L_4700:
        xlct(nerr);
        goto L_8888;

L_4800:	/* Command 48:  PRINT */
/*	xprint ( nerr ) ;
	goto L_8888; */

L_8888:
	return;

} /* end of function */

