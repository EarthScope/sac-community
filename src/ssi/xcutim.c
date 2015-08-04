
#include <string.h>
#include <ctype.h>

#include "ssi.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "sacIO/sacIO.h"
#include "smDataIO.h"
#include "cssListOps/cssListOps.h"
#include "sm_gc.h"

#include "msg.h"
#include "bot.h"
#include "cpf.h"
#include "dff.h"

#include "array.h"

#define MAXPAIRS 10

DFM_EXTERN

sac **cut_data;
sac *cut_file;
void sacpop_no_free();

void
xcutim(int *nerr) {
    /* declare variables */
    char kcutSave[2][9], defaultWorksetName[] = "workset01", *worksetName =
        NULL;
    double dtmp[2];
    int idx, jdx, jdfl, nBounds, nOriginalFiles = saclen(), nSacFiles =
        0, refTimeType = IB;
    sac *s;
    int i, k;
    struct SACheader **header;
    struct SACheader *h;
    const int charsInBase = 9;

    int lname = FALSE, lnotused, lcutSave;
    const int lcuttrue = TRUE;

    float ocutSave[2];

    double refTime;             /* subract this from the picks. */

    DBlist tree;

    struct cutPair {
        char kbase[2][9];
        float offset[2];
    } bounds[MAXPAIRS];

    header = xarray_new('p');
    /* save current global values */
    lcutSave = cmdfm.lcut;
    ocutSave[0] = cmdfm.ocut[0];
    ocutSave[1] = cmdfm.ocut[1];
    strcpy(kcutSave[0], kmdfm.kcut[0]);
    strcpy(kcutSave[1], kmdfm.kcut[1]);

    /* PARSING PHASE */
    /* -- "COMMIT|RECALLTRACE|ROLLBACK":
       how to treat existing data */
    if (lckeyExact("COMMIT", 7))
        cmdfm.icomORroll = COMMIT;
    else if (lckeyExact("RECALLTRACE", 12))
        cmdfm.icomORroll = RECALL;
    else if (lckeyExact("RECALL", 7))
        cmdfm.icomORroll = RECALL;
    else if (lckeyExact("ROLLBACK", 9))
        cmdfm.icomORroll = ROLLBACK;

    /* Get the limits of the cut. */
    for (nBounds = 0; nBounds < MAXPAIRS; nBounds++) {
        if (!lcrtw(&lnotused, (bounds[nBounds].kbase[0]), charsInBase, dtmp)) {
            break;
        } else {
            bounds[nBounds].offset[0] = (float) dtmp[0];
            bounds[nBounds].offset[1] = (float) dtmp[1];
        }

        if (!strcmp(bounds[nBounds].kbase[0], "A       ") &&
            !strcmp(bounds[nBounds].kbase[1], "F       ")) {
            break;
        }
    }                           /* end for */

    /* CHECKING PHASE */

    /* - Check for null data file list. */
    vflist(nerr);
    if (*nerr != 0)
        return;

    /* - Check to make sure all files are evenly spaced time series files. */
    vfeven(nerr);
    if (*nerr != 0)
        return;

    /* Check that cut point information is of the proper form. */
    for (idx = 0; idx < nBounds; idx++) {
        for (jdx = 0; jdx < 2; jdx++) {
            char *test = &(bounds[idx].kbase[jdx][0]);

            if (*test != 'A' && /* first arrival */
                *test != 'B' && /* begin time of trace */
                *test != 'E' && /* end time of trace */
                *test != 'F' && /* end of event */
                *test != 'O' && /* origin */
                *test != 'T' && /* zero seconds */
                *test != 'Z') {
                *nerr = 1405;
                return;
            }
            if (*test == 'T') {
                if (!isdigit(*(test + 1))) {
                    *nerr = 1405;
                    return;
                }
                if (strcmp(test + 2, "      ")) {
                    *nerr = 1405;
                    return;
                }
            } /* end if ( *test == 'T' ) */
            else {
                if (strcmp(test + 1, "       ")) {
                    *nerr = 1405;
                    return;
                }
            }
        }
    }                           /* end for ( idx ) */

    /* EXECUTION PHASE */

    /* - Commit or rollback data according to lmore and cmdfm.icomORroll */
    alignFiles(nerr);
    if (*nerr)
        return;

    /* Get necessary SeisMgr information */
    tree = smGetDefaultTree();

    /* Remove all waveforms from Sacmem */
    deleteAllSacFiles(nerr, FALSE);     /* don't delete file names */
    if (*nerr)
        return;

    cut_data = xarray_new('p');
    for (i = 0; i < saclen(); i++) {
        if (!(s = sacget(i, TRUE, nerr))) {
            *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
            goto L_ERROR;
        }
        cut_data = xarray_append(cut_data, s);
    }
    while (saclen() > 0) {
        sacpop_no_free();
    }

    /* set logical cut to TRUE */
    cmdfm.lcut = TRUE;

    /* Loop between pairs of cut points */
    for (idx = 0; idx < nBounds; idx++) {
        struct wfdiscList *wfL = NULL;

        /* reset cut parameters. */
        strcpy(kmdfm.kcut[0], bounds[idx].kbase[0]);
        strcpy(kmdfm.kcut[1], bounds[idx].kbase[1]);
        cmdfm.ocut[0] = bounds[idx].offset[0];
        cmdfm.ocut[1] = bounds[idx].offset[1];

        /* Loop between waveforms in SeisMgr */
        k = 0;
        do {
            /* Get next waveform. */
            if (!(wfL = dblNextTableInstance(wfL, tree, dbl_LIST_WFDISC)))
                break;
            cut_file = cut_data[k];
            k++;
            /* Get the header to go with the waveform */
            h = (struct SACheader *) malloc(sizeof(struct SACheader));
            header = xarray_append(header, h);
            sacHeaderFromCSS(tree, header[nSacFiles], wfL, refTimeType,
                             &refTime, cmdfm.nMagSpec);

            nSacFiles++;

            if (!lname && nSacFiles > nOriginalFiles)
                lname = TRUE;

            /* Create a SAC file, fill the header and waveform. */
            CSStoSAC(nSacFiles, header[nSacFiles - 1], wfL->seis, lname,
                     lcuttrue, nerr);
            if (*nerr) {
                setmsg("WARNING", 1402);
                outmsg();
                clrmsg();
                *nerr = 0;
            }
        } while (wfL);          /* End loop between waveforms in SeisMgr */

    }                           /* End loop between pairs of cut points. */

    /* Set global number of files */

    /* Remove all waveforms from SeisMgr */
    /*smDeleteDefaultTree() ; */
    worksetName = smGetDefaultWorksetName();
    if (worksetName)
        smDeleteWorksetByName(worksetName);
    else
        worksetName = defaultWorksetName;

    /* Read each waveform from Sacmem back to SeisMgr */
    for (jdfl = 1; jdfl <= nSacFiles; jdfl++) {
        sacSACdata newData;
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_ERROR;
        }

        if (*nerr) {
            *nerr = 1406;
            goto L_ERROR;
        }

        newData.dataType = header[jdfl - 1]->iftype;
        newData.xarray = s->x;
        newData.yarray = s->y;
        header[jdfl - 1]->b = s->h->b;
        header[jdfl - 1]->e = s->h->e;
        header[jdfl - 1]->npts = s->h->npts;

        sacLoadFromHeaderAndData(header[jdfl - 1], &newData, worksetName, FALSE,
                                 jdfl - 1, TRUE, TRUE);
    }                           /* end for ( jdfl ) */

    /* clean up garbage in SeisMgr */
    tree = smGetDefaultTree();
    gcCollect(tree);

    /* Make sure to reset global cut values whether the command worked or not. */
  L_ERROR:
    cmdfm.lcut = lcutSave;
    cmdfm.ocut[0] = ocutSave[0];
    cmdfm.ocut[1] = ocutSave[1];
    strcpy(kmdfm.kcut[0], kcutSave[0]);
    strcpy(kmdfm.kcut[1], kcutSave[1]);
    for (i = 0; i < (int) xarray_length(header); i++) {
        free(header[i]);
        header[i] = NULL;
    }
    xarray_free(header);

}                               /* end xcutim() */
