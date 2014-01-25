/** 
 * @file   xif.c
 * 
 * @brief  Parse "IF"
 * 
 */
#include <string.h>

#include "cnd.h"
#include "cpf.h"
#include "bool.h"
#include "msg.h"

#include "errors.h"


#include "ucf.h"

/** 
 * Parse the action command "IF"
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - ERROR_EVALUATING_LOGICAL_EXPRESSION
 *
 * @note Global Variable
 *    niflevel: Incremented by one.
 *    lifresp:  Set to value of if test.
 *
 * @date   871026:  Fixed bug encoding floating point numbers into "kcond".
 *             Added error number and message.
 * @date   870817:  Original version.
 *
 */
void
xif(int *nerr) {

	char kcond[MCMSG+1], kresult[9];

	*nerr = 0;

	/* - Get rest of command line and store in condition string. */
        memset(kcond,(int)' ',MCMSG);
        kcond[MCMSG] = '\0';

	/* - Evaluate condition. */
	evallogical( kcond,MCMSG+1, kresult,9 );
  if(arg()) {
    if(!token_is_string(arg()) || strcasecmp(arg()->str, "then") != 0) {
      arg_msg("Unexpected characters after IF: ");
    }
  }
	if( memcmp(kresult,"ERROR",5) == 0 )
		goto L_9000;
	cnd.niflevel = cnd.niflevel + 1;
	if( memcmp(kresult,"TRUE",4) == 0 )
		Lifresp[cnd.niflevel] = TRUE;
	if( memcmp(kresult,"FALSE",5) == 0 )
		Lifresp[cnd.niflevel] = FALSE;
	if( !Lifresp[cnd.niflevel] )
		skipif( nerr );

L_8888:
	return;

L_9000:
	*nerr = ERROR_EVALUATING_LOGICAL_EXPRESSION;
	setmsg( "ERROR", *nerr );
	goto L_8888;
}

