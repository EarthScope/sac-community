/** 
 * @file   getsmsg.c
 * 
 * @brief  Get a message from message file
 * 
 */

#include <stdio.h>
#include <string.h>

#include "msg.h"
#include "co.h"

/** 
 * Get a message from message file on disk
 *
 * \param number
 *    Message number idntifying message to get
 * \param *kmsg
 *    Message from the disk file to get
 * \param kmsg_s
 *    Length of string \p kmsg
 * 
 * \return Nothing
 *
 * \see t_cmmsg.nfmsg
 * \see t_cmmsg.ifmsg
 * \see t_kmmsg.kfmsg
 *
 * \bug If no error code found, it should indicate that it
 *       needs to be fixed by stating an undefined error code
 *       was requested.
 *
 * \date   860203:  Original version.
 * \date   860203:  Documented/Reviewed
 *
 */
void 
getsmsg(int   number, 
	char *kmsg, 
	int   kmsg_s)
{
	int j, j_;

	/* - Loop through list of message numbers, looking for a match. */
	for( j = 1; j <= cmmsg.nfmsg; j++ ){
		j_ = j - 1;
		if( number == Ifmsg[j] ){
			fstrncpy( kmsg, kmsg_s-1, kmmsg.kfmsg[j_], strlen(kmmsg.kfmsg[j_]));
			goto L_8888;
			}
		}

	/* - If no match is found, simply encode the error number. */
        sprintf(kmsg,"%s%5d", "Number", number );

L_8888:
	return;
}

