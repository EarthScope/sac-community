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
cnvfmt(char  *kcard, 
       int    kcard_s, 
       char  *kfmt, 
       int    kfmt_s, 
       int    nentry, 
       float *fentry, 
       int   *nerr) {

	int nc;

	*nerr = 0;
  UNUSED(nentry);
  UNUSED(fentry);
    printf("formatted reads are not supported in SAC2000 yet\n");
	goto L_8888;

	/* - Come to here if an error occurs during decode. */

	/* - Determine length of input card. */
	nc = indexb( kcard,kcard_s );

	*nerr = ERROR_DECODING_ALPHANUMERIC_FORMAT;
	setmsg( "ERROR", *nerr );
	aplmsg( "Format statement =",19 );
	apcmsg( kfmt,kfmt_s );
	aplmsg( "Data card =",12 );
	apcmsg( "\"",2 );
        apcmsg2(kcard,nc);
	apcmsg( "\"",2 );

L_8888:
	return;
}

