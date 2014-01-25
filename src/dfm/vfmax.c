/** 
 * @file   vfmax.c
 * 
 * @brief  Get the maximum number of points
 * 
 */

#include "dfm.h"
#include "bool.h"
#include "hdr.h"


#include "dff.h"

/** 
 * Maximum number of data point found is returned
 * 
 * @param maxf 
 *    Maximum number of data points found in the files
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   820622:  Original version.
 *
 */
void 
vfmax(int *maxf, 
      int *nerr) {

  int jdfl, ndx1, ndx2, nlen;
  
  *nerr = 0;
  *maxf = 0;
  
  for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
    
    /* -- Get header from memory manager. */
    getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );
    if( *nerr != 0 )
      goto L_8888;
    
    /* -- See if this files number exceeds maximum found so far. */
    if( *npts > *maxf ) {
      *maxf = *npts;
    }
  }
  
 L_8888:
  return;
}

