

#include "config.h"

#include "proto.h"
/* include png.h needs to go before everything, except config.h
 *   older versions will fail if anything that include setjmp.h 
 *   before png.h
 */
#ifdef HAVE_PNG
#include <png.h>
#endif /* HAVE_PNG */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amf.h"
#include "bool.h"
#include "gdm.h"
#ifdef USE_X11
#include "gd3.x11.h"
#endif

#ifdef HAVE_XPM
#include <X11/xpm.h>
#endif /* HAVE_XPM */

void xpm_write(display_t *out, char *file);
void png_write(display_t *out, char *file);

#ifdef HAVE_XPM

void
xpm_write(display_t *out, char *file) {
  int retval;
  XWindow *xw;
  UNUSED(out);
  xw = plot_window( CURRENT );
  if(!xw) {
    fprintf(stderr, "Writing XPM file requires an open X11 Window\n");
    return;
  }

  retval = XpmWriteFileFromPixmap(DISPLAY(xw),
                                  file,
                                  xw->buffer,
                                  (Pixmap)NULL,
                                  NULL);
  if(retval != 0) {
    fprintf(stderr, "Error writing XPM file: %s [%d]\n", file, retval);
  }
}

#else 

void
xpm_write(display_t *out, char *file) {
  fprintf(stdout, 
          "sac: Writing to XPM files not supported in this version\n"
          "     This functionality can be compiled into SAC\n");
}

#endif /* HAVE_XPM */

#ifdef HAVE_PNG

void
user_error_fn(png_structp png_ptr, png_const_charp msg) {
  fprintf(stderr, "Error writing png: %s\n", msg);
  UNUSED(png_ptr);
}
void
user_warning_fn(png_structp png_ptr, png_const_charp msg) {
  fprintf(stderr, "Error writing png: %s\n", msg);
  UNUSED(png_ptr);
}

void
png_pack_16(png_bytep buf, unsigned int i) {
  buf[0] = (png_byte) ((i>>8) & 0xff); 
  buf[1] = (png_byte) (i & 0xff);      
}

#define PNG_SAMPLE_DEPTH 2 /* PNG Sample size in bytes for 16bpp */
#define RGB_COLORS       3 /* Red, Green and Blue Colors */

int
get_pixel_shift(unsigned long mask) {
    unsigned long int tmp;
    unsigned long int shift;
    shift = 0;
    tmp = 0x000000ff;
    while( mask != tmp) {
        shift += 8;
        tmp = tmp << 8;
        if(shift > 128) {
            exit(-1);
        }
    }
    return shift;
}

void
png_write(display_t *out, char *file) {
  FILE *fp;

  png_structp png_ptr;
  png_infop info_ptr;
  png_byte **image;
  png_bytep *row_ptrs;
  int i,j,n;
  XImage *img;
  XWindow *xw;
  unsigned short r, g, b;
  unsigned long int red_mask, green_mask, blue_mask;
  int red_shift, blue_shift, green_shift;
  unsigned long int pixel;

  UNUSED(out);

  png_ptr  = NULL;
  info_ptr = NULL;
  image    = NULL;
  row_ptrs = NULL;

  xw = plot_window( CURRENT );
  if(!xw) {
    fprintf(stderr, "Writing PNG Image file requires an open X11 window\n");
    return;
  }

  blue_mask  = VISUAL(xw)->blue_mask; 
  green_mask = VISUAL(xw)->green_mask; 
  red_mask   = VISUAL(xw)->red_mask; 
  red_shift = get_pixel_shift( red_mask );
  green_shift = get_pixel_shift( green_mask );
  blue_shift = get_pixel_shift( blue_mask );

  img = XGetImage(DISPLAY(xw), xw->buffer, 0,0,
                  xw->width, xw->height,
                  AllPlanes, ZPixmap);

  row_ptrs = (png_bytep *) malloc(sizeof(png_bytep) * img->height);
  if(!row_ptrs) {
      fprintf(stderr, "Allocating memory for PNG Image file\n");
      goto ERROR;
  }
  image   =  (png_byte **) malloc(sizeof(png_byte *) * img->height);
  if(!image) {
      fprintf(stderr, "Allocating memory for PNG Image file\n");
      goto ERROR;
  }
  memset(image, 0, sizeof(png_byte *) * img->height);

  for(i = 0; i < img->height; i++) {
    image[i] = (png_byte *) malloc(sizeof(png_byte) * RGB_COLORS * PNG_SAMPLE_DEPTH * img->width);

    if(!image[i]) {
        fprintf(stderr, "Allocating memory for PNG Image file\n");
        goto ERROR;
    }
    for(j = 0; j < img->width; j++) {
      n = j * RGB_COLORS * PNG_SAMPLE_DEPTH;

      pixel = XGetPixel(img, j, i);

      /* 
         pixel    = 0xAARRGGBB
         RED_MASK = 0x00FF0000
         RED_SHIFT = 16
         pixel & RED_MASK = 0x00RR0000
         (pixel & RED_MASK) >> RED_SHIFT = 0x000000RR
         (pixel & RED_MASK) >> RED_SHIFT => (short) 0x00RR
         (pixel & RED_MASK) >> RED_SHIFT => (short) << 8 = 0xRR00
       */
      r = ((pixel & red_mask)   >> red_shift);
      g = ((pixel & green_mask) >> green_shift);
      b = ((pixel & blue_mask)  >> blue_shift);
      r = r << 8;
      g = g << 8;
      b = b << 8;
      png_pack_16(&image[i][n+0], r);
      png_pack_16(&image[i][n+2], g);
      png_pack_16(&image[i][n+4], b);
    }
    row_ptrs[i] = (png_bytep) image[i];
  }
  
  if(!(fp = fopen( file, "wb"))) {
    fprintf(stderr, "Error opening file writing: %s\n", file);
    goto ERROR;
  }
  
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                    NULL,
                                    user_error_fn,
                                    user_warning_fn);
  if(!png_ptr) {
    fprintf(stderr, "Error allocating space to write png (png_ptr)\n");
    goto ERROR;
  }
  
  info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr) {
    fprintf(stderr, "Error allocating space to write png (info_ptr)\n");
    goto ERROR;
  }

  if( setjmp(png_jmpbuf( png_ptr )) ) {
    fprintf(stderr, "Error writing png\n");
    fclose(fp);
    goto ERROR;
  }

  png_init_io(png_ptr, fp);
  png_set_IHDR(png_ptr, info_ptr, 
               img->width,
               img->height,
               16, 
               PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);
  /* 
  fprintf(stderr, "info_ptr->rowbytes: %d %d %d\n", info_ptr->rowbytes, info_ptr->width, info_ptr->rowbytes/info_ptr->width);
  fprintf(stderr, "info_ptr->bpp: %d %d\n", info_ptr->bit_depth, info_ptr->pixel_depth);
  */
  png_write_info(png_ptr, info_ptr);

  
  png_write_image(png_ptr, row_ptrs);
  png_write_end(png_ptr, info_ptr);
  fclose(fp);


 ERROR:  
  /* Release Memory */
  FREE(row_ptrs);
  if(image) {
      for(i = 0; i < img->height; i++) {
          FREE(image[i]);
      }
      FREE(image);
  }
  if(png_ptr) {
      png_destroy_write_struct(&png_ptr, &info_ptr);
  }

}

#else

void
png_write(display_t *out, char *file) {
  UNUSED(out);
  UNUSED(file);
  fprintf(stdout, 
          "sac: Writing to png files not supported in this version\n"
          "     This functionality can be compiled into SAC\n");
}

#endif /* HAVE_PNG */
