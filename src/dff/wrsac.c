/** 
 * @file   wrsac.c
 * 
 * @brief  Write a SAC data file 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dff.h"
#include "co.h"
#include "msg.h"
#include "hdr.h"
#include "ucf.h"
#include "amf.h"
#include "dfm.h"
#include "bool.h"

#include "errors.h"

int
get_sac_npts(int num) {
  return Nlndta[num];
}

float * 
get_sac_header(int num) {
  return cmmem.sacmem[ Ndxhdr[num] ];
}

char * 
get_sac_header_char(int num) {
  return (char *) (get_sac_header(num) + SAC_HEADER_NUMBERS);
}

float * 
get_sac_data(int num, int comp) {
  return cmmem.sacmem[ cmdfm.ndxdta[num - 1][comp] ];
}

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

	int ncerr, nun;
	int lswap;

	/* For Determining the byte-order of a file or endianness */
	char *header;
	int begin = 0;
	
	*nerr = 0;
	nun = 0;
	/* - If header and data is to be written, a new file is created.
	 *   If header only is to be written, the old file is opened. */
	if( ldta ){
	    znfile( &nun, kname,kname_s, "DATA",5, nerr );
	    if( *nerr != 0 )
		goto L_8888;
            lswap = sac_byte_order(-1);
	}
	else{
	    zopen_sac( &nun, kname,kname_s, "DATA",5, nerr );
	    if( *nerr != 0 ) {
		goto L_8888;
	    }
	    /* Handle writeheader with different byte-order or endianness */
	    
	    if((header = (char *) malloc(SAC_HEADER_SIZEOF_FILE)) == NULL) {
	      *nerr = ERROR_OUT_OF_MEMORY;
	      setmsg("ERROR", *nerr);
	      goto L_8888;
	    }
	    zrabs((int *)&nun, header, SAC_HEADER_WORDS_FILE, &begin, (int *)nerr);
	    if(*nerr != SAC_OK) {
              goto L_8888;
            }
            
            lswap = sac_check_header_version((float *)header, nerr);
            free(header);
            if(*nerr != SAC_OK) {
              goto L_8888;
            }
          lseek(-nun, SEEK_SET, 0); 
	}

	/* - Write the header */
        sac_header_write(nun, 
                         get_sac_header(idfl),
                         get_sac_header_char(idfl),
                         lswap, 
                         nerr);
        
        /* Write the data */
        if( ldta ){
          sac_data_write(nun, 
                         get_sac_data(idfl, 0), 
                         get_sac_data(idfl, 1), 
                         get_sac_npts(idfl),
                         lswap, 
                         nerr);
	}

L_8888:
	/* Close the file. */
        if(nun != 0) {
            zclose( &nun, &ncerr );
        }
	return;
}

