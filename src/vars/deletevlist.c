/** 
 * @file   deletevlist.c
 * 
 * @brief  Delete a vars list
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amf.h"
#include "mach.h"
#include "vars.h"
#include "msg.h"
#include "co.h"
#include "debug.h"

/** 
 * Delete a vars list
 * 
 * @param vars 
 *    Name of vars list to delete
 * @param vars_s 
 *    Length of \p vars
 * @param mode 
 *    Delete Mode
 *    - 'MEMORY' to only delete list in memory.
 *    - 'BOTH' to delete list in memory and on disk.
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @bug Does not delete the entry in the parent list.
 * @bug Deletion of disk images is done using UNIX system command "rm".
 *      This is NOT portable and should be replaced with calls to "zfiles"
 *      to get all files in directory and "wild" to perform pattern
 *      matching, and "zdest" to actually destroy the matched files.  
 *      This would make the delete machine independent.
 *
 * @date   890727:  Was not deleting sublists or disk images.
 * @date   870407:  Original version.
 * @date   890727:  Documented/Reviewed
 *
 */
void 
deletevlist(char  *vars, 
	    int    vars_s, 
	    char  *mode, 
	    int   *nerr)
{
	*nerr = 0;
  UNUSED(vars_s);
  UNUSED(mode);
  if(!sac_vars_delete(vars)) {
    error(*nerr = 1205, "%s", vars);
  }
	return;
}

