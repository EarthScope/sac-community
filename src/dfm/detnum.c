/** 
 * @file   detnum.c
 * 
 * @brief  Determine the number of output entries from a format
 * 
 */

#include "bot.h"
#include "ucf.h"

/** 
 * Determine the number of output entries that would
 *    result frm reading a card using a given format statement.
 * 
 * @param kfmt 
 *    Format statement
 * @param kfmt_s 
 *    Length of \p kfmt
 * @param nentry 
 *    Number of floating point field to be read
 *
 * @date   970129:  Add parameter (0) to cnvati.  0 means that if a string
 *                  of digits is too long, let it slide by.  maf 
 * @date   860910:  Original version.
 */
void 
detnum(char *kfmt, 
       int   kfmt_s, 
       int  *nentry) {

	int imult, int_, iten, jc, ji, nc, nerr;

	/* - Initialize output argument. */
	*nentry = 0;

	/* - Determine number of characters in format statement. */
	nc = indexb( kfmt,kfmt_s );

	/* - Loop on each character in format statement.
	 *   Ignore the first and last characters which 
	 *   should be parentheses. 
	 */
	jc = 2;
	nc = nc - 1;

L_1000:
	if( jc <= nc ){

		/* -- Search for "F", "E", or "G". */
		if( (kfmt[jc - 1] == 'F'  || 
		     kfmt[jc - 1] == 'E') || 
		    kfmt[jc -  1] == 'G') {
			/* --- If found, search backward to see if 
			 *     there is a preceeding multiplier. 
			 */
			imult = 0;
			iten = 1;
			ji = jc - 1;
L_2000:
			cnvati( &kfmt[ji - 1],1, &int_, 0, &nerr );

			if( nerr == 0 ){
				imult = imult + iten*int_;
				iten = 10*iten;
				ji = ji - 1;
				goto L_2000;
			}

			/* --- Accumulate the number of entries here. */
			if( imult > 0 ){
				*nentry = *nentry + imult;
				}
			else{
				*nentry = *nentry + 1;
				}
			}

		/* -- Loop back until format statement is exhausted. */
		jc = jc + 1;
		goto L_1000;
	}

	return;
}

