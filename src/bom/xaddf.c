/** 
 * @file   xaddf.c
 * 
 * @brief  Execute ADDF
 * 
 */

#include <string.h>
#include <math.h>

#include "amf.h"
#include "mach.h"
#include "hdr.h"
#include "dfm.h"
#include "bom.h"
#include "bool.h"
#include "cpf.h"
#include "dff.h"
#include "co.h"
#include "msg.h"
#include "ucf.h"

#include "errors.h"


#include "clf.h"

int
vbeven() {
    int nerr = 0;
    if( *iftype == *irlim || *iftype == *iamph ){
		nerr = ERROR_OPERATION_ON_SPECTRAL_FILE;
    }
    else if( !*leven ){
		nerr = ERROR_OPERATION_ON_UNEVEN_FILE;
    }
    else if( *iftype == *ixyz ) {
        nerr = ERROR_OPERATION_ON_XYZ_FILE;
    }
    return nerr;
}

int
isFatal(char *key) {
    return (strcmp(key, "FATAL   ") == 0);
}
int
isWarning(char *key) {
    return (strcmp(key, "WARNING ") == 0);
}
int
isIgnore(char *key) {
    return (strcmp(key, "IGNORE  ") == 0);
}

int
delta_equal(float t1, 
            float t2, 
            string_list *list1,
            string_list *list2,
            int n1,
            int n2) {
    int fatal, nerr;
    float value;

    if(isIgnore(kmbom.kecdel)) {
        return 0;
    }
    value = (t2 - t1)/ t2;
    if( !linrng( value, -RNDOFF, RNDOFF ) ){
        fatal = isFatal(kmbom.kecdel);
        nerr = ERROR_HEADER_FILE_MISMATCH;
        message((fatal) ? MERRORS : MWARNINGS, nerr, "DELTA %s %s",
                string_list_get(list1, n1-1), string_list_get(list2, n2-1));
        if(fatal) {
            return nerr;
        }
        outmsg();
        clrmsg();
    }
    return 0;
}

int
station_equal(char *name1, 
              char *name2,
              string_list *list1,
              string_list *list2,
              int n1,
              int n2) {
    int nerr;
    if(memcmp(name1, name2, strlen(name1)) != 0) {
        nerr = 1801;
        error(nerr, "KSTNM %s %s", 
              string_list_get(list1, n1-1), 
              string_list_get(list2,n2-1));
        return nerr;
    }
    return 0;
}

int
npts_equal(int npts1, 
           int npts2,
           string_list *list1,
           string_list *list2,
           int n1,
           int n2) {
    int fatal, nerr;
    
    if(isIgnore(kmbom.kecnpt)) {
        return 0;
    }
    if( npts1 != npts2 ){
        fatal = isFatal(kmbom.kecnpt);
        nerr = ERROR_HEADER_FILE_MISMATCH;
        message((fatal) ? MERRORS : MWARNINGS, nerr, "NPTS %s %s",
                string_list_get(list1, n1-1), string_list_get(list2, n2-1));
        if(fatal) {
            return nerr;
        }
        outmsg();
        clrmsg();
    }
    return 0;
}

int
time_equal(int time1[6], 
           int time2[6],
           float b1,
           float b2,
           string_list *list1,
           string_list *list2,
           int n1,
           int n2) {
    int err;
    int timeb1[6], timeb2[6];
    char s1[33], s2[33];
    float diff;
    if( ldttm( time1 ) && ldttm( time2 ) ){
        idttm( time1, b1, timeb1 );
        idttm( time2, b2, timeb2 );
        ddttm( timeb1, timeb2, &diff );
        if( fabs( diff ) > RNDOFF ){
            kadttm(timeb1, s1, 33, &err);
            kadttm(timeb2, s2, 33, &err);
            warning(1802, "\n   BEG1: %s %s\n   BEG2: %s %s", 
                    s1,string_list_get(list1, n1-1),
                    s2,string_list_get(list2, n2-1));
            outmsg();
            clrmsg();
        }
    }
    return 0;
}

/** 
 * Execute the action command "ADDF". This command adds a set of files 
 *   to data in memory 
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - ERROR_OPERATION_ON_UNEVEN_FILE
 *   - ERROR_OPERATION_ON_SPECTRAL_FILE
 *   - ERROR_HEADER_FILE_MISMATCH
 *
 * @date   881130:  Fixed bug in begin time error checking.
 * @date   850730:  Changes due to new memory manager.
 * @date   820809:  Changed to newest set of parsing and checking functions.
 * @date   820331:  Combined "parse" and "control" modules.
 * @date   810224:  Original version.
 *
 */
void 
xaddf(int *nerr) {
	int j, jbfl, jdfl, n1zdtm[6];
	int ndx1, ndx1b, ndx2, ndx2b, nlen, nlenb, npts1;
	int lnewhdr ; /* let header data come from new file */
	float begin1, delta1, delta2;
    float *Sacmem1, *Sacmem2;
    
    string_list *list;
	*nerr = 0;
    list = NULL;

	/* - Loop on each token in command: */
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
	    goto L_8888;

	/* CHECKING PHASE: */
	/* - Check for null data file list. */
	vflist( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Check to make sure all files are evenly 
	   spaced time series files. */
	vfeven( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Check for a null binop file list. */
    if(!list || string_list_length(list) <= 0) {
        *nerr = ERROR_BINOP_FILE_LIST_EMPTY;
        error(*nerr,"");
	    goto L_8888;
    }

	/* - Make sure each file in BFL are of the proper type. */
	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	    getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );
	    if( *nerr != 0 )
			goto L_8888;
	    npts1 = *npts;
	    delta1 = *delta;
	    copyi( nzdttm, n1zdtm, 6 );
	    begin1 = *begin;
	    jbfl = min( jdfl, string_list_length(list));
	    getbfl( list, jbfl, FALSE, &nlen, &ndx1, &ndx2, nerr );
	    if( *nerr != 0 )
		goto L_8888;

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
	    goto L_8888;

	/* EXECUTION PHASE: */

	/* - Perform the file addition on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	    /* -- Get the next file in DFL, moving header to CMHDR. */

	    getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	    delta2 = *delta;

	    /* -- Get the next file in the BFL, moving header to CMHDR. */

	    jbfl = min( jdfl, string_list_length(list));
	    getbfl( list, jbfl, TRUE, &nlenb, &ndx1b, &ndx2b, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	    *delta = delta2;

	    /* -- Output file is the shorter of two input files. */

	    npts1 = min( nlen, nlenb );
	    Nlndta[jdfl] = npts1;

	    /* -- Perform file addition on these two files. */

	    Sacmem1 = cmmem.sacmem[ndx1];
	    Sacmem2 = cmmem.sacmem[ndx1b];

	    for( j = 0; j <= (npts1 - 1); j++ ){
        *(Sacmem1++) += *(Sacmem2++);
	    }

	    /* -- Adjust header of file in DFL. */
	    if ( !cmbom.lnewhdr ) {
        getfil ( jdfl , FALSE , &nlen , &ndx1 , &ndx2 , nerr ) ;
      }
	    *npts = npts1 ;
	    Sacmem1 = cmmem.sacmem[ndx1];
	    extrma( Sacmem1, 1, *npts, depmin, depmax, depmen );

	    /* -- Return file in DFL to memory manager. */
      putfil( jdfl, nerr );

	    if( *nerr != 0 )
		goto L_8888;

	}

	/* - Release last binop file. */

	relbfl( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

} /* end of function */

