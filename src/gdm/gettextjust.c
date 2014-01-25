
#include "co.h"
#include "gdm.h"

#include "debug.h"

/** 
 * Get current graphics text justification
 *
 * @param khorz
 *    Horizontal text justificiation
 *    - 'LEFT' for left justification.
 *    - 'CENTER' for centered justification.
 *    - 'RIGHT' for right justification.
 * @param khorz_s
 *    Length of \p khorz and \p kvert
 * @param kvert
 *    Vertical text justificiation
 *    - 'BOTTOM' for bottom justification.
 *    - 'CENTER' for centered justification.
 *    - 'TOP' for TOP justification.
 *
 * @date   861017:  Original version.
 *
 */
void 
gettextjust(char *khorz,
            int khorz_s,
            char *kvert,
	    int kvert_s)
{
	if( cmgdm.ihjust == 1 ){
		fstrncpy( khorz, khorz_s-1, "LEFT", 4 );
	}
	else if( cmgdm.ihjust == 2 ){
		fstrncpy( khorz, khorz_s-1, "CENTER", 6 );
	}
	else{
		fstrncpy( khorz, khorz_s-1, "RIGHT", 5 );
	}

	if( cmgdm.ivjust == 1 ){
		fstrncpy( kvert, kvert_s-1, "BOTTOM", 6 );
	}
	else if( cmgdm.ivjust == 2 ){
		fstrncpy( kvert, kvert_s-1, "CENTER", 6 );
	}
	else{
		fstrncpy( kvert, kvert_s-1, "TOP", 3 );
	}

}

