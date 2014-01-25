/** 
 * @file   inifks.c
 * 
 * @brief  Initialize FKS
 * 
 */

#include <string.h>

#include "fks.h"
#include "bool.h"


#include "co.h"

/** 
 * Initialize the FKS Module
 * 
 */
void 
inifks() {

	/* - Variable initialization for xmap. */
	strcpy( cmfks.kmaptype, "ARRAY   " );


	/* - Variable initialization for xbbfk. */

	cmfks.lkfilter = FALSE;
	cmfks.lknorm = FALSE;
	cmfks.leps = TRUE;
	cmfks.eps = .01;
	strcpy( cmfks.kfkmethod, "PDS " );
	cmfks.neigenmlm = 0;
	cmfks.neigenmus = 0;
	cmfks.lexp = FALSE;
	cmfks.iexp = 1;

	cmfks.rkhor = 1.0;
	cmfks.iazs = 90;
	cmfks.iwvs = 32;
	cmfks.nlcontour = 11;
	strcpy( cmfks.scalng, "LINEAR  " );
	fstrncpy( cmfks.ktitle, 80, "$", 1 );

	cmfks.lwr = FALSE;
	fstrncpy( cmfks.kofbbfk, MCPFN, "bbfk ", 5 );
	cmfks.idimsq = 100;


	/* - Variable initialization for xbeam. */

	cmfks.bear = 90.0;
	cmfks.veloc = 9.0;
	cmfks.anginc = 33.;
	cmfks.survel = 6.;
	cmfks.ctrx = 0.;
	cmfks.ctry = 0.;
	cmfks.ctrz = 0.;
	cmfks.nctr = 3;
	fstrncpy( cmfks.kofbeam, MCPFN, "beam ", 5 );

	/* Reference information.  maf 970207 */
	cmfks.lReference = FALSE ;
	cmfks.nReference = 0 ;
	cmfks.rReference[0] = 0 ;
	cmfks.rReference[1] = 0 ;
	cmfks.rReference[2] = 0 ;

	/* Offset information. maf 970306 */
	cmfks.flagOffset = OCASCADE ;
	strcpy ( cmfks.koffset[0] , "CASCADE " ) ;
	strcpy ( cmfks.koffset[1] , "REF     " ) ;
	strcpy ( cmfks.koffset[2] , "USER    " ) ;
	strcpy ( cmfks.koffset[3] , "STATION " ) ;
	strcpy ( cmfks.koffset[4] , "EVENT   " ) ;

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    970306:  Added Offset information.  maf
	 *    970207:  Added Reference information.  maf
	 *    910506:  Original version.
	 *===================================================================== */

} /* end of function */

