/** 
 * @file   setmsg.c
 * 
 * @brief  Set a Message Condition
 * 
 */

#include "msg.h"

/** 
 * Set a Message Condition
 *
 * \param ktype
 *    Type of Message.
 *    Only the first letter need be entered.
 *      - 'ERROR'     for an fatal error message condition.
 *      - 'WARNING'   for a warning message condition.
 *      - 'OUTPUT'    for an output message condition.
 *      - 'COMMANDS'  for commands typed at the terminal.
 *      - 'MACROS'    for commands executed from a command file.
 *      - 'PROCESSED' for processed (evaluated) commands.
 * \param number
 *    Message Number
 *
 * \return Nothing
 *
 * \note Message Handling Function
 *        This is a set of subroutines and a common block that is used to
 *        create, modify, and output messages.  A set of predefined messages
 *        can be stored in file and accessed by message number.  Integer,
 *        real, and character fields can be appended to the message.
 *        The message can be up to five lines long.  New lines are created
 *        automatically as fields are appended to the message.
 *        Output can be to a the terminal, the logfile (when implemented),
 *        a specific file unit, or an active graphics device.
 *
 * \note API
 *  - SETMSG sets a new message condition.
 *  - CLRMSG clears any current message condition.
 *  - TYPMSG sets the type of message.
 *  - GETMSG gets an output message from a message file on disk.
 *  - NAMMSG sets the name of the message file.
 *  - INQMSG inquires about current message condition.
 *  - INIMSG initializes the message function.
 *  - APIMSG appends an integer to output message.
 *  - APFMSG appends a floating point number to output message.
 *  - APCMSG appends a character string to output message.
 *  - APLMSG appends a new line of text to output message.
 *  - APNMSG appends a new message from message file on disk.
 *  - OUTMSG sends entire message to the appropriate output devices.
 *  - WRTMSG writes entire message to a specific file unit.
 *  - PLTMSG writes entire message to active graphics devices.
 *
 * \date   890106:  Added several more types of messages.
 *             Generalized use of the error numbers below 100.
 * \date   860130:  Added hook to let XSC's generate their own error
 *             messages and bypass the MESSAGE disk file.
 * \date   841005:  Merged SETERR, SETWAR, and SETMSG.
 * \date   830916:  Original version.
 * \date   860203:  Documented/Reviewed
 */
void 
setmsg(char *ktype, 
       int   number)
{
	char kmsg[MCMSG+1];
	int idx ;

        for( idx = 0 ; idx < MCMSG ; idx++ )
            kmsg[ idx ] = ' ' ;
        kmsg[ MCMSG ] = '\0' ;

        /* This is added to avoid clearing a message that is already set
         *   and losing the appended text in the message buffer.
         *   Setting of multiple error messages requires extra work and 
         *   a direct call of outmsg() of the command 
         */
        if(number == cmmsg.nummsg && number != 0) {
          return;
        }

	/* - Clear previous message condition. */
	clrmsg();

	/* - Set the type of message condition to raise. */

	typmsg( ktype );

	/* - Get output message from AUX disk file MESSAGES.
	 *   It will be appended to first line of message. */

	/* - Message numbers 1 through 99 are treated differently.
	 *   There are no messages in disk file for those numbers.
	 *   These numbers are used for messages generated entirely
	 *   by the calling subroutine or function. */

	if( number >= 100 ){
		getsmsg( number, kmsg,MCMSG+1 );
		apcmsg( kmsg,MCMSG+1 );
		}

	/* - Save message number. */

	cmmsg.nummsg = number;

	return;
}

