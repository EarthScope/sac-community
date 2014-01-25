/** 
 * @file   xtrace.c
 *
 * @brief  TRACE command
 * 
 */

#include "exm.h"
#include "cpf.h"
#include "bot.h"
#include "bool.h"

#define	MCNAME	16

/** 
 * Execute the TRACE command to control tracing of header and blackboard variables
 * 
 * @param nerr 
 *    Error Return Flag
 *
 * @date   881230:  Original version.
 *
 */
void 
xtrace(int *nerr) {

	char kname[17];
	int  ltracebb;
	int  indexcomma, ncname;
	static int ltracevar = TRUE;

	*nerr = 0;

	/* - Loop on each token in command: */
L_1000:
	if( lcmore( nerr ) ){

		/* -- "ON|OFF":  Turn variable tracing on or off. */
		if( lclog( &ltracevar ) ){

			/* -- "name":  The name of a header or blackboard variable. */
			}
		else if( lcchar( MCNAME, kname,17, &ncname ) ){
			indexcomma = indexa( kname,17, ',', TRUE, TRUE );
			ltracebb = indexcomma == 0;
			tracevariable( ltracevar, ltracebb, kname, nerr );
			if( *nerr != 0 )
				goto L_8888;

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

L_8888:
	return;
}

