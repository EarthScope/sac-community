/** 
 * @file   xexmc.c
 * 
 * @brief  Execute a EXM command
 * 
 */
#include "exm.h"
#include "msg.h"
#include "co.h"
#include "bool.h"


#include "top.h"
#include "cpf.h"

int set_op_order(int want_order);
void
xmath_op() {
  int nerr;
  if(! lcmore(&nerr) ) {
    printf("Math Operator Precedance: %s\n", set_op_order(-1)==0 ? "math" : "none");
    return;
  }
  while(lcmore(&nerr)) {
    if(lckey("NORMAL", 7)) {
      set_op_order(0);
    } else if(lckey("MATH", 5)) {
      set_op_order(0);
    } else if(lckey("FORTRAN", 5)) {
      set_op_order(0);
    } else if(lckey("NONE", 5)) {
      set_op_order(1);
    } else if(lckey("OLD", 5)) {
      set_op_order(1);
    } else {
      cfmt("ILLEGAL OPTION: ", 17);
    }
  }
}

/** 
 * Exectue a EXM command given its index number
 * 
 * @param index 
 *    Command index
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   990119:  Added ABOUT command.
 * @date   900804:  Added LOAD command.
 * @date   890306:  Modified ECHO command.
 * @date   881230:  Added TRANSCRIPT and TRACE commands.
 * @date   880412:  Added UNSETBB command.
 * @date   870514:  Added GETBB command.
 * @date   870415:  Added MACRO, SETMACRO, and INSTALLMACRO  commands.
 * @date   870301:  Added SETBB, READBBF, and WRITEBBF commands.
 * @date   861203:  Deleted QUITSUB command. It is now separate commands
 *                  in each of the subprocesses.
 * @date   840619:  Added EVALUATE command.
 * @date   840206:  Added PAUSE and ECHO commands.
 * @date   831013:  Added SYNTAX command.
 * @date   830104:  Added COMCOR command.
 * @date   821MCPFN+1:  Added QUITSUB command.
 * @date   820817:  Changed to newest set of parsing and checking functions.
 * @date   820721:  Deleted RUN, RUNSAC, and RUNSOCK commands.
 * @date   820304:  Changed HELP to H to avoid PRIMOS conflict.
 * @date   820113:  Deleted ERRCON command. Now an option in READERR.
 *                  Deleted INTOFF and USAGE commands.
 *                  Changed calls to standard naming convention.
 * @date   810514:  Added REPORT command.
 * @date   810115:  Removed AVAIL command.
 *
 */
void 
xexmc(int  index, 
      int *nerr) {

	*nerr = 0;

	/* - Jump to correct command based upon its index number. */
	switch( index ){
		case 1: goto L_100;
		case 2: goto L_200;
		case 3: goto L_300;
		case 4: goto L_400;
		case 5: goto L_500;
		case 6: goto L_600;
		case 7: goto L_700;
		case 8: goto L_800;
		case 9: goto L_900;
		case 10: goto L_1000;
		case 11: goto L_1100;
		case 12: goto L_1200;
		case 13: goto L_1300;
		case 14: goto L_1400;
		case 15: goto L_1500;
		case 16: goto L_1600;
		case 17: goto L_1700;
		case 18: goto L_1800;
		case 19: goto L_1900;
		case 20: goto L_2000;
		case 21: goto L_2100;
		case 22: goto L_2200;
		case 23: goto L_2300;
		case 24: goto L_2400;
		case 25: goto L_2500;
		case 26: goto L_2600;
    case 27: goto L_2700;
		case 28: goto L_2800;
    case 29: goto L_2900;
	}

	/* - Error return if bad index value. */

	*nerr = 901;
	setmsg( "ERROR", *nerr );
	apcmsg( "in XEXMC",9 );
	goto L_8888;


L_100:  /* - Command 01: QUIT --- Terminate program. */
	if( cmexm.linsys ){
		*nerr = 1013;
		setmsg( "ERROR", *nerr );
		apcmsg( "\"sac off\"",10 );
	}
	else{
		zquit();
	}
	goto L_8888;


L_200:  /* - Command 02: PRODUCTION --- set production/interactive mode. */
	xclog( &cmexm.lprod, nerr );
	goto L_8888;


L_300:  /* - Command 03: NEWS --- Print the latest news to the user's terminal. */
	xnews( nerr );
	goto L_8888;


L_400:  /* - Command 04: HELP --- Print items from the online help package. */
	xhelp( FALSE , nerr );
	goto L_8888;


L_500:  /* - Command 05: REPORT --- Print reports about certain status variables. */
	xreport( nerr );
	goto L_8888;


L_600:  /* - Command 06: SYSTEMCOMMAND --- Execute a system command from within SAC. */
	xsystemcommand( nerr );
	goto L_8888;


L_700:  /* - Command 07: INICM --- Reinitialize SAC's common blocks. */
        initsac();
	goto L_8888;


L_800:  /* - Command 08: FUNCGEN --- Generate one of several preset functions. */
	xfg( nerr );
	goto L_8888;


L_900:  /* - Command 09: MESSAGE --- Send a message to the user's terminal. */
	xmsg( nerr );
	goto L_8888;


L_1000: /* - Command 10: PRINTHELP --- Send help page to printer. */
	xhelp ( TRUE , nerr ) ;
	goto L_8888;


L_1100: /* - Command 11: COMCOR --- command correction mode. */
	xclog( &cmexm.lcomcr, nerr );
	goto L_8888;


L_1200: /* - Command 12: SYNTAX --- print command syntax. */
	xsyntx( nerr );
	goto L_8888;


L_1300: /* - Command 13: PAUSE --- pause and wait for message from terminal. */
	xpause( nerr );
	goto L_8888;


L_1400: /* - Command 14: ECHO --- option to echo commands to terminal. */
	xecho( nerr );
	goto L_8888;


L_1500: /* - Command 15: EVALUATE --- evaluate simple arithmetic expressions. */
	xeval( nerr );
	goto L_8888;


L_1600: /* - Command 16: SETBB --- set blackboard variable values. */
	xsetbb( nerr );
	goto L_8888;


L_1700: /* - Command 17: GETBB --- inquire about values of blackboard variables. */
	xgetbb( nerr );
	goto L_8888;


L_1800: /* - Command 18: READBBF --- read a blackboard variable file. */
	xreadbbf( nerr );
	goto L_8888;


L_1900: /* - Command 19: WRITEBBF --- write a blackboard variable file. */
	xwritebbf( nerr );
	goto L_8888;


L_2000: /* - Command 20: MACRO --- execute a SAC macro (command) file. */
	xmacro( nerr );
	goto L_8888;


L_2100: /* - Command 21: SETMACRO --- set SAC macro search path attributes. */
	xsetmacro( nerr );
	goto L_8888;


L_2200: /* - Command 22: INSTALLMACRO --- install a SAC macro in global directory. */
	xinstallmacro( nerr );
	goto L_8888;


L_2300: /* - Command 23: UNSETBB --- unset blackboard variables. */
	xunsetbb( nerr );
	goto L_8888;


L_2400: /* - Command 24: TRANSCRIPT --- turn processing transcript on or off. */
	xtranscript( nerr );
	goto L_8888;


L_2500: /* - Command 25: TRACE --- turn header/blackboard variable tracing on or off. */
	xtrace( nerr );
	goto L_8888;


L_2600: /* - Command 26: LOAD --- dynamically load external SAC commands. */
	xload( nerr );
	goto L_8888;

L_2700: /* - Command 27: CD --- change working directory within SAC. */
        xcd( nerr );
        goto L_8888;

L_2800: /* - Command 28: ABOUT --- display version information */
	xabout () ;
	goto L_8888 ;

 L_2900: /* Command 29: MATHOP -- Change Math operator precedence */
  xmath_op();
  goto L_8888;

L_8888:
	return;

} /* end of function */

