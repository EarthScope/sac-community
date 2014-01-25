/** 
 * @file begindevices.c
 *
 * @brief Start graphics devices 
 *
 */

#include <stdio.h>
#include <string.h>

#include "gdm.h"
#include "bool.h"
#include "gem.h"
#include "gd2.h"
#include "gpm.h"

extern print_device_begin_t  print_device_begin;
extern print_device_end_t    print_device_end;

/** 
 * Start all specified graphics devices 
 *
 *   Starts Graphics 
 *   Begins or Ends Devices 
 *   Sets the Device ON flag to TRUE
 *   Begins a Graphics Window if needed
 *   Calculate the status
 *
 * @param devices
 *    List of graphics devices to start
 *    Legal graphics devices names are:
 *       - 'SGF' for SAC Graphics File.
 *       - 'XWINDOWS' for X-window system.
 * @param devices_s
 *    Length of \p devices (string length)
 * @param ndevices
 *    Total number of devices in \p devices
 * @param nerr
 *    Error Return Code
 *    - 0 on Success
 *    - Non-Zero on Failure
 *    
 * @see begingraphics beginwindow calstatus enddevice
 *
 * @date   920604:  Added skdevfudge fudge factor (skeleton line adjustment)
 *             for device 3. Used with WIDTH option.
 * @date   910508:  Changed "device" to "devices" in call apcmsg(device).
 *             When an illegal device is requested, "device" causes 
 *             a segmentation fault, must have been a typo (wct).
 * @date   870426:  Moved window checking and creation to beginwindow.
 * @date   870127:  Added multi-windowing logic.
 * @date   861010:  Major restructuring.
 * @date   860116:  Fixed bug when a specific terminal type was entered.
 * @date   831027:  Original version.
 *
 */
void
begindevices(char *devices, 
             int   devices_s, 
             int   ndevices, 
             int  *nerr) {

#define DEVICES(I_,J_)	(devices+(I_)*(devices_s)+(J_))

	int jdevice;

        char *p;
        char name[14];

        int i, n;
        display_t **devs;
        display_t *dev;
        
        n    = gdm_get_ndevices();
        devs = gdm_get_devices();

	*nerr = 0;

	/* - Initialize the graphics library if needed. */

	if( !cmgdm.lginit ){
	    begingraphics( nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

        /* Turn off All Devices that are currently on */
        for(i = 0; i < n; i++) {
          if(devs[i]->on) {
            if(devs[i]->end_device) {
              devs[i]->end_device(nerr);
            }
          }
          devs[i]->on = FALSE;
          Lgdon[ devs[i]->id ] = FALSE;
        }

	/* - Check each input device name for correctness. */
	for( jdevice = 0; jdevice < ndevices; jdevice++ ){

          strncpy(&name[0], DEVICES(jdevice,0), devices_s);
          name[devices_s] = 0;
          p = strchr(&name[0], ' ');
          if(p) {
            *p = 0;
          }
          if((dev = gdm_get_device_by_name(name))){
            if(dev->active_device && !dev->on) {
              if(dev->begin_device) {
                dev->begin_device(nerr);
              }
              dev->on = TRUE;
              Lgdon[ dev->id ] = TRUE;
              cmgdm.igdtxt = dev->id;
            }
          } else { 
            fprintf(stderr, "SAC: Unknown device: '%s'\n", name);
          }
        }

        {
          /* Keep RECORD Device On */
          dev = gdm_get_device_by_name("RECORD");
          if(dev) {
            if(!dev->on) {
              dev->on = TRUE;
              if(dev->begin_device) {
                dev->begin_device(nerr);
              }
            }
          } else {
            fprintf(stderr, "SAC: Error finding device: Record\n");
          }
        }
	/* - Begin plotting to the current graphics window. */
	beginwindow( cmgdm.iwindow, nerr );

	/* - Calculate new values for graphics device status variables. */
	calstatus();

L_8888:
	return;

#undef	DEVICES
} 

