/** 
 * @file endframe.c
 *
 * @brief  To the current graphics frame
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "gdm.h"
#include "gem.h"
#include "bool.h"
#include "gam.h"
#include "gd2.h"

extern print_device_begin_t  print_device_begin;
extern print_device_end_t    print_device_end;

/*
 * To end the current graphics frame.
 *
 * @param ldelay
 *    If printing [cmgem.lprint = TRUE] then delay after printing
 * @param nerr
 *    Error return flag. 
 *
 * @note  Must call beginframe to begin frame.
 *
 * @see   saclib:  endframe1, endframe2, endframe3, endframe4
 *
 * @date    2010/02/06 - Changed printing of postscript file to use mktemp
 * @date    891002:  Deleted call to ztrmlg.
 * @date    831026:  Original version.
 *
 * @date    831026:  Documented/Reviewed
 *
 */

void 
endframe(int  ldelay, 
         int *nerr) {

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	*nerr = 0;

	/* - Perform end frame action if necessary. */
	if( cmgdm.lbegf ){

          for(i = 0; i < n; i++) {
            if(dev[i]->on && dev[i]->end_frame) {
              dev[i]->end_frame( nerr );
            }
          }

	    /* Take care of PRINT option. */
	    if ( cmgem.lprint ) {
                
                sgf_print( kmgd2.kfilename, &kmgem.kptrName[0] );

                unlink(kmgd2.kfilename);

		if ( ldelay )
		    sleep( 6 ) ;

	    }

	    cmgdm.lbegf = FALSE ;
	    cmgem.lframe = cmgemsav.lframe = TRUE ;
	    cmgem.lprint = cmgemsav.lprint = FALSE ;
	    kmgem.kptrName[0] = kmgemsav.kptrName[0] = '\0' ;
        if ( cmgem.lSGFtemp ) {
            print_device_end( nerr );
        }

	}

	return;

}

