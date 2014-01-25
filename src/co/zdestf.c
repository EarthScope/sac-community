/** 
 * @file   zdestf.c
 * 
 * @brief  Destroy a family of files
 * 
 */

#include <string.h>

#include "mach.h"
#include "co.h"
#include "bot.h"
#include "msg.h"

#include "errors.h"

/** 
 * Destroy a family of file names given the base name.  A two (2) digit
 *    integer is appended to the base name.
 * 
 * @param kbase 
 *    Base name
 * @param kbase_s 
 *    Length of \p kbase
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_ILLEGAL_BASE_NAME
 *
 * @date   860818:  Changed to new message system.
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   800906:  Fixed bug in determining correct base name.
 * @date   800823:  Added tree name capability [Prime].
 *                  Allowed a lower as well as upper range to be specified.
 * @date   800103:  Original version.
 *
 * @bug Only used by co/zquit() for destroy "ZDLF" files, which may 
 *       not exist anymore.
 *
 */
void 
zdestf(char *kbase, 
       int   kbase_s, 
       int  *nerr) {

	char kname[MCPFN+1];
	int j, jdig, jten, nbase;
	static char kint[10]={'0','1','2','3','4','5','6','7','8','9'};

	char *const Kint = &kint[0] - 1;

	*nerr = 0;

	/* - Make sure basename is legitimate. */
	if( memcmp(kbase," ",1) == 0 ){
		*nerr = ERROR_ILLEGAL_BASE_NAME;
		setmsg( "ERROR", *nerr );
		apcmsg( kname,MCPFN+1 );
		goto L_8888;
	}
	else{

		/* - Determine number of characters in base name. */
		nbase = indexb( kbase,kbase_s );
		if( nbase <= 0 ){
			nbase = MCPFN - 2;
		}
		else if( nbase > MCPFN - 2 ){
			nbase = MCPFN - 2;
		}

		/* - Destroy files with basename staring at "01" 
		 *   until an error occurs. ASSUME this error means 
		 *   that there are no more files with basename. */
		jten = 1;
		jdig = 2;
		for( j = 1; j <= 99; j++ ){

			/* -- Create file name. */
                        memset(kname,(int)' ',MCPFN);
                        kname[MCPFN] = '\0';
                        memcpy(kname,kbase,nbase);
                        kname[nbase] = Kint[jten];
                        kname[nbase + 1] = Kint[jdig];

			/* -- Try to destroy it.  Return on error.
			 *    This should normally be "File does not exist." */
			zdest( kname,MCPFN+1, nerr );
			if( *nerr != 0 )
				goto L_8888;

			/* -- Increment numbers appended to basename. */
			if( jdig < 10 ){
				jdig = jdig + 1;
			}
			else{
				jten = jten + 1;
				jdig = 1;
			}
		}
	}

	/* - Clear error condition for "File does not exist." */
L_8888:
	if( *nerr == ERROR_FILE_DOES_NOT_EXIST ){
		*nerr = 0;
		clrmsg();
	}


	return;

} /* end of function */

