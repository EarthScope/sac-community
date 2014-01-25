/** 
 * @file   map_chdr_out.c
 * 
 * @brief  Convert C string to Fortran
 * 
 * 
 */

#include <string.h>

#include "dff.h"

/** 
 * Convert C strings to Fortran.  This is used primarilly for mapping
 *    character string header variables from memroy format into memory
 *    format.  It only removes null termination to the strings.
 * 
 * @param memarray 
 *    Output
 * @param buffer 
 *    Input
 */
void 
map_chdr_out(float *memarray,
	     float *buffer) {

  char *ptr1, *ptr2;
  int i;

  ptr1 = (char *)memarray;
  ptr2 = (char *)buffer;

  memcpy(ptr2,ptr1,8);
  ptr1 += 9;  ptr2 += 8;

  memcpy(ptr2,ptr1,16);
  ptr1 += 18; ptr2 += 16;

  for (i=0; i<21; i++){
    memcpy(ptr2,ptr1,8);
    ptr1 += 9;
    ptr2 += 8;
  }

  return;

}
