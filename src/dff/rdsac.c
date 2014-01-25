/** 
 * @file   rdsac.c
 * 
 * @brief  Read a SAC file
 * 
 */

#include "dff.h"
#include "amf.h"
#include "hdr.h"
#include "co.h"
#include "bool.h"
#include "clf.h"
#include "dfm.h"

/** 
 * Read a SAC file into memory
 * 
 * @param idfl 
 *    Data file list index number.  Used to store information about 
 *    the location of header and data components after read
 * @param kname 
 *    Name of the data file to read
 * @param kname_s 
 *    Length of \p kname
 * @param lname 
 *    - TRUE if the name is appended to a list of filenames
 *    - FALSE if to not append
 * @param ldta 
 *    - TRUE read the header and data
 *    - FALSE read the header only
 * @param nlen 
 *    Number of data points read
 * @param ndxh 
 *    Index in sacmem array of header
 * @param ndx1 
 *    Index in sacmem array of first data component
 * @param ndx2 
 *    Index in sacmem array of second data component
 *    Set to 0 if second component does not exist
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   980922:  Added lname to discriminate between filenames which
 *                  should or should not be added to the list.  maf.
 * @date   880913:  Fixed bug in defining data file list.
 * @date   850617:  Major rewrite due to addition of memory manager.
 *                  CHANGED NUMBER AND ORDER OF ARGUMENTS.
 * @date   821004:  Fixed bug which was incorrectly clearing error flag.
 * @date   810120:  Changed to output message retrieval from disk.
 */
void 
rdsac(int    idfl, 
      char  *kname, 
      int    kname_s, 
      int    lname, 
      int    ldta, 
      int   *nlen, 
      int   *ndxh, 
      int   *ndx1, 
      int   *ndx2, 
      int   *nerr) {

	int jcomp, ncerr, nun, lswap = 0 ;

	*nerr = 0;

	/* - Open file. */
	zopen_sac( &nun, kname,kname_s, "RODATA",7, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Allocate a memory block for header. */
	allamb( &cmmem, SAC_HEADER_WORDS, ndxh, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Save some parameters about this data file. */
	cmdfm.idflc = idfl;
	Ndxhdr[cmdfm.idflc] = *ndxh;
	if ( lname ) {
        string_list_put(datafiles, kname, kname_s);
    }
	if( *nerr != 0 )
	    goto L_8888;

	/* - Read header record. */
	lswap = rdhdr( idfl, &nun, string_list_get(datafiles, -1), nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Read data if requested. */
	if( ldta ){

	    /* -- Determine memory requirements for this file. */
	    defmem( idfl, TRUE, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Allocate memory block(s) for data. */
	    for( jcomp = 0; jcomp < Ncomp[idfl]; jcomp++ ){
		allamb( &cmmem, Nlndta[idfl], 
			&cmdfm.ndxdta[idfl - 1][jcomp], nerr );
		if( *nerr != 0 )
		    goto L_8888;
	    }

	    /* -- Read data components. */
	    rddta( idfl, &nun, lswap, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Return indicies to data components. */
	    *nlen = Nlndta[idfl];
	    *ndx1 = cmdfm.ndxdta[idfl - 1][0];
	    if( Ncomp[idfl] > 1 )
		*ndx2 = cmdfm.ndxdta[idfl - 1][1];
	    else
		*ndx2 = 0;
	}
	else{
	    *nlen = 0;
	    *ndx1 = 0;
	    *ndx2 = 0;
	}

	/* - Close file and return. */
L_8888:
	zclose( &nun, &ncerr );
	return;
}

