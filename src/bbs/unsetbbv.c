/** 
 * @file   unsetbbv.c
 * 
 * @brief  Unset a Blackboard variable
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "bool.h"
#include "bbs.h"
#include "co.h"
#include "bot.h"
#include "vars.h"
#include "dff.h"

/** 
 * Unset or delete a blackboard variable
 * 
 * @param kname 
 *    Name of the variable
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - Non-Zero on Failure
 * @param kname_s 
 *   Length of \p kname
 *
 * @date   880412:  Original version.
 * @date   880412:  Documented / Reviewed
 *
 */
void 
unsetbbv(char *kname, 
	 int  *nerr, 
	 int   kname_s) {

	char ktemp[33];
	int nc;

	char *kname_c;
	
	kname_c = fstrdup(kname, kname_s);
	kname_s = strlen(kname_c) + 1;
	
	nc = min( indexb( kname_c,kname_s ), 32 );
	strcpy( ktemp, "                                " );
	modcase( TRUE, kname_c, nc, ktemp );
  ktemp[nc] = 0;
	deletev( kmbbs.knmbbs,MCPFN+1, ktemp,33, nerr );

	free(kname_c);

	return;
}



void unsetbbv_ (char      *kname, 
		int       *nerr, 
		int        kname_s) {
  unsetbbv(kname, nerr, kname_s) ;
}
void unsetbbv__ (char      *kname, 
		 int       *nerr, 
		 int        kname_s) {
  unsetbbv(kname, nerr, kname_s) ;
}
