/** 
 * @file   vfmax.c
 * 
 * @brief  Get the maximum number of points
 * 
 */

#include "dfm.h"
#include "bool.h"
#include "hdr.h"

#include "amf.h"
#include "dff.h"
#include "errors.h"
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

int jdfl;
  sac *s;
  *nerr = 0;
  *maxf = 0;
  
  for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
    if(!(s = sacget(jdfl-1, FALSE, nerr))) {
      goto L_8888;
    }
    /* -- Get header from memory manager. */
    //getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );
    
    /* -- See if this files number exceeds maximum found so far. */
    if( s->h->npts > *maxf ) {
      *maxf = s->h->npts;
    }
  }
  
 L_8888:
  return;
}

