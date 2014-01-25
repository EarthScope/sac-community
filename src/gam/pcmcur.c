/** 
 * @file   pcmcur.c
 * 
 * @brief  Cursor Command
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

/** 
 * Cursor Command
 * 
 * @param nunmac 
 *     ???
 *
 * @note Assumptions
 *         - XCEN and YCEN are set correctly prior to this call.
 
 * @date   970129:  Add parameter (0) to cnvati.  0 means that if a string
 *                  of digits is too long, let it slide by.  maf 
 * @date   821013:  Added ability to delete previous command from replay file.
 * @date   821008:  Original version.
 *
 */
void 
pcmcur(FILE *nunmac) {

	char kmsg[MCMSG+1], ktext[MCMSG+1];
	int lend, lquit;
	static int lopt;
	char kchar, kchar2;
	int i1, i2, iop1, iop2, iope, iopei, jope, nc, nctext, nerr, numchar;
	float height, width, xloc, xtemp, xtloc, yloc, ytemp, ytloc;
        char *strtemp1, *strtemp2;

L_5000:

	/* - Set origin (ORI) to current data point (CDP) if requested. */

	if( !cmgam.lglori ){
		cmgam.xori = cmgam.xcdp;
		cmgam.yori = cmgam.ycdp;
		}

	/* - Get cursor location and character.  Convert character to upper case. */

	worldcursor( &cmgam.xcdp, &cmgam.ycdp, &kchar );
	upcase( &kchar, 1, &kchar,1 );

	/* - Check for "termination op" command. */

	if( kchar == kmgam.kopqui )
		goto L_8888;

	/* - Check for "delete last op" command. */

	if( kchar == kmgam.kopdel ){
		if( lopt ){
			backspace( nunmac,1L );

                        if(fgetsp(ktext,MCMSG+1,nunmac) == NULL) goto L_8888;
                        if(ktext[(numchar=strlen(ktext)-1)] == '\n')ktext[numchar] = '\0';

			backspace( nunmac,1L );
			if( memcmp(ktext,"        ",8) == 0 ){
L_3000:
				backspace( nunmac,1L );

                                if(fgetsp(ktext,MCMSG+1,nunmac) == NULL) goto L_8888;
                                if(ktext[(numchar=strlen(ktext)-1)] == '\n')ktext[numchar] = '\0';

				backspace( nunmac,1L );
				if( ktext[0] != Kopt[2] )
					goto L_3000;
				}
			else{
				backspace( nunmac,1L );
				pcrrpl( nunmac, &kchar, &kchar2, &lend, &lquit );
				if( lend )
					goto L_5000;
				backspace( nunmac,1L );
				}
			}
		else{
			backspace( nunmac,1L );
			pcrrpl( nunmac, &kchar, &kchar2, &lend, &lquit );
			if( lend )
				goto L_5000;
			backspace( nunmac,1L );
			}
		goto L_5000;
		}

	/* - Check for "comment line op". */

	if( kchar == kmgam.kopcmt ){
		worldtovport( xtloc, ytloc, &xloc, &yloc );
		cursortext( &xloc, &yloc, ktext,MCMSG+1 );
		vporttoworld( xloc, yloc, &xtloc, &ytloc );
		nctext = indexb( ktext,MCMSG+1 );
                strtemp1 = malloc(nctext+1);
                strncpy(strtemp1, ktext, nctext);
                strtemp1[nctext] = '\0';

                fprintf(nunmac,"%c%s\n",kchar,strtemp1);

                free(strtemp1);
		lopt = TRUE;
		goto L_5000;
		}

	/* - Compute relative location by subtracting center position from CDP. */

	xtemp = cmgam.xcdp - cmgam.xcen;
	ytemp = cmgam.ycdp - cmgam.ycen;

	/* - See if it is a "change environment op". */

	if( kchar == kmgam.kopbe ){
		kmgam.kopetx[0] = kmgam.kopbe;
		jope = 2;
		worldcursor( &cmgam.xcdpe, &cmgam.ycdpe, &kmgam.kopetx[jope - 
		 1] );
L_5500:
		if( kmgam.kopetx[jope - 1] == kmgam.kopee ){
                        strtemp1 = malloc(jope+1);
                        strncpy(strtemp1,kmgam.kopetx,jope);
                        strtemp1[jope] = '\0';

                        fprintf(nunmac,"%s\n",strtemp1);

                        free(strtemp1);
			goto L_5000;
			}
		jope = jope + 1;
		worldcursor( &cmgam.xcdpe, &cmgam.ycdpe, &kmgam.kopetx[jope - 
		 1] );

                strtemp1 = malloc(3);
                strncpy(strtemp1,kmgam.kopetx+jope - 2, 2);
                strtemp1[2] = '\0';
                strtemp2 = malloc(3);
                strncpy(strtemp2,kmgam.kopetx+jope - 2,2);
                strtemp2[2] = '\0';

		upcase( strtemp1, 2, strtemp2,jope-(jope-1)+2 );

         	subscpy( kmgam.kopetx, jope - 2, jope - 1, 80, strtemp2 );

                free(strtemp1);

		iope = nccomp( strtemp2, (char*)kmgam.kope,3, cmgam.nope, 2 );

                free(strtemp2);

		if( iope < cmgam.nopei ){
			pcxope( iope, 0 );
			jope = jope + 1;
			worldcursor( &cmgam.xcdpe, &cmgam.ycdpe, &kmgam.kopetx[jope - 
			 1] );
			}
		else{
			iopei = 0;
			jope = jope + 1;
			worldcursor( &cmgam.xcdpe, &cmgam.ycdpe, &kmgam.kopetx[jope - 
			 1] );
			cnvati( &kmgam.kopetx[jope - 1],1, &i1, 0, &nerr );
							/* add 0, maf 970129 */
			if( nerr == 0 ){
				jope = jope + 1;
				worldcursor( &cmgam.xcdpe, &cmgam.ycdpe, &kmgam.kopetx[jope - 
				 1] );
				cnvati( &kmgam.kopetx[jope - 1],1, &i2, 0, &nerr );
								/* add 0, maf 970129 */
				if( nerr == 0 ){
					iopei = 10*i1 + i2;
					jope = jope + 1;
					worldcursor( &cmgam.xcdpe, &cmgam.ycdpe, &kmgam.kopetx[jope - 
					 1] );
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

                sprintf(kmsg,"%c%10.5f%10.5f", kchar, xtemp, ytemp );
                fprintf(nunmac,"%s\n",kmsg);

		/* -- Set up data points if this is a "rectangle" opcode. */
		if( kchar == 'R' ){
			Xrect[1] = cmgam.xori;
			Yrect[1] = cmgam.ycdp;
			Xrect[2] = cmgam.xori;
			Yrect[2] = cmgam.yori;
			Xrect[3] = cmgam.xcdp;
			Yrect[3] = cmgam.yori;
			Xrect[4] = cmgam.xcdp;
			Yrect[4] = cmgam.ycdp;
			}
		pcxop1( iop1 );
		lopt = FALSE;
		goto L_5000;
		}

	/* - See if it is a "double op". */

	iop1 = nccomp( &kchar, kmgam.kop2, 1, cmgam.nop2, 1 );
	if( iop1 > 0 ){
L_6000:
		worldcursor( &cmgam.xcdp, &cmgam.ycdp, &kchar2 );
		upcase( &kchar2, 1, &kchar2, 1 );
		iop2 = 0;
		if( iop2 > 0 ){
                        sprintf(kmsg,"%c%c%10.5f%10.5f", kchar, kchar2, 
			 xtemp, ytemp );

                        fprintf(nunmac,"%s\n",kmsg);

			pcxop2( iop1, iop2 );
			}
		else{
                        fprintf(MUNOUT," %s%c\n", "Illegal character: ",kchar2);
			goto L_6000;
			}
		lopt = FALSE;
		goto L_5000;
		}

	/* - See if it is an "n-point op". */

	iop1 = nccomp( &kchar, kmgam.kopn, 1, cmgam.nopn, 1 );
	if( iop1 == 1 ){
		Xopnli[1] = cmgam.xori;
		Xopnli[2] = cmgam.xcdp;
		Yopnli[1] = cmgam.yori;
		Yopnli[2] = cmgam.ycdp;

                sprintf(kmsg,"%c%10.5f%10.5f", kchar, xtemp, ytemp );

                fprintf(nunmac,"%s\n",kmsg);

		worldcursor( &cmgam.xcdp, &cmgam.ycdp, &kchar2 );
		upcase( &kchar2, 1, &kchar2, 1 );

		xtemp = cmgam.xcdp - cmgam.xcen;
		ytemp = cmgam.ycdp - cmgam.ycen;

                sprintf(kmsg,"%c%10.5f%10.5f", kchar2, xtemp, ytemp );

                fprintf(nunmac,"%s\n",kmsg);

		Xopnli[3] = cmgam.xcdp;
		Yopnli[3] = cmgam.ycdp;
		iop2 = 1;
		if( kchar2 == 'C' )
			iop2 = 2;
		pcxops( iop1, iop2, cmgam.xopnli, cmgam.yopnli, cmgam.pcdegi );
		lopt = FALSE;
		goto L_5000;
		}

	/* - See if it is a "text op". */

	iop1 = nccomp( &kchar, kmgam.kopt, 1, cmgam.nopt, 1 );
	if( iop1 > 0 ){
		lopt = TRUE;

                sprintf(kmsg,"%c%10.5f%10.5f", kchar, xtemp, ytemp );
                fprintf(nunmac,"%s\n",kmsg);

		/* -- Get text size and convert to world coordinates. */
		gettextsize( &width, &height );
		vporttoworld( width, height, &width, &height );
L_7000:
		fstrncpy( ktext, MCMSG, " ", 1 );
		worldtovport( xtloc, ytloc, &xloc, &yloc );
		cursortext( &xloc, &yloc, ktext,MCMSG+1 );
		vporttoworld( xloc, yloc, &xtloc, &ytloc );
		nc = indexb( ktext,MCMSG+1 );

                strtemp1 = malloc(nc+1);
                strncpy(strtemp1,ktext,nc);
                strtemp1[nc] = '\0';

                fprintf(nunmac,"%s\n",strtemp1);

                free(strtemp1);

		worldmove( cmgam.xcdp, cmgam.ycdp );
		if( nc > 0 ){
			text( ktext,MCMSG+1, nc );
			flushbuffer( &nerr );
			if( nerr != 0 )
				goto L_8888;
			cmgam.ycdp = cmgam.ycdp - height;
			if( iop1 == 2 )
				goto L_7000;
			}
		goto L_5000;
		}

	/* - If none of the above, then user has typed an illegal character. */

        fprintf(MUNOUT," %s%c\n", "Illegal character: ", kchar );
	goto L_5000;

L_8888:
	return;

} /* end of function */

