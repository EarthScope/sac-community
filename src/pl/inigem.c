
#include <string.h>

#include "mach.h"
#include "pl.h"
#include "gem.h"
#include "bool.h"


#include "co.h"
#include "gdm.h"
#include "gtm.h"

void /*FUNCTION*/ inigem()
{
	int j, j_;

	/*=====================================================================
	 * PURPOSE:  Variable initialization of Graphic Environment common.
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/4
	 *=====================================================================
	 * PARAMETERS:
	 *    MCPTXT:  Maximum number of characters in labels, titles, etc. [i]
	 *    MPLAB:   Maximum number of auxiliary plot labels. [i]
	 *    MILINE:  Maximum number of entries in linestyle list. [i]
	 *    MISYM:   Maximum number of entries in symbol list. [i]
	 *    MICOL:   Maximum number of entries in color list. [i]
	 *    MTXSIZ:  Number of fixed text sizes. [i]
	 *=====================================================================
	 * SPECIAL NOTE:  Unless explicitely stated otherwise, for each variable
	 *                defined below that applies to the x axis, there is a
	 *                variable with a similiar name and definition which
	 *                applies to the y axis.
	 *=====================================================================
	 * VARIABLE DEFINITIONS FOR:  attribute constants.
	 *    ILIN:    Used to denote linear interpolation. [i]
	 *    ILOG:    Used to denote logarithmic interpolation. [i]
	 *    ISOLID:  Used to denote solid linestyle. [i]
	 *    ITHIN:   Used to denote thin line-width.
	 *    IDOT:    Used to denote dotted linestyle. [i]
	 *    KTXORI:  Names of allowed text orientations. [k]
	 *    VERT:    Used to denote vertical text orientation. [f]
	 *    HORZ:    Used to denote horizontal text orientation. [f]
	 *    TXSIZ:   Text sizes.  These are the heights of each
	 *             text size in plot coordinates. [f]
	 *    OTXSIZ:  Old set of text sizes. [f]
	 *    DTXSIZ:  New or default set of text sizes. [f]
	 *    TSDEF:   Default text size. [f]
	 *    THWRAT:  Default character height to width ratio. [f]
	 *    KTXSIZ:  Names of allowed text sizes. [k]
	 *    ITINY:   Used to denote tiny text size. [i]
	 *    ISMALL:  Used to denote small text size. [i]
	 *    IMED:    Used to denote medium text size. [i]
	 *    ILARGE:  Used to denote large text size. [i]
	 *    KSIDES:  Names of allowed locations for titles, labels, etc. [k]
	 *    ITOP:    Used to denote top of viewport. [i]
	 *    IBOT:    Used to denote bottom of viewport. [i]
	 *    IRIGHT:  Used to denote right of viewport. [i]
	 *    ILEFT:   Used to denote left of viewport. [i]
	 *===================================================================== */
	/* PROCEDURE: */
        //	cmgem.ilin = 0;
        //	cmgem.ilog = 1;
	// cmgem.isolid = 1;
	// cmgem.ithin = 1;
	// cmgem.idot = 2;
	strcpy( kmgem.ktxori[0], "HORZ    " );
	strcpy( kmgem.ktxori[1], "VERT    " );
	cmgem.dtxsiz[1-1] = 0.015;
	cmgem.dtxsiz[2-1] = 0.020;
	cmgem.dtxsiz[3-1] = 0.030;
	cmgem.dtxsiz[4-1] = 0.040;
	cmgem.dtxrat = 0.6667;
	cmgem.otxsiz[1-1] = 0.01046;
	cmgem.otxsiz[2-1] = 0.01379;
	cmgem.otxsiz[3-1] = 0.01739;
	cmgem.otxsiz[4-1] = 0.02818;
	cmgem.otxrat = 1.0;
	for( j = 1; j <= MTXSIZ; j++ ){
		cmgem.txsiz[j-1] = cmgem.dtxsiz[j-1];
		}
	cmgem.txrat = cmgem.dtxrat;
	cmgem.tsdef = cmgem.txsiz[2-1];
	strcpy( kmgem.ktxsiz[0], "TINY    " );
	strcpy( kmgem.ktxsiz[1], "SMALL   " );
	strcpy( kmgem.ktxsiz[2], "MEDIUM  " );
	strcpy( kmgem.ktxsiz[3], "LARGE   " );

	strcpy( kmgem.ksides[0], "TOP     " );
	strcpy( kmgem.ksides[1], "BOTTOM  " );
	strcpy( kmgem.ksides[2], "RIGHT   " );
	strcpy( kmgem.ksides[3], "LEFT    " );

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  text attributes.
	 *    CHWID:   Current character width. [f]
	 *    CHHT:    Current character height. [f]
	 *    CHGAP:   Current gap between characters. [f]
	 *    TSCUR:   Current character size. [f]
	 *    KGTQUA:  Current graphics text quality. [k]
	 *             = 'HARDWARE' for hardware text.
	 *             = 'SOFTWARE' for software text.
	 *    IGTFNT:  Current graphics text font number. [i]
	 *    KHJUST:  Horizontal text justification. [k]
	 *             = 'LEFT' for left justification.
	 *             = 'CENTER' for center justification.
	 *             = 'RIGHT' for right justification.
	 *    KVJUST:  Vertical text justification. [k]
	 *             = 'BOTTOM' for bottom justification.
	 *             = 'CENTER' for center justification.
	 *             = 'TOP' for top justification.
	 *    KPTXT:   Internal file for encoding plot text. [c132]
	 *===================================================================== */

	cmgem.tscur = cmgem.tsdef;
	strcpy( kmgem.kgtqua, "SOFTWARE" );
	settexttype( kmgem.kgtqua );
	cmgem.igtfnt = 1;
	strcpy( kmgem.khjust, "LEFT    " );
	strcpy( kmgem.kvjust, "BOTTOM  " );

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  input (world) and plot (viewport) limits.
	 *    LXLIM:   Fixed limits if .TRUE., scaled to data if .FALSE. [l]
	 *    XIMN:    Minimum x limit when LXLIM is .TRUE. [f]
	 *    XIMX:    Maximum x limit when LXLIM is .TRUE. [f]
	 *    XIMNU:   Minimum x limit used. [f]
	 *    XIMXU:   Maximum x limit used. [f]
	 *    XIMNZ:   Used to compute input to plot mapping. [f]
	 *             Same as XIMNU unless logarithmic interpolation is on.
	 *    XIMXZ:   See XIMNZ.
	 *    XPMN:    Minimum x axis plot (viewport) limit. [f]
	 *             Plot coordiate space is 0. to 1. with origin
	 *             in lower left hand corner.
	 *    XPMX:    Maximum x axis plot limit. [f]
	 *    XPMNU:   Actual minimum plot coordinate used. [f]
	 *    XPMXU:   Actual maximum plot coordinate used. [f]
	 *    VSPREQ:  Requested viewspace (y to x) ratio. [f]
	 *    XVSPMN:  Minimum x viewspace value. [f;=XVSP(1)]
	 *    XVSPMN:  Maximum x viewspace value. [f;=XVSP(2)]
	 *    YVSPMN:  Minimum y viewspace value. [f;=YVSP(1)]
	 *    YVSPMN:  Maximum y viewspace value. [f;=YVSP(2)]
	 *===================================================================== */

	cmgem.lxlim = FALSE;
	cmgem.ximn = 0.;
	cmgem.ximx = 1.;
	cmgem.plot.xmin = 0.1;
	cmgem.plot.xmax = 0.9;
	cmgem.lylim = FALSE;
	cmgem.yimn = 0.;
	cmgem.yimx = 1.;
	cmgem.plot.ymin = 0.15;
	cmgem.plot.ymax = 0.9;
	setworld( cmgem.ximn, cmgem.ximx, cmgem.ximn, cmgem.ximx );
	setvport( cmgem.plot.xmin, cmgem.plot.xmax, cmgem.plot.ymin, cmgem.plot.ymax );

	cmgem.view.xmin = 0.;
	cmgem.view.xmax = 1.;
	cmgem.view.ymin = 0.;
	cmgem.view.ymax = 0.75;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  input to plot mapping transformations.
	 *    XMPIP1:  X axis input to plot coordinate scale factor. [f]
	 *    XMPIP2:  X axis input to plot coordinate offset. [f]
	 *             Equation: XP = XMPIP1 * XI + XMPIP2 */

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  axes annotation generation attributes.
	 *    LXFUDG:  X axis limits enlarged slightly when .TRUE. [l]
	 *    XFUDG:   Fraction used to increase x axis limits by. [f]
	 *    LXREV:   Reverse x axis limits when .TRUE. [l]
	 *    IXINT:   X axis interpolation mode. [i]
	 *             = ILIN if linear interpolation is to be used.
	 *             = ILOG if logarithmic interpolation is to be used.
	 *    LXFULL:  Force full logarithmic decades when .TRUE. [l]
	 *    LLOGLB:  Force secondary log axis labeling when .TRUE. [l]
	 *    LXDIV:   Force a specific division spacing when .TRUE. [l]
	 *    XDIV:    Division spacing to use when LXDIV is .TRUE. [f]
	 *    LNXDIV:  Force a specific number of divisions when .TRUE. [l]
	 *    NXDIV:   Number of divisions to use when LNXDIV is .TRUE. [i]
	 *===================================================================== */

	cmgem.lxfudg = TRUE;
	cmgem.xfudg = 0.03;
	cmgem.lyfudg = TRUE;
	cmgem.yfudg = 0.03;
	cmgem.lxrev = FALSE;
	cmgem.lyrev = FALSE;
	cmgem.ixint = AXIS_LINEAR;
	cmgem.iyint = AXIS_LINEAR;
	cmgem.lxfull = TRUE;
	cmgem.lyfull = TRUE;
	cmgem.lloglb = TRUE;
	cmgem.xdiv_spacing_on = FALSE;
	cmgem.xdiv_spacing = 1.;
	cmgem.ydiv_spacing_on = FALSE;
	cmgem.ydiv_spacing = 1.;
	cmgem.xdiv_number_on = FALSE;
	cmgem.xdiv_number = 10;
	cmgem.ydiv_number_on = FALSE;
	cmgem.ydiv_number = 10;
	cmgem.lxpowr = TRUE;
	cmgem.lypowr = TRUE;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  axes label attributes.
	 *    LTOPAX:  Place an annotated axes at top of plot window if .TRUE. [l]
	 *    LBOTAX:  Place an annotated axes at bottom of window if .TRUE. [l]
	 *    LRIGAX:  Place an annotated axes to right of window if .TRUE. [l]
	 *    LLEFAX:  Place an annotated axes to left of window if .TRUE. [l]
	 *    LTOPTC:  Place an axes with tick marks at top of window if .TRUE.
	 *    LBOTTC:  Place an axes with tick marks at bottom of window if .TRUE.
	 *    LRIGTC:  Place an axes with tick marks to right of window if .TRUE.
	 *    LLEFTC:  Place an axes with tick marks to left of window if .TRUE.
	 *===================================================================== */

	cmgem.axis[TOP].annotate    = FALSE;
	cmgem.axis[BOTTOM].annotate = TRUE;
	cmgem.axis[RIGHT].annotate  = FALSE;
	cmgem.axis[LEFT].annotate   = TRUE;
	cmgem.axis[TOP].ticks       = TRUE;
	cmgem.axis[BOTTOM].ticks    = TRUE;
	cmgem.axis[RIGHT].ticks     = TRUE;
	cmgem.axis[LEFT].ticks      = TRUE;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  data generation attributes.
	 *    LXGEN:   Generate x axis data if .TRUE. [l]
	 *    XFIRST:  First x axis data value if LXGEN is .TRUE. [f]
	 *    XDELTA:  Increment between x axis data values if LXGEN is .TRUE. [f] */

	cmgem.xgen.on = FALSE;
	cmgem.xgen.first = 0.;
	cmgem.xgen.delta = 1.;
	cmgem.ygen.on = FALSE;
	cmgem.ygen.first = 0.;
	cmgem.ygen.delta = 1.;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  axes width parameters.
	 *    AXWTOP:  Current width of top axis.  This includes primary and
	 *             secondary axis annotations and any axis label.
	 *             Updated by each routine that works with axis.
	 *    AXWBOT:  Current width of bottom axis.
	 *    AXWRIG:  Current width of right axis.
	 *    AXWLEF:  Current width of left axis.
	 *    TSAXIS:  Text size of axis annotation.  See TXSIZ below.
	 *===================================================================== */

	cmgem.axis[TOP].width    = 0.;
	cmgem.axis[BOTTOM].width = 0.;
	cmgem.axis[RIGHT].width  = 0.;
	cmgem.axis[LEFT].width   = 0.;
	cmgem.tsaxis = cmgem.tsdef;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  data rendering attributes.
	 *    LFLOOR:  Puts a minimum value on data when .TRUE. and
	 *             logarithmic interpolation is on. [l]
	 *    FLOOR:   Minimum data value when LFLOOR is .TRUE. [f]
	 *    LFLUSD:  Set to .TRUE. if floor was used in last data set. [l]
	 *    LRQCLP:  Viewport clipping is on if .TRUE. [l]
	 *    LTQDP:   Terminal "Quick and dirty plotting" option on if .TRUE. [l]
	 *             Data is automatically desampled before plotting when on.
	 *    NTQDP:   Approx. number of points to plot when LTQDP is .TRUE. [i]
	 *    LFQDP:   SGF "Quick and dirty plotting" option on if .TRUE. [l]
	 *             Data is automatically desampled before plotting when on.
	 *    NFQDP:   Approx. number of points to plot when LFQDP is .TRUE. [i]
	 *    LNULL:   Set to .TRUE. if want to skip null values on a plot. [i]
	 *    VNULL:   Default NULL data value. [r]
	 *===================================================================== */

	cmgem.lfloor = TRUE;
	cmgem.floor = 1.0e-10;
	cmgem.lflusd = FALSE;
	cmgem.lrqclp = FALSE;
	cmgem.ltqdp = TRUE;
	cmgem.ntqdp = 500;
	cmgem.lfqdp = TRUE;
	cmgem.nfqdp = 1000;
	cmgem.lnull = FALSE;
	cmgem.vnull = 0.0;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  grid information
	 *    LBDR:    A border is drawn around viewport when .TRUE. [l]
	 *    LXGRD:   Grid lines parallel to x axis drawn when .TRUE. [l]
	 *    IXGRD:   X axis grid linestyle. [i]
	 *===================================================================== */

	cmgem.lbdr = FALSE;
	cmgem.lxgrd = FALSE;
	cmgem.ixgrd = LINE_STYLE_DOTTED;
	cmgem.lygrd = FALSE;
	cmgem.iygrd = LINE_STYLE_DOTTED;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  title and label attributes.
	 *    LTITL:   Title is plotted when .TRUE. [l]
	 *    KTITL:   Text of title. [c132]
	 *    ITITLP:  Position of title. [i]
	 *    NTITL:   Number of characters in title. [i]
	 *    TSTITL:  Text size of title. [f]
	 *    TATITL:  Text orientation in degrees ccw from x. [f]
	 *    LXLAB, KXLAB, NXLAB, IXLABP, TSXLAB, TAXLAB:
	 *             X axis label info.  See definitions for LTITL, etc.
	 *    LYLAB, KYLAB, NYLAB, IYLABP, TSYLAB, TAYLAB:
	 *             Y axis label info.  See definitions for LTITL, etc.
	 *===================================================================== */

	cmgem.title.on = FALSE;
	fstrncpy( kmgem.ktitl, 144, " ", 1 );
	cmgem.title.len = 0;
	cmgem.title.pos = TOP;
	cmgem.title.text_size = cmgem.tsdef;
	cmgem.xlabel.on = FALSE;
	fstrncpy( kmgem.kxlab, 144, " ", 1 );
	cmgem.xlabel.len = 0;
	cmgem.xlabel.pos = BOTTOM;
	cmgem.xlabel.text_size = cmgem.tsdef;
	cmgem.ylabel.on = FALSE;
	fstrncpy( kmgem.kylab, 144, " ", 1 );
	cmgem.ylabel.len = 0;
	cmgem.ylabel.pos = LEFT;
	cmgem.ylabel.text_size = cmgem.tsdef;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  plot label attributes.
	 *    NPLAB:      Number of last label modified. [i]
	 *    LPLAB(n):   Label "n" is to be plotted if .TRUE. [l]
	 *    LPLABL(n):  Label "n" to be position below label "n-1" if .TRUE. [l]
	 *                Label "n" to be placed at XPLABL(n),YPLABL(n) if .FALSE.
	 *    XPLABL(n):  X location of label "n". [f]
	 *    TSPLAB(n):  Text size of label "n". [f]
	 *    TAPLAB(n):  Text position of label "n". [f]
	 *    KPLAB(n):   Text of label "n". [c132]
	 *===================================================================== */

	cmgem.nplab = 0;
	cmgem.tsplab[0] = cmgem.tsdef;
	cmgem.taplab[0] = TEXT_HORIZONTAL;
	cmgem.xplabl[0] = 0.15;
	cmgem.yplabl[0] = 0.20;
	for( j = 1; j <= MPLAB; j++ ){
		j_ = j - 1;
		cmgem.lplab[j-1] = FALSE;
		cmgem.tsplab[j] = cmgem.tsdef;
		cmgem.taplab[j] = TEXT_HORIZONTAL;
		fstrncpy( kmgem.kplab[j_], 144, " ", 1 );
		cmgem.lplabl[j-1] = TRUE;
		}
	cmgem.xplabl[1] = 0.15;
	cmgem.yplabl[1] = 0.20;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  line drawing attributes.
	 *    LLINE:      Line drawing is on if .TRUE. [l]
	 *    ICLINE:     Current linestyle. [i]
	 *    LILINE:     Automatic linestyle incrementing on if .TRUE. [i]
	 *                Incrementing done for each new data array.
	 *    IILINE:     Array of linestyles for automatic incrementing. [i]
	 *    NILINE:     Current lenght of IILINE. [i]
	 *    JILINE:     Current pointer in IILINE. [i]
	 *    ISKLIN:     Skeleton linestyle. [i]
	 *                The skeleton includes axes lines, grid lines, etc.
	 *===================================================================== */

	cmgem.lline = TRUE;
	inilin( cmgem.iiline, &cmgem.niline );
	cmgem.liline = FALSE;
	cmgem.jiline = 1;
	cmgem.icline = cmgem.iiline[cmgem.jiline-1];
	cmgem.isklin = LINE_STYLE_SOLID;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  symbol drawing attributes.
	 *    LSYM:       Symbol drawing is on if .TRUE. [l]
	 *    ISYM:       Current symbol number. [i]
	 *    LNWSYM:     Set to .TRUE. if symbol number has been changed. [l]
	 *    LISYM:      Automatic symbol number incrementing on if .TRUE. [i]
	 *                Incrementing done for each new data array.
	 *    IISYM:      Array of symbol numbers for automatic incrementing. [i]
	 *    NISYM:      Current lenght of IISYM. [i]
	 *    JISYM:      Current pointer in IISYM. [i]
	 *    SYMSZ:      Symbol size in world coordinates. [f]
	 *    SYMSP:      Minimum symbol spacing in world coordinates. [f]
	 *===================================================================== */

	cmgem.lsym = FALSE;
	inisym( cmgem.iisym, &cmgem.nisym );
	cmgem.lisym = FALSE;
	cmgem.jisym = 1;
	//cmgem.lnwsym = TRUE;
	cmgem.symsz = 0.01;
	cmgem.symsp = 0.0;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  line-width attributes.
	 *    LWIDTH:       Width drawing is on if .TRUE. [l]
	 *    IWIDTH:       Current width number. [i]
	 *    LIWIDTH:      Automatic width number incrementing on if .TRUE. [i]
	 *    IIWIDTH:      Array of width numbers for automatic incrementing. [i]
	 *    NIWIDTH:      Current lenght of IIWIDTH. [i]
	 *    JIWIDTH:      Current pointer in IIWIDTHL. [i]
	 *    ISKWIDTH:     Current sleleton width value. [i]
	 *    SKDEVFUDGE:   Fudge factor for skeleton (axis) line width. [f]
	 *===================================================================== */

	cmgem.jiwidth = 1;
	cmgem.iwidth = LINE_WIDTH_THIN;
	cmgem.iswidth = LINE_WIDTH_THIN;
        cmgem.isymwidth = LINE_WIDTH_THIN;
	cmgem.iskwidth = LINE_WIDTH_THIN;
	cmgem.isskwidth = LINE_WIDTH_THIN;
        set_skeleton_fudge( 1.0 );
	iniwidth();
	cmgem.lwidth = FALSE;
	cmgem.liwidth = FALSE;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  color attributes.
	 *    LCOL:       Color drawing is on if .TRUE. [l]
	 *    ICOL:       Current color number. [i]
	 *    LICOL:      Automatic color number incrementing on if .TRUE. [i]
	 *    IICOL:      Array of color numbers for automatic incrementing. [i]
	 *    NICOL:      Current lenght of IICOL. [i]
	 *    JICOL:      Current pointer in IICOL. [i]
	 *    ISKCOL:     Skeleton color number. [i]
	 *    IBACOL:     Background color number. [i]
	 *===================================================================== */

	cmgem.lcol = FALSE;
	inicol( cmgem.iicol, &cmgem.nicol );
	cmgem.licol = FALSE;
	cmgem.jicol = 1;
	cmgem.icol = 7;
	cmgem.iskcol = 7;
	cmgem.ibacol = 0;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  automatic framing flag.
	 *    LFRAME:  Automatic framing between data sets on if .TRUE. [l]
	 *===================================================================== */

	cmgem.lframe = TRUE;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  expansion area.
	 *    EXTGEM:  Used to store new variables between major updates. [f]
	 *    KXTGEM:  Used to store new character variables between updates.
	 *===================================================================== */

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  save/restore variables.
	 *    CMGEMA:  Location of beginning of common block CMGEM.
	 *    KMGEMA:  Location of beginning of common block KMGEM.
	 *    LGEMS:   Graphics environment has been saved if .TRUE. [l]
	 *    NCMGEM:  Length of common block CMGEM to save. [i]
	 *    NKMGEM:  Length of common block KMGEM to save. [i]
	 *    CMGEMS:  Area used to save CMGEM. [f]
	 *    KMGEMS:  Area used to save KMGEM. [k]
	 *===================================================================== */

	lgems = FALSE;

	/*=====================================================================
	 * VARIABLE DEFINITIONS FOR:  annotation generation constants.
	 *    FAC:     Fractions used to compute logarithmic scalings. [f]
	 *    KFAC:    Integers 1 through 9 used in logarithmic labeling. [c1]
	 *===================================================================== */

	cmgem.fac[1-1] = 0.0000;
	cmgem.fac[2-1] = 0.3010;
	cmgem.fac[3-1] = 0.4771;
	cmgem.fac[4-1] = 0.6021;
	cmgem.fac[5-1] = 0.6990;
	cmgem.fac[6-1] = 0.7782;
	cmgem.fac[7-1] = 0.8451;
	cmgem.fac[8-1] = 0.9031;
	cmgem.fac[9-1] = 0.9542;
	kmgem.kfac[1-1] = '1';
	kmgem.kfac[2-1] = '2';
	kmgem.kfac[3-1] = '3';
	kmgem.kfac[4-1] = '4';
	kmgem.kfac[5-1] = '5';
	kmgem.kfac[6-1] = '6';
	kmgem.kfac[7-1] = '7';
	kmgem.kfac[8-1] = '8';
	kmgem.kfac[9-1] = '9';

	/* lprint is TRUE when the user wishes to print a plot, FALSE by default 
	   lSGFtemp is TRUE when SGF is turned on exclusively for PRINT option.
	   kptrName is the name of the printer to which to print. */

	cmgem.lprint = FALSE ;
	cmgem.lSGFtemp = FALSE ;
	kmgem.kptrName[0] = '\0' ;


  cmgem.lfill      = FALSE;
  cmgem.ifill[0] = 1;
  cmgem.ifill[1] = 2;
  inicol(cmgem.iifillp, &cmgem.nifill);
  inicol(cmgem.iifilln, &cmgem.nifill);
  cmgem.lifill = FALSE;
  cmgem.jifill[0] = 1;
  cmgem.jifill[1] = 2;
  
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    920527:  Added line-width initialization.
	 *    910301:  Changed iline to icline.
	 *    900529:  Deleted call to setsymbolnum. Fixed VAX/VMS problem.
	 *    900430:  Fixed bug in initial PLABEL position.
	 *    860213:  Deleted LNICE initialization.
	 *    850610:  Modified symbol initialization logic.
	 *    850510:  Changed default color value to 0.
	 *    850211:  Changed default value of text width/height ratio.
	 *    841030:  Changed default text sizes.  Old sizes can be requested.
	 *    841018:  Added text justification constants initialization.
	 *    831005:  Documented GEM common block.
	 *    820324:  Changed initial values of titles and labels to medium.
	 *    820305:  Added color attribute initialization.
	 *    811104:  Changed initial value of LTITL to .FALSE.
	 *    810928:  Added text quality and font attribute initialization.
	 *    810722:  Added symbol attribute initilization.
	 *    810414:  Original version.
	 *===================================================================== */

} /* end of function */

