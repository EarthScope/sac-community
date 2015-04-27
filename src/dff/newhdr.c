/** 
 * @file   newhdr.c
 * 
 * @brief  Prepare a new header
 * 
 */

#include <string.h>

#include "dff.h"
#include "msg.h"
#include "hdr.h"
#include "bool.h"
#include "amf.h"

/** 
 * Perpare a new default header
 * 
 * @date   961031:  ninf and nhst were changed to norid and nevid for
 *                  compatability with the CSS format.  maf 961031
 * @date   870902:  Added calls to INILHF and INIMSG as part of initialization.
 * @date   821001:  Added initialization of LCALDA.
 * @date   811118:  Replaced FMTSAC with literal "2."
 *                  Replaced DFM insert with HDR insert.
 *                  Deleted call to INIHDR.
 *
 */
void
newhdr() {
    sac *s;

    /* - Initialize some common blocks if not already done. */
    sacio_initialize_common();

    s = sac_new();
    s->m->filename = strdup("new_hdr_file.sac");
    sacput(s);

    return;

}

/* Wrapper to make the function more convenient for FORTRAN programmers. */
void
newhdr_() {
    newhdr();
}

void
newhdr__() {
    newhdr();
}
