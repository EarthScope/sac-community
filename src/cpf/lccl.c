/** 
 * @file   lccl.c
 * 
 * @brief  Parse a character list
 * 
 */

#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"


#include "clf.h"

/** 
 * Parse a character list command construct
 * 
 * @param kcl 
 *    Output character list
 * @param kcl_s 
 *    Length of \p kcl
 * @param ncl 
 *    Entries in \p kcl
 * 
 * @return 
 *    - TRUE if the list was found
 *    - FALSE if the list was not found
 *
 * @bug Error condition raised internally and never returned.
 *
 * @date   880923:  Original version (based on LCDFL.)
 *
 */
int 
lccl(char *kcl, 
     int   kcl_s, 
     int  *ncl) {

	char ktoken[MCPFN+1];
	int lccl_v;
	int ncname, nerr;

	nerr = 0;
  memset(ktoken, 0, MCPFN+1);
	/* - Assume the worst. */
	lccl_v = FALSE;

	/* - Initialize the character list and counter. */
	memset ( kcl , ' ' , kcl_s - 1 );
	*ncl = 0;

	/* - Loop until tokens are exhausted. */

L_2000:
	if( arg() ) {

    memset(ktoken, ' ', sizeof(ktoken));
		/* -- Get next token and store it in the character list. */
		if( lcchar( MCPFN, ktoken,MCPFN+1, &ncname ) ){
			putcl( kcl,kcl_s, ktoken,MCPFN+1, &nerr );
			if( nerr != 0 )
				goto L_8888;
			lccl_v = TRUE;
			*ncl = *ncl + 1;

		}
		else{
			/* Raise error condition due to an unexpected token.*/
			nerr = 1001;
			goto L_8888;
		}

		/* -- Loop until command is exhausted. */
		goto L_2000;
	}

L_8888:
	return( lccl_v );
}

