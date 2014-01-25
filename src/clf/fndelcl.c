/** 
 * @file   fndelcl.c
 * 
 * @brief   Delete and entry in a character list
 * 
 */

#include "clf.h"
#include "bool.h"

/** 
 * Delete an entry in a character list
 * 
 * @param kcl 
 *   Character List
 * @param kcl_s 
 *   Lenght of \p kcl
 * @param fileNumber 
 *   Entry to delete
 *
 * @return 
 *   - TRUE \p fileNumber was found and deleted
 *   - FALASE \p fileNumber was not found
 *
 * @note Local Variables:
 *    NCL:     Number of characters in KCL. [i]
 *    KDEL:    Delimiter between entries. [c1]
 *    IDEL:    Used to search for delimiter in character list. [i]
 *
 * @date   980922:  Original version. plagerized from ldelcl.c.  maf
 *
 */
int 
fndelcl(char *kcl, 
        int   kcl_s, 
        int   fileNumber ) {

	char *ptr1 , *ptr2 ;
	int ldelcl_v;
	int index1, index2, j1, idx ;

        idx = 1;

	/* - Assume the entry will not be found. */
	ldelcl_v = FALSE;

	/* - Initialize character pointer and determine length of entry. */
	index1 = 0;

	/* - Loop on each ENTRY in character list. */
	while ( lnxtcl( kcl,kcl_s, &index1, &index2 ) ){

          /* -- If match, delete characters in list, fill with delimiter, 
           *    and set return value to .TRUE. */
		if( idx == fileNumber ) {
			ptr1 = kcl + index1 - 2 ;
			ptr2 = kcl + index2 ;

			for ( j1 = index2 ; j1 < kcl_s - 1 ; j1++ , ptr1++ , ptr2++ )
			    *ptr1 = *ptr2 ;

			while ( ptr1 < ptr2 ) {
			    *ptr1 = ' ' ;
			    ptr1++ ;
			}

			ldelcl_v = TRUE;

			break ;
		}

		idx++ ;
	}

	return( ldelcl_v );
}

