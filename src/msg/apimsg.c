/** 
 * @file   apimsg.c
 * 
 * @brief  Append an integer to the current message
 * 
 */

#include "msg.h"
#include "ucf.h"

/** 
 * Append an integer to the current message 
 *
 * \param integr
 *    Integer to append to the current message
 *
 * \return Nothing
 *
 * \see ncvita ljust apcmsg
 *
 * \bug This could be better served by sprintf 
 *
 * \date  830916:  Original version.
 * \date  830916: Documented/Reviewed
 */
void 
apimsg(int integr)
{
	char kalpha[9];

	/* - Convert integer to alphanumeric. */
	cnvita( integr, kalpha,9 );
	ljust( kalpha,9 );

	/* - Append alphanumeric representation to message. */

	apcmsg( kalpha,9 );

	return;
}

