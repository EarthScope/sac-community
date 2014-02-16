/*******************************************************************************
** PURPOSE:
*    To set the linestyle type.
*
** SUBROUTINES CALLED:
*    XChangeGC, XSetDashes
*
** INPUT ARGUMENTS:
*    linestyle:  Linestyle type.
*
******************************************************************************/

#include <stdio.h>

#include "gd3.x11.h"
#include "string/array.h"

#include "array.h"

void
line_style_x11(char *line, void *data) {
    float f;
    int n;
    char *v;
    char *buf;
    array_t *x11 = (array_t *) data;
    
    v = xarray_new('c');
    buf = line;
    while(buf && sscanf(buf, "%f%n", &f, &n) == 1) {
        v = xarray_append(v, (int)(f+0.5));
        buf += n;
    }
    array_append(x11, v);
}


void 
setlinestyle3(int *linestyle)
{
  XGCValues gcv;
  XWindow *xw;
  int dash_offset = 0;
  static array_t *dashes = NULL;
  char *dash;

  xw = plot_window( CURRENT );

  if ((*linestyle < 1) || (*linestyle > 10)) {
    *linestyle = 1;
  }
  if (*linestyle == 1) {
    gcv.line_style = LineSolid;
    XChangeGC(DISPLAY(xw), xw->gc, GCLineStyle, &gcv);
  } else {
    if(!dashes) {
        dashes = array_new();
        sac_line_style_read(line_style_x11, dashes);
    }
    dash = (char *) array_element(dashes, *linestyle-2);
    if(!dash) {
        return;
    }
    gcv.line_style = LineOnOffDash;
    XChangeGC(DISPLAY(xw), xw->gc, GCLineStyle, &gcv);
    XSetDashes(DISPLAY(xw), 
               xw->gc,
               dash_offset, 
               dash,
               xarray_length(dash));
  }
}

/*******************************************************************************
** MODIFICATION HISTORY:
*    890609:  Changed from NO-OP to operate under X11.  (kjm)
*    870227:  Original Version
*******************************************************************************/
