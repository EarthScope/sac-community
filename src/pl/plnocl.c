
#include <stdio.h>

#include "pl.h"
#include "gem.h"


#include "gtm.h"
#include "gdm.h"

void /*FUNCTION*/ plnocl(xarray, yarray, number, lnewdp)
float xarray[], yarray[];
int *number;
int lnewdp;
{
	int iline;

	/*=====================================================================
	 * PURPOSE: To display a set of data points without clipping.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    XARRAY:  Array of x data points.
	 *    YARRAY:  Array of y data points.
	 *    NUMBER:  Number of x-y data points.
	 *    LNEWDP:  Logical flag.  Set to .TRUE. if this is a new set of data
	 *             points. Set to .FALSE. if a continuation of the last set.
	 *=====================================================================
	 * MODULE/LEVEL:  GEM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GEM:     LLINE, ICLINE, LSYM, ISYM, ISOLID
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  POLYLINE, SETLINESTYLE, SYMBOL
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Connect the data points if requested. */

	if( cmgem.lline && cmgem.icline > 0 ) {
		polyline( xarray, yarray, number );
                stroke();
        }
	/* - Plot scaled symbols if requested. */

	if( cmgem.lsym && cmgem.isym > 0 ){
                iline = cmgdm.iline;
		setlinestyle( LINE_STYLE_SOLID );
		setlinewidth( cmgem.isymwidth );
		symbol( xarray, yarray, *number, lnewdp );
		setlinestyle( iline );
		setlinewidth( cmgem.iwidth );
		}

       
	return;

	/*=====================================================================
	 * MODIFICATION HISTORY:
	 *    920526:  Added line-width.
	 *    910301:  Changed iline to icline.
	 *    860113:  Fixed bug involving linestyle and symbol plotting.
	 *    830926:  Added logic to disable line and symbol plotting
	 *             if current attribute is zero.
	 *    830502:  Fixed bug involving symbols and linestyles.
	 *    811223:  Original version extracted from PLDTA.
	 *===================================================================== */

} /* end of function */

