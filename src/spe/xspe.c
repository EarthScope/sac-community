
#include "spe.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "msg.h"
#include "exm.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xspe(linit, nerr)
int linit;
int *nerr;
{
	int ndx1, ndx2, nlen, firstPowerOf2;



	/*=====================================================================
	 * PURPOSE:  To initialize the Spectral Estimation Subprocess.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    LINIT:   .TRUE. if this call is to initializize subprocess.
	 *             .FALSE. if it is to set up data storage after a read.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 5002, 1302
	 *=====================================================================
	 * MODULE/LEVEL: SPE/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL, MEMNOW, DELTA
	 *    SPE:     MLNPE, SECPDS
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    SPE:     NDXDAT, NLNDAT, NDXCOR, NDXPE, NDXSPE, NDXAUX,
	 *             LCOR, LSPE, SAMFRQ, NLGPDS
	 *    CLF:     ICL
	 *    EXM:     KPRMT
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  GTOUTM, VFFILE, VFEVEN, GETFIL, ZCHMEM, ZSSUBP,
	 *             ALLAMB, RELAMB, SETCOMLIST, SETPROMPT
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    NDX1:    Index to beginning of data. [i]
	 *    NLEN:    Number of samples in data. [i]
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Make sure there is only one file in DFL and that it is evenly spaced. */

	vflist( nerr );
	if( *nerr != 0 ){
	    cmspe.lfile = FALSE;
	    goto L_8888;
	}

	/* - Make sure that there is only one file in DFL. */

	if( cmdfm.ndfl > 1 ){
	    *nerr = 5002;
	    setmsg( "ERROR", *nerr );
	    cmspe.lfile = FALSE;
	    goto L_8888;
	}

	/* - Check to make sure all files are evenly spaced time series files. */

	vfeven( nerr );
	if( *nerr != 0 ){
	    cmspe.lfile = FALSE;
	    goto L_8888;
	}

	/* - Get the file from memory or disk. */

	getfil( 1, TRUE, &nlen, &ndx1, &ndx2, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Get work space from memory manager. */

	/* Find first power of two greater than or equal to nlen. maf 980527 */
	firstPowerOf2 = MINPOW ;
	while ( firstPowerOf2 < nlen )
	    firstPowerOf2 *= 2 ;

	cmspe.firstPowerOf2 = firstPowerOf2 ;

	allamb( &cmmem, firstPowerOf2 * 2, &cmspe.ndxcor, nerr );
	if( *nerr != 0 )
	    goto L_8888;
	allamb( &cmmem, MLNPE, &cmspe.ndxpe, nerr );
	if( *nerr != 0 )
	    goto L_8888;
	allamb( &cmmem, firstPowerOf2, &cmspe.ndxspe, nerr );
	if( *nerr != 0 )
	    goto L_8888;
	allamb( &cmmem, firstPowerOf2 * 5, &cmspe.ndxaux, nerr );
	if( *nerr != 0 )
	    goto L_8888;

        /* - If this is an initialization call, send confirming message
         *   and change to the proper subprocess command list. */

        if( linit ){
            setmsg( "OUTPUT", 5001 );
            outmsg();
            setcomlist( 2 );
            setprompt( "SAC/SPE>",9 );
        }

	/* - Set some global values to their initial values. */

        cmspe.ndxdat = ndx1;
        cmspe.nlndat = nlen;
	cmspe.lfile = TRUE;
	cmspe.lcor = FALSE;
	cmspe.lspe = FALSE;
	cmspe.samfrq = 1./ *delta;

L_8888:
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    990224:  Reorganized so that if it fails, main mode is preserved
	 *    850801:  Changes due to new memory manager.
	 *    850108:  Added LINIT argument so that this subroutine could
	 *             be used as part of the SPE READ command.
	 *    821129:  Original version (from SPEPRO.)
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850801
	 *===================================================================== */

} /* end of function */

