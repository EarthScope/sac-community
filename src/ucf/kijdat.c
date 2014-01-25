/** 
 * @file   kijdat.c
 * 
 * @brief  Convert a Year/Month/Day into a Year/Day_of_Year
 * 
 */

#include "ucf.h"


#include "msg.h"

/** 
 * Convert a Year/Month/Day into a Year/Day_of_Year
 * 
 * @param iyear 
 *    Year on Input and Output
 * @param imonth 
 *    Month on Input
 * @param iday 
 *    Day on Input
 * @param ijday 
 *    Day of Year on Output
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @bug Leap years are handled incorrectly
 * @bug Lots of constants, see the ndays (months)
 *
 * @date   811019:  Cleaned up algorithm and documented module.
 * @date   810120:  Changed to output message retrieval from disk.
 *
 */
void 
kijdat(
       int   iyear, 
       int   imonth, 
       int   iday, 
       int  *ijday, 
       int  *nerr) { 

	int jmonth;
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

	/* - Check month and day fields for validity. */
	if( ((imonth <= 0 || imonth > 12) || iday <= 0) || iday > Ndays[imonth] ){
		*nerr = 909;
		setmsg( "ERROR", *nerr );
		apimsg( iyear );
		apimsg( imonth );
		apimsg( iday );
		*ijday = 0;
		goto L_8888;
		}

	/* - Compute julian day from days-in-month table. */
	*ijday = iday;
	for( jmonth = 1; jmonth <= (imonth - 1); jmonth++ ){
		*ijday = *ijday + Ndays[jmonth];
		}

L_8888:
	return;
}

