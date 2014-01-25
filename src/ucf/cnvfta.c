/** 
 * @file   cnvfta.c
 *
 * @brief  Convert a floating point number to string
 * 
 */

#include <string.h>
#include <math.h>

#include "ucf.h"
#include "debug.h"

/** 
 * Convet a floating point number \p float_ to its ASCII equivalent, 
 *    a string
 * 
 * @param float_ 
 *     Floating point number to convert
 * @param nchar 
 *     Number of characters in the string
 * @param nsig
 *     Number of significant figures in the string 
 * @param kfloat 
 *     Returned string with formatted number
 *     If number cannot be converted then string is set to 'BADINPUT'
 * @param kfloat_s 
 *     Length of string \p kfloat , not used.
 *
 *
 * @date 20080512:  Rewritten to something resembling sanity. <rwg@vt.edu>
 * @date   830922:  Made machine independent by using F77 encode capability.
 * @date   800102:  Original version.
 *
 */
void 
cnvfta(double    float_, 
       int  nchar, 
       int  nsig, 
       char     *kfloat, 
       int       kfloat_s)
{
	char kfmt[9];
	int ret;
  UNUSED(kfloat_s);
	/* - Build format statement by encoding input variables.
	 *   (If NSIG is 0 then create an integer rather than real format.) */

	if (nsig > 0) {
		sprintf(kfmt, "%%%d.%df", nchar, nsig);
		ret = sprintf(kfloat, kfmt, float_);
	} else {
		sprintf(kfmt, "%%%dd", nchar);
		ret = sprintf(kfloat, kfmt, (int)round(float_));
	}

	if (ret < 0) {
		strcpy(kfloat, "BADINPUT");
	}

	return;

}

