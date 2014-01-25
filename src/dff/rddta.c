/** 
 * @file   rddta.c
 * 
 * @brief  Read SAC data components
 * 
 */

#include "dff.h"
#include "amf.h"
#include "hdr.h"
#include "dfm.h"
#include "ucf.h"
#include "co.h"

/** 
 * Read data components frm a SAC disk file to memory
 * 
 * @param idfl 
 *    Data file list index number
 * @param nun 
 *    Fortran file unit on which data file is open
 * @param lswap 
 *    To byte swap the incoming data
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   870515:  Fixed bug involving zero fill option.
 * @date   850415:  Changes due to restructuring of DFM common block.
 * @date   811202:  Added calculation of BEGIN and ENND for uneven data.
 * @date   811120:  Added calculation of ENND.
 * @date   810423:  Deleted option to convert format of spectral files
 *             as they are read into memory.
 * @date   810416:  Replaced CMWORK with local storage.
 * @date   810120:  Changed to output message retrieval from disk.
 *
 */
void 
rddta(int   idfl, 
      int  *nun, 
      int   lswap, 
      int  *nerr) {

	int jcomp, nlcdsk, nlcmem, numrd, offset;
	float unused;

	*nerr = 0;

	/* - Define number of points to read and initial disk location. */
	numrd = Nstop[idfl] - Nstart[idfl] + 1 - Nfillb[idfl] - Nfille[idfl];
        nlcdsk = SAC_HEADER_WORDS_FILE;

	/* - For each data component: */
	for( jcomp = 0; jcomp < Ncomp[idfl]; jcomp++ ){
            offset = 0;

	    /* -- Define initial memory location. */
	    nlcmem = cmdfm.ndxdta[idfl - 1][jcomp];

	    /* -- Fill beginning with zeros if requested.  
	     *    Update memory location. */
	    if( Nfillb[idfl] > 0 ){
		fill( cmmem.sacmem[nlcmem], Nfillb[idfl], 0. );
		offset += Nfillb[idfl];
	    }

	    /* -- Update disk location and read data. */
	    if( numrd > 0 ){
		nlcdsk = nlcdsk + Nstart[idfl] - 1 + Nfillb[idfl];
		zrabs( (int *)nun, (char *)(cmmem.sacmem[nlcmem]+offset), 
		       numrd, (int *)&nlcdsk, (int *)nerr );
                if( lswap ){     /* byteswap if necessary. */
                    int idx ;
                    float *ptr ;

                    for( idx = 0, ptr = cmmem.sacmem[nlcmem]+offset ;
                         idx < numrd ; idx++, ptr++ ) {
                        byteswap( (void *)ptr, 4 ) ;
                    }
                } 
		if( *nerr != 0 )
		    goto L_8888;
		offset += numrd;
	    }

	    /* -- Fill end with zeros if requested. */
	    if( Nfille[idfl] > 0 ){
		fill( cmmem.sacmem[nlcmem]+offset, Nfille[idfl], 0. );
	    }

	    /* -- Update disk location to point to start of next component. */
	    nlcdsk = nlcdsk + Ntotal[idfl] - Nstart[idfl] + 1;
	} /* end for ( jcomp ) */

	/* - Compute some header values. */

	*npts = Nlndta[idfl];
	extrma( cmmem.sacmem[cmdfm.ndxdta[idfl - 1][0]], 1, *npts, depmin, 
	 depmax, depmen );
	if( *leven )
	    *ennd = *begin + (float)( *npts - 1 )**delta;
	else{
	    extrma( cmmem.sacmem[cmdfm.ndxdta[idfl - 1][1]], 1, *npts, begin, 
	     ennd, &unused );
	}

	/* - Move header back to working memory. */
	putfil( idfl, nerr );

L_8888:
	return;
}

