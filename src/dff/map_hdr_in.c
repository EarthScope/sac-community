/** 
 * @file   map_hdr_in.c
 *
 * @brief  Map a SAC file to memory
 * 
 */

#include <string.h>

#include "dff.h"
#include "ucf.h"
#include "hdr.h"

/** 
 *  This routine maps the SAC file header into memory from
 *    a buffer containing it in disk file format.  
 *    In the disk file (deriving from the FORTRAN version of
 *    SAC) these strings are essentially concatenated without
 *    null termination.  In memory, they are carried about as
 *    null terminated strings.  This routine picks out the 
 *    strings and null terminates them, storing them in the
 *    in memory working storage area.
 * 
 * @param memarray 
 *    Output
 * @param buffer 
 *    Input
 * @param lswap 
 *    Swap the buffer if necessary
 *
 */
void 
map_hdr_in(float *memarray,
	   float *buffer,
	   int    lswap) {

  char *ptr1, *ptr2;
  int idx;

  /* First get the header values for the non character   
   * fields fhdr, nhdr, ihdr and lhdr.  These are copied 
   * straight across.                                    
   */
  ptr1 = (char *)memarray;
  ptr2 = (char *)buffer;

  memcpy(ptr1,ptr2, SAC_HEADER_NUMBERS * sizeof(float));

  /* byteswap numeric data if necessary. */
  if( lswap ){
    for( idx = 0 ; idx < SAC_HEADER_NUMBERS ; idx++, ptr1 += SAC_HEADER_SIZEOF_NUMBER )
      byteswap( (void *)ptr1, SAC_HEADER_SIZEOF_NUMBER ) ;
  }
  
  /* Now copy the character variables into the memory    
   * buffer, supplying the additional null termination   
   * character.                                          
   */
  map_chdr_in(memarray + SAC_HEADER_NUMBERS, 
              buffer + SAC_HEADER_NUMBERS);

  return;

}
