/** 
 * @file   createvlist.c
 * 
 * @brief  Create a new vars list
 * 
 */

#include <stdio.h>
#include <string.h>

#include "amf.h"
#include "bool.h"
#include "vars.h"
#include "bot.h"
#include "co.h"
#include "debug.h"

/** 
 * Create a new vars list in memory
 * 
 * @param fullvars 
 *    Name of new vars list to create.  This is the full vars list name
 *    including the directory pathname.  It is not expanded using the 
 *    "current vars list" like most of the vars subroutines.
 * @param fullvars_s 
 *    Length of \p fullvars
 * @param length 
 *    Length of vars list
 * @param node 
 *    Output vars list node number
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @note Special Notes 
 *   - You should use this subroutine to create new TOP LEVEL vars list.
 *   - You should use "putvlist" to create new vars sublists within an
 *     existing vars list.
 *   - The directory pathname is necessary if you plan to save the list on
 *     disk.  It is not needed for temporary vars lists used for scratch space.
 *   - This subroutine is also called by other vars subroutines when
 *     reading existing vars lists from disk to memory.
 *
 * @date   890227:  Added output of vars node number.
 * @date   881205:  Moved encoding of nil descriptor to new subroutine.
 * @date   881114:  Changed name from createvsect to createvlist.
 * @date   870915:  First entry in vars list changed to name of vars
 *                  list.  Before all of them were called "vars".
 * @date   861229:  Original version.
 * @date   861229:  Documented/Reviewed
 *
 */
void 
createvlist(char      *fullvars, 
	    int        fullvars_s, 
	    int        length, 
	    int       *node, 
	    int       *nerr)
{
	*nerr = 0;
  UNUSED(fullvars_s);
  UNUSED(length);
  UNUSED(node);
  sac_vars_create(fullvars);

}

