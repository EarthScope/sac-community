/** 
 * @file   readvfile.c
 * 
 * @brief  Read a vars list from disk
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amf.h"
#include "bot.h"
#include "bool.h"
#include "vars.h"
#include "msg.h"
#include "co.h"
#include "bbf.h"

/** 
 * Read a vars list from disk to memory
 * 
 * @param fullvars 
 *     Full (absolute) name of the vars disk file
 * @param fullvars_s 
 *     Length of \p fullvars
 * @param node
 *     Output vars storage system node number
 * @param nerr 
 *     Error Return Flag
 *     - 0 on Success
 *     - Non-Zero on Error
 *
 * @date   890227:  Changed argument list.
 * @date   881115:  Added output of node number.
 * @date   880321:  Included logic to find and save nil index after read.
 * @date   870916:  Original version.
 * @date   870916:  Documented/Reviewed
 *
 */
void 
readvfile(char *fullvars, 
          int   fullvars_s, 
          int  *node, 
          int  *nerr) {

  char *s1;

	*nerr = 0;
	*node = 0;

  s1 = strcut(fullvars, 1, indexb(fullvars, fullvars_s));
  if((*nerr = sac_vars_read(s1))) {
    error(*nerr, "%s", s1);
  }
  free(s1);

  return;
}

