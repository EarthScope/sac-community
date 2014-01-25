/** 
 * @file   cresp.c
 * 
 * @brief  Standard Error Recovery
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bool.h"
#include "exm.h"
#include "bot.h"


#include "co.h"
#include "ucf.h"

/** 
 * Perform standar error recovery when a syntax error is discovered in the 
 *    current command
 * 
 * @date   860819:  Upcased entire response to fix parsing bug.
 * @date   831013:  Added command syntax print option.
 * @date   830211:  Force response to come from user terminal.
 * @date   820913:  Was not clearing error condition correctly.
 * @date   820615:  Changes due to having CFMT raise syntax error condition.
 * @date   820419:  Original version.
 *
 */
void 
cresp() {
	char kiomsg[9];
	int ic1, ic2, ic3, icpntr, itype, ncmsg, nxerr;
	static char kprmpt[42] = "Please enter correction (or type HELP)  $ ";
	static char kchang[9]  = "CHANGE  ";
	static char kinser[9]  = "INSERT  ";
	static char krepla[9]  = "REPLACE ";
	static char kerror[9]  = "ERROR   ";
	static char kquit[9]   = "QUIT    ";
	static char kdelet[9]  = "DELETE  ";
	static char ksyntx[9]  = "SYNTAX  ";

	/* - Return if command correction option is off. */
	if( !cmexm.lcomcr )
		goto L_8888;

	/* - Prompt user for correction. */
L_1000:
	zgtmsg( kprmpt,42, kiomsg,9 );
	ncmsg = indexb( kiomsg,9 );

	/* - Pop first token off returned message. */
	if( ncmsg > 0 ){
		upcase( kiomsg, ncmsg, kiomsg,9 );
		icpntr = 0;
		poptok( kiomsg, ncmsg, &icpntr, &ic1, &ic2, &itype );
		if( itype <= 0 )
			goto L_1000;
		ic3 = ic2 - ic1 + 1;
	} else {
    ic1 = 1;
    ic2 = 9;
    ic3 = ic2 - ic1 + 1;
  }

	/* - Check token versus allowed responses. */

	/* -- "ERROR", "QUIT", or <cr> means the user wishes to error out.
	 *     Since the command syntax error condition has already been raised
	 *     by CFMT, this is a no-op. */
	if( (memcmp(kiomsg+ic1 - 1,kerror,ic3)  == 0  ||
       memcmp(kiomsg+ic1 - 1,kquit,ic3)   == 0) ||
	    ncmsg == 0 ){
	}
  /* -- "CHANGE" means the user wants to change the current token. */
	else if( memcmp(kiomsg+ic1 - 1,kchang,ic3) == 0 ){
		poptok( kiomsg, ncmsg, &icpntr, &ic1, &ic2, &itype );
    arg_change(strcut(kiomsg, ic1, ic2));
		cmcom.ncerr = 0;
	}
	/* --"INSERT" means the user wishes to insert a new token. */
	else if( memcmp(kiomsg+ic1 - 1,kinser,ic3) == 0 ){
		poptok( kiomsg, ncmsg, &icpntr, &ic1, &ic2, &itype );
    arg_insert(strcut(kiomsg, ic1,ic2));
		cmcom.ncerr = 0;
	}
	/* -- "DELETE" means the user wants to delete the current token. */
	else if( memcmp(kiomsg+ic1 - 1,kdelet,ic3) == 0 ){
    arg_delete();
		cmcom.ncerr = 0;
	}
	/* -- "REPLACE" means the user wishes to replace the
	 *     rest of the command. */
	else if( memcmp(kiomsg+ic1 - 1,krepla,ic3) == 0 ){
    arg_truncate();
		poptok( kiomsg, ncmsg, &icpntr, &ic1, &ic2, &itype );
    while( itype > 0 ){
      arg_append(strcut(kiomsg, ic1,ic2));
      poptok( kiomsg, ncmsg, &icpntr, &ic1, &ic2, &itype );
    }
		cmcom.ncerr = 0;
	}
	/* -- "SYNTAX" means print the command syntax from HELP package. */
	else if( memcmp(kiomsg+ic1 - 1,ksyntx,ic3) == 0 ){
		wrhelp( arg_begin()->str, 9, 2, FALSE , &nxerr );
		goto L_1000;

		}
	else{
	  /* -- Send explanation if the user entered an incorrect response. */
    fprintf(MUNOUT,
            "To change the current symbol type: C symbol\n"
            "To insert a symbol before location type: I symbol\n"
            "To replace the rest of the command line type: R text]\n"
            "To delete the current symbol type: D\n"
            "To error out of the current command type: E, Q, or <cr>\n"
            "To see the command syntax type: S\n");
		goto L_1000;
	}

	return;
 L_8888:
  return;

} 


