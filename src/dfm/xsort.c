/** 
 * @file xsort.c
 *
 * @brief Sort data file list based on a header value
 */

#include <stdlib.h>
#include <string.h>

#include "dfm.h"
#include "lhf.h"
#include "hdr.h"

#include "cssListOps/cssStrucs.h"
#include "cssListOps/cssListStrucs.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "smDataIO.h"
#include "sm_gc.h"


#include "msg.h"
#include "bot.h"
#include "ssi.h"
#include "cpf.h"
#include "dff.h"

static void reorder(int *v, int n, int s, int *c, int *t);

/** 
 * Sort the data file list (DFL) based on a header value.
 *
 * @param *nerr
 *    Error return flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @return Nothing
 *
 * @note ASCEND Sort ascendingly
 * @note COMMIT RECALLTRACE RECALL ROLLBACK Database functionality
 * @note DESCEND Sort decendingly
 * @note Header field to sort on
 *
 * First, gather the header variable used to sort.  Sort indicies based 
 *  on the gathered header variables using Index(). Finally, reorder the
 *  variables which define the order of the sac files in memory.
 *  The routine dblSortWfdiscList was doing the reordering, but did not
 *  work as of 070606 and was then wrapped in an if-else block.
 *
 * @note realIdx - Value of current, before sorted, file ids
 * @note currIdx - Index values after sorting
 * @note tempIdx - Temporary index values for rearrangement
 *
 * @see Index
 *
 * @date 070606:  Documented/Reviewed
 * @date 070606:  if-else block now wraps the SeisMgr database 
 *                  and the basic reorder() routines.
 *
 */
void 
xsort(int *nerr) {

    int i;
    char ktemp[ 9 ], **kdata = NULL ;
    int notUsed , notUsed1 , notUsed2;
    int icat , item , lok ;
    int jsort , jdfl, jdx ;

    int nSortOrder = 0 ;

    float *fdata = NULL ;
    int *ldata = NULL ;
    int *tempIdx , *currIdx = NULL ;
    int *realIdx = NULL ;
    char * knewnames = NULL;
    int Verbose = 0;

    DBlist tree ;

    ktemp[ 0 ] = '\0' ;

    /* PARSING MODE */
    while ( lcmore ( nerr ) ) {

	/* -- "COMMIT|RECALLTRACE|ROLLBACK": how to treat existing data */
	if ( lckeyExact ( "COMMIT" , 7 ) )
	    cmdfm.icomORroll = COMMIT ;
	else if (lckeyExact ( "RECALLTRACE" , 12 ) )
	    cmdfm.icomORroll = RECALL ;
	else if ( lckeyExact ( "RECALL" , 7 ) )
	    cmdfm.icomORroll = RECALL ;
	else if ( lckeyExact ( "ROLLBACK" , 9 ) )
	    cmdfm.icomORroll = ROLLBACK ;


	if ( !lcmore ( nerr ) )
	    break ;

	/* -- ASCEND */
	if ( lckey ( "ASCEND#$" , 9 ) ) {
	    if ( nSortOrder > 0 )
		cmdfm.idirection[ nSortOrder - 1 ] = Ascending ;
	    else {
		setmsg ( "WARNING" , 1384 ) ;
		outmsg () ;
		clrmsg () ;
	    }
	}

	/* -- DESCEND */
	else if ( lckey ( "DESCEND#$" , 10 ) ) {
	    if ( nSortOrder > 0 )
		cmdfm.idirection[ nSortOrder - 1 ] = Descending ;
	    else {
		setmsg ( "WARNING" , 1384 ) ;
		outmsg () ;
		clrmsg () ;
	    }
	}

	/* -- header names */
	else if( lcchar( MCPW, ktemp,9, &notUsed ) ){
	    if ( nSortOrder < MAXSORTFIELDS ) {
		strcpy ( kmdfm.ksort[ nSortOrder ] , ktemp ) ;
		cmdfm.idirection[ nSortOrder ] = Ascending ;
		nSortOrder++ ;
	    }
	}


	/* Bad syntax */
	else {
	    cfmt( "ILLEGAL OPTION:",17 );
	    cresp();
	}
    } /* end while */

    /* if nSortOrder is > 0, then sort options were entered.  
	Otherwise, use sort options from the previous sort command. */
    if ( nSortOrder )
	cmdfm.nSortOrder = nSortOrder ;

    /* if no sort options have ever been entered, error out */
    if ( cmdfm.nSortOrder == 0 ) {
	*nerr = 1379 ;
	setmsg ( "ERROR" , *nerr ) ;
	return ;
    }
    
    /* - Commit or rollback data according to cmdfm.icomORroll */
    alignFiles ( nerr ) ;
    if ( *nerr )
	return ;


    /*  EXECUTION PHASE */

    tempIdx = (int *) malloc ( cmdfm.ndfl * sizeof(int) ) ;
    currIdx = (int *) malloc ( cmdfm.ndfl * sizeof(int) ) ;
    realIdx = (int *) malloc ( cmdfm.ndfl * sizeof(int) ) ;
    if ( !currIdx || !currIdx  || !realIdx )
	goto L_MEMORYERROR ;

    memset(tempIdx, 0, cmdfm.ndfl * sizeof(int));
    memset(currIdx, 0, cmdfm.ndfl * sizeof(int));
    memset(realIdx, 0, cmdfm.ndfl * sizeof(int));

    for ( jdx = 0 ; jdx < cmdfm.ndfl ; jdx++ )
	realIdx[ jdx ] = jdx ;

    for ( jsort = cmdfm.nSortOrder - 1 ; jsort >= 0 ; jsort-- ) {
      /* get info for current sort */
      hdrfld ( kmdfm.ksort[ jsort ], 9, &icat, &item, &lok ) ;

      if ( !lok || icat > cmlhf.icatk ) {
	    *nerr = 1381 ;
	    setmsg ( "ERROR" , *nerr ) ;
	    apcmsg ( kmdfm.ksort[ jsort ] , strlen ( kmdfm.ksort[ jsort ] ) ) ;
        goto ERROR;
      }

	if ( icat == cmlhf.icatf ) {
	    fdata = ( float * ) malloc ( cmdfm.ndfl * sizeof ( float ) ) ;
	    if ( !fdata )
		goto L_MEMORYERROR ;
	    for ( jdfl = 0 ; jdfl < cmdfm.ndfl ; jdfl++ ) {
		getfil ( realIdx[jdfl]+1 , FALSE , &notUsed, &notUsed1, &notUsed2 , nerr ) ;
		fdata[ jdfl ] = Fhdr[ item ] ;
	    }
	    Index ((void*)fdata, cmdfm.ndfl, sizeof(float),
		    cmdfm.idirection[ jsort ] == Ascending ?
		    LessThanFloat : GreaterThanFloat, currIdx );
	    free ( fdata ) ;
	    fdata = NULL ;
	}
	else if ( icat == cmlhf.icatn || icat == cmlhf.icatl ) {
	    ldata = ( int * ) malloc ( cmdfm.ndfl * sizeof ( int ) ) ;
	    if ( !ldata )
		goto L_MEMORYERROR ;
	    for ( jdfl = 0 ; jdfl < cmdfm.ndfl ; jdfl++ ) {
		getfil ( realIdx[jdfl]+1 , FALSE , &notUsed, &notUsed1, &notUsed2 , nerr ) ;
		ldata[ jdfl ] = icat == cmlhf.icatn ? Nhdr[ item ] : Lhdr[ item ] ;
	    }
	    Index ((void*)ldata, cmdfm.ndfl, sizeof(int),
                    cmdfm.idirection[ jsort ] == Ascending ?
		    LessThanLong : GreaterThanLong, currIdx );
	    free ( ldata ) ;
	    ldata = NULL ;
	}
	else if ( icat == cmlhf.icati || icat == cmlhf.icatk ) {
	    kdata = (char **) calloc ( cmdfm.ndfl , sizeof ( char * ) ) ;
	    if ( !kdata ) 
		goto L_MEMORYERROR ;

	    for ( jdfl = 0 ; jdfl < cmdfm.ndfl ; jdfl++ ) {
		getfil ( realIdx[jdfl]+1 , FALSE , &notUsed, &notUsed1, &notUsed2 , nerr ) ;

		kdata[ jdfl ] = (char *) malloc ( 34 * sizeof ( char ) ) ;
		if ( !kdata[ jdfl ] )
		    goto L_MEMORYERROR ;

		if ( icat == cmlhf.icati )
		    strcpy ( kdata[jdfl] , kmlhf.kdiv[ Ihdr[ item ] - 1 ] ) ;
		else {
		    if ( item == 1 )
			strcpy ( kdata[jdfl] , kmhdr.khdr[ 0 ] ) ;
		    else if ( item == 2 ) {	/* kevnm has 17 chars instead of 8 */
			strncpy ( kdata[jdfl] , kmhdr.khdr[ 1 ] , 9 ) ;
			strcpy ( kdata[jdfl] + 9 , kmhdr.khdr[ 2 ] ) ;
		    }
		    else
			strcpy ( kdata[jdfl] , kmhdr.khdr[ item - 1 ] ) ;
		}
	    }
	    Index ((void*)kdata, cmdfm.ndfl, sizeof(char*),
                    cmdfm.idirection[ jsort ] == Ascending ?
		    LessThanStrg : GreaterThanStrg, currIdx );
	    for ( jdfl = 0 ; jdfl < cmdfm.ndfl ; jdfl++ )
		free ( kdata [ jdfl ] ) ;

	    free ( kdata ) ;
	    kdata = NULL ;
	} /* end else */

	/* now rearrange realIdx according to currIdx, using tempIdx as a middleIdx */
	for ( jdx = 0 ; jdx < cmdfm.ndfl ; jdx++ ) {
	    tempIdx[ jdx ] = realIdx[ currIdx[ jdx ] ] ;
	}
	for ( jdx = 0 ; jdx < cmdfm.ndfl ; jdx++ )
	    realIdx [ jdx ] = tempIdx[ jdx ] ;
    } /* end for ( jsort ) */
/**********************************************************************/
    tree = smGetDefaultTree () ;
    if(tree) {
      /* Collect Garbage */
      gcCollect ( tree ) ;

      /* pass index to SeisMgr in a call to re-order the files. */
      if ( !dblSortWfdiscList( tree, realIdx , cmdfm.ndfl ) ) {
	/* error handling */
	*nerr = 1383 ;
	setmsg ( "ERROR" , *nerr ) ;
	outmsg () ;
	clrmsg () ;
      }
    } else {
      /* Added 0710004 to reorder without SeisMgr database
   	       Ndxhdr[id]      - Index of header in sacmem 
               ndxdta[id-1][0] - index of first data component 
               ndxdta[id-1][1] - index of second data component 
               Nlndta[id]      - length of data 
               Ncomp[id]       - Number of components
	       Nstart[id]      - Starting point
	       Nstop[id]       - Ending point 
	       Nfillb[id]      - points to fill before
	       Nfille[id]      - points to fill before
	       Ntotal[id]      - total point in the fiel
	       Nxsdd[id]       - ?
	       Ndsndx[id]      - ?
      */
      reorder (Ndxhdr,       cmdfm.ndfl, 1, currIdx, tempIdx);
      reorder (Nlndta,       cmdfm.ndfl, 1, currIdx, tempIdx);
      reorder (Ncomp,        cmdfm.ndfl, 1, currIdx, tempIdx); 
      reorder (Nstart,       cmdfm.ndfl, 1, currIdx, tempIdx); 
      reorder (Nstop,        cmdfm.ndfl, 1, currIdx, tempIdx); 
      reorder (Nfillb,       cmdfm.ndfl, 1, currIdx, tempIdx); 
      reorder (Nfille,       cmdfm.ndfl, 1, currIdx, tempIdx); 
      reorder (Ntotal,       cmdfm.ndfl, 1, currIdx, tempIdx); 
      reorder (Nxsdd,        cmdfm.ndfl, 1, currIdx, tempIdx); 
      reorder (Ndsndx,       cmdfm.ndfl, 1, currIdx, tempIdx); 
      
      /* Reorder the first data component, indicies start at 0 */
      for(jdx = 0; jdx < cmdfm.ndfl; jdx++) {	tempIdx[ jdx ] = cmdfm.ndxdta[currIdx[jdx]][0];      } 
      for(jdx = 0; jdx < cmdfm.ndfl; jdx++) {	cmdfm.ndxdta[ jdx ][0] = tempIdx[jdx];      } 
      /* Reorder the second data component, indicies start at 0 */
      for(jdx = 0; jdx < cmdfm.ndfl; jdx++) {   tempIdx[ jdx ] = cmdfm.ndxdta[currIdx[jdx]][1];   } 
      for(jdx = 0; jdx < cmdfm.ndfl; jdx++) {   cmdfm.ndxdta[ jdx ][1] = tempIdx[jdx];      } 
    }
    /* Produce a sorted list of file names. */

    for(i = 0; i < cmdfm.ndfl; i++) {
        char *tmp = string_list_get(datafiles, currIdx[i]);
        string_list_put(datafiles, tmp, strlen(tmp));
    }
    for(i = 0; i < cmdfm.ndfl; i++) {
        string_list_delete(datafiles, 0);
    }

    /* free memory. */
    free ( knewnames ) ;
    free ( tempIdx ) ;
    free ( currIdx ) ;
    free ( realIdx ) ;

    if(tree) {
      SeisMgrToSac ( tree , FALSE , nerr, Verbose , TRUE, TRUE ) ;
    }

    return ;

L_MEMORYERROR:

    if ( kdata ) {
	for ( jdfl = 0 ; jdfl < cmdfm.ndfl ; jdfl++ ) {
	    if ( kdata[ jdfl ] )
		free ( kdata[ jdfl ] ) ;
	}
	free ( kdata ) ;
    }

    *nerr = 301 ;
    setmsg ( "ERROR" , *nerr ) ;

 ERROR:
    outmsg () ;
    clrmsg () ;

    if ( tempIdx ) free ( tempIdx ) ;
    if ( currIdx ) free ( currIdx ) ;
    if ( realIdx ) free ( realIdx ) ;

} /* end xsort() */

/** 
 * Reorder an array \p v of length \p n based on the indicies \p c using 
 *    a temporary array \p t. 
 *    First the array \p t is set to what the new array should be by 
 *    t = v [ c ] where c is the sorted indicies.
 *    Then the array in \p t is placed back onto \p v 
 *    The shift \p s is used to account for a Fortran Shift, normally 1.
 * 
 * @param v
 *    The array to reorder
 * @param n 
 *    Length of arrays \p v , \p c , and \p t
 * @param s
 *    Amount to shift the arrays by
 * @param c 
 *    Indicies of new array
 * @param t
 *    Temporary array to hold old version while shifting entries
 *
 * @return Nothing
 *
 * @date 070606: Documented/Reviewed
 *
 */
static void
reorder(int *v, int n, int s, int *c, int *t) {
  int i;
  for(i = 0; i < n; i++) {    t[i]       = v[ c[i] + s ];  }
  for(i = 0; i < n; i++) {    v[ i + s ] = t[ i ];         }
}

