/** 
 * @file createwindow.c
 *
 * @brief Create a graphics window
 *
 */

#include "gdm.h"
#include "co.h"

/** 
 * Create a new graphics window 
 *
 * @param number
 *    Number of graphics window to turn on
 * @param xwinmn
 *    Minimum x location of window on screen [0, 1]
 * @param xwinmx
 *    Maximum x location of window on screen [0, 1]
 * @param ywinmn
 *    Minimum y location of window on screen [0, Ratio]
 * @param ywinmx
 *    Maximum y location of window on screen [0, Ratio]
 * @param nerr
 *    Error Return Code
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   861201:  Original version.
 *
 */
void 
createwindow(int    *number, 
             double  xwinmn, 
             double  xwinmx, 
             double  ywinmn, 
             double  ywinmx, 
             int    *nerr) {

	float xwmn, xwmx, ywmn, ywmx;

	*nerr = 0;

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	/* - Check input window number for correctness. */
	/* - Turn each window on if needed. */

        xwmn = fmax( 0.0, xwinmn );
        xwmx = fmin( 1.0, xwinmx );
        ywmn = fmax( 0.0, ywinmn );
        ywmx = fmin( 1.0, ywinmx );

        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->create_window) {
            dev[i]->create_window( number, &xwmn, &xwmx, &ywmn, &ywmx, nerr );
          }
        }

	return;

}

