/** 
 * @file   xch.c
 * 
 * @brief  Change a Header variable
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <fern/array.h>

#include "dfm.h"
#include "hdr.h"
#include "lhf.h"
#include "bool.h"
#include "cpf.h"

#include "msg.h"
#include "bot.h"
#include "ucf.h"
#include "dff.h"
#include "co.h"
#include "amf.h"
#include "errors.h"
#include "defs.h"

LHF_EXTERN

/** 
 * @param MGDTTM 
 *    15 - Size of a time value
 */
#define	MGDTTM	15

/** 
 * Execute the command CHNHDR (CH), which changes values for header
 *    variables in memory
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   961111:  Changed variables *a, *b, *e, *f, and *o to *arrivl, 
 *                  *begin, *ennd, *fini, and *origin, respectively (see
 *                   hdr.h)  maf.
 * @date   960812:  Added FILE option to allow file specification, maf
 * @date   910808:  Added test for identical station-event location and set
 *                  AZ and BAZ values both to zero. 
 * @date   900517:  Fixed VAX bug in changing longer header character variables.
 * @date   840216:  Added ability to change header times to any GMT time.
 * @date   821004:  Added automatic recalculation of distance, azimuth, etc.
 * @date   820204:  Fixed bug in "ALLT" option.
 * @date   820119:  Fixed bug in changing KHDR values.
 * @date   811120:  Added recalculation of ENND everytime.
 * @date   810630:  Added a special "ALLT' option to change all header times.
 * @date   810120:  Changed to output message retrieval from disk.
 *
 */
void
xch(int *nerr) {

    char khdrc[SAC_HEADER_STRINGS][SAC_HEADER_STRING_LENGTH], ktemp[19],
        ktok[9];
    int lallt, lfound, lhdrc[SAC_HEADER_LOGICALS], lnumbr, log;
    int icat[SAC_HEADER_WORDS], icatx, igdttm, ihdrc[SAC_HEADER_ENUMS],
        item[SAC_HEADER_WORDS];
    int itemx, ival, j, j1, jdfl;
    int ngdttm[MGDTTM][6], nhdrc[SAC_HEADER_INTEGERS];
    int nia, nitem;
    double diff;
    double fhdrc[SAC_HEADER_FLOATS];
    static int icatg = -1;
    double vallt;
    /* 1 if the corresponding file is to be modified */
    int *doFile;
    int idx;
    double fnumbr;
    sac *s;
    int *ip;
    double *const Fhdrc = &fhdrc[0] - 1;
    int *const Icat = &icat[0] - 1;
    int *const Ihdrc = &ihdrc[0] - 1;
    int *const Item = &item[0] - 1;
    int *const Lhdrc = &lhdrc[0] - 1;
    int *const Nhdrc = &nhdrc[0] - 1;

    *nerr = 0;

    doFile = xarray_new_with_len('i', saclen());

    /* PARSING PHASE: */
    /* - Initialize several state variables. */
    nitem = 0;
    lallt = FALSE;
    igdttm = 1;

    /* modify all files by default */
    for (idx = 0; idx < saclen(); idx++)
        doFile[idx] = TRUE;

    /* - Parse position dependant tokens, ie FILE; maf 960812 */
    if (lckeyExact("FILE", 5)) {
        /* initialize doFile to FALSE */
        for (idx = 0; idx < saclen(); idx++)
            doFile[idx] = FALSE;

        /* set specified values of doFile to TRUE */
        while (lcint(&lnumbr)) {
            if (lnumbr <= saclen() && lnumbr >= 1)
                doFile[lnumbr - 1] = TRUE;
            else {
                warning(1003, "%d", lnumbr);
                outmsg();
            }
        }
    }

    /* - Loop on each token in command: */
    while (lcmore(nerr)) {
        /* -- Get next token from command. */
        if (lckeyExact("ALLT$", 6)) {
            if (lcreal(&vallt)) {
                lallt = TRUE;
            } else {
                cerr(1001);
                goto L_8888;
            }
        } else if (lcchar(ktok, sizeof(ktok))) {
            hdrfld(ktok, 9, &icatx, &itemx, &lfound);
            /* --- If it is the name of a SAC header field. */
            if (!lfound) {
                cfmt("ILLEGAL OPTION:", 17);
                cresp();
            } else {
                /* Block changes to certain header fields */
                /* nvhdr, norid, nevid, npts, nsnpts, nwfid */
                if (icatx == INT_TYPE && itemx >= 8 && itemx <= 12) {
                    /* Increment token counter. */
                    arg_next();
                    setmsg("WARNING", 1389);
                    outmsg();
                    clrmsg();
                    continue;
                }
                if (icatx == LOGICAL_TYPE && itemx == 1) {
                    arg_next();
                    setmsg("WARNING", 1389);
                    outmsg();
                    clrmsg();
                    continue;
                }
                if(icatx == FLOAT_TYPE && itemx == 1) {
                    printf("WARNING: Changing DELTA with CHNHDR\n");
                }

                /* ---- Save category and item number. */
                nitem = nitem + 1;
                Icat[nitem] = icatx;
                Item[nitem] = itemx;
                /* ---- Depending upon what kind of header item (floating, integer,
                 *      alphanumeric, etc.), verify and save next token. */
                if (icatx == FLOAT_TYPE) {
                    if (lcreal(&fnumbr)) {
                        Fhdrc[itemx] = fnumbr;
                    } else if (lckey("UNDEF$", 7) || !arg()) {
                        Fhdrc[itemx] = SAC_FLOAT_UNDEFINED;
                    } else
                        if (lkia("GMT$", 5, 1, 6, &ngdttm[igdttm - 1][0], &nia))
                    {
                        Icat[nitem] = icatg;
                        Fhdrc[itemx] = (float) (igdttm);
                        for (j = nia; j < 6; j++) {
                            ngdttm[igdttm - 1][j] = 0.;
                        }
                        if (ngdttm[igdttm - 1][0] <= 99) {
                            time_t t;
                            char *tmptm;
                            int century, year;

                            t = time(NULL);
                            tmptm = ctime(&t);

                            year = atoi(tmptm + 22);
                            century = atoi(tmptm + 20) - year;

                            if (ngdttm[igdttm - 1][0] > year)
                                century -= 100;

                            ngdttm[igdttm - 1][0] =
                                century + ngdttm[igdttm - 1][0];
                        }
                        igdttm = igdttm + 1;
                    } else {
                        cerr(1001);
                        goto L_8888;
                    }
                } else if (icatx == INT_TYPE) {
                    if (lcint(&Nhdrc[itemx])) { /* do nothing */
                    } else if (lckey("UNDEF$", 7)) {
                        Nhdrc[itemx] = SAC_FLOAT_UNDEFINED;
                    } else {
                        cerr(1001);
                        goto L_8888;
                    }
                } else if (icatx == ENUM_TYPE) {
                    ival = 0;
                    if (lclist((char *) kmlhf.kiv, 9, SAC_ENUMS - 1, &ival)) {
                        Ihdrc[itemx] = ival;
                    } else if (lckey("UNDEF$", 7) || arg() == NULL) {
                        Ihdrc[itemx] = SAC_ENUM_UNDEFINED;
                    } else {
                        char *ts = token_as_string(arg());
                        error(*nerr = 1365, "\"%s\"", ts);
                        FREE(ts);
                        goto L_8888;
                    }
                } else if (icatx == LOGICAL_TYPE) {
                    if (lclog2("TRUE$", 6, "FALSE$", 7, &log)) {
                        Lhdrc[itemx] = log;
                    } else if (lclog2("YES$", 5, "NO$", 4, &log)) {
                        Lhdrc[itemx] = log;
                    } else if (lclog2(".TRUE.$", 8, ".FALSE.$", 9, &log)) {
                        Lhdrc[itemx] = log;
                    } else if (lckey("UNDEF$", 7)) {
                        Lhdrc[itemx] = FALSE;
                    } else {
                        cerr(1001);
                        goto L_8888;
                    }
                } else if (icatx == STRING_TYPE) {
                    int n = (itemx == 2) ? 2 : 1;
                    j1 = itemx;
                    memset(khdrc[j1 - 1], ' ', n * 8);
                    if (lckey("UNDEF$", 7)) {
                        /* Disallow undefined kstnm and kcmpnm */
                        if (itemx == 1 || itemx == 21) {
                            nitem--;
                            setmsg("WARNING", 1390);
                            outmsg();
                            clrmsg();
                            continue;
                        }
                        if(itemx == 2) {
                            strncpy(khdrc[j1 - 1], SAC_CHAR_UNDEFINED_2, strlen(SAC_CHAR_UNDEFINED_2));
                        } else {
                            strncpy(khdrc[j1 - 1], SAC_CHAR_UNDEFINED, strlen(SAC_CHAR_UNDEFINED));
                        }
                    } else {
                        strcpy(ktemp, "                  ");
                        if (lcchar_base(ktemp, sizeof(ktemp))) {
                            strncpy(khdrc[j1 - 1], ktemp, strlen(ktemp));
                        }
                    }
                    khdrc[j1 - 1][n * 8] = 0;
                } else {
                    *nerr = 901;
                    setmsg("ERROR", *nerr);
                    apcmsg("CHNHDR", 7);
                    goto L_8888;
                }
            }
        } else {
            /* -- Bad syntax. */
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    /* - The above loop is over when one of two conditions has been met:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
     *   (2) All the tokens in the command have been successfully parsed. */

    if (*nerr != 0)
        goto L_8888;

    /* EXECUTION PHASE: */

    /* - For each file in data file list: */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        if (doFile[jdfl - 1]) { /* file specification. maf 960812 */

            if (!(s = sacget(jdfl - 1, FALSE, nerr))) {
                goto L_8888;
            }
            //getfil( jdfl, FALSE, &junk1, &junk2, &junk3, nerr );

            /* -- Update appropriate header fields. */
            for (j = 1; j <= nitem; j++) {
                switch (Icat[j]) {
                    case FLOAT_TYPE:
                        sac_set_float(s, Item[j], Fhdrc[Item[j]]);
                        if (Item[j] >= 6 && Item[j] <= 20)
                            iztypeMessage(Item[j], s->h->iztype);
                        break;
                    case -1:   // Set Time GMT
                        igdttm = (int) (Fhdrc[Item[j]]);
                        ddttm(&ngdttm[igdttm - 1][0], &s->h->nzyear, &diff);
                        sac_set_float(s, Item[j], diff);
                        break;
                    case INT_TYPE:
                        ip = nhdr(s, Item[j]);
                        if(Item[j] == 7 && (Nhdrc[Item[j]] != 6 && Nhdrc[Item[j]] != 7)) {
                            printf(" WARNING: Invalid header version. Valid values: 6 or 7\n");
                            break;
                        }
                        VALUE(ip) = Nhdrc[Item[j]];
                        break;
                    case ENUM_TYPE:
                        ip = ihdr(s, Item[j]);
                        VALUE(ip) = Ihdrc[Item[j]];
                        break;
                    case LOGICAL_TYPE:
                        ip = lhdr(s, Item[j]);
                        VALUE(ip) = Lhdrc[Item[j]];
                        break;
                    case STRING_TYPE:
                        strcpy(khdr(s, Item[j]), khdrc[Item[j] - 1]);
                        break;
                }
            }

            /* -- Change all time fields if requested. */
            if (lallt) {
                for(int i = SAC_B; i <= SAC_F; i++) {
                    double t = 0.0;
                    sac_get_float(s, i, &t);
                    if(t != SAC_FLOAT_UNDEFINED) {
                        t += vallt;
                        sac_set_float(s, i, t);
                    }
                }
                if (s->h->nzyear != SAC_INT_UNDEFINED) {
                    idttm(&s->h->nzyear, -vallt, &s->h->nzyear);
                }
            }

            /* -- Recompute ending time if appropriate. */
            sac_be(s);

            /* -- Recompute distance, azimuth, etc. if proper header fields are present. */
            update_distaz(s);

        }                       /* end if ( doFile [ jdfl - 1 ] ). maf 960812 */
    }                           /* end for loop between files */

  L_8888:
    xarray_free(doFile);
    return;
}
