/** 
 * @file   zero.c
 * 
 * @brief  Zero a sequence
 * 
 */

#include "dbh.h"

/** 
 * Multiply a sequence by 0.0
 * 
 * @param a 
 *    Seqence 
 * @param n 
 *    Length of \a 
 */
void 
zero(float *a, 
     int    n) {

  int k ;
  
  for( k = 0 ; k < n ; k++ )
    a[ k ] = 0.0 ;
}

