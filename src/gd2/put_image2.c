
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gd2.h"
#include "bool.h"
#include "co.h"
#include "gtm.h"
#include "gdm.h"
#include "xyz.h"

void 
put_image2(char *data,
           unsigned int xloc,
           unsigned int yloc,
           unsigned int width,
           unsigned int height,
           int *nerr)
{
  int nw, nchars, copycount;
  float unused, xvpmin, xvpmax, xvsmin, xvsmax, xwcmin, xwcmax;
  float xfactor, xpsize;

	/*=====================================================================
	 * PURPOSE:  To do an image plot with limited options.
	 *           Need to add options to allow user to specify size of
         *           image.  Default is the size of the raw data.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *      array:  Two-dimensional array of data. [fa]  
	 *     nxsize:  Number of elements in the x (horizontal) direction. [i]
	 *     nysize:  Number of elements in the y (vertical) direction. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error return flag.  Set to 0 if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL: contouring/5
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    gtm:         xvpmin, xvpmax, yvpmin, yvpmax
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *     saclib:  move, draw, setcolorname
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

        if(cmgd2.encodesize){
          getvport(&xvpmin, &xvpmax, &unused, &unused);
          getvspace(&xvsmin, &xvsmax, &unused, &unused);
          xfactor = (xvsmax-xvsmin)/(xvpmax-xvpmin);

          if(strncmp(kmgd2.sizetype,"FIXED",5) == 0) {
              xpsize = cmgd2.sizevalue * xfactor;
	  } else if(strncmp(kmgd2.sizetype,"SCALED",6) == 0){
              getworld(&xwcmin, &xwcmax, &unused, &unused);
              xpsize = cmgd2.sizevalue * (xwcmax-xwcmin) * xfactor;
	  } else {
              xpsize = 10.0;
	  }
          
          Mfbuf[cmgd2.jfbpnt] = MOPSIZ;
          Mfbuf[cmgd2.jfbpnt+1] = 1;
          Mfbuf[cmgd2.jfbpnt+2] = (int)fmin((100.0*xpsize),XW);
          cmgd2.jfbpnt += 3;
          cmgd2.encodesize = FALSE;
        }

        nw = (width*height*3)/2;
        if(nw*2 != (int)(width*height*3))nw++;

	Mfbuf[cmgd2.jfbpnt] = MOPCIMAGE;
	Mfbuf[cmgd2.jfbpnt + 1] = width;
	Mfbuf[cmgd2.jfbpnt + 2] = height;
	Mfbuf[cmgd2.jfbpnt + 3] = xloc;
	Mfbuf[cmgd2.jfbpnt + 4] = yloc;
        cmgd2.jfbpnt += 5;

        flushbuffer2(nerr);

        nchars = 3*width*height;
        copycount = 0;
 
/*        loop to store the data  */
        while ( (copycount+(2*JFBMAX)) <= nchars ) {
          memcpy((char *)&Mfbuf[cmgd2.jfbpnt],&data[copycount],2*JFBMAX);
          cmgd2.jfbpnt += JFBMAX;
          flushbuffer2(nerr);
          copycount += 2*JFBMAX;
	}

        if( (nchars-copycount) > 0 ) {
          memcpy((char *)&Mfbuf[cmgd2.jfbpnt],&data[copycount],nchars-copycount);
          cmgd2.jfbpnt += (nchars-copycount)/2;
          if((nchars-copycount) % 2 ) cmgd2.jfbpnt += 1;
          flushbuffer2(nerr);
	}


       
	return;

} /* end of function */



void
show_image_sgf(float *data,     
               unsigned int iw, 
               unsigned int ih,
               float xmin,
               float xmax,
               float ymin,
               float ymax,
               float x,         
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
  get_geometry2(0, &wpw, &wph, nerr);
  adj_geometry2(&wpw, &wph);

  /* Determine the Image size in Pixels */
  ipw = (float) (wpw) * w;
  iph = (float) (wph) * h;

  /* Determine the Image location in Pixels */
  ipx = (float) (XW) * x ;
  ipy = (float) (YW) * (y - h) ;

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
  cdata = fill_image2(iph, ipw, data_image, 
                      zmin, zmax - zmin, 
                      npseudocolors, nsacolors, ndefcolors, 
                      nerr);

  /* Show data */
  put_image2(cdata, ipx, ipy, ipw, iph, nerr);

  free(data_image);
  free(cdata);
  
}



