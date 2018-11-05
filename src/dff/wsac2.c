/** 
 * @file   wsac2.c
 * 
 * @brief  Write a SAC file
 * 
 */
#include "amf.h"
#include "dff.h"
#include "hdr.h"
#include "bool.h"
#include "msg.h"
#include "errors.h"
#include "SacHeader.h"

#include <fstr.h>

/** 
 * Write an unevenly spaced or spectral SAC file
 * 
 * @param kname 
 *    Name of file to write
 * @param yarray 
 *    Array containing the dependent variable, e.g. Amplitude
 * @param nlen 
 *    Length of \p yarray and \p xarray
 * @param xarray 
 *    Array containing the independent variable, e.g. Time
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 * @param kname_s 
 *    Length of \p kname
 *
 * @date   870902:  Added calls to INILHF and INIMSG as part of initialization.
 * @date   870513:  Changed call to wrtxtd to wrtmsg.
 * @date   800820:  Original version.
 *
 */
void
wsac2(char *kname, float *yarray, int *nlen, float *xarray, int *nerr,
      int kname_s) {

    sac *s;
    *nerr = 0;
    if (*nlen <= 0) {
        *nerr = ERROR_WRITING_FILE;
        return;
    }

    s = sac_new();
    s->m->filename = fstrdup(kname, kname_s);
    sacput(s);

    /* - Set up the header fields passed by the calling program. */
    s->h->npts = *nlen;
    s->h->leven = FALSE;
    sac_be(s);

    /* - Write the file to disk. */
    wsac0(kname, xarray, yarray, nerr, kname_s);

    return;
}

/* Wrapper to make the function more convenient for FORTRAN programmers. */

void
wsac2_(char *kname, float *yarray, int *nlen, float *xarray, int *nerr,
       int kname_s) {
    wsac2(kname, yarray, nlen, xarray, nerr, kname_s);
}

void
wsac2__(char *kname, float *yarray, int *nlen, float *xarray, int *nerr,
        int kname_s) {
    wsac2(kname, yarray, nlen, xarray, nerr, kname_s);
}
