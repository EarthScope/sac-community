/** 
 * @file   reprv.c
 * 
 * @brief  Report a real variable
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exm.h"
#include "msg.h"


#include "bot.h"

/** 
 * Report the value of a real variable
 * 
 * @param ktext 
 *    Variable Name
 * @param ktext_s 
 *    Length of \p ktext
 * @param rv 
 *    Variable value
 *
 * @date   890104:  Changed from direct terminal output to message subsystem.
 * @date   820315:  Original version.
 *
 */
void 
reprv(char   *ktext, 
      int     ktext_s, 
      double  rv) {

	char kline[MCMSG+1];
	int nctext;
        char *strtemp;
    memset(kline, 0, sizeof(kline));
	/* - Determine length of text. */
	nctext = indexc( ktext,ktext_s, '$' );

	/* - Write text and value of variable to message subsystem. */

        strtemp = malloc(nctext+1);
        strncpy(strtemp,ktext,nctext);
        strtemp[nctext] = '\0';

        sprintf(kline,"   %s%s%12.5g", strtemp, " is ", rv );
	aplmsg( kline,MCMSG+1 );

        free(strtemp);

	return;
}

