/** 
 * @file   aplmsg.c
 * 
 * @brief  Append a new line at text to current message
 * 
 */

#include "msg.h"
#include "bot.h"
#include "co.h"

/** 
 * Append a new line of text to the current message
 *
 * \param kalpha
 *   String to begin new line with
 * \param kalpha_s
 *   Length of string \p kalpha
 *
 * \return Nothing
 *
 * \bug Is there some repeat code between this and apcmsg() ?
 *
 * \date   890104:  Added automatic output mode logic.
 * \date   830916:  Original version.
 * \date   890104:  Documented/Reviewed
 */
void 
aplmsg(char *kalpha, 
       int   kalpha_s)
{
	int isave, nalpha;

	/* - If buffer is full and in automatic output mode, 
	 *   send message, clear message buffer. */
	if( cmmsg.nlimsg == MLIMSG && cmmsg.autoout ){
		outmsg();
		isave = cmmsg.itpmsg;
		clrmsg();
		cmmsg.itpmsg = isave;

		/* - Otherwise increment buffer line counter if there is room. */

		}
	else if( cmmsg.nlimsg < MLIMSG ){
		cmmsg.nlimsg = cmmsg.nlimsg + 1;
		}

	/* - Determine length of text string without trailing blanks. */

	nalpha = indexb( kalpha,kalpha_s );

	/* - Start new line of message with string.  Include one trailing blank. */

	fstrncpy( kmmsg.klimsg[cmmsg.nlimsg - 1], MCMSG, kalpha,
                 max( 1,nalpha ));
	cmmsg.nchmsg = nalpha + 1;

	return;
}

