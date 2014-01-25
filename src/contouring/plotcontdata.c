/** 
 * @file   plotcontdata.c
 * 
 * @brief  Make a contour plot
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mach.h"
#include "contouring.h"
#include "bool.h"
#include "bot.h"
#include "co.h"


#include "gdm.h"
#include "ucf.h"

void 
plotcontdata(zdata, nxsize, nysize, nerr)
float zdata[];
int nxsize, nysize, *nerr;
{
	char kformat[9];
	int lfast;
	int ilabel, j, jlevel, jlevel_, jzregion, nc;
	float heightsave, unused, widthsave, zincrement, zlevel, zmaximum, 
	 zminimum;

	/*=====================================================================
	 * PURPOSE:  To make a contour plot in the current viewport of a two
	 *           dimensional data array, using current contour attributes.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    zdata:   Two-dimensional array of z data. [fa]  
	 *    nxsize:  Number of values in the x direction (rows.) [i]
	 *    nysize:  Number of values in the y direction (columns.) [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error return flag. Set to 0 if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL: contouring/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    contouring:  klevelmode, nzlevellist, zlevellist, nznumlevels,
	 *                 zlevelminimum, zlevelmaximum, zlevelincrement,
	 *                 klinemode, klinetype, nlinelist, linelist, klistname,
	 *                 nzregionlist, zregionlist, ktickmode, klabelmode
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    mach:
	 *    contouring:  nzlevels, zlevels, nlines, lines
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:  FastContData, CalcContSegs, MergeContSegs, CalcContTicks
	 *          PlotContSegs, ListContSegs, 
	 *          ReleaseSegments, ReleasePoints, ReleaseLabels,
	 *          ljust, gettextjust, settextjust, gettextsize, settextsize
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900418:  Added labeling and segment list options.
	 *    900406:  Added tick mark options.
	 *    900312:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900406
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Determine list of contour levels to use:
	 * -- from fixed list set by user
	 * -- or from the range requested by user
	 * -- or scaled to the data itself. */

	if( strcmp(kmcontouring.klevelmode,"LIST    ") == 0 ){
		cmcontouring.nzlevels = cmcontouring.nzlevellist;
		for( j = 1; j <= cmcontouring.nzlevellist; j++ ){
			Zlevels[j] = Zlevellist[j];
			}
		}
	else{
		if( strcmp(kmcontouring.klevelmode,"RANGE   ") == 0 ){
			zminimum = cmcontouring.zlevelminimum;
			zmaximum = cmcontouring.zlevelmaximum;
			}
		else{
			extrma( zdata, 1, nxsize*nysize, &zminimum, &zmaximum, 
			 &unused );
			}
		if( cmcontouring.zlevelincrement > 0.0 ){
			zincrement = cmcontouring.zlevelincrement;
			zminimum = zincrement*(float)( (int)( zminimum/zincrement ) );
			zmaximum = zincrement*(float)( (int)( zmaximum/zincrement ) + 
			 1 );
			cmcontouring.nzlevels = min( (int)( (zmaximum - zminimum)/
			 zincrement ) + 1, MZLEVELS );
			}
		else{
			cmcontouring.nzlevels = cmcontouring.nznumlevels;
			zincrement = (zmaximum - zminimum)/(float)( cmcontouring.nzlevels - 
			 1 );
			}
		zlevel = zminimum;
		for( j = 1; j <= cmcontouring.nzlevels; j++ ){
			Zlevels[j] = zlevel;
			zlevel = zlevel + zincrement;
			}
		}

	/* - Determine linestyles to use, either from the user requested list
	 *   or from the list of user specified zregions. */

	if( strcmp(kmcontouring.klinemode,"ON      ") == 0 ){
		if( strcmp(kmcontouring.klinetype,"LIST    ") == 0 ){
			cmcontouring.nlines = cmcontouring.nlinelist;
			for( j = 1; j <= cmcontouring.nlinelist; j++ ){
				Lines[j] = Linelist[j];
				}
			}
		else{
			cmcontouring.nlines = cmcontouring.nzlevels;
			jzregion = 1;
			for( j = 1; j <= cmcontouring.nzlevels; j++ ){
				if( Zlevels[j] <= Zregionlist[jzregion] ){
					Lines[j] = Linelist[jzregion];
					}
				else{
					Lines[j] = Linelist[jzregion + 1];
					jzregion = min( jzregion + 1, cmcontouring.nzregionlist );
					}
				}
			}
		}

	/* - Determine contour labels if requested. */

	if( strcmp(kmcontouring.klabelmode,"ON      ") == 0 ){
		ilabel = 0;
		for( jlevel = 1; jlevel <= cmcontouring.nzlevels; jlevel++ ){
			jlevel_ = jlevel - 1;

			/* -- Determine which element in label list to use. */
			ilabel = ilabel + 1;
			if( ilabel > cmcontouring.nlabellist )
				ilabel = 1;

			/* --- If "ON", use free format encoding. */
			if( strcmp(kmcontouring.klabellist[ilabel - 1],"ON              ") == 
			 0 ){
				sprintf( kmcontouring.klabel[jlevel_], "%g ", Zlevels[jlevel] );
				/* --- If "OFF", pass this on to the rendering subroutines. */
				}
			else if( strcmp(kmcontouring.klabellist[ilabel - 1],"OFF             ") == 
			 0 ){
				strcpy( kmcontouring.klabel[jlevel_], "OFF             " );
				/* --- If "INT", use floating point conversion and delete the period. */
				}
			else if( memcmp(kmcontouring.klabellist[ilabel - 1]
			 ,"INT",3) == 0 ){
                                sprintf(kmcontouring.klabel[jlevel_],"%14.0f",Zlevels[jlevel] );
				nc = indexa( (char*)kmcontouring.klabel[jlevel_],17, 
				 '.', FALSE, TRUE );
				if( nc > 0 )
					subscpy( kmcontouring.klabel[jlevel_], nc - 
					 1, -1, 16, " " );
				goto L_4000;

				/* --- If "FLOAT", use floating point conversion. */
				}
			else if( memcmp(kmcontouring.klabellist[ilabel - 1]
			 ,"FLOAT",5) == 0 ){
				if( kmcontouring.klabellist[ilabel - 1][5] != ' ' ){
					kmcontouring.kdecimal = kmcontouring.klabellist[ilabel - 1][5];
					}
				else{
					ilabel = ilabel + 1;
					kmcontouring.kdecimal = kmcontouring.klabellist[ilabel - 1][0];
					}

                                memset(kformat,(int)' ',8);
                                kformat[8] = '\0';
                                memcpy(kformat,"%14.",4);
                                kformat[4] = kmcontouring.kdecimal;
                                kformat[5] = 'f';

                                sprintf( kmcontouring.klabel[jlevel_], kformat, Zlevels[jlevel]); 
				}
			else if( memcmp(kmcontouring.klabellist[ilabel - 1]
			 ,"EXP",3) == 0 ){
				if( kmcontouring.klabellist[ilabel - 1][3] != ' ' ){
					kmcontouring.kdecimal = kmcontouring.klabellist[ilabel - 1][3];
					}
				else{
					ilabel = ilabel + 1;
					kmcontouring.kdecimal = kmcontouring.klabellist[ilabel - 1][0];
					}

                                memset(kformat,(int)' ',8);
                                kformat[8] = '\0';
                                memcpy(kformat,"%14.",4);
                                kformat[4] = kmcontouring.kdecimal;
                                kformat[5] = 'e';

                                sprintf( kmcontouring.klabel[jlevel_], kformat, Zlevels[jlevel]); 

				/* --- If anything else, the text itself becomes the label. */
				}
			else{
				strcpy( kmcontouring.klabel[jlevel_], kmcontouring.klabellist[ilabel - 1]
				  );
				}

			/* -- Left justify the encoded text. */
L_4000:
			ljust( (char*)kmcontouring.klabel[jlevel_],17 );
			}

		}

	/* - Determine whether we do it the fast way or the nicer but slower way. */

	lfast = TRUE;
	if( strcmp(kmcontouring.ktickmode,"ON      ") == 0 ){
		lfast = FALSE;
		}
	else if( strcmp(kmcontouring.klabelmode,"ON      ") == 0 ){
		lfast = FALSE;
		}
	else if( strcmp(kmcontouring.klinemode,"OFF     ") == 0 ){
		lfast = FALSE;
		}
	else{
		for( j = 1; j <= cmcontouring.nlines; j++ ){
			if( Lines[j] != 1 ){
				lfast = FALSE;
				goto L_7100;
				}
			}
L_7100:
		;
		}

	/* - In fast mode do a simple scan, drawing lines as we go. */

	if( lfast ){
		fastcontdata( zdata, nxsize, nysize, nerr );

		/* - In nice mode, calculate segments, merge them into continues
		 *   segments, and then plot them. This allows more linestyles
		 *   and (later) the labeling and smoothing on contour segments. */

		}
	else{
		calccontsegs( zdata, nxsize, nysize, nerr );
		if( *nerr != 0 )
			goto L_8888;
		mergecontsegs();
        gettextsize( &widthsave, &heightsave );
		if( strcmp(kmcontouring.ktickmode,"ON      ") == 0 )
			calccontticks();
		if( strcmp(kmcontouring.klabelmode,"ON      ") == 0 ){
			settextsize( cmcontouring.widthlabels, cmcontouring.heightlabels );
			calccontrlinks();
			calccontlabels();
			}
		plotcontsegs();
		if( memcmp(kmcontouring.klistname,"OFF",3) != 0 )
			listcontsegs();
		releasesegments();
		releasepoints();
		if( strcmp(kmcontouring.klabelmode,"ON      ") == 0 ){
			releaselabels();
			settextsize( widthsave, heightsave );
			}
		}

L_8888:

	return;

} /* end of function */

