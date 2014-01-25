/** 
 * @file   next2.c
 * 
 * @brief  Get the next power of 2
 * 
 */

#include "ucf.h"

#define  STARTING_POWER_OF_TWO   8

/** 
 * Get the next power of two just greater than \p num
 * 
 * @param num 
 *    Input number 
 * 
 * @return 
 *    Power of two greater than \p num
 *
 * @date    810000:  Original version.
 *
 */
int 
next2(int num) {
  int next2_v;
  
  next2_v = STARTING_POWER_OF_TWO;
    
  while(next2_v < num) {
    next2_v *= 2;
  }
  
  return( next2_v );
}

