
#include "gtm.h"
#include "debug.h"


GTM_EXTERN

void
worldpolylinef(float *xi, float *yi, int n) {
    int i;
    double *x, *y;
    x = (double *) malloc(sizeof(double) * n);
    y = (double *) malloc(sizeof(double) * n);
    for(i = 0; i < n; i++) {
        x[i] = xi[i];
        y[i] = yi[i];
    }
    worldpolyline(x,y,n);
    FREE(x);
    FREE(y);
}

#define MBLOCK 100
void
worldpolyline(double *xwloc, double *ywloc, int number) {
    int j;
    int n;
        /*=====================================================================
	 * PURPOSE:  To draw a line through a set of world locations.
	 *=====================================================================
	 * SPECIAL NOTE:  This polyline subroutine does NOT conform to the
	 *                SIGGRAPH standard in that it moves to the first
	 *                data point as opposed to drawing to it from the CP.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    xwloc:   Array of x world coordinates. [fa]
	 *    ywloc:   Array of y world coordinates. [fa]
	 *    number:  Length of xwloc and ywloc arrays. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  gtm/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    831026:  Original version.
   *    140624:  Changed to use polyline
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861017
	 *===================================================================== */
    /* PROCEDURE: */

    double *x = (double *) malloc(sizeof(double) * number);
    double *y = (double *) malloc(sizeof(double) * number);

    /* Coordinate change from world to view */
    for (j = 0; j < number; j++) {
        x[j] = cmgtm.xmpwv1 * xwloc[j] + cmgtm.xmpwv2;
        y[j] = cmgtm.ympwv1 * ywloc[j] + cmgtm.ympwv2;
    }
    j = 0;
    while (j < number) {
        n = MBLOCK;
        if (j + n >= number) {
            n = number - j;
        } else {
            if (number - (j + n) < 2) {
                n -= 1;
            }
        }
        polyline(&x[j], &y[j], &n);
        j += n;
    }

    FREE(x);
    FREE(y);

    return;

}                               /* end of function */
