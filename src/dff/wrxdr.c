/** 
 * @file   wrxdr.c
 * 
 * @brief  Write a SAC file in XDR format
 * 
 */

#include <stdio.h>

#include "config.h"

#include "dff.h"
#include "dfm.h"
#include "co.h"
#include "amf.h"
#include "errors.h"
#include "debug.h"

#ifdef HAVE_LIBRPC 
#include <rpc/rpc.h>


/** 
 * Write a SAC data file from memory to disk is XDR (portable) format
 * 
 * @param idfl 
 *    Data file list index number
 * @param kname 
 *    Name of file to write
 * @param kname_s 
 *    Length of \p kname
 * @param ldta 
 *    - TRUE to write the data and header
 *    - FALSE to write only the header, not data
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_WRITING_XDR_FILE
 *    - ERROR_ENCODING_XDR_FILE
 *
 * @date   010496:  Original version.
 *
 */
void 
wrxdr(int   idfl, 
      char *kname, 
      int   kname_s, 
      int   ldta, 
      int  *nerr) {

	int jcomp, ncerr, nlcmem, nptwr; 
        FILE *nun;
        XDR xdrs;

	*nerr = 0;

        if( !ldta ){
          *nerr = ERROR_WRITING_XDR_FILE;
          return;
	}

	/* create a file */
	znfiles(&nun, kname, kname_s, "TEXT", 5, nerr);
	if( *nerr != 0 )
	    return;

	/* create a stream for the XDR conversions */
	xdrstdio_create(&xdrs, nun, XDR_ENCODE);

	/* - Write the header to disk. */
	nlcmem = Ndxhdr[idfl];

	xdrhdr(xdrs, cmmem.sacmem[nlcmem],nerr);
	if( *nerr != 0 )
	    goto L_8888;

	/* - Write each data component, if requested. */
	if( ldta ){
	    for( jcomp = 0; jcomp < Ncomp[idfl]; jcomp++ ){
		nlcmem = cmdfm.ndxdta[idfl - 1][jcomp];
		nptwr = Nlndta[idfl];

		if( !xdr_array(&xdrs, (caddr_t *)&cmmem.sacmem[nlcmem],
		    (u_int *)&nptwr, (u_int)nptwr, sizeof(float), xdr_float)){
		    *nerr = ERROR_ENCODING_XDR_FILE;
		    goto L_8888;
		}
	    }
	}

/* - Close disk file. */

L_8888:
        xdr_destroy(&xdrs);
	zcloses( &nun, &ncerr );

	return;

} 

#else 

void 
wrxdr(int   idfl, 
      char *kname, 
      int   kname_s, 
      int   ldta, 
      int  *nerr) {
  librpc_not_available();
  UNUSED(idfl);
  UNUSED(kname);
  UNUSED(kname_s);
  UNUSED(ldta);
  UNUSED(nerr);
}

void
librpc_not_available() {
  fprintf(stderr, 
          "XDR file format read/write feature not compiled into sac\n"
          "    please re-configure with --with-librpc and recompile\n");
}

#endif /* HAVE_LIBRPC */
