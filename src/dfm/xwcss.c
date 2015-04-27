/** 
 * @file   xwcss.c
 * 
 * @brief  Write a CSS File
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "amf.h"
#include "dfm.h"
#include "dff.h"
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
#include "errors.h"


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
	int lexpnd;
	int jdfl, nchar;
	int ibinORasc ;
	static int lwrdir = FALSE, Verbose = FALSE ;
  sac *s;
    char *WorkSetName;
    
    lexpnd = FALSE;
    
    char *file,*pfile, *dir;
    string_list *list;

	*nerr = 0;
    list = NULL;
    dir = NULL;
	/* PARSING PHASE: */
	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){

	    if( lckey( "OVER#$",7 ) ){
            cmdfm.lovrrq = TRUE;
            lexpnd = FALSE;
            list = string_list_init();
            for(i = 0; i < saclen(); i++) {
              if(!(s = sacget(i, TRUE, nerr))) {
                return;
              }
              string_list_put(list, s->m->filename, -1);
            }
	    }

            /* -- "VERBOSE ON|OFF":  turn Verbose mode on or off. */
        else if( lklog( "VER$BOSE",9, &Verbose ) )
            { /* do nothing */ }

	    /* -- "DIR ON|OFF|CURRENT|name":  set the name of the default subdirectory. */
	    else if( lkchar( "DIR#$",6, MCPFN, kmdfm.kwrdir,MCPFN+1, &nchar ) ){
        lwrdir = set_output_path(kmdfm.kwrdir);
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
      display_file_list(list);
	    wrtmsg( MUNOUT );
	} /* end if( cmdfm.lechof && lexpnd ) */


    jdfl = 1;

    file = string_list_get(list, jdfl-1);
    pfile = prepare_output_filename(file, lwrdir, kmdfm.kwrdir);

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
	    WriteCSSflatFiles(WorkSetName, pfile ) ;
	else {
	    if(!WriteCSSBfile(WorkSetName, pfile, Verbose)) *nerr = 115;
	    if(Verbose)printf("\n");
	}
  FREE(pfile);
}

