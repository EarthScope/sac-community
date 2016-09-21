
#include <stdio.h>
#include <stdlib.h>

#include "gdm.h"

GDM_EXTERN

#define	IABOVE	8
#define	IBELOW	4
#define	ILEFT	1
#define	INSIDE	0
#define	IRIGHT	2

/** 
 * Move the the requested viewport point
 *
 * Lower left hand corner is (0.0, 0.0)
 * Upper right hand corner is (1.0, 1.0)
 *
 * @param xloc
 *    X viewport coordinate
 * @param yloc
 *    Y viewport coordinate
 *
 */
void
move(float xloc, float yloc) {
    int iloc;
    float xloc1, yloc1;

    int i, n;
    display_t **dev;
    n = gdm_get_ndevices();
    dev = gdm_get_devices();

    xloc1 = xloc;
    yloc1 = yloc;

    /* - If viewspace clipping is off, send move to active devices. */
    if (!cmgdm.lvsclip) {
        for (i = 0; i < n; i++) {
            if (dev[i]->on && dev[i]->move) {
                dev[i]->move(xloc1, yloc1);
            }
        }
    }

    /* - If viewspace clipping is on: */
    else {
        /* -- Determine location of data point relative to viewport. */
        iloc = INSIDE;
        if (yloc > Yvs[2]) {
            iloc = iloc + IABOVE;
        } else if (yloc < Yvs[1]) {
            iloc = iloc + IBELOW;
        }
        if (xloc > Xvs[2]) {
            iloc = iloc + IRIGHT;
        } else if (xloc < Xvs[1]) {
            iloc = iloc + ILEFT;
        }

        /* -- Move to location if inside viewport. */
        if (iloc == INSIDE) {
            for (i = 0; i < n; i++) {
                if (dev[i]->on && dev[i]->move) {
                    dev[i]->move(xloc1, yloc1);
                }
            }
        }

        cmgdm.iold = iloc;
    }

    /* - Save current point. */

    cmgdm.xold = xloc;
    cmgdm.yold = yloc;

    return;

}

void
set_position(float x, float y) {
    cmgdm.xold = x;
    cmgdm.yold = y;
}

void
get_position(float *x, float *y) {
    *x = cmgdm.xold;
    *y = cmgdm.yold;
}
