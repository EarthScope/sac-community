
#include <stdio.h>

#include "ssi.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"
#include "sm_gc.h"

#include "sacIO/sacIO.h"
#include "smDataIO.h"
#include "cssListOps/dblErrors.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"

void
SeisMgrToSac(DBlist tree, int lname, int *nerr, int Verbose, int lcutnow,
             int takeEvid) {
    /* Declare Variables. */
    int refTimeType = cmdfm.lshift ? IO : IB;
    double refTime;             /* subract this from the picks. */
    int dots = 0;
    int originalNDFL;

    MagType mType = cmdfm.nMagSpec;
    struct wfdiscList *wfL = NULL;
    struct SACheader header;
    *nerr = 0;

    /* Initialize SeisMgr error handler */
    dblClearErrorList();

    /* Delete previous Sac data. */
    originalNDFL = deleteAllSacFiles(nerr, lname);

    if (*nerr) {
        if (originalNDFL < saclen())
            *nerr = 1401;
        else
            *nerr = 1402;
        return;
    }

    /* Loop through waveforms in the tree, write them into Sac one at a time. */
    do {
        int lcuttemp = lcutnow; /* to further discern when to cut */

        /* Get next waveform. */
        if (!(wfL = dblNextTableInstance(wfL, tree, dbl_LIST_WFDISC)))
            break;

        /* Progress indicator in verbose mode. */
        if (Verbose) {
            putchar('.');
            dots++;
            if (dots == 80) {
                putchar('\n');
                dots = 0;
            }
            fflush(stdout);
        }

        memset(&header, 0, sizeof(struct SACheader));
        /* Get the header to go with the waveform */
        sacHeaderFromCSS(tree, &header, wfL, refTimeType, &refTime, mType);

        /* Get picks according to the preferences file and
           pickauth and pickphase commands. */

        if (cmdfm.lpref) {
            prefPicksToHeader(&header, saclen(), wfL->element, tree, refTime,
                              nerr);

            if (*nerr) {
                *nerr = 1401;
                break;
            }
        }
        /* end if( cmdfm.lpref ) */
        if (!lname && saclen() > originalNDFL)
            lname = TRUE;

        /*if ( saclen() <= originalNDFL ) */
        if (!lname)
            lcuttemp = FALSE;

        /* Create a SAC file, fill the header and waveform. */
        CSStoSAC(saclen(), &header, wfL->seis, lname, lcuttemp, nerr);
        if (*nerr) {
            *nerr = 1402;
            break;
        }

        /* If the waveform was cut on the way into SAC,
           send the cut version back to SeisMgr. */
        if (cmdfm.lcut && lcuttemp) {
            sacSACdata newData;
            sac *s;
            if (!(s = sacget(saclen() - 1, TRUE, nerr))) {
                return;
            }

            newData.dataType = header.iftype;
            newData.xarray = s->x;
            newData.yarray = s->y;
            header.b = s->h->b;
            header.e = s->h->e;
            header.npts = s->h->npts;

            sacLoadFromHeaderAndData(&header, &newData,
                                     smGetDefaultWorksetName(), FALSE,
                                     saclen() - 1, TRUE, takeEvid);
        }
    } while (wfL);

    if (Verbose)
        printf("\n");

    gcCollect(tree);

}                               /* end SeisMgrToSac */
