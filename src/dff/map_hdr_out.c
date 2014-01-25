/** 
 * @file   map_hdr_out.c
 * 
 * @brief  Map SAC memory to a file
 * 
 */

#include <string.h>

#include "dff.h"
#include "ucf.h"
#include "hdr.h"
#include "bool.h"

/** 
 * This routine maps a in memory SAC file header to a file.
 * 
 * @param memarray 
 *    Input
 * @param buffer 
 *    Output
 * @param lswap 
 *    If the header needs to be swappeed
 *
 */
void 
map_hdr_out(float *memarray,
	    float *buffer, 
	    int    lswap) {

  char *ptr1, *ptr2;
  int i;
  /* First get the header values for the non character   
   * fields fhdr, nhdr, ihdr and lhdr.  These are copied 
   * straight across.
   */
  
  ptr1 = (char *)memarray;
  ptr2 = (char *)buffer;

  memcpy(ptr2,ptr1, SAC_HEADER_NUMBERS * sizeof(float));

  if(lswap == TRUE) {
    for(i = 0; i < SAC_HEADER_NUMBERS; i++, ptr2 += SAC_HEADER_SIZEOF_NUMBER) {
      byteswap((void *) ptr2, SAC_HEADER_SIZEOF_NUMBER);
    }
  }
  
  /* Now copy the character variables into the output    
   * buffer, eliminating the null termination            
   * character.
   */

  map_chdr_out(memarray + SAC_HEADER_NUMBERS,
               buffer + SAC_HEADER_NUMBERS);

  return;

}
