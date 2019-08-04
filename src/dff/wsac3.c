/** 
 * @file   wsac3.c
 * 
 * @brief  Write a SAC file
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "hdr.h"
#include "msg.h"
#include "co.h"
#include "ucf.h"
#include "errors.h"

#include "debug.h"

/** 
 * Write a SAC file to disk using the current header values
 * 
 * @param kname 
 *    Name of the file to write
 * @param xarray 
 *    Array containing the independent variable, e.g. Time
 *    Not used if the data is evenly spaced.
 *    Array containing the indepdent variable, e.g. Time
 * @param yarray 
 *    Array containing the dependent variable, e.g. Amplitude
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Succces
 *    - ERROR_OVERWRITE_FLAG_IS_OFF
 *
 * @param kname_s 
 *    Length of \p kname
 *
 * @date   2007:    Created from wsac0() by adding a routine to update the distance and azimuth
 * @date   961031:  ninf and nhst were changed to norid and nevid for
 *                  compatability with the CSS format.  maf 961031
 * @date   870513:  Changed call to wrtxtd to wrtmsg.
 * @date   840118:  Deleted call to ZTRUNC.
 * @date   830125:  Changes due to modified header common block.
 * @date   820118:  Added logic to truncate file before closing.
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   800821:  Original version [Prime].
 *
 */
void
wsac3(char *kname, float *xarray, float *yarray, int *nerr, int kname_s) {

    wsac0(kname, xarray, yarray, nerr, kname_s);
    return;
}

/* Wrapper to make the function more convenient for FORTRAN programmers. */

void
wsac3_(char *kname, float *xarray, float *yarray, int *nerr, int kname_s) {
    wsac0(kname, xarray, yarray, nerr, kname_s);
}

void
wsac3__(char *kname, float *xarray, float *yarray, int *nerr, int kname_s) {
    wsac0(kname, xarray, yarray, nerr, kname_s);
}

void
update_distaz(sac * s) {
    float d,a,b,g;
    int ndaerr = 0;
    DEBUG("\n");
    if (s->h->lcalda && s->h->stla != SAC_FLOAT_UNDEFINED &&
        s->h->stlo != SAC_FLOAT_UNDEFINED && s->h->evla != SAC_FLOAT_UNDEFINED
        && s->h->evlo != SAC_FLOAT_UNDEFINED) {
        DEBUG("compute\n");
        /* These temporary values are necessary as distaz() will not
         * calculate an output if the input is < 0
         */
        d = a = b = g = 0;
        distaz(s->h->evla, s->h->evlo,
               (float *) &s->h->stla, (float *) &s->h->stlo,
               1,
               (float *) &d, (float *) &a, (float *) &b, (float *) &g,
               &ndaerr);
        s->h->dist  = d;
        s->h->gcarc = g;
        s->h->az    = a;
        s->h->baz   = b;
        DEBUG("done\n");
        DEBUG("dist: %.2f gcarc: %.2f\n", s->h->dist, s->h->gcarc);
        if (s->h->evla == s->h->stla && s->h->evlo == s->h->stlo) {
            s->h->az = 0;
            s->h->baz = 0;
        }
        if (ndaerr) {
            s->h->dist = SAC_FLOAT_UNDEFINED;
            s->h->az = SAC_FLOAT_UNDEFINED;
            s->h->baz = SAC_FLOAT_UNDEFINED;
            s->h->gcarc = SAC_FLOAT_UNDEFINED;
        }
    }
    return;
}
