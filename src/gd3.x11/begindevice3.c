/** 
 * @file begindevice3.c
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "amf.h"
#include "gd3.x11.h"
#include "gem.h"  
#include "bool.h"

static char *fontnames[] = { "9x15", "fixed", "6x13" };

#define NUMBER_OF_FONTS (sizeof(fontnames) / sizeof(char*))
/*
static char *fontnames_label[] = { "*helvetica-medium-r-normal--8*", 
                                   "9x15", "fixed", "6x13" };
*/
#define NUMBER_OF_FONTS_LABEL (sizeof(fontnames_label) / sizeof(char*))

Font  
load_font(Display *display, char *name) {
  Font font;
  XFontStruct *fontstruct;
  font = None;
  if( (fontstruct = XLoadQueryFont(display, name)) != NULL ) {
    font = XLoadFont(display, name);
  }
  return font;
}

XScreen *xscreen_default = NULL;

XScreen * 
xscreen_get() {
  if(xscreen_default) {
    return xscreen_default;
  }
  fprintf(stderr, "X11 Screen does not exist\n");
  return NULL;
}

void
xscreen_set(XScreen *xs) {
  xscreen_default = xs;
}

XScreen *
xscreen_init(XScreen *xs) {
  xs->display = XOpenDisplay(NULL);
  xs->active  = FALSE;
  if(!xs->display) {
    return xs;
  }
  xs->active = TRUE;
  xs->screen = DefaultScreen( xs->display);
  xs->visual = DefaultVisual( xs->display, xs->screen );
  xs->width  = DisplayWidth( xs->display, xs->screen );
  xs->height = DisplayHeight( xs->display, xs->screen );
  xs->root   = DefaultRootWindow( xs->display );
  xs->depth  = DefaultDepth( xs->display, xs->screen );

  return xs;
}

XScreen * 
xscreen_open() {
  XScreen *xs;
  xs = (XScreen *) malloc(sizeof(XScreen));
  if(!xs) {
    fprintf(stderr, "Error allocating memory for XScreen\n");
    return NULL;
  }
  return xscreen_init(xs);
}

/** 
 * To begin graphics to the XWindow device.
 *
 * @param nerr
 *    Error Return Code
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   890606:  Rewritten from X10 to X11.  (kjm)
 * @date   870317:  Added font initialization.
 * @date   860301:  Original Version
 *
 */
void 
begindevice3(int *nerr)
{
  int i;
  XScreen *xs;
  *nerr = 0;


  xs = xscreen_open();
  if(!xs || ! xs->active) {
    *nerr = 203;
    FREE(xs);
    return;
  }
  i = 0;
  while( ((title_font3 = load_font(xs->display, fontnames[i])) == None) && 
         i < (int)NUMBER_OF_FONTS ) {  
    i++;  
  }
  if(title_font3 == None) {
    fprintf(stderr, "SAC Error loading font for title\n");
  }

  xscreen_set( xs );
  
  set_skeleton_fudge( 0.00095 );
}

