/** 
 * @file   lnxtcl.c
 * 
 * @brief  Get the next entry from a character list
 * 
 */

#include "clf.h"
#include "bool.h"
#include "bot.h"

/** 
 * Get the next entry from a character list
 * 
 * @param kcl 
 *    Character list
 * @param kcl_s 
 *    Length of \p kcl
 * @param index1 
 *    Index pointing to first character of old entry on input
 *    Index pointing to first character of new entry on output
 * @param index2 
 *    Index pointing to last character of old entry on input
 *    Index pointing to last character of new entry on output
 * 
 * @return 
 *
 * @date   900119:  Was not determing length of character list each time.
 * @date   870929:  Was not determining delimiter each time.
 * @date   860918:  Changed initialization logic.
 * @date   860128:  Original version. 
 *
 */
int 
lnxtcl(char *kcl, 
       int   kcl_s, 
       int  *index1, 
       int  *index2) {

	int lnxtcl_v;
	char  kdel;
	int idel, ncl;
        char *cstart;

	/* - Determine delimiter and length of character list. */
	kdel = kcl[0];
	ncl = (kcl_s - 1);

	/* - Set start pointer if first pass.
	 *   Increment start pointer if not first pass. */
	if( *index1 <= 0 ){
		*index1 = 2;
        }
	else{
		*index1 = *index2 + 2;
        }

        cstart = kcl+*index1-1;

L_1000:

	idel = indexa( cstart, ncl-*index1+2, kdel, TRUE, TRUE );

	/* - See if we have an entry or not. */
	if( idel > 1 ){
		lnxtcl_v = TRUE;
		*index2 = *index1 + idel - 2;
		}
	else if( idel == 1 ){
		/* lnxtcl_v = TRUE; */
		*index1 = *index1 + 1;
                cstart++;
		goto L_1000;
		}
	else{
		lnxtcl_v = FALSE;
		*index2 = 0;
		}

	return( lnxtcl_v );
}

