/** 
 * @file   lklist.c
 * 
 * @brief  Parse a keyed alphanumeric list 
 * 
 */

#include <stdio.h>
#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"
#include "bot.h"


#include "ucf.h"

#define	MMATCH	5

/** 
 * Parse a keyed alphanumeric list command construct
 * 
 * @param kkey 
 *    Keyword to search for
 * @param kkey_s 
 *    Length of \p kkey
 * @param klist 
 *    List of possible alphanumeric tokens to search for
 * @param klist_s 
 *    Length of \p klist
 * @param nlist 
 *    Entires in \p klist
 * @param index 
 *    Index in \p klist of the alphanumeric token found
 * 
 * @return 
 *    - TRUE if a token was found
 *    - FALSE if a token was not found
 *
 * @date   870730:  Added logic to convert current token to uppercase.
 * @date   860314:  Changed logic due to use of WRINDX instead of WRLIST.
 * @date   820927:  Fixed bug involving an exact and inexact match.
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   820312:  Factored test for key to LCCKEY function.
 * @date   810207:  Original version.
 *x
 */
int
lklist(char *kkey, 
       int   kkey_s, 
       char *klist, 
       int   klist_s, 
       int   nlist, 
       int  *index) {

	/* - Check for key. */
	if(!lckey( kkey,kkey_s )) {
    return FALSE;
  }

  lclist(klist, klist_s, nlist, index);
  return TRUE;
}

