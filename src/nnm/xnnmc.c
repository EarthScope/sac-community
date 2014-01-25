/** 
 * @file   xnnmc.c
 * 
 * @brief  Execute the neural net module
 * 
 */

#include "nnm.h"
#include "msg.h"

/** 
 * Execute the neural net module command given its index number
 * 
 * @param index 
 *    Index number of the command
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   890306:  Original version.
 *
 */
void 
xnnmc(int  index, 
      int *nerr) {


	*nerr = 0;

	/* - Jump to correct command based upon its index number. */
	switch( index ){
		case 1: goto L_100;
        }
        
	/* - Error return if bad index value. */
	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XNNMC",9 );
	goto L_8888;

	/* - Command 01: WRITENN --- write a neural net file to disk. */
L_100:
	xwritenn( nerr );
	goto L_8888;

L_8888:
	return;

}

