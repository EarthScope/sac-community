/** 
 * @file   pcxrpl.c
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
pcxrpl(FILE *nunrpl, 
       int  *lquit) {

	char kjunk[9], ktext[MCMSG+1];
	int lend;
	char kchar, kchar2;
	int i1, i2, icloc1, icloc2, icpntr, iop1, iop2, iope, iopei, 
	 itype, jope, nc, nctext, nerr, numchar;
        FILE *nunmac;
	float height, width, xcdpsv, ycdpsv;
	static char kbdlin[26] = "Bad line in replay file: ";
        char *s1, *s2;


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
	 * ASSUMPTIONS:
	 *=====================================================================
	 * LIMITATIONS:
	 *=====================================================================
	 * KNOWN ERRORS:
	 *=====================================================================
	 * EXTERNAL DEPENDENCIES:
	 *===================================================================== */
	/* PROCEDURE: */
L_5000:
	;

	/* - Set origin (ORI) to current point (CP) if requested. */

	if( !cmgam.lglori ){
		cmgam.xori = cmgam.xcdp;
		cmgam.yori = cmgam.ycdp;
		}

	/* - Read each line from replay file. */

	pcrrpl( nunrpl, &kchar, &kchar2, &lend, lquit );
	if( lend )
		goto L_8888;

	/* - See if it is a "macro invocation request." */

	if( kchar == kmgam.kopmac ){
		xcdpsv = cmgam.xcdp;
		ycdpsv = cmgam.ycdp;

                if(fgetsp(ktext,MCMSG+1,nunrpl) == NULL) goto L_8888;
                if(ktext[(numchar=strlen(ktext)-1)] == '\n')ktext[numchar] = '\0';

		nctext = indexb( ktext,MCMSG+1 );
		if( nctext > 0 ){
			icpntr = 0;
			poptok( ktext, nctext, &icpntr, &icloc1, &icloc2, &itype );

                        fstrncpy( kmgam.kpcmac, MCPFN, ktext+icloc1 - 
			 1,min(icloc2,MCMSG) - icloc1 + 1);
                        fstrncpy( kmgam.kpcmac+(min(icloc2,MCMSG) - icloc1 + 1),MCPFN -
                                  (min(icloc2,MCMSG) - icloc1 + 1),kmgam.kpcmsu,
                                  strlen(kmgam.kpcmsu));

			poptok( ktext, nctext, &icpntr, &icloc1, &icloc2, &itype );
			if( itype > 0 ){
                s1 = strcut(ktext, icloc1, icloc2);
				cnvatf( s1, icloc2 - icloc1 + 2, &cmgam.scamac, 0, &nerr );
				free(s1);
				if( nerr != 0 )
					cmgam.scamac = 1.;
				}
			poptok( ktext, nctext, &icpntr, &icloc1, &icloc2, &itype );
			if( itype > 0 ){
                s1 = strcut(ktext, icloc1, icloc2);
				cnvatf( s1, icloc2 - icloc1 + 2, &cmgam.rotmac, 0, &nerr );
				free(s1);
				if( nerr != 0 )
					cmgam.rotmac = 0.;
				}
			}

		zopens( &nunmac, kmgam.kpcmac,MCPFN+1, "TEXT",5, &nerr );
		if( nerr != 0 )
			goto L_8888;
		pcmrpl( nunmac, cmgam.scamac, cmgam.rotmac );

                fclose(nunmac);

		cmgam.xcdp = xcdpsv;
		cmgam.ycdp = ycdpsv;
		goto L_5000;
		}

	/* - See if it is a "change environment op". */

	if( kchar == kmgam.kopbe ){
		kmgam.kopetx[0] = kmgam.kopbe;
		jope = 2;
L_5500:
		if( kmgam.kopetx[jope - 1] == kmgam.kopee )
			goto L_5000;
		jope = jope + 1;
        s1=malloc(3);
                strncpy(s1,kmgam.kopetx+jope - 2,2);
                s1[2] = '\0';
                s2=malloc(3);
                strncpy(s2,kmgam.kopetx+jope - 2,2);
                s2[2] = '\0';

		upcase( s1, 2, s2, jope - (jope - 1) + 2 );
		subscpy( kmgam.kopetx, jope - 2, jope - 1, 80, s2 );

                free(s1);

		iope = nccomp( s2, (char*)kmgam.kope,3, cmgam.nope, 2 );
                free(s2);
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
            s1 = strcut(ktext, 1, nctext);
            fprintf(MUNOUT," %s%s\n", kbdlin, s1 );
            free(s1);
			goto L_5000;
			}
		goto L_5000;
		}

	/* - See if it is a "n-point op". */

	iop1 = nccomp( &kchar, kmgam.kopn, 1, cmgam.nopn, 1 );
	if( iop1 == 1 ){
		Xopnli[1] = cmgam.xori;
		Xopnli[2] = cmgam.xcdp;
		Yopnli[1] = cmgam.yori;
		Yopnli[2] = cmgam.ycdp;
		pcrrpl( nunrpl, &kchar2, (char*)kjunk, &lend, lquit );
		if( lend )
			goto L_8888;
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
		/* -- Get text size and convert to world coordinates. */
		gettextsize( &width, &height );
		vporttoworld( width, height, &width, &height );
L_7000:
                if(fgetsp(ktext,MCMSG+1,nunrpl) == NULL) goto L_8888;
                if(ktext[(numchar=strlen(ktext)-1)] == '\n')ktext[numchar] = '\0';

		worldmove( cmgam.xcdp, cmgam.ycdp );
		nc = indexb( ktext,MCMSG+1 );
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
    s1 = strcut(ktext, 1, nctext);
    fprintf(MUNOUT," %s%s\n", kbdlin, s1 );
    free(s1);

	/* - Loop until end-of-file is reached. */

	goto L_5000;

L_8888:

	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
         *    970129:  Add parameter (0) to cnvatf and cnvati.  0 means that if
         *             a string of digits is too long, let it slide by.  maf 
	 *    860106:  Modified rectangle drawing logic.
	 *    830623:  Added enviroment options with arbitrary integer argument.
	 *    810000:  Original version.
	 *===================================================================== */

} /* end of function */

