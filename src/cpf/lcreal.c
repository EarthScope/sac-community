/** 
 * @file   lcreal.c
 * 
 * @brief  Parse a real
 * 
 */
#include <stdio.h>
#include "cpf.h"
#include "com.h"
#include "bool.h"

/** 
 * Prase a real variable command construct
 * 
 * @param realv 
 *    Real variable on output
 * 
 * @return 
 *    - TRUE if the real value was found
 *    - FALSE if the real value was not found
 *
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   810207:  Original version.
 *
 */
int 
lcreal(double *realv) {
  Token *t;

  if((t = arg()) && token_is_number(t)) {
    *realv = t->value;
    arg_next();
    return TRUE;
  }

  return FALSE;

}

