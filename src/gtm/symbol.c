
#include <math.h>

#include "gtm.h"
#include "gdm.h"
#include "co.h"

void /*FUNCTION*/ symbol(xloc, yloc, number, lnewdp)
float xloc[], yloc[];
int number;
int lnewdp;
{
	int j1, j2, j2_;
	float curgap, xcur, ycur;
	static float xsave, ysave;

	float *const Xloc = &xloc[0] - 1;
	float *const Yloc = &yloc[0] - 1;


	/*=====================================================================
	 * PURPOSE: To plot symbols at a set of viewport locations.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xloc:    Array of X viewport locations. [ra]
	 *    yloc:    Array of Y viewport locations. [ra]
	 *    number:  Number of X-Y locations. [i]
	 *    lnewdp:  .TRUE. if this is the beginning of a new data set.
	 *             .FALSE. if this is a continuation of the last data set.
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    gtm:     isym, symsz, symgap, munsym
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  draw, move
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    xsave:    Last X location. [f]
	 *    ysave:    Last Y location. [f]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861027:  Moved from psym.
	 *    850610:  Moved new symbol logic to ssymnu.
	 *    850524:  Added check to make sure symbol plotting is requested.
	 *    810722:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861027
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Set last point to large value if this is a new line segment. */
	if( lnewdp ){
		xsave = 100.;
		ysave = 100.;
		}

	/* - For each input data point: */

	for( j1 = 1; j1 <= number; j1++ ){

		/* -- Move current point to local storage. */
		xcur = Xloc[j1];
		ycur = Yloc[j1];

		/* -- Check spacing between current and last plotted point. */
		curgap = sqrt( powi(xcur - xsave,2) + powi(ycur - ysave,2) );
		if( curgap >= cmgtm.symgap ){
			xsave = xcur;
			ysave = ycur;

			/* -- Loop on each stroke in symbol. */
			for( j2 = cmgtm.jsym1b; j2 <= cmgtm.jsym1e; j2++ ){
				j2_ = j2 - 1;
				/* --- Scaling logic. */
				if( cmgtm.lscsym ){
					xcur = xcur + cmgtm.symsz*cmgtm.xsymtb[j2_];
					ycur = ycur + cmgtm.symsz*cmgtm.ysymtb[j2_];
					}
				else{
					xcur = xcur + cmgtm.xsymtb[j2_];
					ycur = ycur + cmgtm.ysymtb[j2_];
					}

				/* --- Draw/move logic. */
				if( cmgtm.ldrsym[j2_] ){
					draw( xcur, ycur );
					}
				else{
					move( xcur, ycur );
					}
				}
                        stroke();
			/* -- Draw second part of a double symbol. */
			if( cmgtm.ldbsym ){
				/* --- Move back to original PC. */
				xcur = xsave;
				ycur = ysave;
				move( xcur, ycur );
				/* --- Loop on each stroke in symbol. */
				for( j2 = cmgtm.jsym2b; j2 <= cmgtm.jsym2e; j2++ ){
					j2_ = j2 - 1;
					/* ---- Scaling logic. */
					if( cmgtm.lscsym ){
						xcur = xcur + cmgtm.symsz*cmgtm.xsymtb[j2_];
						ycur = ycur + cmgtm.symsz*cmgtm.ysymtb[j2_];
						}
					else{
						xcur = xcur + cmgtm.xsymtb[j2_];
						ycur = ycur + cmgtm.ysymtb[j2_];
						}
					/* ---- Draw/move logic. */
					if( cmgtm.ldrsym[j2_] ){
						draw( xcur, ycur );
                                        }
					else{
						move( xcur, ycur );
                                        }
                                }
                                stroke();
                        }
                }
        }

	return;

} /* end of function */

void
symbol_single(display_t *out, float x, float y) {
  int i;
  for(i = cmgtm.jsym1b; i <= cmgtm.jsym1e; i++) {
    if(cmgtm.lscsym) {
      x = x + cmgtm.symsz * cmgtm.xsymtb[i-1];
      y = y + cmgtm.symsz * cmgtm.ysymtb[i-1];
    } else {
      x = x + cmgtm.xsymtb[i-1];
      y = y + cmgtm.ysymtb[i-1];
    }
    if(cmgtm.ldrsym[i-1]) {
      out->draw(x, y);
    } else {
      out->move(x, y);
    }
  }
  if(out->stroke) {
    out->stroke();
  }
}
