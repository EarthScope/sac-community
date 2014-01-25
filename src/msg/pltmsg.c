/** 
 * @file   pltmsg.c
 * 
 * @brief  Write current message to the active graphics device
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "msg.h"
#include "gem.h"


#include "pl.h"

/** 
 * Write current output message to the active graphics device
 *
 * \param *xloc
 *    X plot location for beginning of text
 * \param *yloc
 *    Y plot location for beginning of text
 *
 * \return Nothing
 *
 * \bug Message types referred to as actual numbers, not names
 * \bug Could probably get away with a sprint here
 *
 * \see pltext
 *
 * \date   860203:  Original version.
 * \date   860203:  Documented/Reviewed
 *
 */
void 
pltmsg(float *xloc, 
       float *yloc)
{
	int j, j_;
	float ytemp;
        char *cattemp;

	/* - Add a prefix to first line of message if appropriate.
	 *   (There is an ASCII BEL embedded in the error prefix.) */
	if( cmmsg.itpmsg == 1 ){
                cattemp = malloc(8+strlen(kmmsg.klimsg[0])+1);
                strcpy(cattemp,"\aERROR: ");
                strcat(cattemp,kmmsg.klimsg[0]);
		pltext( cattemp, 8+strlen(kmmsg.klimsg[0])+1, *xloc, *yloc );
                free(cattemp);
		}
	else if( cmmsg.itpmsg == 2 ){
                cattemp = malloc(9+strlen(kmmsg.klimsg[0])+1);
                strcpy(cattemp,"WARNING: ");
                strcat(cattemp,kmmsg.klimsg[0]);
		pltext( cattemp, 9+strlen(kmmsg.klimsg[0])+1, *xloc, *yloc );
                free(cattemp);
		}
	else{
		pltext( (char*)kmmsg.klimsg[0],MCMSG+1, *xloc, *yloc );
		}

	/* - Write remaining lines of current message. */

	ytemp = *yloc;
	for( j = 2; j <= cmmsg.nlimsg; j++ ){
		j_ = j - 1;
		ytemp = ytemp - cmgem.chht;
		pltext( (char*)kmmsg.klimsg[j_],MCMSG+1, *xloc, ytemp );
		}

	return;
}

