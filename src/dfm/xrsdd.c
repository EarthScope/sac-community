/** 
 * @file   xrsdd.c
 * 
 * @brief  Read a SDD File
 * 
 */
#include <string.h>

#include "dfm.h"
#include "bool.h"

#include "co.h"
#include "ssi.h"
#include "cpf.h"

#include "clf.h"

extern enum filetype { 
  sacfi, 
  alpha, 
  xdr, 
  segy 
} ftype ;

/** 
 * Execute the command READSDD which reads a SDD file
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 *    880308:  Added LDATA flag to call to READFL.
 *    870625:  Factored execution portion to readfl.
 *
 */
void 
xrsdd(int *nerr) {

	char _c0[2];
	int ldata, lmore, lsdd;
	int nchar;

    string_list *list;
    static string_list *last_list = NULL;
    
    if(!last_list) {
        last_list = string_list_init();
    }
	ftype = sacfi ;
	*nerr = 0;

	lmore = FALSE;
  list = NULL;

	/* PARSING PHASE: */
	/* - Parse position-dependent tokens: */

	while ( lcmore( nerr ) ){

	  /* -- "MORE":  signifies addition of more files to current read 
	   *             filelist rather than replacement of current list 
	   *             with new one. 
	   */
	  if( lckey( "MORE#$",7 ) && cmdfm.ndfl > 0 ){
	    lmore = TRUE;
	  }

	  /* -- "DIR CURRENT|name": set the name of the default subdirectory. */
	  else if( lkchar( "DIR#$",6, MCPFN, kmdfm.krddir,MCPFN+1, &nchar ) ){
	    if( memcmp(kmdfm.krddir,"CURRENT",7) == 0 || 
		memcmp(kmdfm.krddir,"current",7) == 0 ){
	      fstrncpy( kmdfm.krddir, MCPFN, " ", 1);
	    }
	    else if( kmdfm.krddir[nchar - 1] != KDIRDL ){
	      _c0[0] = KDIRDL;
	      _c0[1] = '\0';
	      subscpy( kmdfm.krddir, nchar, -1, MCPFN, _c0 );
	    }
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
	  
	  else	/* else move on to next set of tokens. */
	    break ;
	}
	
	/* - Parse position-independent tokens: */
	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){

	  /* -- "filelist":  define a new input filelist. */
	  if( ( list = lcdfl() ) )
	    { /* do nothing */ }
	  else{
	    /* -- Bad syntax. */
	    cfmt( "ILLEGAL OPTION:",17 );
	    cresp();
	  }
	}

	if( *nerr != 0 )
	    goto L_8888;

    if(!list) {
        list = string_list_init();
        string_list_extend(list, last_list);
    }        


	/* EXECUTION PHASE: */
        /* - Commit or rollback data according to lmore and cmdfm.icomORroll */
        if ( lmore ) {
            alignFiles ( nerr ) ;
	    if ( *nerr )
		return ;
	    cmdfm.nfilesFirst = cmdfm.ndfl ;
        } /* end if */
	else
	    cmdfm.nfilesFirst = 0 ;


	/* - Expand the filelist and read the files into memory. */
	ldata = TRUE;
	lsdd = TRUE;
	readfl( ldata, lmore, lsdd, kmdfm.krddir,MCPFN+1, list, nerr );

	if ( *nerr == 0 ){	/* if no error */
	    cmdfm.nreadflag = LOW ;
	    cmdfm.lread = TRUE ;
	    sacToSeisMgr ( !lmore , FALSE , TRUE , nerr ) ;
	    cmdfm.lread = FALSE ;
	}

    string_list_clear(last_list);
    string_list_extend(last_list, list);
    string_list_free(list);
    list = NULL;

L_8888:
	return;

}

