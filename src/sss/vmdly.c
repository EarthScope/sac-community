
#include <string.h>
#include <math.h>

#include "sss.h"
#include "dfm.h"
#include "hdr.h"
#include "bool.h"


#include "msg.h"
#include "clf.h"

void /*FUNCTION*/ vmdly(nerr)
int *nerr;
{
	int lmissd;
	int jdfl;
	float dstsq, t0vmsq, vappsq;
  char *tmp;

	/*=====================================================================
	 * PURPOSE:  To calculate delays for files in stack file list.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  sss/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    dfm:    ndfl
	 *    hdr:    fundef, kdfl
	 *    sss:    dst, ivm, inmo, irefr, vapp, t0vm, tvm
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    sss:    dlyvm
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  setmsg, lnumcl, apcmsg, apimsg
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    lmissd:   Flag used when checking for missing distances. [l]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    881117:  Changed indexing of files in data file list.
	 *    860130:  Fixed bug when there were missing distances.
	 *    840806:  Allowed for "negative" distances in velocity models.
	 *    821207:  Documented subroutine.
	 *    810120:  Changed to output message retrieval from disk.
	 *    790831:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  881117
	 *===================================================================== */
	/* PROCECURE: */
	*nerr = 0;

	/* - Check for traces with missing distances. */

	lmissd = FALSE;
	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
		if( Dst[jdfl] == cmhdr.fundef ){
			if( !lmissd ){
				*nerr = 5104;
				setmsg( "ERROR", *nerr );
				lmissd = TRUE;
				}
            tmp = string_list_get(datafiles, jdfl-1);
            apcmsg2(tmp, strlen(tmp)+1);
			}
		}
	if( lmissd )
		goto L_8888;

	/* - Calculate delays. */

	/* -- Normal moveout delays. */
	if( Ivm[1] == cmsss.inmo ){
		vappsq = Vapp[1]*Vapp[1];
		t0vmsq = T0vm[1]*T0vm[1];
		for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
			dstsq = Dst[jdfl]*Dst[jdfl];
			Dlyvm[jdfl] = cmsss.tvm[0][0] - sqrt( t0vmsq + dstsq/vappsq );
			}

		/* -- Refracted wave delays. */
		}
	else if( Ivm[1] == cmsss.irefr ){
		for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
			Dlyvm[jdfl] = cmsss.tvm[0][0] - T0vm[1] - fabs( Dst[jdfl] )/
			 Vapp[1];
			}

		}
	else{
		*nerr = 5110;
		setmsg( "ERROR", *nerr );
		apimsg( Ivm[1] );
		goto L_8888;
		}

L_8888:
	return;

} /* end of function */

