/** 
 * @file   zopen.c
 * 
 * @brief  Open a file
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "co.h"
#include "bool.h"
#include "dff.h"
#include "msg.h"

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
 *    File unit number
 * @param kname 
 *    File to open
 * @param kname_s 
 *    Length of \p kname
 * @param ktype 
 *    Type of disk file to open [k]:
 *    - 'DATA'    for unformatted, direct-access file.
 *    - 'RODATA'  for read-only DATA file.
 *    - 'ROUNFR'  for read-only FORTRAN unformatted file.
 *    - 'TEXT'    for formatted sequential-access file.
 *    - 'ROTEXT'  for 'READ' for read-only TEXT file.
 *    - 'SCRATCH' for temporary file 'tmp...' that gets
 *                  deleted when the file is closed.
 * @param ktype_s 
 *    Length of \p ktype
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_FILE_DOES_NOT_EXIST
 *    - ERROR_OPENING_FILE
 *
 * @date   920805:  Added 'ROUNFR' option.
 * @date   920319:  Added precompiler flag for SCRATCH file open on 1.
 * @date   910320:  Added option of status='SCRATCH' (wct)
 * @date   871222:  Moved file inquire to zinquire.
 *                  Included MASSCOMP differences.
 * @date   870729:  Added 'RODATA' option.
 * @date   860819:  Changed to new message system.
 * @date   851216:  Modified for UNIX file I/O--D. Trimmer
 * @date   841219:  Expand KBASDR to path name, and convert full path name
 *                  to lower case--D. Trimmer
 * @date   830812:  Original version.
 */
void 
zopen_sac(int  *nfu, 
	  char *kname, 
	  int   kname_s, 
	  char *ktype, 
	  int   ktype_s, 
	  int  *nerr) {

	int lexist, lnewfl, lro;
	int noerr;
	char *kname_c;

	*nerr = 0;

	/* - Error exit if file does not exist. */
	kname_c = fstrdup(kname, kname_s);
	kname_s = strlen(kname_c) + 1;

	zinquire( kname_c, &lexist );

	if( !lexist ){
	    *nerr = ERROR_FILE_DOES_NOT_EXIST;
	    setmsg( "ERROR", *nerr );
	    apcmsg( kname_c,kname_s );
	    goto L_8888;
	}

	/* - Open data file. */
	if( memcmp(ktype,"DATA",4) == 0 ){
	    lnewfl = FALSE;
	    lro = FALSE;
	    zopenc((int*) nfu, kname_c, &lnewfl, &lro, (int*)&noerr, kname_s );
	    if( noerr != 0 ){
		*nerr = ERROR_OPENING_FILE;
		setmsg( "ERROR", *nerr );
		apcmsg( kname_c,kname_s );
		if( noerr == 1 )
		    apcmsg( "(Insufficient access rights.)",30 );
		else
		    apcmsg( "(System error occurred.)",25 );
		goto L_8888;
	    }
	}
	else if( memcmp(ktype,"RODATA",6) == 0 ){
	    lnewfl = FALSE;
	    lro = TRUE;
	    zopenc( (int*)nfu, kname_c, &lnewfl, &lro,(int*) &noerr, kname_s );
	    if( noerr != 0 ){
		*nerr = ERROR_OPENING_FILE;
		setmsg( "ERROR", *nerr );
		apcmsg( kname_c,kname_s );
		if( noerr == 1 )
		    apcmsg( "(Insufficient access rights.)",30 );
		else
		    apcmsg( "(System error occurred.)",25 );
		goto L_8888;
	    }
	}

	/* - Open text file (read-only mode ignored for text files in UNIX.) */
	else if( memcmp(ktype,"ROUNFR",6) == 0 ){
	    noerr = 1;
	    if( noerr != 0 ){
		*nerr = ERROR_OPENING_FILE;
		setmsg( "ERROR", *nerr );
		apcmsg( kname_c,kname_s );
		apcmsg( "ROUNFR files not supported at this time-zopen",46 );
		apimsg( noerr );
		apcmsg( ")",2 );
		goto L_8888;
	    }
	}

	/* - Open data file (read-only mode ignored for text files in UNIX.) */
	else if( memcmp(ktype,"RODIR",5) == 0 ){
	    noerr = 1;
	    if( noerr != 0 ){
		*nerr = ERROR_OPENING_FILE;
		setmsg( "ERROR", *nerr );
		apcmsg( kname_c,kname_s );
		apcmsg( "RODIR files not supported at this time-zopen",45 );
		apimsg( noerr );
		apcmsg( ")",2 );
		goto L_8888;
	    }
	}

	else{
	    *nerr = ERROR_OPENING_FILE;
	    setmsg( "ERROR", *nerr );
	    apcmsg( kname_c,kname_s );
	    apcmsg( "(Bad value for file type =",27 );
	    apcmsg( ktype,ktype_s );
	    apcmsg( ")",2 );
	    goto L_8888;
	}

L_8888:
	free(kname_c);
	kname_c = NULL;
	return;

}


