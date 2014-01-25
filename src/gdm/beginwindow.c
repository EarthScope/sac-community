/** 
 * @file beginwindow.c
 *
 * @brief Begin a Plotting window
 *
 */

#include "co.h"
#include "gdm.h"
#include "gam.h"

/** 
 * Begin a Plotting Window
 *
 * @param number 
 *    Number of the graphics window to turn on
 * @param nerr
 *    Error Return Code
 *    - 0 on success
 *    - Non-Zero on Failure
 *
 * @date   900829;  Fixed bug created by last bug fix, call setctable with
 *                  nctsize+1. [wct]
 * @date   900817:  Fixed bug created by last change. [jet]
 * @date   900803:  Changed method of setting color table for new window. 
 * @date   870426:  Moved window checking and creation from begindevices.
 * @date   861201:  Original version.
 */
void 
beginwindow(int  number, 
            int *nerr) {

	int exists;
	float screenratio, ymin, ymax;

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	*nerr = 0;

	/* - Check input window number for correctness. */
	cmgdm.iwindow = max( 1, min( MWINDOWS, number ) );
    
	/* - Create a new graphics window if necessary. */
	getwindowstatus( &cmgdm.iwindow, &exists );
	if( !exists ){
		getdeviceratio( &screenratio );
                for(i = 0; i < n; i++) {
                  if(dev[i]->on && dev[i]->get_window_size) {
                    dev[i]->get_window_size( & Xwindowmin[cmgdm.iwindow],
                                             & Xwindowmax[cmgdm.iwindow],
                                             & Ywindowmin[cmgdm.iwindow],
                                             & Ywindowmax[cmgdm.iwindow]);
                  }
                }
                ymin = Ywindowmin[cmgdm.iwindow] * screenratio;
                ymax = Ywindowmax[cmgdm.iwindow] * screenratio;
                
		createwindow( &cmgdm.iwindow, 
                              Xwindowmin[cmgdm.iwindow], 
                              Xwindowmax[cmgdm.iwindow], 
                              ymin, ymax, nerr );
		if( *nerr != 0 )
			goto L_8888;


                for(i = 0; i < n; i++) {
                  if(dev[i]->on && dev[i]->set_color_table) {
                    dev[i]->set_color_table(cmgdm.iwindow, 
                                            cmgdm.nctsize + 1, 
                                            &cmgdm.ctred[0], 
                                            &cmgdm.ctgreen[0], 
                                            &cmgdm.ctblue[0]);
                  }
                }
        }
	/* - Activate the graphics window for each active device. */

        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->begin_window ) {
            dev[i]->begin_window( &cmgdm.iwindow, nerr );
          }
        }

	/* - Set default color if window did not exist.
	 *   Must be done after beginwindow, at least for Sunwindows. */

	if( !exists )
		setcolor( cmgdm.nctsize );

	/* - Calculate new values for graphics status variables. */

	calstatus();

L_8888:
	return;

        }

