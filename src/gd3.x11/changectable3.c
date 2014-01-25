/*******************************************************************************
** PURPOSE:
*    To change the auxiliary colors in the color table.
*
** INPUT ARGUMENTS:
*    icolortable: Requested auxiliary color table.  
** GLOBAL OUTPUT:
*    gd3.x11.h:  pixdef->(pixel, red, green, blue)
*
** SUBROUTINES CALLED:
*    XGetWindowAttributes,XAllocColorCells,XStoreColors
*
** LOCAL VARIABLES:
*    Status:  Status of color cells.
*    i:       Loop counter.
*    planes:  Plane mask.
*    pixels:  Pixel values.
*    full:    'Fullest color' -- Colors are in range [0, full].
*
** LIMITATIONS:
*    - Maximum of 256 colors.
*******************************************************************************
** MODIFICATION HISTORY:
*    940921:  Original Version
*******************************************************************************/

#define MGREY 1
#define MCOLOR 2

#include "gd3.x11.h"
#include "gdm.h"

#include "color.h"

void changectable3(nentry,icolortable)
  int nentry, icolortable;
{ 
  Status status;
  int i;
  unsigned long pixels[256];
  unsigned long masks[256];
  unsigned int nalloc;

  XScreen *xs;

  xs = xscreen_get();

  /*  check for depth (black and white = 1; color != 1)  */
  if(xs->depth != 1) {
    
    npscolors = cmgdm.npscimage;
    
    nalloc = nentry + npscolors + 7;
    
    for (i = nentry+7; i < (int)nalloc; i++){
      pixels[i-(nentry+7)] = pixdef3[i].pixel; 
    }
    
    XFreeColors(xs->display,colormap,pixels,npscolors,0);
    
    status = XAllocColorCells(xs->display,
                              colormap,
                              True,
                              masks,
                              0,pixels,npscolors);
    if (status != 0) {
      
        for (i = nentry+7; i < (int)nalloc; i++){
        pixdef3[i].pixel = pixels[i-(nentry+7)];
        
        if(icolortable == MCOLOR){
          pixdef3[i].red   = psred[i-(nentry+7)];
          pixdef3[i].green = psgreen[i-(nentry+7)];
          pixdef3[i].blue  = psblue[i-(nentry+7)];
        } else {
          pixdef3[i].red   = 256*(255-(i+1))-1;
          pixdef3[i].green = 256*(255-(i+1))-1;
          pixdef3[i].blue  = 256*(255-(i+1))-1;
        }
        pixdef3[i].flags = DoRed | DoGreen | DoBlue;
        
      }
      
      /* Store color table */
      XStoreColors(xs->display,colormap,&pixdef3[nentry+7],npscolors);
      XFlush(xs->display);

    }
  } else {
    /* should return an error here */
  }
}

