/** 
 * @file   rdci.c
 * 
 * @brief  Read a SAC Card Image data file
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#include "dfm.h"
#include "hdr.h"
#include "bool.h"
#include "amf.h"
#include "co.h"

#include "errors.h"

#include "string_utils.h"

#include "msg.h"
#include "clf.h"
#include "dff.h"

#include "debug.h"

HDR_EXTERN

/** 
 * Read a SAC Card Image data file into memory
 * 
 * @param idfl 
 *    Data file list index number
 * @param kname 
 *    Name of the file to read
 * @param kname_s 
 *    Length of \p kname
 * @param nlen 
 *    Number of data points read
 * @param ndx1 
 *    Index in sacmem array of first data component
 * @param ndx2 
 *    Index of sacmem array of second data component
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_READING_CARD_IMAGE_HEADER
 *
 * @date   130105:  kevnm bug fix from Kasahara
 * @date   850730:  Changes due to new memory manager.
 *                  CHANGED NUMBER AND ORDER OF ARGUMENTS.
 * @date   830607:  Fixed bug causing extra data point to be read.
 * @date   810728:  Added argument specifying data format..
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   800109:  Original version.
 *
 */
void
rdci(int idfl, char *kname, int kname_s, int *nlen, int *ndx1, int *ndx2,
     int *nerr) {

    int ilhdr[SAC_HEADER_LOGICALS], jdx, jj, jjj, ncards, nremdr;
    FILE *nun;
    char kiline[MCMSG + 1];
    char *kiptr;
    int k;
    sac *s;
    float *fp;
    int *ip;
    int *const Ilhdr = &ilhdr[0] - 1;
    UNUSED(ndx1);
    UNUSED(ndx2);
    UNUSED(nlen);
    UNUSED(idfl);
    *nerr = 0;
    s = sac_new();

    /* - Open file. */
    zopens(&nun, kname, kname_s, nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Read header. */
    jdx = 1;
    for (jj = 1; jj <= (SAC_HEADER_FLOATS / 5); jj++) {
        if (fgetsp(kiline, MCMSG, nun) == NULL) {
            *nerr = ERROR_READING_CARD_IMAGE_HEADER;
            goto L_8888;
        }
        for (jjj = jdx; jjj <= (jdx + 4); jjj++) {
            kiptr = strtok(jjj == jdx ? kiline : NULL, " ");
            if (!kiptr)
                break;
            fp = fhdr(s, jjj);
            VALUE(fp) = atof(kiptr);
        }
        jdx = jdx + 5;
    }

    jdx = 1;

    for (jj = 1; jj <= (SAC_HEADER_INTEGERS / 5); jj++) {
        if (fgetsp(kiline, MCMSG, nun) == NULL) {
            *nerr = ERROR_READING_CARD_IMAGE_HEADER;
            goto L_8888;
        }
        for (jjj = jdx; jjj <= (jdx + 4); jjj++) {
            kiptr = strtok(jjj == jdx ? kiline : NULL, " ");
            if (!kiptr)
                break;
            ip = nhdr(s, jjj);
            VALUE(ip) = atol(kiptr);
        }
        jdx = jdx + 5;
    }

    jdx = 1;
    for (jj = 1; jj <= (SAC_HEADER_ENUMS / 5); jj++) {
        if (fgetsp(kiline, MCMSG, nun) == NULL) {
            *nerr = ERROR_READING_CARD_IMAGE_HEADER;
            goto L_8888;
        }
        for (jjj = jdx; jjj <= (jdx + 4); jjj++) {
            kiptr = strtok(jjj == jdx ? kiline : NULL, " ");
            if (!kiptr)
                break;
            ip = ihdr(s, jjj);
            VALUE(ip) = atol(kiptr);
        }
        jdx = jdx + 5;
    }

    jdx = 1;
    for (jj = 1; jj <= (SAC_HEADER_LOGICALS / 5); jj++) {
        if (fgetsp(kiline, MCMSG, nun) == NULL) {
            *nerr = ERROR_READING_CARD_IMAGE_HEADER;
            goto L_8888;
        }

        for (jjj = 1; jjj <= 5; jjj++) {
            kiptr = strtok(jjj == jdx ? kiline : NULL, " ");
            if (!kiptr)
                break;
            Ilhdr[jjj] = atol(kiptr);
        }

        for (jjj = 1; jjj <= 5; jjj++) {
            ip = lhdr(s, jjj);
            if (Ilhdr[jjj] == 1) {
                VALUE(ip) = TRUE;
            } else {
                VALUE(ip) = FALSE;
            }
            jdx = jdx + 1;
        }
    }

    /* read the character header values */
    if (fgetsp(kiline, MCMSG, nun) == NULL) {
        *nerr = ERROR_READING_CARD_IMAGE_HEADER;
        goto L_8888;
    }

    strncpy(khdr(s, 1), kiline, 8);
    khdr(s, 1)[8] = '\0';
    strncpy(khdr(s, 2), kiline + 8, 16);
    khdr(s, 2)[16] = '\0';
    DEBUG("in: <%s>\n", kiline);
    DEBUG("kstnm: <%s>\n", s->h->kstnm);
    DEBUG("kevnm: <%s>\n", s->h->kevnm);
    for (jj = 4; jj <= SAC_HEADER_STRINGS; jj += 3) {
        if (fgetsp(kiline, MCMSG, nun) == NULL) {
            *nerr = ERROR_READING_CARD_IMAGE_HEADER;
            goto L_8888;
        }
        for (jjj = jj; jjj <= (jj + 2); jjj++) {
            strncpy(khdr(s, jjj), kiline + ((jjj - jj) * 8), 8);
            khdr(s, jjj)[8] = '\0';
        }
    }

    /* - Update the header if necessary. */

    if (s->h->nvhdr < cmhdr.nvhdrc) {
        updhdr(nerr);
        if (*nerr != 0)
            goto L_8888;
    }

    /* - Make sure the most important header values are defined. */

    if (s->h->npts == SAC_INT_UNDEFINED || s->h->b == SAC_FLOAT_UNDEFINED) {
        *nerr = ERROR_READING_CARD_IMAGE_HEADER;
        setmsg("ERROR", *nerr);
        goto L_8888;
    }

    /* - Move header to SACMEM array. */

    /* - Set up data space. */
    s->m->filename = fstrdup(kname, kname_s);

    if (*nerr != 0)
        goto L_8888;

    /* - Read first data component. */
    sac_alloc(s);
    ncards = s->h->npts / 5;
    nremdr = s->h->npts - 5 * ncards;
    k = 0;
    for (jj = 1; jj <= ncards; jj++) {
        if (fgetsp(kiline, MCMSG, nun) == NULL) {
            *nerr = ERROR_READING_CARD_IMAGE_HEADER;
            goto L_8888;
        }
        for (jjj = 0; jjj < 5; jjj++) {
            kiptr = strtok(jjj == 0 ? kiline : NULL, " ");
            if (!kiptr)
                break;
            s->y[k++] = atof(kiptr);
        }
    }
    if (nremdr > 0) {
        if (fgetsp(kiline, MCMSG, nun) == NULL) {
            *nerr = ERROR_READING_CARD_IMAGE_HEADER;
            goto L_8888;
        }
        for (jjj = 0; jjj < nremdr; jjj++) {
            kiptr = strtok(jjj == 0 ? kiline : NULL, " ");
            if (!kiptr)
                break;
            s->y[k++] = atof(kiptr);
        }
    }

    /* - Read second data component if present. */

    k = 0;
    if (sac_comps(s) == 2) {
        for (jj = 1; jj <= ncards; jj++) {
            if (fgetsp(kiline, MCMSG, nun) == NULL) {
                *nerr = ERROR_READING_CARD_IMAGE_HEADER;
                goto L_8888;
            }
            for (jjj = 0; jjj < 5; jjj++) {
                kiptr = strtok(jjj == 0 ? kiline : NULL, " ");
                if (!kiptr)
                    break;
                s->x[k++] = atof(kiptr);
            }
        }
        if (nremdr > 0) {
            if (fgetsp(kiline, MCMSG, nun) == NULL) {
                *nerr = ERROR_READING_CARD_IMAGE_HEADER;
                goto L_8888;
            }
            for (jjj = 0; jjj < nremdr; jjj++) {
                kiptr = strtok(jjj == 0 ? kiline : NULL, " ");
                if (!kiptr)
                    break;
                s->x[k++] = atof(kiptr);
            }
        }
    }

    sacput(s);

    /* - Close file and return. */
    zcloses(&nun, nerr);

  L_8888:
    if (*nerr) {
        sac_free(s);
    }
    return;
}
