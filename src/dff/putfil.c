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
putfil(int idfl, int *nerr) {

    UNUSED(idfl);
    UNUSED(nerr);
    return;
}
