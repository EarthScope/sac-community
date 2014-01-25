/** 
 * @file   basenm.c
 * 
 * @brief  Generate a family of file names
 * 
 */

#include <string.h>

#include "mach.h"
#include "ucf.h"

#include "dff.h"
#include "msg.h"
#include "co.h"
#include "clf.h"
#include "bot.h"

#include "debug.h"

/** 
 * Generate a family of file names from a given base name.  A two digit 
 *    integer is appended to the base name.
 * 
 * @param kbase 
 *    Base name to use
 * @param kbase_s 
 *    Length of \p kbase
 * @param n1 
 *    Starting integer to append to the base name
 * @param n2 
 *    Ending integer to append to the base name
 * @param kdfcl 
 *    Array to store the output file names
 * @param kdfcl_s 
 *    Length of \p kdfcl
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   860917:  Changed output file storage to a character list.
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   800906:  Fixed bug in determining correct base name.
 * @date   800823:  Added tree name capability [Prime].
 *                  Allowed a lower as well as upper range to be specified.
 * @date   800103:  Original version.
 *
 */
void 
basenm(char        *kbase, 
       int          kbase_s, 
       int          n1, 
       int          n2, 
       string_list *list,
       int         *nerr)
{
	char kname[MCPFN+1];
	int  j;

  char *base;

	*nerr = 0;

  base = fstrdup(kbase, kbase_s);
    
	if( memcmp(base," ",1) == 0 ){
    error(914, "%s", base);
    goto L_8888;
  } else if( (n1 > n2 || n1 < 0) || n2 > 99 ) {
		*nerr = 915;
    error(915, "%d %d", n1, n2);
		goto L_8888;
  } else {
    DEBUG("%d => %d\n", n1, n2);
    DEBUG("base: '%s'\n", base);
    for( j = n1; j <= n2; j++) {
      sprintf(kname, "%s%02d", base, j);
      string_list_put(list, kname, strlen(kname));
    }
  }    
 L_8888:
  free(base);
  base = NULL;
	return;
}
