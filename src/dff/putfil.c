/** 
 * @file   putfil.c
 * 
 * @brief  Move current header to working memory
 * 
 */

#include "dff.h"
#include "msg.h"
#include "hdr.h"
#include "amf.h"
#include "dfm.h"
#include "co.h"
#include "ucf.h"

#include "errors.h"

/** 
 * Move current header from Header Block to working memory
 * 
 * @param idfl 
 *    Data file list index number to put
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER
 *
 * @bug Lots of copying done here.  Would it be easier to use a pointer
 *        and shift it to the correct position within the data file list.
 *        Or possibly use an object of a "SAC file + extra goodies"
 *
 * @date   990416:  Removed existing putfil.c and renamed mvhdr.c to putfil.c
 *                  This putfil used to be mvhdr.  There is now no mvhdr.c maf
 * @date   850415:  Changes due to restructuring of DFM common block.
 * @date   820917:  Modification due to change in HDR common blocks.
 *
 */
void 
putfil(int  idfl, 
       int *nerr) {

	int nlcmem;

	*nerr = 0;

	/* - Define memory location for copy. */
	nlcmem = Ndxhdr[idfl];

	/* - Move header.  COPY copies all non-character variables and
	 *   ZPUTC copies the character variables. */
	if( nlcmem > 0 ){
                /* Copy numeric values */
		/* copy( (int*)&Fhdr[1], (int*)cmmem.sacmem[nlcmem], SAC_HEADER_NUMBERS ); */
                copy_float( &(Fhdr[1]), cmmem.sacmem[nlcmem], SAC_HEADER_NUMBERS );
		/* Copy the character strings */
		zputc( kmhdr.khdr[0],9, 
		       (int *)(cmmem.sacmem[nlcmem] + SAC_HEADER_NUMBERS), 
		       (MCPW+1)* SAC_HEADER_STRINGS );
	}
	else{
		*nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
		setmsg( "ERROR", *nerr );
		apimsg( idfl );
	}

	return;
}

