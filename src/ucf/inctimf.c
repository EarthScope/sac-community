/** 
 * @file   inctimf.c
 * 
 * @brief  Add seconds to a time
 * 
 */

#include "ucf.h"

/** 
 * Add seconds to a time
 * 
 * @param nhrold 
 *    Current hour
 * @param nmnold 
 *    Current minute
 * @param nscold 
 *    Current seconds
 * @param nmsold 
 *    Currrent milliseconds
 * @param secinc 
 *    Seconds to add
 * @param secinc 
 *    Milliseconds to add
 * @param nhrnew 
 *    New Hour
 * @param nmnnew 
 *    New Minute
 * @param nscnew
 *    New Seconds 
 * @param nmsnew 
 *    New milliseconds
 * @param nexday 
 *    Number of extra days to add in new time
 *    - 0 is still the same days
 *
 * @bug This should be used exclusively in place of inctim()
 * @bug Lots of constants
 *
 * @date   970328:  Fixed bug if number of seconds = 60, did same for
 *                  minutes while I was there.  maf
 * @date   801018:  Converted to integer second/millisecond representation.
 * @date   801029:  Corrected error involving above change.
 * @date   810309:  Corrected error with negative time increments.
 * @date   810000:  Original version.
 */

void 
inctimf(int  nhrold, 
        int  nmnold, 
        int  nscold, 
        int  nmsold, 
        int  nscinc, 
        int  nmsinc, 
        int *nhrnew, 
        int *nmnnew, 
        int *nscnew, 
        int *nmsnew, 
        int *nexday) {

	int nhradd, nmnadd, nscadd;

	/* - Set new values equal to old values. */
	*nexday = 0;
	*nhrnew = nhrold;
	*nmnnew = nmnold;

	/* Convert the floating point seconds to integer seconds and milliseconds. */

	*nmsnew = nmsold + nmsinc;
	*nscnew = nscold + nscinc;

	/* - Make sure each output field is within it's correct range. */

	/* -- milliseconds. */
	if( *nmsnew > 999 ){
		nscadd = *nmsnew/1000;
		*nscnew = *nscnew + nscadd;
		*nmsnew = *nmsnew - 1000*nscadd;
	}
	else if( *nmsnew < 0 ){
		nscadd = *nmsnew/1000 - 1;
		*nscnew = *nscnew + nscadd;
		*nmsnew = *nmsnew - 1000*nscadd;
	}

	/* -- seconds. */
	if( *nscnew > 59 ){
		nmnadd = *nscnew/60;
		*nmnnew = *nmnnew + nmnadd;
		*nscnew = *nscnew - 60*nmnadd;
	}
	else if( *nscnew < 0 ){
		nmnadd = *nscnew/60 - 1;
		*nmnnew = *nmnnew + nmnadd;
		*nscnew = *nscnew - 60*nmnadd;
		if ( *nscnew == 60 ) {	/* if number of seconds is 60 ... (maf 970328) */
		    (*nmnnew)++ ;	/* increment number of minutes, ... */
		    *nscnew = 0 ;	/* and set number of seconds to 0 */
		}
	}

	/* -- minutes. */
	if( *nmnnew > 59 ){
		nhradd = *nmnnew/60;
		*nmnnew = *nmnnew - 60*nhradd;
		*nhrnew = *nhrnew + nhradd;
	}
	else if( *nmnnew < 0 ){
		nhradd = *nmnnew/60 - 1;
		*nmnnew = *nmnnew - 60*nhradd;
		*nhrnew = *nhrnew + nhradd;
                if ( *nmnnew == 60 ) {  /* if number of minutes is 60 ... (maf 970328) */
                    (*nhrnew)++ ;       /* increment number of hours, ... */
                    *nmnnew = 0 ;       /* and set number of minutes to 0 */
                }
	}

	/* -- hours. */
	if( *nhrnew > 23 ){
		*nexday = *nhrnew/24;
		*nhrnew = *nhrnew - 24**nexday;
	}
	else if( *nhrnew < 0 ){
		*nexday = *nhrnew/24 - 1;
		*nhrnew = *nhrnew - 24**nexday;
	}

	return;
}

