/** 
 * @file   apcmsg.c
 * 
 * @brief  Append String to current message
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "msg.h"
#include "bot.h"
#include "co.h"

/** 
 * Append alphanumberic string \p kalpha to current message
 *
 * \param kalpha
 *    Alphanumberic string to append to current message
 * \param kalpha_s
 *    Length of string \p kalpha
 *
 * \return Nothing
 *
 * \see indexb outmsg clrmsg
 * \see t_cmmsg.nlimsg
 * \see t_cmmsg.nchmsg
 * \see t_cmmsg.autoout
 * \see t_cmmsg.itpmsg
 * \see t_kmmsg.klimsg
 *
 * \date   900518:  Added logic to test for an empty string. 
 *                    This corrects a problem with VAX VMS version.
 * \date   890104:  Added automatic output mode coding.
 * \date   830916:  Original version.
 * \date   890104:  Documented/Reviewed
 */
void 
apcmsg(char *kalpha, 
       int   kalpha_s)
{
	int isave, nalpha, nchmax;
        char *s1;

	/* - Determine length of string without trailing blanks. */
	nalpha = indexb( kalpha,kalpha_s );

	/* - Decrease maximum length of first line of message to
	 *   allow room for a prefix to be added later when written. */

	nchmax = MCMSG;
	if( cmmsg.nlimsg == 1 )
	     nchmax = nchmax - 10;

	/* - Start new line of message if there is not enough room. */

	if( (cmmsg.nchmsg + nalpha + 1) > nchmax ){
	     if( cmmsg.nlimsg < MLIMSG ){
		cmmsg.nlimsg = cmmsg.nlimsg + 1;
	     }
	     else if( cmmsg.autoout ){
		outmsg();
		isave = cmmsg.itpmsg;
		clrmsg();
		cmmsg.itpmsg = isave;
	     }
	     else{
		fstrncpy( kmmsg.klimsg[cmmsg.nlimsg - 1], MCMSG, " ", 1 );
	     }
	     cmmsg.nchmsg = 0;
	}

	/* - Append alphanumeric string to current message line.
	 *   Include one trailing blank.  Update character counter. */


	if( nalpha > 0 ){
        s1 = strcut(kalpha, 1, nalpha);
	    subscpy( kmmsg.klimsg[cmmsg.nlimsg - 1], cmmsg.nchmsg,
		     cmmsg.nchmsg + nalpha + 1, MCMSG, s1);
        free(s1);
	}
	cmmsg.nchmsg = cmmsg.nchmsg + nalpha + 1;

	return;
}

