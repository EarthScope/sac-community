/** 
 * @file   xconv.c
 * 
 * @brief  Convert a file's format
 * 
 */

#include <string.h>

#include "dfm.h"
#include "bool.h"


#include "ssi.h"
#include "cpf.h"
#include "dff.h"

/** 
 * Execute the CONV command which converts a file from one format to another
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   880913:  Added call to clear data file list before execution.
 * @date   850801:  Deleted SOCKITTOME format.
 *                  Changes in argument lists for RDSAC and RDCI.
 * @date   820809:  Changed to newest set of parsing and checking functions.
 * @date   810728:  Added optional format for ALPHA read/write.
 */
void 
xconv(int *nerr) {

	int iinout, junk, ndx1, ndx2, ndxh, nlen;
	static int iin = 1;
	static int iout = 2;

	*nerr = 0;

	/* - Initialize in/out flag. */
	iinout = iin;

	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){

	    /* -- "FROM":  set in/out flag to in. */
	    if( lckey( "&FROM$",7 ) )
		iinout = iin;

	    /* --"TO":  set in/out flag to out. */
	    else if( lckey( "&TO$",5 ) )
		iinout = iout;

	    /* -- "OVER": set in/out flag to out and use same filename as in. */
	    else if( lckey( "&OVER$",7 ) ){
		iinout = iout;
		strcpy( kmdfm.kcfile[iout - 1], kmdfm.kcfile[iin - 1] );
	    }

	    /* -- "SAC/ALPHA":  select in/out file format. */
	    else if(lclist((char*)kmdfm.krwfmt,9, cmdfm.nrwfmt, &Icfmt[iinout]))
	    { /* do nothing */ }

	    else if( lckey( "CI#$",5 ) )
		Icfmt[iinout] = 2;

	    /* -- "FMT string":  define a card-image format for in/out file. */
	    else if( lkchar( "FMT$",5, 16, (char*)kmdfm.kcfmt[iinout - 1]
	     ,strlen( (char*)kmdfm.kcfmt[iinout - 1])+1, &junk ) )
	    { /* do nothing */ }

	    /* -- "filename":  define in/out filename. */
	    else if( lcchar( MCPFN, (char*)kmdfm.kcfile[iinout - 1],MCPFN+1, 
	     &junk ) )
	    { /* do nothing */ }

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	} /* end while */

	if( *nerr != 0 )
	    goto L_8888;

	/* EXECUTION PHASE: */
	cleardfl( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	if( Icfmt[iin] == 1 ){
	    rdsac( 1, (char*)kmdfm.kcfile[iin - 1],MCPFN+1, TRUE, TRUE,
		   &nlen, &ndxh, &ndx1, &ndx2, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}
	else if( Icfmt[iin] == 2 ){
	    rdci( 1 , (char*)kmdfm.kcfile[iin - 1] , MCPFN+1 ,
		  &nlen, &ndx1, &ndx2, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}
	cmdfm.ndfl = 1;

	/* Added to run data through SeisMgr. */
	cmdfm.nreadflag = HIGH ;
	cmdfm.lread = TRUE ;
	sacToSeisMgr ( TRUE , FALSE , TRUE , nerr ) ;
	cmdfm.lread = FALSE ;

	if ( *nerr ) 
	    goto L_8888 ;

	/* Write file if applicable */
	if( Icfmt[iout] == 1 )
	    wrsac( 1, (char*)kmdfm.kcfile[iout - 1],MCPFN+1, TRUE, nerr );

	else if( Icfmt[iout] == 2 )
	    wrci( 1, (char*)kmdfm.kcfile[iout - 1],MCPFN+1,
	     (char*)kmdfm.kcfmt[iout - 1] , nerr );

L_8888:
	return;

}

