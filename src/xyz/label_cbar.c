
#include <stdio.h>

#include "xyz.h"
#include "gem.h"
#include "bool.h"


#include "gdm.h"
#include "pl.h"

void 
label_cbar(float xloc,
           float yloc,
           float width,
           float height,
           float dmin,
           float dmax,
           int *nerr)
{

       int lrigaxsave, llefaxsave, lrigtcsave, lleftcsave,
	    lylabsave, ltitlsave;
       float ypmxsave, ypmnsave, xpmxsave, xpmnsave;
       int llydivsave, llnydivsave, nydivsave, lylimsave, lygrdsave;
       float yimnzsave, yimxzsave, ypmnusave, ypmxusave, xpmnusave;
       float xpmxusave;
       float vspaceratio;
  
	/*=====================================================================
	 * PURPOSE:
	 *        
         *        
         *        
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error return flag.  Set to 0 if no error occurred.
	 *        
	 *=====================================================================
	 * MODULE/LEVEL:  gam/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    dfm:     ndfl, sacmem
	 *    hdr:     begin, ennd, delta
	 *    gem:     lbotax, lbottc, ltopax, ltoptc, lxlab, lylab, ltitl,
	 *             lxgrd, ypmn, ypmx, chht, tsdef
	 *    gam:     kgddef
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * MODIFICATION HISTORY:
         * 
	 *=====================================================================
	 * DOCUMENTED/REVIEWED: 
	 *===================================================================== */
	/* PROCEDURE: */

	*nerr = 0;

	lrigaxsave = cmgem.axis[RIGHT].annotate;
        llefaxsave = cmgem.axis[LEFT].annotate;
        lrigtcsave = cmgem.axis[RIGHT].ticks;
        lleftcsave = cmgem.axis[LEFT].ticks;
        llydivsave = cmgem.ydiv_spacing_on;
        llnydivsave = cmgem.ydiv_number_on;
        lygrdsave   = cmgem.lygrd;
        nydivsave  = cmgem.ydiv_number;

        cmgem.ydiv_number_on = TRUE;
        cmgem.ydiv_number = 4;
        cmgem.ydiv_spacing_on = FALSE;
        cmgem.axis[RIGHT].annotate = FALSE;
        cmgem.axis[LEFT].annotate  = TRUE;
        cmgem.axis[RIGHT].ticks    = FALSE;
        cmgem.axis[LEFT].ticks     = TRUE;
        cmgem.lygrd = FALSE;

	lylabsave = cmgem.ylabel.on;
	ltitlsave = cmgem.title.on;

	cmgem.ylabel.on = TRUE;
	cmgem.title.on = FALSE;

	ypmxsave = cmgem.plot.ymax;
        ypmnsave = cmgem.plot.ymin;
        xpmxsave = cmgem.plot.xmax;
        xpmnsave = cmgem.plot.xmin;

        ypmnusave = cmgem.uplot.ymin;
        ypmxusave = cmgem.uplot.ymax;
        xpmnusave = cmgem.uplot.xmin;
        xpmxusave = cmgem.uplot.xmax;

        getratio(&vspaceratio);

        {
          cmgem.plot.xmin = xloc;
          cmgem.plot.xmax = xloc + width;
          cmgem.plot.ymax = yloc;
          cmgem.plot.ymin = yloc - height;
          
          cmgem.uplot.xmin = cmgem.plot.xmin;
          cmgem.uplot.xmax = cmgem.plot.xmax;
          cmgem.uplot.ymin = cmgem.plot.ymin * vspaceratio;
          cmgem.uplot.ymax = cmgem.plot.ymax * vspaceratio;
        }

        /* calculate the window for ylinax to use in labeling the colorbar */

        /*
          cbar_window(xloc,yloc,height,w_height,w_width,vspaceratio, ypmax, nerr);
        */      
       	/* --- Set up y axis plot limits. */

        lylimsave = cmgem.lylim;
        yimnzsave = cmgem.zdata.ymin;
        yimxzsave = cmgem.zdata.ymax;

       	cmgem.lylim = TRUE;
       	cmgem.zdata.ymin = dmin;
       	cmgem.zdata.ymax = dmax;

     	cmgem.chht = cmgem.tsaxis;
       	cmgem.chwid = cmgem.txrat*cmgem.chht;
       	settextsize( cmgem.chwid, cmgem.chht );

        plcalwvtrans();

	ylinax();

   
        cmgem.axis[RIGHT].annotate = lrigaxsave;
        cmgem.axis[LEFT].annotate  = llefaxsave;
        cmgem.axis[RIGHT].ticks    = lrigtcsave;
        cmgem.axis[LEFT].ticks     = lleftcsave;

        cmgem.lygrd  = lygrdsave;
        cmgem.ydiv_spacing_on = llydivsave;
        cmgem.ydiv_number_on = llnydivsave;
        cmgem.ydiv_number = nydivsave;

        cmgem.ylabel.on = lylabsave;
        cmgem.title.on = ltitlsave;

	cmgem.plot.ymax = ypmxsave;
        cmgem.plot.ymin = ypmnsave;
        cmgem.plot.xmax = xpmxsave;
        cmgem.plot.xmin = xpmnsave;

        cmgem.uplot.ymin = ypmnusave;
        cmgem.uplot.ymax = ypmxusave;
        cmgem.uplot.xmin = xpmnusave;
        cmgem.uplot.xmax = xpmxusave;

        cmgem.lylim = lylimsave;
        cmgem.zdata.ymin = yimnzsave;
        cmgem.zdata.ymax = yimxzsave;

        
	return;
} /* end of function */

