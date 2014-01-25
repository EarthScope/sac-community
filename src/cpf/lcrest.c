/** 
 * @file   lcrest.c
 *
 * @brief  Parse the rest of the command
 * 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"
#include "bot.h"
#include "co.h"

#include "string_utils.h"
#include "ucf.h"
#include "debug.h"

/** 
 * Parse the rest of the command
 * 
 * @param mchar 
 *    Maximum length of \p mchar
 * @param kchar 
 *    Output character string
 * @param kchar_s 
 *    Length of \p kchar on input
 * @param nchar 
 *    Length of \p kchar on output
 * 
 * @return 
 *    - TRUE if the rest was found
 *    - FALSE if the rest was not found
 *
 * @note  Logic in this function should be similiar to that in "ucf/wrcom.f"
 *        If either is changed, then the other should be updated.
 *
 * @date   871022:  Gutted routine and replaced with logic from WRCOM.
 * @date   870211:  Major change in logic.
 * @date   820825:  Original version.
 *
 */
int
lcrest(int   mchar, 
       char *kchar, 
       int   kchar_s, 
       int  *nchar) {

  string *s;
  Token *t;

  s = NULL;

  UNUSED(kchar_s);

  if(!arg()) {
    return FALSE;
  }

  while((t = arg())) {
    char *p = token_as_string(t);
    s = string_append(s, p);
    s = string_append(s, " ");
    arg_next();
    free(p);
  }
  *nchar = min(string_length(s), mchar);
  strncpy(kchar, string_string(s), *nchar);
  kchar[*nchar] = 0;
  return TRUE;

}

