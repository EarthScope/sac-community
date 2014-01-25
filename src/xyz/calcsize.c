
#include <stdio.h>

#include "xyz.h"


#include "gdm.h"

void 
calcsize(unsigned int *w_width,
         unsigned int *w_height,
         unsigned int *width,
         unsigned int *height,
         float xmax,
         float xmin,
         float last,
         float first,
         float xpmn,
         float xpmx,
         float ypmn,
         float ypmx,
         float yfactor,
         int *nerr)
{

        *nerr = 0;

        get_geometry(w_width, w_height,  nerr);
        adj_geometry(w_width, w_height, nerr);
        *width = (float)(*w_width) * (xpmx - xpmn);
        /*  adjust image width for the offset to the begin time of the spectrogram */
 
        *width = (float)(*width) * ((xmax-xmin)/(last - first));   

        *height = (float)(*w_height) * yfactor * (ypmx - ypmn);

	return;
}







