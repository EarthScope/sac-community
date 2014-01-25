/** 
 * @file   readfl.c
 * 
 * @brief  Read a filelist into memory
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#include "wild.h"
#include "bot.h"
#include "msg.h"
#include "clf.h"
#include "co.h"
#include "dff.h"

#ifdef HAVE_LIBRPC 
#include <rpc/rpc.h>
#endif /* HAVE_LIBRPC */

#include "dfm.h"
#include "bool.h"
#include "amf.h"
#include "hdr.h"

#include "errors.h"
#include "debug.h"

extern enum filetype { 
    sacfi, 
    alpha, 
    xdr, 
    segy 
} ftype ;

/** 
 * Read a data filelist into memory
 * 
 * @param ldata 
 *    - TRUE read in the header and data
 *    - FALSE read in only the header
 * @param lmore 
 *    - TRUE files read in are to be appended to current list
 *    - FALSE files read in are to replace the current list
 * @param lsdd 
 *    - TRUE files are in SDD format
 *    - FALSE files are not in SDD format
 * @param kdirin 
 *    Default input directory if the filenames do not contain a directory part
 * @param kdirin_s 
 *    Length of \p kdirin
 * @param kdflin 
 *    Input file list
 * @param kdflin_s 
 *    Length of \o kdflin
 * @param ndflin 
 *    Number of entries in \p kdflin
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_NO_DATA_FILES_READ_IN
 *    - ERROR_UNABLE_TO_READ_SOME_FILES
 *
 * @date   920619:  Changed lfilesok args to use kdflrq, includes directory,
 *             and use " " for source directory. Bug introduced while
 *             fixing READ DIR option.
 * @date   920618:  Commented redundent appends that were probably added when
 *             readcss was implemented. Did not cause a problem unless
 *             "DIR" option was used in the read. Fixes bug reported 
 *             6/15/92 by Owens and others.
 * @date   920501:  Added memory delete on NOFILES and MEMORY DELETE READERR
 *             contitions.
 * @date   920418:  Added call to lfilesok, and moved memory setup stuff 
 *             into logical function, lfilesok. This avoids memory 
 *             shuffle when no files match the requested filelist.
 * @date   910115:  Added file count for current data-set.
 * @date   910826:  Changed kopen = ""  ---> kopen=' ' at 2 places to improve
 *             portability to the DEC 2000 workstations,
 *             per Gyu-sang Jang at UC Davis.
 * @date   910813:  Added multiple data-set stuff; changing many of the data
 *             structure names used for storing information about 
 *             pointers, component count, file size, etc.
 *
 * @note       Some of this may seem a little backwards in that some
 *             subroutines load info into working-storage, and the info
 *             is then copied to permanent data-set storage; But many 
 *             subroutines use the working storage and so this apporach 
 *             avoids making changes in many other subroutines.
 *
 * @bug This function is too long.  Many parts could be moved into separate 
 *      routines.
 *
 * @date   910610:  Fixed bug in opening file not referenced from pwd
 * @date   910222:  Better handling of bombed allamb -- back out to ndflsv.
 * @date   890417:  Fixed bug in processing of errors in read list.
 * @date   880308:  Added LDATA subroutine argument.
 * @date   870915:  Temporarily softened failure mode when memory requirement
 *             exceeds available room.  Still need to add buffered mode.
 * @date   870908:  Fixed bug in do loops limits after file deletion.
 * @date   870626:  Extracted from xr subroutine.
 * @date   860918:  Changed to character lists for data file list storage.
 * @date   860130:  Changed to new message handling package.
 * @date   850409:  Major rewrite due to addition of memory manager.
 * @date   850321:  Fixed bug involving MORE option and null DFL.
 * @date   830930:  Moved range calculation into it's own subroutine.
 * @date   811230:  Added MORE option.
 * @date   810708:  Added logic to compute range of dependent variable.
 * @date   810120:  Changed to output message retrieval from disk. 
 * @date   800512:  Reworked error messages.
 *
 */
void 
readfl(int   ldata, 
       int   lmore, 
       int   lsdd, 
       char *kdirin, 
       int   kdirin_s,
       string_list *list,
       int  *nerr) {

    int i;
	char kcdir[MCPFN+1], kfile[MCPFN+1], kpdir[MCPFN+1];
	int lexpnd, lrdrem, lheader;
	int iflag, idx, j0, j1, j2, jcomp,
	 jdfl, jdflrq, jstart, ncerr, ndflrq, 
	 ndflsv, ntused, nun, lswap[ MDFL ] ;
    char *cattemp;
    char *strtemp;
    
    string_list *files;

#ifdef HAVE_LIBRPC
        XDR xdrs;
#else 
        if(ftype == xdr) {
          *nerr = ERROR_NO_DATA_FILES_READ_IN;
          return;
        }
        #endif /* HAVE_LIBRPC */

	for( idx = 0 ; idx < MDFL ; idx++ )
		lswap[ idx ] = 0 ;

	/* PROCEDURE: */
	*nerr = 0;
	for( idx = 0 ; idx < MCPFN ; idx++ ) {
	    kcdir[ idx ] = ' ' ;
	    kfile[ idx ] = ' ' ;
	    kpdir[ idx ] = ' ' ;
	}
	kcdir[MCPFN] = '\0' ;
	kfile[MCPFN] = '\0' ;
	kpdir[MCPFN] = '\0' ;


	/* - Set up indices and flags depending upon lmore flag.
	 * - Echo out what type of read this is, and to what data-set. */
	if( lmore ){
	    ndflsv = cmdfm.ndfl;
	}
	else{
	  ndflsv = 0;
	}

	/* - Convert KDFLIN which may contain wild-cards, predefined file sets,
	 *   etc. into an expanded file list. */
    DEBUG("wildfl\n");
	files = wildfl( kdirin, kdirin_s, list, &lexpnd );
	fstrncpy( kpdir, MCPFN, " ", 1);
    ndflrq = string_list_length(files);
    DEBUG("wildfl: done\n");
	/* - Echo expanded filelist if requested. */
	if( (cmdfm.lechof && lexpnd) && ndflrq > 0 ){
	    setmsg( "OUTPUT", 0 );
        
	    /* -- Loop until all pathnames in expanded filelist 
           have been processed. */
        for(i = 0; i < string_list_length(files); i++) {
            strtemp = string_list_get(files, i);
            
            /* -- Break pathname into directory and filename parts. */
            getdir( strtemp, strlen(strtemp)+1, kcdir,MCPFN+1, kfile,MCPFN+1 );
            /* -- Echo the filename part if there is no directory part. */
            if( memcmp(kcdir," ",1) == 0 ){
                apcmsg( kfile,MCPFN+1 );
            }
            /* -- Prepend the filename part with some special characters if
             *    directory part is same as that of the previous file. */
            else if ( memcmp ( kcdir , kpdir , 
                               min ( strlen ( kcdir ) ,
                                     strlen ( kpdir ) ) ) == 0 ){
                cattemp = malloc(3+strlen(kfile)+1);
                strcpy(cattemp,"...");
                strcat(cattemp,kfile);
                apcmsg( cattemp, 3+strlen(kfile)+1 );
                free(cattemp);
            }
            /* -- Echo complete pathname if directory part is different. */
            else{
                apcmsg2(strtemp, strlen(strtemp)+1);
                strcpy( kpdir, kcdir );
            }
	    } 
	    wrtmsg( MUNOUT );
	} 
    DEBUG("\n");
	/* -- Test to see whether to expect a sac header. */
	if ( ftype == alpha || ftype == segy || lsdd == TRUE )
	    lheader = FALSE ;
	else
	    lheader = TRUE ;

    DEBUG("lfilesok\n");
	/* -- Test to see that at least one file exists. */
	if( !lfilesok(files, NULL, 0, lmore, lheader, ftype == xdr, nerr ) ){
        DEBUG("files not ok\n");
	    /* --- If destroying files in memory, */
        if( strcmp(kmdfm.kecmem,"DELETE  ") == 0 ) {
            cleardfl( nerr );
        }
	    *nerr = ERROR_NO_DATA_FILES_READ_IN;
	    goto L_8888;
	}
    DEBUG("\n");
	/* - Test number of files requested, limit read to max files SAC can
	 *   handle.  Spit out a message about not being able to read all files
	 */
	if( string_list_length(files) > MDFL ){
	    ndflrq = MDFL;
	    clrmsg();
	    setmsg( "OUTPUT", 0 );
	    apcmsg( "Max files: reading first ",26 );
	    apimsg( MDFL );
	    apcmsg( " files.",8 );
	    wrtmsg( MUNOUT );
	    clrmsg();
	}

	if( ! lmore) {
	  cleardfl(nerr);
	  if(*nerr != 0) {
	    clrmsg();
	    setmsg("OUTPUT", 0);
	    apcmsg("Error clearing Data File List ", 30);
	    wrtmsg( MUNOUT );
	    clrmsg();
	  }
	}

	jstart = 1;
	lrdrem = FALSE;
	cmdfm.ndfl = ndflsv + ndflrq;
	

L_1800:
	iflag = 0 ;
	/* - If it's alpha files, handle that separately. */
	if ( ftype == alpha ) {
	    cmdfm.nreadflag = LOW ;
	    for ( jdflrq = jstart ; jdflrq <= ndflrq ; jdflrq++ ) {
            int nlen, ndx1, ndx2 ;
            jdfl = ndflsv + jdflrq ;

            /* -- Get name of requested file and store in data file list. */
            strtemp = string_list_get(files, jdflrq-1);
            fstrncpy( kfile, MCPFN, strtemp, strlen(strtemp)+1);

            rdci ( jdfl , kfile , MCPFN , &nlen, &ndx1, &ndx2, nerr ) ;

            if ( *nerr ) {
                strcpy ( kmdfm.kecbdf , "WARNING " ) ;
                goto L_4000 ;
            }
	    }
	    *nerr = 0;
	    goto L_4000 ;
	}

	/* - If it's segy files, handle that separately. */
        if ( ftype == segy ) {
	    cmdfm.nreadflag = LOW ;
            for ( jdflrq = jstart ; jdflrq <= ndflrq ; jdflrq++ ) {
                int nlen, ndx1, ndx2 ;
                jdfl = ndflsv + jdflrq ;

                /* -- Get name of requested file and store in data file list. */
                strtemp = string_list_get(files, jdflrq-1);
                fstrncpy( kfile, MCPFN, strtemp, strlen(strtemp)+1);
                terminate ( kfile ) ;

                rdsegy ( jdfl , kfile , &nlen, &ndx1, &ndx2, nerr ) ;

                if ( *nerr ) {
                    strcpy ( kmdfm.kecbdf , "WARNING " ) ;
                    goto L_4000 ;
                }
            }
            *nerr = 0;
            goto L_4000 ;
        }

	/* - Read headers into memory. */

L_2000:
	iflag = 1;
	for( jdflrq = jstart; jdflrq <= ndflrq; jdflrq++ ){
	    jdfl = ndflsv + jdflrq;
        DEBUG("getfile: %d/%d\n", jdfl, ndflrq);
	    /* -- Get name of requested file and store it in data file list. */
        strtemp = string_list_get(files, jdflrq-1);
	    fstrncpy( kfile, MCPFN, strtemp, strlen(strtemp)+1);
        DEBUG("add file to list\n");
        string_list_put(datafiles, kfile, MCPFN+1);
	    if( *nerr != 0 )
            goto L_4000;

	    /* -- Open file. */
        if( ftype == xdr ) {
#ifdef HAVE_LIBRPC 
            znfiles(&fileun, kfile, MCPFN+1, "TEXT", 5, nerr);
            if( *nerr == 0 ){
                xdrstdio_create(&xdrs, fileun, XDR_DECODE);
            }
            else{
                goto L_4000;
            }
#endif /* HAVE_LIBRPC */
	    }
	    else{
            zopen_sac( &nun, kfile,MCPFN+1, "RODATA",7, nerr );
            if( *nerr != 0 )
                goto L_4000;
	    }

        DEBUG("Allocate header block\n");
	    /* -- Allocate block for header. */
	    allamb( &cmmem, SAC_HEADER_WORDS, &Ndxhdr[jdfl], nerr );
	    if( *nerr != 0 )
            goto L_4000;
        
	    for( idx = 0 ; idx < SAC_HEADER_WORDS ; idx++ ) {
            cmmem.sacmem[Ndxhdr[jdfl]][idx] = 0 ;
        }

	    /* -- Copy header data-set ptr to working storage. */
	    Nlndta[jdfl] = 0 ;	/* no data yet */

	    /* -- Set the file number (position) */
	    Ndsndx[jdfl] = 1 ;

        DEBUG("Read header block\n");        
	    /* -- Read header. */
	    if( lsdd ){
            rdshdr( jdfl, &nun, nerr );
	    }
	    else if( ftype == xdr ){
#ifdef HAVE_LIBRPC
            xdrhdr( xdrs, cmmem.sacmem[Ndxhdr[jdfl]], nerr);
#endif /* HAVE_LIBRPC */
	    }
	    else{
            lswap[ jdfl ] = rdhdr(jdfl, 
                                  &nun, 
                                  string_list_get(datafiles,-1), 
                                  nerr);
	    }
        
	    if( *nevid == -12345 || *norid == -12345 )
            cmdfm.nreadflag = LOW ;

	    if( *nerr != 0 )
            goto L_4000;

        DEBUG("Close File\n");
	    /* -- Close file. */
        if( ftype == xdr ){
#ifdef HAVE_LIBRPC 
            xdr_destroy( &xdrs );
            zcloses( &fileun, nerr);
#endif /* HAVE_LIBRPC */
	    }
	    else{
            DEBUG("zclose\n");
            zclose( &nun, nerr );
            DEBUG("zclose: done: %d\n", *nerr);
	    }
	    if( *nerr != 0 )
            goto L_4000;
	}
    
	/* - Skip to bottom if only headers are to be read. */
    DEBUG("ldata: %d\n", ldata);
	if( !ldata )
	    goto L_4000;

	/* - Define memory requirements for each file. */

	jstart = 1;

L_2200:
	iflag = 2;
    DEBUG("Define memory requirements\n");        
	for( jdflrq = jstart; jdflrq <= ndflrq; jdflrq++ ){
	    jdfl = ndflsv + jdflrq;
	    getfil( jdfl, FALSE, &ntused, &ntused, &ntused, nerr );
	    if( *nerr != 0 )
            goto L_4000;
	    defmem( jdfl, TRUE, nerr );
	    if( *nerr != 0 )
            goto L_4000;
	    putfil( jdfl, nerr );
        
	    if( *nerr != 0 )
            goto L_4000;
	}
    
	/* - Allocate memory for each data file.
	 *   If there is not enough room, release allocated memory
	 *   and switch to buffered mode. */
    
	jstart = 1;
    
L_2400:

  DEBUG("Allocate data block\n");
	/* iflag = 3; */

	for( jdflrq = jstart; jdflrq <= ndflrq; jdflrq++ ){
	    jdfl = ndflsv + jdflrq;
	    for( jcomp = 0; jcomp < Ncomp[jdfl]; jcomp++ ){
            allamb ( &cmmem , Nlndta[ jdfl ] ,
                     &cmdfm.ndxdta[ jdfl - 1 ][ jcomp ] , nerr ) ;
            
            if( *nerr != 0 ){
                outmsg () ;
                
                for( j0 = ndflsv + jstart; j0 <= (ndflsv + ndflrq); j0++ )
                    relamb( cmmem.sacmem, Ndxhdr[j0], nerr );
                
                if( *nerr != 0 )
                    goto L_8888;
                for( j1 = ndflsv + jstart; j1 < jdfl ; j1++ ){
                    for( j2 = 0; j2 < Ncomp[j1]; j2++ ){
                        relamb(cmmem.sacmem, cmdfm.ndxdta[j1-1][j2], nerr);
                    }
                }
                if( *nerr != 0 )
                    goto L_8888;
                for( j2 = 0; j2 < jcomp ; j2++ ){
                    relamb(cmmem.sacmem, cmdfm.ndxdta[jdfl - 1][j2], nerr);
                }
                if( *nerr != 0 )
                    goto L_8888;
                
                cmdfm.ndfl = ndflsv;
                
                goto L_8888 ;
            }
	    } /* end for( jcomp = 1; jcomp <= Ncomp[jdfl]; jcomp++ ) */
	} /* end for ( jdflrq = jstart; jdflrq <= ndflrq; jdflrq++ ) */

	/* - Now we are finally ready to actually read in the data. */

	jstart = 1;
L_3000:
    DEBUG("Read data block\n");
	iflag = 4;
	for( jdflrq = jstart; jdflrq <= ndflrq; jdflrq++ ){
	    jdfl = ndflsv + jdflrq;
	    getfil( jdfl, FALSE, &ntused, &ntused, &ntused, nerr );
	    if( *nerr != 0 )
		goto L_4000;
	    /* -- Open file. */
        strtemp = string_list_get(files, jdflrq-1);
	    fstrncpy( kfile, MCPFN, strtemp, strlen(strtemp)+1);
	    if ( ftype != xdr ){
                zopen_sac( &nun, kfile,MCPFN+1, "RODATA",7, nerr );
		if( *nerr != 0 )
		    goto L_4000;
	    }
	    /* -- Read data. */
	    if( lsdd ){
		rdsdta( jdfl, &nun, nerr );
	    }
	    else if( ftype == xdr ){
                #ifdef HAVE_LIBRPC 
		rdxdrdta( jdfl, kfile, MCPFN+1, nerr );
                #endif /* HAVE_LIBRPC */
	    }
	    else{
		rddta( jdfl, &nun, lswap[ jdfl ], nerr );
	    }
	    if( *nerr != 0 )
		goto L_4000;
	    /* -- Close file. */
	    if( ftype != xdr )zclose( &nun, nerr );
	}

	/* - If an error occurred anywhere in read, either quit
	 *   or delete this file from DFL.  See READERR command. */

L_4000:
	if( *nerr != 0 ){
	    zclose( &nun, &ncerr );
	    if( strcmp(kmdfm.kecbdf,"FATAL   ") == 0 ){
            goto L_8888;
	    }
	    else{
            strtemp = string_list_get(files, jdflrq-1);
            fstrncpy( kfile, MCPFN, strtemp, strlen(strtemp));

            if( strcmp(kmdfm.kecbdf,"WARNING ") == 0 ){
                typmsg( "WARNING" );
                outmsg();
            }
            *nerr = 0;
            lrdrem = TRUE;
            string_list_delete(datafiles, 
                               string_list_find(datafiles, kfile, MCPFN+1));
            /* -- Delete name from file lists and reset do loop variables */
            string_list_delete(files, jdflrq-1);
		cmdfm.ndfl = cmdfm.ndfl - 1;
		ndflrq = ndflrq - 1;
		cmdfm.ndsflcnt = cmdfm.ndsflcnt - 1;
		jstart = jdflrq;
		/* -- Depending upon where we are in the reading process, we
		 *    must also move some data length and index parameters. */
		for( jdflrq = jstart; jdflrq <= ndflrq; jdflrq++ ){
		    jdfl = ndflsv + jdflrq;

		    /* --- Get the index position in data-set storage for
			   the next file's info. */
		    if( iflag >= 2 ){
			Ndxhdr[jdfl] = Ndxhdr[jdfl + 1];
		    }

		    if( iflag >= 3 ){
			Nstart[jdfl] = Nstart[jdfl + 1];
			Nstop[jdfl] = Nstop[jdfl + 1];
			Nfillb[jdfl] = Nfillb[jdfl + 1];
			Nfille[jdfl] = Nfille[jdfl + 1];
			Ncomp[jdfl] = Ncomp[jdfl + 1];
			Nlndta[jdfl] = Nlndta[jdfl + 1];
		    }

		    if( iflag >= 4 ){
			for( jcomp = 0; jcomp < Ncomp[jdfl]; jcomp++ ){
			    cmdfm.ndxdta[ jdfl - 1 ][ jcomp ] =
				cmdfm.ndxdta[ jdfl ][ jcomp ] ;
			}
		    } /* end if( iflag >= 4 ) */
		} /* end for( jdflrq = jstart; jdflrq <= ndflrq; jdflrq++ ) */
		/* -- Jump to appropriate loop in the reading process. */
		switch( iflag ){
		    case 0: goto L_1800;
		    case 1: goto L_2000;
		    case 2: goto L_2200;
		    case 3: goto L_2400;
		    case 4: goto L_3000;
		} /* end switch */
	    } /* end else associated with if ( strcmp ( kmdfm.kecbdf ... ) */
	} /* end if ( *nerr != 0 ) */

	/* - Check again for a non-null DFL. */
	if( cmdfm.ndfl <= 0 ){
  	    *nerr = ERROR_NO_DATA_FILES_READ_IN;
	    setmsg( "ERROR", *nerr );
	}

	/* - Write a warning message if some of the files could not be read. */
	else if( lrdrem && strcmp(kmdfm.kecbdf,"WARNING ") == 0 ){
	    setmsg( "WARNING", ERROR_UNABLE_TO_READ_SOME_FILES );
	    apcmsg( " reading the rest of the files.", 32 ) ;
	    outmsg();
	    clrmsg();
	}

	/* - Calculate range of dependent variable for use during plotting. */
	setrng();

L_8888:
	return;
}

