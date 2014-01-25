/** 
 * @file   pcmrpl.c
 * 
 * @brief  Cursor Command
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "complex.h"
#include "proto.h"
#include "mach.h"
#include "gam.h"
#include "gem.h"
#include "bool.h"

#include "string_utils.h"


#include "gtm.h"
#include "bot.h"
#include "ucf.h"
#include "gdm.h"
#include "co.h"
#include "gam.h"

#define	MCTEXT	80

void 
pcmrpl(FILE   *nunmac, 
       double  scale, 
       double  angle) {

	char kjunk[9], ktext[MCMSG+1];
	int lend, lquit;
	char  kchar, kchar2;
	int i1, i2, iop1, iop2, iope, iopei, j, jope, nc, nctext, 
	 nerr, numchar;
	float cosang, height, sinang, theight, twidth, width, xtemp, ytemp;
	static char kbdlin[26] = "Bad line in replay file: ";
        char *strtemp1, *strtemp2;


	/*=====================================================================
	 * PURPOSE:
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *=====================================================================
	 * MODULE/LEVEL:
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:    MUNOUT
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *=====================================================================
	 * GLOBAL COUPLING:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * MODIFICATION HISTORY:
         *    970129:  Add parameter (0) to cnvati.  0 means that if a string
         *             of digits is too long, let it slide by.  maf 
	 *    870924:  Set up logic to scale text.
	 *    860106:  Modified rectangle drawing logic to correctly
	 *             handle rotation of rectangle inside a macro.
	 *    810000:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Set center of macro (CEN) to current data point (CDP). */
	cmgam.xcen = cmgam.xcdp;
	cmgam.ycen = cmgam.ycdp;

	/* - Compute rotation constants. */

	cosang = cos( TORAD*angle );
	sinang = sin( TORAD*angle );

	/* - Get and save current text size.
	 *   Scale text size to macro scaling factor.
	 *   Convert this size to world coordinates. */

	gettextsize( &width, &height );
	twidth = scale*width;
	theight = scale*height;
	settextsize( twidth, theight );
	vporttoworld( twidth, theight, &twidth, &theight );

L_5000:
	;

	/* - Set origin (ORI) to current data point (CDP) if requested. */

	if( !cmgam.lglori ){
		cmgam.xori = cmgam.xcdp;
		cmgam.yori = cmgam.ycdp;
		}

	/* - Read each line from macro file. */

	pcrrpl( nunmac, &kchar, &kchar2, &lend, &lquit );
	if( lend )
		goto L_8888;

	/* - Get CDP by scaling, rotating, and translating the
	 *   relative locations from the macro file and the new center. */

	if( kchar != 'R' ){
		cmgam.xcdp = scale*cmgam.xcdp;
		cmgam.ycdp = scale*cmgam.ycdp;
		xtemp = cmgam.xcdp*cosang + cmgam.ycdp*sinang;
		ytemp = -cmgam.xcdp*sinang + cmgam.ycdp*cosang;
		cmgam.xcdp = cmgam.xcen + xtemp;
		cmgam.ycdp = cmgam.ycen + ytemp;

		/* - Rectangle opcode is handled separately. */

		}
	else{

		/* -- Must first perform inverse transform on origin. */
		xtemp = ((cmgam.xori - cmgam.xcen)*cosang - (cmgam.yori - 
		 cmgam.ycen)*sinang)/scale;
		ytemp = ((cmgam.xori - cmgam.xcen)*sinang + (cmgam.yori - 
		 cmgam.ycen)*cosang)/scale;

		/* -- Define four corners of rectangle. */
		Xrect[1] = xtemp;
		Yrect[1] = cmgam.ycdp;
		Xrect[2] = xtemp;
		Yrect[2] = ytemp;
		Xrect[3] = cmgam.xcdp;
		Yrect[3] = ytemp;
		Xrect[4] = cmgam.xcdp;
		Yrect[4] = cmgam.ycdp;

		/* -- Scale, rotate, and translate each corner of rectangle. */
		for( j = 1; j <= 4; j++ ){
			Xrect[j] = scale*Xrect[j];
			Yrect[j] = scale*Yrect[j];
			xtemp = Xrect[j]*cosang + Yrect[j]*sinang;
			ytemp = -Xrect[j]*sinang + Yrect[j]*cosang;
			Xrect[j] = cmgam.xcen + xtemp;
			Yrect[j] = cmgam.ycen + ytemp;
			}
		cmgam.xcdp = Xrect[4];
		cmgam.ycdp = Yrect[4];
		}

	/* - See if it is a "change environment op". */

	if( kchar == kmgam.kopbe ){
		kmgam.kopetx[0] = kmgam.kopbe;
		jope = 2;
L_5500:
		if( kmgam.kopetx[jope - 1] == kmgam.kopee )
			goto L_5000;
		jope = jope + 1;

                strtemp1 = malloc(3);
                strtemp2 = malloc(3);
                strncpy(strtemp1,kmgam.kopetx+jope - 2,2);
                strncpy(strtemp2,kmgam.kopetx+jope - 2,2);
                strtemp1[2] = '\0';
                strtemp2[2] = '\0';

		upcase( strtemp1, 2, strtemp2, jope-(jope-1)+2 );
		subscpy( kmgam.kopetx, jope - 2, jope - 1, 80, strtemp2 );

                free(strtemp2);

		iope = nccomp( strtemp1, (char*)kmgam.kope,3, cmgam.nope, 2 );
                free(strtemp1);

		if( iope < cmgam.nopei ){
			pcxope( iope, 0 );
			jope = jope + 1;
			}
		else{
			iopei = 0;
			jope = jope + 1;
			cnvati( &kmgam.kopetx[jope - 1],1, &i1, 0, &nerr );
							/* add 0. maf 970129 */
			if( nerr == 0 ){
				jope = jope + 1;
				cnvati( &kmgam.kopetx[jope - 1],1, &i2, 0, &nerr );
								/* add 0. maf 970129 */
				if( nerr == 0 ){
					iopei = 10*i1 + i2;
					jope = jope + 1;
					}
				else{
					iopei = i1;
					}
				}
			pcxope( iope, iopei );
			}
		goto L_5500;
		}

	/* - See if it is a "single op". */

	iop1 = nccomp( &kchar, kmgam.kop1, 1, cmgam.nop1, 1 );
	if( iop1 > 0 ){
		pcxop1( iop1 );
		goto L_5000;
		}

	/* - See if it is a "double op". */

	iop1 = nccomp( &kchar, kmgam.kop2, 1, cmgam.nop2, 1 );
	if( iop1 > 0 ){
		iop2 = 0;
		if( iop2 > 0 ){
			pcxop2( iop1, iop2 );
			}
		else{
                        strtemp1 = malloc(nctext+1);
                        strncpy(strtemp1,ktext,nctext);
                        strtemp1[nctext] = '\0';

                        fprintf(MUNOUT," %s%s\n", kbdlin, strtemp1);

                        free(strtemp1);
			goto L_5000;
			}
		goto L_5000;
		}

	/* - See if it is an "n-point op". */

	iop1 = nccomp( &kchar, kmgam.kopn, 1, cmgam.nopn, 1 );
	if( iop1 == 1 ){
		Xopnli[1] = cmgam.xori;
		Xopnli[2] = cmgam.xcdp;
		Yopnli[1] = cmgam.yori;
		Yopnli[2] = cmgam.ycdp;
		pcrrpl( nunmac, &kchar2, (char*)kjunk, &lend, &lquit );
		if( lend )
			goto L_8888;
		cmgam.xcdp = scale*cmgam.xcdp;
		cmgam.ycdp = scale*cmgam.ycdp;
		xtemp = cmgam.xcdp*cosang + cmgam.ycdp*sinang;
		ytemp = -cmgam.xcdp*sinang + cmgam.ycdp*cosang;
		cmgam.xcdp = xtemp + cmgam.xcen;
		cmgam.ycdp = ytemp + cmgam.ycen;
		Xopnli[3] = cmgam.xcdp;
		Yopnli[3] = cmgam.ycdp;
		iop2 = 1;
		if( kchar2 == 'C' )
			iop2 = 2;
		pcxops( iop1, iop2, cmgam.xopnli, cmgam.yopnli, cmgam.pcdegi );
		goto L_5000;
		}

	/* - See if it is a "text op". */

	iop1 = nccomp( &kchar, kmgam.kopt, 1, cmgam.nopt, 1 );
	if( iop1 > 0 ){
L_7000:
                if(fgetsp(ktext,MCMSG+1,nunmac) == NULL) goto L_8888;
                if(ktext[(numchar=strlen(ktext)-1)] == '\n')ktext[numchar] = '\0';

		worldmove( cmgam.xcdp, cmgam.ycdp );
		nc = indexb( ktext,MCMSG+1 );
		if( nc > 0 ){
			text( ktext,MCMSG+1, nc );
			flushbuffer( &nerr );
			if( nerr != 0 )
				goto L_8888;
			cmgam.ycdp = cmgam.ycdp - theight;
			if( iop1 == 2 )
				goto L_7000;
			}
		goto L_5000;
		}

	/* - If none of the above, then user has typed an illegal character. */

        strtemp1 = malloc(nctext+1);
        strncpy(strtemp1,ktext,nctext);
        strtemp1[nctext] = '\0';

        fprintf(MUNOUT," %s%s\n", kbdlin, strtemp1);

        free(strtemp1);

	/* - Loop until end-of-file is reached. */

	goto L_5000;

	/* - Restore text size and return. */

L_8888:
	settextsize( width, height );
	return;

} /* end of function */

