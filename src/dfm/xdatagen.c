/** 
 * @file   xdatagen.c
 * 
 * @brief  Generate data
 * 
 */

#include "dfm.h"
#include "bool.h"


#include "co.h"
#include "bot.h"
#include "msg.h"
#include "ssi.h"
#include "cpf.h"
#include "clf.h"

/** 
 * Execute the command DATAGEN which reads sample data files into memory
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   880308:  Added LDATA flag to call to READFL.
 * @date   870921:  Fixed bug in building of directory name.
 * @date   870625:  Factored execution portion to readfl.
 *
 */
void 
xdatagen(int *nerr) {

	char _c0[2], kdgdir[MCPFN+1], ktemp[9];
	int ldata, lmore, lsdd, lFirstLoop;
	int nc, idx ;
	int lexist;

    string_list *list;

	*nerr = 0;
    list = NULL;
        for( idx = 0 ; idx < MCPFN ; idx++ )
            kdgdir[ idx ] = ' ' ;
        kdgdir[ MCPFN ] = '\0' ;

	/* - Parse position-dependent tokens: */
	lmore = FALSE;

	do {
	    lFirstLoop = FALSE ;
  
	    /* -- "MORE":  signifies addition of more files to current 
	     *             read filelist rather than replacement of 
	     *             current list with new one. 
	     */
	    if( (lcmore( nerr ) && lckey( "&MORE$",7 )) && cmdfm.ndfl > 0 ) {
		lmore = TRUE;
		lFirstLoop = TRUE ;
	    }

	    /* -- "SUB name": select subdirectory name. */
	    else if( lklist( "&SUB$",6, (char*)kmdfm.kdgsub,9, 
			     MDGSUB, &cmdfm.idgsub ) )
		lFirstLoop = TRUE ;

	    /* -- "COMMIT|RECALLTRACE|ROLLBACK": how to treat existing data */
	    else if ( lckeyExact ( "COMMIT" , 7 ) ) {
		cmdfm.icomORroll = COMMIT ;
		lFirstLoop = TRUE ;
	    }
	    else if (lckeyExact ( "RECALLTRACE" , 12 ) ) {
		cmdfm.icomORroll = RECALL ;
		lFirstLoop = TRUE ;
	    }
	    else if ( lckeyExact ( "RECALL" , 7 ) ) {
		cmdfm.icomORroll = RECALL ;
		lFirstLoop = TRUE ;
	    }
	    else if ( lckeyExact ( "ROLLBACK" , 9 ) ) {
		cmdfm.icomORroll = ROLLBACK ;
		lFirstLoop = TRUE ;
	    }

	} while ( lFirstLoop ) ;

	/* - Parse position-independent tokens: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

	    /* -- "filelist":  define a new input filelist. */
    if( ( list = lcdfl() ) )
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
	/* - Commit or rollback data according to lmore and cmdfm.icomORroll */
	if ( lmore ) {
	    alignFiles ( nerr ) ;
	    if ( *nerr )
		return ;
	    cmdfm.nfilesFirst = cmdfm.ndfl ;
	}
	else{
	    cmdfm.nfilesFirst = 0 ;
	}

	/* - Set up the directory name of the sample data files. */

	/* -- Get name of base directory. */
	zbasename( kdgdir,MCPFN+1 );

	/* -- Append datagen to get subdirectory name. */
	crname( kdgdir,MCPFN+1, KSUBDL, "datagen",8, nerr );

	/* -- See if datagen subdirectory exists */
	zinquire( kdgdir, &lexist );
	if ( lexist == 0 ) {

	  /* -- If necessary, try basename/../datagen -- */
	  zbasename( kdgdir,MCPFN+1 );
	  crname( kdgdir,MCPFN+1, KSUBDL, "../datagen",11, nerr );
	  zinquire( kdgdir, &lexist );
	  if ( lexist == 0 ) {
	    setmsg("ERROR", 131);
	    apcmsg( "Contact LLNL for this data (peterg@llnl.gov)", 45 );
	    outmsg();
	    goto L_8888;
	  }

	}

	if( *nerr != 0 )
	    goto L_8888;
	modcase( FALSE, (char*)kmdfm.kdgsub[cmdfm.idgsub - 1], MCPW, ktemp );
	crname( kdgdir,MCPFN+1, KSUBDL, ktemp,9, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* -- Append directory delimiter. */
	nc = indexb( kdgdir,MCPFN+1 );

        _c0[0] = KDIRDL;
        _c0[1] = '\0';
 	subscpy( kdgdir, nc, -1, MCPFN, _c0 );

	/* - Expand the filelist and read the files into memory. */

    if(!list) {
        list = string_list_init();
    }

	ldata = TRUE;
	lsdd = FALSE;
	readfl( ldata, lmore, lsdd, kdgdir,MCPFN+1, list, nerr );

	/* put it out to SeisMgr. */
	cmdfm.nreadflag = LOW ;
	cmdfm.lread = TRUE ;
	sacToSeisMgr ( !lmore , 0 , 1 , nerr ) ;
	cmdfm.lread = FALSE ;

L_8888:
	return;
}

