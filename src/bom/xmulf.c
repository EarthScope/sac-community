/** 
 * @file   xmulf.c
 * 
 * @brief  Execute MULF
 * 
 */

#include <string.h>
#include <math.h>

#include "amf.h"
#include "bom.h"
#include "bool.h"
#include "dfm.h"
#include "hdr.h"
#include "cpf.h"
#include "dff.h"
#include "co.h"
#include "ucf.h"

#include "errors.h"


#include "msg.h"
#include "clf.h"
#include "exm.h"

/** 
 * Execute the MULF command. This command multiplies a set of files 
 *    by data in memory.
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_HEADER_FILE_MISMATCH
 *    - ERROR_OPERATION_ON_UNEVEN_FILE
 *    - ERROR_OPERATION_ON_SPECTRAL_FILE
 *
 * @date   920331:  Added msg about BINOPEER cmd when header delta differs.
 * @date   881130:  Fixed bug in begin time error checking.
 * @date   850730:  Changes due to new memory manager.
 * @date   820809:  Changed to newest set of parsing and checking functions.
 * @date   820331:  Combined "parse" and "control" modules.
 * @date   810224:  Original version.
 *
 */
void 
xmulf(int *nerr) {

	int jdx, jbfl, jdfl, n1zdtm[6];
	int ndx1, ndx1b, ndx2, ndx2b, nlen, nlenb, npts1;
	int lnewhdr ; /* let header data come from new file */
	float begin1, delta1, delta2;
    float *Sacmem1, *Sacmem2;

    string_list *list;
	*nerr = 0;

    list = NULL;

	while ( lcmore( nerr ) ){
	    /* -- NEWHDR:  take the header from the new file being merged in.*/
	    if ( lklog ( "NEWHDR" , 7 , &lnewhdr ) ) {
		cmbom.lnewhdr = lnewhdr ;
	    }

	    /* -- "filelist':  define a new binop filelist. */
	    if( ( list = lcdfl_wild() ) ){
		cmbom.ibflc = 0;
	    }

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
	    return ;

	/* CHECKING PHASE: */
	/* - Check for null data file list. */
	vflist( nerr );
	if( *nerr != 0 )
	    return ;

	/* - Check to make sure all files are evenly 
	 *   spaced time series files. */
	vfeven( nerr );
	if( *nerr != 0 )
	    return ;

	/* - Check for a null binop file list. */
    if(!list || string_list_length(list) <= 0) {
        *nerr = ERROR_BINOP_FILE_LIST_EMPTY;
        error(*nerr,"");
        return;
    }

	/* - Make sure each file in BFL are of the proper type. */
	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	    getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );
	    if( *nerr != 0 )
		return ;
	    npts1 = *npts;
	    delta1 = *delta;
	    copyi( nzdttm, n1zdtm, 6 );
	    begin1 = *begin;
	    jbfl = min( jdfl, string_list_length(list));
	    getbfl( list, jbfl, FALSE, &nlen, &ndx1, &ndx2, nerr );
	    if( *nerr != 0 )
		return ;

        if((*nerr = vbeven()) != 0) {
            error(*nerr, "%s", string_list_get(list, jbfl-1));
            goto L_8888;
        }
        if((*nerr = delta_equal(delta1, *delta, datafiles, list, jdfl, jbfl)) != 0) {
            goto L_8888;
        }
        if((*nerr = npts_equal(npts1, *npts, datafiles, list, jdfl, jbfl)) != 0) {
            goto L_8888;
        }
        if((*nerr = time_equal(n1zdtm, nzdttm, begin1, *begin, 
                               datafiles, list, jdfl, jbfl)) != 0) {
            goto L_8888;
        }        

	}

	/* - Release last binop file. */
	relbfl( nerr );
	if( *nerr != 0 )
	    return ;

	/* EXECUTION PHASE: */

	/* - Perform the file multiplication on each file in DFL. */
	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

	    /* -- Get the next file in DFL, moving header to CMHDR. */
	    getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
	    if( *nerr != 0 )
		return ;
	    delta2 = *delta;

	    /* -- Get the next file in the BFL, moving header to CMHDR. */
	    jbfl = min( jdfl, string_list_length(list));
	    getbfl( list, jbfl, TRUE, &nlenb, &ndx1b, &ndx2b, nerr );
	    if( *nerr != 0 )
		return ;
	    *delta = delta2;

	    /* -- Output file is the shorter of two input files. */
	    npts1 = min( nlen, nlenb );
	    Nlndta[jdfl] = npts1;

	    /* -- Perform file multiplication on these two files. */
	    Sacmem1 = cmmem.sacmem[ndx1];
	    Sacmem2 = cmmem.sacmem[ndx1b];
	    for( jdx = 0; jdx <= (npts1 - 1); jdx++ ){
		*(Sacmem1++) *= *(Sacmem2++);
	    }

	    /* -- Adjust header of file in DFL. */
	    if ( !cmbom.lnewhdr )
		getfil ( jdfl , FALSE , &nlen , &ndx1 , &ndx2 , nerr ) ;
	    *npts = npts1 ;
	    extrma( cmmem.sacmem[ndx1], 1, *npts, depmin, depmax, depmen );

	    /* -- Return file in DFL to memory manager. */
	    putfil( jdfl, nerr );
	    if( *nerr != 0 )
		return ;

	}

	/* - Release last binop file. */
	relbfl( nerr );
	if( *nerr != 0 )
	    return ;

	/* - Calculate and set new range of dependent variable. */
	setrng();
 L_8888:
    return;
}

