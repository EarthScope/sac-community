/** 
 * @file   settextwait.c
 * 
 * @brief  Set the text during wait mode
 * 
 */

#include <string.h>

#include "exm.h"
#include "bot.h"
#include "bool.h"

/** 
 * Set the text output wait mode
 * 
 * @param mode 
 *    - "ON" to cause SAC to pause and wait for the user to respond after a 
 *         screen full of output has been generated
 *    - "OFF" to cause SAC not to pause
 *
 * @bug The argument should be an int/enum
 *
 * @date   900410:  Original version.
 *
 */
void 
settextwait(char *mode) {
	char test[3];

	/* - Convert first two input characters to upper case. */
    memset(test, 0, sizeof(test));
	modcase( TRUE, mode, 2, test );

	/* - Test versus allowed options. */
	if( strcmp(test,"ON") == 0 ){
		strcpy( kmexm.ktextwait, "ON      " );
		}
	else if( strcmp(test,"OF") == 0 ){
		strcpy( kmexm.ktextwait, "OFF     " );
		}
	else{
		strcpy( kmexm.ktextwait, "ON      " );
		}

	return;
}

