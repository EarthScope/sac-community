/** 
 * @file   xwcss.c
 * 
 * @brief  Write a CSS File
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dfm.h"
#include "bool.h"
#include "sddhdr.h"
#include "cssb.h"
#include "wild.h"
#include "bot.h"
#include "ssi.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"

#include "cssListOps/dblPublicDefs.h"
#include "smDataIO.h"


/** 
 * Write a CSS Ascii Flat File or CSSB (Binary File)
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   910731:  Bug fixed in options PREPEND, DELETE, CHANGE.
 * @date   900904:  Added SDD as a format for write.
 * @date   881228:  Added four new options for generating write file list
 *                  from data file list: APPEND, PREPEND, CHANGE, DELETE.
 * @date   880204:  Fixed logic involving use of DIR option in READ and WRITE
 *                  by adding an ON/OFF flag as well as a directory name.
 * @date   880115:  Deleted call that forced default directory to lowercase.
 * @date   870626:  Added default directory option.
 * @date   860917:  Changed to character lists for storing data file names.
 * @date   850730:  Deleted SOCKITTOME  format.
 * @date   830120:  Added SOCK and CI output formats.
 * @date   820721:  Changed to newest set of parsing and checking functions.
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   810203:  Fixed bug in file overwrite option.
 *
 */
void 
xwcss(int *nerr) {

    int i;
    char dirDelimeter[2], kcdir[9], kdirpart[MCPFN+1], kfile[MCPFN+1]; 
	char kpdir[9], ktemp[9] ;
	int lexpnd;
	int jdfl, nchar, nwrdir ;
	int ibinORasc ;
	static int lwrdir = FALSE, Verbose = FALSE ;
  char *cattemp;
  char *strtemp2;

    char *WorkSetName;
    
    lexpnd = FALSE;
    
    char *file;
    string_list *list;

	*nerr = 0;
    list = NULL;
	/* PARSING PHASE: */
	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){

	    if( lckey( "OVER#$",7 ) ){
            cmdfm.lovrrq = TRUE;
            lexpnd = FALSE;
            list = string_list_init();
            string_list_extend(list, datafiles);
	    }

            /* -- "VERBOSE ON|OFF":  turn Verbose mode on or off. */
        else if( lklog( "VER$BOSE",9, &Verbose ) )
            { /* do nothing */ }

	    /* -- "DIR ON|OFF|CURRENT|name":  set the name of the default subdirectory. */
	    else if( lkchar( "DIR#$",6, MCPFN, kmdfm.kwrdir,MCPFN+1, &nchar ) ){
		modcase( TRUE, kmdfm.kwrdir, MCPW, ktemp );
		if( strcmp(ktemp,"ON      ") == 0 ){
		    lwrdir = TRUE;
		}
		else if( strcmp(ktemp,"OFF     ") == 0 ){
		    lwrdir = FALSE;
		}
		else if( strcmp(ktemp,"CURRENT ") == 0 ){
		    lwrdir = TRUE;
		    fstrncpy( kmdfm.kwrdir, MCPFN, " ", 1);
		}
		else if( kmdfm.kwrdir[nchar - 1] != KDIRDL ){
		    lwrdir = TRUE;
		    dirDelimeter[0] = KDIRDL;
		    dirDelimeter[1] = '\0';
		    subscpy( kmdfm.kwrdir, nchar, -1, MCPFN, dirDelimeter );
		}
		else{
		    lwrdir = TRUE;
		}
	    }

	    /* -- "COMMIT|RECALLTRACE|ROLLBACK": how to treat existing data */
	    else if ( lckeyExact ( "COMMIT" , 7 ) )
		cmdfm.icomORroll = COMMIT ;
	    else if (lckeyExact ( "RECALLTRACE" , 12 ) )
		cmdfm.icomORroll = RECALL ;
	    else if ( lckeyExact ( "RECALL" , 7 ) )
		cmdfm.icomORroll = RECALL ;
	    else if ( lckeyExact ( "ROLLBACK" , 9 ) )
		cmdfm.icomORroll = ROLLBACK ;


	    /* -- "BINARY|ASCII": read CSSB or flatfiles */
	    else if( lclist( (char*)kmdfm.kbinORasc, 9, 2, &ibinORasc ) ) 
		cmdfm.lwascii = ibinORasc - 1 ;

	    /* -- "filelist":  write files using names in new filelist. */
	    else if( ( list = lcdfl() ) ){
		cmdfm.lovrrq = FALSE;
		lexpnd = FALSE;
	    }

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	} /* end while */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
		return;

	/* CHECKING PHASE: */

	/* - Check for null write filelist. */

	if( !list || string_list_length(list) <= 0 ){
	    *nerr = 1311;
	    setmsg( "ERROR", *nerr );
	    return;
	}

	/* EXECUTION PHASE: */

	/* - Echo expanded filelist if requested. */

	if( cmdfm.lechof && lexpnd ){
	    setmsg( "OUTPUT", 0 );

	    /* -- Loop until all pathnames in expanded filelist have been processed. */
        for(i = 0; i < string_list_length(list); i++) {
            file = string_list_get(list, i);
            /* -- Break pathname into directory and filename parts. */
            if ( file[strlen(file)] == KDIRDL ) {
                file[strlen(file)] = '\0' ;
            }
            
            getdir( file, strlen(file), kcdir,9, kfile,MCPFN+1 );

		/* -- Echo the filename part if there is no directory part. */
		if( strcmp(kcdir,"        ") == 0 ){
		    apcmsg( kfile,MCPFN+1 );
		}

		/* -- Prepend the filename part with some special characters if 
		 *    directory part is same as that of the previous file. */
		else if( memcmp(kcdir,kpdir,min(strlen(kcdir),strlen(kpdir))) == 0 ){
		    cattemp = malloc(3+strlen(kfile)+1);
		    strcpy(cattemp, "...");
		    strcat(cattemp,kfile);
		    apcmsg( cattemp, 3+strlen(kfile)+1 );
		    free(cattemp);
		}

		/* -- Echo complete pathname if directory part is different. */
		else{
		    apcmsg2(file, strlen(file)+1);
		    strcpy( kpdir, kcdir );
		}
	    } /* end while */
	    wrtmsg( MUNOUT );
	} /* end if( cmdfm.lechof && lexpnd ) */


	/* -- Prepare output file name:
	 * --- If directory option is ON (lwrdir=.TRUE. and nwrdir>0), 
	 *     concatenate directory name with file name part of write file list.
	 * --- If directory option is CURRENT (lwrdir=.TRUE. and nwrdir=0), 
      	 *     use file name part of write file list.
       	 * --- If directory option is OFF, use write file list. */
    jdfl = 1;

    file = string_list_get(list, jdfl-1);
    if( lwrdir ){
	    nwrdir = indexb( kmdfm.kwrdir,MCPFN+1);
	    if( nwrdir > 0 ){
            fstrncpy( kfile, MCPFN, kmdfm.kwrdir,min(nwrdir,MCPFN));

            strtemp2 = malloc(130-(nwrdir+1));
            strncpy(strtemp2,kfile+nwrdir,MCPFN+1-(nwrdir + 1));
            strtemp2[MCPFN+1-(nwrdir+1)] = '\0';
            
            getdir(file, strlen(file)+1, kdirpart, MCPFN+1, strtemp2,-(nwrdir+1)+130);
            subscpy(kfile,nwrdir,-1,MCPFN,strtemp2);
            
            free(strtemp2);
	    } else{
            fstrncpy( kfile, MCPFN, " ", 1);
            getdir(file, strlen(file)+1, kdirpart,MCPFN+1, kfile,MCPFN+1 );
	    }
	} /* end if ( lwrdir ) */
	else{
	    fstrncpy( kfile, MCPFN, file, strlen(file)+1);
	}
    
	terminate ( kfile ) ;


	if(! smGetDefaultWorkset() ){
        *nerr = 1385;
        return;
    }
    
    WorkSetName = smGetDefaultWorksetName();
    if(! WorkSetName){
        *nerr = 1386;
        return;
    }
    
    /* commit, recall, or rollback data according to user options. */
    alignFiles ( nerr ) ;
    if ( *nerr )
        return ;
    
    
	if ( cmdfm.lwascii )
	    WriteCSSflatFiles(WorkSetName, kfile ) ;
	else {
	    if(!WriteCSSBfile(WorkSetName, kfile, Verbose)) *nerr = 115;
	    if(Verbose)printf("\n");
	}
}

