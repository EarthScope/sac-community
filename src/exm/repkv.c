/** 
 * @file   repkv.c
 * 
 * @brief  Report an alphanumeric variable
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "exm.h"
#include "bot.h"
#include "msg.h"

/** 
 * Report the value of an alphanumeric variable
 * 
 * @param ktext 
 *    Variable name
 * @param ktext_s 
 *    Length of \p ktext
 * @param kv 
 *    Variable Value
 * @param kv_s 
 *    Length of \p kv
 *
 * @date   890104:  Changed from direct terminal output to message subsystem.
 * @date   820505:  Original version.
 *
 */
void 
repkv(char *ktext, 
      int   ktext_s, 
      char *kv, 
      int   kv_s) {

	char kline[MCMSG+1];
	int nct, ncv;
    char *strtemp1, *strtemp2;
    memset(kline, 0, sizeof(kline));
	/* - Determine length of text and string. */
	nct = indexc( ktext,ktext_s, '$' );
	ncv = indexb( kv,kv_s );

	/* - Write text and value of variable to message subsystem. */

        strtemp1 = malloc(nct+1);
        strtemp2 = malloc(ncv+1);
        strncpy(strtemp1,ktext,nct);
        strncpy(strtemp2,kv,ncv);
        strtemp1[nct] = '\0';
        strtemp2[ncv] = '\0';

        sprintf(kline,"   %s%s%c%s%c", strtemp1
	 , " is ", '\'', strtemp2, '\'' );
	aplmsg( kline,MCMSG+1 );

        free(strtemp1);
        free(strtemp2);

	return;
}

