
#include "co.h"
#include "mach.h"
#include "gdm.h"
#include "bot.h"

/** 
 * Set color attribute by name
 *
 * @param color
 *    Name of the desired color
 *    If not found, set to the foreground color
 * @param color_s
 *    Length of \p color
 * 
 * @date   861020:  Original version.
 *
 */
void 
setcolorname(char *color,
             int color_s)
{
	char ktest[9];
	int index, nc;

	/* - Determine length of input color name and convert it to upper case. */
	nc = min( indexb( color,color_s ), MCPW );
	upcase( color, nc, ktest,9 );

	/* - Test name versus list of names in default color table.
	 *   Set requested color if found. */

	if( lequal( ktest,9, (char*)kmgdm.ctname[1],9, cmgdm.nctsize, &index ) ){
		setcolor( index );
        } else {
          /* - If not found, set to foreground color. */
          setcolor( cmgdm.nctsize );
        }

}

