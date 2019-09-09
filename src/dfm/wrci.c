/** 
 * @file   wrci.c
 * 
 * @brief  Write a SAC card image data file
 * 
 */

#include <stdio.h>

#include "dfm.h"
#include "hdr.h"
#include "bool.h"
#include "amf.h"
#include "defs.h"
#include "co.h"
#include "dff.h"
#include "errors.h"

/** 
 * Write a SAC card image data file
 * 
 * @param idfl 
 *    Data file list index number
 * @param kname 
 *    Name of file to write
 * @param kname_s 
 *    Length of \p kname
 * @param kfmt 
 *    Format for floating point numbers
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   130105:  kevnm bug fix from Kasahara
 * @date   830607:  Fixed bug causing extra data point to be written.
 * @date   810728:  Added argument specifying data format.
 * @date   800109:  Original version.
 *
 */
void
wrci(int idfl, char *kname, int kname_s, char *kfmt, int *nerr) {
    UNUSED(kname_s);
    UNUSED(kfmt);
    sac *s = NULL;
    *nerr = 0;
    if (!(s = sacget(idfl - 1, TRUE, nerr))) {
        return;
    }
    sac_write_alpha(s, kname, nerr);
}
