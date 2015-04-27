/** 
 * @file   relbfl.c
 * 
 * @brief  Release the Current file in Binary Operations
 * 
 */

#include "amf.h"
#include "bom.h"
#include "dfm.h"

/** 
 * Release the current Binary Operations file from memory
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 * 
 * @date   880308:  Was not clearing the DFM array variables properly.
 * @date   880306:  Fixed bug that was not releasing header block.
 * @date   850730:  Original version.
 *
 */
void
relbfl(int *nerr) {

    *nerr = 0;

    return;
    /* - Release blocks if they are in memory: */
    /* -- Header block. */
    /* if( cmbom.ndxhbf > 0 ) */
    /* if( *nerr != 0 ) */
    /*      goto L_8888; */

    /* /\* -- First data component. *\/ */
    /* if( cmbom.ndx1bf > 0 ) */
    /* if( *nerr != 0 ) */
    /*      goto L_8888; */

    /* /\* -- Second data component if any. *\/ */
    /* if( cmbom.ndx2bf > 0 ) */
    /* if( *nerr != 0 ) */
    /*      goto L_8888; */

    /* /\* - Zero DFM pointers. (Binary file is stored at end of data file list.) *\/ */

    /* Ndxhdr[saclen() + 1] = 0; */
    /* Nlndta[saclen() + 1] = 0; */
    /* for( jcomp = 1; jcomp <= Ncomp[saclen() + 1]; jcomp++ ){ */
    /*      jcomp_ = jcomp - 1; */
    /*      cmdfm.ndxdta[saclen()][jcomp_] = 0; */
    /* } */
    /* Ncomp[saclen() + 1] = 0; */

    /* /\* - Zero BOM pointers. *\/ */

    /* cmbom.ibflc = 0; */
    /* cmbom.ndxhbf = 0; */
    /* cmbom.ndx1bf = 0; */
    /* cmbom.ndx2bf = 0; */

    return;

}
