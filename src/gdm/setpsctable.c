
#include "gdm.h"
#include "gam.h"

#define DOINITS
#include "color.h"
#include "sgfcolor.h"
#undef DOINITS

/** 
 * Set the Pseudo Color Table
 *
 * @param nerr
 *    Error return flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   900829;  Fixed bug created by last bug fix, call setctable with
 *                  nctsize+1. [wct]
 * @date   900817:  Fixed bug created by last change. [jet]
 * @date   900803:  Changed method of setting color table for new window. 
 * @date   870426:  Moved window checking and creation from begindevices.
 * @date   861201:  Original version.
 *
 */
void 
setpsctable(int *nerr)
{

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	*nerr = 0;

        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->set_pseudo_color_table) {
            dev[i]->set_pseudo_color_table( &cmgdm.iwindow, 
                                            cmgdm.nctsize + 1, 
                                            &cmgdm.ctred[0], 
                                            &cmgdm.ctgreen[0], 
                                            &cmgdm.ctblue[0] );
          }
        }
}

