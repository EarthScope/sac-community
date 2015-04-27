/** 
 * @file   calcBeamOffsets.c
 * 
 * @brief  Calculate Beam Offsets
 * 
 */
#include <string.h>
#include <math.h>

#include "fks.h"
#include "bool.h"
#include "hdr.h"

#include "dff.h"
#include "amf.h"

#define PI M_PI

#define	MXLENB	40000

void cascade();
int isInfoThere();
void refOffsets();
void userOffsets();
void eventOffsets();

void
calcBeamOffsets(int ns, int elevc, float *xr, float *yr, float *zr, int *nerr) {
    sac *s;

    /* ====================================================================
     * PURPOSE: to compute x, y and (future) z offsets from reference
     *          station, for use in beam calculation.  The first station
     *          in the list is assumed to be the reference station, unless
     *          the reference option is set.
     *=====================================================================
     * INPUT ARGUMENTS:
     *      ns:    Number of stations.
     *=====================================================================
     * OUTPUT ARGUMENTS:
     *      xr:    x offset.
     *      yr:    y offset.
     *      zr:    z offset.
     *    nerr:    Error flag. Set to 0 if no error occurred.
     *             Potential error numbers:
     *=====================================================================
     * MODULE/LEVEL:   fks/2
     *=====================================================================
     * GLOBAL INPUT:  (to be updated)
     *    dfm:     ndfl
     *    hdr:     delta, begin
     *=====================================================================
     * GLOBAL OUTPUT:
     *    dfm:     ndfl
     *=====================================================================
     * SUBROUTINES CALLED: (to be updated)
     *    saclib: 
     *=====================================================================
     * LOCAL VARIABLES:
     *=====================================================================
     * MODIFICATION HISTORY:
     *    970306:  Entirely rewritten.  calcBeamOffsets() no longer calls
     *             calcoffsets().  It allows the user to specify the method
     *             of determining offsets.  If the user doesn't specify,
     *             there is a well defined set of rules to determine the 
     *             method.  maf
     *    970207:  Original version (copied from calcoffsets).  maf
     *=====================================================================
     * DOCUMENTED/REVIEWED:  
     *=====================================================================  */
    *nerr = 0;

    switch (cmfks.flagOffset) {
        case OCASCADE:
            cascade(ns, elevc, xr, yr, zr, nerr);

            break;

        case OREFERENCE:
            if (cmfks.lReference)
                refOffsets(ns, cmfks.rReference, xr, yr, zr, nerr);
            else
                *nerr = 5303;

            break;

        case OUSER:
            if (isInfoThere(ns, elevc, nerr) & IUSR)
                userOffsets(ns, xr, yr, zr, nerr);
            else
                *nerr = 5304;

            break;

        case OSTATION:
            if (isInfoThere(ns, elevc, nerr) & ISTATION) {
                float reference[3];
/*		    int ndx1 , ndx2 , idummy ;		 */
                if (!(s = sacget(0, FALSE, nerr))) {
                    return;
                }
                //getfil( 1, FALSE, &ndx1, &ndx2, &idummy, nerr);

                reference[0] = s->h->stla;
                reference[1] = s->h->stlo;
                reference[2] = s->h->stel;

                refOffsets(ns, reference, xr, yr, zr, nerr);
            } else
                *nerr = 5305;

            break;

        case OEVENT:
            if (isInfoThere(ns, elevc, nerr) & IEVENT)
                eventOffsets(ns, xr, yr, zr, nerr);
            else
                *nerr = 5306;

            break;

        default:
            *nerr = 5307;

            break;

    }                           /* end switch */

}                               /* end calcBeamOffsets */

void
cascade(int ns, int elevc, float *xr, float *yr, float *zr, int *nerr) {

    int availableInfo;
    sac *s;
    /* If Reference option is set, use reference lat and lon to calculate 
       x and y offsets */
    if (cmfks.lReference) {
        refOffsets(ns, cmfks.rReference, xr, yr, zr, nerr);

        return;
    }
    /* end if ( cmfks.lReference ) */
    availableInfo = isInfoThere(ns, elevc, nerr);

    /* - Below are three methods of determining offsets, use the one
       set in the OFFSETS option, or if none were set, use the first
       method for which all the information is available. */

    /* Get offsets from USER7, -8, and -9 */
    if (availableInfo & IUSR) {
        userOffsets(ns, xr, yr, zr, nerr);

        return;
    }

    /* Calculate offsets with respect to the first station location. */
    if (availableInfo & ISTATION) {
        float reference[3];
        if (!(s = sacget(0, FALSE, nerr))) {
            return;
        }
        //getfil( 1, FALSE, &ndx1, &ndx2, &idummy, nerr);

        reference[0] = s->h->stla;
        reference[1] = s->h->stlo;
        reference[2] = s->h->stel;

        refOffsets(ns, reference, xr, yr, zr, nerr);

        return;
    }

    /* Calculate offsets with respect to the first event location */
    if (availableInfo & IEVENT) {
        eventOffsets(ns, xr, yr, zr, nerr);

        return;
    }

    /* If we get to here, we didn't have the information we needed to 
       make the beam.  Pass back an error */
    *nerr = 5302;
}                               /* end cascade */

int
isInfoThere(int nFiles, int elevc, int *nerr) {
    int jdfl, returnValue = 0, luser = TRUE, lstation = TRUE, levent = TRUE;
    sac *s;
    for (jdfl = 1; jdfl <= nFiles; jdfl++) {
        if (!(s = sacget(jdfl - 1, FALSE, nerr))) {
            return 0;
        }
        //getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);

        if ((s->h->user7 == SAC_FLOAT_UNDEFINED) ||
            (s->h->user8 == SAC_FLOAT_UNDEFINED) || (elevc &&
                                                     s->h->user9 ==
                                                     SAC_FLOAT_UNDEFINED))
            luser = FALSE;
        if ((s->h->stla == SAC_FLOAT_UNDEFINED) ||
            (s->h->stlo == SAC_FLOAT_UNDEFINED) || (elevc &&
                                                    s->h->stel ==
                                                    SAC_FLOAT_UNDEFINED))
            lstation = FALSE;
        if ((s->h->evla == SAC_FLOAT_UNDEFINED) ||
            (s->h->evlo == SAC_FLOAT_UNDEFINED) || (elevc &&
                                                    s->h->evel ==
                                                    SAC_FLOAT_UNDEFINED))
            levent = FALSE;
    }

    if (luser)
        returnValue += IUSR;
    if (lstation)
        returnValue += ISTATION;
    if (levent)
        returnValue += IEVENT;

    return returnValue;
}                               /* end isInfoThere */

void
refOffsets(int nFiles, float *referencePosition, float *xr, float *yr,
           float *zr, int *nerr) {
    int jdfl;
    float dlat, dlon, avlat;
    sac *s;
    for (jdfl = 1; jdfl <= nFiles; jdfl++) {
        if (!(s = sacget(jdfl - 1, FALSE, nerr))) {
            return;
        }
        //getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);

        dlat = s->h->stla - referencePosition[0];
        dlon = s->h->stlo - referencePosition[1];
        avlat = (referencePosition[0] + s->h->stla) / 2.0;
        xr[jdfl - 1] = 111.19 * dlon * cos(PI * avlat / 180.0);
        yr[jdfl - 1] = 111.19 * dlat;
        zr[jdfl - 1] = referencePosition[2] - s->h->stel;
        /* seismological note:  elevation is subtracted
           in the opposite order of latitude and
           longitude; this signifies that in the Z
           direction, down is positive.  */
    }                           /* end for */

}                               /* end refOffsets */

void
userOffsets(int nFiles, float *xr, float *yr, float *zr, int *nerr) {

    int jdfl;
    sac *s;
    for (jdfl = 1; jdfl <= nFiles; jdfl++) {
        if (!(s = sacget(jdfl - 1, FALSE, nerr))) {
            return;
        }
        //getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);

        xr[jdfl - 1] = s->h->user7;
        yr[jdfl - 1] = s->h->user8;
        zr[jdfl - 1] = s->h->user9;     /* not used at present */
    }
}

void
eventOffsets(int nFiles, float *xr, float *yr, float *zr, int *nerr) {
    int jdfl;
    float dlat, dlon, avlat, reflat, reflon, refel;
    sac *s;
    for (jdfl = 1; jdfl <= nFiles; jdfl++) {
        if (!(s = sacget(jdfl - 1, FALSE, nerr))) {
            return;
        }
        //getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);

        if (jdfl == 1) {
            xr[jdfl - 1] = 0.0;
            yr[jdfl - 1] = 0.0;
            zr[jdfl - 1] = 0.0;
            reflat = s->h->evla;
            reflon = s->h->evlo;
            refel = s->h->evel;
        } else {
            dlat = s->h->evla - reflat;
            dlon = s->h->evlo - reflon;
            avlat = (reflat + s->h->evla) / 2.0;
            xr[jdfl - 1] = 111.19 * dlon * cos(PI * avlat / 180.0);
            yr[jdfl - 1] = 111.19 * dlat;
            zr[jdfl - 1] = refel - s->h->evel;
            /* seismological note:  elevation is subtracted
               in the opposite order of latitude and
               longitude; this signifies that in the Z
               direction, down is positive.  */
        }
    }
}                               /* end eventOffsets */
