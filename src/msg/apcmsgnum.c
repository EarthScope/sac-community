/** 
 * @file   apcmsgnum.c
 * 
 * @brief  Append a numered message to Current Message
 * 
 */

#include "msg.h"

/** 
 * Append a numbered message to the Current Message
 *
 * \param number
 *    Message Number
 *
 * \return Nothing
 *
 * \see getsmsg apcmsg
 * 
 * \date   900430:  Changed name from apnmsg to apcmsgnum.
 * \date   860203:  Original version.
 * \date   900430:  Documented/Reviewed
 *
 */
void 
apcmsgnum(int number)
{
	char kmsg[MCMSG+1];

	/* - Get message from disk file. */
	getsmsg( number, kmsg,MCMSG+1 );

	/* - Append to current message. */

	apcmsg( kmsg,MCMSG+1 );

	return;
}
