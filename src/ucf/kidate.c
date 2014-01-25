/** 
 * @file   kidate.c
 * 
 * @brief  Convert a Year/Day_of_year to a Year/Month/Day
 * 
 */

#include "ucf.h"


#include "msg.h"

/** 
 * Convert a Year/Day_of_year to a Year/Month/Day
 * 
 * @param iyear 
 *    Year on Input and Output
 * @param ijday 
 *    Doy of Year on Input
 * @param imonth 
 *    Month on Output
 * @param iday 
 *    Day on Output
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @bug Leap year is handled incorrectly
 * @bug Extra days are not handled properly
 * 
 * @date   811019:  Documention completed.
 * @date   810120:  Changed to output message retrieval from disk.
 *
 */
void 
kidate(int   iyear, 
       int   ijday, 
       int  *imonth, 
       int  *iday, 
       int  *nerr) {

	static int ndays[12]={31,28,31,30,31,30,31,31,30,31,30,31};

	int *const Ndays = &ndays[0] - 1;

	*nerr = 0;

	/* - Make temporary leap year adjustment to days-in-month table
	 *   if necessary. */

	if( (iyear/4)*4 == iyear ){
		Ndays[2] = 29;
		}
	else{
		Ndays[2] = 28;
		}

	/* - Subtract days-in-month from julian day until proper month is found. */

	*iday = ijday;
	for( *imonth = 1; *imonth <= 12; (*imonth)++ ){
          if( *iday <= Ndays[*imonth] )
            goto L_8888;
          *iday = *iday - Ndays[*imonth];
        }
        
	/* - Process error due to bad input here. */

	*nerr = 909;
	setmsg( "ERROR", *nerr );
	apimsg( iyear );
	apimsg( ijday );
	*imonth = 0;
	*iday = 0;

L_8888:
	return;

}

