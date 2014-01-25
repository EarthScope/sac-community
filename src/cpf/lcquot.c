/** 
 * @file   lcquot.c
 * 
 * @brief  Parse a quoted character string
 * 
 */
#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"


#include "co.h"
#include "ucf.h"
#include "debug.h"

/** 
 * Parse a quoted character string command construct
 * 
 * @param mquot 
 *    Maximum Length of \p kquot
 * @param kquot 
 *    Output character string
 * @param kquot_s 
 *    Length of \p kquot on input
 * @param nquot 
 *    Length of \p kquot on output
 * 
 * @return 
 *    - TRUE if the string was found
 *    - FALSE if the string was not found
 *
 * @date   890828:  Minor modified to make it palatable to DEC ULTRIX f77.
 * @date   820505:  Now allowing both single and double quotes.
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   810928:  Modified due to changes in TOKENS.
 * @date   810416:  Removed option to use underline as substitute for blank.
 * @date   810208:  Original version.
 *
 */
int 
lcquot(int   mquot, 
       char *kquot, 
       int   kquot_s, 
       int  *nquot) {

  Token *t;

  UNUSED(kquot_s);

  if((t = arg()) && token_is_quoted_string(t)) {
    *nquot = min(strlen(t->str), mquot);
    strncpy(kquot, t->str, *nquot);
    kquot[*nquot] = 0;
    arg_next();
    return TRUE;
  }
  return FALSE;
}

