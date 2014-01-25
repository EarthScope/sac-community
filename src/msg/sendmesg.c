/** 
 * @file   sendmesg.c
 * 
 * @brief  Activate/Deactivate the sending of Messages
 * 
 */

#include <stdio.h>

#include "msg.h"

/** 
 * Activate or Deactivate the send of output messages 
 *    to a psecific file unit
 *
 * \param *unitnumber
 *    Fortran file unit number to change message actions
 * \param activate
 *    Activate or Deactivate message sending
 *    - TRUE  Turn on message sending
 *    - FALSE Turn off message sending and reverts back to 
 *             original settings
 * \param send_[]
 *    Logical Array of contents to be sent. Each element
 *      represents a different message type.  A value of TRUE
 *      activates the sending of that type of message
 *       - 1 Error messages
 *       - 2 Warning messages
 *       - 3 Output messages
 *       - 4 Commands Type at the Terminal
 *       - 5 Commands Executed from a Macro File
 *       - 6 Processed (evaluated) Terminal or macro commands
 *
 * \return Nothing
 *
 * \date   881230:  Original version.
 * \date   881230:  Documented/Reviewed
 *
 */
void 
sendmesg(FILE *unitnumber, 
	 int   activate, 
	 int   send_[])
{
	int j, j_, jtpmsg, jtpmsg_, junit, nunitsav;

	int *const Send_ = &send_[0] - 1;

	if( activate ){
		junit = 1;
L_100:
		if( unitnumber == cmmsg.iunits[junit-1] ){
			for( jtpmsg = 1; jtpmsg <= MTPMSG; jtpmsg++ ){
				jtpmsg_ = jtpmsg - 1;
				cmmsg.lsend[junit - 1][jtpmsg_] = Send_[jtpmsg];
				}
			}
		else if( junit < cmmsg.nunits ){
			junit = junit + 1;
			goto L_100;
			}
		else if( cmmsg.nunits < MUNITS ){
			cmmsg.nunits = cmmsg.nunits + 1;
			cmmsg.iunits[cmmsg.nunits-1] = unitnumber;
			for( jtpmsg = 1; jtpmsg <= MTPMSG; jtpmsg++ ){
				jtpmsg_ = jtpmsg - 1;
				cmmsg.lsend[cmmsg.nunits - 1][jtpmsg_] = Send_[jtpmsg];
				}
			}
		}
	else{
		nunitsav = cmmsg.nunits;
		for( junit = 1; junit <= cmmsg.nunits; junit++ ){
			if( unitnumber == cmmsg.iunits[junit-1] ){
				nunitsav = cmmsg.nunits - 1;
				for( j = junit; j <= nunitsav; j++ ){
					j_ = j - 1;
					cmmsg.iunits[j_] = cmmsg.iunits[j_ + 1];
					for( jtpmsg = 1; jtpmsg <= MTPMSG; jtpmsg++ ){
						jtpmsg_ = jtpmsg - 1;
						cmmsg.lsend[j_][jtpmsg_] = cmmsg.lsend[j_ + 1][jtpmsg_];
						}
					}
				goto L_1001;
				}
			}

L_1001:
		cmmsg.nunits = nunitsav;

		}

	return;
}

