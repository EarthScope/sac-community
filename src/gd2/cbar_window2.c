
#include "gd2.h"
#include "gem.h"
#include "debug.h"

#define HT_ADJUST 32 /* these parameters also exist in gdm/adj_geometry */
#define WD_ADJUST 32 /* these parameters also exist in gdm/adj_geometry */


void cbar_window2(xloc,yloc,height,w_height,w_width,vspaceratio,ypmax,nerr)
unsigned int xloc,yloc,height,w_height,w_width;
float vspaceratio,ypmax;
int *nerr;
{
  *nerr = 0;
  UNUSED(yloc);
/*        cmgem.ypmx = 1.0 - ((float)yloc/((float)w_height*(float)HT_ADJUST));*/
        cmgem.plot.ymax = ypmax;

        cmgem.plot.ymin = cmgem.plot.ymax - ((float)height/(float)w_height);

        cmgem.plot.xmin = (float)xloc/((float)w_width*(float)WD_ADJUST);

/* is multiplication by vspaceratio necessary for gd2? */
        cmgem.uplot.ymin = cmgem.plot.ymin * vspaceratio;
        cmgem.uplot.ymax = cmgem.plot.ymax * vspaceratio;

        cmgem.uplot.xmin = cmgem.plot.xmin;

	return;

} /* end of function */






