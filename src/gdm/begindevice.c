/** 
 * @file begindevice.c
 *
 * @brief Start all specified devices
 *
 */ 

#include "gdm.h"

/** 
 * Start specified graphic devices
 *
 * @param device
 *   List of graphics devices to start
 *     - 'SGF' - SAC Graphics File
 *     - 'XWINDOWS' - for X11 Window system
 * @param device_s
 *   Length of \p device
 * @param nerr
 *   Error return code
 *   - 0 on Success
 *   - Non-Zero on Failure 
 *     - 201
 *
 * @see begindevices
 *
 * @date   870416:  Reduced this subroutine to a call to BEGINDEVICES.
 * @date   870127:  Added multi-windowing logic.
 * @date   861010:  Major restructuring.
 * @date   860116:  Fixed bug when a specific terminal type was entered.
 * @date   831027:  Original version.
 *
 */
void 
begindevice(char *device, 
            int   device_s, 
            int  *nerr) {
  
  *nerr = 0;
  
  begindevices( device,device_s, 1, nerr );
  
  return;
}

