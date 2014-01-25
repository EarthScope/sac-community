/** 
 * @file   xreport.c
 * 
 * @brief  REPORT command
 * 
 */

#include "exm.h"
#include "cpf.h"
#include "msg.h"
#include "bool.h"

/** 
 * Execute the report command showing the current status of sac variables
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   920528:  Added line WIDTH report.
 * @date   920403:  Added DATASET report.
 * @date   870728:  Added LINE, SYMBOL, GTEXT, YLIM, and MTW reports.
 *             Cleaned up calling arguments to q... routines.
 * @date   850729:  Added AM (array manager) report.
 * @date   840912:  Added terminal list report.
 * @date   830121:  Added CUT and XLIM reports.
 * @date   821208:  Added TITLE, XLABL, and YLABL reports.
 * @date   820817:  Changed to newest set of parsing and checking functions.
 * @date   820325:  Fixed bug in initializing NREP.
 * @date   820323:  Added DISPLAY command report.
 * @date   820316:  Added COLOR command report.
 * @date   810514:  Original version.
 *
 */
void 
xreport(int *nerr) {

	int index, jrep;

	*nerr = 0;

	/* - Reset report request counter if there are tokens in command. */

	if( lcmore( nerr ) )
		cmexm.nrep = 0;

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

		/* -- "item":  the name of a reportable item. */
		if( lclist( (char*)kmexm.kreptp,9, cmexm.nreptp, &index ) ){
			cmexm.nrep = cmexm.nrep + 1;
			Irep[cmexm.nrep] = index;
		}

		/* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();
		}
	}

	if( *nerr != 0 )
		goto L_8888;

	/* - Set up automatic output mode. */
	autooutmsg( TRUE );
	setmsg( "OUTPUT", 99 );

	/* - For each item in list, perform a case branch and report value. */

	for( jrep = 1; jrep <= cmexm.nrep; jrep++ ){
		switch( Irep[jrep] ){
			case 1: qhpf () ;
				break ;
			case 2: qapf () ;
				break  ;
			case 3: qcolor () ;
				break ;
			case 4: qfid () ;
				break ;
			case 5: qpicks () ;
				break ;
			case 6: qtitle () ;
				break ;
			case 7: qxlabl () ;
				break ;
			case 8: qylabl () ;
				break ;
			case 9: qcut () ;
				break ;
			case 10: qxlim () ;
				 break ;
			case 11: qam () ;
				 break ;
			case 12: qdevices () ;
				 break ;
			case 13: qline () ;
				 break ;
			case 14: qsymbol () ;
				 break ;
			case 15: qgtext () ;
				 break ;
			case 16: qylim () ;
				 break ;
			case 17: qmtw () ;
				 break ;
			case 18: qwidth () ;
				 break ;
		} 
	} 
	autooutmsg( FALSE );

L_8888:
	return;
}

