/** 
 * @file   lklog.c
 * 
 * @brief  Parse a keyed logical value
 * 
 */

#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"


#include "bot.h"


/** 
 * Parse a keyed logical variable command construct
 * 
 * @param kkey 
 *    Keyword to search for
 * @param kkey_s 
 *    Length of \p kkey
 * @param logv 
 *    Logical value on output
 * 
 * @return 
 *    - TRUE if logical value was found
 *    - FALSE if logical value was not found
 *
 * @date   870730:  Added logic to convert current token to uppercase.
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   820312:  Factored test for key to LCKEY function.
 * @date   810207:  Original version.
 *
 */
int
lklog(char *kkey, 
      int   kkey_s, 
      int  *logv) {

	/* - Check for key. */
	if(!lckey( kkey,kkey_s )) {
    return FALSE;
  }

  if(!lclog(logv)) {
    *logv = TRUE;
  }

  return TRUE;
}

