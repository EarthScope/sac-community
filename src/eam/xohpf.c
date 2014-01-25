/** 
 * @file   xohpf.c
 * 
 * @brief  Execute the command OHPF
 * 
 */

#include "eam.h"
#include "bool.h"


#include "co.h"
#include "msg.h"
#include "cpf.h"

/** 
 * Execute the action command OHPF to open a HYPO Pick File (HPF).
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 n Success
 *    - Non-Zero on Error
 *    - 1901 Error opening HYPO Pick File 
 *
 * @date   920505:  Added hypo input file End-Of-File identifier string.
 *                  (That's 17 spaces and a '10').
 * @date   870507:  Fixed bug in call to old message system.
 * @date   820810:  Changed names for HPF and APF variables.
 * @date   820621:  Changed to newest set of parsing and checking functions.
 * @date   810414:  Added call to ZGTFUN to get an unused fortran file unit.
 * @date   810205:  Replaced call to ZFILNM with more general ZCHSTR.
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   800308:  Original version.
 * @date   820624:  Documented/Reviewed
 */
void 
xohpf(int *nerr)
{
	int njunk;

	*nerr = 0;

	/* PARSING PHASE: */
	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){
		/* -- "name":  name of HPF to open. */
		if( lcchar( MCPFN, kmeam.khpfnm,MCPFN+1, &njunk ) )
		{ /* do nothing */ }

		/* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();
		}
	}

	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */
	/* - Close previous HPF, (after writing eof string), if it is open. */
	if( cmeam.lhpfop ){
                fprintf(cmeam.nhpfun,"%19s\n","10");
		zcloses( &cmeam.nhpfun, nerr );
	}

	if( *nerr != 0 ){
		*nerr = 1901;
		setmsg( "ERROR", *nerr );
		apcmsg( kmeam.khpfnm,MCPFN+1 );
		goto L_8888;
	}

	/* - Open new file. */
	znfiles( &cmeam.nhpfun, kmeam.khpfnm,MCPFN+1, "TEXT",5, nerr );
	if( *nerr != 0 ){
		*nerr = 1901;
		setmsg( "ERROR", *nerr );
		apcmsg( kmeam.khpfnm,MCPFN+1 );
		goto L_8888;
	}

	/* - Position to end-of-file. */
	if ( fseek (cmeam.nhpfun , 0L , SEEK_END ) != 0 )
		fprintf ( stdout , "fseek returned error-xohpf\n" ) ;

	/* - Remove the previous hypo eof marker by simply backspacing. */
	backspace( cmeam.nhpfun, 1L );

	/* - Set flag showing that a HPF is open. */
	cmeam.lhpfop = TRUE;

L_8888:
	return;

}

