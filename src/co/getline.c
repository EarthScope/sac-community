/** 
 * @file   getline.c
 * 
 * @brief  Get a line from a file descriptor
 * 
 */

#include <stdio.h>

#include "co.h"

/** 
 * Read a line from a file descriptor and null terminate
 * 
 * @param pfd 
 *    File descriptor
 * @param pch 
 *    Character string
 * @param maxlen 
 *    Length of \p pch
 * 
 * @return Number of characters read
 *
 * @bug Should be replaced by fgets() or equivalent and looks very similar 
 *      to getfline() already in sac
 *      Only used by zgtmsg() when readline is on
 *      and zfiles() (which should be replaced too with a fnmatch or glob)
 *
 */ 
int
getline_sac(FILE *pfd,
	char *pch,
	int   maxlen) {

	char *pchsave;	/* save string pointer */
	int   i;	/* index and number of characters read */
	short ichar;	/* used to read characters--is integer so EOF (usually
			   a -1 can be read) */
 
	pchsave = pch;
    ichar = '\0';
	for (i=0;i<(maxlen-1) && (ichar=getc(pfd))!='\0' && ichar!='\n' &&
		ichar!=EOF;++i)
			*(pchsave++) = (char) ichar;
    if(ichar == EOF) {
        i = EOF;
    }

	*(pchsave) = '\0';
 
	return(i);
}
 
