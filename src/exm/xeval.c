/** 
 * @file   xeval.c
 * 
 * @brief  EVALUATE command
 * 
 */

#include <string.h>
#include <math.h>
#include <ctype.h>

#include "exm.h"
#include "cpf.h"
#include "bool.h"
#include "cnv.h"


#include "msg.h"
#include "co.h"
#include "bot.h"
#include "ucf.h"
#include "bbs.h"
#include "debug.h"

/** 
 * Execute the EVALUATE command which handles simple arithmetic expressions
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   920330:  Added global icnver, set bbvar value to NaN.
 * @date   920320:  Made kevaln length mcmsg. Previous defaulted to 8.
 * @date   880404:  Added option to format results in integer format.
 * @date   880326:  Fixed bug in computing neval and increased precision
 *                  of output floating point values.
 * @date   870724:  Modified to use blackboard rather than global variable.
 * @date   840822:  Defaulted first variable to 1.0 if omitted.
 * @date   840615:  Original version (from FCALC program.)
 */
void 
xeval(int *nerr)  {
  UNUSED(nerr);
  Token *t;
  if((t = arg())) {
    if((token_is_int_precision(t, TOKEN_INT_PRECISION_NON_ARGUMENT))) {
      printf(" %d\n", token_as_int(t));
    } else if((token_is_number(t))) {
      printf(" %g\n", t->value);
    }
  }

  return;
}

