/** 
 * @file   rdshdr.c
 * 
 * @brief  Read a SDD Header
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "sddhdr.h"
#include "co.h"
#include "hdr.h"
#include "amf.h"
#include "dfm.h"
#include "ucf.h"
#include "errors.h"

struct t_kmshdr kmshdr;

int *const iscalg = (int *) (kmshdr.kshdr + 112);
int *const isclas = (int *) kmshdr.kshdr;
int *const iscom = (int *) (kmshdr.kshdr + 40);
int *const isdate = (int *) (kmshdr.kshdr + 80);
int *const isdelt = (int *) (kmshdr.kshdr + 88);
int *const isfrmt = (int *) (kmshdr.kshdr + 4);
int *const ishdr = (int *) kmshdr.kshdr;
int *const isnpts = (int *) (kmshdr.kshdr + 92);
int *const isrep = (int *) (kmshdr.kshdr + 116);
int *const issdep = (int *) (kmshdr.kshdr + 108);
int *const issel = (int *) (kmshdr.kshdr + 96);
int *const issla = (int *) (kmshdr.kshdr + 100);
int *const isslo = (int *) (kmshdr.kshdr + 104);
int *const istime = (int *) (kmshdr.kshdr + 84);
char *const kschan = (char *) (kmshdr.kshdr + 28);
char *const kschdr = (char *) kmshdr.kshdr;
char *const ksclas = (char *) kmshdr.kshdr;
char *const kscom = (char *) (kmshdr.kshdr + 40);
char *const ksevnm = (char *) (kmshdr.kshdr + 12);
char *const ksfrmt = (char *) (kmshdr.kshdr + 4);
char *const ksstnm = (char *) (kmshdr.kshdr + 20);
int *const Iscom = (int *) (kmshdr.kshdr + 40 - 4);
int *const Ishdr = (int *) (kmshdr.kshdr - 4);
int *const Isrep = (int *) (kmshdr.kshdr + 116 - 4);

/** 
 * Read a SDD Header from a currently open file into memory
 * 
 * @param idfl 
 *    Data file list index number
 * @param nun 
 *    Fortran file unit on which data file is open
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   900904:  Created to read SDD headers.
 *
 */
void
rdshdr(int idfl, int *nun, int *nerr) {

    int i, idd, imm, is, iss, nlcdsk, numrd;
    char strtemp[9];

    //int *Isacmem;
    sac *s;
    kschan[12] = '\0';
    kschdr[80] = '\0';
    ksclas[4] = '\0';
    kscom[40] = '\0';
    ksevnm[8] = '\0';
    ksfrmt[8] = '\0';
    ksstnm[8] = '\0';

    *nerr = 0;
    if (!(s = sacget(idfl - 1, TRUE, nerr))) {
        goto L_8888;
    }
    /* - Read header into SDD header common. */
    numrd = MWSHDR;
    nlcdsk = 0;
    zrabs((int *) nun, (char *) (&Ishdr[1]), numrd, (int *) &nlcdsk,
          (int *) nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Convert header from SDD format to SAC format */
    fstrncpy(s->h->kevnm, 17, ksevnm, strlen(ksevnm));
    ksstnm[8] = 0;
    strcpy(s->h->kstnm, ksstnm);

    strncpy(strtemp, kschan, 8);
    strtemp[8] = '\0';

    subscpy(s->h->kcmpnm, 0, 7, 8, strtemp);

    strcpy(s->h->kinst, "        ");

    strncpy(strtemp, kschan + 8, 4);
    strtemp[4] = '\0';

    subscpy(s->h->kinst, 0, 3, 8, strtemp);

    s->h->delta = (1.0 / (float) (*isdelt)) * 100.0;
    s->h->npts = *isnpts;
    s->h->stel = (float) (*issel) / 100.0;
    s->h->stdp = (float) (*issdep) / 100.0;

    /* - Convert lat/lon from dddmmsshh to ddd.frac */

    iss = *issla % 10000;
    is = *issla / 10000;
    imm = is % 100;
    is = is / 100;
    s->h->stla = (float) (is) + (float) (imm) / 60.0 + (float) (iss) / 360000.0;

    iss = *isslo % 10000;
    is = *isslo / 10000;
    imm = is % 100;
    is = is / 100;
    s->h->stlo = (float) (is) + (float) (imm) / 60.0 + (float) (iss) / 360000.0;

    /* - Convert time from yyyymmdd and hhmmssttt to separate fields */

    s->h->nzyear = *isdate / 10000;
    imm = *isdate / 100 - s->h->nzyear * 100;
    idd = *isdate % 100;
    kijdat(s->h->nzyear, imm, idd, &s->h->nzjday, nerr);

    s->h->nzhour = *istime / 10000000;
    s->h->nzmin = *istime / 100000 - s->h->nzhour * 100;
    s->h->nzsec = *istime / 1000 - (s->h->nzmin + s->h->nzhour * 100) * 100;
    s->h->nzmsec = *istime % 1000;

    /* - Allocate memory for leftover stuff from the SDD header
     *   that doesn't go anywhere else */

    numrd = MWESHD;

    s->sddhdr = (int *) malloc(sizeof(int) * MWESHD);
    //nlcmem = Nxsdd[idfl];

    s->sddhdr[0] = *isclas;
    s->sddhdr[1] = *isfrmt;
    s->sddhdr[2] = *iscalg;
    //*Isacmem = *isclas;
    //*(Isacmem + 1) = *isfrmt;
    //*(Isacmem + 2) = *iscalg;
    //offset = 3;

    //Isacmem += offset;

    for (i = 1; i <= MSCOM; i++) {
        s->sddhdr[3 + i - 1] = Iscom[i];
        //*(Isacmem++) = Iscom[i];
        //offset++;
    }

    for (i = 1; i <= MSREP; i++) {
        s->sddhdr[3 + MSCOM + i - 1] = Isrep[i];
        //*(Isacmem++) = Isrep[i];
        //offset++;
    }

    /* - Compute distance, azimuth, etc. if proper header fields are present. */

    update_distaz(s);

    if (s->h->nzyear >= 0 && s->h->nzyear <= 99)
        s->h->nzyear = s->h->nzyear + 1900;

    s->h->b = 0.0;
    sac_be(s);

  L_8888:
    return;

}
