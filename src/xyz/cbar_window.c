
#include "xyz.h"
#include "gdm.h"

#include "config.h"

#include "gd2.h"
#ifdef USE_X11
#include "gd3.x11.h"
#endif

void 
cbar_window(unsigned int xloc,
            unsigned int yloc,
            unsigned int height,
            unsigned int w_height,
            unsigned int w_width,
            float vspaceratio,
            float ypmax,
            int *nerr)
{

    *nerr = 0;

    if( Lgdon[2] )
        cbar_window2(xloc,yloc,height,w_height,w_width,vspaceratio,ypmax,nerr);
#ifdef X11_APPLICATION
    if( Lgdon[3] )
        cbar_window3(xloc,yloc,height,w_height,w_width,vspaceratio,ypmax,nerr);
#endif

    return;
} /* end of function */







