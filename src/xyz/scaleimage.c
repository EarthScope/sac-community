
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xyz.h"
#include "bool.h"

void 
scaleimage(float *input_image,
           unsigned int width,
           unsigned int height,
           float *output_image,
           unsigned int width_out,
           unsigned int height_out,
           float xstart,
           float xstop,
           float ystart,
           float ystop,
           int *nerr)
{
  float *temp_image;
  int w_alloc = FALSE;

  *nerr = 0;


  if(width != width_out){

    if((temp_image = (float *)malloc(width_out*height*sizeof(float))) == NULL) {
      printf("error allocating memory--scaleimage\n");
      *nerr = 0301;
      goto L_8888;
    }

    w_alloc = TRUE;

    adjust_width(input_image, width, height, temp_image, width_out,  
                 xstart, xstop, nerr);

    if( *nerr != 0 ){
      printf("error adjusting image width--scaleimage\n");
      *nerr = 1;
      goto L_8888;
    }
  } else {
      temp_image = input_image;
  }

  if(height != height_out){

    adjust_height(temp_image, width_out, height, output_image, height_out, 
                  ystart, ystop, nerr);

    if( *nerr != 0 ){
      printf("error adjusting image height--scaleimage\n");
      *nerr = 1;
      goto L_8888;
    }

  } else {
    /* copy to output image array */
    memcpy((char *)output_image, (char *)temp_image, width_out*height_out*sizeof(float));
  }
  
L_8888:
  if (w_alloc) free(temp_image);


  return;
  
  }


