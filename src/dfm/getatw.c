/** 
 * @file   getatw.c
 * 
 * @brief  Convert a relative time window to an absolute one
 * 
 */

#include <string.h>

#include "co.h"
#include "amf.h"
#include "dfm.h"
#include "hdr.h"

#include "errors.h"

#include "SacHeader.h"
#include "msg.h"
#include "clf.h"
#include "bot.h"

DFM_EXTERN

/** 
 * Convert a relative time window to an absolute time window by 
 *   examing the header for the current data file
 * 
 * @param krtw 
 *    Two element array containing the starting and ending relative
 *    time picks
 * @param krtw_s
 *    Length of \p krtw 
 * @param ortw 
 *    Two element array containing the starting and ending relative
 *    time offsets.
 * @param tmin
 *    Minimum value of absolute time window on output
 * @param tmax 
 *    Maximum value of absolute time window on output
 * @param nofmin 
 *    Window offset in points of minimum
 * @param nlnwin 
 *    Number of points in the absolute time window
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_ILLEGAL_RELATIVE_TIME_PICK
 *    - ERROR_UNDEFINED_START_CUT_TIME
 *    - ERROR_UNDEFINED_STOP_CUT_TIME
 *
 * @date    860304:  Added calculation of NOFWIN and NLNWIN.
 * @date    820623:  Original version.
 *
 */
void
getatw(char *krtw, int krtw_s, double *ortw, double *tmin, double *tmax,
       int *nofmin, int *nlnwin, int *nerr) {

#define KRTW(I_,J_)	(krtw+(I_)*(krtw_s)+(J_))

    int irtb, irte, nofmax, num;
    float rtrb, rtre;
    double *const Ortw = &ortw[0] - 1;
    sac *s;

    *nerr = 0;
    s = sacget_current();
    /* - Get indexes of start and stop time picks. */

    irtb = nequal(KRTW(0, 0), (char *) kmdfm.kpick, 9, MPICK);
    if ((irtb <= 0 || irtb == cmdfm.ipckn) || irtb == cmdfm.ipckg) {
        *nerr = ERROR_ILLEGAL_RELATIVE_TIME_PICK;
        setmsg("ERROR", *nerr);
        goto L_8888;
    }
    irte = nequal(KRTW(1, 0), (char *) kmdfm.kpick, 9, MPICK);
    if (irte <= 0 || irte == cmdfm.ipckg) {
        *nerr = ERROR_ILLEGAL_RELATIVE_TIME_PICK;
        setmsg("ERROR", *nerr);
        goto L_8888;
    }

    /* - Determine start absolute time window. */
    if (irtb == cmdfm.ipckz) {
        *tmin = Ortw[1];
    } else {
        rtrb = VALUE(fhdr(s, cmdfm.ipckhd[irtb - 1]));
        if (rtrb != SAC_FLOAT_UNDEFINED) {
            *tmin = rtrb + Ortw[1];
        } else {
            *tmin = s->h->b;
            *nerr = ERROR_UNDEFINED_START_CUT_TIME;
            setmsg("ERROR", *nerr);
            apcmsg2(s->m->filename, strlen(s->m->filename) + 1);
            goto L_8888;
        }
    }

    /* - Determine stop absolute window. */

    if (irte == cmdfm.ipckz) {
        *tmax = Ortw[2];
    } else if (irte == cmdfm.ipckn) {
        num = (int) (Ortw[2]);
        *tmax = *tmin + s->h->delta * (float) (num);
    } else {
        rtre = VALUE(fhdr(s, cmdfm.ipckhd[irte - 1]));
        if (rtre != SAC_FLOAT_UNDEFINED) {
            *tmax = rtre + Ortw[2];
        } else {
            *tmax = s->h->e;
            *nerr = ERROR_UNDEFINED_STOP_CUT_TIME;
            setmsg("ERROR", *nerr);
            apcmsg2(s->m->filename, strlen(s->m->filename) + 1);
            goto L_8888;
        }
    }
    if(*tmin > *tmax) {
        error(*nerr = 8009, "(%.5g > %.5g)", *tmin, *tmax);
        return;
    }
    if(*tmax < s->h->b && *tmin < s->h->b) {
        error(*nerr = 8010, "(%.5g < %.5g)", *tmax, s->h->b);
        return;
    }
    if(*tmin > s->h->e && *tmax > s->h->e) {
        error(*nerr = 8011, "(%.5g > %.5g)", *tmin, s->h->e);
        return;
    }

    if(*tmin < s->h->b) {
        warning(8012, "(%.5g < %.5g)", *tmin, s->h->b);
        outmsg();
        clrmsg();
        *tmin = s->h->b;
    }

    if(*tmax > s->h->e) {
        warning(8013, "(%.5g > %.5g)", *tmax, s->h->e);
        outmsg();
        clrmsg();
        *tmax = s->h->e;
    }

    /* - Determine offset and length of window in points. */

    *nofmin = (int) ((*tmin - s->h->b) / s->h->delta);
    nofmax = (int) ((*tmax - s->h->b) / s->h->delta);
    *nlnwin = nofmax - *nofmin + 1;

    if(*nlnwin <= 0) {
        message(MOUTPUT, 0, "Warning: No data points within Time window\n"
                "\tStart time %.5g End time: %.5g", *tmin, *tmax);
        outmsg();
        clrmsg();
    }
    
  L_8888:
    return;

#undef	KRTW
}
