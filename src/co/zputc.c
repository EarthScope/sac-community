/** 
 * @file   zputc.c
 * 
 * @brief  Put a string into an array
 * 
 */

#include "debug.h"

/** 
 * Place a character string into a real or integer array
 * 
 * @param str 
 *    Source character array
 * @param strlen 
 *    Length of \p str
 * @param array 
 *    Destination array
 * @param pnumc 
 *    Length of \p array
 *
 * @bug Look into this function and co/zgetc().  They use pointer casting to
 *      manipulate the data in non-intuitive ways.
 */ 
void
zputc(char *str,
      int   strlen,
      int  *array,
      int   pnumc) {

	char  *parray;		/* pointer into array */
	char  *pstr;		/* pointer into string */
	int    i;		/* index */

  UNUSED(strlen);

	parray = (char *) &array[0];	/* character pointer into array */
	pstr   = &str[0];		/* character pointer into str */
	for (i=0;i<pnumc;++i)
		*parray++ = *pstr++;	/* copy characters */
 
	return;
}
 
