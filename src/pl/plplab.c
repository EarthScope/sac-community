/** 
 * @file plplab.c
 *
 * @brief Plot Labels
 *
 */
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "mach.h"
#include "pl.h"
#include "gem.h"

#include "gdm.h"

/** 
 * Write plot labels to the current frame
 *
 * @date   850208:  Added call to SETTEXTJUST.
 * @date   830209:  Original version.
 * @date   850208:  DOCUMENTED/REVIEWED
 * 
 */
void  
plplab() {
	int j;
	float angcur, chhts, chwids, xcur, ycur;
    label *p;
    
    xcur = 0.0;
    ycur = 0.0;

	/* -- Save current text size. */
	chwids = cmgem.chwid;
	chhts = cmgem.chht;

	/* - Set up text "current point." */

	settextangle( 0. );
	angcur = 0.;
	settextjust( "LEFT", "BOTTOM" );

	/* - For each label: */

	for( j = 1; j <= label_store_length() ; j++ ){ /* Number of labels */
        
          p = label_store_get(j);

          /* -- If label is to be plotted: */
          if( p->plot ) { 
            
            /* --- Set text size. */
            cmgem.chht = p->size;
            cmgem.chwid = cmgem.txrat*cmgem.chht;
            settextsize( cmgem.chwid, cmgem.chht );
            
            /* --- If location of label is to be "below" last label,
             *     calculate the position. */
            if( p->relative ) { 
              xcur = xcur + cmgem.chht*sin( TORAD*angcur );
              ycur = ycur - cmgem.chht*cos( TORAD*angcur );
            } else {
              /* --- Otherwise, use defined text location and orientation. */
              xcur = p->x;
              ycur = p->y * cmgem.view.ymax;
              angcur = p->angle;
              settextangle( angcur );
            }
            
            /* --- Finally ready to actually plot the label. */
            pltext( p->text, strlen(p->text), xcur, ycur );
            
          }
          
        }
        
	/* - Restore current character size. */
        
	settextsize( chwids, chhts );
	settextangle( 0. );

	return;
}

