/** 
 * @file   ldttm.c
 * 
 * @brief  Validate a date/time
 * 
 */

#include "ucf.h"
#include "hdr.h"
#include "bool.h"

/** 
 * Validate a date time
 * - Each element is check to see if it is defined, but is not checked
 *   to see if it lies within the valid range for that element.
 *   This feature will be added if it is needed.
 * 
 * @param ndttm 
 *    Date time to validate
 *      ndttm(0) = year
 *      ndttm(1) = day of the year
 *      ndttm(2) = hour
 *      ndttm(3) = minute
 *      ndttm(4) = second
 *      ndttm(5) = millisecond
 * 
 * @return 
 *    - TRUE if the date/time is valid
 *    - FALSE if the date/time is not valid
 *
 * @bug Convert the time values into 64 bit integers (long long int)
 *
 * @date   810202:  Original version.
 *
 */
int 
ldttm(int *ndttm) {

	int ldttm_v;

	int *const Ndttm = &ndttm[0] - 1;

	/* - Check to make sure each element of the date-time array is defined. */
	if( Ndttm[1] == cmhdr.nundef ){
          ldttm_v = FALSE;
        }
	else if( Ndttm[2] == cmhdr.nundef ){
          ldttm_v = FALSE;
        }
	else if( Ndttm[3] == cmhdr.nundef ){
          ldttm_v = FALSE;
        }
	else if( Ndttm[4] == cmhdr.nundef ){
          ldttm_v = FALSE;
        }
	else if( Ndttm[5] == cmhdr.nundef ){
          ldttm_v = FALSE;
        }
	else if( Ndttm[6] == cmhdr.nundef ){
          ldttm_v = FALSE;
        }
	else{
          ldttm_v = TRUE;
        }
        
	return( ldttm_v );
}

