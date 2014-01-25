/** 
 * @file   xpause.c
 * 
 * @brief  PAUSE command
 * 
 */

#include <stdio.h>

#include "exm.h"
#include "msg.h"
#include "bot.h"
#include "co.h"
#include "bool.h"


#include "cpf.h"

/** 
 * Execute the PAUSE command to pause.
 *    The command sends a message to the terminal and then pauses 
 *    and waits for a return message
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   860925:  Added PERIOD option.
 * @date   840206:  Original version.
 *
 */
void 
xpause(int *nerr) {

	char kret[9];
	int nc;
	double fperio;

	*nerr = 0;
    fperio = cmexm.nperio / 1000.0;
	while ( lcmore( nerr ) ){

		/* -- "PERIOD ON|OFF|v":  set period of time to pause. */
		if( lklogr( "PERIOD$",8, &cmexm.lperio, &fperio ) ){
			cmexm.nperio = (int)( 1000.0*fperio );
			if( cmexm.nperio <= 0 )
				cmexm.lperio = FALSE;
		}

		/* -- Determine text of pause message. */
		else if( lkchar( "MESSAG$",8, MCMSG - 2, kmexm.kpause,MCMSG+1, 
		 &nc ) ){
			subscpy( kmexm.kpause, nc, -1, MCMSG, " $" );
		}

		/* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();
		}
	}
	if( cmexm.lperio ){
		nc = indexb( kmexm.kpause,MCMSG+1 );
		if( nc > 2 ) {
      int n = 1;
      char *p = &kmexm.kpause[0];
      while(*p && *p != '$' && n < nc-1) {
        fprintf(stdout, "%c", *p);
        p++;
        n++;
      }
      fflush(stdout);
    }
		zsleep( cmexm.nperio );
	}
	else{
		zgtmsg( kmexm.kpause,MCMSG+1, kret,9 );
	}


	return;
}

