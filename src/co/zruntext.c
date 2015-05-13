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
#include "debug.h"

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
zruntext(char *text, int text_s, FILE * nfun, int *nerr) {

    *nerr = 0;
    UNUSED(text_s);
    /* - Write line to the file. */
    rstrip(text);
    fprintf(nfun, "%s\n", text);


    return;
}
