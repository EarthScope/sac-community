
#include <string.h>

#include "gd2.h"

void /*FUNCTION*/ setsgfsize(type, value)
char *type;
double value;
{



	/*=====================================================================
	 * PURPOSE:  To set the plot size of a SAC Graphics File.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    type:    Type of plot size desired [c]
	 *             = 'NORMAL' where the plot size is determined by the 
	 *               conversion program.
	 *             = 'FIXED' where the plot is sized so that the width
	 *               of the current x viewport in inches in specified
	 *               by the second argument.
	 *             = 'SCALED' where the plot is sized so that the width
	 *               of the current x viewport in inches in determined
	 *               by multiplying the second argument by the width of
	 *               the current x world coordinate limits.
	 *             CASE INSENSITIVE; ONLY FIRST CHARACTER NEEDED.
	 *    value:   Fixed size or scaling factor, depending upon the
	 *             value of the first argument. Ignored if 'NORMAL'. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  gd2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gd2:     sizetype, sizevalue
	 *=====================================================================
	 * GLOBAL COUPLING:
	 * - BEGINFRAME2 performs sizing and scaling calculations and
	 *   issues opcode to set size as each SGF is created.
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900310:  Completely changed. Now size is determined from viewport
	 *             not viewspace and scaling option was added.
	 *    850822:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWD:  900310
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Save passed arguments in common block. */
	if( type[0] == 'N' || type[0] == 'n' ){
		strcpy( kmgd2.sizetype, "NORMAL  " );
		}
	else if( type[0] == 'F' || type[0] == 'f' ){
		strcpy( kmgd2.sizetype, "FIXED   " );
		cmgd2.sizevalue = value;
		}
	else if( type[0] == 'S' || type[0] == 's' ){
		strcpy( kmgd2.sizetype, "SCALED  " );
		cmgd2.sizevalue = value;
		}
	else{
		strcpy( kmgd2.sizetype, "NORMAL  " );
		}

       
	return;

} /* end of function */

