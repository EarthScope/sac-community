
#include "xyz.h"


#include "co.h"

void /*FUNCTION*/ xyzcleanup()
{
	int _l0, nerr;

	/*=====================================================================
	 * PURPOSE:  To cleanup xyz images and temporary files if necessary.
	 *=====================================================================
	 * MODULE/LEVEL:  xyz/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    xyz:   lcleanup
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:   zsysop
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900310:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900310
	 *===================================================================== */
	/* - If cleanup flag is set, execute the command to delete image windows 
	 *   and destory temporary files. */
	if( cmxyz.lcleanup ){
                _l0 = 20;
		zsysop( "Utahcleanup getsun &",21, &_l0, &nerr );
		if( nerr != 0 )
			goto L_8888;
		}

L_8888:
	return;

} /* end of function */

