/** 
 * @file   rdsdta.c
 * 
 * @brief  Read a SDD file
 * 
 */

#include "dff.h"
#include "hdr.h"
#include "co.h"
#include "ucf.h"
#include "dfm.h"
#include "amf.h"

#include "sddhdr.h"
#include "errors.h"

SDD_EXTERN

/** 
 * Read data components from a SDD disk file to memory
 * 
 * @param idfl 
 *    Data file list index number
 * @param nun 
 *    Fortran file unit on which data file is open
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   870515:  Fixed bug involving zero fill option.
 * @date   850415:  Changes due to restructuring of DFM common block.
 * @date   811202:  Added calculation of BEGIN and ENND for uneven data.
 * @date   811120:  Added calculation of ENND.
 * @date   810423:  Deleted option to convert format of spectral files
 *                  as they are read into memory.
 * @date   810416:  Replaced CMWORK with local storage.
 * @date   810120:  Changed to output message retrieval from disk.
 *
 */
void
rdsdta(int idfl, int *nun, int *nerr) {

    int i, jcomp, nlcdsk, numrd, offset;
    float unused;
    float *d;
    int *sdd;
    sac *s;
    kschan[12] = '\0';
    kschdr[80] = '\0';
    ksclas[4] = '\0';
    kscom[40] = '\0';
    ksevnm[8] = '\0';
    ksfrmt[8] = '\0';
    ksstnm[8] = '\0';

    if (!(s = sacget(idfl - 1, TRUE, nerr))) {
        goto L_8888;
    }

    *nerr = 0;
    offset = 0;

    /* - Define number of points to read and initial disk location. */
    numrd = s->m->nstop - s->m->nstart + 1 - s->m->nfillb - s->m->nfille;
    nlcdsk = MWSHDR;

    /* - For each data component: */
    for (jcomp = 1; jcomp <= sac_comps(s); jcomp++) {
        d = (jcomp == 1) ? s->y : s->x;
        /* -- Fill beginning with zeros if requested.  
         *    Update memory location. */
        if (s->m->nfillb > 0) {
            fill(d, s->m->nfillb, 0.);
            offset += s->m->nfillb;
        }

        /* -- Update disk location and read data. */
        if (numrd > 0) {
            nlcdsk = nlcdsk + s->m->nstart - 1 + s->m->nfillb;
            sdd = (int *) malloc(sizeof(int) * numrd);
            zrabs((int *) nun, (char *) sdd, numrd, (int *) &nlcdsk,
                  (int *) nerr);
            for (i = 0; i <= (numrd - 1); i++) {
                d[offset + i] = (float) (sdd[i]) / 100.0;
                //*(Sacmem++) = *(Isacmem++)/100.0;
            }
            FREE(sdd);
            offset += numrd;
        }

        /* -- Fill end with zeros if requested. */
        if (s->m->nfille > 0) {
            fill(d + offset, s->m->nfille, 0.);
            offset += s->m->nfille;
        }

        /* -- Update disk location to point to 
         *    start of next component. */
        nlcdsk = nlcdsk + s->m->ntotal - s->m->nstart + 1;
    }

    /* - Compute some header values. */
    extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax, &s->h->depmen);
    if (s->h->leven) {
        s->h->e = s->h->b + (float) (s->h->npts - 1) * s->h->delta;
    } else {
        extrma(s->x, 1, s->h->npts, &s->h->b, &s->h->e, &unused);
    }

  L_8888:
    return;
}
