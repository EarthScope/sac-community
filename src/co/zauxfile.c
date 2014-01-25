/** 
 * @file   zauxfile.c
 * 
 * @brief  Install a file in the aux directory
 * 
 */

#include <stdio.h>
#include <string.h>

#include "mach.h"
#include "co.h"
#include "bot.h"
#include "msg.h"

#include "errors.h"


#include "wild.h"

/** 
 * Install a file in the SAC auxillary directory
 * 
 * @param ksub 
 *    Name of the auxillary subdirectory
 * @param ksub_s 
 *    Length of \p ksub
 * @param kfile 
 *    Name of the file
 * @param kfile_s 
 *    Length of \p kfile
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_FILE_DOES_NOT_EXIST
 *    - ERROR_FILE_ALREADY_EXISTS
 *    
 * @date   871222:  File inquiry moved to zinquire.
 * @date   870921:  Original version.
 *
 * @bug Only used by exm/xinstallmacro() to install a macro
 *      Not sure if this is used anymore
 *
 */
void 
zauxfile(char *ksub, 
	 int   ksub_s, 
	 char *kfile, 
	 int   kfile_s, 
	 int  *nerr) {

	char kdirpart[MCPFN+1], kfilepart[MCPFN+1], kgfile[MCPFN+1], kmsg[MCMSG+1];
	int lexist;
	int nc, nc1, nc2, idx ;

	*nerr = 0;

	for( idx = 0 ; idx < MCPFN ; idx++ )
	    kgfile[ idx ] = ' ' ;
        kgfile[ MCPFN ] = '\0' ;

	/* - Make sure file exists. */

	zinquire( kfile, &lexist );

	if( !lexist ){
		*nerr = ERROR_FILE_DOES_NOT_EXIST;
		setmsg( "ERROR", *nerr );
		apcmsg( kfile,kfile_s );
		goto L_8888;
	}

	/* - Build global file name. */

	getdir( kfile,kfile_s, kdirpart,MCPFN+1, kfilepart,MCPFN+1 );
	zbasename( kgfile,MCPFN+1 );
	crname( kgfile,MCPFN+1, KSUBDL, ksub,ksub_s, nerr );
	if( *nerr != 0 )
		goto L_8888;
	crname( kgfile,MCPFN+1, KDIRDL, kfilepart,MCPFN+1, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Check to see that a global macro by this name does not already exist. */

	zinquire( kgfile, &lexist );
	if( lexist ){
                fprintf(MUNOUT," Global file with this name already exists\n");
L_3000:
		zgpmsg( "Do you want to overwrite this file?  $",39, kmsg,MCMSG+1 );
		if( kmsg[0] == 'n' || kmsg[0] == 'N' ){
			*nerr = ERROR_FILE_ALREADY_EXISTS;
			setmsg( "ERROR", *nerr );
			apcmsg( kgfile,MCPFN+1 );
			goto L_8888;
			}
		else if( kmsg[0] != 'y' && kmsg[0] != 'Y' ){
                        fprintf(MUNOUT,"  Please answer with a \"YES\" or \"NO\".\n");
			goto L_3000;
			}
		}

	/* - Copy input file to global directory.
	 *   UNIX Implementation:  use zsysop to first run "cp" to copy file
	 *   and then run "chmod" to change protection modes so anyone can use it. */

	nc1 = indexb( kfile,kfile_s );
	nc2 = indexb( kgfile,MCPFN+1 );

        memcpy(kmsg,"cp ",3);
        memcpy(kmsg+3,kfile,nc1);
        kmsg[nc1+3] = ' ';
        memcpy(kmsg+nc1+4,kgfile,nc2);
        kmsg[nc1+nc2+4] = '\0';

	nc = indexb( kmsg,MCMSG+1 );
	zsysop( kmsg,MCMSG+1, &nc, nerr );

        memcpy(kmsg,"chmod a=rwx ",12);
        memcpy(kmsg+12,kgfile,nc2);
        kmsg[nc2+12] = '\0';

	nc = indexb( kmsg,MCMSG+1 );
	zsysop( kmsg,MCMSG+1, &nc, nerr );

L_8888:

	return;
} /* end of function */

