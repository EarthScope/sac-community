/*******************************************************************************
** PURPOSE:
*    To erase the window.
*
** GLOBAL INPUT:
*    gd3.x11.h:  plotw3->win
*
** SUBROUTINES CALLED:
*    XClearWindow, XFlush
*******************************************************************************/

#include <stdio.h>

#include "gd3.x11.h"

#include "config.h"

#include "gem.h"
#include "gdm.h"

GD3_EXTERN

extern XWindow plotw3[MAX_WINS + 1];

void
fill_background3(int window) {

    XWindow *w = plot_window(window);

    if(color_on()) {
        setcolor_bg();
    } else {
        setcolor_bg_def();
    }

    XSetForeground(DISPLAY(w), w->gc, w->color.pixel);
    XSetBackground(DISPLAY(w), w->gc, w->color.pixel);

    XFillRectangle(DISPLAY(w), w->buffer, w->gc, 0, 0, w->width, w->height);

    if(color_on()) {
        setcolor_fg();
    } else {
        setcolor_fg_def();
    }

}

void
erase3() {

/* Erase window */

    fill_background3(c_win3);

}

/*******************************************************************************
** MODIFICATION HISTORY:
*    890608:  Modified to run under X11 rather than X10.  (kjm)
*    870318:  Changes due to gd3.x10.h structure change.
*    870223:  Original Version
*******************************************************************************/
