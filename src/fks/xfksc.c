/** 
 * @file   xfksc.c
 * 
 * @brief  Execute an FKS commans
 * 
 */

#include "fks.h"
#include "msg.h"

/** 
 * Execute a FKS command given its index number
 * 
 * @param index 
 *    Index number of the command
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   901201:  Original version.
 *
 */
void 
xfksc(int  index, 
      int *nerr) {

	*nerr = 0;

	/* - Jump to correct command based upon its index number. */
	switch( index ){
		case 1: goto L_100;
		case 2: goto L_200;
		case 3: goto L_300;
                case 4: goto L_400;
		}

	/* - Error return if bad index value. */

	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XGAMC",9 );
	goto L_8888;

	/* - Command 01: MAP */

L_100:
	xmap( nerr );
	goto L_8888;

	/* - Command 02: BBFK */

L_200:
	;
	xbbfk( nerr );
	goto L_8888;

	/* - Command 03: BEAM */

L_300:
	;
	xbeam( nerr );
	goto L_8888;

        /* - Command 04: GMAP */

L_400:
        ;
        xgmtmap( nerr );
        goto L_8888;

L_8888:
	return;

}

