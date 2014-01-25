/*******************************************************************************
 ** PURPOSE:
 *    To set the color table.
 *
 ** INPUT ARGUMENTS:
 *    win_num:      Window number to set table for. (Pointer)
 *    ncolors:      Number of entries in color table. (Pointer)
 *    red:          Array containing intensities of reds.  Range:  [0.0,1.0]
 *    green:        Array containing intensities of greens.  Range: [0.0,1.0]
 *    blue:         Array containing intensities of blues.  Range: [0.0,1.0]
 *
 ** GLOBAL OUTPUT:
 *    gd3.x11.h:  pixdef->(pixel, red, green, blue)
 *
 ** SUBROUTINES CALLED:
 *    XGetWindowAttributes,XAllocColorCells,XStoreColors
 *
 ** LOCAL VARIABLES:
 *    Status:  Status of color cells.
 *    i:       Loop counter.
 *    full:    'Fullest color' -- Colors are in range [0, full].
 *
 ** LIMITATIONS:
 *    - Maximum of 256 colors.
 *******************************************************************************
 ** MODIFICATION HISTORY:
 *    030227:  Fix for colors on 24 bit displays.  Intelligence provided by
 *             Bob Herrmann.  Implemented by Mike Firpo (maf).
 *    910318:  Added check for black and white monitor (ammon)
 *    890608:  Modified to run under X11 rather than X10.  (kjm)
 *    870316:  Changed call sequence and arrangement of color table.
 *    870310:  Original Version
 *******************************************************************************/

#include "gd3.x11.h"
#include "gam.h"
#include "debug.h"

#define FULL 65535.0 

void 
setctable3(int win_num,
           unsigned int nentry,
           float red[],
           float green[],
           float blue[])
{ 
	unsigned int i;
	Colormap colormap;
	XColor exact_def;        /* 030227, maf */
	XScreen *xs;
	
  UNUSED(win_num);
	if (nentry < 2)
		return;
	
	xs = xscreen_get();
	
	/*  check for depth (black and white = 1; color != 1)  */
	if (xs->depth != 1) {
		/* Scale red, green, blue to be in range [0, full]. */
		/* Colors are passed in with the first color representing the background */
		/* color and the last color representing the foreground color.  These    */
		/* need to be switched around to conform to X standards -- pixel value   */
		/* 0 representing foreground (black) and pixel value of 1 representing   */
		/* background (white).                                                   */
		
		colormap = DefaultColormap(xs->display, xs->screen);
		
		pixdef3[0].pixel = WhitePixel(xs->display, xs->screen);
		pixdef3[0].red   = FULL * red[0];
		pixdef3[0].green = FULL * green[0];
		pixdef3[0].blue  = FULL * blue[0];
		pixdef3[0].flags = DoRed | DoGreen | DoBlue;
		
		for (i = 1; i < nentry - 1; i++) {
			pixdef3[i].pixel = pixdef3[i - 1].pixel;
			pixdef3[i].red   = FULL * red[i];
			pixdef3[i].green = FULL * green[i];
			pixdef3[i].blue  = FULL * blue[i];
			pixdef3[i].flags = DoRed | DoGreen | DoBlue;
			
			exact_def.red    = pixdef3[i].red;
			exact_def.green  = pixdef3[i].green;
			exact_def.blue   = pixdef3[i].blue;
			
			XAllocColor(xs->display, colormap, &exact_def) ;
			
			pixdef3[i].pixel = exact_def.pixel;
			pixdef3[i].red   = exact_def.red;
			pixdef3[i].green = exact_def.green;
			pixdef3[i].blue  = exact_def.blue;
		}
		
		pixdef3[nentry - 1].pixel = BlackPixel(xs->display, xs->screen);
		pixdef3[nentry - 1].red   = FULL * red[nentry - 1];
		pixdef3[nentry - 1].green = FULL * green[nentry - 1];
		pixdef3[nentry - 1].blue  = FULL * blue[nentry - 1];
		pixdef3[nentry - 1].flags = DoRed | DoGreen | DoBlue;
		
		/* Store color table */
		XFlush(xs->display);
	} else {
		/*  black and white */
		pixdef3[0].pixel = WhitePixel(xs->display, xs->screen);
		for (i = 1; i < nentry - 1; i++) 
			pixdef3[i].pixel = BlackPixel(xs->display, xs->screen);
	}
	
	cmgam.cmap = MDEFAULT;
}
