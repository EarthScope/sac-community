/** 
 * @file   zclose.c
 * 
 * @brief  Close a file
 * 
 */

#include "co.h"

/** 
 * Close a disk file if open
 * 
 * @param nfu 
 *    Fortran file unit
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   880826:  Fixed bug with F77--a call to close when there is no
 *             file open on file unit ties up that file unit. Added
 *             a check to make sure file is open before calling close.
 * @date    851216:  Modified for UNIX file I/O--D. Trimmer
 * @date   830812:  Original version.
 *
 * @note Only calls co/zclosec()
 *
 */
void 
zclose(int *nfu, 
       int *nerr)
{
	*nerr = 0;

	if( *nfu < 0 ){
	  zclosec( (int *) nfu );
	}

	return;
}

