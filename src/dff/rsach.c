/** 
 * @file   rsach.c
 * 
 * @brief  Read Sac Header
 * 
 */

#include "dff.h"
#include "hdr.h"
#include "bool.h"
#include "co.h"
#include "msg.h"
#include "ucf.h"

#include "errors.h"

/** 
 * Initialize the Common block for sacio
 * 
 */
void
sacio_initialize_common() {
  if( cmhdr.fundef != SAC_FLOAT_UNDEFINED ) {
    inihdr();
    inilhf();
    inimsg();
  }
}

/** 
 * Check the Sac Header Version
 * 
 * @param hdr 
 *   Sac Header to Check
 * @param nerr 
 *   Error return Flag
 *   - SAC_OK
 *   - SAC_ERROR_NOT_A_SAC_FILE
 * 
 * @return 
 *    - TRUE if file needs to be swapped
 *    - FALSE if file does not need to be swapped
 *    - -1 if there was an error checking the header
 *
 * @date July 01, 2007 Initial Version -- B. Savage
 */
int
sac_check_header_version(float *hdr, 
			 int   *nerr) {
  int lswap;
  int *ver;

  lswap = FALSE;
  *nerr = SAC_OK;
  /* determine if the data needs to be swapped. */
  ver = (int *)( hdr + SAC_VERSION_LOCATION ) ;
  if( *ver < 1 || *ver > SAC_HEADER_MAJOR_VERSION ){
    byteswap( (void *)ver, SAC_HEADER_SIZEOF_NUMBER ) ;

    if( *ver < 1 || *ver > SAC_HEADER_MAJOR_VERSION ) {
      *nerr = ERROR_NOT_A_SAC_FILE ;
      setmsg( "ERROR", *nerr ) ;
      aplmsg( "not in sac format, nor byteswapped sac format.", 62 );
      outmsg();
      clrmsg();
      return -1;
    }  else {
      /* swap back, so it can be */
      byteswap( (void *)ver, SAC_HEADER_SIZEOF_NUMBER );
      lswap = TRUE;
    }
  }
  return lswap;
}

/** 
 * Swap the Sac Header
 * 
 * @param hdr 
 *    Sac Header to Swap, packed
 *
 * @bug Unexpected and compiler packing of the structure
 *    will cause problems swapping the header, each value
 *    should be swapped individually, it is the safest way
 *
 * @date July 01, 2007 Initial Version -- B. Savage
 */
void
sac_header_swap(float *hdr) {
  int i;
  float *ptr;
  for( i = 0, ptr = hdr ; i < SAC_HEADER_NUMBERS ; i++, ptr++ ){
    byteswap( (void*)ptr, SAC_HEADER_SIZEOF_NUMBER ) ;
  }
}

/** 
 * Read a Sac Header
 * 
 * @param nun 
 *    Logical file unit to read Sac Header From
 * @param nerr 
 *    Error return Flag
 *    - SAC_OK
 *    - Non-Zero on Error
 * 
 * @bug Sac Header Character decoding
 *      This is dodgy at best
 *      24 = Character Strings
 *      9  = Character String Length in Memory (includes terminator)
 *      8  = Character String Length in File (no terminator)
 *      4  = Size of Float
 *      6*4 = 24 Terminators, UGH!
 *      Sac Header in the File
 *             8 * 24 = 192 bytes  (Actual String)
 *         4 * 2 * 24 = 192 bytes  (Size in Floats)
 *      Sac Header in Memory
 *             9 * 24 = 216 bytes  (Actual String)
 *   4 * (2 * 24 + 6) = 216 bytes  (Size in Floats)
 *
 * @return 
 *    - TRUE if file needs to be swapped
 *    - FALSE if file does not need to be swapped
 *
 * @bug This routine should be reading in the character strings as 
 *      characters and null terminating them correctly and not 
 *      shifting the data through a float using co/zrabs().
 *
 * @date July 01, 2007 Initial Version -- B. Savage
 */
int
sac_header_read(int nun, int *nerr) {
  int lswap;
  float temp2[2 * SAC_HEADER_STRINGS];
  float temp[(2 * SAC_HEADER_STRINGS) + 6];
  int word;
                                           
  lswap = FALSE;
  word = 0;
  zrabs( &nun, (char *)cmhdr.fhdr, SAC_HEADER_NUMBERS, &word, nerr );
  if( *nerr != SAC_OK )
    return lswap;

  lswap = sac_check_header_version(cmhdr.fhdr, nerr);
  
  if( lswap ){     /* byteswap all the non-character header elements. */
    sac_header_swap(cmhdr.fhdr);
  }
  
  word = word + SAC_HEADER_NUMBERS;
  zrabs( &nun, (char *)temp2, 2 * SAC_HEADER_STRINGS, &word, nerr );
  if( *nerr != SAC_OK )
    return lswap;

  map_chdr_in(temp,temp2);
  
  zgetc( (int *) temp, 
	 (char *) kmhdr.khdr, 
	 SAC_HEADER_STRING_LENGTH * SAC_HEADER_STRINGS );
  
  return lswap;
 
}

/** 
 * Read a Sac Header
 * 
 * @param kname 
 *    File to read Sac Header from
 * @param nerr 
 *    Error Return Flag
 *    - SAC_OK
 *    - Non-Zero on Error
 * @param kname_s 
 *    Length of filename \p kname
 *
 * @date July 01, 2007 Initial Version -- B. Savage
 */
void
rsach(char *kname,
       int  *nerr,
      int   kname_s) {
  int ncerr;
  int nun;

  nun = 0;

  *nerr = SAC_OK;

  sacio_initialize_common();
  
  /* - Open the file. */
  zopen_sac( (int *)&nun, kname,kname_s, "RODATA",7, (int *)nerr );
  if( *nerr != SAC_OK )
    goto ERROR;
  
  sac_header_read(nun, nerr);
  if( *nerr != SAC_OK )
    goto ERROR;

 ERROR:
  zclose((int *)&nun, (int *)&ncerr);
  return;
}
