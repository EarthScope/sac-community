/** 
 * @file   autooutmsg.c
 * 
 * @brief  Set the message output mode type
 * 
 */

#include "msg.h"

/** 
 * Set the mesage output mode type
 *
 * \param lmode
 *    Automatic message output mode type
 *    - TRUE Automatically send and clean internal output message
 *           when the buffer is full
 *    - FALSE Disable automatic mode, must use outmsg() and clrmsg()
 *
 * \return Nothing
 *
 * \see outmsg clrmsg
 * \see t_cmmsg.autoout
 *
 * \date   890104:  Original version.
 * \date   890104:  Documented/Reviewed
 * 
 */
void 
autooutmsg(int lmode)
{
	/* - Save new value of automatic output mode flag. */
	cmmsg.autoout = lmode;

	/* - If terminating automatic output, 
	   send and clear buffer of remaining message. */

	if( !cmmsg.autoout ){
		outmsg();
		clrmsg();
	}

	return;
}

