/** 
 * @file   wrindx.c
 * 
 * @brief  Display a list 
 * 
 */

#include <stdio.h>
#include <string.h>

#include "mach.h"
#include "ucf.h"


#include "co.h"
#include "bot.h"

/** 
 * Write a list of tokens to the user's terminal.  A list of 
 *   index number determines the order of writing.  Tokens
 *   are writtent with two spaces between them.  The number
 *   of tokens per line is based on the longest token.
 * 
 * @param klist 
 *    Token list
 * @param klist_s 
 *    Length of \p klist
 * @param ilist 
 *    Index number of tokens
 * @param nlist 
 *    Number of tokens in \p klist
 *
 * @date   860314:  Original version (based on WRLIST.)
 *
 */
void 
wrindx(char *klist, 
       int   klist_s, 
       int  *ilist, 
       int   nlist) {

#define KLIST(I_,J_)	(klist+(I_)*(klist_s)+(J_))

	char kmsg[MCMSG+1], ktemp[21];
	int j, j1, j2, jc1, jc2, 
	 nc, ncmax, nperl;

	int *const Ilist = &ilist[0] - 1;

	/* - Determine maximum number of characters in list of tokens. */
	ncmax = 0;
	for( j = 1; j <= nlist; j++ ){
		j1 = Ilist[j];
		fstrncpy( ktemp, 20, KLIST(j1 - 1,0), strlen(KLIST(j1 - 1,0)));
		nc = indexb( ktemp,21 );
		ncmax = max( ncmax, nc );
		}

	/* - Determine how many tokens to put on each line.
	 *   (Two spaces will be put between each token.) */

	nperl = MCMSG/(2 + ncmax);

	/* - Write each line to standard output. */

	for( j1 = 1; j1 <= nlist; j1 += nperl ){
		j2 = min( j1 + nperl - 1, nlist );
		fstrncpy( kmsg, MCMSG, " ", 1 );
		jc1 = 3;
		for( j = j1; j <= j2; j++ ){
			jc2 = jc1 + ncmax - 1;
			fstrncpy( ktemp, 20, KLIST(Ilist[j] - 1,0),
                                      strlen(KLIST(Ilist[j] - 1,0)));
			subscpy( kmsg, jc1 - 1, jc2 - 1, MCMSG, ktemp );
			jc1 = jc2 + 2;
    }
    rstrip(kmsg);
    fprintf(stdout," %s\n",kmsg);
  }

       
	return;

#undef	KLIST

}


