/** 
 * @file   xwgse.c
 * 
 * @brief  Write a GSE File
 * 
 */

#include <stdio.h>
#include <string.h>

#include "dfm.h"
#include "bool.h"

#include "clf.h"
#include "co.h"
#include "msg.h"
#include "bot.h"
#include "ssi.h"
#include "cpf.h"
#include "gse.h"
#include "smDataIO.h"

/** 
 * Execute the command WRITEGSE which writes a GSE file
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   990420:  Original Version, plagerized from xw.c.   maf
 *
 */
void 
xwgse(int *nerr) {

	char delimiter[2], ktemp[9], outfile[256];
	char kdstemp[ 21 ] ;
	static char kdatasource[ 21 ] = "" ;
	static int ldatasource = FALSE ;
	int ntraces ;
	int nchar;
	static int lwrdir = FALSE;

    string_list *list;

	*nerr = 0;

	/* PARSING PHASE: */
	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){

	    /* -- "SOURCE"  specify the data source for the MSG_ID line */
	    if ( lkcharExact ( "SOURCE#$" , 9 , 20 , kdstemp , 21 , &nchar ) ) {
		modcase( TRUE , kdstemp , MCPW , ktemp ) ;
		if( strcmp(ktemp,"ON      ") == 0 )
		    ldatasource = TRUE ;
		else if( strcmp(ktemp,"OFF     ") == 0 )
		    ldatasource = FALSE ;
		else {
		    ldatasource = TRUE ;
		    strcpy ( kdatasource , kdstemp ) ;
		    terminate ( kdatasource ) ;
		}
	    }
		

            /* -- CM6:  if this option is on, write waveforms in CM6 compressed
                        format.  If it is off, write INT data (default). */
            else if( lklog( "CM6#$",6, &cmdfm.lcm6 ) )
            { /* do nothing */ }


	    /* -- "DIR ON|OFF|CURRENT|name":  set the name of the default subdirectory. */
	    else if( lkcharExact( "DIR#$",6, MCPFN, kmdfm.kwrdir,MCPFN+1, &nchar ) ){
		modcase( TRUE, kmdfm.kwrdir, MCPW, ktemp );
		if( strcmp(ktemp,"ON      ") == 0 )
		    lwrdir = TRUE;
		else if( strcmp(ktemp,"OFF     ") == 0 )
		    lwrdir = FALSE;
		else if( strcmp(ktemp,"CURRENT ") == 0 ){
		    lwrdir = TRUE;
		    fstrncpy( kmdfm.kwrdir, MCPFN, " ", 1);
		}
		else if( kmdfm.kwrdir[nchar - 1] != KDIRDL ){
		    lwrdir = TRUE;
		    delimiter[0] = KDIRDL;
		    delimiter[1] = '\0';
		    subscpy( kmdfm.kwrdir, nchar, -1, MCPFN, delimiter );
		}
		else
		    lwrdir = TRUE;
	    }

	    /* -- "COMMIT|RECALLTRACE|ROLLBACK": 
	          how to treat existing data */
	    else if ( lckeyExact ( "COMMIT" , 7 ) )
		cmdfm.icomORroll = COMMIT ;
	    else if (lckeyExact ( "RECALLTRACE" , 12 ) )
		cmdfm.icomORroll = RECALL ;
	    else if ( lckeyExact ( "RECALL" , 7 ) )
		cmdfm.icomORroll = RECALL ;
	    else if ( lckeyExact ( "ROLLBACK" , 9 ) ) 
		cmdfm.icomORroll = ROLLBACK ;


	    /* -- "filelist":  write files using names in new filelist. */
	    else if( ( list = lcdfl() ) )
	    { /* do nothing */ }

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	}

	if( *nerr != 0 )
	    goto L_8888;

	/* CHECKING PHASE: */
	/* - Check for null write filelist. */
	if( string_list_length(list) <= 0 ){
	    *nerr = 1311;
	    setmsg( "ERROR", *nerr );
	    goto L_8888;
	}

	/* - Make sure the write filelist has one entry */
	if( string_list_length(list) != 1 ){
	    *nerr = 1312;
        error(*nerr, "%d %d", string_list_length(list), 1);
	    goto L_8888;
	}

	/* EXECUTION PHASE: */
        /* - Commit or rollback data according to lmore and cmdfm.icomORroll */
	alignFiles ( nerr ) ;
	if ( *nerr )
	    return ;

	/* make filename */
	if ( lwrdir ) {
        sprintf(outfile, "%s%s", kmdfm.kwrdir, string_list_get(list, 0));
	}
	else {
        sprintf(outfile, "%s", string_list_get(list, 0));
	}

	ntraces = WriteGSEFile( outfile , smGetDefaultTree() ,
                                ldatasource ? kdatasource : "" , cmdfm.lcm6 ) ;
	if ( ntraces > 0 )
	    printf ( "%d waveforms written in %s\n" , ntraces , outfile ) ;
	else {
        *nerr = 1344;
        error(*nerr, "%s", outfile);
	    outmsg () ;
	    clrmsg () ;
	}

L_8888:
	return;
}

