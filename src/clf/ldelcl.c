/** 
 * @file   ldelcl.c
 * 
 * @brief  Delete an entry in a character list
 * 
 */

#include <string.h>

#include "clf.h"
#include "bool.h"
#include "bot.h"
#include "co.h"

/** 
 * Delete an entry in a character list
 * 
 * @param kcl 
 *    Character list
 * @param kcl_s 
 *    Length of \p kcl
 * @param kentry 
 *    Entry to delete
 * @param kentry_s 
 *    Length of \p kentry
 * 
 * @return 
 *   - TRUE \p fileNumber was found and deleted
 *   - FALASE \p fileNumber was not found
 *
 * @note Local Variables
 *    NCL:     Number of characters in KCL. [i]
 *    IDEL:    Used to search for delimiter in character list. [i]
 *
 * @date   870429:  Corrected an indexing bug.
 * @date   860918:  Original version.
 *
 */
int 
ldelcl(char *kcl, 
       int   kcl_s, 
       char *kentry, 
       int   kentry_s) {

	int  ldelcl_v;
	int index1, index2, j1, j2, ncl, nentry;

	/* - Assume the entry will not be found. */
	ldelcl_v = FALSE;

	/* - Initialize character pointer and determine length of entry. */
	index1 = 0;

	nentry = indexb( kentry,kentry_s );

	/* - Loop on each ENTRY in character list. */
	while ( lnxtcl( kcl,kcl_s, &index1, &index2 ) ){

          /* -- If match, delete characters in list, fill with delimiter, 
           *    and set return value to .TRUE. */

		if( memcmp(kentry,kcl+index1 - 1,max(nentry,index2-index1+1)) == 0 ){
			ncl = (kcl_s - 1);
            j2 = ncl;
			for( j1 = index2 + 2; j1 <= ncl; j1++ ){
				j2 = j1 - nentry - 1;
				kcl[j2 - 1] = kcl[j1 - 1];
			}
			for( j1 = j2 + 1; j1 <= ncl; j1++ ){
				kcl[j1 - 1] = kcl[0];
			}
			ldelcl_v = TRUE;

			break ;
		}
	}

	return( ldelcl_v );
}

