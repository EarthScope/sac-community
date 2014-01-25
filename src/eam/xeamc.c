/** 
 * @file   xeamc.c
 * 
 * @brief  Execute EAM command
 * 
 */

#include "eam.h"


#include "msg.h"
#include "bot.h"

/** 
 * Execute a EAM command given its index number \p index
 * 
 * @param index 
 *    The index number of the command to execute
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *    -  901 Bad index command \p index
 *
 * @date   820000:  Original version.
 *
 */
void 
xeamc(int  index, 
      int *nerr)
{

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
	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XEAMC",9 );
	goto L_8888;

	/* - Command 01: OHPF */
L_100:
	xohpf( nerr );
	goto L_8888;

	/* - Command 02: CHPF */
L_200:
	chpf( nerr );
	goto L_8888;

	/* - Command 03: WHPF */
L_300:
	xwhpf( nerr );
	goto L_8888;

	/* - Command 04: OAPF */
L_400:
	xoapf( nerr );
	goto L_8888;

	/* - Command 05: CAPF */
L_500:
	capf( nerr );
	goto L_8888;

	/* - Command 06: APK */
L_600:
	xapk( nerr );
	goto L_8888;

L_8888:
	return;
}
