
#include <stdio.h>
#include <stdlib.h>

#include "gdm.h"

/** 
 * Plot an image.
 *
 * @param data
 *    2D array of data to plot
 * @param xloc
 *    X position to place the image, left side
 * @param yloc
 *    Y position to place the image, top side
 * @param width
 *    Width of the image
 * @param height
 *    Height of the image
 * @param nerr
 *    Error return flag
 *    - 0 on Success
 *    - Non-Zero on error
 */
void 
put_image(char **data,
          unsigned int xloc,
          unsigned int yloc,
          unsigned int width,
          unsigned int height,
          int *nerr)
{
	int idx ;

        int i, n;
        display_t **dev;

        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	*nerr = 0;
        
        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->put_image) {
            dev[i]->put_image(data[ dev[i]->id - 1 ], xloc, yloc, width, height, nerr);
          }
        }
	if ( data ) {
	    for ( idx = 0 ; idx < MGD+2 ; idx++ ) {
              if ( data[idx] ) {
                free ( data[idx] ) ;
                data[idx] = NULL;
              }
	    }

	    free ( data ) ;
            data = NULL;
	}

	return;

}


void
show_image(float *data,     /* Image Data */
           unsigned int iw, /* Image Size */
           unsigned int ih,
           float xmin,
           float xmax,
           float ymin,
           float ymax,
           float x,         /* In View space Coordinates [0,1] */
           float y,
           float w,
           float h,
           int nspeudocolors,
           int nsacolors,
           int ndefcolors,
           int lbinary,
           int *nerr) {
  
  
  int i, n;
  display_t **dev;
  
  n   = gdm_get_ndevices();
  dev = gdm_get_devices();

  for(i = 0; i < n; i++) {
    if(dev[i]->on && dev[i]->show_image) {
      dev[i]->show_image(data, iw, ih, xmin, xmax, ymin, ymax, x,y,w,h, 
                         nspeudocolors, nsacolors, ndefcolors, 
                         lbinary, nerr);
    }
  }
  

}
