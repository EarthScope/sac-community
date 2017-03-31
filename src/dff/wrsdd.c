/** 
 * @file   wrsdd.c
 * 
 * @brief  Write a SDD File
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "sddhdr.h"
#include "hdr.h"
#include "co.h"
#include "dfm.h"
#include "amf.h"

#include "errors.h"

#include "ucf.h"


SDD_EXTERN

/** 
 * Write a SDD file from memory to file
 * 
 * @param idfl 
 *    Data file list index number
 * @param kname 
 *    Name of the file to write
 * @param kname_s 
 *    Length of \p kname
 * @param ldta 
 *    - TRUE to write the header and data
 *    - FALSE to write only the header, not data
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   910826:  Changed nint(x) to int(x + .5) to improve portability to
 *             DEC 5000 workstation per Gyu-sang Jang @ UC Davis.
 * @date   870730:  Added logic to check file permissions before writing.
 * @date   850731:  Changes due to new memory manager.
 * @date   840118:  Deleted call to ZTRUNC.
 * @date   800510:  Original version.
 *
 */
void
wrsdd(int idfl, char *kname, int kname_s, int ldta, int *nerr) {

    int idx, idd, ideg, ifrac, ihh, ijday, imm, imsec;
    int iss, itm, jcomp, ncerr, nlcdsk, nptwr;
    int nun;
    float frac;
    sac *s;
    int *sdd;
    kschan[12] = '\0';
    kschdr[80] = '\0';
    ksclas[4] = '\0';
    kscom[40] = '\0';
    ksevnm[8] = '\0';
    ksfrmt[8] = '\0';
    ksstnm[8] = '\0';

    *nerr = 0;

    /* - If header and data is to be written, a new file is created.
     *   If header only is to be written, the old file is opened. */
    if (ldta) {
        znfile(&nun, kname, kname_s, nerr);
        if (*nerr != 0)
            goto L_8888;
    } else {
        zopen_sac(&nun, kname, kname_s, "DATA", 5, nerr);
        if (*nerr != 0)
            goto L_8888;
    }
    if (!(s = sacget(idfl - 1, TRUE, nerr))) {
        goto L_8888;
    }

    /* - Initialize fields in SDD header */
    fstrncpy(kschdr, 80, " ", 1);

    for (idx = 21; idx <= MWSHDR; idx++) {
        Ishdr[idx] = 0;
    }

    /* - Convert header from SAC format to SDD format */

    if (!is_kundef2(s->h->kevnm)) {
        strscpy(ksevnm, s->h->kevnm, 8);
    }
    if (!is_kundef(s->h->kstnm)) {
        strcpy(ksstnm, s->h->kstnm);
    }
    if (!is_kundef(s->h->kcmpnm)) {
        char strtemp[9];
        strncpy(strtemp, s->h->kcmpnm, 8);
        strtemp[8] = '\0';
        subscpy(kschan, 0, 7, 12, strtemp);
    }
    if (!is_kundef(s->h->kinst)) {
        char strtemp[5];
        strncpy(strtemp, s->h->kinst, 4);
        strtemp[4] = '\0';
        subscpy(kschan, 8, 11, 12, strtemp);
    }
    if (s->h->delta != SAC_FLOAT_UNDEFINED)
        *isdelt = (int) ((1.0 / s->h->delta) * 100.0 + .5);
    *isnpts = s->h->npts;
    if (s->h->stel != SAC_FLOAT_UNDEFINED)
        *issel = (int) (s->h->stel * 100.0 + .5);
    if (s->h->stdp != SAC_FLOAT_UNDEFINED)
        *issdep = (int) (s->h->stdp * 100.0 + .5);

    /* - Pack date and time into one word */

    ijday = s->h->nzjday;
    itm =
        s->h->nzmsec + (((s->h->nzhour * 60 + s->h->nzmin) * 60) +
                        s->h->nzsec) * 1000;
    if (s->h->b != SAC_FLOAT_UNDEFINED && s->h->b != 0.0) {
        itm = itm + (int) (s->h->b * 1000.0 + .5);
        if (itm < 0) {
            ijday = ijday - 1;
            itm = itm + 8640000;
            if (ijday < 0) {
                *nerr = 1377;
                goto L_8888;
            }
        }
        imsec = itm % 1000;
        itm = itm / 1000;
        iss = itm % 60;
        itm = itm / 60;
        imm = itm % 60;
        ihh = itm / 60;
        *istime = imsec + ((ihh * 100 + imm) * 100 + iss) * 1000;
    } else {
        *istime =
            s->h->nzmsec + ((s->h->nzhour * 100 + s->h->nzmin) * 100 +
                            s->h->nzsec) * 1000;
    }
    kidate(s->h->nzyear, ijday, &imm, &idd, nerr);
    *isdate = idd + (s->h->nzyear * 100 + imm) * 100;

    /* - Convert lat/lon back from fraction to minutes/seconds */
    if (s->h->stla != SAC_FLOAT_UNDEFINED) {
        ideg = s->h->stla;
        frac = s->h->stla - (float) (ideg);
        imm = frac * 60.0;
        frac = frac * 60.0 - (float) (imm);
        iss = frac * 60.0;
        frac = frac * 60.0 - (float) (iss);
        ifrac = (int) (frac * 100.0 + .5);
        *issla = ifrac + ((ideg * 100 + imm) * 100 + iss) * 100;
    }

    if (s->h->stlo != SAC_FLOAT_UNDEFINED) {
        ideg = s->h->stlo;
        frac = s->h->stlo - (float) (ideg);
        imm = frac * 60.0;
        frac = frac * 60.0 - (float) (imm);
        iss = frac * 60.0;
        frac = frac * 60.0 - (float) (iss);
        ifrac = (int) (frac * 100.0 + .5);
        *isslo = ifrac + ((ideg * 100 + imm) * 100 + iss) * 100;
    }

    /* - Copy extra SDD information, if exists, to header */

    if (s->sddhdr) {
        *isclas = s->sddhdr[0];
        *isfrmt = s->sddhdr[1];
        *iscalg = s->sddhdr[2];

        for (idx = 1; idx <= MSCOM; idx++) {
            Iscom[idx] = s->sddhdr[3 + idx - 1];
        }

        for (idx = 1; idx <= MSREP; idx++) {
            Isrep[idx] = s->sddhdr[3 + MSCOM + idx - 1];
        }
    }

    /* - Write the header to disk. */

    nlcdsk = 0;
    nptwr = MWSHDR;

    zwabs((int *) &nun, (char *) (&Ishdr[1]), nptwr, (int *) &nlcdsk,
          (int *) nerr);

    /* - Write each data component, if requested. */

    if (ldta) {
        sdd = (int *) malloc(sizeof(int) * s->h->npts);
        for (jcomp = 0; jcomp < sac_comps(s); jcomp++) {
            nlcdsk = nlcdsk + nptwr;
            /*         Convert data to integers before writing */
            for (idx = 0; idx < s->h->npts; idx++) {
                sdd[idx] =
                    ((jcomp ==
                      0) ? roundf(s->y[idx] * 100.0) : roundf(s->x[idx] *
                                                              100.0));
            }

            zwabs((int *) &nun, (char *) sdd, s->h->npts, (int *) &nlcdsk,
                  (int *) nerr);
        }
    }

    /* - Close disk file. */

  L_8888:
    zclose(&nun, &ncerr);

    return;

}                               /* end of function */
