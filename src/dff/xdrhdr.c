/** 
 * @file   xdrhdr.c
 * 
 * @brief  Write a SAC header in XDR format
 * 
 */

#include "config.h"

#ifdef HAVE_LIBRPC 
#include "dff.h"
#include "hdr.h"

#include "errors.h"

/** 
 * Write a SAC header from memory to disk in XDR (portable) format
 * 
 * @param xdrs 
 *    XDR file
 * @param headerbuf 
 *    Pointer to a buffer containing a SAC header
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @bug This routine uses a xdr_long value to write to a file
 *       What is the size of xdr_long ?
 *
 * @date   010496:  Original version.
 *
 */
void 
xdrhdr(XDR    xdrs, 
       float *headerbuf, 
       int   *nerr) {

        int nfloat, nlong, nbytes;
        char *cbuf;
        int *lbuf;

	*nerr = 0;

        if( !xdr_setpos(&xdrs, 0)){
          *nerr = ERROR_ENCODING_XDR_FILE;
          goto L_8888;
	}

	/* Read/Write the floating point header block */
        nfloat = SAC_HEADER_FLOATS;
        if( !xdr_array(&xdrs, (caddr_t *)&headerbuf, (u_int *)&nfloat,
                       (u_int)nfloat, sizeof(float), xdr_float)){
          *nerr = ERROR_ENCODING_XDR_FILE;
          goto L_8888;
	}

	/* Read/Write the long header vars (nhdr, ihdr and lhdr) */
        nlong = SAC_HEADER_INTEGERS + SAC_HEADER_ENUMS + SAC_HEADER_LOGICALS;
        lbuf = (int *)(headerbuf+nfloat);
        if( !xdr_array(&xdrs, (caddr_t *)&lbuf, (u_int *)&nlong,
                       (u_int)nlong, sizeof(int), xdr_int)){
          *nerr = ERROR_ENCODING_XDR_FILE;
          goto L_8888;
	}

	/* Read/Write the character header fields */
        nbytes = SAC_HEADER_STRINGS * 9;
        cbuf = (char *)(headerbuf+nfloat+nlong);
        if( !xdr_bytes(&xdrs, &cbuf, (u_int *)&nbytes, (u_int)nbytes)){
          *nerr = ERROR_ENCODING_XDR_FILE;
          goto L_8888;
        } 

L_8888:
	return;
}

#else 

void xdrhdr_no_librpc() { 
  
}

#endif 


