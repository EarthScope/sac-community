/** 
 * @file dispid.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dfm.h"
#include "hdr.h"
#include "gem.h"
#include "gam.h"
#include "co.h"
#include "gdm.h"
#include "pl.h"
#include "bot.h"
#include "dff.h"

/** 
 * Put file ID information on a current plot 
 *
 * @param ldfl
 *    - 0 Do Not plot file number
 *    - 1 Plot File number 
 * @param idfl
 *    File Number
 *
 * @date   970130:  Added ldfl and idfl for plotting of file number.  maf
 * @date   850515:  Outputs FILENAME if all header fields undefined.
 * @date   820324:  Changed character size to medium.
 *                  Now computing number of characters in each string.
 * @date   801018:  Changes to reflect header version 5.
 * @date   800922:  Modified station id format.
 * @date   800823:  Original version (factored out of P1 and PP).
 *
 */
void 
dispid (int ldfl,
        int idfl,
        int nlast,
        char **last)
{
        int i;
	int jfidnm, jfidnm_, jfidtx, jfidtx_, nc1, nc2, nerr;
	float slen1, slen1m, slen2, slen2m;
        char *strtemp;

        textbox *tbox;

	/* - Change text size. */
	cmgem.chht = cmgam.tsfid ;
	cmgem.chwid = cmgem.txrat*cmgem.chht;
	settextsize( cmgem.chwid, cmgem.chht );

        gettextsize( &cmgem.chwid, &cmgem.chht );

	/* - Compute location for id, even if standard fileid is not requested.
	 *   This is done so that auxiliary information can be written anyway. */

	cmgam.fidbdr = cmgem.chht;
	if( !cmgam.lfidrq ){
		if( cmgam.ifidlc == cmgam.iur ){
			cmgam.xfidlc = cmgem.uplot.xmax - cmgam.fidbdr - 16.0*cmgem.chwid;
			cmgam.yfidlc = cmgem.uplot.ymax - cmgam.fidbdr - cmgem.chht;
		}
		else if( cmgam.ifidlc == cmgam.iul ){
			cmgam.xfidlc = cmgem.uplot.xmin + cmgam.fidbdr;
			cmgam.yfidlc = cmgem.uplot.ymax - cmgam.fidbdr - cmgem.chht;
		}
		else if( cmgam.ifidlc == cmgam.ilr ){
			cmgam.xfidlc = cmgem.uplot.xmax - cmgam.fidbdr - 16.0*cmgem.chwid;
			cmgam.yfidlc = cmgem.uplot.ymin + cmgam.fidbdr + 4.0*cmgem.chht;
		}
		else if( cmgam.ifidlc == cmgam.ill ){
			cmgam.xfidlc = cmgem.uplot.xmin + cmgam.fidbdr;
			cmgam.yfidlc = cmgem.uplot.ymax + cmgam.fidbdr + 4.0*cmgem.chht;
		}
		settextjust( "LEFT", "BOTTOM" );
		goto L_8888;
	} /* end if( !cmgam.lfidrq ) */

	cmgam.nfidtx = 0;

        tbox = textbox_new( cmgam.nfidnm + nlast );


	for( jfidnm = 1; jfidnm <= cmgam.nfidnm; jfidnm++ ){
		jfidnm_ = jfidnm - 1;
		cmgam.nfidtx = cmgam.nfidtx + 1;
		formhv( (char*)kmgam.kfidnm[jfidnm_],9, cmgam.ifidfm, 
		  (char*)kmgam.kfidtx[cmgam.nfidtx - 1] ,41, &nerr );
		if( nerr != 0 ) {
                  cmgam.nfidtx = cmgam.nfidtx - 1;
                  tbox->text[jfidnm-1] = strdup(" ");
                  tbox->color[jfidnm-1] = color_foreground_default();
                } else {
                  tbox->text[jfidnm-1]  = fstrdup(kmgam.kfidtx[cmgam.nfidtx-1], 41);
                  tbox->color[jfidnm-1] = color_foreground_default();
                }
	}
        
	if( cmgam.nfidtx <= 0 ){
		cmgam.nfidtx = 1;
		formhv( "FILENAME",9, cmgam.ifidfm, (char*)kmgam.kfidtx[cmgam.nfidtx - 1]
		 ,41, &nerr );
                tbox->text[0]  = fstrdup(kmgam.kfidtx[cmgam.nfidtx-1], 41);
                tbox->color[0] = color_foreground_default();
	}
        if(nlast > 0) {
          for(i = 0; i < nlast; i++) {
            tbox->text[cmgam.nfidnm+i]  = strdup(last[i]);
            tbox->color[cmgam.nfidnm+i] = color_foreground_default();          
          }
        }

	if( cmgam.ifidfm == 1 ){
		slen1m = 0.;
		slen2m = 0.;
		for( jfidtx = 1; jfidtx <= cmgam.nfidtx; jfidtx++ ){
			jfidtx_ = jfidtx - 1;
			nc1 = indexc( (char*)kmgam.kfidtx[jfidtx_],41, '=' ) + 1;
			getstringsize( (char*)kmgam.kfidtx[jfidtx_], nc1, &slen1 );
			slen1m = fmax( slen1m, slen1 );
			nc2 = indexb( (char*)kmgam.kfidtx[jfidtx_],41 );

                        strtemp = malloc(42-(nc1+1));
                        strncpy(strtemp,kmgam.kfidtx[jfidtx_]+nc1,-(nc1 + 1) + 41);
                        strtemp[41-(nc1+1)] = '\0';

			getstringsize( strtemp, nc2 - nc1 + 1, &slen2 );

                        free(strtemp);
			slen2m = fmax( slen2m, slen2 );
		}
		if( cmgam.ifidlc == cmgam.iur ){
			cmgam.xfidlc = cmgem.uplot.xmax - cmgam.fidbdr - slen2m;
			cmgam.yfidlc = cmgem.uplot.ymax - cmgam.fidbdr - cmgem.chht;
		}
		else if( cmgam.ifidlc == cmgam.iul ){
			cmgam.xfidlc = cmgem.uplot.xmin + cmgam.fidbdr + slen1m;
			cmgam.yfidlc = cmgem.uplot.ymax - cmgam.fidbdr - cmgem.chht;
		}
		else if( cmgam.ifidlc == cmgam.ilr ){
			cmgam.xfidlc = cmgem.uplot.xmax - cmgam.fidbdr - slen2m;
			cmgam.yfidlc = cmgem.uplot.ymin + cmgam.fidbdr + 
			  (float)( cmgam.nfidtx - 1 )*cmgem.chht;
		}
		else if( cmgam.ifidlc == cmgam.ill ){
			cmgam.xfidlc = cmgem.uplot.xmin + cmgam.fidbdr + slen1m;
			cmgam.yfidlc = cmgem.uplot.ymin + cmgam.fidbdr + 
			  (float)( cmgam.nfidtx - 1 )*cmgem.chht;
		}
		else{
			cmgam.xfidlc = cmgem.uplot.xmin + cmgam.fidbdr + slen1m;
			cmgam.yfidlc = cmgem.uplot.ymax - cmgam.fidbdr - cmgem.chht;
		}
		for( jfidtx = 1; jfidtx <= cmgam.nfidtx; jfidtx++ ){
			jfidtx_ = jfidtx - 1;
			settextjust( "RIGHT", "BOTTOM" );
			nc1 = indexc( (char*)kmgam.kfidtx[jfidtx_],41, '=' ) + 1;

                        strtemp = malloc(nc1+1);
                        strncpy(strtemp,kmgam.kfidtx[jfidtx_], nc1);
                        strtemp[nc1] = '\0';

			pltext( strtemp, nc1+1, cmgam.xfidlc, cmgam.yfidlc );

                        free(strtemp);

			settextjust( "LEFT", "BOTTOM" );
			nc2 = indexb( (char*)kmgam.kfidtx[jfidtx_],41 );

                        strtemp = malloc(nc2-(nc1+1)+2);
                        strncpy(strtemp,kmgam.kfidtx[jfidtx_]+nc1, nc2 - 
			 (nc1 + 1) + 1);
                        strtemp[nc2-(nc1+1)+1] = '\0';

			pltext( strtemp, nc2 - (nc1 + 1) + 2, cmgam.xfidlc, 
			 cmgam.yfidlc );

                        free(strtemp);
			cmgam.yfidlc = cmgam.yfidlc - cmgem.chht;
		}
	}
	else{
		if( cmgam.ifidlc == cmgam.iur ){
                        tbox->x = cmgem.uplot.xmax - cmgam.fidbdr;
                        tbox->y = cmgem.uplot.ymax - cmgam.fidbdr;
                        tbox->location = TEXT_BOX_UPPER | TEXT_BOX_RIGHT;
		}
		else if( cmgam.ifidlc == cmgam.iul ){
                        tbox->x = cmgem.uplot.xmin + cmgam.fidbdr;
                        tbox->y = cmgem.uplot.ymax - cmgam.fidbdr;
                        tbox->location = TEXT_BOX_UPPER | TEXT_BOX_LEFT;
		}
		else if( cmgam.ifidlc == cmgam.ilr ){
                        tbox->x = cmgem.uplot.xmax - cmgam.fidbdr;
                        tbox->y = cmgem.uplot.ymin + cmgam.fidbdr;
                        tbox->location = TEXT_BOX_LOWER | TEXT_BOX_RIGHT;
		}
		else if( cmgam.ifidlc == cmgam.ill ){
                        tbox->x = cmgem.uplot.xmin + cmgam.fidbdr;
                        tbox->y = cmgem.uplot.ymin + cmgam.fidbdr;
                        tbox->location = TEXT_BOX_LOWER | TEXT_BOX_LEFT;
		}
		else{
                        tbox->x = cmgem.uplot.xmin + cmgam.fidbdr;
                        tbox->y = cmgem.uplot.ymax - cmgam.fidbdr;
                        tbox->location = TEXT_BOX_UPPER | TEXT_BOX_LEFT;
		}

		settextjust( "LEFT", "BOTTOM" );
                
                textbox_show( tbox );
                textbox_free( tbox );
                tbox = NULL;
	}

	/* Plot file number if appropriate.  maf 970130 */
	if ( ldfl ) {
		char kdfl[ 4 ] ;
		float xPosition, yPosition ;
		float minHeight = 0.0111866 , minWidth = 0.007458103 ;

		/* make sure characters are big enought to be read. */
		if ( cmgem.chht < minHeight || cmgem.chwid < minWidth )
		    settextsize( minWidth, minHeight );

		sprintf ( kdfl , "%d", idfl ) ;

		xPosition = ( cmgem.view.xmax - cmgem.view.xmin ) * 0.92 + cmgem.view.xmin ;
		yPosition = cmgam.yfidlc + cmgem.chht ;

		pltext ( kdfl , strlen ( kdfl ) + 1 , xPosition , yPosition ) ;
	}

L_8888:
	return;


}

