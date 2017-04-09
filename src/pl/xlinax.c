
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "pl.h"
#include "gem.h"
#include "bool.h"
#include "proto.h"
#include "co.h"

#include "gdm.h"
#include "ucf.h"
#include "gtm.h"


GEM_EXTERN

#define	FDIVSP	5.

void
xlinax() {
    int lpower;
    char kfmt[32];
    int ia, ib, igdlog, jpower, jstep = 1, mds, nds, ntick,
        nxdivu;
    int i, j, k;
    float divlog, divtry, factor, grdlog, power, skfudge, value, valuei, xdivu,
        xgrdmn, xgrdmx, xref, xrefi, xtick, xticki, xvpmax, xvpmin, yloc, ypow = 0.0,
        yvpmax, yvpmin;
    static char kvalue[17] = "                ";
    static char kpower[9] = "        ";

        /*=====================================================================
	 * PURPOSE:  To produce a linearly-scaled axis at the bottom and/or
	 *           top of the current plot window.
	 *=====================================================================
	 * MODULE/LEVEL:  gem/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:    VSMALL
	 *    GEM:     LXDIV, XDIV, LNXDIV, NXDIV, CHHT, CHWID, LXREV,
	 *             XPMNU, XPMXU, XIMNZ, XIMXZ,
	 *             LBOTAX, LBOTTC, LTOPAX, LTOPTC,
	 *             IHORZ, IVERT, XMPIP1, XMPIP2,
	 *             LXGRD, IXGRD, IWIDTH, ISKWIDTH, ITHIN, SKDEVFUDGE
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GEM:     AXWBOT, AXWTOP
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  CNVITA, LJUST, SETLINESTYLE, LINE, PLTEXT, CNVFTA, 
	 *             SETTEXTJUST, SETLINEWIDTH, GETVPORT
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    xdivu:   Divison spacing used.
	 *    lpower:  Set to .TRUE. if there is a multiplying scale factor.
	 *    power:   Multiplying scale factor.
	 *    kpower:  Character string containing formatted scale factor.
	 *    ypow:    Y location in plot coordinates of scale factor.
	 *    divtry:  Trial division spacing.
	 *    jstep:   Integer trial step size (constrained to be 10, 5 or 2).
	 *    xgrdmn:  Minimum labeled grid value (including scale factor).
	 *    xgrdmx:  Maximum labeled grid value (including scale factor).
	 *    value:   Labeled grid value excluding scale factor.
	 *    valuei:  Increment in VALUE.
	 *    kvalue:  Character string containing formatted label value.
	 *    xref:    Location of labeled grid value in plot coordinates.
	 *    xrefi:   Increment in XREF.
	 *=====================================================================
	 * ASSUMPTIONS:
	 * - plmap has set up world to plot coordiate mapping.
	 * - Text orientation is horizontal.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920526:  Added line-width. TEXT is always line-width THIN!
	 *    830929:  Added secondary tick marks.
	 *    830927:  Moved grid drawing logic into its own do loop.
	 *    830223:  Fixed logic in computing annotation format.
	 *    820928:  Cleaned up and documented.
	 *    810120:  Original PRIME version.
	 *===================================================================== */
    /* PROCEDURE: */
    settextangle(TEXT_HORIZONTAL);

    /* - Determine division spacing.  There are three possibilities:
     *   (1) The division spacing is set by user (LXDIV=.TRUE.).
     *   (2) The (approximate) number of divsions is set (LNXDIV=.TRUE.).
     *   (3) "Nice" division spacings are calculated. */
    if (cmgem.xdiv_spacing_on) {
        xdivu = cmgem.xdiv_spacing;
        power = log10(xdivu);
        if (power < 0.)
            power = power - 1.;
        jpower = power;
    } else {
        if (cmgem.xdiv_number_on) {
            nxdivu = cmgem.xdiv_number;
        } else {
            nxdivu =
                (fabs(cmgem.uplot.xmax - cmgem.uplot.xmin) /
                 (FDIVSP * cmgem.chht)) + .001;
            if (nxdivu < 5)
                nxdivu = 5;
        }
        divtry = (cmgem.zdata.xmax - cmgem.zdata.xmin) / nxdivu;
        if (divtry > 0.) {
            power = log10(divtry);
        } else {
            power = 0.;
        }
        if (power < 0.)
            power = power - 1.;
        jpower = power;

        /* -- Limit divison spacings to steps of 10, 5, or 2 [cases (2) and (3)]. */
        jstep = divtry * (powi(10., -jpower));
        if (jstep > 5) {
            jstep = 1;
            jpower = jpower + 1;
            /* power = power + 1.; */
        } else if (jstep > 2) {
            jstep = 5;
        } else {
            jstep = 2;
        }
        xdivu = jstep * (powi(10., jpower));
    }

    /* - Determine "nice-numbered" starting and ending values. */

    ia = cmgem.zdata.xmin / xdivu;
    xgrdmn = xdivu * ia;
    if (xgrdmn < cmgem.zdata.xmin) {
        ia = ia + 1;
        xgrdmn = xgrdmn + xdivu;
    }
    ib = cmgem.zdata.xmax / xdivu;
    xgrdmx = xdivu * ib;
    if (xgrdmx > cmgem.zdata.xmax) {
        ib = ib - 1;
        xgrdmx = xgrdmx - xdivu;
    }
    nxdivu = ib - ia + 1;

    /* - Determine the format (Fn.m) of the labels.
     *   The variable NDS assumes the role of "n" and MDS the role of "m". */

    /* - The "magic numbers" used in this algorithm  generate good division
     *   spacings almost all of the time.  Modify them at your own risk. */

    grdlog = log10(fmax(fabs(xgrdmn), fabs(xgrdmx)) + 0.001);
    if (grdlog >= 0.) {
        grdlog = grdlog + 1.001;
    } else {
        grdlog = grdlog - 0.999;
    }
    divlog = log10(xdivu);
    if (divlog >= 0.) {
        divlog = divlog + 1.001;
    } else {
        divlog = divlog - 0.999;
    }
    lpower = FALSE;
    factor = 1.;
    if (grdlog * divlog >= 0.) {
        if (grdlog < 0.) {
            igdlog = grdlog;
        } else {
            igdlog = divlog;
        }
        if (labs(igdlog) >= 3 && cmgem.lxpowr) {
            mds = 0;
            nds = max(4, (int) (grdlog) - (int) (divlog) + 2);
            snprintf(kpower, sizeof(kpower), "X 10%+d", jpower);
            factor = powi(10., -jpower);
            lpower = TRUE;
        } else {
            mds = labs(minfi(0., divlog));
            nds = maxfi(1., grdlog);
            if (mds > 0)
                nds = nds + mds + 2;
        }
    } else {
        mds = labs(minfi(0., divlog));
        nds = maxfi(0., grdlog);
        if (mds > 0)
            nds = nds + mds + 2;
    }

    /* - Determine axes fudge factor for thick axes lines. */
    getvport(&xvpmin, &xvpmax, &yvpmin, &yvpmax);
    skfudge = cmgem.skdevfudge * ((yvpmin - yvpmax) / (xvpmin - xvpmax));

    /* - Draw the axes. */

    setlinestyle(LINE_STYLE_SOLID);
    setlinewidth(cmgem.iskwidth);

    for(i = 0; i < 2; i++){
        int ax = (i == 0 ) ? BOTTOM : TOP;
    if (cmgem.axis[ax].annotate || cmgem.axis[ax].ticks) {
        float x0,x1,y0,y1,tick;
        int dir;
        x0 = cmgem.uplot.xmin;
        x1 = cmgem.uplot.xmax;
        tick = cmgem.chwid;
        if(ax == BOTTOM) {
            y0 = y1 = cmgem.uplot.ymin;
            dir = +1;
        } else {
            y0 = y1 = cmgem.uplot.ymax;
            dir = -1;
        }
        /* -- Bottom Axes line. */
        if (cmgem.iskwidth > LINE_WIDTH_THIN) {
            line(x0 - cmgem.iskwidth * skfudge, y0,
                 x1 + cmgem.iskwidth * skfudge, y1);
        } else {
            line(x0,y0,x1,y1);
        }

        /* -- Label for multiplying scale factor. */
        if (lpower && cmgem.axis[ax].annotate) {
            if(ax == BOTTOM) {
                ypow = fmax(y0 - 2.2 * cmgem.chht, 0.1 * cmgem.chht);
            } else {
                ypow = fmin(y0 + 2.2 * cmgem.chht, y0 - 0.1 * cmgem.chht);
            }
            settextjust((cmgem.lxrev) ? RIGHT : LEFT, ax);
            pltext(kpower, x0, ypow);
            setlinewidth(cmgem.iskwidth);
        }

        /* -- Calculate constants for labeled tick marks. */
        value = xgrdmn * factor;
        xref = xgrdmn * cmgem.xmpip1 + cmgem.xmpip2;
        valuei = xdivu * factor;
        xrefi = xdivu * cmgem.xmpip1;
        strcpy(kvalue, "                ");

        /* -- Draw secondary tick marks before first labeled one. */
        ntick = 1;
        if (xrefi >= 0.10) {
            ntick = 3;
            if (jstep == 5)
                ntick = 4;
        }
        if (xrefi >= 0.25)
            ntick = 9;
        xticki = xrefi / (float) (ntick + 1);
        xtick = xref - xrefi;
        for (j = 1; j <= ntick; j++) {
            xtick = xtick + xticki;
            if (xtick >= x0) {
                line(xtick, y0, xtick, y0 + dir * 0.5 * tick);
            }
        }

        /* -- Loop on labeled tick marks. */
        for (k = 1; k <= nxdivu; k++) {
            line(xref, y0, xref, y0 + dir * tick);
            if (cmgem.axis[ax].annotate) {
                snprintf(kfmt, sizeof(kfmt), "%%.%df", mds);
                snprintf(kvalue, sizeof(kvalue), kfmt, value);
                yloc = y0 - dir * 0.1 * cmgem.chht;
                settextjust(CENTER, (ax == BOTTOM) ? TOP : BOTTOM);
                pltext(kvalue, xref, yloc);
                setlinewidth(cmgem.iskwidth);
            }
            /* --- Loop on secondary tick marks. */
            xtick = xref;
            for (j = 1; j <= ntick; j++) {
                xtick = xtick + xticki;
                if (xtick <= x1) {
                    line(xtick, y0, xtick, y0 + dir * 0.5 * tick);
                }
            }
            value = value + valuei;
            xref = xref + xrefi;
        }

        /* -- Save axes widths. */
        if (cmgem.axis[ax].annotate) {
            cmgem.axis[ax].width = 1.1 * cmgem.chht;
            if (lpower) {
                if(ax == BOTTOM) {
                    cmgem.axis[ax].width = y0 - ypow;
                } else {
                    cmgem.axis[ax].width = ypow - y0;
                }
            }
        } else {
            cmgem.axis[ax].width = 0.;
        }

    }
    }

    /* - Grid lines. */

    if (cmgem.lxgrd) {
        setlinewidth(LINE_WIDTH_THIN);
        xref = xgrdmn * cmgem.xmpip1 + cmgem.xmpip2;
        xrefi = xdivu * cmgem.xmpip1;
        setlinestyle(cmgem.ixgrd);
        for (j = 1; j <= nxdivu; j++) {
            line(xref, cmgem.uplot.ymin, xref, cmgem.uplot.ymax);
            xref = xref + xrefi;
        }
        setlinestyle(LINE_STYLE_SOLID);
        setlinewidth(cmgem.iskwidth);
    }

    return;

}                               /* end of function */
