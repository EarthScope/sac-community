/** 
 * @file   pltplr.c
 * 
 * @brief  Polar Plot
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dbh.h"
#include "bool.h"
#include "co.h"
#include "gam.h"
#include "gem.h"
#include "gdm.h"
#include "pl.h"
#include "gtm.h"


/** 
 * Polar Plot
 * 
 * @param s 
 * @param naz 
 * @param ninc 
 * @param levels 
 * @param scalng 
 *    - 'DB'
 *    - 'LINEAR'
 * @param lprint 
 *    Whether to print the plot
 * @param label 
 *    Label for the plot
 * @param label_s 
 *    Length of \p label
 * @param title 
 *    Title for the plot
 * @param title_s 
 *    Length of \p title
 *
 * @see cpolar()
 *
 */
void
pltplr(float *s, 
       int    naz, 
       int    ninc, 
       int    levels, 
       char  *scalng, 
       int    lprint, 
       char  *label, 
       int    label_s, 
       char  *title, 
       int    title_s) {

	int lany, lfullsav;
	int _l0, i, iend_, lenlab, nerr, np, ptr;
	float angle, aspect, ratiosav, side, smax_, 
	 txtxps, txtyin, txtyps, xd, xleft, xright, xym, ybot, yd, ytop, 
	 zhigh, zlow;
        char *strtemp;
        char *csave;

	float *const S = &s[0] - 1;





	/* - If no graphics device is open, try to open the default device.
	 * */
	getstatus( "ANY", &lany );
	if( !lany ){
		begindevice( kmgam.kgddef,9, &nerr );
		if( nerr != 0 )
			goto L_9999;
		}

	/* - Save current plot environment. */

	getvspacetype( &lfullsav, &ratiosav );
	plsave();
	setvspacetype( TRUE, 1.0 );

	/* - Begin new frame if requested. */

	if( cmgem.lframe ){
		beginframe( lprint , &nerr );
		if( nerr != 0 )
			goto L_8888;
		}

	/*  Set up world coordinates                                                     
	 * */
	getratio( &aspect );
	xym = .07;
	if( aspect <= 1.0 ){
		xd = .7 - 2.*xym;
		yd = aspect - 2.*xym;
		side = fmin( xd, yd );
		xleft = .3 + .5*(.7 - 2.*xym - side);
		xright = xleft + side;
		ybot = xym + .5*(aspect - 2.*xym - side);
		ytop = ybot + side;
		}
	else{
		xd = 1.0 - 2.*xym;
		yd = .7*aspect - 2.*xym;
		side = fmin( xd, yd );
		/*         XLEFT =XY_MARGIN + .5*(1.0-2.*XY_MARGIN-SIDE)                          */
		xleft = (xym) + (.5*(1.0 - (2.*xym) - side));
		xright = xleft + side;
		ybot = xym + .5*(.7*aspect - 2.*xym - side);
		ytop = ybot + side;
		}
	setvport( xleft, xright, ybot, ytop );
	setworld( -1.0, 1.0, -1.0, 1.0 );

	/*  Find max and min of data                                                     
	 * */
	np = naz*ninc;
	smax_ = 0.;
	for( i = 1; i <= np; i++ ){
		smax_ = fmax( smax_, S[i] );
		}
	zhigh = smax_;
	zlow = 0.;

	/*  Log scaling, if requested                                                    
	 *
	 *        IF (  EXISTS( SCALNG, 'DB' ) ) THEN   */
	if( memcmp(scalng,"DB",2) == 0 ){
		for( i = 1; i <= np; i++ ){
			S[i] = 10.*log10( fmax( S[i], 0.000001*smax_ ) );
			}
		zhigh = 10.*log10( smax_ );
		zlow = zhigh - 60.;
		}

	/*  Call contouring routine                                                      
	 * */
	cpolar( s, ninc, naz, levels, zlow, zhigh );

	/*  Add grid                                                                     
	 * */
	setcolor( 3 );
	setlinestyle( 2 );
	worldsector( 0.0, 0.0, 0.8/3., 0., 360., 1. );
	worldsector( 0.0, 0.0, 2.*0.8/3., 0., 360., 1. );
	for( i = 0; i <= 11; i++ ){
		angle = M_PI / 6.*(float)( i );
		worldmove( 0.0, 0.0 );
		worlddraw( 0.8*sin( angle ), 0.8*cos( angle ) );
		}
	setlinestyle( 1 );

	/*  Labelling                                                                    
	 * */
	if( aspect <= 1.0 ){
		settextsize( 0.014, 0.021 );
		}
	else{
		settextsize( 0.028, 0.042 );
		}
	settextfont( 4 );
	settexttype( "SOFTWARE" );

	/*  Label axes                                                                   
	 * */
	setcolor( 2 );
	settextjust( "CENTER", "BOTTOM" );
	worldmove( 0.0, 0.82 );
	text( "NORTH",6, 5 );
	settextjust( "LEFT", "CENTER" );
	worldmove( 0.82, 0.00 );
	text( "EAST",5, 4 );

	/*    Title                                                                      
	 * */
	setcolor( 7 );
	settextjust( "CENTER", "CENTER" );
	txtxps = .5;
	txtyps = .9*aspect;
	move( txtxps, txtyps );
	/*	CALL TEXT( TITLE, LENGTH(TITLE) )                                     */


        _l0 = (int) (strstr(title,"$") - title);
	text( title,title_s, _l0 );

	/*    Label to left side of plot                                                 
	 * */
	if( aspect <= 1.0 ){

		txtxps = 0.10;
		txtyps = .8*aspect;
		settextjust( "LEFT", "CENTER" );

		}
	else{

		txtxps = .5;
		txtyps = .8*aspect;
		settextjust( "CENTER", "CENTER" );

		}
	txtyin = 0.04;

	ptr = 1;
	lenlab = (label_s - 1);
	/*        LENLAB = LENGTH( LABEL )                                         */
L_4:
	;
	if( ptr > lenlab )
		goto L_5;


        iend_ = 0;
        csave = memchr(label,'$',lenlab);
        if(csave != NULL ) iend_ = (int)(csave - label + 1);

	if( iend_ > 0 ){
		move( txtxps, txtyps );
                
                strtemp = malloc(iend_-ptr+1);
                strncpy(strtemp,label+ptr - 1,iend_-ptr);
                strtemp[iend_-ptr] = '\0';

		text( strtemp, iend_-ptr+1, (iend_ - ptr) );

                free(strtemp);

		txtyps = txtyps - txtyin;
		ptr = iend_ + 1;
		label[iend_ - 1] = ' ';
		}
	else{
		goto L_6;
		}

	goto L_4;
L_5:
	;
L_6:
	;

	if( memcmp(scalng,"LINEAR",6) == 0 ){
		move( txtxps, txtyps );
		text( "Linear contour spacing",23, 22 );
		}
	else if( memcmp(scalng,"DB",2) == 0 ){
		move( txtxps, txtyps );
		text( "Log contour spacing (dB)",25, 24 );
		}

	/* - Home cursor and end frame if requested. */

	plhome();
	if( cmgem.lframe )
		endframe( FALSE , &nerr );
        else 
          flushbuffer(&nerr);
          
	/* - Restore plot environment and return. */

L_8888:
	plrest();
	settextjust( "LEFT", "BOTTOM" );
	setvspacetype( lfullsav, ratiosav );

L_9999:
	return;

} /* end of function */

