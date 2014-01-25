/** 
 * @file   zruntext.c
 * 
 * @brief  Run a line of text
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "co.h"
#include "bot.h"

/** 
 * Add a line of text to a run file
 * 
 * @param text 
 *    Text to add to run file
 * @param text_s 
      Length of \p text
 * @param nfun 
 *    File Descriptor
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   871014:  Original version.
 *
 */
void 
zruntext(char *text, 
	 int   text_s, 
	 FILE *nfun, 
	 int  *nerr) {

	int nctext;
  char *strtemp;
  *nerr = 0;
	/* - Write line to the file. */
	nctext = max( 1, indexb( text,text_s ) );

        strtemp = malloc(nctext+1);
        strncpy(strtemp,text,nctext);
        strtemp[nctext] = '\0';

        fprintf(nfun,"%s\n",strtemp);
  
        free(strtemp);

	return;
}

