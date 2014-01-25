
#include <stdlib.h>

#include "gdm.h"
#include "msg.h"

/** 
 * Fill a colorbar array
 *
 * @param npseudocolors
 *     Number of pseudocolors in the colortable
 * @param width
 *     Number of elements (width) in one scan line of the color bar 
 * @param npricolors
 *     Number of SAC primary colors 
 * @param ndefcolors
 *     Number of default colors in the colortable 
 * @param nerr
 *     Error return flag
 *     - 0 on Success
 *     - Non-Zero on Error
 *       - 301 Error Allocating the output array
 */
char **
fill_colorbar (
    int npseudocolors,   
    int width,       
    int npricolors,  
    int ndefcolors,  
    int *nerr )
{
    char **array;

    int i, n;
    display_t **dev;
    n   = gdm_get_ndevices();
    dev = gdm_get_devices();
        
    array = NULL;

    array = (char **) calloc ( MGD+2 , sizeof ( char * ) ) ;
    if ( array == NULL ) {
	*nerr = 301 ;
	setmsg ( "ERROR" , *nerr ) ;
	outmsg () ;
	goto L_8888 ;
    }

    for(i = 0; i < n; i++) {
      if(dev[i]->on && dev[i]->fill_colorbar) {
        array[ dev[i]->id - 1 ] = dev[i]->fill_colorbar(npseudocolors,
                                                    width,
                                                    npricolors,
                                                    ndefcolors,
                                                    nerr);
      }
    }

L_8888:
    return array;

}

