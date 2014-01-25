
#include <stdio.h>
#include <stdlib.h>

#include "gd2.h"
#include "debug.h"

#include "sgfcolor.h"

char *
fill_image2(unsigned int height, 
            unsigned int width, 
            float data[], 
            float dmin, 
            float range, 
            int npseudocolors, 
            int nsaccolors, 
            int ndefcolors, 
            int *nerr) {

  char *array;
  int i,j,isave;
  float fsave;
  float wrange;

  UNUSED(ndefcolors);
  UNUSED(nsaccolors);

  wrange = range == 0.0 ? RNDOFF: range;
 
/* allocate the output array */


  if((array  = (char *)malloc(3*width*height*sizeof(char)))
                 == NULL){
    printf("error allocating image data byte array--fill_image2\n");
    *nerr = 0301;
    goto L_8888;
  };


/*  Standard algorithm to scale data to 0-(npseudocolors-1) range */

        *nerr = 0;

        isave = 0;
        for (i = height-1; i >= 0; i--){
            for (j = 0; j < (int)width; j++)     {
          fsave = (((data[(i*width)+j] - dmin)/wrange)*(float)(npseudocolors));
          fsave = ( fsave < 0.0 ) ? 0.0 : fsave;
          fsave = ( fsave > (float)(npseudocolors-1)) ? (float)(npseudocolors-1) : fsave;
          array[isave++] = sred[(unsigned int)fsave];
          array[isave++] = sgreen[(unsigned int)fsave];
          array[isave++] = sblue[(unsigned int)fsave];
          }
        }


L_8888:
	return array;

} /* end of function */






