/** 
 * @file beginframe.c
 *
 * @brief Begin a new graphics frame
 *
 */

#include <string.h>

#include "gdm.h"
#include "gem.h"
#include "bool.h"
#include "gam.h"
#include "co.h"
#include "gd2.h"

extern print_device_begin_t  print_device_begin;
extern print_device_end_t    print_device_end;

/** 
 * Begin plotting a new graphics frame
 *
 *   Must call endframe when finished
 *
 * @param lprint
 *    - TRUE print the frame when finished
 *    - FALSE do not print
 * @param nerr
 *    Error Return Code
 *    - 0 on Succes
 *    - Non-Zero on Failure
 *
 * @date   891002:  Deleted call to ztrmlg.
 * @date   831026:  Original version.
 */
void 
beginframe(int lprint, 
           int *nerr) {
	int lany;
    int j;
    char devs[1024*9];
	*nerr = 0;

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	/* - If no graphics device is open, try to open the terminal. */

	getstatus( "ANY", &lany );
	if( !lany ){
	    begindevice( kmgam.kgddef,9, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

	/* - End last frame if not already done. */

	if( cmgdm.lbegf ){
	    endframe( FALSE , nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

	/* - If requested, establish the print option. */
	if ( lprint ) {
	    cmgem.lprint = TRUE ;

	    /* SGF device needs to be on:  there are four cases:
	     *  1) SGF is already on:  do nothing.
	     *  2) Some other device is on:  turn SGF on temporarily.
	     *  3) No devices on; SGF is default:  turn on SGF.
	     *  4) No devices on; SGF is not default:  turn on default &
	     *                                         turn on SGF temporarily.
	     */
        memset(devs, 0, 1024*9);
        j = 0;
        for(i = 0; i < n; i++) {
            if(dev[i]->on || dev[i]->id == SGF) {
                memset(devs+j, ' ', 9);
                memcpy(devs+j, dev[i]->name, strlen(dev[i]->name));
                j = j + 9;
                devs[j] = 0;
            }
            if(dev[i]->id == SGF && ! dev[i]->on) {
                cmgem.lSGFtemp = TRUE;
                print_device_begin(nerr);
            }
        }
        begindevices(devs, 9, (int)(j/9), nerr);
	} 
	/* - Perform begin frame action for each active device. */
        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->begin_frame) {
            dev[i]->begin_frame( nerr );
          }
        }

	/* - Calculate viewspace which might have changed since last frame. */
	calvspace();

	/* - Set begin frame flag. */
	cmgdm.lbegf = TRUE;

L_8888:
	return;

}

