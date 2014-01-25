/** 
 * @file   inctim.c
 * 
 * @brief  Add seconds to a time
 * 
 */

#include <string.h>
#include <math.h>

#include "ucf.h"
#include "co.h"
#include "config.h"

#define TOLERANCE 1e-3
#define TOLERANCE_EXCEEDED "Resulting error from shift exceeded tolerance (1e-3): "
#define BUG_REPORT "Please report bug to: "

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
 *    Seconds + milliseconds to add
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
 * @bug secinc should be two integers, one for seconds and the other for milliseconds
 * @see inctimf
 * 
 * @date   970328:  Fixed bug if number of seconds = 60, did same for
 *                  minutes while I was there.  maf
 * @date   801018:  Converted to integer second/millisecond representation.
 * @date   801029:  Corrected error involving above change.
 * @date   810309:  Corrected error with negative time increments.
 * @date   810000:  Original version.
 *
 */
void 
inctim(int     nhrold, 
       int     nmnold, 
       int     nscold, 
       int     nmsold, 
       double  secinc, 
       int    *nhrnew, 
       int    *nmnnew, 
       int    *nscnew, 
       int    *nmsnew, 
       int    *nexday) { 

	int nmsinc, nscinc;

        nscinc = floor(secinc);
        nmsinc = lround( 1000.0 * (secinc - (float)( nscinc )) );
        
        return inctimf(nhrold, nmnold, nscold, nmsold,
                       nscinc, nmsinc,
                       nhrnew, nmnnew, nscnew, nmsnew, 
                       nexday);
        
}

