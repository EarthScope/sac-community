/** 
 * @file   katime.c
 * 
 * @brief  Stringify a time
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "ucf.h"
#include "bot.h"

#include "msg.h"
#include "co.h"

/** 
 * Convert a time into its alphanumeric equivalent
 * 
 * @param ihour 
 *    Current hour
 * @param imin 
 *    Current minute
 * @param isec 
 *    Current second
 * @param imsec 
 *    Current millisecond
 * @param nctime 
 *    Maximum Length of \p kktime
 *    Must be at least 12 characters long
 * @param kktime 
 *    Output string
 *      Form:  HH:MM:SS.SSS
 * @param kktime_s 
 *    Length of \p kttime on output
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @bug A nice sprintf statement here would reduce the complexity
 *
 * @date   811019:  Made minor changes and added documentation.
 * @date   810313:  Rewrite to include leading zeros in all time fields.
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   801113:  Changed to use of F77 concantenation rather than ZMOVEC.
 * @date   801018:  Changed from floating sec. to integer sec. and millisec.
 *
 */
void 
katime(int   ihour, 
       int   imin, 
       int   isec, 
       int   imsec, 
       int   nctime, 
       char *kktime, 
       int   kktime_s, 
       int  *nerr) {

	char kenc[5];
        char *s1;

	*nerr = 0;

	/* - Make sure output character variable is long enough. */
	if( nctime < 12 ){
		*nerr = 905;
		setmsg( "ERROR", *nerr );
		subscpy( kktime, 0, nctime - 1, kktime_s - 1, "********"
		  );
		goto L_8888;
		}

	/* - If input integer fields have valid values: */
	if( ((((((ihour >= 0 && ihour <= 24) && imin >= 0) && imin <= 
	 59) && isec >= 0) && isec <= 59) && imsec >= 0) && imsec <= 999 ){

		/* -- Define general form. */
		subscpy( kktime, 0, nctime - 1, kktime_s - 1, "00:00:00.000"
		  );

		/* -- Encode hour field. */
                sprintf(kenc,"%4d",ihour);
		if( ihour >= 10 ){
            s1 = strcut(kenc, 3, 4);
			subscpy( kktime, 0, 1, kktime_s - 1, s1 );
            free(s1);
        }
		else{
			kktime[1] = kenc[3];
        }

		/* -- Encode minute field. */
                sprintf(kenc,"%4d",imin);
		if( imin >= 10 ){
            s1 = strcut(kenc, 3, 4);
			subscpy( kktime, 3, 4, kktime_s - 1, s1 );
            free(s1);
        }
		else{
			kktime[4] = kenc[3];
        }

		/* -- Encode second field */
        sprintf(kenc,"%4d",isec);
		if( isec >= 10 ){
            s1 = strcut(kenc, 3, 4);
			subscpy( kktime, 6, 7, kktime_s - 1, s1 );
            free(s1);
        }
		else{
			kktime[7] = kenc[3];
        }

		/* -- Encode millisecond field. */
        sprintf(kenc,"%4d",imsec);
		if( imsec >= 100 ){
            s1 = strcut(kenc, 2, 4);
			subscpy( kktime, 9, 11, kktime_s - 1, s1 );
            free(s1);
        }
		else if( imsec >= 10 ){
            s1 = strcut(kenc, 3, 4);
			subscpy( kktime, 10, 11, kktime_s - 1, s1 );
            free(s1);
        }
		else{
			kktime[11] = kenc[3];
        }

		/* - Set up error message if input fields were bad. */

		}
	else{
		*nerr = 907;
		setmsg( "ERROR", *nerr );
		apimsg( ihour );
		apimsg( imin );
		apimsg( isec );
		apimsg( imsec );
		subscpy( kktime, 0, nctime - 1, kktime_s - 1, "********"
		  );
		goto L_8888;
		}

L_8888:
	return;
}

