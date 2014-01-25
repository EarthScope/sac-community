/** 
 * @file   getbbv.c
 * 
 * @brief  Get a Blackboard variable
 * 
 */

#include <string.h>
#include <stdlib.h>

#include "bool.h"
#include "bbs.h"
#include "dff.h"
#include "bot.h"
#include "vars.h"
#include "co.h"

/** 
 * Get a Blackboard Variable
 * 
 * @param kname 
 *   Blackboard variable name
 * @param kvalue 
 *   Blackboard variable value
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - Non-Zero on Failure
 * @param kname_s 
 *   Length of \p kname
 * @param kvalue_s 
 *   Length of \p kvalue
 *
 * @date  870301:  Original version.
 * @date  870301:  Documented / Reviewed
 *
 */
void 
getbbv(char  *kname, 
       char  *kvalue, 
       int   *nerr, 
       int    kname_s, 
       int    kvalue_s) {

	char ktemp[33];
	int nc;

	int callFromC = 0;
	char *kname_c;

	if(kname_s < 0) {
	  callFromC = 1;
	}
	kname_c  = fstrdup(kname, kname_s);
	kname_s  = strlen(kname_c) + 1;
	
	nc = min( indexb( kname,kname_s ), 32 );
	strcpy( ktemp, "                                " );
	modcase( TRUE, kname, nc, ktemp );
  ktemp[nc] = 0;
	getvvstring( kmbbs.knmbbs,MCPFN+1, ktemp,33, &nc, kvalue,kvalue_s, 
	 nerr );
	if( *nerr != 0 ) {
        if(callFromC) {
            nc = max(0,min(kvalue_s-1, strlen("UNDEFINED")));
        } else {
            nc = max(0,min(kvalue_s,   strlen("UNDEFINED")));
        }
        strncpy( kvalue, "UNDEFINED", nc);
	}

	if(callFromC) {
	  kvalue[nc] = 0;
	} else {
	  if(nc < kvalue_s) {
	    memset(kvalue + nc, ' ', kvalue_s - nc);
	  }
	}

	free(kname_c);

	return;

}

var * 
getbb(char *name) {
  var *v;
  char *key;
  if(!name) {
    return NULL;
  }
  key = upcase_dup(name);
  v = sac_vars_get_var(kmbbs.knmbbs, key);
  free(key);
  key = NULL;
  return v;
}


/* Added for FORTRAN friendliness */
void getbbv_ (char      *kname, 
	      char      *kvalue, 
	      int       *nerr, 
	      int        kname_s, 
	      int        kvalue_s) {
  getbbv(kname, kvalue, nerr, kname_s, kvalue_s) ;
}
void getbbv__ (char      *kname, 
	       char      *kvalue, 
	       int       *nerr, 
	       int        kname_s, 
	       int        kvalue_s) {
  getbbv(kname, kvalue, nerr, kname_s, kvalue_s) ;
}
