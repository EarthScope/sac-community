/** 
 * @file   xsetbb.c
 * 
 * @brief  SETBB command
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exm.h"
#include "cpf.h"
#include "msg.h"
#include "bbs.h"
#include "bool.h"
#include "errors.h"
#include "vars.h"

#include "co.h"
#include "bot.h"
#include "bool.h"

#define	MTEMP	1000

#define ERROR(x) { *nerr = x; goto L_8888; }

/** 
 * Execute the SETBB comman which defines a blackboard variable
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   871109:  Added option to append text to a blackboard variable.
 * @date   870904:  Fixed bug with numbers greater than a token in width.
 * @date   870301:  Original version.
 *
 */
void 
xsetbb(int *nerr) {
	char kname[MCMSG+1], ktemp[1001], kvalue[MCMSG+1];
	int  lappend;
	int nchar;
  Token *t;

	*nerr = 0;
    memset(kvalue, 0, sizeof(kvalue));
    memset(ktemp, 0, sizeof(ktemp));
    memset(kname, 0, sizeof(kname));
	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){
    /* -- "name [APPEND] value":  the name and value of the blackboard variable.
     *    Store the name and value in the blackboard store. */
    if( lcchar( MCMSG, kname,MCMSG+1, &nchar ) ){
      lappend = lckey( "APPEND#$",9);

L_1200:
    if((t = arg())) {
      if(!lappend) {
        if(!token_to_bb(t, kname)) {
          ERROR( ERROR_VARIABLE_TYPE_INCORRECT );
        }
      } else {
        char *new;
        var *v;
        if(!(v = getbb(kname))) {
          ERROR( ERROR_FINDING_VARIABLE );
        }
        if((!token_is_string(t) && !token_is_quoted_string(t) && !token_is_escape_string(t)) || v->type != VAR_STRING) {
          ERROR( ERROR_VARIABLE_TYPE_INCORRECT );
        }
        asprintf(&new, "%s%s", v->str, t->str);
        setbb(kname, VAR_STRING, new);
        free(new);
      }
      arg_next();
    }
		else {
      arg_end();
      cfmt( "NEED A BLACKBOARD VALUE",24 );
      cresp();
      if( lcmore( nerr ) )
        goto L_1200;
		}

    }
    else{
      /* -- Bad syntax. */
      cfmt( "ILLEGAL OPTION:",17 );
      cresp();
    }
	}

L_8888:
	return;
}

