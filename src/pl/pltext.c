
#include <stdio.h>
#include <string.h>

#include "pl.h"
#include "gem.h"
#include "gdm.h"

#include "bot.h"

#include "debug.h"

GEM_EXTERN

void /*FUNCTION*/
pltext(ktext, xloc, yloc)
     char *ktext;
     float xloc, yloc;
{
    int iline;
        /*=====================================================================
	 * PURPOSE:  To plot a text string at a specific location.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    ktext:   Text string. [c]
	 *    xloc:    X plot coordinate to start string at. [f]
	 *    yloc:    Y plot coordinate to start string at. [f]
	 *             xloc and yloc are in the range 0. to 1.
	 *=====================================================================
	 * MODULE/LEVEL:  pl/4
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  indexb, move, getlinestyle, setlinestyle, text,
	 *             setlinewidth.
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    gem:  ITHIN, IWIDTH
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920530:  Text is always in thin line-width.
	 *             Added gem include.
	 *    890523:  Added calls to make sure solid linestyle is used for text.
	 *    810000:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  890523
	 *===================================================================== */
    /* PROCEDURE: */
    /* - Move to plot location. */

    move(xloc, yloc);

    /* - Plot text. Make sure linestyle is solid. */

    getlinestyle(&iline);
    setlinestyle(1);
    setlinewidth(LINE_WIDTH_THIN);
    text(ktext, 0, strlen(ktext));
    setlinestyle(iline);
    setlinewidth(cmgem.iwidth);

    return;

}                               /* end of function */
