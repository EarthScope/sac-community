/** 
 * @file   incdat.c
 * 
 * @brief  Add days to a current date/time
 * 
 */

#include "ucf.h"
#include "ncpf.h"

/** 
 * Add days to a current date/time
 * 
 * @param nyrold 
 *    Current Year
 * @param njdold 
 *    Current Day of the Year 
 * @param njdinc 
 *    Days to add to get new Year and Day of the year
 *    May be positive of negative
 * @param nyrnew 
 *    New Year
 * @param njdnew 
 *    New Days of the year
 * 
 * @bug The while loop in here is funky, fix to be if ndays is < 1 or
 *       > number of days in the year
 * 
 * @date   810000:  Original version.
 *
 */
void 
incdat(int   nyrold, 
       int   njdold, 
       int   njdinc, 
       int  *nyrnew, 
       int  *njdnew) {

        int ndays;

	*nyrnew = nyrold;
	*njdnew = njdold + njdinc;

	while ( 1 ) {
	    if( *njdnew < 1 ){
        (*nyrnew) -- ;
		ndays = isLeapYear( *nyrnew ) ? 366 : 365;
		*njdnew += ndays;
	    }
	    else{
		ndays = isLeapYear( *nyrnew ) ? 366 : 365;
		if( *njdnew > ndays ){
		    *nyrnew = *nyrnew + 1;
		    *njdnew = *njdnew - ndays;
		}
		else{
		    return;
		}
	    }
	}
}

