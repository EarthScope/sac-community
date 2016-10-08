/** 
 * @file   xwritenn.c
 * 
 * @brief  Execute WRITENN
 * 
 */

#include "nnm.h"
#include "cpf.h"
#include "co.h"
#include "msg.h"
#include "dfm.h"
#include "bool.h"
#include "amf.h"
#include "hdr.h"

#include "dff.h"

struct t_kmnnm kmnnm;
struct t_cmnnm cmnnm;

/** 
 * Exectue WRITENN which writes data files in a neural net format
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   890306:  Original version.
 *
 */
void
xwritenn(int *nerr) {

    int jdfl, nlenheader, nlocdisk, nun;
    sac *s;
    *nerr = 0;

    while (lcmore(nerr)) {

        /* -- "filename":  name of neural net file to write. */
        if (lcchar(kmnnm.kwritenn, sizeof(kmnnm.kwritenn))) {   /* do nothing */
        }

        /* -- Bad syntax. */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    if (*nerr != 0)
        goto L_8888;

    /* CHECKING PHASE: */
    /* - Check for null data file list. */
    if (saclen() <= 0) {
        *nerr = 1301;
        setmsg("ERROR", *nerr);
        goto L_8888;
    }

    /* EXECUTION PHASE: */

    /* - Start filling in header values. */

    cmnnm.numpoints = 0;
    cmnnm.numfiles = saclen();
    nlenheader = 2 + cmnnm.numfiles;

    /* - Create data file and write dummy header into it. */

    znfile(&nun, kmnnm.kwritenn, MCPFN + 1, nerr);
    if (*nerr != 0)
        goto L_8888;

    nlocdisk = 0;
    zwabs((int *) &nun, (char *) (&cmnnm.numpoints), nlenheader,
          (int *) &nlocdisk, (int *) nerr);
    if (*nerr != 0)
        goto L_8888;
    nlocdisk = nlenheader;

    /* - Write each file in memory to disk. */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

        /* -- Check number of data points. */
        if (jdfl == 1) {
            cmnnm.numpoints = s->h->npts;
        } else if (s->h->npts != cmnnm.numpoints) {
            *nerr = 2801;
            setmsg("ERROR", *nerr);
            goto L_8888;
        }

        /* -- Write the data. */
        zwabs((int *) &nun, (char *) s->y, cmnnm.numpoints, (int *) &nlocdisk,
              (int *) nerr);
        if (*nerr != 0)
            goto L_8888;
        nlocdisk = nlocdisk + cmnnm.numpoints;

    }

    /* - Rewrite header now that it is complete. */

    nlocdisk = 0;
    zwabs((int *) &nun, (char *) (&cmnnm.numpoints), nlenheader,
          (int *) &nlocdisk, (int *) nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Close file. */

    zclose(&nun, nerr);

  L_8888:
    return;

}                               /* end of function */
