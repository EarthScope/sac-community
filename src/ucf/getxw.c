/** 
 * @file   getxw.c
 * 
 * @brief  Get x data point from y value
 * 
 */

#include "ucf.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"

/** 
 * Get x data point from current data file given y value
 * 
 * @param ywloc 
 *    Input Y value in world coordinates
 * @param xwloc 
 *    Output x value in world coordinates
 *
 * @date   961219:  Original version; copied from getyw.c,  maf.
 * @date   850617:  getyw.c initial
 *
 */
void
getxw(double ywloc, float *xwloc) {

    int ioffst;
    sac *s;
    /* - Compute index offset into current array. */
    if (!(s = sacget_current())) {
        return;
    }
    ioffst = (ywloc - s->h->b + 0.5 * s->h->delta) / s->h->delta;

    /* - Return corresponding x world coordinate. */
    *xwloc = s->y[ioffst];

    return;
}
