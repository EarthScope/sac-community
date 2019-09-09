/** 
 * @file   rdci.c
 * 
 * @brief  Read a SAC Card Image data file
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#include "dfm.h"
#include "hdr.h"
#include "bool.h"
#include "amf.h"
#include "co.h"

#include "errors.h"

#include "string_utils.h"

#include "msg.h"
#include "clf.h"
#include "dff.h"

#include "debug.h"
#include <fstr.h>

HDR_EXTERN

/** 
 * Read a SAC Card Image data file into memory
 * 
 * @param idfl 
 *    Data file list index number
 * @param kname 
 *    Name of the file to read
 * @param kname_s 
 *    Length of \p kname
 * @param nlen 
 *    Number of data points read
 * @param ndx1 
 *    Index in sacmem array of first data component
 * @param ndx2 
 *    Index of sacmem array of second data component
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_READING_CARD_IMAGE_HEADER
 *
 * @date   130105:  kevnm bug fix from Kasahara
 * @date   850730:  Changes due to new memory manager.
 *                  CHANGED NUMBER AND ORDER OF ARGUMENTS.
 * @date   830607:  Fixed bug causing extra data point to be read.
 * @date   810728:  Added argument specifying data format..
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   800109:  Original version.
 *
 */
void
rdci(int idfl, char *kname, int kname_s, int *nlen, int *ndx1, int *ndx2,
     int *nerr) {
    UNUSED(idfl);
    UNUSED(kname_s);
    UNUSED(nlen);
    UNUSED(ndx1);
    UNUSED(ndx2);

    sac *s = NULL;
    if((s = sac_read_alpha(kname, nerr))) {
        sacput(s);
    }
    return;
}
