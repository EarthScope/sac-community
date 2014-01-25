/** 
 * @file   zdest.c
 * 
 * @brief  Destroy a disk file 
 * 
 */
#include <unistd.h>

#include "co.h"
#include "bot.h"
#include "msg.h"

#include "errors.h"

/** 
 * Destroy a a diesk file if it exists
 * 
 * @param kname 
 *    Name of file to be destroyed
 * @param kname_s 
 *    Length of \p kname
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_FILE_DOES_NOT_EXIST
 *
 * @date   871222:  Moved file inquire to zinquire.
 * @date   850108:  Call ZEXPND to expand file path name--D. Trimmer
 * @date   830812:  Original version.
 *
 */
void 
zdest(char *kname, 
      int   kname_s, 
      int  *nerr) {

	int lexist;
	int nc;

	*nerr = 0;

	/* - Check existence of file. */
	zinquire( kname, &lexist );

	/* - If file exists: */
	if( lexist ){
            nc = indexb(kname,kname_s);
            kname[nc] = '\0';
            *nerr = unlink(kname);
	}
	else{
	    *nerr = ERROR_FILE_DOES_NOT_EXIST;
	    setmsg( "ERROR", *nerr );
	    apcmsg( kname,kname_s );
	}

	return;
}

