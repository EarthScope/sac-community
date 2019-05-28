/** \file
 * Handle Polezero Subtype Files for transfer
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <ctype.h>
#include <glob.h>

#include "amf.h"
#include "icm.h"

#include "bool.h"
#include "hdr.h"

#include "string_utils.h"

#include "co.h"
#include "msg.h"
#include "bot.h"
#include "ucf.h"

#include "EVRESPnames.h"
#include "SacHeader.h"
#include "sac_datetime.h"

#ifdef WIN32
#define pointer char *
#else
#define pointer void *
#endif

#define	MPOLES	30
#define	MZEROS	30

#define KEY_CONSTANT  "CONSTANT"
#define KEY_ZEROS     "ZEROS"
#define KEY_POLES     "POLES"
#define KEY_STAR      '*'

datetime *
datetime_get_file_time(datetime * t) {
    int dir;
    sac *s;
    s = sacget_current();
    if (!t) {
        t = datetime_new();
    }

    dir = getTransferDirection();
    if (isSet(TIME, dir)) {
        datetime_set_hour(t, getTime(dir, EV_HOUR));
        datetime_set_minute(t, getTime(dir, EV_MIN));
        datetime_set_second(t, getTime(dir, EV_SEC));
        datetime_set_nanosecond(t, getTime(dir, EV_MSEC) * 1000000);
    } else {
        datetime_set_hour(t, s->h->nzhour);
        datetime_set_minute(t, s->h->nzmin);
        datetime_set_second(t, s->h->nzsec);
        if(s->h->nzmsec >= 0) {
            datetime_set_nanosecond(t, s->h->nzmsec * 1000000);
        }
    }
    if (isSet(DATE, dir)) {
        datetime_set_year(t, getYear(dir) - 1900);
        datetime_set_doy(t, getJday(dir) - 1);
        datetime_doy2ymd(t);
    } else {
        datetime_set_year(t, s->h->nzyear);
        datetime_set_doy(t, s->h->nzjday);
        datetime_doy2ymd(t);
    }
    datetime_normalize(t);
    return t;
}

/** 
 * Handle a Pole Zero file \p subtyp for the transfer command.  Parse and return
 *   the appropriate variables.
 *
 *   Generic transfer function - user supplies poles and zeros
 *        Search for the polezero file.  Search order is:
 *         - (1) current directory.
 *         - (2) global polezero directory. 
 *
 *   Polezero understands Four different Key Words
 *      - CONSTANT real-number
 *          Scaling Value
 *      - ZEROS npoles
 *          Number of Zeros
 *          Location of Complex Zeros follow -->  Real Imaginary
 *      - POLES npoles
 *          Number of Poles
 *          Location of Complex Poles follow -->  Real Imaginary
 *      - * 
 *          Comment, rest of the line is ignored
 *
 * \param nfreq
 *     Input Number of frequencies.  Defined from the maximum number of points in the
 *     entire data file list: npts_max.  The next larger power of 2 is taken
 *     from npts_max to find the the number of points for the FFT: nfft.  Then
 *     nfft is divided by 2 and given one more point. 
 *     \see xtransfer
 * \param delfrq
 *     Input Frequency spacing, defined as 1 / ( nfft * dt ). 
 *     \see transfer
 * \param xre
 *     Output Real Part of the transfer function in the frequency domain.
 *     Length \p nfreq
 * \param xim
 *     Output Imaginary Part of the transer function in the frequency domain.
 *     Length \p nfreq
 * \param subtyp
 *     Input Name of file containing the Pole Zero Response
 * \param subtyp_s
 *     Input Length of string \p subtyp
 * \param nerr
 *     Error return flag
 *          - 0 on Success
 *          - Non-Zero on Error
 *
 * \see xtransfer
 *
 *
 * \date   970129:  Add parameter (0) to cnvatf and cnvati.  0 means that if
 *                  a string of digits is too long, let it slide by.  maf 
 * \date   071003   Documented/Reviewed
 * \date   071003   Fixed a bug with the length of a string key.  Was defined as
 *                  length 9, now automatically determined.
 */

void
polezero(int nfreq, double delfrq, double xre[], double xim[], char *subtyp,
         int subtyp_s, int *nerr) {
    char kfile[MCPFN + 1], kiline[MCMSG + 1];
    char *key;
    char *kline;
    int lexist, lopen, lpoles, lzeros;
    int i, idx, ic, ic1, ic2, ipoles = 0, itype, izeros = 0, nc, ncerr, npoles, nzeros,
        numsave;
    FILE *nun;
    float temp1, temp2;
    double const_;
    complexd poles[MPOLES], zeros[MZEROS];
    char *s1;

    pzmeta_t *meta, *meta_used;
    datetime *filetime;

    char *stat, *net, *loc, *chan;
    char *pstat, *pnet, *ploc, *pchan;
    sac *s;

    complexd *const Poles = &poles[0] - 1;
    complexd *const Zeros = &zeros[0] - 1;
    s = sacget_current();
    memset(kfile, 0, sizeof(kfile));
    memset(kiline, 0, sizeof(kiline));
    pstat = pnet = ploc = pchan = NULL;
    *nerr = 0;
    meta = NULL;
    meta_used = NULL;
    lopen = FALSE;

    for (idx = 0; idx < MCPFN; idx++)
        kfile[idx] = ' ';
    kfile[MCPFN] = '\0';

    filetime = datetime_get_file_time(NULL);
    meta  = polezero_meta_new();
    enum Direction dir = getTransferDirection();

#define SET_OR_VALUE(key, pstr, str, hdr, func)         \
    do {                                                \
        if(isSet(key, dir)) {                           \
            pstr = str = strdup( func (dir));           \
        } else {                                        \
            pstr  = strdup(s->h->hdr);                  \
            str   = rstrip(lstrip(pstr));               \
            if (!SAC_CHAR_DEFINED(str)) { str[0] = 0; } \
        }                                               \
    } while(0)

    SET_OR_VALUE(NETWORK, pnet,  net,  knetwk, getNetworkName);
    SET_OR_VALUE(STATION, pstat, stat, kstnm,  getStationName);
    SET_OR_VALUE(LOCID,   ploc,  loc,  khole,  getLocidName);
    SET_OR_VALUE(CHANNEL, pchan, chan, kcmpnm, getChannelName);

    // Do search for polezero file in current directory
    if(strcmp(subtyp, "") == 0) {
        glob_t g;
        int ok = FALSE;
        char glob_path[512];
        if(isSet(PATH, dir)) {
            char *path = getPath(dir);
            sprintf(glob_path, "%s/SAC_PZs_%s_%s_%s_%s_*", path, net, stat, chan, loc);
        } else {
            sprintf(glob_path, "SAC_PZs_%s_%s_%s_%s_*", net, stat, chan, loc);
        }
        glob(glob_path, 0, NULL, &g);
        for(size_t i = 0; i < g.gl_pathc; i++) {
            // Recursive Call Here
            polezero(nfreq, delfrq, xre, xim, g.gl_pathv[i], subtyp_s, nerr);
            if(*nerr == 0) {
                ok = TRUE;
                break;
            }
        }
        globfree(&g);
        if(!ok) {
            clrmsg();
            error(*nerr = 2104, "POLEZERO.\n"
                  "             Search for sacpz file failed using pattern: %s\n"
                  "             Please specify a sacpz file for the sub-type", glob_path);
        }
        goto L_8888;
    }

    // Check is file exists
    fstrncpy(kfile, MCPFN, subtyp, strlen(subtyp));

    {
        glob_t g;
        int ok = FALSE;
        glob(subtyp, 0, NULL, &g);

        // No pattern matching occurred
        if(g.gl_pathc == 0) {
            // No files found
            globfree(&g);
        } else if(g.gl_pathc == 1 && strcmp(subtyp, g.gl_pathv[0]) == 0) {
            // No pattern matching occurred, file specified directly
            globfree(&g);
        } else {
            // Pattern present with matches
            for(size_t i = 0; i < g.gl_pathc; i++) {
                polezero(nfreq, delfrq, xre, xim, g.gl_pathv[i], subtyp_s, nerr);
                if(*nerr == 0) {
                    ok = TRUE;
                    break;
                }
            }
            globfree(&g);
            if(!ok) {
                clrmsg();
                error(*nerr = 2114, "\n"
                      "             Search for sacpz file failed using pattern: %s\n"
                      "             net.stat.loc.cha: %s.%s.%s.%s",
                      subtyp, net, stat, chan, loc);
            }
            goto L_8888;
        }
    }


    zinquire(kfile, &lexist);
    if (lexist) {
        goto L_5000;
    }

    if(strlen(rstrip(lstrip(kfile))) == 0) {
        error(*nerr = 2104, "POLEZERO");
    } else {
        error(*nerr = 108, subtyp);
    }
    goto L_8888;

    /* - Set default values for constant, poles, and zeros. */

  L_5000:
    const_ = 1.0;
    npoles = 0;
    nzeros = 0;
    for (i = 1; i <= MZEROS; i++) {
        Zeros[i] = dbltocmplx(0.0, 0.0);
    }

    for (i = 1; i <= MPOLES; i++) {
        Poles[i] = dbltocmplx(0.0, 0.0);
    }

    /* - Open file. */

    zopens(&nun, kfile, MCPFN + 1, nerr);
    if (*nerr != 0)
        goto L_8888;
    lopen = TRUE;

    /* - Read and decode lines in file. */

    lpoles = FALSE;
    lzeros = FALSE;
  L_6000:
    if (fgetsp(kiline, MCMSG + 1, nun) == NULL) {
        if (feof(nun))
            goto L_7000;
        goto L_9000;
    }

    /* remove leading blanks. */
    kline = kiline;
    while ((*kline == ' ') || (*kline == '\n') || (*kline == '\t')) {
        kline++;
    }

    /* Continue if empty line */
    if ((numsave = strlen(kline)) == 0) {
        goto L_6000;
    }
    /* Convert newline into space */
    if (kline[numsave - 1] == '\n') {
        kline[numsave - 1] = ' ';
    }

    /* eliminate tabs in the input line */
    for (i = 0; i < numsave; i++) {
        if (kline[i] == '\t') {
            kline[i] = ' ';
        }
    }
    nc = indexb(kline, strlen(kline) + 1);
    ic = 0;
    poptok(kline, nc, &ic, &ic1, &ic2, &itype);

    /* Make space for the possible KeyValue and truncate the string */
    key = strdup(kline);
    memset(key, 0, strlen(key));

    modcase(TRUE, kline + ic1 - 1, ic2 - ic1 + 1, key);

    if (key[0] == KEY_STAR) {   /* Comment Line */
        polezero_comment_parse(kiline, meta);
    } else if (!polezero_is_correct_block(meta, filetime, stat, net, loc, chan)) {

    } else if (strncmp(key, KEY_CONSTANT, strlen(KEY_CONSTANT)) == 0) {
        poptok(kline, nc, &ic, &ic1, &ic2, &itype);
        strncpy((s1 = malloc(ic2 - ic1 + 2)), kline + ic1 - 1, ic2 - ic1 + 1);
        s1[ic2 - ic1 + 1] = '\0';

        const_ = atof(s1);
        if (const_ == 0 || const_ == HUGE_VAL || const_ == -HUGE_VAL ||
            isnan(const_)) {
            *nerr = 2118;
            setmsg("ERROR", *nerr);
            apcmsg("Unrecognized Constant: ", 24);
            apcmsg(s1, strlen(s1) + 1);
            free(s1);
            goto L_8888;
        }
        free(s1);
        meta_used = polezero_meta_copy(meta);

    } else if (strncmp(key, KEY_POLES, strlen(KEY_POLES)) == 0) {
        poptok(kline, nc, &ic, &ic1, &ic2, &itype);
        strncpy((s1 = malloc(ic2 - ic1 + 2)), kline + ic1 - 1, ic2 - ic1 + 1);
        s1[ic2 - ic1 + 1] = '\0';
        cnvati(s1, ic2 - ic1 + 2, &npoles, 0, nerr);    /* add 0 before nerr. maf 970129 */
        free(s1);
        if (*nerr != 0)
            goto L_8888;
        if (npoles > MPOLES) {
            *nerr = 2109;
            setmsg("ERROR", *nerr);
            apcmsg(subtyp, subtyp_s);
            apimsg(MPOLES);
            goto L_8888;
        }
        lpoles = TRUE;
        lzeros = FALSE;
        ipoles = 0;
    } else if (strncmp(key, KEY_ZEROS, strlen(KEY_ZEROS)) == 0) {
        poptok(kline, nc, &ic, &ic1, &ic2, &itype);
        strncpy((s1 = malloc(ic2 - ic1 + 2)), kline + ic1 - 1, ic2 - ic1 + 1);
        s1[ic2 - ic1 + 1] = '\0';
        cnvati(s1, ic2 - ic1 + 2, &nzeros, 0, nerr);    /* add 0 before nerr. maf 970129 */
        free(s1);
        if (*nerr != 0)
            goto L_8888;
        if (nzeros > MZEROS) {
            *nerr = 2109;
            setmsg("ERROR", *nerr);
            apcmsg(subtyp, subtyp_s);
            apimsg(MZEROS);
            goto L_8888;
        }
        lpoles = FALSE;
        lzeros = TRUE;
        izeros = 0;
    } else if (lpoles) {
        if (ipoles < MPOLES) {
            ipoles = ipoles + 1;
            strncpy((s1 =
                     malloc(ic2 - ic1 + 2)), kline + ic1 - 1, ic2 - ic1 + 1);
            s1[ic2 - ic1 + 1] = '\0';
            cnvatf(s1, ic2 - ic1 + 2, &temp1, 0, nerr); /* add 0 before nerr. maf 970129 */
            if (*nerr != 0) {
                *nerr = 2126;
                setmsg("ERROR", *nerr);
                apcmsg(s1, strlen(s1) + 1);
                free(s1);
                goto L_8888;
            }
            free(s1);
            poptok(kline, nc, &ic, &ic1, &ic2, &itype);
            strncpy((s1 =
                     malloc(ic2 - ic1 + 2)), kline + ic1 - 1, ic2 - ic1 + 1);
            s1[ic2 - ic1 + 1] = '\0';
            cnvatf(s1, ic2 - ic1 + 2, &temp2, 0, nerr); /* add 0 before nerr. maf 970129 */
            if (*nerr != 0) {
                *nerr = 2126;
                setmsg("ERROR", *nerr);
                apcmsg(s1, strlen(s1) + 1);
                free(s1);
                goto L_8888;
            }
            free(s1);
            Poles[ipoles] = dbltocmplx(temp1, temp2);
        } else {
            *nerr = 2108;
            setmsg("ERROR", *nerr);
            apcmsg(subtyp, subtyp_s);
            apimsg(MPOLES);
            goto L_8888;
        }
    } else if (lzeros) {
        if (izeros < MZEROS) {
            izeros = izeros + 1;
            strncpy((s1 =
                     malloc(ic2 - ic1 + 2)), kline + ic1 - 1, ic2 - ic1 + 1);
            s1[ic2 - ic1 + 1] = '\0';
            cnvatf(s1, ic2 - ic1 + 2, &temp1, 0, nerr); /* add 0 before nerr. maf 970129 */
            if (*nerr != 0) {
                *nerr = 2127;
                setmsg("ERROR", *nerr);
                apcmsg(s1, strlen(s1) + 1);
                free(s1);
                goto L_8888;
            }
            free(s1);
            poptok(kline, nc, &ic, &ic1, &ic2, &itype);
            strncpy((s1 =
                     malloc(ic2 - ic1 + 2)), kline + ic1 - 1, ic2 - ic1 + 1);
            s1[ic2 - ic1 + 1] = '\0';
            cnvatf(s1, ic2 - ic1 + 2, &temp2, 0, nerr); /* add 0 before nerr. maf 970129 */
            if (*nerr != 0) {
                *nerr = 2127;
                setmsg("ERROR", *nerr);
                apcmsg(s1, strlen(s1) + 1);
                free(s1);
                goto L_8888;
            }
            free(s1);
            Zeros[izeros] = dbltocmplx(temp1, temp2);
        } else {
            *nerr = 2109;
            setmsg("ERROR", *nerr);
            apcmsg(subtyp, subtyp_s);
            apimsg(MZEROS);
            goto L_8888;
        }
    } else {
        *nerr = 2110;
        setmsg("ERROR", *nerr);
        apcmsg(subtyp, subtyp_s);
        apcmsg(key, 9);
        goto L_8888;
    }
    free(key);
    goto L_6000;

    /* - Compute transfer function. */

  L_7000:
    if (const_ == 1.0 && npoles == 0 && nzeros == 0) {
        *nerr = 2114;
        error(*nerr, "\n Station: %s.%s.%s.%s", net, stat, chan, loc);
    } else {
        printf(" Using polezero response for %s, %s, %s, %s from %s\n", stat, chan,
               net, loc, subtyp);
        if (FALSE) {
            printf("\n");
            if (meta_used && datetime_status(meta_used->start) == DATETIME_OK &&
                datetime_status(meta_used->end) == DATETIME_OK) {
                printf("  Station:  %s.%s.%s.%s <> %s.%s.%s.%s \n",
                       meta_used->net, meta_used->stat, meta_used->chan,
                       meta_used->loc, net, stat, chan, loc);
                printf("  On:       ");
                datetime_printn(meta_used->start);
                printf("  Off:      ");
                datetime_printn(meta_used->end);
                printf("  File:     ");
                datetime_printn(filetime);
            } else {
                printf("  Station:  %s.%s.%s.%s \n", net, stat, chan, loc);
            }
            printf("  Polezero Response\n");
            printf("\tconstant: %e\n", const_);
            printf("\tzeros:    %d\n", nzeros);
            for (i = 0; i < nzeros; i++) {
                printf("\t    %e  %e\n", zeros[i].re, zeros[i].im);
            }
            printf("\tpoles:    %d\n", npoles);
            for (i = 0; i < npoles; i++) {
                printf("\t    %e  %e\n", poles[i].re, poles[i].im);
            }
        }
    }

    getrand(nfreq, delfrq, const_, nzeros, zeros, npoles, poles, xre, xim);
    datetime_free(filetime);
    polezero_meta_free(meta);
    polezero_meta_free(meta_used);

  L_8888:
    if (pstat) {
        free(pstat);
    }
    if (pchan) {
        free(pchan);
    }
    if (pnet) {
        free(pnet);
    }
    if (ploc) {
        free(ploc);
    }
    pstat = pchan = pnet = ploc = NULL;

    if (lopen)
        zcloses(&nun, &ncerr);
    return;

  L_9000:
    *nerr = 114;
    setmsg("ERROR", *nerr);
    apcmsg(kfile, MCPFN + 1);
    goto L_8888;

}
