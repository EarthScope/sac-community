/** 
 * @file   xecho.c
 * 
 * @brief  ECHO command
 * 
 */
#include <stdio.h>

#include "exm.h"
#include "msg.h"
#include "cpf.h"
#include "bool.h"

/** 
 * Parse the command ECHO controlling the echoing of output to the terminal
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   890324:  Modified command logic.
 * @date   890306:  Original version.
 *
 */
void 
xecho(int *nerr) {

	int lactivate, lcontentreq, lcontents[MTPMSG], lflag;

	int *const Lcontents = &lcontents[0] - 1;

	*nerr = 0;

	/* - Get the current contents being sent to the terminal. */
	inquiremsg( MUNOUT, &lactivate, lcontents );

	/* - Loop on each token in command: */

	lflag = TRUE;
	lcontentreq = FALSE;

L_1000:
	if( lcmore( nerr ) ){

		/* -- "ON|OFF": Flag to say whether echoing is desired or not. */
		if( lclog( &lflag ) ){

			/* -- "ERRORS":  errors messages. */
			}
		else if( lckey( "ERRORS$",8 ) ){
			Lcontents[1] = lflag;
			lcontentreq = TRUE;

			/* -- "WARNINGS":  warning messages. */
			}
		else if( lckey( "WARNINGS$",10 ) ){
			Lcontents[2] = lflag;
			lcontentreq = TRUE;

			/* -- "OUTPUT":  output messages. */
			}
		else if( lckey( "OUTPUT$",8 ) ){
			Lcontents[3] = lflag;
			lcontentreq = TRUE;

			/* -- "COMMANDS":  commands typed at the terminal. */
			}
		else if( lckey( "COMMANDS$",10 ) ){
			Lcontents[4] = lflag;
			lcontentreq = TRUE;

			/* -- "MACROS":  command executed from a macro file. */
			}
		else if( lckey( "MACROS$",8 ) ){
			Lcontents[5] = lflag;
			lcontentreq = TRUE;

			/* -- "PROCESSED":  "processed" commands. */
			}
		else if( lckey( "PROCESSED$",11 ) ){
			Lcontents[6] = lflag;
			lcontentreq = TRUE;

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	/*   EXECUTION PHASE: */

	/* - If no content was input assume "commands", "macros", and "processed".
	 *   This is to maintain consistency with a previous version of this command. */

	if( !lcontentreq ){
		Lcontents[4] = lflag;
		Lcontents[5] = lflag;
		Lcontents[6] = lflag;
		}

	/* - Set the new message content to be sent to the terminal. */

	sendmesg( MUNOUT, lactivate, lcontents );

       
	return;

} /* end of function */

void
echo_switch(int value) {
  int lactivate, lcontents[MTPMSG];
  int *const Lcontents = &lcontents[0] - 1;
  inquiremsg( MUNOUT, &lactivate, lcontents );  
  Lcontents[MCOMMANDS]  = value;
  Lcontents[MMACROS]    = value;
  Lcontents[MPROCESSED] = value;
  sendmesg( MUNOUT, lactivate, lcontents );  
}
