/** 
 * @file convcolornum.c
 * 
 * @brief Convert a color number to a name
 */

#include <string.h>

#include "gdm.h"
#include "co.h"

/** 
 * Convert a color number to it;s equivalen color name 
 *
 * @param number
 *   Number of desired color
 * @param name 
 *   Name of color on output
 *   Set to 'unknown' if not found
 * @param name_s
 *   Length of \p name on input
 *
 * @date   861020:  Original version.
 * 
 */
void 
convcolornum(int   number, 
             char *name, 
             int   name_s) {

	/* - If number is in the correct range. */
	if( number > 0 && number <= cmgdm.nctsize ){
		fstrncpy( name, 
                          name_s-1, 
                          kmgdm.ctname[number], 
                          strlen(kmgdm.ctname[number]));

        } else {
          /* - If not found, return 'UNKNOWN'. */
          fstrncpy( name, name_s-1, "UNKNOWN", 7);
          
        }

}

