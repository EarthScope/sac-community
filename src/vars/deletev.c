/** 
 * @file   deletev.c
 * 
 * @brief  Delete a vars entry
 * 
 */
#include <stdlib.h>
#include <stdio.h>

#include "amf.h"
#include "bool.h"
#include "vars.h"
#include "bot.h"
#include "msg.h"
#include "co.h"
#include "ucf.h"
#include "debug.h"

/** 
 * Delete a vars entry
 * 
 * @param vars 
 *    Name of vars list
 * @param vars_s 
 *    Length of \p vars
 * @param name 
 *    Name of entry to delete
 * @param name_s 
 *    Length of \p name
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   900207:  Was not setting modified flag when deleting variable.
 * @date   890329:  Fixed bug in deleting a variable with an indirect block.
 * @date   870107:  Added logic to remove indirect data block if necessary.
 * @date   861229:  Original version.
 * @date   861229:  Documented/Reviewed
 *
 */
void 
deletev(char   *vars, 
	int     vars_s, 
	char   *name, 
	int     name_s, 
	int    *nerr)
{
  char *s1; 
  *nerr = 0;
  UNUSED(vars_s);
  UNUSED(name_s);
  s1 = upcase_dup(name);
  if(!sac_vars_delete_var(vars, s1)) {
    error(*nerr = 1205, "%s", s1);
  }
  free(s1);
}

