/** 
 * @file   defmem.c
 * 
 * @brief  Define memory requirement for a file
 * 
 */

#include <string.h>

#include "amf.h"
#include "dfm.h"
#include "bool.h"
#include "hdr.h"
#include "SacHeader.h"
#include "errors.h"

#include "msg.h"
#include "clf.h"

DFM_EXTERN

/** 
 * Define memory requirement for a given data fiile
 * 
 * @param idfl 
 *    Data file list number
 * @param lcutnow 
 *    - TRUE if the file is being read and needs to be cut
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   850415:  Changes due to restructuring of DFM common block.
 * @date   840228:  Moved cut logic to new subroutine, DEFCUT.
 * @date   820809:  Changed method of storing cut parameters.
 * @date   810903:  Fixed bug in computing stop time when "N" option used.
 *
 */
void
defmem(int idfl, int lcutnow, int *nerr) {

    int lall;
    char *tmp;
    sac *s;
    *nerr = 0;
    if (!(s = sacget(idfl - 1, FALSE, nerr))) {
        goto L_8888;
    }
    /* - Get name of data file. */
    tmp = s->m->filename;
    /* - Entire file is read if:
     *   (1) cut option is off.
     *   (2) file is a spectral file
     *   (3) file is unevenly spaced */

    if (!cmdfm.lcut || !lcutnow) {
        lall = TRUE;
    } else if (s->h->iftype == IRLIM || s->h->iftype == IAMPH) {
        setmsg("WARNING", ERROR_CANT_CUT_SPECTRAL_FILE);
        apcmsg2(tmp, strlen(tmp) + 1);
        outmsg();
        lall = TRUE;
    } else if (!s->h->leven) {
        setmsg("WARNING", ERROR_CANT_CUT_UNEVENLY_SPACED_FILE);
        apcmsg2(tmp, strlen(tmp) + 1);
        outmsg();
        lall = TRUE;
    } else {
        lall = FALSE;
    }

    /* - Set parameters if entire file is to be read. */
    if (lall) {
        s->m->nstart = 1;
        s->m->nstop = s->h->npts;
        s->m->ntotal = s->h->npts;
        s->m->nfillb = 0;
        s->m->nfille = 0;
    }

    /* - Set parameters for partial read. */
    else {
        defcut(kmdfm.kcut, cmdfm.ocut, idfl, nerr);
        if (*nerr != 0)
            goto L_8888;
    }

    s->h->npts = s->m->nstop - s->m->nstart + 1;

  L_8888:
    return;
}
