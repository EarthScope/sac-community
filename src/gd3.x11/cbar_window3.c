
#include "gd3.x11.h"
#include "gem.h"
#include "debug.h"

void cbar_window3(xloc,yloc,height,w_height,w_width,vspaceratio,ypmax,nerr)
unsigned int xloc,yloc,height,w_height,w_width;
float vspaceratio,ypmax;
int *nerr;
{
      *nerr = 0;
      UNUSED(yloc);

/*        cmgem.ypmx = 1.0 - ((float)yloc/(float)w_height); */
        cmgem.plot.ymax = ypmax;

        cmgem.plot.ymin = cmgem.plot.ymax - ((float)height/(float)w_height);

        cmgem.plot.xmin = (float)xloc/(float)w_width;

        cmgem.uplot.ymin = cmgem.plot.ymin*vspaceratio;
        cmgem.uplot.ymax = cmgem.plot.ymax*vspaceratio;

        cmgem.uplot.xmin = cmgem.plot.xmin;


	return ;

} /* end of function */






