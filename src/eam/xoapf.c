/** 
 * @file   xoapf.c
 * 
 * @brief  Execute the command OAPF
 * 
 */

#include "eam.h"
#include "bool.h"


#include "co.h"
#include "msg.h"
#include "cpf.h"

/** 
 * Execute the action command OAPF to open an alphanumeric pick file
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Succes
 *    - Non-Zero on Error
 *    - 1902 Cannot open card image pick file
 *    - 1903 CAnnot close previous card image pick file
 *
 * @date   820809:  Changed to newest set of parsing and checking functions.
 *                  Deleted option for a global APF.
 * @date   810414:  Added call to ZGTFUN to get a free fortran file unit.
 * @date   810219:  Changed location of global pick files.
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   800725:  Original version.
 *
 */
void 
xoapf(int *nerr)
{
	int junk;

	*nerr = 0;

	/* PARSING PHASE: */
	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){
		/* -- "STD/NAME":  use standard pick id or simply name of data file. */
		if( lclog2( "STANDARD$",10, "NAME$",6, &cmeam.lpfstd ) )
		{ /* do nothing */ }

		/* -- "filename":  the name of the APF to open. */
		else if( lcchar( MCPFN, kmeam.kapfnm,MCPFN+1, &junk ) )
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
	/* - Close previous APF if it is open. */
	if( cmeam.lapfop ){
		zcloses( &cmeam.napfun, nerr );
		if( *nerr == 0 ){
			cmeam.lapfop = FALSE;
		}
		else{
			*nerr = 1903;
			setmsg( "ERROR", *nerr );
			goto L_8888;
		}
	}


	/* - Open APF. */
	znfiles( &cmeam.napfun, kmeam.kapfnm,MCPFN+1, "TEXT",5, nerr );
	if( *nerr == 0 ){
		cmeam.lapfop = TRUE;
	}
	else{
		*nerr = 1902;
		setmsg( "ERROR", *nerr );
		apcmsg( kmeam.kapfnm,MCPFN+1 );
		goto L_8888;
	}

	/* - Position to end-of-file. */

	if ( fseek ( cmeam.napfun , 0L , SEEK_END ) != 0 )
		fprintf ( stdout , "fseek returned error-xoapf\n" ) ;

L_8888:
	return;
}

