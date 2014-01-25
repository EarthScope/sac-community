/** 
 * @file   lcidi.c
 * 
 * @brief  Parse an integer - integer
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"


#include "bot.h"

/** 
 * Parse an integer "-" integer command construct
 * 
 * @param int1 
 *    First integer on output
 * @param int2 
 *    Second integer on output
 * 
 * @return 
 *   - TRUE if the int - int was found
 *   - FALSE if the int - int was not found
 *
 * @date   990521:  Original version.  Plagerized from lcint.  (maf)
 *
 */
int 
lcidi(int *int1, 
      int *int2) {

	int temp ;
  Token *t;


	/* String should conform to a stict format, wherein there may be one
	   dash (-) and the rest of the characters are digits (the only
	   permited whitespace is end padding).  The dash may not be the 
	   first character, nor the last character before the padding. */
  if(!(t = arg()) || !token_is_string(t) || !strchr(t->str, '-')) {
    return FALSE;
  }

  if(sscanf(t->str, "%d-%d%n", int1, int2, &temp) != 2) {
    return FALSE;
  }
  if(temp != (int)strlen(t->str)) {
    return FALSE;
  }
  arg_next();
  return TRUE;


}

