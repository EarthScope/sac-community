/** 
 * @file adj_geometry.c
 *
 * @brief Adjust the geometry of the graphics device
 *
 */ 

#include "gdm.h"

#define HT_ADJUST 32
#define WD_ADJUST 32

/** 
 * Adjust the geometry of the graphics device
 *
 * @param width
 *   Width to be adjusted
 * @param height
 *   Height to be adjusted
 * @param nerr
 *   Error return code
 *   - 0 on Success
 *   - Never Fails
 *
 */
void 
adj_geometry(unsigned int *width, 
             unsigned int *height, 
             int          *nerr) {

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();
        *nerr = 0;
        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->adjust_geometry) {
            dev[i]->adjust_geometry(width, height);
          }
        }
        /*
	if( Lgdon[2] ){
            *width /= WD_ADJUST;
            *height /= HT_ADJUST;
            *nerr = 0;
	}
	if( Lgdon[3] )
            *nerr = 0;
            */
	return;
}







