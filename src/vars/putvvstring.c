/** 
 * @file   putvvstring.c
 * 
 * @brief  Put a string vars value 
 * 
 */
#include <stdlib.h>
#include <stdio.h>

#include "amf.h"
#include "vars.h"
#include "bot.h"
#include "co.h"
#include "debug.h"

/** 
 * Put a vars value of type string
 * 
 * @param vars 
 *    Name of vars list
 * @param vars_s 
 *    Length of \p vars
 * @param name 
 *    Name of vars entry
 * @param name_s 
 *    Length of \p name
 * @param numchars 
 *    Number of characters in value.  If less than zero this value 
 *    is computed to be the length of the string without trailing 
 *    blanks.
 * @param value 
 *    Entry value to set
 * @param value_s 
 *    Length of \p value
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   121221:  Changed vars system to use a hash table / dictionary
 * @date   881107:  Changed name of subroutine.
 * @date   861229:  Original version.
 * @date   881107:  Doumented/Reviewed
 *
 */
void 
putvvstring(char *vars, 
	    int   vars_s, 
	    char *name, 
	    int   name_s, 
	    int   numchars, 
	    char *value, 
	    int   value_s, 
	    int  *nerr)
{
  char *n;
  *nerr = 0;
  UNUSED(vars_s);
  UNUSED(name_s);
  UNUSED(numchars);
  UNUSED(value_s);
  n = upcase_dup(name);
  if(!sac_vars_put_var(vars, n, VAR_STRING, value)) {
    if(!sac_vars_exists(vars)) {
      *nerr = 1203; /* variable list not found */
    } else {
      *nerr = 1205; /* Error deleting variable */
    }
  }
  free(n);
	return;
}

