/** 
 * @file   map_chdr_in.c
 * 
 * @brief  Convert Fortran strings to C
 * 
 */

#include <stdio.h>
#include <string.h>

#include "dff.h"
#include "hdr.h"
/** 
 * Convert Fortran strings to C.  This is used primarilly for mapping
 *    character string header variables from disk format into memory
 *    format.  It only adds null termination to the strings.
 * 
 * @param memarray 
 *    Output
 * @param buffer 
 *    Input
 *
 * @bug Only used in dff/rsach() and should not be necessary if 
 *       character strings were read in correctly.
 *
 */
void 
map_chdr_in(float *memarray,
	    float *buffer) {

  char *ptr1, *ptr2;
  int i;

  ptr1 = (char *)memarray;
  ptr2 = (char *)buffer;

  memcpy(ptr1,ptr2,8); *(ptr1+8) = '\0';
  ptr1 += 9;  ptr2 += 8;

  memcpy(ptr1,ptr2,16);*(ptr1+16) = '\0';
  ptr1 += 18; ptr2 += 16;

  for (i=0; i<21; i++){
    memcpy(ptr1,ptr2,8);
    *(ptr1+8) = '\0';
    ptr1 += 9;
    ptr2 += 8;
  }

  return;

}
