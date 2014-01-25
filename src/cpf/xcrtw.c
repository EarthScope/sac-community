/** 
 * @file   xcrtw.c
 * 
 * @brief  Parse a relative time window
 * 
 */
#include <stdio.h>
#include "cpf.h"

#include "errors.h"

/** 
 * Parse a relative time window 
 * 
 * @param lrtw 
 *    - TRUE if a relative time window wa turned on
 *    - FALSE if a relative time window wa turned off
 * @param krtw 
 *    Relative time window reference times
 *    Length = 2, First is the starting reference time
 *    Seconds is the stopping reference time
 * @param krtw_s 
 *    Length of \p krtw
 * @param ortw 
 *    Relative time window offset times, Length = 2
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_BAD_COMMAND_SYNTAX 
 * @date   820608:  Original version.
 *
 */
void 
xcrtw(int    *lrtw, 
      char   *krtw, 
      int     krtw_s, 
      double *ortw, 
      int    *nerr) {

	*nerr = 0;

	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){

		/* -- Check for RTW construct at current location. */
		if( lcrtw( lrtw, krtw,krtw_s, ortw ) )
		{ /* do nothing */ }
		else{
		        /* -- Bad syntax. */
			*nerr = ERROR_BAD_COMMAND_SYNTAX;
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();
		}
	}

	return;
}

