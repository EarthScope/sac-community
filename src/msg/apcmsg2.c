/** 
 * @file   apcmsg2.c
 * 
 * @brief  Copy String to Current Message
 * 
 */

#include <string.h>

#include "msg.h"
#include "co.h"

/** 
 * Copy substrings to null terminated string to pass to
 *    apcmsg().
 *
 * \param kalpha
 *    Alphanumberic string to append to current message
 * \param kalpha_s
 *    Length of string \p kalpha
 *
 * \return Nothing
 *
 * \see apcmsg
 *
 * \bug Why would one use apcmsg() or apcmsg2() over the other one ?
 *
 * \date 09/08/93  New Routine to copy substrings into null 
 *                   terminated strings to pass to apcmsg.
 *                   L. Minner
 */
void 
apcmsg2(char *kalpha, 
	int   kalpha_s)
{
        char message[MCMSG];
        
        int msglen = (kalpha_s < (MCMSG - 1)) ? kalpha_s : (MCMSG - 1);

        strncpy(message,kalpha,msglen);
        message[msglen] = '\0';

        apcmsg(message,msglen + 1);

        return;

}

