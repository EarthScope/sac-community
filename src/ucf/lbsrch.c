/** 
 * @file   lbsrch.c
 * 
 * @brief  Search a string
 * 
 */

#include <string.h>

#include "ucf.h"
#include "bool.h"
#include "debug.h"

/** 
 * Search a string
 * 
 * @param ksrch 
 *    String to search for
 * @param ncsrch 
 *    Length of \p ksrch
 * @param klist 
 *    List to search
 * @param klist_s 
 *    Length of \p klist
 * @param nlist 
 *    Number of elements in klist
 * @param index 
 *    Index in \p klist of match
 *    - 0 if not match was found
 * 
 * @return
 *    - TRUE if a match was found
 *    - FALSE if a match was not found
 *
 * @date   820617:  Original version.
 *
 */
int 
lbsrch(char *ksrch, 
       int   ncsrch, 
       char *klist, 
       int   klist_s, 
       int   nlist, 
       int  *index) {

#define KLIST(I_,J_)	(klist+(I_)*(klist_s)+(J_))

	int lbsrch_v;
	int jbot, jcur, jtop;

  UNUSED(ncsrch);
	/* - Assume the worst. */
	*index = 0;
	lbsrch_v = FALSE;

	/* - Return immediately if string being search for is not
	 *   lexically contained within list. */

	if( strcmp( ksrch, KLIST(0,0) ) < 0|| 
            strcmp( ksrch, KLIST(nlist - 1,0)) > 0 ){
		goto L_8888;
		}
	else{
		jbot = 0;
		jtop = nlist + 1;
		}

	/* - Halve the search interval each iteration until a match is found or
	 *   the list is exhausted. */

L_1000:
	jcur = (jtop + jbot)/2;
	if( strcmp(ksrch,KLIST(jcur - 1,0)) == 0 ){
		*index = jcur;
		lbsrch_v = TRUE;
		}
	else if( strcmp( ksrch, KLIST(jcur - 1,0)) > 0){
		jbot = jcur;
		if( (jtop - jbot) != 1 )
			goto L_1000;
		}
	else{
		jtop = jcur;
		if( (jtop - jbot) != 1 )
			goto L_1000;
		}

L_8888:
	return( lbsrch_v );

#undef	KLIST

}

