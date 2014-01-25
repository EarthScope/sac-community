/** 
 * @file   qdevices.c
 * 
 * @brief  Report available graphics devices
 * 
 */

#include <stdio.h>
#include <string.h>

#include "eam.h"
#include "msg.h"
#include "gdm.h"

/** 
 * Report the currently available graphics devices
 * 
 * @date   870514:  Original version.
 *
 */
void
qdevices() {
	char kline[MCMSG+1], name[13];
	int j, number;

    memset(kline, 0, sizeof(kline));
        sprintf(kline,"   %s", "Available graphics devices are:");
	aplmsg( kline,MCMSG+1 );

	getmaxdevices( &number );
	for( j = 1; j <= number; j++ ){
		getdevicename( j, name,13 );
		if( strcmp(name,"            ") != 0 &&
                    strcasecmp(name, "RECORD") != 0 &&
                    strcasecmp(name, "TEXT") != 0 ){
                        sprintf(kline,"%s",name);
			aplmsg( kline,MCMSG+1 );
			}
		}

	return;
}

