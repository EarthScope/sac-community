/** 
 * @file   cnvfre.c
 * 
 * @brief  Convert a free format alphanumeric into an array
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dfm.h"

#include "errors.h"


#include "co.h"
#include "msg.h"
#include "bot.h"
#include "ucf.h"

/** 
 * Convert a free format alphanumeric data card into a floating point array
 * 
 * @param kcard 
 *    Alphanumeric data card
 * @param kcard_s 
 *    Length of \p kcarc
 * @param mentry 
 *    Maximum number of floating point fields to convert
 * @param nentry 
 *    Number of floating point field converted
 * @param fentry 
 *    Floating point array in output
 * @param ientry 
 *    - -2   Save alphanumeric field
 *    - -1   Skip
 *    - >= 0 Decode
 * @param kalpha 
 *    Alphanumeric storage on output
 * @param kalpha_s 
 *    Length of \p kalpha
 * @param lstrict 
 *    How to deal with errors
 *    - 1 Catch error and warn the user
 *    - 0 Let the error slide unnoticed
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_MAX_NUM_FREE_FORMAT_EXCEEDED
 * @date   970129:  Added lstrict to indicate how to deal with errors of
 *                  a string of digits that is to long to be converted to
 *                  an int.  1 means catch the error and warn the user;
 *                  0 means let it slide unnoticed.  maf 
 * @date   860910:  Original version.
 *
 */
void 
cnvfre(char  *kcard, 
       int    kcard_s, 
       int    mentry, 
       int   *nentry, 
       float *fentry, 
       int   *ientry, 
       char  *kalpha, 
       int    kalpha_s, 
       int    lstrict, 
       int   *nerr) {

	int ic, ic1, ic2, itype, nc;
        char *strtemp;

	float *const Fentry = &fentry[0] - 1;
	int *const Ientry = &ientry[0] - 1;

	*nerr = 0;

	/* - Determine length of input card. */
	nc = indexb( kcard,kcard_s );

	/* - Initialize pointer to current character in string and
	 *   number of output values. */
	ic = 0;
	*nentry = 0;

	/* - Loop on each token in string.
	 *   Terminates when string is exhausted or maximum number
	 *   of output entries is reached. */
	poptok( kcard, nc, &ic, &ic1, &ic2, &itype );

	/* -- Loop until there are no more tokens in string or */
	/*    until there is no more room in output list. */
	while ( itype != 0 && *nentry < mentry ) {
	    /* -- Convert this token to a floating point variable. */
	    *nentry = *nentry + 1;
	    if( Ientry[*nentry] >= 0 ){
		strtemp = malloc( ic2 - ic1 + 2 ) ;
		strncpy ( strtemp , kcard + ic1 - 1 , ic2 - ic1 + 1 ) ;
		strtemp[ ic2 - ic1 + 1 ] = '\0' ;

		cnvatf( strtemp , ic2-ic1+2 , &Fentry[*nentry], lstrict, nerr );

		free ( strtemp ) ;
	    }
	    else if ( Ientry[ *nentry ] == -2 ){
		fstrncpy( kalpha, kalpha_s-1, kcard+ic1 - 1, ic2 - ic1 + 1);
	    }
	    if( *nerr )
		break ;

	    poptok( kcard, nc, &ic, &ic1, &ic2, &itype ) ;
	}

	if ( *nentry >= mentry ) {
	    *nerr = ERROR_MAX_NUM_FREE_FORMAT_EXCEEDED;
	    setmsg( "ERROR", *nerr );
	    apimsg( mentry );
	}

}

