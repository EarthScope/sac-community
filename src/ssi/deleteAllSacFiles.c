
#include "ssi.h"
#include "dfm.h"
#include "amf.h"


#include "co.h"
#include "msg.h"
#include "clf.h"

/* return the number of files deleted */

int deleteAllSacFiles ( nerr , lname )
int * nerr ;
int lname ;	/* TRUE deletes file name list, FALSE preserves it */
{
    /* Declare Variables. */
    int  returnValue ;
    int jdfl;

    /*=====================================================================
     * PURPOSE:  To remove all files from SACs data file manager (dfm)
     *=====================================================================
     * INPUT ARGUMENTS:
     *=====================================================================
     * OUTPUT ARGUMENTS:
     *    nerr:    Error flag. Set to 0 if no error occurred.
     *=====================================================================
     * MODIFICATION HISTORY:
     *    971202:  Original version.  maf plagerized from deletechannel
     *===================================================================== */

    *nerr = 0 ;

    /* loop between sac files. */
    for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	/* -- Release memory blocks. */
	if( Ndxhdr[jdfl] > 0 ) {
	    relamb( cmmem.sacmem, Ndxhdr[jdfl], nerr );
	    if ( *nerr ) 
		goto L_8888 ;
	}
	if( cmdfm.ndxdta[ jdfl - 1 ][ 0 ] > 0 ) {
	    relamb( cmmem.sacmem, cmdfm.ndxdta[ jdfl - 1 ][ 0 ], nerr );
	    if ( *nerr ) 
		goto L_8888 ;
	}
	if( cmdfm.ndxdta[ jdfl - 1 ][ 1 ] > 0 ) {
	    relamb( cmmem.sacmem, cmdfm.ndxdta[ jdfl - 1 ][ 1 ], nerr );
	    if ( *nerr ) 
		goto L_8888 ;
	}
	/* -- Remove entry from list of data file names. */
	if ( lname ) {
        string_list_delete(datafiles, 0);
    }
	/* -- Reset DFM array variables.           */
	Ndxhdr[jdfl] = 0 ;
	Nlndta[jdfl] = 0 ;
	cmdfm.ndxdta[jdfl][0] = 0 ;
	cmdfm.ndxdta[jdfl][1] = 0 ;
	Ncomp[jdfl] = 0 ;
	Nstart  [ jdfl ] = 0 ;
	Nstop   [ jdfl ] = 0 ;
	Nfillb  [ jdfl ] = 0 ;
	Nfille  [ jdfl ] = 0 ;
	Ntotal  [ jdfl ] = 0 ;
	Nxsdd   [ jdfl ] = 0 ;
	Ndsndx  [ jdfl ] = 0 ;
	
    } /* end for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ) */

    returnValue = cmdfm.ndfl ;

    cmdfm.ndfl = 0 ;

    return returnValue ;

L_8888:
    setmsg ( "ERROR" , *nerr ) ;
    apimsg ( jdfl ) ;
    outmsg () ;
    clrmsg () ;
    return jdfl - 1 ;

} /* end deleteAllSacFiles() */
