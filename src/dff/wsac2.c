/** 
 * @file   wsac2.c
 * 
 * @brief  Write a SAC file
 * 
 */

#include "dff.h"
#include "hdr.h"
#include "bool.h"
#include "msg.h"
#include "errors.h"

/** 
 * Write an unevenly spaced or spectral SAC file
 * 
 * @param kname 
 *    Name of file to write
 * @param yarray 
 *    Array containing the dependent variable, e.g. Amplitude
 * @param nlen 
 *    Length of \p yarray and \p xarray
 * @param xarray 
 *    Array containing the independent variable, e.g. Time
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 * @param kname_s 
 *    Length of \p kname
 *
 * @date   870902:  Added calls to INILHF and INIMSG as part of initialization.
 * @date   870513:  Changed call to wrtxtd to wrtmsg.
 * @date   800820:  Original version.
 *
 */
void 
wsac2(char  *kname, 
      float *yarray, 
      int   *nlen, 
      float *xarray, 
      int   *nerr, 
      int    kname_s) {

	float *const Xarray = &xarray[0] - 1;

	*nerr = 0;

	/* - Initialize some common blocks if not already done. */
	if( cmhdr.fundef != -12345. ){
	    inihdr();
	    inilhf();
	    inimsg();
	}

	/* - Initialize all header fields to their default values. */
	newhdr();

    if(*nlen <= 0) {
        *nerr = ERROR_WRITING_FILE;
        return;
    }
	/* - Set up the header fields passed by the calling program. */
	*npts = *nlen;
	*begin = Xarray[1];
	*ennd = Xarray[*nlen];
	*leven = FALSE;

	/* - Write the file to disk. */
	wsac0( kname, xarray, yarray, nerr, kname_s );

	if( *nerr != 0 )
	    outmsg();
	return;

}




/* Wrapper to make the function more convenient for FORTRAN programmers. */

void wsac2_ (char      *kname, 
	     float     *yarray, 
	     int       *nlen, 
	     float     *xarray, 
	     int       *nerr, 
	     int        kname_s) {
  wsac2 ( kname , yarray , nlen , xarray , nerr , kname_s ) ;
}
void wsac2__ (char      *kname, 
	      float     *yarray, 
	      int       *nlen, 
	      float     *xarray, 
	      int       *nerr, 
	      int        kname_s) {
  wsac2 ( kname , yarray , nlen , xarray , nerr , kname_s ) ;
}
