/** 
 * @file   xtranscript.c
 * 
 * @brief  TRANSCRIPT command
 * 
 */

#include <string.h>

#include "exm.h"
#include "cpf.h"
#include "msg.h"
#include "bool.h"
#include "select.h"
#include "co.h"

/** 
 * Execute the TRANSCRIPT command which controls the output of the session
 *    to a transcript file
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   891128:  Added "FLUSH" option.
 * @date   890117:  Added message option and changed some variable names.
 * @date   881230:  Original version.
 *
 */
void 
xtranscript(int *nerr) {

	char kmessage[MCMSG+1], kname[MCPFN+1];
	int lcontents[MTPMSG], lnewcontents, lnewmessage, lnewname;
	int jdx, jtpmsg, nchar, nderr ;
	static int lhist = FALSE;


	*nerr = 0;

	/* - Initialize parsing flags. */

	lnewcontents = FALSE;
	lnewmessage = FALSE;
	lnewname = FALSE;

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

	    /* -- "OPEN":  Open a (possibly existing) transcript file and
			   position to the bottom of it
			   (i.e. append to file.) */
	    if( lckey( "OPEN$",6 ) ){
		cmexm.imodetranscript = 1;
	    }

	    /* -- "CREATE":  Create a new transcript file. */
	    else if( lckey( "CR#EATE$",9 ) ){
		cmexm.imodetranscript = 2;
	    }

	    /* -- "CLOSE":  Close a current transcript file. */
	    else if( lckey( "CL#OSE$",8 ) ){
		cmexm.imodetranscript = 3;
	    }

	    /* -- "CHANGE": Change the contents of a current transcript file */
	    else if( lckey( "CH#ANGE$",9 ) ){
		cmexm.imodetranscript = 4;
	    }

	    /* -- "WRITE":  Don't change status of transcript file.
			    Just write message to it. */
	    else if( lckey( "W#RITE$",8 ) ){
		cmexm.imodetranscript = 5;
	    }

	    /* -- "FLUSH":  Flush the buffer associated with a
			    current transcript file. */
	    else if( lckey( "FL#USH$",8 ) ){
		cmexm.imodetranscript = 6;
	    }

	    /* -- "HISTORY":  Set up command line history */
	    else if( lckey( "HIST#ORY$",10 ) ){
		cmexm.imodetranscript = 7;
	    }

	    /* -- "FILE filename":  Set name of transcript file. */
	    else if( lkchar( "FILE$",6, MCPFN, kname,MCPFN+1, &nchar ) ){
		lnewname = TRUE;
	    }

	    /* -- "CONTENTS list": Define the contents of this transcript file*/
	    else if( lkentries( "CO#NTENTS$",11, (char*)kmmsg.ktpmsg,9, 
		     MTPMSG, lcontents ) ){
		lnewcontents = TRUE;
	    }

	    /* -- "MESSAGE text":  Write this message to transcript file. */
	    else if( lkchar( "MESSAGE$",9, MCMSG, kmessage,MCMSG+1, &nchar ) ){
		lnewmessage = TRUE;
	    }

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	}

	/* - Search list of active transcript files for this entry if name was given.
	 *   Create a new entry if necessary. */

	if( lnewname ){
	    jdx = 1;
L_2000:
	    if( memcmp(kname,kmexm.knametranscript[jdx - 1], min(strlen(kname),
		strlen(kmexm.knametranscript[jdx - 1]))) == 0 ){
		if( lnewcontents ){
		    for( jtpmsg = 0; jtpmsg < MTPMSG; jtpmsg++ ){
			cmexm.lsendtranscript[jdx - 1][jtpmsg] =
			      lcontents[jtpmsg];
		    }
		}
	    }
	    else if( jdx < cmexm.ntranscripts ){
		jdx = jdx + 1;
		goto L_2000;
	    }
	    else if( jdx < MTRANSCRIPTS ){
		if( cmexm.ntranscripts > 0 )
		    jdx = jdx + 1;
		strcpy( kmexm.knametranscript[jdx - 1], kname );
		if( lnewcontents ){
		    for( jtpmsg = 0; jtpmsg < MTPMSG; jtpmsg++ ){
			cmexm.lsendtranscript[jdx - 1][jtpmsg] =
			      lcontents[jtpmsg];
		    }
		}
		else{
		    for( jtpmsg = 0; jtpmsg < MTPMSG; jtpmsg++ ){
			cmexm.lsendtranscript[jdx - 1][jtpmsg] = TRUE;
		    }
		}
		cmexm.ntranscripts = cmexm.ntranscripts + 1;
	    }
	    else{
		*nerr = 1118;
		apimsg( MTRANSCRIPTS );
		goto L_8888;
	    }
	    cmexm.itranscript = jdx;
	}

	/* - If name was not given, assume the previous transcript file. */
	else{
	    jdx = cmexm.itranscript;
	}

	/* - If OPEN (imodetranscript=1) was requested:
	 *   (1) Get an unused fortran file unit.
	 *   (2) Use "znfile" to open (or create if necessary) transcript file.
	 *   (3) Position to the end of the file.
	 *   (4) Activate message output to this file.
	 *   (5) Write message to file if requested. */

	if( cmexm.imodetranscript == 1 ){
	    znfiles( &cmexm.nuntranscript[jdx-1],
		     (char*)kmexm.knametranscript[jdx - 1] ,
		     MCPFN+1, "TEXT",5, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	    if ( fseek ( cmexm.nuntranscript[jdx-1] , 0L , SEEK_END ) != 0 )
		fprintf ( stdout , "fseek returned error-xtranscript\n" ) ;
	    sendmesg( cmexm.nuntranscript[jdx-1], TRUE,
		      &cmexm.lsendtranscript[jdx - 1][0] );
	    if( lnewmessage ){
		setmsg( "OUTPUT", 99 );
		apcmsg( kmessage,MCMSG+1 );
		wrtmsg( cmexm.nuntranscript[jdx-1] );
		clrmsg();
	    }
	}


	/* - If CREATE (imodetranscript=2) was requested:
	 *   (1) Get an unused fortran file unit.
	 *   (2) Destroy existing file by this name.
	 *       (Ignore error return which means file did not exist.)
	 *   (3) Use znfile to create a new transcript file.
	 *   (4) Activate message output to this file.
	 *   (5) Write message to file if requested. */

	else if( cmexm.imodetranscript == 2 ){
	    zdest( (char*)kmexm.knametranscript[jdx - 1],MCPFN+1, &nderr );
	    if( nderr != 0 )
		clrmsg();
	    znfiles( &cmexm.nuntranscript[jdx-1],
		     (char*)kmexm.knametranscript[jdx - 1] ,
		     MCPFN+1, "TEXT",5, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    sendmesg( cmexm.nuntranscript[jdx-1], TRUE,
		      &cmexm.lsendtranscript[jdx - 1][0] );
	    if( lnewmessage ){
		setmsg( "OUTPUT", 99 );
		apcmsg( kmessage,MCMSG+1 );
		wrtmsg( cmexm.nuntranscript[jdx-1] );
		clrmsg();
	    }
	}


	/* - If CLOSE (imodetranscript=3) was requested:
	 *   (1) Write message to file if requested.
	 *   (2) Close transcript file.
	 *   (3) Deactivate message output to this file.
	 *   (4) Zero the fortran file unit number storage so that
	 *       we know that the file is currently closed. */

	else if( cmexm.imodetranscript == 3 ){
	    if( lnewmessage ){
		setmsg( "OUTPUT", 99 );
		apcmsg( kmessage,MCMSG+1 );
		wrtmsg( cmexm.nuntranscript[jdx-1] );
		clrmsg();
	    }
	    zcloses( &cmexm.nuntranscript[jdx-1], nerr );
	    if( *nerr != 0 )
		goto L_8888;
	    sendmesg( cmexm.nuntranscript[jdx-1], FALSE,
			&cmexm.lsendtranscript[jdx - 1][0] );
	    cmexm.nuntranscript[jdx-1] = 0;
	}


	/* - If CHANGE (imodetranscript=4) and file is open, send change to the
	 *   message handling subsystem.  If file is currently closed, simply
	 *   store change until the next time this transcript file is opened.
	 *   Write message to file if requested. */

	else if( cmexm.imodetranscript == 4 ){
	    if( cmexm.nuntranscript[jdx-1] != 0 ){
		sendmesg( cmexm.nuntranscript[jdx-1], TRUE,
			  &cmexm.lsendtranscript[jdx - 1][0] );
	    }
	    if( lnewmessage ){
		setmsg( "OUTPUT", 99 );
		apcmsg( kmessage,MCMSG+1 );
		wrtmsg( cmexm.nuntranscript[jdx-1] );
		clrmsg();
	    }
	}

	/* - If WRITE(imodetranscript=5) just write message to transcript file*/
	else if( cmexm.imodetranscript == 5 ){
	    if( lnewmessage ){
		setmsg( "OUTPUT", 99 );
		apcmsg( kmessage,MCMSG+1 );
		wrtmsg( cmexm.nuntranscript[jdx-1] );
		clrmsg();
	    }
	}


	/* - If FLUSH (imodetranscript=6) was requested:
	 *   (1) Close the transcript file to flush the buffer.
	 *   (2) Use "znfile" to reopen transcript file.
	 *   (3) Position to the end of the file.
	 *   (4) Activate message output to this file.
	 *   (5) Write message to file if requested. */

	else if( cmexm.imodetranscript == 6 ){
	    zcloses( &cmexm.nuntranscript[jdx-1], nerr );
	    if( *nerr != 0 )
		goto L_8888;
	    znfiles( &cmexm.nuntranscript[jdx-1],
		     (char*)kmexm.knametranscript[jdx - 1] ,
		     MCPFN+1, "TEXT",5, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	    if ( fseek ( cmexm.nuntranscript[jdx-1] , 0L , SEEK_END ) != 0 )
		fprintf ( stdout , "fseek returned error-xtranscript\n" ) ;
	    sendmesg( cmexm.nuntranscript[jdx-1],
			TRUE, &cmexm.lsendtranscript[jdx - 1][0] );
	    if( lnewmessage ){
		setmsg( "OUTPUT", 99 );
		apcmsg( kmessage,MCMSG+1 );
		wrtmsg( cmexm.nuntranscript[jdx-1] );
		clrmsg();
	    }
	}

        /* - Check that if history, only file name given or default used.  */

	else if( cmexm.imodetranscript == 7 ){
            if (lnewcontents) {
                *nerr = 1107;
                setmsg("ERROR", *nerr);
		goto L_8888;
	    }
            if (lnewname) {
	        /* Save name for history save upon quit; load history if first
		   use. */
	        int i;
		for(i=sizeof(kname)-2; i>=0 && (' ' == kname[i]); i--)
		    /* NOTHING */ ;
	        kname[i+1] = '\0';
	        sac_history_file_set(kname);
                if (!lhist)
	            sac_history_load(kname);
                lhist = TRUE;
	    } else
	        /* If no name given, use default.  History loaded when first
		   input line processed.  */
	        sac_history_file_set(NULL);

	    /* Add a message to the history, if one given */
            /*
            if (lnewmessage)
            No way to do this
            zclhist('ADD',kmessage,nerr);
            */
            *nerr = 0;
        }

L_8888:
	return;

}
