/** 
 * @file   getnfiles.c
 * 
 * @brief  Get the number of files in the data file list
 * 
 */
#include "amf.h"
#include "dff.h"
#include "dfm.h"

/** 
 * Get the number of files in the data file list
 * 
 * @param nfiles 
 *    Number of files in the data file list
 *    Set to \p saclen()
 *
 *    900305:  Original version.
 *
 */
void 
getnfiles(int *nfiles) {

	*nfiles = saclen();
	return;
}

