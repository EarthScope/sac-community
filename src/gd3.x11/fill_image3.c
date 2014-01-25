
#include <stdio.h>
#include <stdlib.h>

#include "mach.h"
#include "gd3.x11.h"

#include "config.h"

char * 
fill_image3(unsigned int height,
            unsigned int width,
            float data[],
            float dmin,
            float range,
            int npseudocolors,
            int nsaccolors,
            int ndefcolors,
            int *nerr)
{

  int i, j;
  float fsave,fmin,fmax;
  int isave;
  char *array;
  float wrange;

  XWindow *xw;


#ifdef USE_X11_MULTIPLE_DEPTHS
  unsigned short *array2;
  unsigned int *array4;
  int depth;
  int pixel_size;

  xw = plot_window( CURRENT );

  depth = DEPTH(xw);
  if(depth > 16) 
    pixel_size = 4;
  else if(depth > 8)
    pixel_size = 2;
  else 
    pixel_size = 1;
  
#endif

  wrange = range == 0.0 ? RNDOFF: range;

#ifndef USE_X11_MULTIPLE_DEPTHS
  array = (char *) malloc(width * height * sizeof(char));
#else
  array = (char *) malloc(width * height * pixel_size);
 
  array4 = (unsigned int *)   array;
  array2 = (unsigned short *) array;

#endif
  if(array == NULL) {
    printf("error allocating image data byte array--fill_image3\n");
    *nerr = 0301;
    goto L_8888;
  }

/*  Standard algorithm to scale data to (cmgdm.nctsize-(cmgdm.nctsize+npscolors-1)) range */
  
        *nerr = 0;

        fmin = (float)nsaccolors+1.0+(float)ndefcolors;
        fmax = (float)(nsaccolors+npseudocolors+ndefcolors);
        isave = 0;
        for (i = height-1; i >= 0; i--){
            for (j = 0; j < (int)width; j++)     {
          fsave = (((data[(i*width)+j] - dmin)/wrange)*(float)(npseudocolors))+(float)nsaccolors+1.0+(float)ndefcolors;
          fsave = fsave < fmin ? fmin : fsave;
          fsave = fsave > fmax ? fmax : fsave;
#ifdef USE_X11_MULTIPLE_DEPTHS
	  if(depth > 16) 
	    array4[isave++] = pixdef3[(unsigned int)fsave].pixel;
	  else if(depth > 8)
	    array2[isave++] = pixdef3[(unsigned int)fsave].pixel;
	  else 
#endif	  
          array[isave++] = pixdef3[(unsigned int)fsave].pixel;
          }
        }
        
L_8888:
	return array;

} /* end of function */






