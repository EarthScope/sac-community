/** 
 * @file   upcase.c
 * 
 * @brief  Conver a character string to upper case
 * 
 */

#include <string.h>
#include <ctype.h>
#include "bot.h"
#include "co.h"

/** 
 * Convert a character string to upper case
 * 
 * @param kinput 
 *   Input character string
 * @param nchar 
 *   Length of \p kinput
 * @param koutpt 
 *   Output character string
 * @param koutpt_s 
 *   Length of \p koutpt
 *
 * @note Local Variables
 *   - ICONV:   Offset used to convert from lower case to upper case.
 *             INTEGER CONSTANT: set to "-32" for ASCII character set.
 *             See SPECIAL NOTE below.
 *   - KCHAR:   Character that is currently being converted.
 *
 * @note 
 * - Defined for the ASCII character set.  Only the value of ICONV
 *   would probably change for a different character set.
 *   For example, ICONV would be "+64" for the EBCDIC character set.
 *
 * @date   871214:  Added ktemp and length variables for bug fix. (BKH)
 * @date   820309:  Original version.
 *
 */
void 
upcase(char *kinput, 
       int   nchar, 
       char *koutpt, 
       int   koutpt_s) {

	char ktemp[133];
	char kchar;
	int itemp, jchar, length;
	static int iconv = -32;

	/* - Initialize output string to blanks: */
        memset(ktemp,(int)' ',132);
        ktemp[132] = '\0';

	/* - For each character in input string: */

	length = nchar;
	if( length > 132 )
		length = 132;
	for ( jchar = 1 ; jchar <= length ; jchar++ ){

		/* -- Copy to local variable. */

		kchar = kinput[jchar - 1];

		/* -- If between "a" and "z":
		 * --- Convert character to integer.
		 * --- Added conversion offset.
		 * --- Convert from integer back to character. */

		if( (kchar >= 'a') && (kchar <= 'z') ){
			itemp = ( kchar );
			itemp = itemp + iconv;
			kchar = (itemp);
		}

		/* -- Copy local variable to output string. */

		ktemp[jchar - 1] = kchar;

	} /* end for */

        if ( koutpt_s == 1 )
		*koutpt = ktemp[0];
        else
		fstrncpy(koutpt,koutpt_s - 1,ktemp,jchar-1);

	return;

}

char * 
upcase_dup(char *s) {
  char *p, *new;
  p = new = strdup(s);
  while(*p) {
    *p = toupper(*p);
    p++;
  }
  return new;
}
