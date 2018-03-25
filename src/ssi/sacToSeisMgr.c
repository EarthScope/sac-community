
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "ssi.h"
#include "hdr.h"
#include "dfm.h"
#include "amf.h"
#include "clf.h"
#include "bool.h"
#include "select.h"
#include "exm.h"
#include "smDataIO.h"
#include "sacIO/sacIO.h"
#include "sm_gc.h"
#include "msg.h"
#include "ucf.h"
#include "dff.h"

#include "cssListOps/dblErrors.h"

DFM_EXTERN
extern int const allHeader;
extern int const eventHeader;

int
Unique(int *array, int size, int wfid) {
    /* Check for a unique wfid */
    int j;
    for (j = 0; j <= size; ++j)
        if (wfid == array[j])
            return 0;
    return 1;
}

int
next_wfid(int *array, int size) {
    /* Set next wfid to max_wfid + 1 */
    int j;
    int tmp = 0;
    for (j = 0; j <= size; ++j) {
        if (array[j] >= tmp)
            tmp = array[j] + 1;
    }
    return tmp;
}

void
sacToSeisMgr(int lnew, int lupdate, int ldata, int *nerr) {
    /* Declare Variables. */
    char *worksetName, defaultWorksetName[] = "workset01";
    int jdfl;
    int takeEvid = FALSE;
    float unused;
    sacSACdata *data;
    int *nwfid_array;
    sac *s;
    struct SACheader header;
    *nerr = 0;
    nwfid_array = NULL;
    data = NULL;

    if (!use_database(OPTION_GET))
        return;

    /* Initialize SeisMgr error handler */
    dblClearErrorList();

    /* Get default workset name */
    worksetName = smGetDefaultWorksetName();

    if (worksetName && lnew) {
        smDeleteWorksetByName(worksetName);
    } else if (!worksetName) {
        lnew = TRUE;
        worksetName = defaultWorksetName;
    }

    /* Be self consistent */
    if (lnew)
        lupdate = FALSE;

    if (lupdate)
        ldata = FALSE;

    /* Allocate sacSACdata */
    if (ldata) {
        data = (sacSACdata *) malloc(sizeof(sacSACdata));
        if (!data) {
            *nerr = 301;
            setmsg("ERROR", *nerr);
            outmsg();
            clrmsg();
            goto L_ERROR;
        }
    }

    if (lnew) {
        if (0 != smCreateEmptyWorkset(worksetName)) {
            *nerr = 301;
            setmsg("ERROR", *nerr);
            outmsg();
            clrmsg();
            goto L_ERROR;
        }
    }

    /* Allocate memory for wfid array */
    nwfid_array = (int *) malloc(saclen() * sizeof(int));
    if (!nwfid_array) {
        *nerr = 301;
        setmsg("ERROR", *nerr);
        outmsg();
        clrmsg();
        goto L_ERROR;
    }

    /* Loop through sac data file list, writing data to the tree. */

    for (jdfl = 0; jdfl < saclen(); jdfl++) {
        int localLdata;
        if (!(s = sacget(jdfl, FALSE, nerr))) {
            goto L_ERROR;
        }

        localLdata = (!s->y) ? FALSE : ldata;

        /* Get next waveform. */
        //getfil ( jdfl+1 , localLdata , &nunused , &ndx1 , &ndx2 , nerr ) ;
        if (*nerr) {
            setmsg("ERROR", *nerr);
            outmsg();
            clrmsg();
            *nerr = 1401;
            goto L_ERROR;
        }

        /* Check for nonunique wfid that are defined */
        nwfid_array[jdfl] = 0;
        if (SAC_INT_DEFINED(s->h->nwfid) &&
            !Unique(nwfid_array, jdfl, s->h->nwfid)) {
            s->h->nwfid = next_wfid(nwfid_array, jdfl);
            nwfid_array[jdfl] = s->h->nwfid;
        } else {
            nwfid_array[jdfl] = s->h->nwfid;
        }

        /* update pertinent information */
        sac_be(s);
        update_distaz(s);

        if (localLdata) {
            extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax,
                   &s->h->depmen);
        }

        /* Disallow undefined kstnm and kcmpnm */
        //if ( uniqueStaAndChan () )

        /* Put the header into the sacHeader struct. */
        memset(&header, 0, sizeof(struct SACheader));
        SacHeaderToDB(&header, lupdate ? eventHeader : allHeader, jdfl + 1);

        /* Put the data into the sacData struct. */
        if (localLdata) {
            data->dataType = s->h->iftype;
            data->yarray = s->y;
            data->xarray = s->x;
        }

        /* determine takeEvid to pass into sacLoadFromHeaderAndData */
        if (!cmdfm.lread || jdfl < cmdfm.nfilesFirst) {
            takeEvid = TRUE;
        } else {
            if (cmdfm.nreadflag == RDB)
                takeEvid = TRUE;

            else if (cmdfm.nreadflag == HIGH) {
                if (cmdfm.ltrust)
                    takeEvid = TRUE;
                else
                    takeEvid = FALSE;
            }

            else {
                takeEvid = FALSE;
            }
        }

        /* Put the data into the tree */
        sacLoadFromHeaderAndData(&header, data, worksetName, 0,
                                 lnew ? -1 : jdfl, localLdata, takeEvid);

    }                           /* end for */

    gcCollect(smGetDefaultTree());

  L_ERROR:
    FREE(nwfid_array);
    FREE(data);

}                               /* end sacToSeisMgr */
