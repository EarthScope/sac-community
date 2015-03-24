/** 
 * @file   gd3.x11.h
 * 
 * @brief  Graphics Device 3 (X11)
 * 
 */

#ifndef _GD3_X11_H_
#define _GD3_X11_H_

#include "config.h"

#ifdef X11_APP

/*
 * COMMON for XWindow graphics device.
 *
 * DESCRIPTION:
 *    draw_pos:            Pixel location of current point in XWindow
 *                         coordinates.
 *    basew3:              Base window structure attributes.
 *    titlew3:             Title window structure attributes.
 *    plotw3:              Plot window structure attributes.
 *    iconw3:              Icon window structure attributes.
 *    win_attr:            Window structure attributes.
 *      win:               Handle of window.
 *      width_p:           Width of window in pixels.
 *      height_p:          Height of window in pixels.
 *      status:            Status of window.
 *    title_font3:         Font id of title label.
 *    pixdef3:             Pixel color definitions.
 *    borderwidth3:        Width of window borders in pixels.
 *    num_wins3:           Number of windows which have been created.
 *    c_win3:              Current window.  Numbered 1 to n
 *    device_type:         Device type.
 *    cursor_on:           Set to 1 if waiting for cursor input, otherwise 0.
 *    cursortext_on4:      Set to 1 if waiting for cursor text input, otherwise 0.
 *    color:               Index into color table to current color entry.
 *    xcursor_p3:          x position of cursor in pixels.
 *    ycursor_p3:          y position of cursor in pixels.
 *    scr_width_p3:         Width of screen in pixels.
 *    scr_height_p3:        Height of screen in pixels.
 *    char_cursor:         Character struck at cursor location.
 *    text_cursor4:        Line of text struck at cursor location.
 *    device_name:         Device name.
 *
 ** MODIFICATION HISTORY:
 *  2010 August : win_status3 is now XWindow
 *    920609:  Changed OW_OFFSET from hardcoded -8 to 0. Problem with
 *             XdrawLine callback was fixed in OpenWindows version 3.
 *    920318:  Protability to IBM required changing "struct{int x,y} point"
 *             to "struct{int x,y;}
 *    910506:  Added defines OW_OFFSET and ZERO for kludge to openwindows
 *             cursor problem. XdrawLine (callback to server) retruns
 *             eight pixels more than the values sent to it. Only happens
 *             immediately after the XQueryPointer call. The offset is
 *             applied in cursor3.c cursortext3.c and dispatcheve3.c (wct).
 *    890606:  Added GC and display3 functionality.
 *    870323:  Added variables for cursortext3_ routine.
 *    870318:  Changed win_status3 structure.
 *    870317:  Added title font.
 *    870310:  Added icon.
 *    870309:  Added title window.
 *    870223:  Original Version.
 ******************************************************************************
 */

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "config.h"

#include "gd3.aux.h"

#include "gdm.h"

#ifdef HAVE_XFT
#include <X11/Xft/Xft.h>
#endif /* HAVE_XFT */

typedef struct _XWindow XWindow;
typedef struct _XScreen XScreen;
typedef struct _XFont   XFont;

typedef void (*resize_f) (XWindow *xw, int width, int height, void *data);
typedef void (*expose_f) (XWindow *xw, void *data);
typedef void (*draw_f)   (XWindow *xw, void *data);

typedef enum _XFontWeight XFontWeight;
typedef enum _XFontSlant XFontSlant;

#ifdef HAVE_XFT
enum _XFontWeight {
  XFONT_WEIGHT_LIGHT    = XFT_WEIGHT_LIGHT,
  XFONT_WEIGHT_MEDIUM   = XFT_WEIGHT_MEDIUM,
  XFONT_WEIGHT_DEMIBOLD = XFT_WEIGHT_DEMIBOLD,
  XFONT_WEIGHT_BOLD     = XFT_WEIGHT_BOLD,
  XFONT_WEIGHT_BLACK    = XFT_WEIGHT_BLACK
};

enum _XFontSlant {
  XFONT_SLANT_ROMAN   = XFT_SLANT_ROMAN,
  XFONT_SLANT_ITALIC  = XFT_SLANT_ITALIC,
  XFONT_SLANT_OBLIQUE = XFT_SLANT_OBLIQUE,
};

#else 
enum _XFontWeight {
  XFONT_WEIGHT_LIGHT,
  XFONT_WEIGHT_MEDIUM,
  XFONT_WEIGHT_DEMIBOLD,
  XFONT_WEIGHT_BOLD,
  XFONT_WEIGHT_BLACK,
};

enum _XFontSlant {
  XFONT_SLANT_ROMAN,
  XFONT_SLANT_ITALIC,
  XFONT_SLANT_OBLIQUE,
};

#endif /* HAVE_XFT */ 

typedef enum _XFontType XFontType;
enum _XFontType {
  XFONT_TYPE_SOFTWARE,
  XFONT_TYPE_CORE,
  XFONT_TYPE_XFT,
};

struct _XFont {
  float       size;      /* Point Size */
  char       *family;    /* Name 
                          * Courier, Helvetica, Times, Symbol, ZapfDingbats 
                          *    - Minimum set by PDF
                          */
  XFontSlant  slant;     /* Roman, Italic, Oblique */
  XFontWeight weight;    /* Light, Medium, Demibold, Bold, Black */
  int         antialias; /* Boolean */

  XFontType   type;
  XFontStruct *font_core;
  
#ifdef HAVE_XFT
  XftFont     *font_xft;
#endif
};

struct _XWindow {
  Window    win;
  int       status;
  int       width;
  int       height;
  int       border;
  GC        gc;

  int       use_buffer;
  Pixmap    buffer;

  XScreen  *xscreen;

  resize_f  resize;
  expose_f  expose;
  draw_f    draw;

  void     *resize_data;
  void     *expose_data;
  void     *draw_data;
  
  XColor   *color;
  
  XFont    *font;

};

struct _XScreen {
  Display *display;
  Visual  *visual;
  int      active;
  int      width;
  int      height;
  int      screen;
  int      depth;
  Window   root;
};

#define CURRENT -1


point draw_pos;

XWindow basew3[MAX_WINS+1];
XWindow titlew3[MAX_WINS+1];
XWindow plotw3[MAX_WINS+1];
GC cursorgc3[MAX_WINS+1];

int num_wins3;
int c_win3;
int device_type3;
int cursor_on3;
int cursortext_on3;
int linestyle3;
int xcursor_p3, ycursor_p3;
char char_cursor3[1];
char text_cursor3[132];
char device_name3[13];
Font title_font3;
XColor pixdef3[256];
unsigned long color3;

void fill_background3            ( int window);
void begindevice3                ( int *nerr);
void beginframe3                 ( int *nerr);
void beginwindow3                ( int *win_num, 
                                   int *nerr);
void calc_loc3                   ( unsigned int *xloc, 
                                   unsigned int *yloc, 
                                   unsigned int *cbarxoffset, 
                                   unsigned int *cbaryoffset, 
                                   unsigned int w_width, 
                                   unsigned int w_height, 
                                   double xpmn, 
                                   double xpmx, 
                                   double xmin, 
                                   double first, 
                                   double last, 
                                   double ypmn, 
                                   double ypdel, 
                                   int *nerr);
void cbar_window3                ( unsigned int xloc, 
                                   unsigned int yloc, 
                                   unsigned int height, 
                                   unsigned int w_height, 
                                   unsigned int w_width, 
                                   double vspaceratio, 
                                   double ypmax, 
                                   int *nerr);
void changectable3               ( int nentry, 
                                   int icolortable);
void createwindow3               ( int *win_num, 
                                   float *xmin_vp, 
                                   float *xmax_vp, 
                                   float *ymin_vp, 
                                   float *ymax_vp, 
                                   int *nerr);
void cursor3                     ( float *xloc_vp, 
                                   float *yloc_vp, 
                                   char cchar[], 
                                   int cchar_length);
void cursortext3                 ( float *xloc_vp, 
                                   float *yloc_vp, 
                                   char ktext[], 
                                   int ktext_length);
int use_large_crosshairs         ( int getset);
void dispatchevent3              ( int *nerr);
void draw3                       ( float xloc_vp, 
                                   float yloc_vp);
void drawpoly3                   (float *xloc_vp, 
                                  float *yloc_vp, 
                                  int npts);
void fillpoly3                   (float *x, 
                                  float *y, 
                                  int n);
void enddevice3                  ( int *nerr);
void endframe3                   ( int *nerr);
int error3                       (Display *display, XErrorEvent *error);
void fill_background3            ( int window);
void erase3                      (void);
int ioerror3                     (Display *display);

void error3_handling             (void);
void expose3                     (void);
char *fill_clrbar3               ( int npseudocolors, 
                                   int width, 
                                   int npricolors, 
                                   int ndefcolors, 
                                   int *nerr);
char *fill_image3                ( unsigned int height, 
                                   unsigned int width, 
                                   float data[], 
                                   float dmin, 
                                   float range, 
                                   int npseudocolors, 
                                   int nsaccolors, 
                                   int ndefcolors, 
                                   int *nerr);
void flushbuffer3                ( int *nerr);
void get_geometry3               ( int window, 
                                   unsigned int *width_return, 
                                   unsigned int *height_return, 
                                   int *nerr);
void getalphainfo3               ( int *num_lines, 
                                   char erase[], 
                                   int erase_length);
void getdeviceinfo3              ( char dev_name[], 
                                   int dev_name_length, 
                                   int *dev_type);
void getdevicerat3               ( float *ratio);
void getratio3                   ( float *ratio);
void getwindowstat3              ( int win_num, 
                                   int *exists);
void initdevice3                 (void);
void make_label3                 ( char string[], 
                                   int *num, 
                                   char label[]);
void move3                       ( float xloc_vp, float yloc_vp);
void put_image3                  ( char *data, 
                                   unsigned int xloc, 
                                   unsigned int yloc, 
                                   unsigned int width, 
                                   unsigned int height, 
                                   int *nerr);
void setcolor3                   ( int index);
void setctable3                  ( int win_num, 
                                   unsigned int nentry, 
                                   float red[], 
                                   float green[], 
                                   float blue[]);
void setlinestyle3               ( int *linestyle);
void setpsctable3                ( int *win_num, 
                                   unsigned int nentry, 
                                   float red[], 
                                   float green[], 
                                   float blue[]);
void settextsize3                ( float width, 
                                   float height);
void setwidth3                   ( int index);

int set_window_width3            (int w);
int set_window_height3           (int h);
int is_window_size_set_in_pixels (void);
int get_window_width3            (void);
int get_window_height3           (void);
int get_screen_width3            (void);
int get_screen_height3           (void);
void get_window_size_viewport3   (float *xmin,
                                  float *xmax,
                                  float *ymin,
                                  float *ymax);


Font load_font                   (Display *display, 
                                  char *name);
void calculate_location3         (float *x,
                                  float *y,
                                  float *cx,
                                  float *cy,
                                  unsigned int w,
                                  unsigned int h,
                                  unsigned int iw,
                                  unsigned int ih) ;

void show_image_x11              (float *data,
                                  unsigned int iw, /* Image Size, data points */
                                  unsigned int ih,
                                  float xmin, /* Limits of the data */ 
                                  float xmax,
                                  float ymin,
                                  float ymax,
                                  float x,  /* View space Coordinates [0,1] */
                                  float y,
                                  float w,
                                  float h,
                                  int npseudocolors,
                                  int nsacolors,
                                  int ndefcolors,
                                  int lbinary,
                                  int *nerr);

void text_x11                    (display_t *out, char *text, int n);

void text_box_x11                (textbox *tbox);
void set_window_width_x11        (int width);
void set_window_height_x11       (int height);
int  get_window_width_x11        ( void );
int  get_window_height_x11       ( void );
int  get_file_descriptor_x11     ( void );
void get_window_size_x11         (float *xmin,
                                  float *xmax,
                                  float *ymin,
                                  float *ymax);
char * handle_event_x11(int *nerr);

/* XWindow Functions */

XWindow * plot_window(int num);
XScreen * xscreen_get();

#define DISPLAY(xw) ( xw->xscreen->display )
#define SCREEN(xw)  ( xw->xscreen->screen )
#define VISUAL(xw)  ( xw->xscreen->visual )
#define ROOT(xw)    ( xw->xscreen->root )
#define DEPTH(xw)   ( xw->xscreen->depth )

int xwindow_init                (XScreen *xs, 
                                 XWindow *xw, 
                                 XWindow *parent,
                                 int      x,
                                 int      y,
                                 int      width,
                                 int      height);
void xwindow_draw               (XWindow *xw, void *data);
void xwindow_fill_background    (XWindow *xw);
void xwindow_double_buffer_free (XWindow *xw);
void xwindow_double_buffer_new  (XWindow *xw);
void xwindow_double_buffer      (XWindow *xw, 
                                 int      flag);
void xwindow_resize             (XWindow *xw,
                                 int      width, 
                                 int      height,
                                 void    *data);
void xwindow_expose             (XWindow *xw,
                                 void    *data);

void xwindow_font_set_family (XWindow *xw, char *family);
void xwindow_font_set_size   (XWindow *xw, float size);
void xwindow_font_set_weight (XWindow *xw, int weight);
void xwindow_font_set_slant  (XWindow *xw, int slant);
void xwindow_font_set_type   (XWindow *xw, int type);
void xwindow_font_load       (XWindow *xw);

XFont *xfont_new             ();

float view_to_x11_x(float x, XWindow *xw);
float view_to_x11_y(float y, XWindow *xw);
float x11_to_view_x(float x, XWindow *xw);
float x11_to_view_y(float y, XWindow *xw);

void set_constrain_plot_ratio_x11(int set);
void set_plot_ratio_x11(float ratio);
void xwindow_set_font_system(int type);
void xwindow_set_font_base(int type);
void xwindow_redraw(XWindow *xw);
Font load_font(Display *display, char *name);

#endif /* X11_APP */
#endif /* _GD3_X11_H_ */

