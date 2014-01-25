/** 
 * @file   replv.c
 * 
 * @brief  Report a logical value
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exm.h"
#include "msg.h"
#include "bot.h"

/** 
 * Report a logical variable
 * 
 * @param ktext 
 *    Variable name
 * @param ktext_s 
 *    Length of \p ktext
 * @param lv 
 *    Variable value
 *
 * @date   820315:  Original version.
 *
 */
void 
replv(char *ktext, 
      int   ktext_s, 
      int   lv) {
	char kline[MCMSG+1], kvalue[9];
        int nctext;
        char *strtemp;
    memset(kline, 0, sizeof(kline));
	/* - Determine length of text. */
	nctext = indexc( ktext,ktext_s, '$' );

	/* - Encode value of variable. */

	if( lv ){
		strcpy( kvalue, "ON      " );
		}
	else{
		strcpy( kvalue, "OFF     " );
		}

	/* - Write text and value of variable to message subsystem. */

        strtemp = malloc(nctext+1);
        strncpy(strtemp,ktext,nctext);
        strtemp[nctext] = '\0';

        sprintf(kline,"   %s%s%s", strtemp, " is ", kvalue );
	aplmsg( kline,MCMSG+1 );

        free(strtemp);

	return;
}

