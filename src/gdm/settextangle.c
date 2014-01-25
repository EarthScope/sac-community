
#include <stdio.h>
#include <stdlib.h>

#include "gdm.h"


void
settextangle_internal(float angle) {
  cmgdm.tangle = angle;
}

float 
gettextangle() {
  return cmgdm.tangle;
}

/** 
 * Set the text angle
 *
 * @param angle
 *    Angle in degrees counter-clockwise from horizontal
 *
 * @date   861017:  Original version.
 *
 */
void 
settextangle(float angle) {

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	/* - Save value passed in. */
        settextangle_internal(angle);

        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->set_text_angle) {
            dev[i]->set_text_angle( cmgdm.tangle );
          }
        }
}

