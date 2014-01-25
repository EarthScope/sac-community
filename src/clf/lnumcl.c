/** 
 * @file   lnumcl.c
 * 
 * @brief  Get a entry from a character list
 * 
 */

#include "clf.h"
#include "bool.h"

/** 
 * Get a specific entry from a character list
 * 
 * @param kcl 
 *    Character list
 * @param kcl_s 
 *    Length of \p kcl
 * @param num 
 *    Number of entry to get
 * @param index1 
 *    Index pointing to first character in \p kcl
 * @param index2 
 *    Index pointing to last character in \p kc l
 * 
 * @return 
 *   - TRUE \p num was found
 *   - FALASE \p num was not found
 *
 * @date   860917:  Changed initialization method.
 * @date   860128:  Original version.
 *
 */
int 
lnumcl(char *kcl, 
       int   kcl_s, 
       int   num, 
       int  *index1, 
       int  *index2) {

	int lnumcl_v;
	int j;

	/* - Initialize pointer. */
	*index1 = 0;

	/* - Loop until we have the specific entry or list is exhausted. */
	j = 1;
L_1000:
	if( lnxtcl( kcl,kcl_s, index1, index2 ) ){
		if( j == num ){
			lnumcl_v = TRUE;
                }
		else{
			j = j + 1;
			goto L_1000;
                }
        }
	else{
		lnumcl_v = FALSE;
        }

	return( lnumcl_v );
}

