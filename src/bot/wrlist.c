/** 
 * @file   wrlist.c
 *
 * @brief  Write a list of tokens
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mach.h"
#include "bot.h"
#include "co.h"


/** 
 * Write a list of tokens to the user's terminal. Tokens are written with
 *    two spaces between them.  Numer of tokens per line is base on the 
 *    length of the longest token.
 * 
 * @param klist 
 *    List of tokens
 * @param klist_s 
 *    Length of \p klist
 * @param nlist 
 *    Number of entries of \p klist
 *
 * @date   860124:  Added ability to determine format based on token size.
 * @date   840119:  Original version.
 *
 */
void 
wrlist(char *klist, 
       int   klist_s, 
       int   nlist)
{

#define KLIST(I_,J_)	(klist+(I_)*(klist_s)+(J_))

	char kmsg[MCMSG+1];
	int j, j1, j2, j_, jc1, jc2, 
	 ncmax, nperl;
    char *strtemp;

    if(nlist <= 0) {
        return;
    }
	/* - Determine maximum number of characters in list of tokens. */
	ncmax = 0;
	for( j = 1; j <= nlist; j++ ){
		j_ = j - 1;
		ncmax = max( ncmax, indexb( KLIST(j_,0),klist_s ) );
	}

	/* - Determine how many tokens to put on each line.
	 *   (Two spaces will be put between each token.) */
	nperl = MCMSG/(2 + ncmax);

	/* - Write each line to standard output. */
	for( j1 = 1; j1 <= nlist; j1 += nperl){
		j2 = min( j1 + nperl - 1, nlist );
                memset(kmsg,(int)' ',MCMSG);
                kmsg[MCMSG] = '\0';
		jc1 = 3;
        jc2 = 1;
		for( j = j1; j <= j2; j++ ){
			j_ = j - 1;
			jc2 = jc1 + ncmax - 1;
			subscpy( kmsg, jc1 - 1, jc2 - 1, MCMSG, KLIST(j_,0) );
			jc1 = jc2 + 2;
		}
                strtemp = malloc(jc2+1);
                strncpy(strtemp,kmsg,jc2);
                strtemp[jc2] = '\0';
                fprintf(stdout," %s\n",strtemp);
                free(strtemp);
	}

	return;

#undef	KLIST
}

