/** 
 * @file   lcia.c
 * 
 * @brief  Parse an integer array 
 * 
 */

#include "cpf.h"
#include "com.h"
#include "bool.h"

/** 
 * Parse an integer array command construct
 * 
 * @param mnint 
 *    Minimum Number of integers to return
 * @param mxint 
 *    Maximum Number of integers to return
 * @param ia 
 *    Output integer array
 * @param nia 
 *    Entries in \p ia
 * 
 * @return 
 *    - TRUE if the integer array was found
 *    - FALSE if the integer array was not found
 *
 *    820624:  Original version.
 *
 */
int
lcia(int mnint, 
     int mxint, 
     int *ia, 
     int *nia) {
	int lcia_v;
	int nerr;
  Token *t;

	/* - Get integer variables until maximum is reached or
	 *   until an alphanumeric token is found.
	 * - Perform standard command error recovery if not found. */

	*nia = 0;
	lcia_v = TRUE;
  while(lcmore(&nerr)) {
    if((t = arg()) && token_is_int(t)) {
      if(*nia < mxint) {
        ia[*nia] = token_as_int(t);
        arg_next();
        (*nia)++;
      } else {
        goto L_8888;
      }
    } else if(*nia >= mnint) { /* Have ay least min ints */
      goto L_8888;
    } else if(*nia > 0) { /* Have at least 1, but less than min */
      cfmt("NEED A INTEGER VARIABLE:",26 );
      cresp();
      if( lcmore( &nerr ) )
        continue;
      lcia_v = TRUE;
      goto L_8888;
    } else { /* No ints */
      lcia_v = FALSE;
      goto L_8888;
    }
  }

L_8888:
	return( lcia_v );

}

