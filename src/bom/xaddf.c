/** 
 * @file   xaddf.c
 * 
 * @brief  Execute ADDF
 * 
 */

#include <string.h>
#include <math.h>

#include "amf.h"
#include "mach.h"
#include "hdr.h"
#include "dfm.h"
#include "bom.h"
#include "bool.h"
#include "cpf.h"
#include "dff.h"
#include "co.h"
#include "msg.h"
#include "ucf.h"
#include "errors.h"
#include "debug.h"
#include "clf.h"

BOM_EXTERN

int
vbeven(sac * s) {
    int nerr = 0;
    if (s->h->iftype == IRLIM || s->h->iftype == IAMPH) {
        nerr = ERROR_OPERATION_ON_SPECTRAL_FILE;
    } else if (!s->h->leven) {
        nerr = ERROR_OPERATION_ON_UNEVEN_FILE;
    } else if (s->h->iftype == IXYZ) {
        nerr = ERROR_OPERATION_ON_XYZ_FILE;
    }
    return nerr;
}

int
isFatal(char *key) {
    return (strcmp(key, "FATAL   ") == 0);
}

int
isWarning(char *key) {
    return (strcmp(key, "WARNING ") == 0);
}

int
isIgnore(char *key) {
    return (strcmp(key, "IGNORE  ") == 0);
}

int
delta_equal(sac * s1, sac * s2) {
    int fatal, nerr;
    float value;

    if (isIgnore(kmbom.kecdel)) {
        return 0;
    }
    value = (DT(s2) - DT(s1)) / DT(s2);
    if (!linrng(value, -RNDOFF, RNDOFF)) {
        fatal = isFatal(kmbom.kecdel);
        nerr = ERROR_HEADER_FILE_MISMATCH;
        message((fatal) ? MERRORS : MWARNINGS, nerr, "DELTA %s %s",
                s1->m->filename, s2->m->filename);
        if (fatal) {
            return nerr;
        }
        outmsg();
        clrmsg();
    }
    return 0;
}

int
station_equal(sac * s1, sac * s2) {
    int nerr;
    if (memcmp(s1->h->kstnm, s2->h->kstnm, strlen(s1->h->kstnm)) != 0) {
        nerr = 1801;
        error(nerr, "KSTNM %s %s", s1->m->filename, s2->m->filename);
        return nerr;
    }
    return 0;
}

int
npts_equal(sac * s1, sac * s2) {
    int fatal, nerr;

    if (isIgnore(kmbom.kecnpt)) {
        return 0;
    }

    if (s1->h->npts != s2->h->npts) {
        fatal = isFatal(kmbom.kecnpt);
        nerr = ERROR_HEADER_FILE_MISMATCH;
        message((fatal) ? MERRORS : MWARNINGS, nerr, "NPTS %s %s",
                s1->m->filename, s2->m->filename);
        if (fatal) {
            return nerr;
        }
        outmsg();
        clrmsg();
    }
    return 0;
}

int
time_equal(sac * s1, sac * s2) {
    int err;
    int timeb1[6], timeb2[6];
    char t1[33], t2[33];
    double diff;

    if (ldttm(&s1->h->nzyear) && ldttm(&s2->h->nzyear)) {
        idttm(&s1->h->nzyear, B(s1), timeb1);
        idttm(&s2->h->nzyear, B(s2), timeb2);
        ddttm(timeb1, timeb2, &diff);
        if (fabs(diff) > RNDOFF) {
            kadttm(timeb1, t1, 33, &err);
            kadttm(timeb2, t2, 33, &err);
            warning(1802, "\n   BEG1: %s %s\n   BEG2: %s %s", t1,
                    s1->m->filename, t2, s2->m->filename);
            outmsg();
            clrmsg();
        }
    }
    return 0;
}

int
files_similar(string_list * list) {
    sac *s1, *s2;
    int jdfl, jbfl, nbfl, nerr;

    nbfl = string_list_length(list);
    /* - Make sure each file in BFL are of the proper type. */
    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        if (!(s1 = sacget(jdfl - 1, TRUE, &nerr))) {
            DEBUG("get: %d/%d\n", jdfl - 1, saclen());
            return -1;
        }
        jbfl = min(jdfl - 1, nbfl - 1);
        if (!(s2 = bflget(list, jbfl))) {
            DEBUG("BOM get: %d/%d\n", jdfl - 1, string_list_length(list));
            return 1301;
        }
        //printf("npts: %d %d delta: %f %f\n", s1->h->npts, s2->h->npts, s1->h->delta, s2->h->delta);
        if ((nerr = vbeven(s2)) != 0) {
            error(nerr, "%s", string_list_get(list, jbfl - 1));
            return nerr;
        }
        if ((nerr = delta_equal(s1, s2)) != 0) {
            return nerr;
        }
        if ((nerr = npts_equal(s1, s2)) != 0) {
            return nerr;
        }
        if ((nerr = time_equal(s1, s2)) != 0) {
            return nerr;
        }
    }
    return 0;
}

void
xbom_op(char op, int *nerr) {
    int j, jdfl;
    int lnewhdr;                /* let header data come from new file */
    int n;
    string_list *list;
    sac *s1, *s2;
    int nbfl;
    *nerr = 0;
    list = NULL;

    /* - Loop on each token in command: */
    while (lcmore(nerr)) {
        /* -- NEWHDR:  take the header from the new file being merged in. */
        if (lklog("NEWHDR", 7, &lnewhdr)) {
            cmbom.lnewhdr = lnewhdr;
        }

        /* -- "filelist':  define a new binop filelist. */
        if ((list = lcdfl_wild())) {
            cmbom.ibflc = 0;
        }

        /* -- Bad syntax. */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    /* - The above loop is over when one of two conditions has been met:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
     *   (2) All the tokens in the command have been successfully parsed. */
    if (*nerr != 0)
        goto L_8888;

    /* CHECKING PHASE: */
    /* - Check for null data file list. */
    vflist(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Check to make sure all files are evenly 
       spaced time series files. */
    vfeven(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Check for a null binop file list. */
    nbfl = string_list_length(list);
    if (!list || nbfl <= 0) {
        *nerr = ERROR_BINOP_FILE_LIST_EMPTY;
        error(*nerr, "");
        goto L_8888;
    }

    if ((*nerr = files_similar(list)) != 0) {
        goto L_8888;
    }

    /* EXECUTION PHASE: */

    /* - Perform the file addition on each file in DFL. */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {

        /* -- Get the next file in DFL, moving header to CMHDR. */
        if (!(s1 = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }

        /* -- Get the next file in the BFL, moving header to CMHDR. */
        if (!(s2 = bflget(list, min(jdfl - 1, string_list_length(list) - 1)))) {
            goto L_8888;
        }
        n = min(s1->h->npts, s2->h->npts);
        switch (op) {
            case '+':
                for (j = 0; j < n; j++) {
                    s1->y[j] += s2->y[j];
                }
                break;
            case '-':
                for (j = 0; j < n; j++) {
                    s1->y[j] -= s2->y[j];
                }
                break;
            case '*':
                for (j = 0; j < n; j++) {
                    s1->y[j] *= s2->y[j];
                }
                break;
            case '/':
                for (j = 0; j < n; j++) {
                    if (fabs(s2->y[j]) <= VSMALL) {
                        s1->y[j] = sign(VLARGE, s1->y[j] * s2->y[j]);
                    } else {
                        s1->y[j] = s1->y[j] / s2->y[j];
                    }
                }
                break;
        }

        if (cmbom.lnewhdr) {
            /* Copy new file's header */
            memcpy(s1->h, s2->h, sizeof(sac_hdr));
        }
        sac_extrema(s1);
    }

    /* - Calculate and set new range of dependent variable. */

    setrng();

  L_8888:
    bflclear();
    string_list_free(list);
    list = NULL;
    return;

}

/** 
 * Execute the action command "ADDF". This command adds a set of files 
 *   to data in memory 
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - ERROR_OPERATION_ON_UNEVEN_FILE
 *   - ERROR_OPERATION_ON_SPECTRAL_FILE
 *   - ERROR_HEADER_FILE_MISMATCH
 *
 * @date   881130:  Fixed bug in begin time error checking.
 * @date   850730:  Changes due to new memory manager.
 * @date   820809:  Changed to newest set of parsing and checking functions.
 * @date   820331:  Combined "parse" and "control" modules.
 * @date   810224:  Original version.
 *
 */
void
xaddf(int *nerr) {
    xbom_op('+', nerr);
}

void
xsubf(int *nerr) {
    xbom_op('-', nerr);
}

void
xmulf(int *nerr) {
    xbom_op('*', nerr);
}

void
xdivf(int *nerr) {
    xbom_op('/', nerr);
}
