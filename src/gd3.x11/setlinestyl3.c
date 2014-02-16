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
    char ***x11 = (char ***) data;
    
    v = xarray_new('c');
    buf = line;
    while(buf && sscanf(buf, "%f%n", &f, &n) == 1) {
      v = xarray_append(v, (char) ((int)(f+0.5)));
      buf += n;
    }
    *x11 = xarray_append(*x11, v);
}


void 
setlinestyle3(int *linestyle)
{
  XGCValues gcv;
  XWindow *xw;
  int dash_offset = 0;
  static char **dashes = NULL;
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
        dashes = xarray_new('p');
        sac_line_style_read(line_style_x11, &dashes);
    }
    dash = dashes[*linestyle-2];
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
