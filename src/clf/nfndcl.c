/** 
 * @file   nfndcl.c
 * 
 * @brief  Find an entry in a character list
 * 
 */

#include <string.h>

#include "clf.h"
#include "bot.h"
#include "co.h"

/** 
 * Find an entry in a character list
 * 
 * @param kcl 
 *    Character List
 * @param kcl_s 
 *    Length of \p kcl
 * @param kentry 
 *    Entry to search for
 * @param kentry_s 
 *    Length of \p kentry
 * @param index1 
 *    Index of first character in entry
 * @param index2 
 *    Index of last character in entry
 * 
 * @return 
 *   - TRUE - entry was found
 *   - FALSE - entry was not found
 *
 * @date   860918:  Original version.
 *
 */
int 
nfndcl(char *kcl, 
       int   kcl_s, 
       char *kentry, 
       int   kentry_s, 
       int  *index1, 
       int  *index2) {

	int jentry, nentry, nfndcl_v;

	/* - Assume the worst. */
	nfndcl_v = 0;

	/* - Initialize character pointer and determine length of entry. */
	*index1 = 0;
	nentry = indexb( kentry,kentry_s );

	/* - Loop on each token in character list. */
	jentry = 1;

L_1000:
	if( lnxtcl( kcl,kcl_s, index1, index2 ) ){

		/* -- If match, set index and return. */
		if( memcmp(kentry,kcl+*index1 - 1,max(nentry,*index2 - *index1 + 1)) == 0 ){
			nfndcl_v = jentry;
			goto L_8888;
                }
		else{
			/* -- Otherwise, increment counter and loop. */
			jentry = jentry + 1;
			goto L_1000;
                }
                
        }

L_8888:
	return( nfndcl_v );
}

