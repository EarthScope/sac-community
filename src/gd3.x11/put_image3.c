
/** 
 * Place a image into an X11 Window
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gd3.x11.h"
#include "gdm.h"
#include "xyz.h"
#include "dff.h"

#include "proto.h"

#include "config.h"


/** 
 * Place a image into an X11 window 
 *
 * @param data
 *    Image data, encoding 
 * @param xloc
 *    Image X Position
 * @param yloc
 *    Image Y position
 * @param width
 *    Image Width
 * @param height
 *    Image Height
 * @param nerr
 *    Error return flag
 *      - 0 on Success
 *      - Non-Zero on Error
 *
 */
void 
put_image3(char *data,
           unsigned int xloc,
           unsigned int yloc,
           unsigned int width,
           unsigned int height,
           int *nerr)
{

   XImage *image;
   XWindow *xw;

   xw = plot_window( CURRENT );

   /* PROCEDURE: */
   *nerr = 0;
   
   /* create the image structure. */
   
   if(!data) {
     fprintf(stderr, "put_image3(): Data does not exist\n");
     return;
   }

#ifdef USE_X11_MULTIPLE_DEPTHS
	/* 
	   Bitmap Pad     Depth
	   8 bits         8bpp
	   16 bits        16bpp
	   32 bits        24bpp
	   32 bits        32bpp
	*/
	{
	  int depth;
	  int bitmap_pad;

	  depth = DEPTH(xw);

	  if(depth > 16) 
	    bitmap_pad = 32;
	  else if(depth > 8)
	    bitmap_pad = 16;
	  else
	    bitmap_pad = 8;

	  image =  XCreateImage(DISPLAY(xw), 
                            VISUAL(xw), 
                            depth, 
                            ZPixmap, 0, data, 
                            width, height, 
                            bitmap_pad, 0);
      /* Make the X Server and Machine Byte Orders Match */
      if(image->byte_order == MSBFirst && 
         CheckByteOrder()  == ENDIAN_LITTLE) {
          image->byte_order       = LSBFirst;
          image->bitmap_bit_order = LSBFirst;
      }
      if(image->byte_order == LSBFirst && 
         CheckByteOrder()  == ENDIAN_BIG) {
          image->byte_order       = MSBFirst;
          image->bitmap_bit_order = MSBFirst;
      }
	}

	XPutImage(DISPLAY(xw), xw->buffer, xw->gc,
		  image, 0, 0, xloc, yloc, width, height);
#else /* USE_X11_MULTIPLE_DEPTHS */
        image =  XCreateImage(DISPLAY(xw),
                              VISUAL(xw),
                              8, ZPixmap, 0, NULL, width,
                              height, 8, 0);
        image->data = data;

        XPutImage(DISPLAY(xw), 
                  xw->win, xw->gc,
                  image, 0, 0, xloc, yloc, width, height);
#endif /* USE_X11_MULTIPLE_DEPTHS */

        XFlush( DISPLAY(xw) );

        /* destroy the image structure */
        XDestroyImage(image);
        data = NULL;

	return;

}


void
show_image_x11(float *data,
               unsigned int iw, /* Size of the image in data points */
               unsigned int ih,
               float xmin, /* Limits of the data */ 
               float xmax,
               float ymin,
               float ymax,
               float x,  /* In View space Coordinates [0,1] */
               float y,
               float w,
               float h,
               int npseudocolors,
               int nsacolors,
               int ndefcolors,
               int lbinary,
               int *nerr) {

  unsigned int wpw, wph;
  unsigned int ipw, iph;
  unsigned int ipx, ipy;
  float zmin, zmax;
  char *cdata;
  float *data_image;
  
  /* Get the size of the window in pixels */
  get_geometry3(c_win3, &wpw, &wph, nerr);

  /* Determine the Image size in Pixels */
  ipw = (float) (wpw) * w;
  iph = (float) (wph) * h;

  /* Determine the Image location in Pixels */
  ipx = (float) (wpw) * x;
  ipy = (float) (wph) * (1.0 - y);

  /* Scale Image from data [iw,ih] to output [ipw, iph] */
  data_image = (float *) malloc(sizeof(float) * ipw * iph);

  scaleimage(data, iw, ih, 
             data_image, ipw, iph, 
             xmin, xmax, 
             ymin, ymax, 
             nerr);

  /* Flatten to +- binary if wanted */
  if(lbinary) {
    fbinary(data_image, ipw * iph);
  }

  /* Determine the Z-Range of the image */
  frange(data_image, ipw * iph, &zmin, &zmax);

  /* Convert float data into color data */
  cdata = fill_image3(iph, ipw, data_image, 
                      zmin, zmax - zmin, 
                      npseudocolors, nsacolors, ndefcolors, 
                      nerr);

  /* Show data 
     put_image3() will free cdata through XDestroyImage
  */
  put_image3(cdata, ipx, ipy, ipw, iph, nerr);

  free(data_image);

}
