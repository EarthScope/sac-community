
#include "gdm.h"
#include "msg.h"

/** 
 * Initialize the memory base color table from a named file
 *
 * @param name
 *    Name of color table to set
 *     - 'default' for default color table.
 *     - 'grayscale' for a gray-scale table.
 *     - 'rainbow' for an interesting color table.
 * @param name_s
 *    Length of \p name
 * @param nentry
 *    Number of entries in the color table on output
 * @param nerr
 *    Error return flag
 *    - 0 on Success
 *    - Non-Zero on rrror
 *      - 2203 values must be between 0.0 and 1.0
 *
 * @date   900803:  Original version taken from part of old setctablename.
 *
 */
void 
initctable(char *name,
           int name_s,
           int *nentry,
           int *nerr)
{
	int j, nbad;

	/* - Read color table from disk file. */
	readctable( name,name_s, MCTSIZE, &cmgdm.ctred[0], &cmgdm.ctgreen[0], 
	 &cmgdm.ctblue[0], (char*)kmgdm.ctname[0],9, nentry, nerr );
	if( *nerr > 0 )
		goto L_8888;
	cmgdm.nctsize = *nentry-1;

	/* - Range check input color table. */

	nbad = 0;
	for( j = 0; j < cmgdm.nctsize; j++ ){
		if( cmgdm.ctred[j] < 0.0 ){
			nbad = nbad + 1;
			cmgdm.ctred[j] = 0.0;
			}
		else if( cmgdm.ctred[j] > 1.0 ){
			nbad = nbad + 1;
			cmgdm.ctred[j] = 1.0;
			}
		if( cmgdm.ctgreen[j] < 0.0 ){
			nbad = nbad + 1;
			cmgdm.ctgreen[j] = 0.0;
			}
		else if( cmgdm.ctgreen[j] > 1.0 ){
			nbad = nbad + 1;
			cmgdm.ctgreen[j] = 1.0;
			}
		if( cmgdm.ctblue[j] < 0.0 ){
			nbad = nbad + 1;
			cmgdm.ctblue[j] = 0.0;
			}
		else if( cmgdm.ctblue[j] > 1.0 ){
			nbad = nbad + 1;
			cmgdm.ctblue[j] = 1.0;
			}
		}

	if( nbad > 0. ){
		*nerr = 2203;
		setmsg( "ERROR", *nerr );
		apimsg( nbad );
		aplmsg( "Values must be in the range 0.0 to 1.0",39 );
		aplmsg( "Name of color table is",23 );
		apcmsg( name,name_s );
		}

L_8888:
	return;

} /* end of function */

