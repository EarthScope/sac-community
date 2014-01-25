/** 
 * @file   lcmore.c
 * 
 * @brief  Tell if more tokens exist
 * 
 */
#include <stdio.h>
#include "cpf.h"
#include "com.h"

/** 
 * Tell whether there are more tokens in the current command
 * 
 * @param nerr 
 *    - Command Error Number
 * 
 * @return 
 *    - TRUE if more token are available
 *    - FALSE if no more token are available
 *
 * @date   820420:  Original version.
 *
 */
int 
lcmore(int *nerr) {

  //	int lcmore_v;
  *nerr = cmcom.ncerr;
  return (arg() && *nerr == 0);

	/* - Set function value to .TRUE. if:
	 *   (1) There are more tokens in current command  AND
	 *   (2) No parsing error has occurred since last call */
	//lcmore_v = cmcom.jcom <= cmcom.ncom && cmcom.ncerr == 0;

	/* - Also return the command error number. */
	//*nerr = cmcom.ncerr;

	//return( lcmore_v );
}

