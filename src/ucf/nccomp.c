/** 
 * @file   nccomp.c
 * 
 * @brief  Compare tokens
 * 
 */

#include <string.h>

#include "ucf.h"

/** 
 * Compare a token against a list of other tokens
 * 
 * @param ktoken 
 *    Token to search for
 * @param klist 
 *    List of tokens to search against
 * @param klist_s 
 *    Length of \p klist
 * @param nlist
 *    Number of tokens in \p klist 
 * @param nchar 
 *    Length of \p ktoken and \p klist
 * 
 * @return 
 *    - TRUE if \p ktoken was found in \p klist
 *    - FALSE if \p ktoken was not found in \p klist
 *
 * @date   820419:  Original version.
 *
 */
int 
nccomp(char *ktoken, 
       char *klist, 
       int   klist_s, 
       int   nlist, 
       int   nchar) {

#define KLIST(I_,J_)	(klist+(I_)*(klist_s)+(J_))

	int j, j_, nccomp_v;

	nccomp_v = 0;
	for( j = 1; j <= nlist; j++ ){
          j_ = j - 1;
          if( memcmp(ktoken,KLIST(j_,0),nchar) == 0 ){
            nccomp_v = j;
            goto L_8888;
          }
        }

L_8888:
	return( nccomp_v );

#undef	KLIST

}

