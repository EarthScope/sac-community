
#include "gdm.h"
#include "bool.h"

/** 
 * Change the Graphics Text Quality
 *
 * @param kqual
 *    Quality of the text
 *     - 'SOFTWARE' for software quality text.
 *     - 'HARDWARE' for hardware quality text.
 *     - (Only the first character need be entered.)
 *
 * @note  Some of the devices simulate hardware text by using
 *        the simplest software text font (font 1.)   Therefore, if you select
 *        software text after first selecting hardware text you may also
 *        have reselect the desired software font by calling settextfont.
 *
 * @date   861017:  Original version.
 *
 */
void 
settexttype(char *kqual)
{
	/* - If first character is an "H", turn software quality text flag off.
	 *   Otherwise, turn software quality text flag on. */
	if( kqual[0] == 'H' || kqual[0] == 'h' ){
		cmgdm.ltsoft = FALSE;
        } else {
		cmgdm.ltsoft = TRUE;
        }

	/* - If hardware text has been chosen, select the simplest font. */

	if( !cmgdm.ltsoft ) {
		settextfont( 1 );
        }

}

