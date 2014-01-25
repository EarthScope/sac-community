/** 
 * @file   getvFILEptr.c
 * 
 * @brief  Get a vars FILE pointer
 * 
 */

#include <stdio.h>
#include <string.h>

#include "vars.h"
#include "debug.h"

/** 
 * Get a vars FILE pointer
 * 
 * @param vars 
 *    Name of vars list
 * @param vars_s 
 *    Length of \p vars
 * @param name 
 *    Name of vars entry
 * @param name_s 
 *    Length of \p name
 * @param value 
 *    Output value of vars FILE pointer
 * @param nerr 
 *    Error Return Pointer
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @bug This routine can return an undefined error of 1
 *
 * @date   121493:  Original version.
 *
 */
void 
getvFILEptr(char  *vars, 
	    int    vars_s, 
	    char  *name, 
	    int    name_s, 
	    FILE **value, 
	    int   *nerr)
{
	int i;
  struct varsfile *flist;

  UNUSED(vars_s);
  UNUSED(name_s);

	*nerr = 0;
  flist = vfilelist.filelist;

	/* search for entry */
        for (i=0; i<vfilelist.nentries; i++, flist++){
          if(strcmp(flist->varsname,vars) == 0) {
            if(strcmp(flist->variable,name) == 0) {
              *value = flist->value;
              goto L_8888;
            }
          }
        }

	/* if we got here we didnt find it */
        *nerr = 1;
        
L_8888:
	return;
}

