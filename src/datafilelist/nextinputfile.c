/** 
 * @file   nextinputfile.c
 * 
 * @brief  Get the next entry in the data file list
 * 
 */

#include <string.h>

#include "datafilelist.h"
#include "bool.h"


#include "dff.h"

/** 
 * Get the next entry in the input data file list
 * 
 * @param ientry 
 *    - 0 to initialize
 *    - Do not change between calls
 * 
 * @return 
 *    - TRUE if there are more entries in the data file list
 *    - FALSE if there are no more entries in the data file list
 *
 * @date   900409:  Original version.
 *
 */
int 
nextinputfile(int *ientry) {

	int nextinputfile_v;

	/* - If in "ALL" mode: */
	if( strcmp(kmdatafilelist.kselectmode,"ALL     ") == 0 ){

		/* -- Initialize entry pointer and number of files */
		if( *ientry <= 0 ){
			*ientry = 0;
			getnfiles( &cmdatafilelist.nentries );
		}

		/* -- Increment entry number */
		if( *ientry < cmdatafilelist.nentries ){
			*ientry = *ientry + 1;
			nextinputfile_v = TRUE;
		}
		else{
                       /* -- Zero entry number and return with FALSE 
			*    value if no more entries. */
		       *ientry = 0;
		       nextinputfile_v = FALSE;
		}
	}
	else{
		/* - If in "SELECT" mode: */

		/* -- Initialize select pointer and if necessary. */
		if( *ientry <= 0 )
			cmdatafilelist.jselect = 0;

		/* -- Increment select pointer if there are more selections.
		 *    Set entry number to selected entry. */
		if( cmdatafilelist.jselect < cmdatafilelist.nselect ){
			cmdatafilelist.jselect = cmdatafilelist.jselect + 1;
			*ientry = Iselect[cmdatafilelist.jselect];
			nextinputfile_v = TRUE;
		}
		else{
			/* -- Zero entry number and return with FALSE 
			 *    value if no more selections. */
			*ientry = 0;
			nextinputfile_v = FALSE;
		}
	}

	return( nextinputfile_v );

}

