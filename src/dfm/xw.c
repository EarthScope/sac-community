/** 
 * @file   xw.c
 * 
 * @brief  Write
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dfm.h"
#include "bool.h"
#include "hdr.h"

#include "sddhdr.h"

#include "wild.h"
#include "bot.h"
#include "ssi.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"
#include "ncpf.h"

/** 
 * Write a File to disk
 * 
 * @param lsdd 
 *    Set the Output to be a SDD file
 * @param nerr 
 *    Error Return Flag 
 *    - 0 on Success
 *
 * @date   970702:  Changed lckey and lkchar to lckeyExact and lkcharExact
 *                  throughout xw.c.  This will allow files to begin with 
 *                  the same string as the various options (eg. sacxz.021.z)
 *                  maf.
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
xw(int  lsdd, 
   int *nerr) {

    int i;
        char delimiter[2], kcdir[9], kchange[MCPFN+1], kdirpart[MCPFN+1];
	char kfile[MCPFN+1], kpdir[9], kstring[MCPFN+1], ktemp[9];
	int lexpnd;
	int jdfl, nchange, nchar, nchg, ndx1, ndx2;
	int nlen, nstr, nstring, nwrdir;
	static int lwrdir = FALSE;
    char *cattemp;
    char *strtemp1, *strtemp2, *strtemp3;
    
    char *file;
    string_list *list, *files;

	kschan[12]='\0';
	kschdr[80]='\0';
	ksclas[4]='\0';
	kscom[40]='\0';
	ksevnm[8]='\0';
	ksfrmt[8]='\0';
	ksstnm[8]='\0';
    memset(kfile, 0, sizeof(kfile));
    memset(kdirpart, 0, sizeof(kdirpart));
    memset(kchange, 0, sizeof(kchange));
    memset(ktemp, 0, sizeof(ktemp));
    memset(kstring, 0, sizeof(kstring));
    memset(kpdir, 0, sizeof(kpdir));
    memset(kcdir, 0, sizeof(kcdir));
    memset(delimiter, 0, sizeof(delimiter));

        lexpnd = FALSE;

	*nerr = 0;

    files = string_list_init();
    list  = NULL;

	if( lsdd )
	    cmdfm.iwfmt = 3;

	/* PARSING PHASE: */
	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){

	    /* -- "SAC|ALPHA":  set format to be used in writing files. */
	    if( lckeyExact( "SAC#$",6 ) )
		cmdfm.iwfmt = 1;

	    else if( lckeyExact( "ALPHA#$",8 ) )
		cmdfm.iwfmt = 2;

	    else if( lckeyExact( "CI#$",5 ) )
		cmdfm.iwfmt = 2;

	    else if( lckeyExact( "SDD#$",6 ) )
		cmdfm.iwfmt = 3;

	    else if( lckeyExact( "XDR#$",6 ) ) {
		cmdfm.iwfmt = 4;
            }
            else if( lckeyExact( "SEGY#$", 7 ) )
                cmdfm.iwfmt = 5;

	    /* -- "OVER":  overwrite files from previous READ command. */
	    else if( lckeyExact( "OVER#$",7 ) ){
		cmdfm.lovrrq = TRUE;
		lexpnd = FALSE;
        string_list_extend(files, datafiles);
	    }

	    /* generate names from the KSTCMP header field */
	    else if( lckeyExact( "KSTCMP#$",9 ) ){
		lexpnd = FALSE;
		gennames("KSTCMP ",7,files,string_list_length(datafiles),nerr);
		if(*nerr != 0)
		    goto L_8888;
	    }

	    /* -- "APPEND string": append string to filenames from READ command. */
	    else if( lkcharExact( "APPEND#$",9, MCPFN, kstring,MCPFN+1, &nstring ) ){
        for(i = 0; i < cmdfm.ndfl; i++) {
            strtemp1 = string_list_get(datafiles, i);
		    appendstring( kstring,MCPFN+1, strtemp1, strlen(strtemp1)+2, kfile,MCPFN+1 );

            string_list_put(files, kfile, MCPFN+1);
		    if( *nerr != 0 )
                goto L_8888;
		}
		cmdfm.lovrrq = FALSE;
		lexpnd = TRUE;
	    }

	    /* -- "PREPEND string": prepend string to filenames from READ command. */
	    else if( lkcharExact( "PREPEND#$",10, MCPFN, kstring,MCPFN+1, &nstring ) ){
        for(i = 0; i < cmdfm.ndfl; i++) {
		    strtemp1 = malloc(nstring+1);
		    strncpy(strtemp1,kstring,nstring);
		    strtemp1[nstring] = '\0';
            strtemp2 = string_list_get(datafiles, i);
		    prependstring( strtemp1, nstring+1, strtemp2, strlen(strtemp2)+2, kfile,MCPFN+1);

		    free(strtemp1);
            string_list_put(files, kfile, MCPFN+1);
		    if( *nerr != 0 )
			goto L_8888;
		}
		cmdfm.lovrrq = FALSE;
		lexpnd = TRUE;
	    }

	    /* -- "DELETE string": delete string from filenames from READ command. */
	    else if( lkcharExact( "DELETE#$",9, MCPFN, kstring,MCPFN+1, &nstring ) ){
        for(i = 0; i < cmdfm.ndfl; i++) {
		    strtemp1 = malloc(nstring+1);
		    strncpy(strtemp1,kstring,nstring);
		    strtemp1[nstring] = '\0';
            strtemp2 = string_list_get(datafiles, i);

		    deletestring( strtemp1, nstring+1, strtemp2, strlen(strtemp2)+2, kfile,MCPFN+1);

		    free(strtemp1);
            string_list_put(files, kfile, MCPFN+1);
		    if( *nerr != 0 )
			goto L_8888;
		}
		cmdfm.lovrrq = FALSE;
		lexpnd = TRUE;
	    }

	    /* -- "CHANGE string1 string2": change string1 to string2 in READ filenames. */
	    else if( lkcharExact( "CHANGE#$",9, MCPFN, kstring,MCPFN+1, &nstring ) ){
		lcchar( MCPFN, kchange,MCPFN+1, &nchange );
        for(i = 0; i < cmdfm.ndfl; i++) {
		    nstr = indexb( kstring,MCPFN+1 );
		    nchg = indexb( kchange,MCPFN+1 );

		    strtemp1 = malloc(nstr+1);
		    strtemp2 = malloc(nchg+1);
		    strncpy(strtemp1,kstring,nstr);
		    strncpy(strtemp2,kchange,nchg);
		    strtemp1[nstr] = '\0';
		    strtemp2[nchg] = '\0';
            strtemp3 = string_list_get(datafiles, i);
		    changestring( strtemp1, nstr+1, strtemp2, nchg+1,
                          strtemp3, strlen(strtemp3)+2, kfile,MCPFN+1 );

		    free(strtemp1);            
		    free(strtemp2);

            string_list_put(files, kfile, MCPFN+1);
		    if( *nerr != 0 )
			goto L_8888;
		}
		cmdfm.lovrrq = FALSE;
		lexpnd = TRUE;
	    }

	    /* -- "DIR ON|OFF|CURRENT|name":  set the name of the default subdirectory. */
	    else if( lkcharExact( "DIR#$",6, MCPFN, kmdfm.kwrdir,MCPFN+1, &nchar ) ){
		modcase( TRUE, kmdfm.kwrdir, MCPW, ktemp );

		if( strncmp(ktemp,"OFF     ",8) == 0 ) {
          lwrdir = FALSE;
        } else if( strncmp(ktemp,"CURRENT ",8) == 0 ){
          lwrdir = TRUE;
          fstrncpy( kmdfm.kwrdir, MCPFN, " ", 1);
		} else if( kmdfm.kwrdir[nchar - 1] != KDIRDL ){ 
          /* If the string is mising the "/" path separator */
          lwrdir = TRUE;
          delimiter[0] = KDIRDL;
          delimiter[1] = '\0';
          subscpy( kmdfm.kwrdir, nchar, -1, MCPFN, delimiter );
		} else {
          /* Path is not OFF, CURRENT and has the "/" at the end */
          lwrdir = TRUE;
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
	} /* end while ( lcmore( nerr ) ) */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
	    goto L_8888;

	/* CHECKING PHASE: */
    if(!list) {
        list = files;
    } else {
        /* List + Modifiers :: Use List */
        string_list_free(files);
        files = NULL;
    }

	/* - Check for null write filelist. */
	if( string_list_length(list) <= 0 ){
	    *nerr = 1311;
	    setmsg( "ERROR", *nerr );
	    goto L_8888;
	}

	/* - Make sure the write filelist has as many entries as read filelist. */

	if( string_list_length(list) != cmdfm.ndfl ){
	    *nerr = 1312;
        error(1312, "%d %d", string_list_length(list), cmdfm.ndfl);
	    goto L_8888;
	}

	/* EXECUTION PHASE: */

        /* - Commit or rollback data according to cmdfm.icomORroll */
	alignFiles ( nerr ) ;
	if ( *nerr )
	    return ;


	/* - Echo expanded filelist if requested. */

	if( cmdfm.lechof && lexpnd ){
	    setmsg( "OUTPUT", 0 );

        for(i = 0; i < string_list_length(list); i++) {
            file = string_list_get(list, i);

            getdir( file, strlen(file)+1, kcdir,9, kfile,MCPFN+1 );

		/* -- Echo the filename part if there is no directory part. */
            if( strcmp(kcdir,"        ") == 0 )
                apcmsg( kfile,MCPFN+1 );

		/* -- Prepend the filename part with some special characters if
         *    directory part is same as that of the previous file. */
            else if( memcmp(kcdir,kpdir,min(strlen(kcdir),strlen(kpdir)))
                     == 0 ){
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
	    }
	    wrtmsg( MUNOUT );
	}

	/* - Write each file in memory to disk. */

	nwrdir = indexb( kmdfm.kwrdir,MCPFN+1 );
	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	    /* -- Get file from memory manager. */
        file = string_list_get(list, jdfl-1);
	    getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* isolate file name */
        file = string_list_get(list, jdfl-1);

	    /* -- Check overwrite-protect flag in header record. */
	    if( cmdfm.lovrrq && !*lovrok ){
		*nerr = 1303;
		setmsg( "ERROR", *nerr );
		apcmsg2(file, strlen(file)+1);
		outmsg () ;
		clrmsg () ;
		goto L_8888;
	    }

	    /* -- Prepare output file name:
	     * --- If directory option is ON (lwrdir=.TRUE. and nwrdir>0), 
	     *     concatenate directory name with file name part of write file list.
	     * --- If directory option is CURRENT (lwrdir=.TRUE. and nwrdir=0), 
	     *     use file name part of write file list.
	     * --- If directory option is OFF, use write file list. */
	    if( lwrdir ){
		if( nwrdir > 0 ){
		    fstrncpy( kfile, MCPFN, kmdfm.kwrdir,min(nwrdir,MCPFN));

            strtemp1 = file;
		    strtemp2 = malloc(130-(nwrdir+1));
		    strncpy(strtemp2,kfile+nwrdir,MCPFN+1-(nwrdir + 1));
		    strtemp2[MCPFN+1-(nwrdir+1)] = '\0';
		    getdir( strtemp1, strlen(strtemp1)+1, 
                    kdirpart, MCPFN+1, strtemp2,-(nwrdir+1)+130);
		    subscpy(kfile,nwrdir,-1,MCPFN,strtemp2);
		    free(strtemp2);
		}
		else{
		    fstrncpy( kfile, MCPFN, " ", 1);
		    getdir( file, strlen(file)+1, kdirpart,MCPFN+1, kfile,MCPFN+1 );
		}
	    }
	    else {
            fstrncpy( kfile, MCPFN, file, strlen(file));
        }
	    /* -- Write file in appropriate format. */
	    if( cmdfm.iwfmt == 2 )
		wrci( jdfl, kfile,MCPFN+1, "%#15.7g", nerr );

	    else if( cmdfm.iwfmt == 3 )
		wrsdd( jdfl, kfile,MCPFN+1, TRUE, nerr );

	    else if( cmdfm.iwfmt == 4 )
		wrxdr( jdfl, kfile,MCPFN+1, TRUE, nerr );

	    else if( cmdfm.iwfmt == 5 )
		wrsegy( jdfl , kfile , nerr ) ;

	    else
		wrsac( jdfl, kfile,MCPFN+1, TRUE, nerr );

	    if( *nerr != 0 )
		goto L_8888;

	} /* end for ( jdfl ) */

L_8888:
	return;
}

