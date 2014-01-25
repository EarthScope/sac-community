/** 
 * @file   lkentries.c
 * 
 * @brief  Parse a keyed alpha numeric entry list
 * 
 */

#include <string.h>

#include "cpf.h"
#include "bool.h"


#include "co.h"
#include "bot.h"

#define ERROR_MESSAGE "NEED \"ALL\", \"NONE\", or one or more of the following:"

/** 
 * Parse a keyed alpha numeric entry list command construct
 * 
 * @param kkey 
 *    Key to search for
 * @param kkey_s 
 *    Length of \p kkey
 * @param klist 
 *    Keywords to search for
 * @param klist_s 
 *    Length of \p klist
 * @param nlist 
 *    Entries in \p klist
 * @param llist 
 *    Logical array indicating which entries were found.
 *      llist(j) is set to .TRUE. if  entry klist(j) was found
 *      and set to .FALSE. if not found.
 *      The keyword "ALL" sets all entries to .TRUE.
 *      The keyword "NONE" sets all entries to .FALSE.
 *      Parsing ends when the command is exhausted or a
 *      token is found that does not match the entry list.
 * 
 * @return 
 *    - TRUE if the list was found
 *    - TRUE if the list was not found
 *
 * @date   890105:  Original version.
 *
 */
int
lkentries(char *kkey, 
	  int   kkey_s, 
	  char *klist, 
	  int   klist_s, 
	  int   nlist, 
	  int  *llist) {

#define KLIST(I_,J_)	(klist+(I_)*(klist_s)+(J_))

	char kmsg[MCMSG+1];
	int lcentries, lkentries_v;
	int index, j, nerr;

	int *const Llist = &llist[0] - 1;

	/* - Check next token for key.
	 *   Return if key is not found. */
	lkentries_v = lckey( kkey,kkey_s );
	if( !lkentries_v )
		goto L_8888;

	/* - Initialize output values. */

	lcentries = FALSE;
	for( j = 1; j <= nlist; j++ ){
		Llist[j] = FALSE;
		}

	/* - Loop until command is exhausted or 
	 *   the next token does not match. 
	 */
L_2000:
	if( lcmore( &nerr ) ){

		/* -- Test for individual entries. */
		if( lclist( klist,klist_s, nlist, &index ) ){
			lcentries = TRUE;
			Llist[index] = TRUE;
			goto L_2000;

		}
		/* -- Test for ALL keyword. */
		else if( lckey( "ALL#$",6 ) ){
			/* lcentries = TRUE; */
			for( j = 1; j <= nlist; j++ ){
				Llist[j] = TRUE;
				}

		}
		/* -- Test for NONE keyword. */
		else if( lckey( "NONE#$",7 ) ){
			/* lcentries = TRUE;*/
			for( j = 1; j <= nlist; j++ ){
				Llist[j] = FALSE;
			}
		}
		/* -- Perform standard error recovery if we still 
		 *    have not found an entry. 
		 */
		else if( !lcentries ){
		  fstrncpy(kmsg,MCMSG, ERROR_MESSAGE, strlen(ERROR_MESSAGE));
		  cfmt( kmsg,MCMSG+1 );
		  wrlist( klist,klist_s, nlist );
		  cresp();
		  goto L_2000;
		}
	}

L_8888:
	return( lkentries_v );

#undef	KLIST

}

