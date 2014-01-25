/** 
 * @file   lcirc.c
 * 
 * @brief  Parse a range-checked integer
 * 
 */

#include <stdio.h>

#include "cpf.h"
#include "bool.h"
#include "com.h"

/** 
 * Parse a range-checked integer variable command construct
 * 
 * @param intmn 
 *    Minimum Integer value
 * @param intmx 
 *    Maximum Integer value
 * @param intv 
 *    Integer value on output
 * 
 * @return 
 *    - TRUE is the integer was found
 *    - FALSE is the integer was not found
 *
 * @bug This function is the same as cpf/lcint() but the range checking.
 * @bug Make sure the conversion from float to int to done correctly.
 *
 * @date   820423:  Adjustments due to new command parsing system.
 * @date   810207:  Original version.
 *
 */
int
lcirc(int  intmn, 
      int  intmx, 
      int *intv) { 

	int iv, nerr;
  Token *t;

	/* -- Get integer variable from next symbol.
	 * -- Check variable against allowed range.
	 * -- Perform standard command error recovery if out of range. */
L_2000:
  if((t = arg()) && token_is_int(t)) {
		iv = token_as_int(t);
		if( iv >= intmn && iv <= intmx ){
			*intv = iv;
      arg_next();
			return TRUE;
		}	else {
			cfmt( "OUTSIDE ALLOWED RANGE:",24 );
      fprintf(MUNOUT," Allowed range is: %10d%10d\n",intmn, intmx );
			cresp();
			if( lcmore( &nerr ) )
				goto L_2000;
      return TRUE;
		}
	}
	return FALSE;

}

