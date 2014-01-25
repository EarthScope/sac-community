
#include "gdm.h"

/** 
 * Change the text justification
 *
 * @param khorz
 *    Horizontal Justification
 *     - 'LEFT' for left justification.
 *     - 'CENTER' for centered justification.
 *     - 'RIGHT' for right justification.
 *     -  (Only the first character need be entered.)
 * @param kvert
 *    Vertical Justification
 *     - 'BOTTOM' for bottom justification.
 *     - 'CENTER' for centered justification.
 *     - 'TOP' for TOP justification.
 *     -  (Only the first character need be entered.)
 *
 *    861017:  Original version.
 *
 */
void 
settextjust(char *khorz,
            char *kvert)
{
	/* - Only check first character, but check for upper and lower case. */
	/* - Errors result in default (LEFT, BOTTOM). */
	if( khorz[0] == 'L' || khorz[0] == 'l' ){
		cmgdm.ihjust = 1;
		}
	else if( khorz[0] == 'C' || khorz[0] == 'c' ){
		cmgdm.ihjust = 2;
		}
	else if( khorz[0] == 'R' || khorz[0] == 'r' ){
		cmgdm.ihjust = 3;
		}
	else{
		cmgdm.ihjust = 1;
		}

	if( kvert[0] == 'B' || kvert[0] == 'b' ){
		cmgdm.ivjust = 1;
		}
	else if( kvert[0] == 'C' || kvert[0] == 'c' ){
		cmgdm.ivjust = 2;
		}
	else if( kvert[0] == 'T' || kvert[0] == 't' ){
		cmgdm.ivjust = 3;
		}
	else{
		cmgdm.ivjust = 1;
		}
}

