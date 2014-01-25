/** 
 * @file   apfmsg.c
 * 
 * @brief  Append a floating point number to the current message
 * 
 */

#include <stdio.h>

#include "msg.h"
#include "ucf.h"

/** 
 * Append a floating point number to the current message
 *
 * \param float_
 *    Number to append to current Message
 *
 * \return Nothing
 *
 * \see apcmsg
 *
 * \date   850805:  Changed conversion to float to a WRITE.
 * \date   830916:  Original version.
 * \date   830916: Documented/Reviewed
 *
 */
void 
apfmsg(double float_)
{
	char kalpha[17];

	/* - Convert integer to alphanumeric. */
        sprintf(kalpha,"%16.5g",float_);
	ljust( kalpha,17 );

	/* - Append alphanumeric representation to message. */

	apcmsg( kalpha,17 );

	return;
}

