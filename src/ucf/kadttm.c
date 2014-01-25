/** 
 * @file   kadttm.c
 * 
 * @brief  Stringify a date/time
 * 
 */

#include <string.h>

#include "ucf.h"


#include "co.h"

/** 
 * Convert a integer date/time array to its alphanumeric equivalent
 * 
 * @param idttm 
 *    Current date/time, Length of 6
 *      Elements are year, julian day, hour, minute,
 *      second, and millisecond.
 * @param kdttm 
 *    Output string
 *      Form is: MMM DD (JJJ), YYYY  HH:MM:SS.S
 *      Should be at least 32 characters long.
 * @param kdttm_s 
 *    Length of \p kdttm
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @bug A sprintf would work just fine here
 *
 * @date   900502:  Forced length of kdttm to be 32 characters long.
 * @date   860203:  Original version.
 *
 */
void 
kadttm(int  *idttm, 
       char *kdttm, 
       int   kdttm_s, 
       int  *nerr) {

	char kdt[19], ktm[13];

	int *const Idttm = &idttm[0] - 1;
    memset(kdt, 0, sizeof(kdt));
    memset(ktm, 0, sizeof(ktm));
	/* - Convert date part. */
	kadate( Idttm[1], Idttm[2], 18, kdt,19, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Convert time part. */
	katime( Idttm[3], Idttm[4], Idttm[5], Idttm[6], 12, ktm,13, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Append two parts to form output argument. */
        fstrncpy( kdttm, kdttm_s-1, kdt, strlen(kdt));
        fstrncpy( kdttm+strlen(kdt), kdttm_s-1-strlen(kdt),
                                           " ", 1 );
        fstrncpy( kdttm+strlen(kdt)+1, kdttm_s-1-strlen(kdt)-1,
                                    ktm, strlen(ktm));

L_8888:
	return;
}

