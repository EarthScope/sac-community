/** 
 * @file   evallogical.c
 * 
 * @brief  Evaulate a logical expression
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "mach.h"
#include "ucf.h"
#include "co.h"
#include "bool.h"


#include "cpf.h"
#include "bot.h"
#include "token.h"
#include "debug.h"

int
is_undefined(char *s) {
  if(strcasecmp(s, "undefined") == 0) {
    return 1;
  }
  if(strcasecmp(s, "undef") == 0) {
    return 1;
  }
  return 0;
}

#define	MLOGICALS	6

/** 
 * Parse and evaluate a "logical" expression
 * 
 * @param string 
 *    Character string containing logical expression. [c]
 *    Must be of the form: "number logical number" where
 *    number is an integer or floating point number and
 *    logical is one of: 'LT', 'LE', 'GT', 'GE', 'EQ', 'NE'.
 *    Integers are converted to floating before being evaluated.
 *    Leading, trailing, and extra white space is ignored.
 * @param string_s 
 *    Length of \p string
 * @param result 
 *    - "TRUE" if expression is true
 *    - "FALSE" if expression is false
 *    - "ERROR" if and error occurred during parsing
 * @param result_s 
 *    Length of \p result
 *
 * @bug This should return an int (-1,0,1)
 *
 * @date   970129:  Add parameter (0) to cnvatf.  0 means that if a string
 *             of digits is too long, let it slide by.  maf 
 * @date   900606:  Modified tests for reals to handle case where
 *             both numbers are zero.
 * @date   871117:  Changed tests of (in)equality of reals to include
 *             the possible effects of roundoff.
 *             These are now tests for equivalence not equality.
 * @date   871103:  Added ability to check for (in)equality of strings.
 * @date   870811:  Original version.
 *
 */
void 
evallogical(char *string, 
            int   string_s, 
            char *result, 
            int   result_s) {

  UNUSED(string);
  UNUSED(string_s);

	int lresult;
	float x, y;

  Token *t1, *t2, *top;

  if(!(t1 = arg()))  { goto ERROR; }
  arg_next();
  if(!(top = arg())) { goto ERROR; }
  arg_next();
  if(!(t2 = arg()))  { goto ERROR; }
  arg_next();

	/* - If first and third tokens are not floating point numbers (i.e. strings)
	 *   then the only tests that are allowed are "EQ" and "NE". */

	if( (token_is_string(t1) || token_is_quoted_string(t1) || token_is_escape_string(t1)) &&
      (token_is_string(t2) || token_is_quoted_string(t2) || token_is_escape_string(t2))) {
    lresult = strcmp(t1->str, t2->str);
    if(is_undefined(t1->str) && strcmp(t2->str, "-12345  ") == 0) {
      lresult = 0;
    }
    if(is_undefined(t2->str) && strcmp(t1->str, "-12345  ") == 0) {
      lresult = 0;
    }
    if( token_is_eq(top) ) {
      lresult = lresult == 0;
      goto L_8000;
    }	else if( token_is_ne(top) ) {
      lresult = lresult != 0;
			goto L_8000;
    }	else {
			goto ERROR;
    }
  }
  if(!token_is_number(t1) || !token_is_number(t2)) {
    if( (token_is_string(t1) || token_is_quoted_string(t1) || token_is_escape_string(t1)) &&
        is_undefined(t1->str) &&
        token_is_number(t2) ) {
        t1->value = -12345;
    } else if( (token_is_string(t2) || token_is_quoted_string(t2) || token_is_escape_string(t2)) &&
               is_undefined(t2->str) &&
               token_is_number(t1) ) {
      t2->value = -12345;
    } else {
      goto ERROR;
    }
  }

	/* - Evaluate logical expressions involving floating point numbers. */

  if(token_is_lt(top)) {
    lresult = t1->value < t2->value;
  } else if(token_is_le(top)) {
    lresult = t1->value <= t2->value;
  } else if(token_is_gt(top)) {
    lresult = t1->value > t2->value;
  } else if(token_is_ge(top)) {
    lresult = t1->value >= t2->value;
  } else if(token_is_eq(top)) {
    /* y = t1 - t2;
       x = MAX((t1 + t2)/2, 1e-30)
       (t1-t2)/(t1+t2)/2 <= RNDOFF
    */
    y = fabs( t1->value - t2->value );
    x = fmax( 0.5*(t1->value + t2->value), VSMALL );
    lresult = (y/x) <= RNDOFF;
  } else if(token_is_ne(top)) {
    y = fabs( t1->value - t2->value );
    x = fmax( 0.5*(t1->value + t2->value), VSMALL );
    lresult = (y/x) > RNDOFF;
  } else {
    goto ERROR;
  }

L_8000:
	if( lresult ){
		fstrncpy( result, result_s - 1, "TRUE", 4 );
		}
	else{
		fstrncpy( result, result_s - 1, "FALSE", 5 );
		}
	return;

 ERROR:
  fstrncpy( result, result_s-1, "ERROR", 5 );
  return;
} /* end of function */

