/** 
 * @file   zwabs.c
 * 
 * @brief  Write a file
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
 * Write a data file
 * 
 * @param pfd 
 *    File Descriptor
 * @param array 
 *    Data to write
 * @param pnwords 
 *    32 bit (4 byte) words to write
 * @param pswords 
 *    First 32 bit work to write (starts at 0)
 * @param pnerr 
 *    Error Return Flag
 *    - 0 on Succcess
 *    - ERROR_WRITING_FILE
 *
 * @date Feb 7, 2008 - Removed Buffered write and DEV_BSIZE - Savage
 * @date 03/18/92  Added default DEV_BSIZE. wct
 * @date 03/16/87  Really Tested--initialized ret to 0--B.Hickman
 * @date 12/16/85  Sorta Tested--D. Trimmer
 *
 */
void
zwabs(int  *pfd,
      char *array,
      int   pnwords,
      int  *pswords,
      int  *pnerr) {

  int ret;

  *pnerr = 0;

  if(lseek(-(*pfd), (off_t)(*pswords * sizeof(float)), 0) == -1) {
    *pnerr = ERROR_WRITING_FILE;
    setmsg("ERROR", *pnerr);
    return;
  }

  ret = write(-(*pfd), &array[0], pnwords * sizeof(float));

  if((size_t)ret != pnwords * sizeof(float)) {
    *pnerr = ERROR_WRITING_FILE;
    setmsg("ERROR", *pnerr);
    return;
  }

  return;
}

