/** 
 * @file   lclist.c
 * 
 * @brief  Parse an alphanumeric list
 * 
 */

#include <stdio.h>
#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bot.h"
#include "bool.h"



#include "ucf.h"

#define	MMATCH	5

/** 
 * Parse an alphanumeric list command construct
 * 
 * @param klist 
 *    List of alphanumeric tokens to search for
 * @param klist_s 
 *    Length of \p klist
 * @param nlist 
 *    Entries in \p klist
 * @param index 
 *    Index in \p klist of the alpha numeric token found
 * 
 * @return 
 *    - TRUE if the a list item was found
 *    - FALSE if the a list item was not found
 *
 * @date   870730:  Added logic to convert current token to uppercase.
 * @date   860314:  Changed logic due to use of WRINDX instead of WRLIST.
 * @date   830406:  Added logic to handle quoted string at current token.
 * @date   820927:  Fixed bug involving an exact and inexact match.
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   820312:  Factored test for key to LCCKEY function.
 * @date   810207:  Original version.
 *
 */
int
lclist(char *klist, 
       int   klist_s, 
       int   nlist, 
       int  *index) {

#define KLIST(I_,J_)	(klist+(I_)*(klist_s)+(J_))

  Token *t;
	char ktoken[9];
	int lclist_v;
	int imatch[MMATCH], j, j_, nerr, nmatch;
	int *const Imatch = &imatch[0] - 1;

L_2000:
	nmatch = 0;
        
  strcpy(ktoken,"        ");
  if(!(t = arg()) || !token_is_string(t)) {
    return 0;
  } 
	/* - Check token against each item in list.
	 * -- If it is an exact match, return with the result.
	 * -- If only a match through "NCHAR" characters, saved as
	 *    as a possible match and continue search. */
	for( j = 1; j <= nlist; j++ ){
		j_ = j - 1;
    strncpy(ktoken, KLIST(j_,0), 8);
    ktoken[8] = 0;
    rstrip(ktoken);
    /*  Exact : If input string matches list string, up to length of list string
           and the length of the list string is longer than the input string
           or the list string is the max length (8)
    */
    if(strncasecmp(t->str,ktoken,strlen(ktoken)) == 0 &&
       (strlen(ktoken) >= strlen(t->str) || strlen(ktoken) == 8)){
      nmatch = 1;
      Imatch[1] = j;
      break ;
    }
    else if(strncasecmp(t->str,ktoken,strlen(t->str)) == 0 ){
      if( nmatch < MMATCH ) {
        nmatch = nmatch + 1;
      }
      Imatch[nmatch] = j;
    }
	}

	/* - If only one match, return with this result. */
	if( nmatch == 1 ){
		*index = Imatch[1];
    arg_next();
		lclist_v = TRUE;
	}

	/* - If more than one match, perform standard error recovery. */
	else if( nmatch > 0 ){
		cfmt( "AMBIGUOUS OPTION",18 );
    fprintf(stdout," Possible matches are:\n");
		wrindx( klist,klist_s, imatch, nmatch );
		cresp();
		if( lcmore( &nerr ) )
			goto L_2000;
		lclist_v = TRUE;
	}

	/* - If no match found, return with LCLIST set to .FALSE. */
	else{
		lclist_v = FALSE;
	}

	return( lclist_v );

#undef	KLIST

}

