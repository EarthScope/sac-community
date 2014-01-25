/** 
 * @file   selectinputfi.c
 * 
 * @brief  Select the active entries in the input data file list
 * 
 */

#include "datafilelist.h"
#include "co.h"

/** 
 * Select the active entres in the input data file list for use in
 *    subsequent action commands
 * 
 * @param list 
 *    List of active entries in the input data file list
 *    Numbers in \p list refer to the order of the entries in
 *    the data file list.  An entry in \p list becomes active 
 *    for subsequent commands
 * @param nlist 
 *    Length of \p list
 *
 * @date   900409:  Original version.
 *
 */
void 
selectinputfiles(int  *list, 
		 int   nlist) {
	int j;

	int *const List = &list[0] - 1;

	/* - Save list in common block. */
	cmdatafilelist.nselect = min( nlist, MDFL );

	for( j = 1; j <= nlist; j++ ){
		Iselect[j] = List[j];
	}

	return;
}

