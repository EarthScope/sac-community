/** 
 * @file   kadate.c
 * 
 * @brief  Stingify a date
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "ucf.h"
#include "bot.h"

#include "msg.h"
#include "co.h"

/** 
 * Convert a year/day_of_year int a string of form MMM DD (JJJ), YYYY
 * 
 * @param iyear 
 *    Input Year
 * @param ijday 
 *    Input day of the year
 * @param ncdate 
 *    Maximum length of output string, must be at least 18
 * @param kkdate 
 *    Output string
 * @param kkdate_s 
 *    Length of \p kkdate on output
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 * 
 * @date   811019:  Rewrote using F77 character string maninpulations.
 * @date   810120:  Changed to output message retrieval from disk.
 *
 */
void
kadate(int   iyear, 
       int   ijday, 
       int   ncdate, 
       char *kkdate, 
       int   kkdate_s, 
       int  *nerr) {

	char kenc[5];
	int iday, imonth;
	static char kmonth[12][5]={"JAN ","FEB ","MAR ","APR ","MAY ",
	 "JUN ","JUL ","AUG ","SEP ","OCT ","NOV ","DEC "};
        char *cattemp;
        char *s1;


	*nerr = 0;

	/* - Make sure output character variable is long enough. */
	if( ncdate < 18 ){
		*nerr = 906;
		setmsg( "ERROR", *nerr );
		subscpy( kkdate, 0, ncdate - 1, kkdate_s - 1, "********" );
		goto L_8888;
        }

	/* - Convert from julian day to month and day. */
	kidate( iyear, ijday, &imonth, &iday, nerr );

	/* - Return if input fields were bad. */
	if( *nerr != 0 ){
          subscpy( kkdate, 0, ncdate - 1, kkdate_s - 1, "********"  );
          goto L_8888;
        }

	/* - Define month subfield: */
	subscpy( kkdate, 0, 3, kkdate_s - 1, kmonth[imonth - 1] );

	/* - Encode day subfield: */
        sprintf(kenc,"%4d",iday);
	if( iday >= 10 ){
        s1 = strcut(kenc, 3, 4);
		subscpy( kkdate, 4, 5, kkdate_s - 1, s1 );
        free(s1);
    }
	else{
        cattemp = malloc(3);
        cattemp[0] = '0';
        cattemp[1] = kenc[3];
        cattemp[2] = '\0';
		subscpy( kkdate, 4, 5, kkdate_s - 1, cattemp );
        free(cattemp);
    }

	/* - Encode julian day subfield. */

	subscpy( kkdate, 6, 7, kkdate_s - 1, " (" );
    sprintf(kenc,"%4d",ijday);
	if( ijday >= 100 ){
        s1 = strcut(kenc, 2, 4);
		subscpy( kkdate, 8, 10, kkdate_s - 1, s1 );
        free(s1);
    }
	else if( ijday >= 10 ){
		kkdate[8] = '0';
        s1 = strcut(kenc, 3, 4);
		subscpy( kkdate, 9, 10, kkdate_s - 1, s1 );
        free(s1);
    }
	else{
		subscpy( kkdate, 8, 9, kkdate_s - 1, "00" );
		kkdate[10] = kenc[3];
    }

	/* - Encode year subfield, blank filling to end of output field. */

	subscpy( kkdate, 11, 13, kkdate_s - 1, "), " );
    sprintf(kenc,"%4d",iyear);
    s1 = strcut(kenc, 1, 4);
	subscpy( kkdate, 14, ncdate - 1, kkdate_s - 1, s1 );
    free(s1);
        
L_8888:

	return;
}

