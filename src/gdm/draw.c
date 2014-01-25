
#include "gdm.h"
#include "bot.h"

#define	IABOVE	8
#define	IBELOW	4
#define	ILEFT	1
#define	INSIDE	0
#define	IRIGHT	2

/** 
 * Draw from the current point to the requested location
 *
 * @param xloc
 *   New x locatin in viewspace
 * @param yloc
 *   New y locatin in viewspace
 *
 * @date  870501:   Added option to disable viewspace clipping.
 * @date  860910:   Added viewspace clipping.
 * @date  830523:   Original version.
 */
void 
draw(float xloc,
     float yloc)
{
	int iloc, ixyloc[2], unused;
	float xline[2], yline[2];

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	int *const Ixyloc = &ixyloc[0] - 1;
	float *const Xline = &xline[0] - 1;
	float *const Yline = &yline[0] - 1;


         /* LOCAL VARIABLES:
	 *   xline:    X locations of line from current point to input point.
	 *   yline:    Y locations of line from current point to input point.
	 *   ixyloc:   Locations of line segment relative to viewspace.
	 *   iabove:   Value indicating a point is above the viewspace.
	 *   ibelow:   Value indicating a point is below the viewspace.
	 *   iright:   Value indicating a point is to right of viewspace.
	 *   ileft:    Value indicating a point is to left of viewspace.
	 *   inside:   Value indicating a point is inside viewspace.
         */

	/* - If viewspace clipping is off, send draw to active devices. */
	if( !cmgdm.lvsclip ){
          for(i = 0; i < n; i++) {
            if(dev[i]->on && dev[i]->draw) {
              dev[i]->draw(xloc, yloc);
            }
          }
	} else {
          /* - If viewspace clipping is on: */
          /* -- Determine location of data point relative to viewspace. */
	    iloc = INSIDE;
	    if( yloc > Yvs[2] )
		iloc = iloc + IABOVE;
	    else if( yloc < Yvs[1] )
		iloc = iloc + IBELOW;
	    if( xloc > Xvs[2] )
		iloc = iloc + IRIGHT;
	    else if( xloc < Xvs[1] )
		iloc = iloc + ILEFT;


	    /* -- There are three cases to handle:
	     *    (1) Entire line segment is inside viewspace:
	     *        Move has already been done. Do draw. */

	    if( (cmgdm.iold + iloc) == INSIDE ){
              for(i = 0; i < n; i++) {
                if(dev[i]->on && dev[i]->draw) {
                  dev[i]->draw(xloc, yloc);
                }
              }
            }

	    /*    (2) Part of line segment is inside viewspace:
	     *        Clip line segment to viewspace. Do move and draw. */
	    else if( ( cmgdm.iold & iloc ) == 0 ){
		Xline[1] = cmgdm.xold;
		Xline[2] = xloc;
		Yline[1] = cmgdm.yold;
		Yline[2] = yloc;
		Ixyloc[1] = cmgdm.iold;
		Ixyloc[2] = iloc;
		clipdp( xline, yline, ixyloc, cmgdm.xvs, cmgdm.yvs, &unused );

                for(i = 0; i < n; i++) {
                  if(dev[i]->on && dev[i]->draw && dev[i]->move) {
                    dev[i]->move(Xline[1], Yline[1]);
                    dev[i]->draw(Xline[2], Yline[2]);
                  }
                }
	    }

	    /*    (3) Entire line segment is outside viewspace: Do nothing. */

	    /* Save iloc information */
	    cmgdm.iold = iloc;
	}

	/* - Set current point to data point. */

	cmgdm.xold = xloc;
	cmgdm.yold = yloc;

	return;

}

