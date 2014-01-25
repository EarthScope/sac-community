
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gdm.h"
#include "gem.h"
#include "bool.h"
#include "dff.h"

#include "sgfcolor.h"
#include "debug.h"

#define XSCALE 32000
#define YSCALE 24000

display_t record;
display_t xtext;
display_t xpm;
display_t png;

void png_write(display_t *out, char *file);
void xpm_write(display_t *out, char *file);

static FILE *text_fp;

static float current_ratio = 1.0;

typedef struct _record_object_t record_object_t;
typedef struct _record_beginframe_t record_beginframe_t;
typedef struct _record_color_t record_color_t;
typedef struct _record_endframe_t record_endframe_t;
typedef struct _record_draw_t record_draw_t;
typedef struct _record_drawpoly_t record_drawpoly_t;
typedef struct _record_image_t record_image_t;
typedef struct _record_move_t record_move_t;
typedef struct _record_width_t record_width_t;
typedef struct _record_store_t record_store_t;
typedef struct _record_stroke_t record_stroke_t;
typedef struct _record_style_t record_style_t;
typedef struct _record_text_t record_text_t;
typedef struct _record_textbox_t record_textbox_t;
typedef struct _record_t record_t;

typedef void (*free_t) (record_object_t *obj);
typedef void (*play_t) (record_object_t *obj, display_t *out);

record_t *Record = NULL;

enum {
  RECORD_STROKE,
  RECORD_MOVE,
  RECORD_DRAW,
  RECORD_IMAGE,
  RECORD_DRAWPOLY,
  RECORD_FILLPOLY,
  RECORD_COLOR,
  RECORD_TEXT,
  RECORD_TEXTBOX,
  RECORD_BEGINFRAME,
  RECORD_ENDFRAME,
  RECORD_WIDTH,
  RECORD_STYLE,
};

record_t        * record_new  ();
void              record_free(record_t *r);
int               record_add  (record_t *r, record_object_t *obj);
record_object_t * record_draw (float x, float y);
record_object_t * record_move (float x, float y);
record_object_t * record_color(int index);
record_object_t * record_width(int index);
record_object_t * record_style(int index);
record_object_t * record_stroke();
record_object_t * record_beginframe();
record_object_t * record_endframe();
record_object_t * record_text(char *text, int n);
record_object_t * record_textbox(textbox *tbox);
void              record_play(record_t *r, display_t *out);
record_object_t * record_drawpoly(float *x, float *y, int n);
record_object_t * record_fillpoly(float *x, float *y, int n);
void              record_object_init(record_object_t *r, 
                                     int type, 
                                     play_t p, 
                                     free_t f);
record_object_t * record_image(float *data,
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
                               int lbinary);

void record_write_file(display_t *out, char *file);


struct _record_object_t {
  int type;
  free_t free;
  play_t play;
};

struct _record_beginframe_t {
  record_object_t base;
  int color;
};

struct _record_stroke_t {
  record_object_t base;
};
struct _record_endframe_t {
  record_object_t base;
};

struct _record_width_t {
  record_object_t base;
  int width;
};
struct _record_style_t {
  record_object_t base;
  int style;
};

struct _record_color_t {
  record_object_t base;
  int index;
};

struct _record_draw_t {
  record_object_t base;
  float x;
  float y;
};

struct _record_drawpoly_t {
  record_object_t base;
  int n;
  float *x;
  float *y;
};

struct _record_move_t {
  record_object_t base;
  float x;
  float y;
};

struct _record_textbox_t {
  record_object_t base;
  textbox *tbox;
};

struct _record_text_t {
  record_object_t base;
  char  *text;
  int    n;
  int    vert;
  int    horiz;
  float  width;
  float  height;
  float  angle;
};

struct _record_image_t {
  record_object_t base;
  float *data;
  int nx;
  int ny;
  float xmin, xmax, ymin, ymax;
  float x, y, w, h;
  int npseudocolors;
  int nsacolors;
  int ndefcolors;
  int lbinary;
};

struct _record_store_t {
  int n;
  int nalloc;
  record_object_t **data;
};

struct _record_t {
  record_store_t *objs;
};

#define OBJ(r) ((record_object_t *) r)

#define ADD_OR_FREE(to, obj) do {          \
    if(obj) {                              \
        if(!to || !record_add(to, obj)) {  \
            obj->free(obj);                \
        }                                  \
    }                                      \
} while(0);

typedef struct _color color;
struct _color {
  float r,g,b;
};

/*
static color COLORS[] = { 
  {1.0, 1.0, 1.0},
  {1.0, 0.0, 0.0},
  {0.0, 1.0, 0.0},
  {0.0, 0.0, 1.0},
  {1.0, 1.0, 0.0},
  {0.0, 1.0, 1.0},
  {1.0, 0.0, 1.0},
  {0.0, 0.0, 0.0},
};
*/

void begindevice_record(int *nerr);
void beginframe_record(int *nerr);
void changectable_record(int nentry, int icolortable);
void draw_record(float x, float y);
void drawpoly_record(float *x, float *y, int n);
void fillpoly_record(float *x, float *y, int n);
void endframe_record(int *nerr);
void getwindowstat_record(int number, int *exists);
void getratio_record(float *aspect);
void get_geometry_record(int number, unsigned int *width, unsigned int *height, int *nerr);
void move_record(float x, float y);
void setcolor_record(int index);
void setctable_record(int iwindow, unsigned int nentry, float red[], float green[], float blue[]);
void setlinestyle_record(int *iline);
void setwidth_record(int index);
void settextangle_record(float angle);
void settextsize_record(float width, float height);
void text_record(display_t *out, char *text, int n);

char *
record_filename(char *in) {
  static char *filename = NULL;
  if(in) { /* Set filename if requested */
    if(filename) {
      free(filename);
      filename = NULL;
    }
    if(*in) {
      filename = strdup(in);
    }
  }
  return filename;
}

/*
void
sgf_write_file(char *file) {
  record_filename(file);
  record_play(Record, &sgf);
}

void
x11_replay() {
  record_play(Record, &x11);
}

void
txt_write_file(char *file) {
  record_filename(file);
  record_play(Record, &xtext);
}
void
ps_write_file(char *file) {
  record_filename(file);
  record_play(Record, &ps);
}
void
pdf_write_file(char *file) {
  record_filename(file);
  record_play(Record, &_pdf);
}
*/

void
record_write_file(display_t *out, char *file) {
  record_filename(file);
  record_play(Record, out);
  record_filename("");
}

char *
record_type(int type) {
  switch(type) {
  case RECORD_STROKE:     return "Stoke"; break;
  case RECORD_MOVE:       return "Move"; break;
  case RECORD_DRAW:       return "Draw"; break;
  case RECORD_IMAGE:      return "Image"; break;
  case RECORD_DRAWPOLY:   return "Drawpoly"; break;
  case RECORD_COLOR:      return "Color"; break;
  case RECORD_TEXT:       return "Text"; break;
  case RECORD_TEXTBOX:    return "TextBox"; break;
  case RECORD_BEGINFRAME: return "Beginframe"; break;
  case RECORD_ENDFRAME:   return "Endframe"; break;
  case RECORD_WIDTH:      return "Width"; break;
  case RECORD_STYLE:      return "Style"; break;
  default:
    return "Unknown Type";
  }
  return "Unknown Type";
}

void 
begindevice_record(int *nerr) {
  *nerr = 0;
  set_skeleton_fudge( 0.0003) ;
}

void
adjust_geometry_record(unsigned int *w, unsigned int *h) {
  UNUSED(w);
  UNUSED(h);
}

void
beginframe_record(int *nerr) {

  *nerr = 0;

  if(Record) {
    record_free(Record);
    Record = NULL;
  }

  Record = record_new();
  record_add(Record, record_beginframe());
}

void
changectable_record(int nentry, int icolortable) {
  UNUSED(nentry);
  UNUSED(icolortable);  
}

void
draw_record(float x, float y) {
  record_object_t *r;
  r = record_draw(x,y);  
  ADD_OR_FREE(Record, r);
}

void
drawpoly_record(float *x, float *y, int n) {
  record_object_t *r;
  r = record_drawpoly(x,y,n);
  ADD_OR_FREE(Record, r);
}

void
fillpoly_record(float *x, float *y, int n) {
  record_object_t *r;
  r = record_fillpoly(x,y,n);
  ADD_OR_FREE(Record, r);  
}

void
endframe_record(int *nerr) {
  record_object_t *r;
  UNUSED(nerr);
  r = record_endframe();
  ADD_OR_FREE(Record, r);
}

void
getwindowstat_record(int number, int *exists) {
  UNUSED(number);
  *exists = FALSE;
}

void
get_geometry_record(int number, 
                unsigned int *width,
                unsigned int *height,
                int *nerr) {
  UNUSED(number);
  UNUSED(nerr);
  *width  = 1;
  *height = 1;
}


void 
setctable_record(int          iwindow, 
                 unsigned int nentry, 
                 float        red[], 
                 float        green[], 
                 float        blue[] ) {
  UNUSED(iwindow);
  UNUSED(nentry);
  UNUSED(red);
  UNUSED(green);
  UNUSED(blue);
}

void
move_record(float x, float y) {
  record_object_t *r;
  r = record_move(x,y);
  ADD_OR_FREE(Record, r);
}
void
setcolor_record(int index) {
  record_object_t *r;
  r = record_color(index);
  ADD_OR_FREE(Record, r);
}
void
setlinestyle_record(int *iline) {
  record_object_t *r;
  r = record_style(*iline);
  ADD_OR_FREE(Record, r);
}
void
setwidth_record(int index) {
  record_object_t *r;
  r = record_width(index);
  ADD_OR_FREE(Record, r);
}
void
text_record(display_t *out, char *text, int n) {
  UNUSED(out);
  record_add(Record, record_text(text, n));
}
void
textbox_record(textbox *tbox) {
  record_object_t *r;
  r = record_textbox(tbox);
  ADD_OR_FREE(Record, r);
}
void
stroke_record(void) {
  record_object_t *r;
  r = record_stroke();
  ADD_OR_FREE(Record, r);
}

void
show_image_record(float *data,
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
  record_object_t *im;
  UNUSED(nerr);

  im = record_image(data, iw, ih, xmin, xmax, ymin, ymax, 
                    x, y, w, h,
                    npseudocolors, nsacolors, ndefcolors, 
                    lbinary);
  ADD_OR_FREE(Record, im);
}

static char *record_name = "RECORD";
static char *record_ext  = "record";

void
initdevice_record() {
  
  initdevice_null( &record );
  record.name               = record_name;
  record.extension          = record_ext;
  record.on                 = TRUE;
  record.id                 = REC;
  record.mirror_device      = TRUE;
  record.adjust_geometry    = adjust_geometry_record;
  record.begin_device       = begindevice_record;
  record.begin_frame        = beginframe_record;
  record.change_color_table = changectable_record;
  record.draw               = draw_record;
  record.drawpoly           = drawpoly_record;
  record.fillpoly           = fillpoly_record;
  record.end_frame          = endframe_record;
  record.get_window_status  = getwindowstat_record;
  record.get_geometry       = get_geometry_record;
  record.move               = move_record; 
  record.set_color          = setcolor_record;
  record.set_color_table    = setctable_record;
  record.set_line_style     = setlinestyle_record;
  record.set_line_width     = setwidth_record;
  record.stroke             = stroke_record;
  record.text               = text_record;  
  record.textbox            = textbox_record;  
  record.show_image         = show_image_record;
  gdm_register_device( &record );
}

void adjust_geometry_text(unsigned int *width,
                          unsigned int *height) { 
  UNUSED(width);
  UNUSED(height);
}
void begindevice_text(int *nerr) { 
  UNUSED(nerr);
}
void calculate_location_text(float *x, float *y, float *cx, float *cy, unsigned int w, unsigned int h, unsigned int iw, unsigned int ih) {
  UNUSED(x);
  UNUSED(y);
  UNUSED(cx);
  UNUSED(cy);
  UNUSED(w);
  UNUSED(h);
  UNUSED(iw);
  UNUSED(ih);
}

void get_geometry_text(int number, unsigned int *width, unsigned int *height, int *nerr) {
  UNUSED(number);
  UNUSED(width);
  UNUSED(height);
  UNUSED(nerr);
}
void getwindowstat_text(int number, int *exists) {
  UNUSED(number);
  UNUSED(exists);
}
void put_image_text(char *data, unsigned int x, unsigned int y, unsigned int w, unsigned int h, int *nerr) {
  UNUSED(data);
  UNUSED(x);
  UNUSED(y);
  UNUSED(w);
  UNUSED(h);
  UNUSED(data);
  UNUSED(nerr);
}
void setctable_text(int win_num, unsigned int nentry, float r[], float g[], float b[]) {
  UNUSED(win_num);
  UNUSED(nentry);
  UNUSED(r);
  UNUSED(g);
  UNUSED(b);
}


void
save_text(display_t *out, char *file) {
  record_write_file(out, file);
}

void 
getratio_text(float *ratio) {
  *ratio = 1.0;
}

void 
setlinestyle_text(int *style) {
  fprintf(text_fp, "setlinestyle: %d\n", *style);
}
void 
setwidth_text(int width) {
  fprintf(text_fp, "setlinewidth: %d\n", width);
}
void 
beginframe_text(int *nerr) { 
  UNUSED(nerr);
  text_fp = fopen(record_filename(NULL), "w");
  fprintf(text_fp, "beginframe: color %d\n", color_background());
}
void 
endframe_text(int *nerr) { 
  UNUSED(nerr);
  fprintf(text_fp, "endframe\n");
  fclose(text_fp);
}
void 
text_text(display_t *out, char *text, int len) {
  UNUSED(out);
  fprintf(text_fp, "text[%d]: '%s'\n", len, text);
}
void
textbox_text(textbox *tbox) {
  UNUSED(tbox);
  fprintf(text_fp, "textbox\n");
}
void 
setcolor_text(int index) {
  fprintf(text_fp, "color: %d\n", index);
}
void 
drawpoly_text(float *x, float *y, int n) {
  fprintf(text_fp, "drawpoly: x,y: (%p, %p) n: %d\n", x, y, n);
}
void 
draw_text(float x, float y) {
  fprintf(text_fp, "draw %f %f\n", x, y);
}

void 
move_text(float x, float y) {
  fprintf(text_fp, "move %f %f\n", x, y);
}

void 
stroke_text() {
  fprintf(text_fp, "stroke\n");
}

void
initdevice_null(display_t *d) {
  d->name                   = NULL;
  d->extension              = NULL;
  d->on                     = FALSE;
  d->id                     = -1;
  d->cursor_flag            = FALSE;

  d->active_device          = FALSE;
  d->cursor_enabled         = FALSE;
  d->mirror_device          = FALSE;

  d->adjust_geometry        = NULL;

  d->begin_device           = NULL;
  d->begin_frame            = NULL;
  d->begin_window           = NULL;

  d->change_color_table     = NULL;
  d->create_window          = NULL;
  d->cursor                 = NULL;
  d->cursor_text            = NULL;
  d->calc_loc               = NULL;

  d->draw                   = NULL;
  d->drawpoly               = NULL;
  d->fillpoly               = NULL;

  d->erase                  = NULL;
  d->end_device             = NULL;
  d->end_frame              = NULL;

  d->fill_image             = NULL;
  d->fill_colorbar          = NULL;
  d->flush_buffer           = NULL;

  d->get_window_status      = NULL;
  d->get_ratio              = NULL;
  d->get_device_ratio       = NULL;
  d->get_alpha_info         = NULL;
  d->get_geometry           = NULL;

  d->move                   = NULL;

  d->put_image              = NULL;

  d->set_color              = NULL;
  d->set_color_table        = NULL;
  d->set_line_style         = NULL;
  d->set_line_width         = NULL;
  d->set_pseudo_color_table = NULL;
  d->set_text_angle         = NULL;
  d->set_text_size          = NULL;
  d->stroke                 = NULL;
  d->show_image             = NULL;

  d->text                   = NULL;
  d->textbox                = NULL;

  d->set_window_width       = NULL;
  d->set_window_height      = NULL;

  d->get_window_size        = NULL;

  d->save                   = NULL;
  d->get_file_descriptor    = NULL;
  d->handle_event           = NULL;
}

static char *xpm_name = "PIXMAP";
static char *xpm_ext  = "xpm";
static char *text_name = "TEXT";
static char *text_ext  = "txt";
static char *png_name = "PNG";
static char *png_ext  = "png";


void
initdevice_xpm() {
  initdevice_null( &xpm );
  xpm.name      = xpm_name;
  xpm.extension = xpm_ext;
  xpm.id        = XPM;
  xpm.save      = xpm_write;
  gdm_register_device( &xpm );
}
void
initdevice_png() {
  initdevice_null( &png );
  png.name      = png_name;
  png.extension = png_ext;
  png.id        = PNG;
  png.save      = png_write;
  gdm_register_device( &png );
}

void
initdevice_text() {
  
  initdevice_null( &xtext );
  xtext.name               = text_name;
  xtext.extension          = text_ext;
  xtext.id                 = TEXT;
  xtext.adjust_geometry    = adjust_geometry_text;
  xtext.begin_device       = begindevice_text;
  xtext.begin_frame        = beginframe_text;
  xtext.calc_loc           = calculate_location_text;
  xtext.draw               = draw_text;
  xtext.drawpoly           = drawpoly_text;
  xtext.end_frame          = endframe_text;
  xtext.get_window_status  = getwindowstat_text;
  xtext.get_ratio          = getratio_text;
  xtext.get_device_ratio   = getratio_text;
  xtext.get_geometry       = get_geometry_text;
  xtext.move               = move_text; 
  xtext.put_image          = put_image_text;
  xtext.set_color          = setcolor_text;
  xtext.set_color_table    = setctable_text;
  xtext.set_line_style     = setlinestyle_text;
  xtext.set_line_width     = setwidth_text;
  xtext.stroke             = stroke_text;
  xtext.text               = text_text;  
  xtext.textbox            = textbox_text;  
  xtext.save               = save_text;
  gdm_register_device( &xtext );
}


/* Record Library */

float * 
record_array_copy(float *a, int n) {
  float *out;
  out = (float *) malloc(sizeof(float) * n);
  memcpy(out, a, sizeof(float) * n);
  return out;
}

void
record_array_scale(float *a, int n, float z) {
  int i;
  for(i = 0; i < n; i++) {
    a[i] = a[i] * z;
  }
}

void
record_image_play(record_object_t *obj, display_t *out) {
  int nerr;
  record_image_t *r = (record_image_t *) obj;
  if(out->show_image) {
    out->show_image(r->data, r->nx, r->ny,
                    r->xmin, r->xmax, r->ymin, r->ymax,
                    r->x, r->y, r->w, r->h,
                    r->npseudocolors,
                    r->nsacolors,
                    r->ndefcolors,
                    r->lbinary,
                    &nerr);
  }
}

void
record_image_free(record_object_t *obj) {
  record_image_t *r = (record_image_t *) obj;
  free(r->data);
  r->data = NULL;
  free(r);
  r = NULL;
}

record_object_t *
record_image(float *data,
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
             int lbinary) {
  record_image_t *r;
  r = (record_image_t *) malloc(sizeof(record_image_t));
  r->data = record_array_copy(data, iw * ih);
  r->nx = iw;
  r->ny = ih;
  r->xmin = xmin;
  r->xmax = xmax;
  r->ymin = ymin;
  r->ymax = ymax;
  r->x = x;
  r->y = y;
  r->w = w;
  r->h = h;
  r->npseudocolors = npseudocolors;
  r->nsacolors = nsacolors;
  r->ndefcolors = ndefcolors;
  r->lbinary = lbinary;

  record_object_init(OBJ(r), RECORD_IMAGE, record_image_play, record_image_free);
  return OBJ(r);
}


void
record_style_free(record_object_t *obj) {
  record_style_t *r = (record_style_t *) obj;
  free(r);
  r = NULL;
}
void
record_style_play(record_object_t *obj, display_t *out) {
  record_style_t *r = (record_style_t *) obj;
  if(out->set_line_style) {
    out->set_line_style(&r->style);
  }
}

record_object_t * 
record_style(int style) {
  record_style_t *r;
  r = (record_style_t *) malloc(sizeof(record_style_t));
  r->style = style;
  record_object_init(OBJ(r), RECORD_STYLE, record_style_play, record_style_free);
  return OBJ(r);
}

void
record_width_free(record_object_t *obj) {
  record_width_t *r = (record_width_t *) obj;
  free(r);
  r = NULL;
}
void
record_width_play(record_object_t *obj, display_t *out) {
  record_width_t *r = (record_width_t *) obj;
  if(out->set_line_width) {
    out->set_line_width(r->width);
  }
}

record_object_t * 
record_width(int width) {
  record_width_t *r;
  r = (record_width_t *) malloc(sizeof(record_width_t));
  r->width = width;
  record_object_init(OBJ(r), RECORD_WIDTH, record_width_play, record_width_free);
  return OBJ(r);
}
void
record_textbox_free(record_object_t *obj) {
  record_textbox_t *r = (record_textbox_t *) obj;
  textbox_free(r->tbox);
  r->tbox = NULL;
  free(r);
  r = NULL;
}

void
record_textbox_play(record_object_t *obj, display_t *out) {
  float y;
  record_textbox_t *r = (record_textbox_t *) obj;
  
  if(out->textbox) { 
    y = r->tbox->y;
    r->tbox->y = r->tbox->y / current_ratio;
    out->textbox( r->tbox );
    r->tbox->y = y;
  }
}

record_object_t * 
record_textbox(textbox *tbox) {
  record_textbox_t *r;
  r = (record_textbox_t *) malloc(sizeof(record_textbox_t ));
  r->tbox = textbox_copy( tbox );
  record_object_init(OBJ(r), RECORD_TEXTBOX, record_textbox_play, record_textbox_free);
  return OBJ(r);
}

void
record_text_free(record_object_t *obj) {
  record_text_t *r = (record_text_t *) obj;
  free(r->text);
  free(r);
  r = NULL;
}

void
record_text_play(record_object_t *obj, display_t *out) {
  record_text_t save;
  char *horz[] = { "", "LEFT",   "CENTER", "RIGHT" };
  char *vert[] = { "", "BOTTOM", "CENTER", "TOP" };
  record_text_t *r = (record_text_t *) obj;

  gettextsize(&save.width, &save.height);
  save.angle = gettextangle();
  save.vert  = cmgdm.ivjust;
  save.horiz = cmgdm.ihjust;

  /* Text Size, calls all active devices */
  settextsize_internal(r->width, r->height);
  if(out->set_text_size) {
    out->set_text_size(r->width, r->height);
  }
  
  /* Text Justificiation */
  settextjust(horz[r->horiz], vert[r->vert]);

  /* Text Angle */
  settextangle_internal(r->angle);
  if(out->set_text_angle) {
    out->set_text_angle(r->angle);
  }

  /* Show Text */
  if(out->text) {
    out->text(out, r->text, r->n);
  }

  /* Return back to original values */
  settextsize_internal(save.width, save.height);
  settextangle_internal(save.angle);
  settextjust(horz[save.horiz], vert[save.vert]);

}

record_object_t * 
record_text(char *text, int n) {
  record_text_t *r;
  r = (record_text_t *) malloc(sizeof(record_text_t));

  r->text   = fstrdup(text, n);

  r->n      = n;
  r->angle  = gettextangle();
  r->vert   = cmgdm.ivjust;
  r->horiz  = cmgdm.ihjust;
  gettextsize(&r->width, &r->height);
  record_object_init(OBJ(r), RECORD_TEXT, record_text_play, record_text_free);
  return OBJ(r);
}

void
record_color_free(record_object_t *obj) {
  record_color_t *r = (record_color_t *) obj;
  free(r);
  r = NULL;
}

void
record_color_play(record_object_t *obj, display_t *out) {
  record_color_t *r = (record_color_t *) obj;
  if(out->set_color) {
    out->set_color(r->index);
  }
}

record_object_t * 
record_color(int index) {
  record_color_t *r;
  r = (record_color_t *) malloc(sizeof(record_color_t));
  r->index = index;
  record_object_init(OBJ(r), RECORD_COLOR, record_color_play, record_color_free);
  return OBJ(r);
}

void
record_move_play(record_object_t *obj, display_t *out) {
  record_move_t *r = (record_move_t *) obj;

  /* Previous location is set by move */
  cmgdm.xold = r->x;
  cmgdm.yold = r->y / current_ratio;
  if(out->move) {
    out->move(r->x, r->y/current_ratio);
  }
}

void
record_move_free(record_object_t *obj) {
  record_move_t *r = (record_move_t *) obj;
  free(r);
  r = NULL;
}

record_object_t *
record_move(float x, float y) {
  record_move_t *r;
  r = (record_move_t *) malloc(sizeof(record_move_t));
  r->x = x;
  r->y = y;
  record_object_init(OBJ(r), RECORD_MOVE, record_move_play, record_move_free);
  return OBJ(r);
}

void
record_draw_play(record_object_t *obj, display_t *out) {
  record_draw_t *r = (record_draw_t *) obj;
  cmgdm.xold = r->x;
  cmgdm.yold = r->y / current_ratio;

  if(out->draw) {
    out->draw(r->x, r->y / current_ratio);
  }
}

void
record_draw_free(record_object_t *obj) {
  record_draw_t *r = (record_draw_t *) obj;
  free(r);
  r = NULL;
}

record_object_t *
record_draw(float x, float y) {
  record_draw_t *r;
  r = (record_draw_t *) malloc(sizeof(record_draw_t));
  if(r) {
      r->x = x;
      r->y = y;
      record_object_init(OBJ(r), RECORD_DRAW, record_draw_play, record_draw_free);
  }
  return OBJ(r);
}

void
record_drawpoly_play(record_object_t *obj, display_t *out) {
  float *y;
  record_drawpoly_t *r = (record_drawpoly_t *) obj;
  
  y = record_array_copy(r->y, r->n);
  record_array_scale(y, r->n, 1.0 / current_ratio);

  if(out->drawpoly) {
    out->drawpoly(r->x, y, r->n);
  }
  free(y);
  y = NULL;
}
void
record_drawpoly_free(record_object_t *obj) {
  record_drawpoly_t *r = (record_drawpoly_t *) obj;
  free(r->x);
  free(r->y);
  r->x = NULL;
  r->y = NULL;
  free(r);
  r = NULL;
}

record_object_t *
record_drawpoly(float *x, float *y, int n) {
  record_drawpoly_t *r;
  r = (record_drawpoly_t *) malloc(sizeof(record_drawpoly_t));
  r->n = n;
  r->x = record_array_copy(x, n);
  r->y = record_array_copy(y, n);
  record_object_init(OBJ(r), RECORD_DRAWPOLY, record_drawpoly_play, record_drawpoly_free);
  return OBJ(r);
}

void
record_fillpoly_play(record_object_t *obj, display_t *out) {
  float *y;
  record_drawpoly_t *r = (record_drawpoly_t *) obj;
  
  y = record_array_copy(r->y, r->n);
  record_array_scale(y, r->n, 1.0 / current_ratio);

  if(out->fillpoly) {
    out->fillpoly(r->x, y, r->n);
  }
  free(y);
  y = NULL;
}

record_object_t *
record_fillpoly(float *x, float *y, int n) {
  record_drawpoly_t *r;
  r = (record_drawpoly_t *) malloc(sizeof(record_drawpoly_t));
  r->n = n;
  r->x = record_array_copy(x, n);
  r->y = record_array_copy(y, n);
  record_object_init(OBJ(r), RECORD_FILLPOLY, record_fillpoly_play, record_drawpoly_free);
  return OBJ(r);  
}


void
record_stroke_free(record_object_t *r) {
  record_stroke_t *s = (record_stroke_t *) r;
  free(s);
  s = NULL;
}
void
record_stroke_play(record_object_t *r, display_t *out) {
  UNUSED(r);
  if(out->stroke) {
    out->stroke();
  }
}

record_object_t * 
record_stroke() {
  record_stroke_t *r;
  r = (record_stroke_t*) malloc(sizeof(record_stroke_t));
  record_object_init(OBJ(r), RECORD_STROKE, record_stroke_play, record_stroke_free);
  return OBJ(r);
}

void
record_endframe_free(record_object_t *obj) {
  record_endframe_t *r = (record_endframe_t *) obj;
  free(r);
  r = NULL;
  current_ratio = 1.0;
}

void
record_endframe_play(record_object_t *obj, display_t *out) {
  int nerr;
  UNUSED(obj);
  if(out->end_frame) {
    out->end_frame( &nerr );
  }
}

record_object_t * 
record_endframe() {
  record_endframe_t *r;
  r = (record_endframe_t *) malloc(sizeof(record_endframe_t));
  record_object_init(OBJ(r), RECORD_ENDFRAME, record_endframe_play, record_endframe_free);
  return OBJ(r);
}

void
record_beginframe_free(record_object_t *obj) {
  record_beginframe_t *r = (record_beginframe_t *) obj;
  free(r);
  r = NULL;
}

void
record_beginframe_play(record_object_t *obj, display_t *out) {
  int color;
  int nerr;
  record_beginframe_t *r = (record_beginframe_t *) obj;

  if(out->begin_frame) {
    /* Copy current and set color value 
     * Color is set in beginframe */
    color = color_background();
    color_background_set(r->color);

    out->begin_frame( &nerr );

    /* Return to original value */
    color_background_set(color);
  }
}

record_object_t * 
record_beginframe() {
  record_beginframe_t *r;
  r = (record_beginframe_t *) malloc(sizeof(record_beginframe_t));
  r->color = color_background();
  getratio(& current_ratio );

  record_object_init(OBJ(r), RECORD_BEGINFRAME, record_beginframe_play, record_beginframe_free);
  return OBJ(r);
}

void
record_object_free(record_object_t *r) {
  r->free( r );
}

void
record_object_init(record_object_t *r, int type, play_t p, free_t f) {
  r->type = type;
  r->play = p;
  r->free = f;
}

void
record_object_play(record_object_t *r, display_t *out) {
  if(!r) {
    fprintf(stderr, "SAC Record: Attempt to play an unitialized record object\n");
    return;
  }
  if(!r->play) {
    fprintf(stderr, "SAC Record: Attempt to play an unitialized record play method\n");
    return;
  }

  r->play( r, out );
}

void
record_store_play(record_store_t *rs, display_t *out) {
  int i;

  for(i = 0; i < rs->n; i++) {
    record_object_play( rs->data[i], out );
  }
}

void
record_store_free(record_store_t *rs) {
  int i;
  for(i = 0; i < rs->n; i++) {
    record_object_free(rs->data[i]);
  }
  free(rs->data);
  rs->data = NULL;
  free(rs);
  rs = NULL;
}

int
record_store_add(record_store_t *rs, record_object_t *obj) {
  record_object_t **tmp;
  if(!rs || !obj) {
      return FALSE;
  }
  if(rs->n + 1 >= rs->nalloc) {
    rs->nalloc = rs->nalloc * 2;
    tmp = (record_object_t **) realloc(rs->data, 
                                       sizeof(record_object_t *) * rs->nalloc);
    if(!tmp) {
      fprintf(stderr, "SAC Record: Error creating space for more record objects\n");
      return FALSE;
    }
    rs->data = tmp;
  }
  rs->data[rs->n] = obj;
  rs->n = rs->n + 1;
  return TRUE;
}

record_store_t * 
record_store_new() {
  record_store_t *rs;
  rs = (record_store_t *) malloc(sizeof(record_store_t));
  rs-> n = 0;
  rs->nalloc = 4;
  rs->data = (record_object_t **) malloc(sizeof(record_object_t *) * rs->nalloc);
  return rs;
}

int
record_add(record_t *r, record_object_t *obj) {
  return record_store_add(r->objs, obj);
}

void
record_free(record_t *r) {
  record_store_free(r->objs);
  free(r);
  r = NULL;
}

record_t * 
record_new() {
  record_t *r;
  r = (record_t *) malloc(sizeof(record_t));
  r->objs = record_store_new();
  return r;
}

void
record_play_current(display_t *out) {
  record_play(Record, out);
}

void
record_play(record_t *r, display_t *out) {
  float current_ratio_save;
  float ratio;

  if(!r) {
    return;
  }

  /* Save the original ratio */
  current_ratio_save = current_ratio;

  /* Get Output device ratio and apply */
  out->get_ratio( & ratio );
  current_ratio = current_ratio / ratio;

  /* Play the command stack */
  record_store_play(r->objs, out);

  /* Restore the original ratio */
  current_ratio = current_ratio_save;
}
