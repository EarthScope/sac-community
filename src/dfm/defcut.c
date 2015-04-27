/** 
 * @file   defcut.c
 * 
 * @brief  Define cut parameters
 * 
 */

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "amf.h"
#include "dfm.h"
#include "hdr.h"

#include "errors.h"

#include "msg.h"
#include "clf.h"
#include "bot.h"

#include "SacHeader.h"

//#define __DEBUG__
#include "debug.h"

void cut_define(float b, float delta, double dt, int *n);
void cut_define_check(float start, float stop, int npts, int cuterr,
                      int *nstart, int *nstop, int *nfillb, int *nfille,
                      int *nerr);

/** 
 * Define cut parameters for a given data file
 * 
 * @param kcut
 *    Which parameter
 * @param ocut
 *    Cut times
 * @param idfl
 *    Data file list number
 * @param nerr
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_SAC_LOGIC_ERROR
 *    - ERROR_UNDEFINED_START_CUT_TIME
 *    - ERROR_UNDEFINED_STOP_CUT_TIME
 *    - ERROR_START_TIME_LESS_THAN_BEGIN
 *    - ERROR_STOP_TIME_GREATER_THAN_END
 *    - ERROR_START_TIME_GREATER_THAN_END
 *    - ERROR_STOP_TIME_LESS_THAN_BEGIN
 *    - ERROR_START_TIME_GREATER_THAN_STOP
 *    - ERROR_CORRECTED_BY_FILL_WITH_ZEROS
 *    - ERROR_CORRECTED_BY_USING_START_TIME
 *    - ERROR_CORRECTED_BY_USING_END_TIME
 *
 * @date   961211, 970114, 970214, and 970304:  
 *             I made incremental changes to harden cut.  It now cuts
 *             precisely on the values entered.  maf
 * @date   880128:  Fixed bug that occurred when the sampling interval
 *             was smaller than the machine roundoff factor.
 * @date   850415:  Changes due to restructuring of DFM common block.
 * @date   840228:  Original version from DEFMEM.
 *
 */
void
defcut(char kcut[2][9], double ocut[2], int idfl, int *nerr) {

    int jdx, nptrd;
    double pick[2], start, stop;
    double *const Pick = &pick[0] - 1;

    sac *s;
    *nerr = 0;

    if (!(s = sacget(idfl - 1, FALSE, nerr))) {
        return;
    }

    /* - Save total number of points in file. */
    s->m->ntotal = s->h->npts;
    DEBUG("kcut <%s> <%s>\n", kcut[0], kcut[1]);
    /* - Compute start value. */
    if (strcmp(kcut[0], "Z       ") == 0) {
        Pick[1] = 0.;
    }

    else if (strcmp(kcut[0], "N       ") != 0) {
        jdx = nequal((char *) kcut[0], (char *) kmdfm.kpick, 9, MPICK);
        DEBUG("START pick index: %d (string list) => %d\n", jdx,
              cmdfm.ipckhd[jdx - 1]);
        if (jdx > 0)
            Pick[1] = VALUE(fhdr(s, cmdfm.ipckhd[jdx - 1]));

        else {
            *nerr = ERROR_SAC_LOGIC_ERROR;
            setmsg("ERROR", *nerr);
            apcmsg("DEFCUT #2", 10);
            return;
        }
    } else {
        *nerr = ERROR_SAC_LOGIC_ERROR;
        setmsg("ERROR", *nerr);
        apcmsg("DEFCUT #2", 10);
        return;
    }

    /* - Check to make sure the requested pick field 
     *   in the header is defined. 
     */
    if (Pick[1] == SAC_FLOAT_UNDEFINED) {
        if (cmdfm.icuter == 1) {
            *nerr = ERROR_UNDEFINED_START_CUT_TIME;
            setmsg("ERROR", *nerr);
            apcmsg2(s->m->filename, strlen(s->m->filename) + 1);
            return;
        } else {
            setmsg("WARNING", ERROR_UNDEFINED_START_CUT_TIME);
            apcmsg2(s->m->filename, strlen(s->m->filename) + 1);
            outmsg();
            setmsg("OUTPUT", ERROR_CORRECTED_BY_USING_BEGIN_TIME);
            outmsg();
            start = s->h->b;
            s->m->nstart = 1;
        }
    } else {
        /* start time of data to read */
        start = Pick[1] + ocut[0];
        cut_define(s->h->b, s->h->delta, start, &s->m->nstart);
    }

    /* -  Compute stop value. */
    if (strcmp(kcut[1], "N       ") == 0) {
        nptrd = ocut[1] + RNDOFF * s->h->delta;
        stop = start + (double) (nptrd - 1) * s->h->delta;
        s->m->nstop = s->m->nstart + nptrd - 1;
        Pick[2] = 0.;
    } else {
        jdx = nequal((char *) kcut[1], (char *) kmdfm.kpick, 9, MPICK);
        DEBUG("STOP pick index: %d (string list) => %d\n", jdx,
              cmdfm.ipckhd[jdx - 1]);
        if (jdx <= 0) {
            *nerr = ERROR_SAC_LOGIC_ERROR;
            setmsg("ERROR", *nerr);
            apcmsg("DEFCUT #3", 10);
            return;
        } else {
            if (strcmp(kcut[1], "Z       ") == 0) {
                Pick[2] = 0.0;
            } else {
                Pick[2] = VALUE(fhdr(s, cmdfm.ipckhd[jdx - 1]));
            }
            stop = Pick[2] + ocut[1];
            cut_define(s->h->b, s->h->delta, stop, &s->m->nstop);
        }
    }

    /* - Make sure stop pick is defined. */
    if (Pick[2] == SAC_FLOAT_UNDEFINED) {
        if (cmdfm.icuter == 1) {
            *nerr = ERROR_UNDEFINED_STOP_CUT_TIME;
            setmsg("ERROR", *nerr);
            apcmsg2(s->m->filename, strlen(s->m->filename) + 1);
            return;
        } else {
            setmsg("WARNING", ERROR_UNDEFINED_STOP_CUT_TIME);
            apcmsg2(s->m->filename, strlen(s->m->filename) + 1);
            outmsg();
            setmsg("OUTPUT", ERROR_CORRECTED_BY_USING_END_TIME);
            outmsg();
            stop = s->h->e;
            s->m->nstop = s->h->npts;
        }
    }

    /* Check the cut time and adjust Nstart, Nstop, Nfillb, Nfille */
    cut_define_check(start, stop, s->h->npts, cmdfm.icuter, &s->m->nstart,
                     &s->m->nstop, &s->m->nfillb, &s->m->nfille, nerr);
    /* Error checking */
    if (*nerr) {
        switch (*nerr) {
            case ERROR_START_TIME_GREATER_THAN_STOP:
                error(*nerr, "%s\n\ttime:  %f >= %f\n\tindex: %d >= %d",
                      s->m->filename, start, stop, s->m->nstart, s->m->nstop);
                return;
                break;
            case ERROR_START_TIME_GREATER_THAN_END:
                error(*nerr, "%s\n\ttime:  %f > %f\n\tindex: %d > %d",
                      s->m->filename, start, s->h->e, s->m->nstart, s->h->npts);
                return;
                break;
            case ERROR_STOP_TIME_LESS_THAN_BEGIN:
                error(*nerr, "%s\n\ttime:  %f < %f\n\tindex: %d < %d",
                      s->m->filename, stop, s->h->b, s->m->nstop, 1);
                return;
                break;
            case ERROR_START_TIME_LESS_THAN_BEGIN:
                if (cmdfm.icuter == 2) {
                    setmsg("WARNING", *nerr);
                    apcmsg2(s->m->filename, strlen(s->m->filename) + 1);
                    outmsg();
                    setmsg("OUTPUT", ERROR_CORRECTED_BY_USING_BEGIN_TIME);
                    outmsg();
                    *nerr = SAC_OK;
                } else {
                    setmsg("ERROR", *nerr);
                    apcmsg2(s->m->filename, strlen(s->m->filename) + 1);
                    return;
                }
                break;
            case ERROR_STOP_TIME_GREATER_THAN_END:
                if (cmdfm.icuter == 2) {
                    setmsg("WARNING", *nerr);
                    apcmsg2(s->m->filename, strlen(s->m->filename) + 1);
                    outmsg();
                    setmsg("OUTPUT", ERROR_CORRECTED_BY_USING_END_TIME);
                    outmsg();
                    *nerr = SAC_OK;
                } else {
                    setmsg("ERROR", *nerr);
                    apcmsg2(s->m->filename, strlen(s->m->filename) + 1);
                    return;
                }
                break;
            case ERROR_CUT_TIMES_BEYOND_DATA_LIMITS:
                /* Begin */
                setmsg("WARNING", ERROR_START_TIME_LESS_THAN_BEGIN);
                apcmsg2(s->m->filename, strlen(s->m->filename) + 1);
                outmsg();
                setmsg("OUTPUT", ERROR_CORRECTED_BY_USING_BEGIN_TIME);
                outmsg();
                /* End */
                setmsg("WARNING", ERROR_STOP_TIME_GREATER_THAN_END);
                apcmsg2(s->m->filename, strlen(s->m->filename) + 1);
                outmsg();
                setmsg("OUTPUT", ERROR_CORRECTED_BY_USING_END_TIME);
                outmsg();
                *nerr = SAC_OK;
                break;
        }
    }
    /* - Convert these start and stop points to new begin and end times. */
    s->h->b = s->h->b + (double) (s->m->nstart - 1) * s->h->delta;
    s->h->npts = s->m->nstop - s->m->nstart + 1;
    s->h->e = s->h->b + (double) (s->h->npts - 1) * s->h->delta;

    DEBUG("nstart[%d]: %d\n", idfl, Nstart[idfl]);
    DEBUG("nstop[%d]:  %d\n", idfl, Nstop[idfl]);
    DEBUG("npts[%d]:   %d\n", idfl, Nstop[idfl] - Nstart[idfl] + 1);
    DEBUG("CUT: %f %f %d %f\n", *begin, *ennd, *npts, *ennd - *begin);
}
