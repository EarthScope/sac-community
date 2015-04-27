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
getfil(int idfl, int ldta, int *nlen, int *ndx1, int *ndx2, int *nerr) {

    UNUSED(idfl);
    UNUSED(ldta);
    UNUSED(nlen);

    UNUSED(ndx1);
    UNUSED(ndx2);
    UNUSED(nerr);
    return;

}
