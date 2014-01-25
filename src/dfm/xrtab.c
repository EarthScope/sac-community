/** 
 * @file   xrtab.c
 * 
 * @brief  Read a Table file
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dfm.h"
#include "bool.h"
#include "hdr.h"
#include "com.h"
#include "gdm.h"
#include "gem.h"
#include "amf.h"

#include "clf.h"

#include "string_utils.h"


#include "wild.h"
#include "pl.h"
#include "bot.h"
#include "ucf.h"
#include "ssi.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

#define	MALPHA	100
#define	MAXCH	40
#define	MBLKSZ	500
#define	MENTRY	40

struct t_xracom {
  int nhlines;
} xracom;

/** 
 * Execute the command READTABLE wich reads a general alphanumeric data file
 * 
 * @param lplot 
 *    Whether to plot an alpha file
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   970129:  Add parameter (1) to cnvfre.  1 means that if a string
 *                  of digits is too long, warn the user and end the 
 *                  command.  maf 
 * @date   920501:  Added test to clear memory on NOFILES and MEMORY delete
 *                  READERR contition.
 * @date   920429:  Added lfilesok test and nsndfl to save current file count.
 * @date   920319:  Bug fix. Changed "more=.false." to "lmore.eq.false"
 * @date   920226:  Added data-set storage logic.
 * @date   911009:  Added more descriptive error message when potential
 *                  formatting error occurs (Error = 0002) in cmvfre or
 *                  cmvfmt.
 * @date   870722:  Added descriptive error messages.
 * @date   860910:  Original version.
 *
 */
void 
xrtab(int  lplot, 
      int *nerr) {

	char slash[2], kalpha[MALPHA][21], kcard[MCMSG+1], kcont[MCMSG+1],
    kform[MCMSG+1];
	int lbcksp, lexpnd, lfree, lmore, ltoend, numchar,
	     lprint = FALSE , ltry = FALSE ;
	int idx, ic, ic1, ic2, iopch[MAXCH], 
	 ipt, itype, jdx, jch, jdfl, jen, nc, 
	 nchar, ndcont, ndflsv, ndform, ndxch[MAXCH-(0)+1],
	 nentry, newlen, newndx, nlnch[MAXCH-(0)+1], notused,
	 nptch[MAXCH-(0)+1], nsndfl, numch, numxch, numych ;
  FILE *nun;
	float fentry[MENTRY], unused;

  static string_list *last_list = NULL;
  int i;
  char *tmp, *file;
  string_list *list, *files;
	static int _aini = 1;
  char *cattemp;
        
	float *const Fentry = &fentry[0] - 1;
	int *const Iopch = &iopch[0] - 1;

    if(!last_list) {
        last_list = string_list_init();
    }

    list = NULL;

	if( _aini ){ /* Do 1 TIME INITIALIZATIONS! */
	    xracom.nhlines = 0;
	    _aini = 0;
	}

        /* initialize character arrays */
        for( idx = 0 ; idx < MCMSG ; idx++ ){
  	  kcard[idx] = kcont[idx] = kform[idx] = ' ' ;
        }
  	kcard[MCMSG] = kcont[MCMSG] = kform[MCMSG] = '\0' ;

	*nerr = 0;

	/* PARSING PHASE: */
	lmore = FALSE;
	ndflsv = 0;

	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){

	    /* -- "MORE":  signifies addition of more files to current read 
	     *             filelist rather than replacement of current list
	     *             with new one. */
	    if( lckey( "&MORE$",7 ) ){
		lmore = TRUE;
		ndflsv = cmdfm.ndfl;
	    }

	    /* -- "DIR CURRENT|name":  set name of the default subdirectory. */
	    else if( lkchar( "DIR#$",6, MCPFN, kmdfm.krddir,MCPFN+1, &nchar ) ){
		if(memcmp(kmdfm.krddir,"CURRENT",7) == 0 || memcmp(kmdfm.krddir,
		 "current",7) == 0 ){
		    fstrncpy( kmdfm.krddir, MCPFN, " ", 1);
		}
		else if( kmdfm.krddir[nchar - 1] != KDIRDL ){
		    slash[0] = KDIRDL;
		    slash[1] = '\0';
		    subscpy( kmdfm.krddir, nchar, -1, MCPFN, slash );
		}
	    }

	    /* -- "HEADER count": set number of lines to skip. */
	    else if( lkint( "HEADER$",8, &xracom.nhlines ) )
	    { /* do nothing */ }

	    /* -- "CONTENT string": set default content. */
	    else if( lkchar( "&CONTENT$",10, MCMSG, kmdfm.kdcont,MCMSG+1, 
	     &ndcont ) ){
		modcase( TRUE, kmdfm.kdcont, ndcont, kmdfm.kdcont );
	    }

	    /* -- "FREE":  use free-field rather than formatted input. */
	    else if( lckey( "&FREE$",7 ) )
		cmdfm.ldfree = TRUE;

	    /* -- "FORMAT string": use formatted input & set default format. */
	    else if(lkchar("&FORMAT$",9, MCMSG, kmdfm.kdform,MCMSG+1, &ndform)){
		cmdfm.ldfree = FALSE;
		modcase( TRUE, kmdfm.kdform, ndform, kmdfm.kdform );
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

	    /* if PRINT option is tried, get printer name */
	    else if ( lplot && ltry ) {
		char command[ 81 ] ;
		lcchar ( MAXPRNTRNAMELEN   , kmgem.kptrName ,
			 MAXPRNTRNAMELEN+1 , &notused ) ;
		terminate ( kmgem.kptrName ) ;

		sprintf ( command , "lpstat -v | grep %s" , kmgem.kptrName ) ;
		if ( system ( command ) ) {
      //cmcom.jcom-- ;
		    kmgem.kptrName[0] = '\0' ;
		}

		if ( !lprint )
		    kmgem.kptrName[0] = '\0' ;

		ltry = FALSE ;
	    }

	    /* -- "PRINT":  print the final product */
	    else if( lplot && lckey( "PRINT#$", 8 ) ) {
		ltry = TRUE ;
		if ( cmgdm.lbegf ) {
		    setmsg ( "WARNING" , 2403 ) ;
		    outmsg () ;
		    clrmsg () ;
		}
		else {
		    lprint = TRUE ;
		}
	    }

	    /* -- TRUST:  whether or not to trust matching evids while moving
                          data from SAC buffers to CSS buffers. */
	    else if( lklog( "TRUST#$",8, &cmdfm.ltrust ) )
	    { /* do nothing */ }

	    /* -- "filelist":  define a new filelist (or add to old filelist) */
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


	/* - Echo expanded file list if requested. */
	/* - Save current file count of wroking storage, incase we can't read
	 *   the requested file. */

	nsndfl = cmdfm.ndfl;

	/* - Set the current file count for using read or read-more. */

	cmdfm.ndfl = ndflsv;

	files = wildfl( kmdfm.krddir,MCPFN+1, list, &lexpnd ) ;
	if( cmdfm.lechof && lexpnd ){
	    setmsg( "OUTPUT", 0 );
	    ic1 = 0;
        for(i = 0; i < string_list_length(files); i++) {
            tmp = string_list_get(files, i);
            apcmsg2(tmp, strlen(tmp)+1);
        }
	    wrtmsg( MUNOUT );
	}

	/* -- Test to see that at least one file can be opened and read.
	 *    If not, resore current file count of working storage. */

	if( !lfilesok( files, NULL, 0, lmore, FALSE, FALSE, nerr ) ){
	    cmdfm.ndfl = nsndfl;

	    /* --- If destroying files in memory, */
	    if( strcmp(kmdfm.kecmem,"DELETE  ") == 0 )
		cleardfl( nerr );

	    *nerr = 1301;
	    goto L_8888;
	}


	/* - Release all old memory blocks and initialize data file list
	 *   unless MORE option is on. */

	/* -- Release all memory blocks. */

	if( !lmore ){
	    cleardfl( nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

	/* - Main loop on each input file: */

    for(i = 0; i < string_list_length(files); i++) {
        file = string_list_get(files, i);

	    /* -- Open input alphanumeric data file. */
	    zopens( &nun, file, strlen(file), "ROTEXT",7, nerr );

	    if( *nerr != 0 )
            goto L_8888;

	    /* -- Read first card.
	     * --- If is a description card, decode it into CONTENT and FORMAT
		   information.
	     * --- Otherwise, backspace input file. */
	    strcpy( kcont, kmdfm.kdcont );
	    nc = indexb( kmdfm.kdform,MCMSG+1 );
	    if( kmdfm.kdform[0] == '(' && kmdfm.kdform[nc - 1] == ')' ){
		fstrncpy( kform, MCMSG, kmdfm.kdform, nc);
	    }
	    else{
		fstrncpy(kform, MCMSG, "(", 1);
		fstrncpy(kform+1, MCMSG-1, kmdfm.kdform, nc);
		fstrncpy(kform+1+nc, MCMSG-1-nc, ")", 1);
	    }
	    lfree = cmdfm.ldfree;

	    if(fgetsp(kcard,MCMSG+1,nun) == NULL) {
            if(feof(nun)) goto L_5000;
            else goto L_9000;
	    }
	    if(kcard[(numchar=strlen(kcard)-1)] == '\n')kcard[numchar] = '\0';

	    /* replace tabs with blanks */
	    for( idx=0; idx<numchar; idx++) if(kcard[idx] == '\t')
		kcard[idx] = ' ';

	    nc = indexb( kcard,MCMSG+1 );
	    ic = 0;
	    upcase( kcard, nc, kcard,MCMSG+1 );
	    poptok( kcard, nc, &ic, &ic1, &ic2, &itype );
	    lbcksp = TRUE;

	    while ( 1 ) {
		if( memcmp(kcard+ic1 - 1,"CONTENT",7) == 0 ){
		    poptok( kcard, nc, &ic, &ic1, &ic2, &itype );
		    if( itype > 0 )
			fstrncpy( kcont, MCMSG, kcard+ic1 - 1, ic2 - ic1 + 1);
		    lbcksp = FALSE;
		    poptok( kcard, nc, &ic, &ic1, &ic2, &itype );
		}
		else if( memcmp(kcard+ic1 - 1,"FREE",4) == 0 ){
		    lfree = TRUE;
		    break ;
		}
		else if( memcmp(kcard+ic1 - 1,"FORMAT",6) == 0 ){
		    lfree = FALSE;
		    poptok( kcard, nc, &ic, &ic1, &ic2, &itype );
		    if( itype > 0 ){
			if( kcard[ic1 - 1] == '(' && kcard[ic2 - 1] == ')' ){
			    fstrncpy( kform, MCMSG, kcard+ic1 - 1, ic2-ic1+1);
			}
			else{
			    fstrncpy( kform, MCMSG, "(", 1);
			    fstrncpy( kform+1, MCMSG-1, kcard+ic1-1, ic2-ic1+1);
			    fstrncpy( kform+1+(ic2-ic1+1),MCMSG-1-(ic2-ic1+1),
				      ")",1);
			}
		    }
		    lbcksp = FALSE;
		    poptok( kcard, nc, &ic, &ic1, &ic2, &itype );
		}
		else if( lbcksp ){
		    backspace( nun, 1L );
		    break ;
		}
	    } /* end while (1) */

	    /* -- Skip over header lines */
	    for( idx = 1; idx <= xracom.nhlines; idx++ ){
		if(fgetsp(kcard,MCMSG+1,nun) == NULL) {
		    if(feof(nun)) goto L_5000;
		    else goto L_9000;
		}
	    }

	    /* -- Determine number of entries per card if formatted option. */
	    if( !lfree )
		detnum( kform,MCMSG+1, &nentry );

	    /* -- Decode content information. */
	    decont( kcont,MCMSG+1, MAXCH, &numch, &numxch, &numych, iopch, 
		    &ltoend, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Prime it to read in first card to see how many
		  entries are really there */
	    do{
		if(fgetsp(kcard,MCMSG+1,nun) == NULL) {
		    if(feof(nun)) goto L_5000;
		    else goto L_9000;
		}
		if(kcard[(numchar=strlen(kcard)-1)] == '\n')
		    kcard[numchar] = '\0';

		/* replace tabs with blanks */
		for( idx=0; idx<numchar; idx++) if(kcard[idx] == '\t')
		    kcard[idx] = ' ';

        for( idx = 0; idx < MALPHA; idx ++) {
          memset(kalpha[idx], ' ', 21);
        }

		/* --- Convert card to list of floating point values.
		*     Each card is in either free field or formatted. */
		if( lfree ){
		    cnvfre( kcard,MCMSG+1, MENTRY, &nentry, fentry, iopch,
			    (char*)kalpha[0] ,21, 1, nerr );
		}
		else{
		    cnvfmt( kcard,MCMSG+1, kform,MCMSG+1, nentry, fentry, nerr );
		}

		/* -- Loop on each line in text file.
		 * --- Say something descriptive about data problems and get out. */
		if( *nerr != 0 ){
		    setmsg( "WARNING", *nerr );
		    apcmsg( "Trouble reading line. Skipping...",34 );
		    apcmsg( kcard,MCMSG+1 );
		    outmsg();
		}
	    } while( *nerr ) ;

	    /* --- Redetermine how many y channels there really are */
	    for( idx = 1; idx <= nentry; idx++ ){
		if( Iopch[idx] > 0 )
		    numych = Iopch[idx];
	    }

	    /* -- Allocate memory block for x channel if present.
	     *    This is treated as channel 0 in the storage scheme. */
	    if( numxch == 1 ){
		nlnch[0] = MBLKSZ;
		nptch[0] = 0;
		allamb( &cmmem, nlnch[0], &ndxch[0], nerr );
		if( *nerr != 0 )
		    goto L_8888;
	    }
	    else if( numxch > 1 ){
		*nerr = 1361;
		setmsg( "ERROR", *nerr );
		apcmsg( "Can only have one X channel per file.",38 );
		goto L_8888;
	    }

	    /* -- Allocate memory blocks for y channel(s). */
	    if( numych >= 1 ){
		for( jdx = 1; jdx <= numych; jdx++ ){
			nlnch[jdx] = MBLKSZ;
			nptch[jdx] = 0;
			allamb( &cmmem, nlnch[jdx], &ndxch[jdx], nerr );
			if( *nerr != 0 )
			    goto L_8888;
		}
	    }
	    else{
		*nerr = 1362;
		setmsg( "ERROR", *nerr );
		goto L_8888;
	    }

	    /* --- Say something descriptive about data problems and get out. */
L_4000:
	    if( *nerr != 0 ){
		setmsg( "WARNING", *nerr );
		apcmsg( "Trouble reading line. Skipping...",34 );
		apcmsg( kcard,MCMSG+1 );
		outmsg();
	    }

	    /* --- Fill in the channels, according to the the content vector. */
	    else {
		for( jen = 1; jen <= nentry; jen++ ){
		    jch = Iopch[jen];
		    if( jch >= 0 ){
			if( nptch[jch] >= nlnch[jch] ){
			    newlen = 2*nlnch[jch];
			    allamb( &cmmem, newlen, &newndx, nerr );
			    if( *nerr != 0 )
				goto L_8888;
			    /* copy( (int*)cmmem.sacmem[ndxch[jch]], (int*)cmmem.sacmem[newndx], nptch[jch] ); */
			    copy_float( cmmem.sacmem[ndxch[jch]], cmmem.sacmem[newndx], nptch[jch] );
			    relamb( cmmem.sacmem, ndxch[jch], nerr );
			    if( *nerr != 0 )
				goto L_8888;
			    ndxch[jch] = newndx;
			    nlnch[jch] = newlen;
			}
			nptch[jch] = nptch[jch] + 1;
			ipt = nptch[jch] + 1;
			*( cmmem.sacmem[ ndxch[ jch ] ] + nptch[ jch ] - 1 ) =
			  Fentry[ jen ] ;
		    }
		}
	    }

	    /* -- Read in next card. */
	    if(fgetsp(kcard,MCMSG+1,nun) == NULL) {
		if(feof(nun)) goto L_5000;
		else goto L_9000;
	    }
	    if(kcard[(numchar=strlen(kcard)-1)] == '\n')kcard[numchar] = '\0';

	    /* replace tabs with blanks */
	    for( idx=0; idx<numchar; idx++) if(kcard[idx] == '\t')
		kcard[idx] = ' ';

	    /* --- Convert card to list of floating point values.
	     *     Each card is in either free field or formatted. */
	    if( lfree ){
		if( ipt >= MALPHA )
		    ipt = MALPHA;

		cnvfre( kcard,MCMSG+1, MENTRY, &nentry, fentry, iopch,
			(char*)kalpha[ipt - 1] ,21, 1, nerr );
	    }
	    else{
		cnvfmt( kcard,MCMSG+1, kform,MCMSG+1, nentry, fentry, nerr );
	    }
	    /* --- Loop on each card in input file. */
	    goto L_4000;

	    /* -- Come to here on end-of-file. */

L_5000:
	    /* -- Close input file. */
	    zcloses( &nun, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Copy pointers to and sizes of y blocks to appropriate
	     *     data file list variables. */
	    for( jch = 1; jch <= numych; jch++ ){
		jdfl = cmdfm.ndfl + jch;
		Ncomp[jdfl] = 1;
		cmdfm.ndxdta[jdfl - 1][0] = ndxch[jch];
		Nlndta[jdfl] = nptch[jch];
	    }

	    /* -- Copy pointer to x block if there is one.
	     *    Allocate extra x blocks and copy original if necessary. */
	    if( numxch == 1 ){
		jdfl = cmdfm.ndfl + 1;
		cmdfm.ndxdta[jdfl - 1][1] = ndxch[0];
		Ncomp[jdfl] = 2;
		for( jch = 2; jch <= numych; jch++ ){
		    jdfl = cmdfm.ndfl + jch;
		    allamb( &cmmem, Nlndta[jdfl], &cmdfm.ndxdta[jdfl - 1][1], 
			    nerr );
		    if( *nerr != 0 )
			goto L_8888;

		    Ncomp[jdfl] = 2;
		    /* copy( (int*)cmmem.sacmem[ndxch[0]],  (int*)cmmem.sacmem[cmdfm.ndxdta[jdfl - 1][1]],  Nlndta[jdfl] ); */
		    copy_float( cmmem.sacmem[ndxch[0]], cmmem.sacmem[cmdfm.ndxdta[jdfl - 1][1]], Nlndta[jdfl] );
		}
	    }

	    /* -- Allocate space for header(s). */
	    for( jch = 1; jch <= numych; jch++ ){
		jdfl = cmdfm.ndfl + jch;
		allamb( &cmmem, SAC_HEADER_WORDS, &Ndxhdr[jdfl], nerr );
		if( *nerr != 0 )
		    goto L_8888;
	    }

	    /* -- Build header for each new file and move it to SACMEM block */
	    newhdr();
	    for( jch = 1; jch <= numych; jch++ ){
		jdfl = cmdfm.ndfl + jch;
		*npts = Nlndta[jdfl];
		if( numxch == 0 ){
		    *leven = TRUE;
		    *delta = 1.;
		    *begin = 0.;
		    *ennd = *begin + (float)( *npts - 1 )**delta;
		}
		else{
		    *leven = FALSE;
		    *delta = cmhdr.fundef;
		    extrma( cmmem.sacmem[cmdfm.ndxdta[jdfl - 1][1]], 1, *npts, 
			    begin, ennd, &unused );
		}
		extrma( cmmem.sacmem[cmdfm.ndxdta[jdfl - 1][0]], 1, *npts, 
			depmin, depmax, depmen );

		putfil( jdfl, nerr );
		if( *nerr != 0 )
		    goto L_8888;
	    }

	    /* -- Create new file names for these output files. */
	    if( numych == 1 ){
            string_list_put(datafiles, file, strlen(file));
	    }
	    else{
            for( jch = 0; jch < numych; jch++ ){
                cattemp = malloc(strlen(file) + strlen(kmdfm.ksuffx[jch])+1);
                sprintf(cattemp, "%s%s", file, kmdfm.ksuffx[jch]);
                string_list_put(datafiles, cattemp, strlen(cattemp));
                free(cattemp);
                if( *nerr != 0 )
                    goto L_8888;
            }
	    }

	    /* -- Update current number of files in data file list. */
	    cmdfm.ndfl = cmdfm.ndfl + numych;
	}

	/* - Check again for a non-null DFL. */
	if( cmdfm.ndfl <= 0 ){
	    *nerr = 1301;
	    setmsg( "ERROR", *nerr );
	}

	/* -- UPDATE DATA-SET STORAGE - BEGIN
	 * -- Save number of files in this (the current) data-set. */
	/* -- For each file get next available data-set storage slot index. */
	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	    Ndsndx[jdfl] = 1 ;
	}

	/* -- UPDATE DATA-SET STORAGE - END */
	/* - Calculate range of dependent variable for use during plotting. */
	setrng();

	/* - Make a plot if requested. */
	if( lplot )
	    plalpha( (char*)kalpha,21, MALPHA, lprint , nerr );
	/* - Return. (Try to close alphanumeric data file just to be sure.) */

    string_list_clear(last_list);
    string_list_extend(last_list, list);
    string_list_free(list);
    list = NULL;

L_8888:
	if ( *nerr == 0 ) { 
	    cmdfm.nreadflag = LOW ;
	    cmdfm.lread = TRUE ;
	    sacToSeisMgr ( !lmore , FALSE , TRUE , nerr ) ;
	    cmdfm.lread = FALSE ;
	}
	return;

L_9000:
	*nerr = 114;
	setmsg( "ERROR", *nerr );
    apcmsg2(file, strlen(file)+1);
	goto L_8888;
    
}

