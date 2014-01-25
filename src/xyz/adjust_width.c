
#include "xyz.h"

/* output_image is assumed to be width_out*height int */

void 
adjust_width(float *input_image,
             unsigned int width,
             unsigned int height,
             float *output_image,
             unsigned int width_out,
             float xmin,
             float xmax,
             int *nerr)
{
  int i;

  /* use the linear interpolation routine to adjust data width */

  for( i=0; i<(int)height; i++){
    linear_interp(&input_image[i*width], xmin, xmax, (int)width,
                &output_image[i*width_out], (int)width_out, nerr);
  } 

  return;
}
