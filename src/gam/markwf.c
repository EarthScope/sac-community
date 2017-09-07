#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gdm.h"
#include "gam.h"

#include "gtm.h"
#include "bot.h"

void
markwf(int jmark1, int jmark2, double *xloc1, double *xloc2, double ytop, double ydel, char *klabel, int klabel_s) {
    int jmark, nc;
    double yloc1, yloc2;

    /* - Set text justification. */
    settextjust(RIGHT, TOP);

    /* - Draw and label marker lines on requested subplots. */

    for (jmark = jmark1; jmark <= jmark2; jmark++) {
        yloc1 = ytop - ((float) (jmark - 1) + 0.05) * ydel;
        yloc2 = yloc1 - 0.90 * ydel;
        rectangle(xloc1, xloc2, &yloc1, &yloc2);
        nc = indexb(klabel, klabel_s);
        if (nc > 0) {
            move(*xloc2 - 0.005, yloc1 - 0.005);
            text(klabel, klabel_s, nc);
        }
    }

    /* - Reset text justification. */

    settextjust(LEFT, BOTTOM);

    return;

}                               /* end of function */
