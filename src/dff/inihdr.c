/** 
 * @file   inihdr.c
 * 
 * @brief  Initialize the Header Block
 * 
 */

#include <string.h>

#include "dff.h"
#include "hdr.h"
#include "bool.h"

struct t_cmhdr cmhdr;

/** 
 * Initialize the Header Block
 * 
 * @date   961212:  Added linc and llh as part off adding the INCLUSIVE 
 *             option to the LISTHDR command.  maf
 * @date   910820:  Added include file dfm and subscripted nlnhdr.
 * @date   810414:  Original version.
 *
 */
void
inihdr() {

    cmhdr.nvhdrc = SAC_HEADER_MAJOR_VERSION;

    /* lh starts without the INC option. */
    cmhdr.linc = FALSE;
    /* not currently executing xlh(). */
    cmhdr.llh = FALSE;

    return;
}

void
inihdr_() {
    inihdr();
}

void
inihdr__() {
    inihdr();
}
