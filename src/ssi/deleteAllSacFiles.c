
#include "ssi.h"
#include "dfm.h"
#include "amf.h"

#include "co.h"
#include "msg.h"
#include "clf.h"
#include "debug.h"
/* return the number of files deleted */

int
deleteAllSacFiles(int *nerr, int lname) /* TRUE deletes file name list, FALSE preserves it */
{
    /* Declare Variables. */
    int returnValue;
    UNUSED(lname);
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

    *nerr = 0;
    /* loop between sac files. */
    //for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
    /* -- Release memory blocks. */
    //  if( Ndxhdr[jdfl] > 0 ) {
    //      if ( *nerr ) 
    //          goto L_8888 ;
    //  }
    //  if( cmdfm.ndxdta[ jdfl - 1 ][ 0 ] > 0 ) {
    //      if ( *nerr ) 
    //          goto L_8888 ;
    //  }
    //  if( cmdfm.ndxdta[ jdfl - 1 ][ 1 ] > 0 ) {
    //      if ( *nerr ) 
    //          goto L_8888 ;
    //  }
    /* -- Remove entry from list of data file names. */
    //if ( lname ) {
    //string_list_delete(datafiles, 0);
    //}
    /* -- Reset DFM array variables.           */
    //Ndxhdr[jdfl] = 0 ;
    //Nlndta[jdfl] = 0 ;
    //  cmdfm.ndxdta[jdfl][0] = 0 ;
    //  cmdfm.ndxdta[jdfl][1] = 0 ;
    //  Ncomp[jdfl] = 0 ;
    //Nstart  [ jdfl ] = 0 ;
    //  Nstop   [ jdfl ] = 0 ;
    //  Nfillb  [ jdfl ] = 0 ;
    //    Nfille  [ jdfl ] = 0 ;
    //  Ntotal  [ jdfl ] = 0 ;
    //  Nxsdd   [ jdfl ] = 0 ;
    //  Ndsndx  [ jdfl ] = 0 ;

    //    } /* end for( jdfl = 1; jdfl <= saclen(); jdfl++ ) */

    returnValue = saclen();

    return returnValue;

}                               /* end deleteAllSacFiles() */
