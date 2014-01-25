/** 
 * @file   inquiremsg.c
 * 
 * @brief  Inquire about the current message status
 * 
 */

#include <stdio.h>

#include "msg.h"
#include "bool.h"

/** 
 * Inquire about the current output message system status
 *    associated with a specific file unit.
 *
 * \param *unitnumber
 *    FILE pointer to specific file unit
 * \param *activate
 *    Whether sending messages is activated (output)
 *    - TRUE IF sending of message is activated
 *    - FALSE If sending of message is off
 * \param send_[]
 *   Array containing Contents being sent
 *    - Values of TRUE means that message of that type are 
 *       being set to the requested file \p unitnumber
 *       - 1 Error messages
 *       - 2 Warning messages
 *       - 3 Output messages
 *       - 4 Commands Type at the Terminal
 *       - 5 Commands Executed from a Macro File
 *       - 6 Processed (evaluated) Terminal or macro commands
 *
 * \return Nothing
 *
 * \see t_cmmsg.iunits
 * \see t_cmmsg.nunits
 * \see t_cmmsg.lsend
 *
 * \date   890110:  Original version.
 * \date   890110:  Documented/Reviewed
 *
 */
void 
inquiremsg(FILE *unitnumber, 
	   int  *activate, 
	   int   send_[])
{
	int jtpmsg, jtpmsg_, junit;

	int *const Send_ = &send_[0] - 1;

	junit = 1;
L_100:
	if( unitnumber == cmmsg.iunits[junit-1] ){
		*activate = TRUE;
		for( jtpmsg = 1; jtpmsg <= MTPMSG; jtpmsg++ ){
			jtpmsg_ = jtpmsg - 1;
			Send_[jtpmsg] = cmmsg.lsend[junit - 1][jtpmsg_];
			}
		}
	else if( junit < cmmsg.nunits ){
		junit = junit + 1;
		goto L_100;
		}
	else{
		*activate = FALSE;
		for( jtpmsg = 1; jtpmsg <= MTPMSG; jtpmsg++ ){
			Send_[jtpmsg] = FALSE;
			}
		}

	return;
}

