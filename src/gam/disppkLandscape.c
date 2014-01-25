#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "hdr.h"
#include "lhf.h"
#include "gem.h"
#include "gam.h"
#include "co.h"

#include "gdm.h"
#include "pl.h"
#include "bot.h"
#include "ucf.h"
#include "gtm.h"

void disppkLandscape(tdelay)
double tdelay;
{
	char kpktxt[9];
	int j, j_;
	float xploc, xploc1, xploc2, xtloc, xwloc, xpdel, yploc, 
	 yploc1, yploc2, ytloc, ywloc;


	/* ind
	 *=====================================================================
	 * PURPOSE: To display any defined time picks in the current
	 *          subplot window, in landscape mode of prs.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    TDELAY:  Time offset to add to time picks before converting to plot
	 *             coordinates.  Used when files have been shifted relative
	 *             to each other in some of the more complicated plot formats.
	 *=====================================================================
	 * MODULE/LEVEL:  GAM/3
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    HDR:     FHDR, KHDR, FUNDEF, KUNDEF
	 *    LHF:     MTM, ITMKRF, ITMFNM, KFHDR
	 *    GAM:     IPKTYP(), PKWDTH, PKHGTH
	 *    GEM:     XPMNU, XPMXU, YPMNU, YPMXU, CHHT,
	 *             THWRAT, LWIDTH, IWIDTH, ITHIN
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  SETTEXTSIZE, LINE PLTEXT GETXW
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    961219:  Original version; copied from disppk.c,  maf.
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Return if this option has been turned off */
	if( !cmgam.ldsppk )
		goto L_8888;

	/* - Change to the smallest text size. */

	cmgem.chht = cmgam.tspk;
	cmgem.chwid = cmgem.txrat*cmgem.chht;
	settextsize( cmgem.chwid, cmgem.chht );

	/* - Determine length of various line segments used in pick display. */

	xpdel = cmgem.uplot.xmax - cmgem.uplot.xmin;
	xploc1 = cmgem.uplot.xmax - 0.05*xpdel;
	xploc2 = cmgem.uplot.xmin + 0.05*xpdel;

	/* - Loop on each time field in header: */

	for( j = 1; j <= MTM; j++ ){
		j_ = j - 1;

		/* -- If time pick is defined and pick display is not off: */
		if( Fhdr[Itmfnm[j]] != cmhdr.fundef && Ipktyp[j] > 0 ){

			/* --- Map the input y location in WC to PC. */
			ywloc = Fhdr[Itmfnm[j]] + tdelay;
			yploc = cmgem.ympip1*ywloc + cmgem.ympip2;

			/* --- If time pick is within y plot window: */
			if( yploc >= cmgem.uplot.ymin && yploc <= cmgem.uplot.ymax ){
				/* ---- Determine time pick text: either pick id (KTn) or pick name. */
				if( memcmp(kmhdr.khdr[cmlhf.itmkrf + j_ - 1],kmhdr.kundef,
				 strlen(kmhdr.kundef)) != 0 ){
					strcpy( kpktxt, kmhdr.khdr[cmlhf.itmkrf + j_ - 1]
					  );
					}
				else{
					strcpy( kpktxt, kmlhf.kfhdr[Itmfnm[j] - 1] );
					}
				/* ---- Display a horizontal line, a vertical line or a cross at pick.
				 *      Also display time pick text at appropriate location. */
				setlinewidth( LINE_WIDTH_THIN );
				if( Ipktyp[j] == 1 ){
					setlinewidth( cmgem.iwidth );
					line( xploc1, yploc, xploc2, yploc );
					setlinewidth( LINE_WIDTH_THIN );
					pltext( kpktxt,9, xploc2 + 0.005, yploc + 0.005 );
					}
				else{
					yploc = cmgem.ympip1*ywloc + cmgem.ympip2;
					getxw( Fhdr[Itmfnm[j]], &xwloc );
					xploc = cmgem.xmpip1*xwloc + cmgem.xmpip2;
					yploc1 = fmax( cmgem.uplot.ymin, yploc - 0.5*cmgam.pkwdth );
					yploc2 = fmin( cmgem.uplot.ymax, yploc + 0.5*cmgam.pkwdth );
					setlinewidth( cmgem.iwidth );
					line( xploc, yploc1, xploc, yploc2 );
					if( Ipktyp[j] == 3 ){
						xploc1 = fmax( cmgem.uplot.xmin, xploc - 0.5*cmgam.pkhgth );
						xploc2 = fmin( cmgem.uplot.xmax, xploc + 0.5*cmgam.pkhgth );
						line( xploc1, yploc, xploc2, yploc );
						}
					setlinewidth( LINE_WIDTH_THIN );
					ytloc = yploc;
					if( (xploc - cmgem.uplot.xmin) > 0.5*xpdel ){
						xtloc = xploc + 0.005;
						}
					else{
						xtloc = xploc - cmgem.chht - 0.005;
						}
					pltext( kpktxt,9, xtloc, ytloc );
					}
				}
			}
		}

L_8888:
	return;

} /* end of function */

