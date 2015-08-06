/** 
 * @file   xmap.c
 * 
 * @brief  Draw an array map
 * 
 */

#include <string.h>
#include <math.h>

#include "fks.h"
#include "bool.h"
#include "hdr.h"
#include "gam.h"
#include "gem.h"
#include "dfm.h"
#include "co.h"
#include "amf.h"

#include "gdm.h"
#include "pl.h"
#include "gtm.h"
#include "cpf.h"
#include "dff.h"
#include "array.h"

GAM_EXTERN
GEM_EXTERN
FKS_EXTERN

void
xmap(int *nerr) {

    int lany, lformer, lfullsav;
    int i, j, jdfl, nch, nchsav;
    float ratiosav, square, *x, xmax, xmin, xp, *y, ymax, ymin, yp;
    sac *s;

        /*=====================================================================
	 * PURPOSE:  To draw a map of the array, or its coarray.
	 *
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:   fks/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    gem:     lframe
	 *    gam:     kgddef
	 *    dfm:     ndfl
	 *=====================================================================
	 * GLOBAL OUTPUT:  (none)
	 *=====================================================================
	 * SUBROUTINES CALLED:  (to be updated)
	 *    saclib:  lcmore, lclog2, cfmt, cresp, vflist, 
	 *             plhome, endframe
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    901201:  Port from the version in XAP, which has
	 *             DLM (in src hdr) 860624, and DLM (in dir) 891016. JYio
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;
    x = xarray_new_with_len('f', saclen());
    y = xarray_new_with_len('f', saclen());
    /* PARSING PHASE: */

    /* - Loop on each token in command: */

  L_1000:
    if (lcmore(nerr)) {

        if (lclog2("A#RRAY$", 8, "C#OARRY$", 9, &lformer)) {
            if (lformer) {
                strcpy(cmfks.kmaptype, "ARRAY   ");
            } else {
                strcpy(cmfks.kmaptype, "COARRAY ");
            }

            /* -- Bad syntax. */
        } else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();

        }
        goto L_1000;

    }

    if (*nerr != 0)
        goto L_9999;

    /* - The above loop is over when one of 3 conditions has been met:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
     *   (2) When a token for the desired binary argument is encountered,
     *        for the first time.
     *   (3) All the tokens in the command have been successfully parsed. */

    /* CHECKING PHASE: */

    /* - Check for null data file list. */

    vflist(nerr);
    if (*nerr != 0)
        goto L_9999;
    nch = saclen();

    /* DATA INPUT PHASE: */

    nchsav = nch;
    s = NULL;
    for (jdfl = 1; jdfl <= nch; jdfl++) {
        if (!(s = sacget(jdfl - 1, FALSE, nerr))) {
            goto L_9999;
        }
        //getfil( jdfl, FALSE, &nlen, &ndxy, &ndxx, nerr );

        if (s->h->iftype != ITIME) {
            nchsav = jdfl - 1;
            goto L_2001;
        }
        x[jdfl - 1] = s->h->user7;
        y[jdfl - 1] = s->h->user8;
    }
  L_2001:
    nch = nchsav;

    /* INITIALIZE GRAPH PARAMETERS
     *
     *  Find array element at maximum distance from origin                           
     * */
    if (strcmp(cmfks.kmaptype, "ARRAY   ") == 0) {

        s->h->dist = 0.;
        for (i = 1; i <= nch; i++) {
            square = powi(x[i - 1], 2) + powi(y[i - 1], 2);
            if (powi(s->h->dist, 2) < square) {
                s->h->dist = sqrt(square);
            }
        }

    } else if (strcmp(cmfks.kmaptype, "COARRAY ") == 0) {

        s->h->dist = 0.;
        for (i = 1; i <= nch; i++) {
            for (j = 1; j <= nch; j++) {
                square =
                    powi(x[i - 1] - x[j - 1], 2) + powi(y[i - 1] - x[j - 1], 2);
                if (powi(s->h->dist, 2) < square) {
                    s->h->dist = sqrt(square);
                }
            }
        }

    }

    /* GRAPHICS PHASE: */

    /* - If no graphics device is open, try to open the default device. */

    getstatus("ANY", &lany);
    if (!lany) {
        begindevice(kmgam.kgddef, 9, nerr);
        if (*nerr != 0)
            goto L_9999;
    }

    /* - Save current plot environment. */

    getvspacetype(&lfullsav, &ratiosav);
    plsave();
    setvspacetype(FALSE, 1.0);

    /* - Begin new frame if requested. */

    if (cmgem.lframe) {
        beginframe(FALSE, nerr);
        if (*nerr != 0)
            goto L_8888;
    }

    /*  Set world coordinate system window.                                          
     * */
    getvspace(&xmin, &xmax, &ymin, &ymax);
    setvport(xmin, xmax, ymin, ymax);
    setworld(-1.2, 1.2, -1.2, 1.2);

    /*BKH    CALL GETRATIO(ASPECT)                                                   
     *BKH    CALL SETVP( FIXED_VIEWPORT )                                            
     *BKH    XD = .6                                                                 
     *BKH    YD = .8 * ASPECT                                                        
     *BKH    SIDE = AMIN1(XD,YD)                                                     
     *BKH    XL = .5*(1.0-SIDE)                                                      
     *BKH    XR = XL + SIDE                                                          
     *BKH    YB = .5*(ASPECT-SIDE)                                                   
     *BKH    YT = YB + SIDE                                                          
     *BKH    CALL SETVPC( XL, XR, YB, YT )                                           
     *BKH    CALL SETWC(-1.0, 1.0, -1.0, 1.0)                                        
     *
     *  Add axes to plot                                                             
     * */
    setcolor_index(3);
    worldmove(-.85, 0.05);
    worlddraw(.85, 0.05);
    worldmove(0., -.80);
    worlddraw(0., .90);

    /*  Add scale at bottom of plot                                                  
     * */
    s->h->scale = .001;
  L_4:
    ;
    if (10.0 * s->h->scale > s->h->dist || s->h->scale > 9.999)
        goto L_5;
    s->h->scale = s->h->scale * 10.;
    goto L_4;
  L_5:
    ;
    yp = -0.90;
    xp = (s->h->scale / s->h->dist / 2.) * 0.80;
    worldmove(-xp, yp);
    worlddraw(xp, yp);
    worldmove(xp, yp - 0.01);
    worlddraw(xp, yp + 0.01);
    worldmove(-xp, yp - 0.01);
    worlddraw(-xp, yp + 0.01);

    /*  Add labels to axes                                                           
     * */
    settextsize(0.014, 0.021);
    settextfont(2);
    settexttype("SOFTWARE");
    settextjust(LEFT, BOTTOM);
    setcolor_index(2);
    worldmove(0.03, 0.865);
    text("NORTH", 6, 5);

    worldmove(0.88, .0285);
    text("EAST", 5, 4);

    /*  Label scale                                                                  
     * */
    settextjust(CENTER, CENTER);
    if (s->h->scale > .099 && s->h->scale < .101) {

        worldmove(0.0, -1.0);
        text("100 METERS", 11, 10);

    } else if (s->h->scale > .999 && s->h->scale < 1.001) {

        worldmove(0.0, -1.0);
        text("1 KILOMETER", 12, 11);

    } else {

        worldmove(0.0, -1.0);
        text("10 KILOMETERS", 14, 13);

    }

    /*  Label entire plot                                                            
     * */
    if (strcmp(cmfks.kmaptype, "ARRAY   ") == 0) {

        worldmove(0.0, 0.985);
        text("MAP OF ARRAY ELEMENT LOCATIONS", 31, 30);

    } else if (strcmp(cmfks.kmaptype, "COARRAY ") == 0) {

        worldmove(0.0, 0.985);
        text("MAP OF COARRAY SAMPLE LOCATIONS", 32, 31);

    }

    /*  Plot location of each array element                                          
     * */
    setcolor_index(1);

    if (strcmp(cmfks.kmaptype, "ARRAY   ") == 0) {

        for (i = 1; i <= nch; i++) {
            xp = (x[i - 1] / s->h->dist) * 0.80;
            yp = (y[i - 1] / s->h->dist) * 0.80 + 0.05;
            worldsector(xp, yp, .01, 0., 360., 5.);
        }

    } else if (strcmp(cmfks.kmaptype, "COARRAY ") == 0) {

        for (i = 1; i <= nch; i++) {
            for (j = 1; j <= nch; j++) {
                xp = ((x[i - 1] - x[j - 1]) / s->h->dist) * 0.80;
                yp = ((y[i - 1] - y[j - 1]) / s->h->dist) * 0.80 + 0.05;
                worldsector(xp, yp, .01, 0., 360., 5.);
            }
        }

    }

    /* - Home cursor and end frame if requested. */

    plhome();
    if (cmgem.lframe)
        endframe(FALSE, nerr);
    else
        flushbuffer(nerr);
    /* - Restore plot environment and return. */

  L_8888:
    plrest();
    settextjust(LEFT, BOTTOM);
    setvspacetype(lfullsav, ratiosav);

  L_9999:
    xarray_free(x);
    xarray_free(y);
    return;

}                               /* end of function */
