/** 
 * @file   gd3.aux.h
 * 
 * @brief  Extra Things for X11, which also appear in gd5.gui
 * 
 */

#ifndef _GD3_AUX_H_
#define _GD3_AUX_H_

#define ZERO 0
#ifndef OW_OFFSET
#define OW_OFFSET ZERO
#endif

#define MAX_WINS     10  
#define AVAILABLE    0
#define UNAVAILABLE  1

typedef struct {int x, y;} point;

Colormap colormap;
int npscolors;
unsigned long blackpixel;
unsigned long whitepixel;


#endif /* GD3_AUX_H_ */
