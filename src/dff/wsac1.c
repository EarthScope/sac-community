/** 
 * @file   wsac1.c
 * 
 * @brief  Write a SAC file
 * 
 */
#include "amf.h"
#include "dff.h"
#include "hdr.h"
#include "msg.h"
#include "bool.h"
#include "SacHeader.h"

/** 
 * Write an evenly spaced SAC file 
 * 
 * @param kname 
 *    Name of file to write
 * @param yarray 
 *    Array containing dependent variable, e.g. Amplitude
 * @param nlen 
 *    Length of \p yarray
 * @param beg 
 *    Beginning value of the independent variable, "B header value"
 * @param del 
 *    Samplint interval of the independent variable, "Delta header value"
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
wsac1(char *kname, float *yarray, int *nlen, float *beg, float *del, int *nerr,
      int kname_s) {

    float xdummy;
    sac *s;
    *nerr = 0;

    xdummy = 0;

    /* - Initialize all header fields to their default values. */
    s = sac_new();
    s->m->filename = fstrdup(kname, kname_s);
    sacput(s);

    /* - Set up the header fields passed by the calling program. */
    s->h->npts = *nlen;
    s->h->delta = *del;
    s->h->b = *beg;
    sac_be(s);
    s->h->leven = TRUE;

    /* - Write the file to disk. */
    wsac0(kname, &xdummy, yarray, nerr, kname_s);
    return;
}

/* Wrapper to make the function more convenient for FORTRAN programmers. */

void
wsac1_(char *kname, float *yarray, int *nlen, float *beg, float *del, int *nerr,
       int kname_s) {
    wsac1(kname, yarray, nlen, beg, del, nerr, kname_s);
}

void
wsac1__(char *kname, float *yarray, int *nlen, float *beg, float *del,
        int *nerr, int kname_s) {
    wsac1(kname, yarray, nlen, beg, del, nerr, kname_s);
}
