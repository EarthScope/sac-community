/** 
 * @file   indexc.c
 * 
 * @brief  Find the length of a character string
 * 
 */

#include "bot.h"
#include "bool.h"

/** 
 * Find the length of a character string.  String is terminated by the 
 *    first occurance of a character or by the end of the string.
 * 
 * @param string 
 *    Character string
 * @param string_s 
 *    Length of \p string
 * @param kchar 
 *    Requested termination character
 * 
 * @return Index of character before the requested character
 *
 * @date   920326:  Changed parameter char to kchar.
 * @date   830527:  Original version.
 *
 */
int
indexc(char *string, 
       int   string_s, 
       int   kchar)
{
	int indexc_v, j;

	/* - Call general character search routine, searching for the first
	 *   occurance of the delimiter searching forward. */
	j = indexa( string,string_s, kchar, TRUE, TRUE );

	/* - If CHAR was found, return with pointer at previous character.
	 *   If CHAR was not found, return with pointer at end of string. */

	if( j > 0 ){
		indexc_v = j - 1;
	}
	else {
		indexc_v = (string_s - 1);
	}

	return( indexc_v );

}

