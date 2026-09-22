
#include "ssi.h"
#include "hdr.h"
#include "dfm.h"
#include "amf.h"

#include "ucf.h"
#include "dff.h"
#include "dfm.h"

DFM_EXTERN

void
DBwfToSac(sac *s, struct trace *seis, int *nerr)
{
    /* Declare Variables. */
    int idx, jcomp, numrd;
    float *pArray;
    int ncomp;
    /*=====================================================================
     * PURPOSE:  Copy a waveform from SeisMgr to SAC
     *=====================================================================
     * INPUT ARGUMENTS:
     *    idfl   file number of first file being handled currently
     *    seis:  SeisMgr struct containing a seismogram.
     *=====================================================================
     * OUTPUT ARGUMENTS:
     *    nerr:    Error flag. Set to 0 if no error occurred.
     *=====================================================================
     * MODIFICATION HISTORY:
     *    971202:  Original version.  maf
     *===================================================================== */

    *nerr = 0;
    /* - Define number of points to read and initial disk location. */
    numrd = s->m->nstop - s->m->nstart + 1 - s->m->nfillb - s->m->nfille;

    ncomp = sac_comps(s);
    /* - For each data component: */
    for (jcomp = 0, pArray = seis->i; jcomp < ncomp; jcomp++, pArray = seis->r) {
        cut_data(pArray, s->m->nstart, s->m->nstop, s->m->nfillb, s->m->nfille, s->y);
        if (cmdfm.lscale && s->h->scale != SAC_FLOAT_UNDEFINED &&
            s->h->scale != 1.0) {
            for (idx = 0; idx < numrd; idx++) {
                s->y[idx] *= s->h->scale;
            }
            s->h->scale = 1.0;
        }
    }

    /* - Compute some header values. */
    extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax, &s->h->depmen);
    sac_be(s);

    return;

}                               /* end DBwfToSac */
