/** 
 * @file   lclog.c 
 * 
 * @brief  Parse a logical
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "cpf.h"
#include "bool.h"
#include "com.h"


#include "bot.h"

/** 
 * Parse a logical variable command construct
 * 
 * @param logv 
 *    Logical variable on output
 * 
 * @return 
 *    - TRUE if the logical variable was found
 *    - FALSE if the logical variable was not found
 *
 * @date   870730:  Added logic to convert current token to uppercase.
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   820312:  Factored test for key to LCCKEY function.
 * @date   810207:  Original version.
 *
 */
int
lclog(int *logv) {

	int lclog_v;
  char *key, *c;
  Token *t;

  if(!(t = arg()) || !(c = token_as_string(t))) {
    return FALSE; 
  }
  key = upcase_dup(c);
  free(c);
  c = NULL;
  
  lclog_v = FALSE;
	/* - Check for "ON" or "OFF" at next token.
	 * - Do not change value of logical variable if not found. */
	if( memcmp(key,"ON",2) == 0 ){
		*logv = TRUE;
		lclog_v = TRUE;
	}
	else if( memcmp(key,"OF",2) == 0 ){
		*logv = FALSE;
		lclog_v = TRUE;
	}

  if(lclog_v) {
    arg_next();
  }
  free(key);

	return( lclog_v );
}

