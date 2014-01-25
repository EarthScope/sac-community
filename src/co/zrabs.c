/** 
 * @file   zrabs.c
 * 
 * @brief  Read a data file
 * 
 */

#include <unistd.h>
#include <sys/types.h>  
#include <sys/stat.h>                                                         
#include <fcntl.h>     

#include "co.h"
#include "msg.h"

#include "errors.h"

/** 
 * Read a data file
 * 
 * @param pfd 
 *    File Descriptor
 * @param array 
 *    Output data
 * @param pnwords
 *    32-bit (4 bytes) words to be read
 * @param pswords 
 *    First word to be read (starts at 0)
 * @param pnerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_READING_FILE 
 *
 * @bug This routine assumes the size of the words are 4 bytes.
 *
 * @date 970123:   Replaced block-at-a-time reading with one big
 *                 read.  DEV_BSIZE & BSIZE are no longer used. maf
 * @date 03/18/92  Made default DEV_BSIZE 512 if not defined. wct
 * @date 03/13/87  Really Tested--initialized ret to 0--B. Hickman
 * @date 12/16/85  Sorta Tested--D. Trimmer
 *
 */
void
zrabs(int  *pfd,
      char *array,
      int   pnwords,
      int  *pswords,
      int  *pnerr) {

  int ret;

  *pnerr = 0;

  lseek(-(*pfd), (off_t)(*pswords * sizeof(float)), SEEK_SET);

  ret = read ( -(*pfd) , array , pnwords * sizeof(float) ) ;

  if ( (size_t)ret != pnwords * sizeof(float) ) {
    *pnerr = ERROR_READING_FILE;
    setmsg ( "ERROR" , *pnerr ) ;
  }

  return;
}

