/** 
 * @file   xdeletechannel.c
 * 
 * @brief  Delete a data file 
 * 
 */

#include <string.h>

#include "dfm.h"
#include "amf.h"

#include "cssListOps/cssStrucs.h"
#include "cssListOps/cssListStrucs.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "smDataIO.h"

#include "ucf.h"
#include "ssi.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"
#include "array.h"
#include "errors.h"

#include <fstr.h>

DFM_EXTERN

/** 
 * Execute the command DELETECHANNEL which deletes one or more channels from
 *    the file list
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   970203:  Original version copied from xdeletestack.  maf
 *
 */
void
xdeletechannel(int *nerr) {

    char kfile[MCPFN + 1];
    int lincr;

    int lall = FALSE;
    int *idel, jdel, jdfl;
    int ndel;
    int first, last;

    DBlist tree;
    sac *s;
    tree = smGetDefaultTree();

    *nerr = 0;
    ndel = 0;

    idel = xarray_new_with_len('i', saclen());

    memset(kfile, 0, sizeof(kfile));
    /* - Loop on each token in command: */
    while (lcmore(nerr)) {

        if (lckey("ALL#$", 6))  /* ALL option added. maf 970902 */
            lall = TRUE;

        /* -- "n":  the number of a file. */
        else if (lcint(&jdfl)) {
            if (jdfl < 1 || jdfl > saclen()) {
                *nerr = 5107;
                setmsg("ERROR", *nerr);
                apimsg(jdfl);
                goto L_8888;
            }
            idel[ndel] = jdfl;
            ndel++;
        }

        /* -- "n-m": a range of filenumbers denoted by the first and last 
           numbers in the range separated by a dash (-). */
        else if (lcidi(&first, &last)) {
            if (first < 1 || last > saclen()) {
                *nerr = 5107;
                setmsg("ERROR", *nerr);
                apimsg(first < 1 ? first : last);
                goto L_8888;
            }
            for (jdfl = first; jdfl <= last; jdfl++) {
                idel[ndel] = jdfl;
                ndel++;
            }
        }

        /* -- "filename":  the name of a file. */
        else if (lcchar(kfile, sizeof(kfile))) {
            char *kfile2 = fstrdup(kfile, MCPFN + 1);
            jdfl = 1 + sac_find_filename(kfile2);
            if (jdfl <= 0) {
                *nerr = 5106;
                setmsg("ERROR", *nerr);
                apcmsg(kfile, MCPFN + 1);
                goto L_8888;
            }
            idel[ndel] = jdfl;
            ndel++;
        }

        /* -- Bad syntax. */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }

    }

    /* EXECUTION PHASE: */
    /* - First do case of ALL option not specified */
    if (!lall) {
        /* - Sort the list of file numbers into decreasing order */
        lincr = FALSE;
        sorti(idel, ndel, lincr, idel);

        /* - For each file to be deleted: */

        jdfl = 0;
        for (jdel = 0; jdel < ndel; jdel++) {
            if (idel[jdel] != jdfl) {
                jdfl = idel[jdel];
                /* -- Release memory blocks. */
                sacdel(jdfl - 1);
            }
        }

        for (jdel = 0; jdel < ndel; jdel++)
            idel[jdel]--;

        dblDeleteWfdiscs(tree, (int *) idel, ndel);
    }

    /* Now handle case of ALL option specified.  maf 970902 */
    else {
        /* declare Workset name */
        char *worksetName;

        sacclear();

        /* delete files from SAC memory */
        deleteAllSacFiles(nerr, TRUE);

        /* delete files from SeisMgr regardless of lcommit */
        worksetName = smGetDefaultWorksetName();
        if (worksetName)
            smDeleteWorksetByName(worksetName);

        /* if COMMIT is set, warn user */
        if (lall && cmdfm.lcommit) {
            setmsg("WARNING", 1382);
            outmsg();
            clrmsg();
        }
    }

    /* If all files were deleted, let the user know. */
    if (saclen() < 1)
        printf("\nAll files deleted.\n");

    /* Else make first file in the smaller list current. */
    else if (!(s = sacget(0, FALSE, nerr))) {
        goto L_8888;
    }
    //getfil( 1, FALSE, &ntused, &ntused, &ntused, nerr );
  L_8888:
    xarray_free(idel);
    return;

}
