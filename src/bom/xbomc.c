/** 
 * @file   xbomc.c
 * 
 * @brief  Execute a Binary Operation Command
 * 
 */

#include "bom.h"
#include "msg.h"

#include "errors.h"

/** 
 * Execute a Binary Operations command given its index number
 * 
 * @param index 
 *    Index number of the command
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    
 * @date   820824:  Original version.
 *
 */
void 
xbomc(int  index, 
      int *nerr) {

	*nerr = 0;

	/* - Jump to correct command based upon its index number. */
	switch( index ){
		case 1: goto L_100;
		case 2: goto L_200;
		case 3: goto L_300;
		case 4: goto L_400;
		case 5: goto L_500;
		case 6: goto L_600;
		}

	/* - Error return if bad index value. */
	*nerr = ERROR_SAC_LOGIC_ERROR;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XBOMC",9 );
	goto L_8888;

	

L_100: /* - Command 01: MERGE */
	xmerge( nerr );
	goto L_8888;

L_200: /* - Command 02: ADDF */
	xaddf( nerr );
	goto L_8888;

L_300: /* - Command 03: SUBF */
	xsubf( nerr );
	goto L_8888;

L_400: /* - Command 04: MULF */
	xmulf( nerr );
	goto L_8888;

L_500: /* - Command 05: DIVF */
	xdivf( nerr );
	goto L_8888;

L_600: /* - Command 06: BINOPERR */
	xboec( nerr );
	goto L_8888;

L_8888:
	return;
}

