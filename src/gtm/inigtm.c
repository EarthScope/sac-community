
#include "gtm.h"

#include "bool.h"

void /*FUNCTION*/ inigtm()
{

	/*=====================================================================
	 * *** INTERNAL SUBROUTINE:  NOT NORMALLY CALLED BY USER ***
	 *=====================================================================
	 * PURPOSE: To initialize the Graphics Tool Module common block.
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870501:  Added initialization of viewport clipping flag.
	 *    861027:  Original version from inisym and iniwvt.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861027
	 *===================================================================== */
	/* PROCEDURE: */
	/*=====================================================================
	 * INITIALIZATION FOR:  World to viewport mapping transformation.
	 *=====================================================================
	 * VARIABLE DEFINITIONS:
	 * xwcmin:     Minimum X world coordinates value. [f]
	 * xwcmax:     Maximum X world coordinates value. [f]
	 * ywcmin:     Minimum Y world coordinates value. [f]
	 * ywcmax:     Maximum Y world coordinates value. [f]
	 * xvpmim:     Minimum X viewport value. [f]
	 * xvpmax:     Maximum X viewport value. [f]
	 * yvpmim:     Minimum Y viewport value. [f]
	 * yvpmax:     Maximum Y viewport value. [f]
	 * xmpwv[12]:  X axis world to viewport mapping transformation. [f]
	 *             Equation is: XVP=XMPWV1*XWC+XMPWV2
	 * ympwv[12]:  Y axis world to viewport mapping transformation. [f]
	 *             Equation is: YVP=YMPWV1*YWC+YMPWV2
	 * lvpfull:    Flag for full or fixed viewport. [l]
	 *             = .TRUE. for full viewport.
	 *             = .FALSE. for fixed viewport ratio.
	 * vpratioreq: Requested fixed viewport ratio. [f]
	 * vpleft:     Left hand side viewport border size. [f]
	 * vpright:    Right hand side viewport border size. [f]
	 * vpbottom:   Bottom side viewport border size. [f]
	 * vptop:      Top side viewport border size. [f]
	 *===================================================================== */
	cmgtm.xwcmin = 0.;
	cmgtm.xwcmax = 1.;
	cmgtm.ywcmin = 0.;
	cmgtm.ywcmax = 1.;

	cmgtm.xvpmax = 1.;
	cmgtm.yvpmax = 1.;

	cmgtm.xmpwv1 = 1.;
	cmgtm.xmpwv2 = 0.;
	cmgtm.ympwv1 = 1.;
	cmgtm.ympwv2 = 0.;

	cmgtm.lvpclip = TRUE;

	/*=====================================================================
	 * INITIALIZATION FOR: Symbol drawing constants.
	 *=====================================================================
	 * PARAMETERS:
	 *    msym:       Total number of stroked symbols. [i]
	 *    munsym:     Number of unscalable symbols.  Remainder are scalable. [i]
	 *    msisym:     Number of basic symbols.  Remainder are complex
	 *                symbols made from two basic symbols. [i]
	 *    msymtb:     Length of symbol storage table. [i]
	 *=====================================================================
	 * VARIABLE DEFINITIONS:
	 *    nsymlc(n):  Location within symbol tables of first element
	 *                of basic symbol number "n".
	 *    isyml1(j):  The number of the first basic symbol for symbol "j".
	 *    isyml2(j):  The number of the second basic symbol for symbol "j".
	 *                Set to 0 if symbol contains only a single basic symbol.
	 *    ldrsym(i):  Draw to location if element "i" is .TRUE.
	 *    xsymtb(i):  X location of element "i".  Full range is (-1.,1.).
	 *    ysymtb(i):  Y location of element "i".
	 *===================================================================== */


	cmgtm.nsymlc[0] = 1;
	cmgtm.nsymlc[1] = 6;
	cmgtm.nsymlc[2] = 11;
	cmgtm.nsymlc[3] = 15;
	cmgtm.nsymlc[4] = 24;
	cmgtm.nsymlc[5] = 29;
	cmgtm.nsymlc[6] = 33;
	cmgtm.nsymlc[7] = 37;
	cmgtm.nsymlc[8] = 41;

	cmgtm.isyml1[0] = 1;
	cmgtm.isyml1[1] = 2;
        cmgtm.isyml1[2] = 3;
	cmgtm.isyml1[3] = 4;
	cmgtm.isyml1[4] = 5;
	cmgtm.isyml1[5] = 6;
	cmgtm.isyml1[6] = 7;
	cmgtm.isyml1[7] = 8;
	cmgtm.isyml1[8] = 7;
	cmgtm.isyml1[9] = 2;
	cmgtm.isyml1[10] = 4;
	cmgtm.isyml1[11] = 5;
	cmgtm.isyml1[12] = 2;
	cmgtm.isyml1[13] = 4;
	cmgtm.isyml1[14] = 2;
	cmgtm.isyml1[15] = 2;

	cmgtm.isyml2[0] = 0;
	cmgtm.isyml2[1] = 0;
	cmgtm.isyml2[2] = 0;
	cmgtm.isyml2[3] = 0;
	cmgtm.isyml2[4] = 0;
	cmgtm.isyml2[5] = 0;
	cmgtm.isyml2[6] = 0;
	cmgtm.isyml2[7] = 0;
	cmgtm.isyml2[8] = 8;
	cmgtm.isyml2[9] = 7;
	cmgtm.isyml2[10] = 7;
	cmgtm.isyml2[11] = 7;
	cmgtm.isyml2[12] = 8;
	cmgtm.isyml2[13] = 8;
	cmgtm.isyml2[14] = 3;
	cmgtm.isyml2[15] = 6;

        cmgtm.ldrsym[0] = FALSE;
	cmgtm.ldrsym[1] = TRUE;
	cmgtm.ldrsym[2] = TRUE;
	cmgtm.ldrsym[3] = TRUE;
	cmgtm.ldrsym[4] = TRUE;
	cmgtm.ldrsym[5] = FALSE;
	cmgtm.ldrsym[6] = TRUE;
	cmgtm.ldrsym[7] = TRUE;
	cmgtm.ldrsym[8] = TRUE;
	cmgtm.ldrsym[9] = TRUE;
	cmgtm.ldrsym[10] = FALSE;
	cmgtm.ldrsym[11] = TRUE;
	cmgtm.ldrsym[12] = TRUE;
	cmgtm.ldrsym[13] = TRUE;
	cmgtm.ldrsym[14] = FALSE;
	cmgtm.ldrsym[15] = TRUE;
	cmgtm.ldrsym[16] = TRUE;
	cmgtm.ldrsym[17] = TRUE;
	cmgtm.ldrsym[18] = TRUE;
	cmgtm.ldrsym[19] = TRUE;
	cmgtm.ldrsym[20] = TRUE;
	cmgtm.ldrsym[21] = TRUE;
	cmgtm.ldrsym[22] = TRUE;
	cmgtm.ldrsym[23] = FALSE;
	cmgtm.ldrsym[24] = TRUE;
	cmgtm.ldrsym[25] = TRUE;
	cmgtm.ldrsym[26] = TRUE;
	cmgtm.ldrsym[27] = TRUE;
	cmgtm.ldrsym[28] = FALSE;
	cmgtm.ldrsym[29] = TRUE;
	cmgtm.ldrsym[30] = TRUE;
	cmgtm.ldrsym[31] = TRUE;
	cmgtm.ldrsym[32] = FALSE;
	cmgtm.ldrsym[33] = TRUE;
	cmgtm.ldrsym[34] = FALSE;
	cmgtm.ldrsym[35] = TRUE;
	cmgtm.ldrsym[36] = FALSE;
	cmgtm.ldrsym[37] = TRUE;
	cmgtm.ldrsym[38] = FALSE;
	cmgtm.ldrsym[39] = TRUE;

        cmgtm.xsymtb[0] = -0.0015;
        cmgtm.xsymtb[1] = 0.000;
        cmgtm.xsymtb[2] = 0.003;
        cmgtm.xsymtb[3] = 0.000;
        cmgtm.xsymtb[4] = -0.003;
        cmgtm.xsymtb[5] = -0.50;
        cmgtm.xsymtb[6] = 0.00;
        cmgtm.xsymtb[7] = 1.00;
        cmgtm.xsymtb[8] = 0.00;
        cmgtm.xsymtb[9] = -1.00;
        cmgtm.xsymtb[10] = -0.50;
        cmgtm.xsymtb[11] = 0.50;
        cmgtm.xsymtb[12] = 0.50;
        cmgtm.xsymtb[13] = -1.00;
        cmgtm.xsymtb[14] = -0.50;
        cmgtm.xsymtb[15] = 0.00;
        cmgtm.xsymtb[16] = 0.25;
        cmgtm.xsymtb[17] = 0.50;
        cmgtm.xsymtb[18] = 0.25;
        cmgtm.xsymtb[19] = 0.00;
        cmgtm.xsymtb[20] = -0.25;
        cmgtm.xsymtb[21] = -0.50;
        cmgtm.xsymtb[22] = -0.25;
        cmgtm.xsymtb[23] = -0.50;
        cmgtm.xsymtb[24] = 0.50;
        cmgtm.xsymtb[25] = 0.50;
        cmgtm.xsymtb[26] = -0.50;
        cmgtm.xsymtb[27] = -0.50;
        cmgtm.xsymtb[28] = 0.00;
        cmgtm.xsymtb[29] = -0.50;
        cmgtm.xsymtb[30] = 1.00;
        cmgtm.xsymtb[31] = -0.50;
        cmgtm.xsymtb[32] = -0.50;
        cmgtm.xsymtb[33] = 1.00;
        cmgtm.xsymtb[34] = -0.50;
        cmgtm.xsymtb[35] = 0.00;
        cmgtm.xsymtb[36] = -0.50;
        cmgtm.xsymtb[37] = 1.00;
        cmgtm.xsymtb[38] = -1.00;
        cmgtm.xsymtb[39] = 1.00;

        cmgtm.ysymtb[0] = -0.0015;
        cmgtm.ysymtb[1] = 0.003;
        cmgtm.ysymtb[2] = 0.000;
        cmgtm.ysymtb[3] = -0.003;
        cmgtm.ysymtb[4] = 0.000;
        cmgtm.ysymtb[5] = -0.50;
        cmgtm.ysymtb[6] = 1.00;
        cmgtm.ysymtb[7] = 0.00;
        cmgtm.ysymtb[8] = -1.00;
        cmgtm.ysymtb[9] = 0.00;
        cmgtm.ysymtb[10] = -0.50;
        cmgtm.ysymtb[11] = 1.00;
        cmgtm.ysymtb[12] = -1.00;
        cmgtm.ysymtb[13] = 0.00;
        cmgtm.ysymtb[14] = -0.25;
        cmgtm.ysymtb[15] = 0.50;
        cmgtm.ysymtb[16] = 0.25;
        cmgtm.ysymtb[17] = 0.00;
        cmgtm.ysymtb[18] = -0.25;
        cmgtm.ysymtb[19] = -0.50;
        cmgtm.ysymtb[20] = -0.25;
        cmgtm.ysymtb[21] = 0.00;
        cmgtm.ysymtb[22] = 0.25;
        cmgtm.ysymtb[23] = 0.00;
        cmgtm.ysymtb[24] = 0.50;
        cmgtm.ysymtb[25] = -0.50;
        cmgtm.ysymtb[26] = -0.50;
        cmgtm.ysymtb[27] = 0.50;
        cmgtm.ysymtb[28] = -0.50;
        cmgtm.ysymtb[29] = 1.00;
        cmgtm.ysymtb[30] = 0.00;
        cmgtm.ysymtb[31] = -1.00;
        cmgtm.ysymtb[32] = 0.00;
        cmgtm.ysymtb[33] = 0.00;
        cmgtm.ysymtb[34] = 0.50;
        cmgtm.ysymtb[35] = -1.00;
        cmgtm.ysymtb[36] = -0.50;
        cmgtm.ysymtb[37] = 1.00;
        cmgtm.ysymtb[38] = 0.00;
        cmgtm.ysymtb[39] = -1.00;

	/*=====================================================================
	 * INITIALIZATION FOR:  Symbol drawing attributes.
	 *=====================================================================
	 * VARIABLE DEFINITIONS:
	 *    isym:    Current symbol number. [i]
	 *    symsz:   Current symbol size. [f]
	 *    symgap:  Current symbol gap. [f]
	 *===================================================================== */

	/* - Set common block value to 0 and then call subroutine to set
	 *   symbol number to 1.  This subroutine sets up the other common
	 *   block variables for the current symbol number. */

	cmgtm.isym = 0;
	setsymbolnum( 1 );

	cmgtm.symsz = 0.01;
	cmgtm.symgap = 0.;

	/*=====================================================================
	 * INITIALIZATION FOR:  Axis drawing attributes.
	 *=====================================================================
	 * VARIABLE DEFINITIONS:
	 *    lxdiv:   .TRUE. if fixed x division spacing is desired. [l]
	 *    xdiv:    Fixed x division spacing. [f]
	 *    lnxdiv:  .TRUE. if fixed number of divisions is desired. [l]
	 *    nxdiv:   Fixed number of divisions. [i]
	 *    xnicsp:  Minimum spacing between "nice" divisions. [f]
	 *             This is in terms of the number of characters of the
	 *             current size that would fit between divisions.
	 *    ******   Same definitions for y axis variables.
	 *===================================================================== */

	cmgtm.lxdiv = FALSE;
	cmgtm.xdiv = 1.;
	cmgtm.lnxdiv = FALSE;
	cmgtm.nxdiv = 5;
	cmgtm.xnicsp = 5.0;

	cmgtm.lydiv = FALSE;
	cmgtm.ydiv = 1.;
	cmgtm.lnydiv = FALSE;
	cmgtm.nydiv = 5;
	cmgtm.ynicsp = 5.0;

	cmgtm.fac[0] = 0.0000;
	cmgtm.fac[1] = 0.3010;
	cmgtm.fac[2] = 0.4771;
	cmgtm.fac[3] = 0.6021;
	cmgtm.fac[4] = 0.6990;
	cmgtm.fac[5] = 0.7782;
	cmgtm.fac[6] = 0.8451;
	cmgtm.fac[7] = 0.9031;
	cmgtm.fac[8] = 0.9542;
	kmgtm.kfac[0] = '1';
	kmgtm.kfac[1] = '2';
	kmgtm.kfac[2] = '3';
	kmgtm.kfac[3] = '4';
	kmgtm.kfac[4] = '5';
	kmgtm.kfac[5] = '6';
	kmgtm.kfac[6] = '7';
	kmgtm.kfac[7] = '8';
	kmgtm.kfac[8] = '9';

	return;

} /* end of function */

