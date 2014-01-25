/** 
 * @file   repav.c
 * 
 * @brief  Report alphanumeric variable
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eam.h"
#include "bot.h"
#include "msg.h"

/** 
 * Report the value of an alphanumeric variable
 * 
 * @param ktext 
 *    Variable name
 * @param ktext_s 
 *    Length of \p ktext
 * @param av 
 *    Variable value
 * @param av_s 
 *    Length of \p av
 *
 * @date   890104:  Changed from direct terminal output to message subsystem.
 * @date   820315:  Original version.
 *
 */
void 
repav(char  *ktext, 
      int    ktext_s, 
      char  *av, 
      int    av_s) {

	char kline[MCMSG+1];
	int nct, ncv;
        char *strtemp1, *strtemp2;
    memset(kline, 0, sizeof(kline));
	/* - Determine length of text and value. */
	nct = indexc( ktext,ktext_s, '$' );
	ncv = indexb( av,av_s );

	/* - Encode text and value of variable and send to message subsystem. */

        strtemp1 = malloc(nct+1);
        strtemp2 = malloc(ncv+1);
        strncpy(strtemp1,ktext,nct);
        strtemp1[nct] = '\0';
        strncpy(strtemp2,av,ncv);
        strtemp2[ncv] = '\0';

        sprintf(kline,"   %s%s%s", strtemp1, " is ", strtemp2 );
	aplmsg( kline,MCMSG+1 );

        free(strtemp1); 
        free(strtemp2);

	return;
}

