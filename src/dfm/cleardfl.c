/** 
 * @file   cleardfl.c
 * 
 * @brief  Clear the data file list
 * 
 */

#include <string.h>

#include "clf.h"
#include "dfm.h"
#include "amf.h"
#include "debug.h"

#include "debug.h"

/** 
 * Clear pointers in the data set storage for current data sets
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   920406:  Update count of total files in SAC memory.
 * @date   911017:  Added reset to nflncds, number of files in the data-set.
 * @date   910827:  Adapted for multiple data-sets.
 * @date   910813:  Original version. Replaces part of the old cleardfl.
 *
 */
void
cleardfl(int *nerr) {

    UNUSED(nerr);

    return;
}

void
clear_file(int i, int *nerr) {
    UNUSED(i);
    UNUSED(nerr);
}
