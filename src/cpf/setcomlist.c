/** 
 * @file   setcomlist.c
 * 
 * @brief  Set the command list number
 * 
 */

#include "cpf.h"
#include "com.h"
#include "comlists.h"

/** 
 * Set the command list number, which define the set of commands
 *    that are available for execution
 * 
 * @param number 
 *    - 1 for Standard SAC commands
 *    - 2 for Spectral Estimation Subprocess (SES) commands
 *    - 3 for Signal Stacking Subprocess (SSS) commands
 *
 * @date   900804:  Changed from clf to comlist.
 * @date   861203:  Original version.
 *
 */
void 
setcomlist(int number) {

	if( number >= 1 && number <= 3 )
	  cmcomlists.icomlist = number;

	return;
}

