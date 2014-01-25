/** 
 * @file   xsitecom.c
 * 
 * @brief  Execute a command int he site command module
 * 
 */

#include "site.h"


#include "msg.h"

/** 
 * Execute a command in the site command module 
 * 
 * @param index 
 *    Command index
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - 901 
 *
 * @date   900804:  Changed from idm to site.
 * @date   831107:  Original version.
 *
 */
void 
xsitecom(int  index, 
         int *nerr) {

	/* - Jump to correct command based upon its index number. */
	switch( index ){
		case 1: goto L_100;
        }
        
	/* - Error return if bad index value. */
	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XSITECOM",12 );
	goto L_8888;

	/* - Command 01: TESTSITE --- Test the site command module logic. */
L_100:
	xtestsite( nerr );
	goto L_8888;

L_8888:
	return;

} 

