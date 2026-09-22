
#include <string.h>

#include "gem.h"
#include "gam.h"
#include "gdm.h"
#include "bool.h"
#include "cpf.h"
#include "gd3.x11.h"

GEM_EXTERN
GAM_EXTERN
GDM_EXTERN

char *font_systems[] = { "SOFTWARE", "CORE\0\0\0\0", "XFT\0\0\0\0\0" };

char *fonts[] = { "HELVETICA   ",
    "TIMES-ROMAN ",
    "COURIER     ",
    "ZAPFDINGBATS"
};

void /*FUNCTION*/
xgt(int *nerr)
{
    int lhardw;
    int index;
    int i;

        /*=====================================================================
	 * PURPOSE:  To execute the parameter-setting command GTEXT.
	 *           This command defines certain graphic text attributes.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL: gem/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gem:     kgtqua, igtfnt
	 *             tsdef, tsaxis, tstitl, tsxlab, tsylab, tsplab
	 *    gam:     tsfid, tspk
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - All text size attributes are initialized by INIGEM, INIGAM, etc.
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:    lcmore, cfmt, cresp, lkint, lklist
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900312:  Fixed bug in setting text type.
	 *    841102:  Changed names of several options.
	 *    840621:  Added ability to set text height/width ratio.
	 *    820817:  Changed to newest set of parsing and checking functions.
	 *    820325:  Added option to set default character size.
	 *    811013:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900312
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;
    i = -1;
    index = -1;
    /* PARSING PHASE: */

    /* - Loop on each token in command: */

  L_1000:
    if (lcmore(nerr)) {

        /* -- "HARDWARE|SOFTWARE": define new text quality. */
        if (lclog2("HARDWARE$", 10, "SOFTWARE$", 10, &lhardw)) {
            if (lhardw) {
                strcpy(kmgem.kgtqua, "HARDWARE");
            } else {
                strcpy(kmgem.kgtqua, "SOFTWARE");
            }
            settexttype(kmgem.kgtqua);

            /* -- "FONT n":  define new software text font. */
        } else if (lklist("SYS#TEM$", 9, (char *) font_systems[0], 9, 3, &i)) {
            if (i >= 0) {
#ifdef X11_APP
                xwindow_set_font_system(i - 1);
#endif
            }
        } else if (lklist("NAME$", 6, (char *) fonts[0], 13, 4, &i)) {
            if (i >= 0) {
#ifdef X11_APP
                xwindow_set_font_base(i - 1);
#endif
            }
        } else if (lkint("FONT$", 6, &cmgem.igtfnt)) {

            /* -- "FORCE": Force Hardware fonts */
        } else if (lckey("FOR#CE$", 8)) {
            cmgdm.lfhard = TRUE;
            strcpy(kmgem.kgtqua, "HARDWARE");
            settexttype(kmgem.kgtqua);

            /* -- "SIZE TINY|SMALL|MEDIUM|LARGE":  set new default char. size. */
        } else
            if (lklist("SI#ZE$", 7, (char *) kmgem.ktxsiz, 9, MTXSIZ, &index)) {
            cmgem.tsdef = cmgem.txsiz[index - 1];
            cmgem.tsaxis = cmgem.tsdef;
            cmgem.title.text_size = cmgem.tsdef;
            cmgem.xlabel.text_size = cmgem.tsdef;
            cmgem.ylabel.text_size = cmgem.tsdef;
            cmgam.tsfid = cmgem.tsdef;
            cmgam.tspk = cmgem.tsdef;
            cmgem.tsplab[0] = cmgem.tsdef;

            /* -- Bad syntax. */
        } else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();

        }
        goto L_1000;

    }

    /* - The above loop is over when one of two conditions has been met:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
     *   (2) All the tokens in the command have been successfully parsed. */

    return;

}                               /* end of function */
