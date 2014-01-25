
#include <stdlib.h>

#include "gdm.h"
#include "msg.h"

/** 
 * Fill and color and image from a data array 
 * 
 * @param height
 *    Height of output image and \p data
 * @param width
 *    Width of output image and \p data
 * @param data
 *    Input data array of size width * height
 * @param dmin
 *    Minimum value of data array
 * @param range
 *    Range of data, not the maximum value
 * @param npseudocolors
 *    Number of pseudocolors in the colortable
 *    These are the colars used to color the image
 * @param nsacolors
 *    Number of SAC primary colors
 *    Used as an offset in the colortable
 * @param ndefcolors
 *    Number of default colors in the colortable
 *    Used as an offset in the colortable
 * @param nerr
 *    Error return flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *       - 301 Error allocating space for image 
 *
 */
char **
fill_image (
    unsigned int height,
    unsigned int width,
    float data[],  /* data array  */
    float dmin,
    float range,
    int npseudocolors,
    int nsaccolors,
    int ndefcolors,
    int *nerr
)

{
    char **array;
    int i, n;
    display_t **dev;
    n   = gdm_get_ndevices();
    dev = gdm_get_devices();

    array = NULL ;

    array = (char **) calloc ( MGD+2 , sizeof ( char * ) ) ;
    if ( array == NULL ) {
	*nerr = 301 ;
	setmsg ( "ERROR" , *nerr ) ;
	outmsg () ;
	goto L_8888 ;
    }
    for(i = 0; i < MGD+2; i++) {
      array[i] = NULL;
    }

    for(i = 0; i < n; i++) {
      if(dev[i]->on && dev[i]->fill_image) {
        array[ dev[i]->id - 1 ] = dev[i]->fill_image(height, 
                                                     width, 
                                                     data, 
                                                     dmin, 
                                                     range,
                                                     npseudocolors, 
                                                     nsaccolors, 
                                                     ndefcolors, 
                                                     nerr);
      }
    }

L_8888:
	return array;

}






