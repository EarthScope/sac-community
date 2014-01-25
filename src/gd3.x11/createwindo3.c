/*******************************************************************************
** PURPOSE:
*    To create a window.
*
** INPUT ARGUMENTS:
*    win_num:  Window number. (Pointer)
*    xmin_vp:  Minimum x placement of window in device viewport dimensions.  
*              Range:  [0.0,1.0] (Pointer)
*    xmax_vp:  Maximum x placement of window in device viewport dimensions.  
*              Range:  [0.0,1.0] (Pointer)
*    ymin_vp:  Minimum y placement of window in device viewport dimensions.
*              Range:  [0.0,device aspect ratio] (Pointer)
*    ymax_vp:  Maximum y placement of window in device viewport dimensions.
*              Range:  [0.0,device aspect ratio] (Pointer)
*
** OUTPUT ARGUMENTS:
*    nerr:  Error flag.  Set to 0 if no error occurs. (Pointer)
*
** GLOBAL INPUT:
*    gd3.x11.h: titlefont3
*
** GLOBAL OUTPUT:
*    gd3.x11.h:  basew3->(win, width_p, height_p, status),
*             titlew3->(win, width_p, height_p, status),
*             plotw3->(win, width_p, height_p, status),
*             iconw3->(win, width_p, height_p, status), num_wins3
*
** SUBROUTINES CALLED:
*    XCreateWindow, XCreateGC, XSetStandardProperties, 
*    XSelectInput, XMapWindow, XDrawString,
*    XFlush, XSync, dispatchevent3_, make_label3
*
** LOCAL VARIABLES:
*    kwin_num:      Character equivalent of win_num.
*    xul_p:         Upper left x coordinate of plot window in pixels.
*    yul_p:         Upper left y coordinate of plot window in pixels.
*    width_p:       Width of plot window in pixels.
*    height_p:      Height of plot window in pixels.
*    title_label:   Title label.
*    ptitle_label:  Pointer to title label. (Pointer)
*******************************************************************************
** MODIFICATION HISTORY:
*    910426:  Added call XDefineCursor to set the crosshair in graphics window. (wct)
*    890830:  Used same window mask for title window as for plot window. (jet)
*    890606:  Modified to be X11 compatible.  (kjm)
*    870318:  Changes due to gd3.x11.h structure change.
*    870310:  Added icon.
*    870309:  Added title window.
*    870227:  Original Version.
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "co.h"
#include "gd3.x11.h"
#include <X11/cursorfont.h>

#include "bool.h"

#include "config.h"

#include "sac.bitmap"
#include "debug.h"

extern display_t x11;

Cursor cursor;

int
xwindow_init(XScreen *xs, 
             XWindow *xw, 
             XWindow *parent,
             int      x,
             int      y,
             int      width,
             int      height) {

  XGCValues xgcdef;
  unsigned int valuemask, gcmask;
  XSetWindowAttributes winatt;

  xw->xscreen  = xs;
  xw->width    = width;
  xw->height   = height;
  xw->status   = AVAILABLE;
  xw->border   = 4;

  /* Window Attributes:
   * - override_redirect: Allow window decorations around the window
   * - background_pixel:  Color of the pixel making the window background
   * - bit_gravity:       Define regions of the window to be kept during resize
   *                      Contents should not move or be discarded during resize
   *                      This makes for smoother window resizing
   */
  winatt.override_redirect = False; 
  winatt.background_pixel  = WhitePixel(xs->display, xs->screen);
  winatt.bit_gravity       = StaticGravity;
  valuemask                = CWBackPixel | 
                             CWOverrideRedirect |
                             CWBitGravity ;

  xw->win = XCreateWindow(DISPLAY(xw),
                          (parent) ? parent->win : ROOT(xw),
                          x, y, width, height,
                          xw->border,   
                          CopyFromParent,
                          InputOutput,
                          CopyFromParent,
                          valuemask, 
                          &winatt);
  if(xw->win == 0) {
    return FALSE;
  }

  /* Graphics Context */
  xgcdef.foreground = BlackPixel(DISPLAY(xw), SCREEN(xw));
  xgcdef.background = WhitePixel(DISPLAY(xw), SCREEN(xw));
  xgcdef.graphics_exposures = False; /* Avoid GraphicsExpose and NoExpose */
  gcmask = GCBackground | GCForeground | GCGraphicsExposures;
  
  xw->gc       = XCreateGC(DISPLAY(xw), xw->win, gcmask, &xgcdef);
                           
  /* Window should respond to exposure and window property changes */
  XSelectInput(DISPLAY(xw), xw->win, ExposureMask | StructureNotifyMask);

  /* Turn off Double Buffer Pixmap */
  xw->use_buffer = FALSE;
  xw->buffer     = None;

  xw->expose     = xwindow_expose;
  xw->resize     = xwindow_resize;
  xw->draw       = xwindow_draw;

  xw->expose_data = NULL;
  xw->resize_data = NULL;
  xw->draw_data   = NULL;

  xw->font = xfont_new();

  return TRUE;
}

void 
xwindow_draw(XWindow *xw, void *data) {
  UNUSED(xw);
  UNUSED(data);
  return;
}

void
xwindow_fill_background(XWindow *xw) {
  XGCValues val;

  XGetGCValues(DISPLAY(xw), xw->gc, GCForeground | GCBackground, &val);
  XSetForeground(DISPLAY(xw), xw->gc, val.background);
  XFillRectangle(DISPLAY(xw), xw->buffer, xw->gc, 0, 0, xw->width, xw->height);
  XSetForeground(DISPLAY(xw), xw->gc, val.foreground);
}

void
xwindow_double_buffer_free(XWindow *xw) {
  if(xw->buffer != None) {
    XFreePixmap(DISPLAY(xw), xw->buffer);
    xw->buffer = None;
  }
}

void
xwindow_double_buffer_new(XWindow *xw) {
  if(xw->use_buffer) {
    if(xw->buffer) {
      xwindow_double_buffer_free(xw);
    }
    xw->buffer = XCreatePixmap(DISPLAY(xw), xw->win, xw->width, xw->height, DEPTH(xw));
    xwindow_fill_background(xw);
  }
}

void
xwindow_double_buffer(XWindow *xw, int flag) {
  xw->use_buffer = flag;
  xwindow_double_buffer_free(xw);
  if(xw->use_buffer) {
    xwindow_double_buffer_new(xw);
  }
}

void
xwindow_redraw(XWindow *xw) {
  if(xw->use_buffer) {
    xwindow_double_buffer_new(xw);
  }
  xw->draw(xw, xw->draw_data);
}

void
xwindow_resize(XWindow *xw, int width, int height, void *data) {
  UNUSED(data);
  if(width == xw->width && height == xw->height) {
    return;
  }
  xw->width  = width;
  xw->height = height;
  if(xw->use_buffer) {
    xwindow_double_buffer_new(xw);
    xw->draw(xw, xw->draw_data);
  }
}

void
xwindow_expose(XWindow *xw, void *data) {
  UNUSED(data);
  if(xw->use_buffer) {
    if(xw->buffer) {
      XCopyArea(DISPLAY(xw), xw->buffer, xw->win, xw->gc,
                0, 0, xw->width, xw->height, 0, 0);
    }
  } else {
    xw->draw(xw, xw->draw_data);
  }
}

void
xwindow_plot_redraw(XWindow *xw, void *data) {
  UNUSED(data);
  UNUSED(xw);
  record_play_current(&x11);
}

int
xwindow_font_defined(XWindow *xw) {
  XGCValues val;
  XGetGCValues(DISPLAY(xw), xw->gc, GCFont, &val);
  if(val.font) {
    return TRUE;
  }
  return FALSE;
}

void
xwindow_title_draw(XWindow *title, void *data) {
  char *p;
  char label[32];
  int *index = (int *) data;
  p = label;
  if(!xwindow_font_defined(title)) {
    return;
  }
  make_label3("Graphics Window:  ", index, label);
  XDrawString(DISPLAY(title), title->win, title->gc, 6, 17, p, strlen(p));
}

void
xwindow_base_resize(XWindow *xw, int width, int height, void *data) {
  XWindow **kids;
  XWindow *title, *plot;
  if(width == xw->width && height == xw->height) {
    return;
  }
  
  kids = (XWindow **) data;
  title = kids[0];
  plot  = kids[1];

  XMoveResizeWindow(DISPLAY(title), 
                    title->win,
                    -title->border, 
                    -title->border,
                    width, 
                    title->height);
  XMoveResizeWindow(DISPLAY(plot),
                    plot->win,
                    -plot->border,
                    title->height,
                    width, 
                    height - title->height - plot->border);
  xw->width  = width;
  xw->height = height;
}


void
xwindow_set_draw_function(XWindow *xw, draw_f draw, void *data) {
  xw->draw      = draw;
  xw->draw_data = data;
}
void
xwindow_set_resize_function(XWindow *xw, resize_f resize, void *data) {
  xw->resize      = resize;
  xw->resize_data = data;
}

void 
createwindow3(int *win_num,
              float *xmin_vp,
              float *xmax_vp,
              float *ymin_vp,
              float *ymax_vp,
              int *nerr)
{
  char name[8];
  int xul_p, yul_p;
  unsigned int width_p, height_p;
  char title_label[32];
  Atom wm_delete_window;
  //XSetWindowAttributes winatt;
  XGCValues xgccurs,xgcicon;
  XWMHints wmhints;
  XSizeHints *sizehints;
  Pixmap SacPixmap;
  GC icongc;
  int stringwid;
  Font icon_font;
  XFontStruct *font_info;
  char *iconfont = "fixed";

  XWindow *title, *base, *plot;
  XWindow **kids;
  XScreen *xs;

  int *fu;

  *nerr = 0;
  /* ptitle_label = title_label; */

  xs = xscreen_get();

  wm_delete_window = XInternAtom(xs->display, "WM_DELETE_WINDOW", False);


  /* Set up boundaries of window */

  xul_p    = (int) ((float) (xs->width-1) * *xmin_vp);
  yul_p    = (int) ((float) (xs->height-1) * (1.0 - *ymax_vp));
  width_p  = (unsigned int) ((float) (xs->width-1) * (*xmax_vp - *xmin_vp));
  height_p = (unsigned int) ((float) (xs->height-1) * (*ymax_vp - *ymin_vp));

  /* Create title label */
  make_label3("Graphics Window:  ", win_num, title_label);

  /* Create base window and set attributes */
  title = &(titlew3[*win_num]);
  base  = &(basew3[*win_num]);
  plot  = &(plotw3[*win_num]);

  base->border    = 4;
  title->height = 25;

  /* Window Attributes */
  /* winatt.override_redirect = False; */
  /* winatt.background_pixel  = WhitePixel(xs->display, xs->screen); */
  /* winatt.bit_gravity       = StaticGravity; */
  /* valuemask                = CWBackPixel |  */
  /*                            CWOverrideRedirect | */
  /*                            CWBitGravity ; */

  /* Base Window */

  if(! xwindow_init(xs, base, NULL,
                    xul_p, yul_p, 
                    width_p, height_p + title->height + base->border) ) {
    *nerr = 1;
    return;
  }
                    
  error3_handling();

  make_label3("SAC ",win_num,name);

  wmhints.flags  = InputHint | StateHint | IconPixmapHint;
  wmhints.input  = True;
  wmhints.initial_state = NormalState;

  /* Icon */
  SacPixmap = XCreateBitmapFromData (xs->display,
                                     base->win,
				     (const char*) sac_bits, 
                                     sac_width, 
                                     sac_height);

  if( (icon_font = load_font(xs->display,iconfont)) == None ) {
    fprintf(stderr, "SAC Error loading font for icon\n");
  } else {
    xgcicon.font       = icon_font;
    xgcicon.foreground = BlackPixel(xs->display, xs->screen);
    xgcicon.background = WhitePixel(xs->display, xs->screen);
    icongc = XCreateGC(xs->display,
                       SacPixmap,
                       (GCBackground | GCForeground | GCFont),
                       &xgcicon);
    font_info = XQueryFont(xs->display,XGContextFromGC(icongc));
    stringwid = XTextWidth(font_info,name,strlen(name));
    XDrawString(xs->display,
                SacPixmap,
                icongc,
                32-(stringwid>>1), 
                59,
                name,
                strlen(name));
    wmhints.icon_pixmap = SacPixmap;
    XSetWMHints(DISPLAY(base), base->win, &wmhints);
  }

  sizehints = XAllocSizeHints();
  sizehints->flags = 0;
  sizehints->flags = PPosition | PSize;
  sizehints->x     = xul_p;
  sizehints->y     = yul_p;
  XSetWMNormalHints(DISPLAY(base), base->win, sizehints);
  XSetWMProtocols(DISPLAY(base), base->win, &wm_delete_window, 1);

  /* Title Window */

  if(! xwindow_init(xs, title, base, -base->border, -base->border, width_p, title->height)) {
    *nerr = 1;
    return;
  }

  xwindow_font_set_type(title, XFONT_TYPE_CORE);

  fu = (int *) malloc(sizeof(int));
  *fu = *win_num;
  xwindow_set_draw_function(title, xwindow_title_draw, (void *)fu);

  /* Plot Window */

  if(! xwindow_init(xs, plot, base, -base->border, title->height, width_p, height_p) ) {
    *nerr = 1;
    return;
  }
  xwindow_double_buffer(plot, TRUE);
  plot->draw = xwindow_plot_redraw;
  xwindow_set_draw_function(plot, xwindow_plot_redraw, NULL);


  /* Resizing the Base Window Function */
  kids = (XWindow **) malloc(sizeof(XWindow *) * 2);
  kids[0] = title;
  kids[1] = plot;
  xwindow_set_resize_function(base, xwindow_base_resize, (void *) kids);

  /* Cursor Context */
  xgccurs.function = GXxor;
  cursorgc3[*win_num] = XCreateGC(xs->display,
                                  plot->win, 
                                  GCFunction, 
                                  &xgccurs);

  /* Select events will be accepted by each window; notify window manager
     that we will handle window deletions, not it.  */
  
  
  /* Map the windows */

  XMapWindow(DISPLAY(base),  base->win);
  XMapWindow(DISPLAY(title), title->win);
  XMapWindow(DISPLAY(title), plot->win);

  /* Set the cursor to be the crosshair */

  cursor = XCreateFontCursor(DISPLAY(base), XC_crosshair);
  XDefineCursor(DISPLAY(base), base->win, cursor);

  num_wins3 = max(num_wins3, *win_num);

  /* Process events */

#ifndef USE_X11_DOUBLE_BUFFER
  dispatchevent3(nerr);
#endif /* USE_X11_DOUBLE_BUFFER */

}

