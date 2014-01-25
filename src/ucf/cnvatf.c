/** 
 * @file   cnvatf.c
 * 
 * @brief  Convert an ASCII symbol to its floating point equivalent
 * 
 */

#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <float.h>

#include "ucf.h"
#include "dff.h"
#include "debug.h"

#define	MOK	12

#define CONVERSION_OUT_OF_RANGE          4002
#define CONVERSION_TOO_LARGE             4003
#define CONVERSION_TOO_SMALL             4004
#define CONVERSION_NOT_NUMERIC           4005
#define CONVERSION_EXTRA_CHARACTERS      4006
#define CONVERSION_NOT_CONVERTED         4007
#define CONVERSION_BELOW_RESOLUTION      4008

#define SETMSG_ERROR "ERROR"

double
cdouble(char *s, 
        int  *nerr) {
  double v;
  char *endptr;
  *nerr = 0;

  v = strtod(s, &endptr);

#ifdef __UNIT_TESTING_DEBUG__
  fprintf(stderr, "v: %15.6e %4d %4d <%-12s> <%12s>\n", 
	  v, errno, strlen(endptr), endptr, s);
#endif /* __UNIT_TESTING_DEBUG__ */
  
  /* Error Checking */
  if(endptr == s)                      { *nerr = CONVERSION_NOT_NUMERIC;      }
  else if(*endptr != '\0')             { *nerr = CONVERSION_EXTRA_CHARACTERS; }
  else if(errno == ERANGE) {
    if(v > 0 && v >=   DBL_MAX)            { *nerr = CONVERSION_TOO_LARGE;        }
    if(fabs(v) >= 0 && fabs(v) <= DBL_MIN) { *nerr = CONVERSION_BELOW_RESOLUTION; }
    if(v < 0 && v <=  -DBL_MAX)            { *nerr = CONVERSION_TOO_SMALL;        }
  }
  errno = 0;
  return v;
}



/** 
 * Convert an ASCII symbol \p kfloat to its floating point equivalent \p floatn.
 * 
 * @param kfloat 
 *    - Character string (ASCII symbol)
 * @param kfloat_s 
 *    - Length of \p kfloat
 * @param floatn 
 *    - Output floating point number
 * @param lstrict 
 *    - TRUE - Catch the error and warn the user
 *    - FALSE - Let the error go unnoticed
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *    - 1 - Ran off end of \p kfloat while skipping leading blanks
 *    -   - In Fractional Part, but found a '.'
 *    -   - Found 'E|e' at first position
 *    -   - Found 'E|e' but not in whole number or Fractional Part
 *    - 2 - Current character = '.', Error converting whole number integer portion
 *    - 3 - Found 'E|e', Error Converting fractional number integer portion 
 *    - 4 - String Done, Error Converting whole number integer portion
 *    - 5 - String Done, Error Converting fractional number integer portion
 *    - 6 - String Done, Error Converting exponental number integer portion
 *    - 7 - Found 'E|e', Error Converting whole number integer portion 
 *
 * @date   2009:    Converted to using strtol() C library function
 * @date   970129:  Added lstrict to indicate how to deal with errors of
 *                  a string of digits that is to long to be converted to
 *                  an int.  1 means catch the error and warn the user;
 *                  0 means let it slide unnoticed.  maf 
 * @date   830824:  Fixed bug involving symbols containing only blanks.
 * @date   820924:  Fixed bug involving leading blanks.
 * @date   820304:  Added logic for variable length input symbol.
 * @date   810212:  Changed to output message retrieval from disk.
 *
 */
void 
cnvatf(char  *kfloat, 
       int    kfloat_s, 
       float *floatn, 
       int    lstrict, 
       int   *nerr)
{
        char *kfloat_copy;
        UNUSED(lstrict);
        kfloat_copy = fstrdup(kfloat, kfloat_s);

        *floatn = (float )cdouble(kfloat_copy, nerr);

        free(kfloat_copy);
        return;
} 

