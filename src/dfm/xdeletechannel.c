/** 
 * @file   xdeletechannel.c
 * 
 * @brief  Delete a data file 
 * 
 */

#include <string.h>

#include "dfm.h"
#include "amf.h"

#include "cssListOps/cssStrucs.h"
#include "cssListOps/cssListStrucs.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "smDataIO.h"


#include "ucf.h"
#include "ssi.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

/** 
 * Execute the command DELETECHANNEL which deletes one or more channels from
 *    the file list
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   970203:  Original version copied from xdeletestack.  maf
 *
 */
void 
xdeletechannel(int *nerr) {

	char kfile[MCPFN+1];
	int lincr;

	int lall = FALSE ;	
	int idel[MDFL], jdel, jdfl, jdfl2, jdfl2_;
	int jdfl3, ncfile, ndel, ntused ;
	int first , last ; 
    char *tmp;
	int *const Idel = &idel[0] - 1;

	DBlist tree ;

	tree = smGetDefaultTree () ;

	*nerr = 0;
	ndel = 0;

    memset(kfile, 0, sizeof(kfile));
	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){

	    if( lckey( "ALL#$",6 ) )	/* ALL option added. maf 970902 */
		lall = TRUE ;

	    /* -- "n":  the number of a file. */
	    else if( lcint( &jdfl ) ){
		if( jdfl < 1 || jdfl > cmdfm.ndfl ){
		    *nerr = 5107;
		    setmsg( "ERROR", *nerr );
		    apimsg( jdfl );
		    goto L_8888;
		}
		ndel = ndel + 1;
		Idel[ndel] = jdfl;
	    }

	   /* -- "n-m": a range of filenumbers denoted by the first and last 
			numbers in the range separated by a dash (-). */
	    else if ( lcidi ( &first , &last ) ) {
		if ( first < 1 || last > cmdfm.ndfl ) {
		    *nerr = 5107 ;
		    setmsg ( "ERROR" , *nerr ) ;
		    apimsg ( first < 1 ? first : last ) ;
		    goto L_8888 ;
		}
		for ( jdfl = first ; jdfl <= last ; jdfl++ ) {
		    ndel++ ;
		    Idel[ ndel ] = jdfl ;
		}
	    }

	    /* -- "filename":  the name of a file. */
	    else if( lcchar( MCPFN, kfile,MCPFN+1, &ncfile ) ){
            jdfl = 1 + string_list_find(datafiles, kfile, MCPFN+1);
            if( jdfl <= 0 ){
                *nerr = 5106;
                setmsg( "ERROR", *nerr );
                apcmsg( kfile,MCPFN+1 );
                goto L_8888;
            }
            ndel = ndel + 1;
            Idel[ndel] = jdfl;
	    }

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }

	}

	/* EXECUTION PHASE: */
	/* - First do case of ALL option not specified */
	if ( !lall ) {
	    /* - Sort the list of file numbers into decreasing order */
	    lincr = FALSE;
	    sorti( idel, ndel, lincr, idel );

	    /* - For each file to be deleted: */

	    jdfl = 0;
	    for( jdel = 1; jdel <= ndel; jdel++ ){
	        if( Idel[jdel] != jdfl ){
		    jdfl = Idel[jdel];
		    /* -- Release memory blocks. */
		    if( Ndxhdr[jdfl] > 0 )
		        relamb( cmmem.sacmem, Ndxhdr[jdfl], nerr );
		    if( cmdfm.ndxdta[ jdfl - 1 ][ 0 ] > 0 )
		        relamb( cmmem.sacmem, cmdfm.ndxdta[ jdfl - 1 ][ 0 ], nerr );
		    if( cmdfm.ndxdta[ jdfl - 1 ][ 1 ] > 0 )
		        relamb( cmmem.sacmem, cmdfm.ndxdta[ jdfl - 1 ][ 1 ], nerr );
		    /* -- Remove entry from list of data file names. */
            tmp = string_list_get(datafiles, jdfl-1);
            fstrncpy( kfile, MCPFN, tmp, strlen(tmp)+1);
            string_list_delete(datafiles, jdfl-1);
		    /* -- Move DFM array variables down.           */
		    for( jdfl2 = jdfl; jdfl2 <= (cmdfm.ndfl - 1); jdfl2++ ){
		        jdfl2_ = jdfl2 - 1;
		        jdfl3 = jdfl2 + 1;
		        Ndxhdr[jdfl2] = Ndxhdr[jdfl3];
		        Nlndta[jdfl2] = Nlndta[jdfl3];
		        cmdfm.ndxdta[jdfl2_][0] = cmdfm.ndxdta[jdfl3 - 1][0];
		        cmdfm.ndxdta[jdfl2_][1] = cmdfm.ndxdta[jdfl3 - 1][1];
		        Ncomp[jdfl2] = Ncomp[jdfl3];

		        /* Added.  maf  970203 */
		        Nstart  [ jdfl2 ] = Nstart  [ jdfl3 ] ;
		        Nstop   [ jdfl2 ] = Nstop   [ jdfl3 ] ;
		        Nfillb  [ jdfl2 ] = Nfillb  [ jdfl3 ] ;
		        Nfille  [ jdfl2 ] = Nfille  [ jdfl3 ] ;
		        Ntotal  [ jdfl2 ] = Ntotal  [ jdfl3 ] ;
		        Nxsdd   [ jdfl2 ] = Nxsdd   [ jdfl3 ] ;
		        Ndsndx  [ jdfl2 ] = Ndsndx  [ jdfl3 ] ;
		    }
		    /* -- Decrement file count. */
		    cmdfm.ndfl = cmdfm.ndfl - 1;
	        } 
	    } 

	    for ( jdel = 0 ; jdel < ndel ; jdel++ )
	      idel[ jdel ] -- ;
	    
	    dblDeleteWfdiscs ( tree , (int *) idel , ndel ) ;
	}

	/* Now handle case of ALL option specified.  maf 970902 */
	else {
	    /* declare Workset name */
	    char * worksetName ;

	    /* delete files from SAC memory */
	    deleteAllSacFiles ( nerr , TRUE ) ;

	    /* delete files from SeisMgr regardless of lcommit */
	    worksetName = smGetDefaultWorksetName () ;
	    if ( worksetName )
		smDeleteWorksetByName ( worksetName ) ;

	    /* if COMMIT is set, warn user */
	    if ( lall && cmdfm.lcommit ) {
		setmsg ( "WARNING" , 1382 ) ;
		outmsg () ;
		clrmsg () ;
	    }
	}
	    


	/* If all files were deleted, let the user know. */
	if ( cmdfm.ndfl < 1 )
	    printf ( "\nAll files deleted.\n" ) ;

        /* Else make first file in the smaller list current.
           This is done to make sure that cmdfm.idflc is 
	   consistant with the working data. */
	else 
	    getfil( 1, FALSE, &ntused, &ntused, &ntused, nerr );

L_8888:
	return;

}

