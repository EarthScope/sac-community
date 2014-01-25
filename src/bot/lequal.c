/** 
 * @file   lequal.c
 * 
 * @brief  Search for a token from a list of tokens
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mach.h"
#include "bool.h"
#include "bot.h"
#include "co.h"

#define	MMATCH	5

#include "string_utils.h"

/** 
 * To search for a token from a list of tokens.
 * 
 * @param ksrch 
 *    Token to search for
 * @param ksrch_s 
 *    Length of \p ksrch
 * @param klist 
 *    List of possible tokens to search for
 * @param klist_s 
 *    Length of \p klist
 * @param nlist 
 *    Length of \p klist
 * @param index 
 *    Output index in \p klist of the token found
 *
 * @return 
 *    - TRUE if the token was found
 *    - FALSE if the token was not found
 *
 * @note Local Variables
 *   - MMATCH:  Maximum number of allowed matches. [ip]
 *   - nmatch:  Number of token-list matches. [i]
 *   - imatch:  Indices (hopefully only one) of matched items. [ia]
 *   - kmsg:    Response from user when there is more than one match. [c]
 *
 * @date   830406:  Added logic to handle quoted string at current token.
 * @date   820927:  Fixed bug involving an exact and inexact match.
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   820312:  Factored test for key to LCCKEY function.
 * @date   810207:  Original version.
 */

int 
lequal(char *ksrch, 
       int   ksrch_s, 
       char *klist, 
       int   klist_s, 
       int   nlist, 
       int  *index)
{

#define KLIST(I_,J_)	(klist+(I_)*(klist_s)+(J_))

	char ktoken[MCMSG+1];
	int  lequal_v;
	int imatch[MMATCH], j, j_, nc, nmatch;
        char *strtemp;

	int *const Imatch = &imatch[0] - 1;

        fstrncpy( ktoken, MCMSG, " ", 1 );
        
	nc = indexb( ksrch,ksrch_s );
    if(nc <= 0) {
        fprintf(stderr, "lequal: Error finding length of input\n");
        return FALSE;
    }
	modcase( TRUE, ksrch, nc, ktoken );
L_2000:
	nmatch = 0;

	/* - Check token against each item in list.
	 * -- If it is an exact match, return with the result.
	 * -- If only a match through "NC" characters, saved as
	 *    as a possible match and continue search. */

	for( j = 1; j <= nlist; j++ ){
		j_ = j - 1;
		if( memcmp(ktoken,KLIST(j_,0),strlen(KLIST(j_,0))) == 0 ){
			nmatch = 1;
			Imatch[1] = j;
			goto L_3100;
		}
		else if( memcmp(ktoken,KLIST(j_,0),nc) == 0)   {
			if( nmatch < MMATCH )
				nmatch = nmatch + 1;
			Imatch[nmatch] = j;
		}
	}

	/* - If only one match, return with this result. */

L_3100:
	if( nmatch == 1 ){
		*index = Imatch[1];
		lequal_v = TRUE;

		/* - If more than one match, perform standard error recovery. */

	}
	else if( nmatch > 0 ){
                strtemp = malloc(nc+1);
                strncpy(strtemp,ktoken,nc);
                strtemp[nc] = '\0';

                fprintf(stdout," %s%s\n", strtemp, " is an AMBIGUOUS OPTION." );

                free(strtemp);

                fprintf(stdout," %s\n", "Possible matches are:");

		for( j = 1; j <= nmatch; j++ ){
                        fprintf(stdout,"   %s\n", KLIST(Imatch[j] - 1,0));
		}
                fprintf(stdout," %s", "Please enter desired option or <cr>.");
                /* gets(ktoken); */
                fgetsp( ktoken, MCMSG, stdin ) ;
		nc = indexb( ktoken,MCMSG+1 );
		if( nc <= 0 ){
			lequal_v = FALSE;
			*index = 0;
			goto L_8888;
		}
		else{
			modcase( TRUE, ktoken, nc, ktoken );
			goto L_2000;
		}

		/* - If no match found, return with LEQUAL set to .FALSE. */

	}
	else{
		lequal_v = FALSE;
		*index = 0;
	}

L_8888:
	return( lequal_v );

#undef	KLIST
}

