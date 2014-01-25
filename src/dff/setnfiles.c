/** 
 * @file   setnfiles.c
 * 
 * @brief  Set the number of files in the data file list
 * 
 */

#include "mach.h"
#include "dff.h"
#include "dfm.h"
#include "co.h"

/** 
 * Set the number of files in the data file list
 * 
 * @param nfiles 
 *    Number of files in the data file list
 *
 * @see \p cmdfm.ndfl
 *
 * @date   900305:  Original version.
 *
 */
void 
setnfiles(int nfiles)  {

	cmdfm.ndfl = min( nfiles, MDFL );

	return;
}

