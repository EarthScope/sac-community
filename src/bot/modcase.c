/** 
 * @file   modcase.c
 * 
 * @brief  Modify the case of a string
 * 
 */

#include "bool.h"

/** 
 * Modify the case of a character string
 * 
 * @param upflag 
 *    - TRUE - Convert to upper case
 *    - FALSE - Conver to lower case
 * @param input 
 *    Character string input
 *    Text between single or double quotes is unchanged. 
 * @param nchar 
 *    Length of \p input
 * @param output 
 *    Output character string
 *
 * @note   ICONV:   Offset used to convert from lower case to upper case.
 *             INTEGER CONSTANT: set to "-32" for ASCII character set.
 *             See SPECIAL NOTE below.
 *
 * @note 
 *   - Defined for the ASCII character set.  Only the value of ICONV
 *     would probably change for a different character set.
 *     For example, ICONV would be "-64" for the EBCDIC character set.
 *
 * @date   900508:  Removed use of ktemp from previous fix. (jet)
 * @date   871214:  Added ktemp and length variables for bug fix. (bkh)
 * @date   870514:  Fixed bug when converting to lower case.
 * @date   861229:  Original version based upon UPCASE.
 *
 */
void 
modcase(int   upflag, 
	char *input, 
	int   nchar, 
	char *output) {

	int  quoted;
	char kchar;
	int  itemp, jchar;
	static int iconv = 32;

	quoted = FALSE;

	/* - For each character in input string: */
	for ( jchar = 0 ; jchar < nchar ; jchar++ ){

		/* -- Copy to local variable. */
		kchar = input[jchar];

		/* -- Toggle quote flag if necessary. */
		if( kchar == '"' || kchar == '\'' ){
			quoted = !quoted;

			/* -- If a character and not inside quotation marks:
			 * --- Convert character to integer.
			 * --- Added conversion offset.
			 * --- Convert from integer back to character. */
		}
		else if( !quoted ){
			if( (upflag && (kchar >= 'a'))  && (kchar <= 'z') ){
				itemp = ( kchar );
				itemp = itemp - iconv;
				kchar = (itemp);
			}
			else if( (!upflag && (kchar >= 'A')) && (kchar <= 'Z') ){
				itemp = ( kchar );
				itemp = itemp + iconv;
				kchar = (itemp);
			}
		}

		/* -- Copy local variable to output string. */

		output[jchar] = kchar;

	}

	return;
}

