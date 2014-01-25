/** 
 * @file   repiv.c
 * 
 * @brief  Report a integer variable
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "exm.h"
#include "bot.h"
#include "msg.h"

/** 
 * Report the value of an integer variable
 * 
 * @param ktext 
 *    Variable name
 * @param ktext_s 
 *    Length of \p ktext
 * @param iv 
 *    Variable value
 *
 * @date   890104:  Changed from direct terminal output to message subsystem.
 * @date   820315:  Original version.
 *
 */
void 
repiv(char *ktext, 
      int   ktext_s, 
      int   iv) {

	char kline[MCMSG+1];
	int nct;
        char *strtemp;
    memset(kline, 0, sizeof(kline));
	/* - Determine length of text. */
	nct = indexc( ktext,ktext_s, '$' );

	/* - Write text and value of variable to message system. */

        strtemp = malloc(nct+1);
        strncpy(strtemp,ktext,nct);
        strtemp[nct] = '\0';

        sprintf(kline,"   %s%s%5d", strtemp, " is ", iv );
	aplmsg( kline,MCMSG+1 );

        free(strtemp);

	return;
}

