/** 
 * @file   zopens.c
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
 * Open a file
 * 
 * @param nfu 
 *    File descriptor
 * @param kname 
 *    File to open
 * @param kname_s 
 *    Length of \p kname
 * @param ktype 
 *    Type of file to open
 *    - 'DATA'    for unformatted, direct-access file. (Unsupported)
 *    - 'RODATA'  for read-only DATA file. (Unsupported)
 *    - 'ROUNFR'  for read-only FORTRAN unformatted file. (Unsupported) 
 *    - 'TEXT'    for formatted sequential-access file.
 *    - 'ROTEXT'  for 'READ' for read-only TEXT file.
 *    - 'SCRATCH' for temporary file 'tmp...' that gets
 *                    deleted when the file is closed. (Unsupported)
 * @param ktype_s 
 *    Length of \p ktype
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_OPENING_FILE
 *    - ERROR_FILE_DOES_NOT_EXIST
 *
 * @bug This function is very similar to co/znfiles(). They should 
 *      probably be merged at some point.  There are about 5 different
 *      ways to open a file in this code base, there should be only two
 *      binary data (int/read/write) and 
 *      text-file string data (FILE/fscanf/fprintf)
 *
 * @date   920805:  Added 'ROUNFR' option.
 * @date   920319:  Added precompiler flag for SCRATCH file open on 1.
 * @date   910320:  Added option of status='SCRATCH' (wct)
 * @date   871222:  Moved file inquire to zinquire.
 *             Included MASSCOMP differences.
 * @date   870729:  Added 'RODATA' option.
 * @date   860819:  Changed to new message system.
 * @date   851216:  Modified for UNIX file I/O--D. Trimmer
 * @date   841219:  Expand KBASDR to path name, and convert full path name
 *                  to lower case--D. Trimmer
 * @date   830812:  Original version.
 *
 */
void 
zopens(FILE **nfu, 
       char  *kname, 
       int    kname_s, 
       char  *ktype, 
       int    ktype_s, 
       int   *nerr) {

    char *tmp;
	int lexist;
	int noerr;

	*nerr = 0;
        noerr = 0;
    tmp = NULL;
	/* - Error exit if file does not exist. */
	zinquire( kname, &lexist );

	if( !lexist ){
	    *nerr = ERROR_FILE_DOES_NOT_EXIST;
	    setmsg( "ERROR", *nerr );
	    apcmsg( kname,kname_s );
	    goto L_8888;
	}

	if( (memcmp(ktype,"TEXT",4) == 0 || 
	     memcmp(ktype,"ROTEXT",6) == 0) || 
	    memcmp(ktype,"READ",4) == 0 ){
        tmp = rstrip(strdup(kname));
        if((*nfu = fopen(tmp,"r")) == NULL) noerr = 1;
	    if( noerr != 0 ){
		*nerr = ERROR_OPENING_FILE;
		setmsg( "ERROR", *nerr );
		apcmsg( kname,kname_s );
		apcmsg( "(Fortran i/o error number =",28 );
		apimsg( noerr );
		apcmsg( ")",2 );
		goto L_8888;
	    }

	}
	else{
	    *nerr = ERROR_OPENING_FILE;
	    setmsg( "ERROR", *nerr );
	    apcmsg( kname,kname_s );
	    apcmsg( "(Bad value for file type =",27 );
	    apcmsg( ktype,ktype_s );
	    apcmsg( ")",2 );
	    goto L_8888;
	}

L_8888:
    if(tmp) {
        free(tmp);
    }
	return;

} /* end of function */

