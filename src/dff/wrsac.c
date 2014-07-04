/** 
 * @file   wrsac.c
 * 
 * @brief  Write a SAC data file 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "unistdx.h"

#include "dff.h"
#include "co.h"
#include "msg.h"
#include "hdr.h"
#include "ucf.h"
#include "amf.h"
#include "dfm.h"
#include "bool.h"

#include "errors.h"



/** 
 * Write a SAC data file from memory to disk
 * 
 * @param idfl 
 *    Data file list index number
 * @param kname 
 *    Name of file to write
 * @param kname_s 
 *    Length of \p kname
 * @param ldta 
 *    - TRUE to write header and data
 *    - FALSE to write header only, not data
 * @param nerr 
 *    Error Retrun Flag
 *    - 0 on Success
 *    - ERROR_OUT_OF_MEMORY
 *    - ERROR_NOT_A_SAC_FILE
 *    - ERROR_WRITING_FILE
 *
 * @date   120108:  Bug fix for things added for v101.5.  
 * @date   870730:  Added logic to check file permissions before writing.
 * @date   850731:  Changes due to new memory manager.
 * @date   840118:  Deleted call to ZTRUNC.
 * @date   800510:  Original version.
 *
 */
void 
wrsac(int   idfl, 
      char *kname, 
      int   kname_s, 
      int   ldta, 
      int  *nerr) {

	int lswap;
  sac *s;
  char *filename;

	*nerr = 0;
  if(!(s = sacget(idfl-1, ldta, nerr))) {
    return;
  }
  filename = fstrdup(kname, kname_s);

  /* Determine if swapping is necessary */
  lswap = sac_byte_order(-1);

  /* Write the file */
  sac_write_r(s, filename, ldta, lswap, nerr);

  FREE(filename);

	return;
}

