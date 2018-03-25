/** 
 * @file   rdxdrdta.c
 * 
 * @brief  Read data components from a XDR format SAC disk file
 * 
 */
#include <stdio.h>

#include "config.h"

#include "dff.h"
#include "amf.h"
#include "hdr.h"
#include "dfm.h"
#include "ucf.h"
#include "co.h"
#include "debug.h"

#ifdef HAVE_LIBRPC
#include <rpc/rpc.h>

/** 
 * Read data components from an XDR format SAC disk file to memory
 * 
 * @param idfl 
 *    Data file list index number
 * @param kname 
 *    Name of the file
 * @param kname_s 
 *    Length of \p kname
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date 010996:  Original version.
 *
 */
void
rdxdrdta(int idfl, char *kname, int kname_s, int *nerr) {

    int jcomp, nlcmem;
    int lendata, ncerr;
    float unused;
    FILE *nun;
    XDR xdrs;

    sac *s;
    *nerr = 0;

    if (!(s = sacnew())) {
        goto L_8888;
    }
    s->m->filename = strdup(kname);
    /* Open the input file */
    if((nun = fopen(kname, "rb")) == NULL) {
        *nerr = ERROR_OPENING_FILE;
        return;
    }

    /* Create a stream for the XDR decoding */
    xdrstdio_create(&xdrs, nun, XDR_DECODE);

    /* Read the header from disk */
    /* For portability, read and throw away the header first
       to correctly position file for read of data.   */
    xdrhdr(xdrs, s->h, nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Define number of points to read. */
    lendata = s->h->npts;

    sac_alloc(s);

    /* - For each data component: */
    for (jcomp = 0; jcomp < Ncomp[idfl]; jcomp++) {

        if (!xdr_array
            (&xdrs, (caddr_t *) (jcomp == 0) ? s->y : s->x, (u_int *) & lendata,
             (u_int) lendata, sizeof(float), xdr_float)) {
            *nerr = 123;
            goto L_8888;
        }
    }

    /* - Compute some header values. */

    sac_extrema(s);
    sac_be(s);

    sacput(s);

  L_8888:
    if (*nerr) {
        sac_free(s);
    }
    xdr_destroy(&xdrs);
    zcloses(&nun, &ncerr);

    return;

}

#else

void
rdxdrdta(int idfl, char *kname, int kname_s, int *nerr) {
    librpc_not_available();
    UNUSED(idfl);
    UNUSED(kname);
    UNUSED(kname_s);
    UNUSED(nerr);
}
#endif /* HAVE_LIBRPC */
