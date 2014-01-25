/** 
 * @file   wrtmsg.c
 * 
 * @brief  Write output message
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "msg.h"
#include "bot.h"

/** 
 * Write output message to a specific file unit \p nunit
 *
 * \param *nunit
 *   Fortran file unit to write the message to
 *
 * \return Nothing
 *
 * \see indexb
 * \see t_cmmsg.itpmsg
 * \see t_cmmsg.nlimsg
 * \see t_kmmsg.klimsg
 *
 * \date   870811:  Added logic to determine the end of each line.
 * \date   860203:  Original version.
 * \date   860203:  Documented/Reviewed
 *
 */
void 
wrtmsg(FILE *nunit)
{
	int j, nc;
        char *s1, *message;

	/* - Add a prefix to first line of message if appropriate.
	 *   (There is an ASCII BEL embedded in the error prefix.) */
	nc = indexb( (char*)kmmsg.klimsg[0],MCMSG+1 );
    s1 = strcut(kmmsg.klimsg[0], 1, nc);
    message = (char *)malloc(nc+100+1);

	if( cmmsg.itpmsg == MERRORS ){
                sprintf(message,"%s%s", " ERROR: ", s1 );
                  fprintf(nunit,"\a%s\n", message );
		}
	else if( cmmsg.itpmsg == MWARNINGS ){
                sprintf(message,"%s%s", "WARNING: ", s1 );
                  fprintf(nunit,"%s\n", message );
		}
	else{
                  fprintf(nunit,"%s\n",s1);
		}

        free(s1);
        free(message);

	/* - Write remaining lines of current message. */

	for( j = 2; j <= cmmsg.nlimsg; j++ ){
		nc = indexb( (char*)kmmsg.klimsg[j-1],MCMSG+1 );
        s1 = strcut(kmmsg.klimsg[j-1], 1, nc);
        fprintf(nunit,"%s\n",s1);
        free(s1);
    }

	return;
}

