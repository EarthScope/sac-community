/** 
 * @file   getfline.c
 * 
 * @brief  Get a line from a file
 * 
 */

#include <stdio.h>

#include "co.h"
 
/** 
 * Get a line from a file descriptor and pad the line with blanks
 * 
 * @param pfd 
 *    Pointer to the file descriptor
 * @param pch 
 *    pointer to the character string
 * @param maxlen 
 *    Length of \p pch
 *
 * @return Number of characters read
 *
 * @date 08/17/84       Under development--D. Trimmer
 * @date 08/17/84       Tested--D. Trimmer
 *
 * @bug Should be replaced with fgets() or equivalent
 *      Only used by zgpmsg() and only if readline is disabled
 *
 */
int 
getfline(FILE *pfd,
	 char *pch,
	 int   maxlen) {

	char *pchsave;	/* save string pointer */
	int   i;	/* index and number of characters read */
	int   ret;	/* save return value */
	int   ichar;	/* used to read characters--is integer so EOF (usually
			   a -1 can be read) */
 
	pchsave = pch;
    ichar = '\0';
	for (i=0; 
	     i<(maxlen) && 
	       (ichar=getc(pfd))!='\0' && 
	       ichar!='\n' &&
	       ichar!=EOF;
	     ++i)
			*(pchsave++) = (char) ichar;
 
	ret = i;
    if(ichar == EOF) {
        ret = EOF;
    }

	/* Pad with blanks */
	for (;i<(maxlen);++i)
		*(pchsave++) = ' ';

        pch[maxlen-1] = '\0';
 
	return(ret);
}
 
