/** 
 * @file   lcircp.c
 * 
 * @brief  Parse a range check real pair
 * 
 */

#include <stdio.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"

/** 
 * Parse a range check real variable pair command construct
 * 
 * @param intmn 
 *    Minimum value
 * @param intmx 
 *    Maximum value
 * @param intv1 
 *    First value on output
 * @param intv2 
 *    Second value on output
 * 
 * @return 
 *    - TRUE if the output pair was found
 *    - FALSE if the output pair was not found
 *
 * @date   820622:  Original version.
 *
 */
int
lcircp(int  intmn, 
       int  intmx, 
       int *intv1, 
       int *intv2) {

	int lcircp_v;
	int iv, nerr;
  Token *t;

	/* - Get real variable from next symbol.
	 * - Check variable against allowed range.
	 * - Perform standard command error recovery if not found.
	 * - Repeat for second real. */
L_2000:
  if((t = arg()) && token_is_int(t)) {
		lcircp_v = TRUE;
		iv = token_as_int(t);
		if( iv >= intmn && iv <= intmx ){
			*intv1 = iv;
      arg_next();
L_3000:
      if((t = arg()) && token_is_int(t)) {
				iv = token_as_int(t);
				if( iv >= *intv1 && iv <= intmx ){
					*intv2 = iv;
          arg_next();
				}
				else{
					cfmt( "OUTSIDE ALLOWED RANGE:",24 );
          fprintf(stdout," Allowed range is: %10d%10d\n", *intv1, intmx );
					cresp();
					if( lcmore( &nerr ) )
						goto L_3000;
					lcircp_v = TRUE;
				}
			}
			else{
				cfmt( "NEED A INTEGER VARIABLE:",26 );
				cresp();
				if( lcmore( &nerr ) )
					goto L_2000;
				lcircp_v = TRUE;
			}
		}
		else{
			cfmt( "OUTSIDE ALLOWED RANGE:",24 );
      fprintf(stdout," Allowed range is: %10d%10d\n", intmn, intmx );
			cresp();
			if( lcmore( &nerr ) )
				goto L_2000;
		}
	}
	else{
		lcircp_v = FALSE;
	}

	return( lcircp_v );
}

