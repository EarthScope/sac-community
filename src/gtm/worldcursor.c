/** 
 * @file   worldcursor.c
 * 
 * @brief  Locate Cursor
 * 
 */

#include "gtm.h"


#include "gdm.h"

/** 
 * To perform "locator" graphics input function.
 *      The cursor is turned on and initially placed at the
 *      world coordinate location given by xwloc and ywloc.
 *      When a single character is typed at the terminal,
 *      the new cursor location and character are returned.
 * 
 * @param xwloc 
 *    Input:  Initial X plot coordinate for cursor. [f]
 *    Output: Current X plot coordinate for cursor. [f]
 * @param ywloc 
 *    Input:  Initial Y plot coordinate for cursor. [f]
 *    Output: Current Y plot coordinate for cursor. [f]
 * @param kchar 
 *    Alphanumeric character typed at terminal. [c1]
 *
 * @date   861027:  Original version.
 *
 */
void
worldcursor(float *xwloc, 
            float *ywloc, 
            char *kchar) {

	float xloc, yloc;

	/* - Move to input world coordinate position. */
	worldmove( *xwloc, *ywloc );

	/* - Get character and cursor position (in viewport coordinates.) */
	cursor0( &xloc, &yloc, kchar );

	/* - Convert viewport location to world coordinates. */
	*xwloc = (xloc - cmgtm.xmpwv2)/cmgtm.xmpwv1;
	*ywloc = (yloc - cmgtm.ympwv2)/cmgtm.ympwv1;

	return;
}

