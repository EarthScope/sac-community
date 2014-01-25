/** 
 * @file   setinputmode.c
 * 
 * @brief  Set the input data file list mode
 * 
 */

#include <string.h>

#include "datafilelist.h"
#include "bool.h"


#include "bot.h"

/** 
 * Set the input data file list mode
 * 
 * @param mode 
 *    - 'ALL' to activate all entries in the data file list
 *    - 'SELECT' to activate selected entries in the data file list
 *        Use selectinputfiles() to select specific entries
 *
 * @date   900409:  Original version.
 *
 */
void 
setinputmode(char *mode) {

	char test;

	/* - Convert first input character to upper case. */
	modcase( TRUE, mode, 1, &test );

	/* - Test versus allowed options. */
	if( test == 'A' ){
		strcpy( kmdatafilelist.kselectmode, "ALL     " );
	}
	else if( test == 'S' ){
		strcpy( kmdatafilelist.kselectmode, "SELECT  " );
	}
	else{
		strcpy( kmdatafilelist.kselectmode, "ALL     " );
	}

	return;
}

