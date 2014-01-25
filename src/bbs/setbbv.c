/** 
 * @file   setbbv.c
 * 
 * @brief  Set a Blackboard Variable
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "bbs.h"
#include "bool.h"
#include "co.h"
#include "bot.h"
#include "dff.h"
#include "vars.h"

/** 
 * Set or Define a Blackboard Variable
 * 
 * @param kname 
 *    Name of the variable
 * @param kvalue 
 *    Value of the variable
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - Non-Zero on Failure
 * @param kname_s 
 *   Length of \p kname
 * @param kvalue_s 
 *   Length of \p kvalue
 *
 * @date   870301:  Original version.
 * @date   870301:  Documented / Reviewed
 *
 */
void 
setbbv(char *kname, 
       char *kvalue, 
       int  *nerr, 
       int   kname_s, 
       int   kvalue_s) {

	char ktemp[33] = "                                " ;
	int nc;

	char *kname_c;
	char *kvalue_c;

	kname_c  = fstrdup(kname, kname_s);
	kvalue_c = fstrdup(kvalue, kvalue_s);

	kname_s  = strlen(kname_c) + 1;
	kvalue_s = strlen(kvalue_c) + 1;

 	nc = min( indexb( kname_c,kname_s ), 32 ); 
 	modcase( TRUE, kname_c, nc, ktemp ); 
	nc = indexb( kvalue_c,kvalue_s );
	putvvstring( kmbbs.knmbbs,MCPFN+1, ktemp,33, nc, kvalue_c,kvalue_s, nerr );

	free(kname_c);
	free(kvalue_c);

	return;
}

int
setbb(char *name, int type, ...) {
  int retval;
  va_list ap;
  va_start(ap, type);
  retval = setvar_ap(kmbbs.knmbbs, name, type, ap);
  va_end(ap);
  return retval;
}

int
token_to_bb(Token *tok, char *name) {
  return token_to_var(tok, kmbbs.knmbbs, name);
}


/* Added for FORTRAN friendliness */
void setbbv_ (char      *kname, 
	      char      *kvalue, 
	      int       *nerr, 
	      int        kname_s,
	      int        kvalue_s) {
  setbbv(kname, kvalue, nerr, kname_s, kvalue_s) ;
}
void setbbv__ (char      *kname, 
	       char      *kvalue, 
	       int       *nerr, 
	       int        kname_s,
	       int        kvalue_s) {
  setbbv(kname, kvalue, nerr, kname_s, kvalue_s) ;
}
