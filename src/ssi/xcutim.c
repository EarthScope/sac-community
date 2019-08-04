
#include <string.h>
#include <ctype.h>

#include <fern/array.h>

#include "ssi.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "sacIOdb/sacIOdb.h"
#include "smDataIO.h"
#include "cssListOps/cssListOps.h"
#include "sm_gc.h"

#include "msg.h"
#include "bot.h"
#include "cpf.h"
#include "dff.h"
#include "debug.h"
#include "defs.h"

#define MAXPAIRS 10

DFM_EXTERN

#define CUT_NOW  TRUE

sac *cut_file;
void sacpop_no_free();

char *
generate_filename(sac *s) {
    char *filename;
    char tempSta[32], tempChan[32];
    if(SAC_CHAR_DEFINED(s->h->kstnm)) {
        strcpy(tempSta, s->h->kstnm);
    } else {
        sprintf(tempSta, "s%03d", saclen());
    }
    if(SAC_CHAR_DEFINED(s->h->kcmpnm)) {
        strcpy(tempChan, s->h->kcmpnm);
    } else {
        sprintf(tempChan, "c%03d", saclen());
    }
    rstrip(tempSta);
    rstrip(tempChan);
    asprintf(&filename, "%s.%s.%04d%03d%02d%02d%02d", tempSta, tempChan,
             s->h->nzyear, s->h->nzjday, s->h->nzhour, s->h->nzmin,
             s->h->nzsec);
    return filename;
}


void
xcutim(int *nerr) {
    /* declare variables */
    char kcutSave[2][9];
    double dtmp[2];
    int idx, jdx, nBounds;
    sac *s;
    int i;
    struct SACheader **header;
    const int charsInBase = 9;

    int lnotused, lcutSave;

    float ocutSave[2];

    sac **cut_data_im = NULL;

    struct cutPair {
        char kbase[2][9];
        float offset[2];
    } bounds[MAXPAIRS];

    header = xarray_new('p');
    /* save current global values */
    lcutSave = cmdfm.lcut;
    ocutSave[0] = cmdfm.ocut[0];
    ocutSave[1] = cmdfm.ocut[1];
    strcpy(kcutSave[0], kmdfm.kcut[0]);
    strcpy(kcutSave[1], kmdfm.kcut[1]);

    /* PARSING PHASE */
    /* -- "COMMIT|RECALLTRACE|ROLLBACK":
       how to treat existing data */
    if (lckeyExact("COMMIT", 7))
        cmdfm.icomORroll = COMMIT;
    else if (lckeyExact("RECALLTRACE", 12))
        cmdfm.icomORroll = RECALL;
    else if (lckeyExact("RECALL", 7))
        cmdfm.icomORroll = RECALL;
    else if (lckeyExact("ROLLBACK", 9))
        cmdfm.icomORroll = ROLLBACK;

    /* Get the limits of the cut. */
    for (nBounds = 0; nBounds < MAXPAIRS; nBounds++) {
        if (!lcrtw(&lnotused, (bounds[nBounds].kbase[0]), charsInBase, dtmp)) {
            break;
        } else {
            bounds[nBounds].offset[0] = (float) dtmp[0];
            bounds[nBounds].offset[1] = (float) dtmp[1];
        }

        if (!strcmp(bounds[nBounds].kbase[0], "A       ") &&
            !strcmp(bounds[nBounds].kbase[1], "F       ")) {
            break;
        }
    }                           /* end for */

    /* CHECKING PHASE */

    /* - Check for null data file list. */
    vflist(nerr);
    if (*nerr != 0)
        return;

    /* - Check to make sure all files are evenly spaced time series files. */
    vfeven(nerr);
    if (*nerr != 0)
        return;

    /* Check that cut point information is of the proper form. */
    for (idx = 0; idx < nBounds; idx++) {
        for (jdx = 0; jdx < 2; jdx++) {
            char *test = &(bounds[idx].kbase[jdx][0]);

            if (*test != 'A' && /* first arrival */
                *test != 'B' && /* begin time of trace */
                *test != 'E' && /* end time of trace */
                *test != 'F' && /* end of event */
                *test != 'O' && /* origin */
                *test != 'T' && /* zero seconds */
                *test != 'Z') {
                *nerr = 1405;
                return;
            }
            if (*test == 'T') {
                if (!isdigit(*(test + 1))) {
                    *nerr = 1405;
                    return;
                }
                if (strcmp(test + 2, "      ")) {
                    *nerr = 1405;
                    return;
                }
            } /* end if ( *test == 'T' ) */
            else {
                if (strcmp(test + 1, "       ")) {
                    *nerr = 1405;
                    return;
                }
            }
        }
    }                           /* end for ( idx ) */

    /* Move files from sac memory into local array */
    cut_data_im = xarray_new('p');
    for (i = 0; i < saclen(); i++) {
        if (!(s = sacget(i, TRUE, nerr))) {
            *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
            goto L_ERROR;
        }
        cut_data_im = xarray_append(cut_data_im, s);
    }
    /* Clear List but do not free memory */
    while (saclen() > 0) {
        sacpop_no_free();
    }

    /* set logical cut to TRUE */
    cmdfm.lcut = TRUE;

    /* Loop between pairs of cut points */
    for (idx = 0; idx < nBounds; idx++) {
        /* reset cut parameters. */
        strcpy(kmdfm.kcut[0], bounds[idx].kbase[0]);
        strcpy(kmdfm.kcut[1], bounds[idx].kbase[1]);
        cmdfm.ocut[0] = bounds[idx].offset[0];
        cmdfm.ocut[1] = bounds[idx].offset[1];
        DEBUG("ocut[0,1] %f %f\n", cmdfm.ocut[0], cmdfm.ocut[1]);
        for(size_t i = 0; i < xarray_length(cut_data_im); i++) {
            DEBUG("FILE: %d/%d\n", i+1, xarray_length(cut_data_im));
            sac *s = sac_new();
            sac *old = cut_data_im[i];
            // Copy Header and Meta to New Data :: old => s
            sac_header_copy(s, old);
            sac_meta_copy(s, old);
            sacput(s);

            if(idx > 0) { /* Create new file name for multiple files in cut */
                FREE(s->m->filename);
                s->m->filename = generate_filename(s);
            }
            // Define Memory and Allocate space
            defmem(saclen(), CUT_NOW, nerr);
            sac_alloc(s);
            int numrd = s->m->nstop - s->m->nstart + 1 - s->m->nfillb - s->m->nfille;
            /* - Cut data for each data component: */
            for (int j = 0; j < sac_comps(s); j++) {
                DEBUG("COMPS: %d/%d\n", j+1, sac_comps(s));
                float *oldy = (j == 0) ? old->y : old->x;
                float *newy = (j == 0) ? s->y   : s->x;
                cut_data(oldy, s->m->nstart, s->m->nstop, s->m->nfillb, s->m->nfille, newy);
            }
            // Rescale data
            if (cmdfm.lscale && s->h->scale != SAC_FLOAT_UNDEFINED && s->h->scale != 1.0) {
                float *newy = s->y;
                for (int k = 0; k < numrd; k++) {
                    newy[k] *= s->h->scale;
                }
                s->h->scale = 1.0;
            }
            /* - Compute some header values. */
            sac_extrema(s);
            sac_be(s);
        }
    }                           /* End loop between pairs of cut points. */


  L_ERROR:
    cmdfm.lcut = lcutSave;
    cmdfm.ocut[0] = ocutSave[0];
    cmdfm.ocut[1] = ocutSave[1];
    strcpy(kmdfm.kcut[0], kcutSave[0]);
    strcpy(kmdfm.kcut[1], kcutSave[1]);
    for (i = 0; i < (int) xarray_length(header); i++) {
        free(header[i]);
        header[i] = NULL;
    }
    xarray_free(header);

}                               /* end xcutim() */
