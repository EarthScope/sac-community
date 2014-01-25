
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "amf.h"
#include "gdm.h"
#include "gem.h"
#include "bool.h"
#include "xyz.h"

#include "sgfcolor.h"

#include "docs/ps.h"
#include "docs/font.h"
#include "string/array.h"

#include "debug.h"

#include "debug.h"

static ps_t *PSC = NULL;

char *record_filename(char *in);

#define XSCALE 792
#define YSCALE 612
#define LINESCALE 1
#define XADJUSTMENT 1
#define YADJUSTMENT 1

#define PS_LINE_SPACING    1.333
#define PS_FONT_SIZE_MIN  10.0

display_t ps;

static ps_color_t COLORS[] = { 
  {1.0, 1.0, 1.0},
  {1.0, 0.0, 0.0},
  {0.0, 1.0, 0.0},
  {0.0, 0.0, 1.0},
  {1.0, 1.0, 0.0},
  {0.0, 1.0, 1.0},
  {1.0, 0.0, 1.0},
  {0.0, 0.0, 0.0},
};

void begindevice_ps(int *nerr);
void beginframe_ps(int *nerr);
void changectable_ps(int nentry, int icolortable);
void draw_ps(float x, float y);
void drawpoly_ps(float *x, float *y, int n);
void endframe_ps(int *nerr);
char *fill_image_ps(unsigned int height, unsigned int width, float data[], float dmin, float range, int npseudocolors, int nsaccolors, int ndefcolors, int *nerr);
char *fill_clrbar_ps(int npseudocolors, int width, int npricolors, int ndefcolors, int *nerr);
void getwindowstat_ps(int number, int *exists);
void getratio_ps(float *aspect);
void get_geometry_ps(int number, unsigned int *width, unsigned int *height, int *nerr);
void move_ps(float x, float y);
void put_image3(char *data, unsigned int xloc, unsigned int yloc, unsigned int width, unsigned int height, int *nerr);
void setcolor_ps(int index);
void setctable_ps(int iwindow, unsigned int nentry, float red[], float green[], float blue[]);
void setlinestyle_ps(int *iline);
void setwidth_ps(int index);
void settextangle_ps(float angle);
void settextsize_ps(float width, float height);
void text_ps(display_t *out, char *text, int n);
void init_postscript(void);

void
save_ps(display_t *out, char *file) {
  record_write_file(out, file);
}

void 
begindevice_ps(int *nerr) {
  *nerr = 0;
  set_skeleton_fudge( 0.0003) ;
}

void
adjust_geometry_ps(unsigned int *w, unsigned int *h) {
  *w = *w / XADJUSTMENT;
  *h = *h / YADJUSTMENT;
}

void
beginframe_ps(int *nerr) {
  char *file;
  ps_t *ps;
  ps_color_t c;

  *nerr = 0;


  /* Open a file, currently this file may already exist */
  file = record_filename(NULL);

  ps = ps_new();
  if(!ps) {
    *nerr = 102;
    return;
  }
  if(! ps_file_open(ps, file) ) {
    *nerr = 102;
    return;
  }

  PSC = ps_current( ps );

  ps_header(ps);

  if(color_on()) {
    c = COLORS[color_background()];
    if(c.r != 1.0 || c.b != 1.0 || c.g != 1.0) {
        ps_save(ps);
        ps_color(ps, c);
        ps_rectangle(ps, XSCALE, YSCALE);
        ps_restore(ps);
    }
  }
}

void
calculate_location_ps(float *x,
                      float *y,
                      float *cx,
                      float *cy,
                      unsigned int w,
                      unsigned int h,
                      unsigned int iw,
                      unsigned int ih) {
  UNUSED(iw);
  *x = *x * XSCALE;
  *y = (1.0 - *y - (float)ih/(float)h) * YSCALE;
  *cx = (*cx / (float) w) * XSCALE;
  *cy = ((ih - cmgdm.npscimage)/(float)h) * YSCALE;
}


void
draw_ps(float x, float y) {
  ps_lineto(PSC,  x * XSCALE, y * YSCALE);
}

void
drawpoly_ps(float *x, float *y, int n) {
  int i;
  ps_line_join_style(PSC, PS_LINE_JOIN_STYLE_ROUND);
  move_ps(x[0], y[0]);
  for(i = 1; i < n; i++) {
    ps_lineto(PSC, x[i] * XSCALE, y[i] * YSCALE);
  }
}
void
fillpoly_ps(float *x, float *y, int n) {
  int i;
  ps_line_join_style(PSC, PS_LINE_JOIN_STYLE_ROUND);
  move_ps(x[0], y[0]);
  for(i = 1; i < n; i++) {
    ps_lineto(PSC, x[i] * XSCALE, y[i] * YSCALE);
  }
  ps_fill(PSC);
}

void
endframe_ps(int *nerr) {
  UNUSED(nerr);
  ps_footer(PSC);
  ps_close(PSC);
}

char * 
fill_image_ps(unsigned int height,
              unsigned int width,
              float data[],
              float dmin,
              float range,
              int npseudocolors,
              int nsaccolors,
              int ndefcolors,
              int *nerr) {
  int i,j,k;
  char *array;
  float v;
  UNUSED(nerr);
  UNUSED(ndefcolors);
  UNUSED(nsaccolors);

  array = (char *) malloc(sizeof(char) * 3 * width * height);
  k = 0;
  for(i = height-1; i >= 0; i--) {
      for(j = 0; j < (int)width; j++) {
      v = ((data[(i*width) + j] - dmin) / range) * (float) npseudocolors;
      v = ( v < 0.0 ) ? 0.0 : v ;
      v = ( v > (float)npseudocolors - 1) ? (float)(npseudocolors - 1) : v;
      array[k++] =   sred[(unsigned int) v];
      array[k++] = sgreen[(unsigned int) v];
      array[k++] =  sblue[(unsigned int) v];
    }
  }
  return array;
}

char * 
fill_clrbar_ps (
    int npseudocolors,   /* number of pseudocolors in the colortable */
    int width,       /* number of elements in one scan line of the color bar */
    int npricolors,  /* number of SAC primary colors */
    int ndefcolors,  /* number of default colors in the colortable */
    int *nerr) {

  int i,j,k,n;
  char *array;
  UNUSED(nerr);
  UNUSED(ndefcolors);
  UNUSED(npricolors);


  array = (char *) malloc(3 * width * npseudocolors * sizeof(char));
  k = npseudocolors - 1;
  n = 0;
  for(i = 0; i < npseudocolors; i++) {
    for(j = 0; j < width; j++) {
      array[n++] =   sred[k];
      array[n++] = sgreen[k];
      array[n++] =  sblue[k];
    }
    k--;
  }
  return array;
}

void
getwindowstat_ps(int number, int *exists) {
  UNUSED(number);
  *exists = FALSE;
}

void
getratio_ps(float *aspect) {
  *aspect = 1.00;
}

void
get_geometry_ps(int number, 
                unsigned int *width,
                unsigned int *height,
                int *nerr) {
  UNUSED(nerr);
  UNUSED(number);
  *width  = XSCALE;
  *height = YSCALE;
}

void
move_ps(float x, float y) {
  ps_moveto(PSC, x * XSCALE, y * YSCALE);
}

void 
put_image_ps(char *data,
           unsigned int xloc,
           unsigned int yloc,
           unsigned int width,
           unsigned int height,
           int *nerr) {
    UNUSED(nerr);

    ps_image(PSC, data, (float)xloc, (float)yloc, width, height);
}


void
show_image_ps(float *data,
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
  
  DEBUG("size: [%d %d] %d\n", iw, ih, iw * ih);

  /* Get the size of the window in pixels */
  get_geometry_ps(0, &wpw, &wph, nerr);
  adjust_geometry_ps(&wpw, &wph);

  /* Determine the Image size in Pixels */
  ipw = (float) (wpw) * w;
  iph = (float) (wph) * h;

  /* Determine the Image location in Pixels */
  ipx = (float) (wpw) * x * XADJUSTMENT;
  ipy = (float) (wph) * (y - h) * YADJUSTMENT;

  DEBUG("size:  [%d %d]\n", wpw, wph);
  DEBUG("size: [%d %d] %d\n", ipw, iph, ipw*iph);
  DEBUG("loc:  [%d %d]\n", ipx, ipy);

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
  cdata = fill_image_ps(iph, ipw, data_image, 
                        zmin, zmax - zmin, 
                        npseudocolors, nsacolors, ndefcolors, 
                        nerr);

  /* Show data */
  put_image_ps(cdata, ipx, ipy, ipw, iph, nerr);

  free(data_image);
  free(cdata);
}

void
setcolor_ps(int index) {
  ps_color_t c = COLORS[index];
  if(PSC && PSC->fp) {
    ps_color(PSC, c);
  }
}

void
line_style_ps(char *line, void *data) {
    char *c;
    array_t *ps = (array_t *) data;
    //asprintf(&c, "/ls%d {[ %s ]} def\n", array_length(ps)+1, line);
    asprintf(&c, "[ %s ] 0 setdash\n", line);
    array_append(ps, strdup(c));
    FREE(c);
}

void
setlinestyle_ps(int *iline) {
  int k;
  static array_t *dash = NULL;
  k = *iline ;
  if(k <= 0) {
    k = 1;
  }
  if(!dash) {
      dash = array_new();
      array_append(dash, strdup("[] 0 setdash"));
      sac_line_style_read(line_style_ps, dash);
  }
  /*ps_raw(PSC, "xls%d\n", k);*/
  ps_raw(PSC, "%s\n", array_element(dash, k-1));
}

void
setwidth_ps(int index) {
  ps_linewidth(PSC,  LINESCALE * index);
}

void
textbox_ps(textbox *t) {
  int i;
  float w, h, size, x, y, sx, sy;
  int height, len, xlen;

  if(!t || t->n == 0) {
    return;
  }

  gettextsize(&w, &h);
  size = h * 10 * 72;
  
  x = t->x * XSCALE;
  y = t->y * YSCALE;

  height = string_height("Helvetica", size) * PS_LINE_SPACING;
  len = 0;
  if(t->location & TEXT_BOX_RIGHT) {
    for(i = 0; i < t->n; i++) {
      xlen = string_width("Helvetica", t->text[i], size);
      if(xlen > len) {
        len = xlen;
      }
    }
    x = x - len;
  }
  if(t->location & TEXT_BOX_LOWER) {
    y += height * (t->n - 1);
  } else {
    y -= height;
  }

  if(t->use_style && t->location & TEXT_BOX_LEFT) {
      x += 0.5 * 1.25 * string_average_width("Helvetica", size) + 
          TEXTBOX_LINE_LENGTH_PIXELS;
  }
  

  ps_save(PSC);
  for(i = 0; i < t->n; i++) {
    ps_moveto(PSC, x, (y - height * i) );
    setcolor_ps( t->color[i] );
    ps_textsize(PSC, size );
    ps_textshow(PSC, t->text[i]);
    sx = x / XSCALE;
    sy = (y - height * i) / YSCALE;
    if(t->use_style) {
      text_box_line(&ps, sx, sy, t->width[i], t->style[i],
                    1.25 * string_average_width("Helvetica", size)/(float)XSCALE,
                    (height / 2.0) / (float)YSCALE);
    }
    if(t->use_symbol) {
      text_box_symbol(&ps, sx, sy, t->symbol[i], t->use_style,
                      1.25 * string_average_width("Helvetica", size)/(float)XSCALE,
                      (height / 2.0) / (float)YSCALE);                      
    }
  }
  ps_restore(PSC);
}

void
text_ps(display_t *out, char *text, int n) {
  char c;
  float w, h, size;
  UNUSED(out);

  gettextsize(&w, &h);
  size = h * 10 * 72;

  c = text[n];
  text[n] = 0;

  DEBUG("text: %s size: %f \n", text, size);

  size = (size < PS_FONT_SIZE_MIN) ? PS_FONT_SIZE_MIN : size;

  ps_newline(PSC);
  ps_save(PSC);
  ps_textsize(PSC, size );
  ps_rotate(PSC,  gettextangle() );
  ps_textjustify(PSC, text, cmgdm.ihjust, cmgdm.ivjust, size);
  ps_textshow(PSC, text);
  ps_restore(PSC);
  text[n] = c;
}

void
stroke_ps(ps_t *ps) {
  UNUSED(ps);
  ps_stroke(PSC);
}


static char *ps_name = "PS";
static char *ps_ext  = "ps";

void
initdevice_postscript() {
  
  initdevice_null( &ps );

  ps.name               = ps_name;
  ps.extension          = ps_ext;
  ps.id                 = PS;
  ps.adjust_geometry    = adjust_geometry_ps;
  ps.begin_device       = begindevice_ps;
  ps.begin_frame        = beginframe_ps;
  ps.calc_loc           = calculate_location_ps;
  ps.draw               = draw_ps;
  ps.drawpoly           = drawpoly_ps;
  ps.fillpoly           = fillpoly_ps;
  ps.end_frame          = endframe_ps;
  ps.get_window_status  = getwindowstat_ps;
  ps.get_ratio          = getratio_ps;
  ps.get_device_ratio   = getratio_ps;
  ps.get_geometry       = get_geometry_ps;
  ps.move               = move_ps; 
  ps.put_image          = put_image_ps;
  ps.set_color          = setcolor_ps;
  ps.set_line_style     = setlinestyle_ps;
  ps.set_line_width     = setwidth_ps;
  ps.stroke             = stroke_ps;
  ps.text               = text_ps;  
  ps.textbox            = textbox_ps;
  ps.show_image         = show_image_ps;
  ps.save               = save_ps;
  gdm_register_device( &ps );
}


