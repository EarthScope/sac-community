/** 
 * @file   rddta.c
 * 
 * @brief  Read SAC data components
 * 
 */

#include "dff.h"
#include "amf.h"
#include "hdr.h"
#include "dfm.h"
#include "ucf.h"
#include "co.h"

#include "debug.h"
/** 
 * Read data components frm a SAC disk file to memory
 * 
 * @param idfl 
 *    Data file list index number
 * @param nun 
 *    Fortran file unit on which data file is open
 * @param lswap 
 *    To byte swap the incoming data
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   870515:  Fixed bug involving zero fill option.
 * @date   850415:  Changes due to restructuring of DFM common block.
 * @date   811202:  Added calculation of BEGIN and ENND for uneven data.
 * @date   811120:  Added calculation of ENND.
 * @date   810423:  Deleted option to convert format of spectral files
 *             as they are read into memory.
 * @date   810416:  Replaced CMWORK with local storage.
 * @date   810120:  Changed to output message retrieval from disk.
 *
 */
void
rddta(sac * s, int *nun, int lswap, int *nerr) {

    int jcomp, nlcdsk, numrd, offset;
    float unused;
    float *d;
    *nerr = 0;

    /* - Define number of points to read and initial disk location. */

    numrd = s->m->nstop - s->m->nstart + 1 - s->m->nfillb - s->m->nfille;
    nlcdsk = SAC_HEADER_WORDS_FILE;
    /* - For each data component: */
    for (jcomp = 0; jcomp < sac_comps(s); jcomp++) {
        offset = 0;
        d = (jcomp == 0) ? s->y : s->x;
        DEBUG("comp: %d/%d\n", jcomp, sac_comps(s));
        DEBUG("npts: %d [%d]\n", s->h->npts, numrd);

        /* -- Fill beginning with zeros if requested.  
         *    Update memory location. */
        if (s->m->nfillb > 0) {
            fill(d, s->m->nfillb, 0.);
            offset += s->m->nfillb;
        }

        /* -- Update disk location and read data. */
        if (numrd > 0) {
            nlcdsk = nlcdsk + s->m->nstart - 1 + s->m->nfillb;
            zrabs((int *) nun, (char *) (d + offset), numrd, (int *) &nlcdsk,
                  (int *) nerr);
            if (lswap) {        /* byteswap if necessary. */
                int idx;
                float *ptr;

                for (idx = 0, ptr = d + offset; idx < numrd; idx++, ptr++) {
                    byteswap((void *) ptr, 4);
                }
            }

            if (*nerr != 0)
                goto L_8888;
            offset += numrd;
        }

        /* -- Fill end with zeros if requested. */
        if (s->m->nfille > 0) {
            fill(d + offset, s->m->nfille, 0.);
        }

        /* -- Update disk location to point to start of next component. */
        nlcdsk = nlcdsk + s->m->ntotal - s->m->nstart + 1;
    }                           /* end for ( jcomp ) */

    /* - Compute some header values. */

    //    s->h->npts = s->m->nlndta;
    extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax, &s->h->depmen);
    sac_be(s);

  L_8888:
    return;
}
