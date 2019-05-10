/** 
 * @file: xwindow.c
 *
 */

#include "gem.h"
#include "gdm.h"

#include "gd3.x11.h"

#include "bool.h"
#include "cpf.h"

GDM_EXTERN

/** 
 * WINDOW command, parse the parameter setting command
 *    Sets graphics window attributes
 *
 * @param nerr
 *    Error Return Code
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   861230:  Original version.
 *
 */
void
xwindow(int *nerr) {
    int iwin;

    double ratio;
    int ratio_on = FALSE;
    double tmp[2];

    *nerr = 0;
    iwin = 1;
    ratio = 11.0 / 8.5;
    /* - Loop on each token in command: */

  L_1000:
    if (lcmore(nerr)) {

        /* -- Set up window number. */
        if (lcirc(1, MWINDOWS, &iwin)) {

        } else if (lkrrcp("XSIZE$", 7, 0., 1., &tmp[0], &tmp[1])) {
            Xwindowmin[iwin] = tmp[0];
            Xwindowmax[iwin] = tmp[1];

        } else if (lkrrcp("YSIZE$", 7, 0., 1., &tmp[0], &tmp[1])) {
            Ywindowmin[iwin] = tmp[0];
            Ywindowmax[iwin] = tmp[1];

        } else if (lklogr("ASPECT$", 8, &ratio_on, &ratio)) {
            window_use_ratio[iwin] = ratio_on;
            if(ratio_on) {
                window_ratio[iwin] = ratio;
            }
        } else {

            cfmt("ILLEGAL OPTION:", 17);
            cresp();

        }
        goto L_1000;
    }

    return;

}
