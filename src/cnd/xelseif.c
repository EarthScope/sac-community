/** 
 * @file   xelseif.c
 * 
 * @brief  Parse "ELSEIF"
 * 
 */

#include <string.h>

#include "cnd.h"
#include "bool.h"


#include "ucf.h"
#include "cpf.h"
#include "msg.h"
#include "errors.h"

/** 
 * Parse the action command "ELSEIF"
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *
 * @note Global Coupling
 *   - niflevel: Incremented by one.
 *   - lifresp:  Set to value of if test.
 *
 * @date   870817:  Original version.
 *
 */
void 
xelseif(int *nerr) {

	char kcond[MCMSG+1], kresult[9];

	*nerr = 0;

	/* - Copy rest of command into the condition string and evaluate it. */
	if( cnd.niflevel > 0 ){
		if( Lifresp[cnd.niflevel] ){
			skipif( nerr );
		}
		else{
                        memset(kcond,(int)' ',MCMSG);
                        kcond[MCMSG] = '\0';
			evallogical( kcond,MCMSG+1, kresult,9 );
      if(arg()) {
        if(!token_is_string(arg()) || strcasecmp(arg()->str, "then") != 0) {
          arg_msg("Unexpected characters after ELSEIF: ");
        }
      }
			if( memcmp(kresult,"ERROR",5) == 0 )
				goto L_9000;
			if( memcmp(kresult,"TRUE",4) == 0 )
				Lifresp[cnd.niflevel] = TRUE;
			if( memcmp(kresult,"FALSE",5) == 0 )
				Lifresp[cnd.niflevel] = FALSE;
			if( !Lifresp[cnd.niflevel] ){
				skipif( nerr );
			}
		}
	}
	else{
		*nerr = 1;
	}

L_8888:
	return;

L_9000:
	*nerr = ERROR_EVALUATING_LOGICAL_EXPRESSION;
	setmsg( "ERROR", *nerr );
	goto L_8888;
}

