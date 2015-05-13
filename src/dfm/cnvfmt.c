/** 
 * @file   cnvfmt.c
 * 
 * @brief  Convert a alphanumeric into an floating point array
 * 
 */

#include <stdio.h>

#include "dfm.h"
#include "errors.h"
#include "msg.h"
#include "bot.h"
#include "debug.h"

/** 
 * Convert a formatted alphanumeric data card into a floating point array
 * 
 * @param kcard 
 *    Alphanumeric data card
 * @param kcard_s 
 *    Length of \p kcard
 * @param kfmt 
 *    Format statement to use in the conversion
 * @param kfmt_s 
 *    Length of \p kfmt
 * @param nentry 
 *    Number of floating point values to convert
 * @param fentry 
 *    Floating point array on output
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    = ERROR_DECODING_ALPHANUMERIC_FORMAT
 *
 * @date   860910:  Original version.
 *
 */
void
cnvfmt(char *kcard, int kcard_s, char *kfmt, int kfmt_s, int nentry,
       float *fentry, int *nerr) {


    *nerr = 0;
    UNUSED(kcard);
    UNUSED(kcard_s);
    UNUSED(kfmt);
    UNUSED(kfmt_s);
    UNUSED(nentry);
    UNUSED(fentry);
    printf("formatted reads are not supported in SAC2000 yet\n");
    return;
}
