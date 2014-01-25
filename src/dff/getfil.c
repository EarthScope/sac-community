/** 
 * @file   getfil.c
 * 
 * @brief  Get a data file from memory
 * 
 */

#include "dff.h"
#include "hdr.h"
#include "msg.h"
#include "dfm.h"
#include "co.h"
#include "amf.h"
#include "ucf.h"

#include "errors.h"

/** 
 * Get a data file from the memory manager
 * 
 * @param idfl 
 *    Data file list index number
 * @param ldta 
 *    - TRUE to get data and header
 *    - FALSE to only get the header
 * @param nlen 
 *    Length of data \p ndx1 and \p ndx2
 * @param ndx1 
 *    Index in sacmem array of first data component
 * @param ndx2 
 *    Index in sacmem array of second data component
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER
 *    - ERROR_ONLY_HEADERS_IN_MEMORY
 *
 * @date   900322:  Changed value of ndx2 from 0 to 1 when there is no second
 *                  data component. (VAX/VMS bug fix.)
 * @date   850801:  Changes in argument list for RDSAC.
 * @date   850415:  Changes due to restructuring of DFM common block.
 * @date   810923:  Added error return when data was requested
 *                  and only the headers were read.
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   790606:  Original version.
 *
 */
void 
getfil(int    idfl, 
       int    ldta, 
       int   *nlen, 
       int   *ndx1, 
       int   *ndx2, 
       int   *nerr) {

	int nlcmem;

	*nerr = 0;

	/* - If legitimate data file index number: */
	if( idfl > 0 && idfl <= cmdfm.ndfl ){

	  /* -- If in memory mode, move header to HDR common 
	   *    and get data indexes. */
		nlcmem = Ndxhdr[idfl];
		/* copy( (int*)cmmem.sacmem[nlcmem], (int*)&Fhdr[1], SAC_HEADER_NUMBERS ); */
		copy_float( cmmem.sacmem[nlcmem], &(Fhdr[1]), SAC_HEADER_NUMBERS );
		zgetc( (int *)cmmem.sacmem[nlcmem] + SAC_HEADER_NUMBERS, kmhdr.khdr[0], (MCPW+1)* SAC_HEADER_STRINGS );
		if( ldta ){
			*nlen = Nlndta[idfl];
			if( *nlen <= 0 ){
				*nerr = ERROR_ONLY_HEADERS_IN_MEMORY;
				setmsg( "ERROR", *nerr );
				*ndx1 = 0;
				*ndx2 = 0;
				goto L_8888;
			}
			*ndx1 = cmdfm.ndxdta[idfl - 1][0];
			if( Ncomp[idfl] > 1 ){
				*ndx2 = cmdfm.ndxdta[idfl - 1][1];
			}
			else{
				*ndx2 = 1;
			}
		}
		cmdfm.idflc = idfl;
	}
	else{
		*nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
		setmsg( "ERROR", *nerr );
		apimsg( idfl );
		*ndx1 = 0;
		*ndx2 = 0;
		*nlen = 0;
	}
L_8888:

	return;

}

