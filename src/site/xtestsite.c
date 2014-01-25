/** 
 * @file   xtestsite.c
 * 
 * @brief  Testsite command
 * 
 */

#include "site.h"
#include "msg.h"

#include "errors.h"

/** 
 * Test site command
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 * 
 * @date   900804:  Original version.
 *
 */
void 
xtestsite(int *nerr) {

	*nerr = SAC_OK;

	/* - Acknowledge that this test routine has been executed. */
	setmsg( "OUTPUT", 99 );
	apcmsg( "Executing Site Command TESTSITE.",33 );
	outmsg();
	clrmsg();

	return;
}

