/** 
 * @file   rdxdrdta.c
 * 
 * @brief  Read data components from a XDR format SAC disk file
 * 
 */
#include <stdio.h>

#include "config.h"

#include "dff.h"
#include "amf.h"
#include "hdr.h"
#include "dfm.h"
#include "ucf.h"
#include "co.h"
#include "debug.h"

#ifdef HAVE_LIBRPC 
#include <rpc/rpc.h>

/** 
 * Read data components from an XDR format SAC disk file to memory
 * 
 * @param idfl 
 *    Data file list index number
 * @param kname 
 *    Name of the file
 * @param kname_s 
 *    Length of \p kname
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date 010996:  Original version.
 *
 */
void 
rdxdrdta(int   idfl, 
	 char *kname, 
	 int   kname_s, 
	 int  *nerr) {

	int jcomp, nlcmem ;
        int lendata, ncerr;
	float unused;
        FILE *nun;
        XDR xdrs;


	*nerr = 0;

	/* Open the input file */
	znfiles(&nun, kname, kname_s, "TEXT", 5, nerr);
	if( *nerr != 0 ) return;

	/* Create a stream for the XDR decoding */
	xdrstdio_create(&xdrs, nun, XDR_DECODE);

	/* Read the header from disk */
	/* For portability, read and throw away the header first
	   to correctly position file for read of data.   */
	xdrhdr(xdrs, cmmem.sacmem[Ndxhdr[idfl]], nerr);
	if( *nerr != 0 ) goto L_8888;

	/* - Define number of points to read. */
	lendata = Nlndta[idfl]; 
 
	/* - For each data component: */
	for( jcomp = 0; jcomp < Ncomp[idfl]; jcomp++ ){

	    /* -- Define initial memory location. */
	    nlcmem = cmdfm.ndxdta[idfl - 1][jcomp];

	    if( !xdr_array(&xdrs, (caddr_t *) &cmmem.sacmem[nlcmem],
		(u_int *)&lendata, (u_int)lendata, sizeof(float), xdr_float)){
                  *nerr = 123;
                  goto L_8888;
	    }
	}

	/* - Compute some header values. */

	*npts = Nlndta[idfl];
	extrma( cmmem.sacmem[cmdfm.ndxdta[idfl - 1][0]], 1, *npts, depmin, 
	 depmax, depmen );
	if( *leven ){
	    *ennd = *begin + (float)( *npts - 1 )**delta;
	}
	else{
	    extrma( cmmem.sacmem[cmdfm.ndxdta[idfl - 1][1]], 1, *npts,
		    begin, ennd, &unused );
	}

	/* - Move header back to working memory. */

	putfil( idfl, nerr );

L_8888:
	xdr_destroy( &xdrs );
	zcloses( &nun, &ncerr );

	return;

}

#else 

void 
rdxdrdta(int   idfl, 
	 char *kname, 
	 int   kname_s, 
	 int  *nerr) {
  librpc_not_available();
  UNUSED(idfl);
  UNUSED(kname);
  UNUSED(kname_s);
  UNUSED(nerr);
}
#endif /* HAVE_LIBRPC */
