
#include <string.h>

#include "sss.h"
#include "dfm.h"
#include "dff.h"
#include "amf.h"
#include "bool.h"

#include "smDataIO.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "msg.h"
#include "co.h"
#include "clf.h"
#include "ucf.h"
#include "cpf.h"
#include "array.h"

void /*FUNCTION*/
xdeletestack(nerr)
     int *nerr;
{
    char kfile[MCPFN + 1];
    int lincr;
    int *idel, jdel, jdfl, jdfl2, jdfl3, ndel;

    DBlist tree;

    tree = smGetDefaultTree();

        /*=====================================================================
	 * PURPOSE:  To execute the DELETESTACK command.  This command deletes
	 *           one or more files from the signal stack.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred. [i]
	 *             Potential error numbers:  1001, 5106, 5107
	 *=====================================================================
	 * MODULE/LEVEL:  sss/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    mcpfn
	 *    dfm:     mdfl
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    dfm:     ndfl, kdfl, ndxhdr, nlndta, ndxdta, ncomp
	 *    sss:     dlyn, dlyt, dlyni, dlyti, ldlyt, ldlyi, wt, dst, lpol
	 *             beginTime, endTime
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp, lcdfl, lcint,
	 *             setmsg, apcmsg, apimsg, sorti
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    idel:    Array of signal stack file numbers to delete. [i]
	 *    ndel:    Length of idel. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    970203:  Added several lines and swapped some array elements
	 *             to fix the bug that it just didn't work.  maf
	 *    960701:  Added begin and end time, maf
	 *    881130:  Fixed bug in handling of data file list.
	 *    881117   Changes due to restructuring data file access methods.
	 *    850812:  Major revision of subprocess.
	 *    821201:  Changed to newest set of parsing and checking functions.
	 *    810120:  Changed to output message retrieval from disk.
	 *    790601:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850812
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;
    ndel = 0;

    idel = xarray_new_with_len('i', saclen());
    /* PARSING PHASE: */

    /* - Loop on each token in command: */

    while (lcmore(nerr)) {

        /* "COMMIT": removes infromation from SeisMgr as well as SAC */
/*	    if( lklog( "COMMIT$",8, &lcommit ) ){
                cmdfm.lcommit = lcommit ;
            }	*/

        /* -- "n":  the number of a file from the signal stack. */
        if (lcint(&jdfl)) {
            if (jdfl < 1 || jdfl > saclen()) {
                *nerr = 5107;
                setmsg("ERROR", *nerr);
                apimsg(jdfl);
                goto L_8888;
            }
            idel[ndel] = jdfl;
            ndel++;
        }

        /* -- "filename":  the name of a file from the signal stack. */
        else if (lcchar(kfile, sizeof(kfile))) {
            char *kfile2 = fstrdup(kfile, -1);
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

    }                           /* end while */

    /* - The above loop is over when one of two conditions has been met:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
     *   (2) All the tokens in the command have been successfully parsed. */

    /* EXECUTION PHASE: */

    /* - Sort the list of file numbers into decreasing order */

    lincr = FALSE;
    sorti(idel, ndel, lincr, idel);

    /* - For each file to be deleted: */

    jdfl = 0;
    for (jdel = 1; jdel <= ndel; jdel++) {
        if (idel[jdel - 1] != jdfl) {
            jdfl = idel[jdel - 1];
            /* -- Release memory blocks. */
            sacdel(jdfl - 1);
            /* -- Move DFM and SSS array variables down.           */
            for (jdfl2 = jdfl; jdfl2 <= (saclen() - 1); jdfl2++) {
                jdfl3 = jdfl2 + 1;
                Dlyt[jdfl2] = Dlyt[jdfl3];
                Dlyti[jdfl2] = Dlyti[jdfl3];
                Dlyn[jdfl2] = Dlyn[jdfl3];
                Dlyni[jdfl2] = Dlyni[jdfl3];
                Wt[jdfl2] = Wt[jdfl3];
                Dst[jdfl2] = Dst[jdfl3];
                Tbegin[jdfl2] = Tbegin[jdfl3];  /* maf 960701 */
                Tend[jdfl2] = Tend[jdfl3];      /* maf 960701 */
                Lpol[jdfl2] = Lpol[jdfl3];

            }

        }                       /* end if( Idel[jdel] != jdfl ) */
    }                           /* end for( jdel = 1; jdel <= ndel; jdel++ ) */

    /* If COMMIT option set, delete files from SeisMgr as well. */
/*	if ( cmdfm.lcommit ) {	*/
    /* decrement elements of idel for the sake of the C array numbering */
    for (jdel = 0; jdel < ndel; jdel++)
        idel[jdel]--;

    dblDeleteWfdiscs(tree, (int *) idel, ndel);
/*	} * end if ( cmdfm.lcommit ) */

  L_8888:
    xarray_free(idel);
    return;

}                               /* end of function */
