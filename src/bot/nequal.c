/** 
 * @file   nequal.c
 * 
 * @brief  Search a list of characters
 * 
 */

#include <string.h>

#include "bot.h"
#include "co.h"

/** 
 * Search a list of characters to a character string
 * 
 * @param ksrch 
 *   Character string to search for
 * @param klist 
 *   Character string to search
 * @param klist_s 
 *   Length of \p klist
 * @param nlist 
 *   Number of entries in \p klist
 * 
 * @return Index of the match if found, 0 otherwise
 *
 * @date   810000:  Original version.
 *
 */
int 
nequal(char *ksrch, 
       char *klist, 
       int   klist_s, 
       int   nlist) {

#define KLIST(I_,J_)	(klist+(I_)*(klist_s)+(J_))

	int jdx, jdx_, nequal_v;

	nequal_v = 0;

	for ( jdx = 1 ; jdx <= nlist ; jdx++ ) {
		jdx_ = jdx - 1 ;
		if( memcmp ( ksrch , KLIST ( jdx_ , 0 ) ,
		    min ( strlen ( ksrch ) , klist_s ) ) == 0 ) {
			nequal_v = jdx ;
			return( nequal_v ) ;
		}
	}

	return( nequal_v ) ;

#undef	KLIST

}

