/** 
 * @file   reprtw.c
 * 
 * @brief  Report a relative time window
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exm.h"
#include "msg.h"

/** 
 * Report the value of a "relative time window"
 * 
 * @param ktext 
 *    Variable name 
 * @param ktext_s 
 *    Length of \p ktext
 * @param lrtw 
 *    - TRUE if the window is relative
 *    - FALE if the window is not relative
 * @param krtw 
 *    Starting and stopping times 
 * @param krtw_s 
 *    Length of \p krtw
 * @param ortw 
 *    Floating point array giving starting and stopping offsets in seconds
 *
 * @date   890104:  Changed from direct terminal output to message subsystem.
 * @date   830121:  Original version.
 *
 */
void 
reprtw(char  *ktext, 
       int    ktext_s, 
       int    lrtw, 
       char  *krtw, 
       int    krtw_s, 
       float *ortw) {

#define KRTW(I_,J_)	(krtw+(I_)*(krtw_s)+(J_))

	char kline[MCMSG+1];
        char chartemp[3];

	float *const Ortw = &ortw[0] - 1;
    memset(kline, 0, sizeof(kline));
	/* - Report logical variable with text. */
	replv( ktext,ktext_s, lrtw );

	/* - Report stop and stop window if on. */

	if( lrtw ){
                strcpy(chartemp,"  ");
                memcpy(chartemp,KRTW(0,0),2);
                sprintf(kline,"   %s%s%12.5g", "Start is ", chartemp, Ortw[1] );
		aplmsg( kline,MCMSG+1 );

                memcpy(chartemp,KRTW(1,0),2);
                sprintf(kline,"   %s%s%12.5g", "Stop  is ", chartemp, Ortw[2] );
		aplmsg( kline,MCMSG+1 );
		}

	return;
#undef	KRTW

}

