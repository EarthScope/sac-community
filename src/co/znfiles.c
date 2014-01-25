/** 
 * @file   znfiles.c
 * 
 * @brief  Open a file
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "co.h"
#include "msg.h"
#include "bot.h"
#include "errors.h"

/** 
 * @param MRECLB
 *    Length of a fixed record in bytes for "DATA" type files.
 */
#define	MRECLB	512

/** 
 * Open an existing file or create a new one
 * 
 * @param nfu 
 *    File Descriptor
 * @param kname 
 *    File to open
 * @param kname_s 
 *    Length of \p kname
 * @param ktype 
 *    Type of file to open
 *    - 'DATA' for unformatted direct-access data (Unsupported)
 *    - 'TEXT' for formatted sequential access data 
 * @param ktype_s 
 *    Length of \p ktype
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_OPENING_FILE
 *
 * @date   871222:  Changed file inquire to zinquire.
 *             Included MASSCOMP differences.
 * @date   870526:  Merged BSD4.2 and SYSTEM V versions.
 * @date   860819:  Changed to new message system.
 * @date   851216:  Modified for UNIX file I/O--D. Trimmer
 * @date   850108:  Call ZEXPND to expand file path name.--D. Trimmer
 * @date   840117:  Now using ZDEST to destroy existing files.
 * @date   830812:  Original version.
 *
 */
void 
znfiles(FILE **nfu, 
	char  *kname, 
	int    kname_s, 
	char  *ktype, 
	int    ktype_s, 
	int   *nerr) {

    char *tmp;
	int lexist ;
	int noerr;

	*nerr = 0;
        noerr = 0;

	/* - Check existance of file */
	zinquire( kname, &lexist );

	/* - Open data file. */
	if( memcmp(ktype,"TEXT",4) == 0 ){
        tmp = rstrip(strdup(kname));
	    if( lexist ) {
		if((*nfu = fopen(tmp,"r+")) == NULL)
		    noerr = 1;
	    }
	    else {
		if((*nfu = fopen(tmp,"w+")) == NULL)
		    noerr = 1;
	    }
        free(tmp);
        tmp = NULL;
	    if( noerr != 0 ){
		*nerr = ERROR_OPENING_FILE;
		setmsg( "ERROR", *nerr );
		apcmsg( kname,kname_s );
		apcmsg( "(i/o error number =",19 );
		apimsg( noerr );
		apcmsg( ")",2 );
		goto L_8888;
	    }

	}
	else{
	    *nerr = ERROR_OPENING_FILE;
	    setmsg( "ERROR", *nerr );
	    apcmsg( kname,kname_s );
	    aplmsg( "Bad value for file type = ",27 );
	    apcmsg( ktype,ktype_s );
	    goto L_8888;
	}

L_8888:
	return;

}

