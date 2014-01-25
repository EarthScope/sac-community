/** 
 * @file   qgtext.c
 * 
 * @brief  Report gtext parameters
 * 
 */

#include <stdio.h>
#include <string.h>

#include "eam.h"
#include "gem.h"
#include "msg.h"


#include "exm.h"

/** 
 * Report current values of the gtext value parameters
 * 
 * @date   870728:  Original version.
 * 
 */
void
qgtext() {
	char kline[MCPFN+1];
    memset(kline, 0, sizeof(kline));
	if( kmgem.kgtqua[0] == 'H') {
        sprintf(kline,"   %s", "HARDWARE text being used.");
		aplmsg( kline,MCPFN+1 );
    }
	else{
        sprintf(kline,"   %s", "SOFTWARE text being used.");
		aplmsg( kline,MCPFN+1 );
    }
	repiv( "Text FONT$",11, cmgem.igtfnt );
	reprv( "Text SIZE$",11, cmgem.tsdef );

	return;
}






