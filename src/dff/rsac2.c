/** 
 * @file   rsac2.c
 * 
 * @brief  Read an unevely spaced or spectral SAC file
 * 
 */

#include "dff.h"
#include "bool.h"
#include "msg.h"
#include "co.h"
#include "hdr.h"

#include "errors.h"

/** 
 * Read and unevenly spaced or spectral SAC file
 * 
 * @param kname 
 *    Name of disk file to read, should b blank filled
 * @param yarray 
 *    Output array of the dependent variable from the disk file
 * @param nlen 
 *    Number of data points to read, should be less than or equal to \p max_
 * @param xarray 
 *    Output array of the independent variable from the disk file
 * @param max_ 
 *    Length of \p yarray and \p xarray
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *    - 802 If the file is evenly spaced
 *    - 803 If the number of points in the file is greater than \p max_
 *          In this case the first \p max_ points are read
 *    - 1317 File is not a SAC file
 * @param kname_s 
 *    Length of \p kname
 *
 * @note Local Variables
 *   - TEMP:    Array used while reading header character data. [ra]
 *
 * @date   Aug  4 2008 B. Savage <savage_at_uri.edu>
 *                     Changed wrtmsg() to outmsg()
 * @date   Mar 18 2007 B. Savage <savage_at_uri.edu>
 *                     Updated routines for better interpability with C
 *                     and Fortran when used as an external library
 * @date   870902:  Added calls to INILHF and INIMSG as part of initialization.
 * @date   870513:  Changed call to wrtxtd to wrtmsg.
 * @date   830125:  Changes due to modified header common block.
 * @date   810212:  Changed to output message retrieval from disk.
 * @date   800922:  Original version.
 * @date   870902:  Documented/Reviewed
 *
 */
void 
rsac2(char      *kname, 
      float     *yarray, 
      int       *nlen, 
      float     *xarray, 
      int       *max_, 
      int       *nerr, 
      int        kname_s)
{
  int ncerr, nun;
  int lswap;
  int truncated;
  
  float *const Xarray = &xarray[0] - 1;
  
  *nerr     = 0;
  truncated = FALSE;
  nun       = 0;

  /* - Initialize some common blocks if not already done. */
  sacio_initialize_common();
  
  /* - Open the file. */
  zopen_sac( &nun, kname,kname_s, "RODATA",7, nerr );
  if( *nerr != SAC_OK )
    goto ERROR;
  
  lswap = sac_header_read(nun, nerr);
  
  /* - Make sure file is unevenly spaced. */
  
  if( !*leven ){
    if( *npts <= *max_ ){
      *nlen = *npts;
    }
    else{
      *nlen = *max_;
      truncated = TRUE;
    }
  } else {
    *nerr = ERROR_SAC_FILE_NOT_UNEVENLY_SPACED;
    setmsg( "ERROR", *nerr );
    apcmsg( kname,kname_s );
    outmsg();
    clrmsg();
    goto ERROR;
  }
  
  /* - Read in the data. */
  sac_data_read(nun, yarray, *nlen, SAC_FIRST_COMPONENT, lswap, (int *)nerr);
  if( *nerr != SAC_OK )
    goto ERROR;
  
  sac_data_read(nun, xarray, *nlen, SAC_SECOND_COMPONENT, lswap, (int *)nerr);
  if( *nerr != SAC_OK )
    goto ERROR;
  
  /* - Adjust several header fields. */
  *npts = *nlen;
  *ennd = Xarray[*npts];
  
 ERROR:
  *nerr = ( *nerr == SAC_OK && truncated == TRUE) ? -ERROR_SAC_DATA_TRUNCATED_ON_READ : *nerr;
  
  zclose( &nun, &ncerr );
  return;
}


void 
rsac2_ (char      *kname, 
	float     *yarray,
	int       *nlen, 
	float     *xarray, 
	int       *max_, 
	int       *nerr, 
	int        kname_s) {
  rsac2 ( kname , yarray , nlen , xarray , max_ , nerr , kname_s ) ;
}


void 
rsac2__ (char      *kname, 
	 float     *yarray, 
	 int       *nlen, 
	 float     *xarray, 
	 int       *max_, 
	 int       *nerr, 
	 int        kname_s) {
  rsac2 ( kname , yarray , nlen , xarray , max_ , nerr , kname_s ) ;
}
