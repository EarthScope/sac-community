/** 
 * @file   xwhpf.c
 * 
 * @brief  Execute the command WHPF 
 * 
 */

#include "eam.h"
#include "bool.h"


#include "msg.h"
#include "cpf.h"

/** 
 * Parse and execute the action command WHPF to write an auxillary card
 *    to the HYPO Pick File (HPF).
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   820810:  Changed names for HPF and APF variables.
 * @date   820621:  Changed to newest set of parsing and checking functions.
 * @date   810220:  Changed to output message retrieval from disk.
 * @date   800308:  Original version.
 * @date   820624:  Documented/Reviewed
 *
 */
void 
xwhpf(int *nerr)
{
	int notusd;

	*nerr = 0;

	/* - PARSING PHASE: */
	/* - Loop on each token in command: */
L_1000:
	if( lcmore( nerr ) ){

		/* -- "IC n1 n2":  change IC constants. */
		if( lkia( "IC$",4, 1, 2, cmeam.nhpfic, &notusd ) ){
			cmeam.lhpfic = TRUE;

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */
	/* - Make sure HPF is open. */
	if( !cmeam.lhpfop ){
		*nerr = 1908;
		setmsg( "ERROR", *nerr );
		goto L_8888;
		}

	/* - Write IC card to HPF if requested. */

	if( cmeam.lhpfic )
		{
                fprintf(cmeam.nhpfun,"                 %1d%1d\n", Nhpfic[1], 
		                                                    Nhpfic[2] );
		}

L_8888:
	return;

}

