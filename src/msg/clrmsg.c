/** 
 * @file   clrmsg.c
 * 
 * @brief  Clear the Current Message
 * 
 */

#include "msg.h"
#include "co.h"

/** 
 * Clear current Message condition
 *
 * \return Nothing
 *
 * \see t_cmmsg.nummsg
 * \see t_cmmsg.itpmsg
 * \see t_cmmsg.nlimsg
 * \see t_cmmsg.nchmsg
 * \see t_cmmsg.klimsg
 *
 * \date   830916:  Original version.
 * \date   860203:  Documented/Reviewed
 */
void 
clrmsg()
{
	int j, j_;

	/* - Reset message number and message length. */
	cmmsg.nummsg = 0;
	cmmsg.itpmsg = 0;

	for( j = 1; j <= cmmsg.nlimsg; j++ ){
		j_ = j - 1;
		fstrncpy( kmmsg.klimsg[j_], MCMSG, " ", 1);
		}
	cmmsg.nlimsg = 1;
	cmmsg.nchmsg = 0;

	return;
}

