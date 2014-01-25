/** 
 * @file   zgetc.c
 * 
 * @brief  Get a stirng from an array
 * 
 */
 
/** 
 * Get a character string from an integer or real array
 * 
 * @param array 
 *    Real or integer source array (Apparently a int array)
 * @param str 
 *    Character array, destination
 * @param pnumc 
 *    Number of characters to copy
 *
 * @bug This function is screwy. Casting a series of ints into characters
 *      I think the calling functions are casting incorrectly.
 *
 * @date 07/23/84   Under development--D. Trimmer
 * @date 07/23/84   Tested--D. Trimmer
 *
 */
void
zgetc(int  *array,
      char *str,
      int   pnumc) {

	char  *parray;		/* pointer into array */
	char  *pstr;		/* pointer into string */
	int    i;		/* index */
 
	parray = (char *) &array[0];	/* character pointer into array */
	pstr   = &str[0];		/* character pointer into str */
	for (i=0;i<pnumc;++i)
		*pstr++ = *parray++;	/* copy characters */
 
	return;
}
