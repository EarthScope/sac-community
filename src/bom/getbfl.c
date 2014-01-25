/** 
 * @file   getbfl.c
 * 
 * @brief  Get a Binary Op File 
 * 
 */

#include <string.h>
#include <stdlib.h>

#include "bool.h"
#include "bom.h"
#include "dfm.h"
#include "msg.h"
#include "clf.h"
#include "dff.h"

#include "errors.h"

/** 
 * Get a Binary Operations file from memory
 * 
 * @param ibfl 
 *    Binary Operation file index number
 * @param ldta 
 *    On input
 *    - TRUE if header and data are requested
 *    - FALSE if only header is requested
 * @param nlen 
 *    Length of each data component
 * @param ndx1 
 *    Index in SACMEM array of first data component
 * @param ndx2 
 *    Index in SACMEM array of second data component
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Succes
 *    - ERROR_ILLEGAL_BINARY_DATA_INDEX
 *
 * @note  NLEN, NDX1, and NDX2 are set to zero if LDTA if .FALSE.
 *        or NERR is nonzero.
 *
 * @date   980922:  Added parameter to call to rdsac to inhibit rdsac from
 *             adding the filename to the list of filenames.  maf
 * @date   880306:  Fixed bug when only headers were requested.
 * @date   870811:  Fixed bug when only one file was in the bfl.
 * @date   850730:  Changes due to new memory manager.
 *             Changes in argument lists for RDSAC.
 * @date   810224:  Fixed bug in computing memory index for binop file.
 * @date   810130:  Original version.
 *
 */
void 
getbfl(string_list *list,
       int  ibfl, 
       int  ldta, 
       int *nlen, 
       int *ndx1, 
       int *ndx2, 
       int *nerr) {

  char *file;
	*nerr = 0;

	/* - Make sure a valid binop file entry has been requested. */
	if( ibfl <= 0 || ibfl > string_list_length(list) ){
		*nerr = ERROR_ILLEGAL_BINARY_DATA_INDEX;
        error(*nerr, "%d", ibfl);
		*nlen = 0;
		*ndx1 = 0;
		*ndx2 = 0;
	}

	/* - Only read if binop file if it is different from current one. */
	else if( ibfl != cmbom.ibflc || TRUE){

		/* -- Release memory blocks for previous file. */
		relbfl( nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Read data file into memory.
		 *    (Use DFM index NDFL+1 to temporarily store pointers during read.) */
        file = string_list_get(list, ibfl-1);
		rdsac(cmdfm.ndfl + 1, file, strlen(file),
		      FALSE, ldta, &cmbom.nlenbf,
		      &cmbom.ndxhbf, &cmbom.ndx1bf, &cmbom.ndx2bf, 
		      nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Save current binary file list number. */
		cmbom.ibflc = ibfl;

		/* -- Return pointers to header and data. */
		*nlen = cmbom.nlenbf;
		*ndx1 = cmbom.ndx1bf;
		*ndx2 = cmbom.ndx2bf;
	}

	/* - If same file as last time, simply return the pointers. */
	else{
		*nlen = cmbom.nlenbf;
		*ndx1 = cmbom.ndx1bf;
		*ndx2 = cmbom.ndx2bf;
	}

L_8888:
	return;
} 

