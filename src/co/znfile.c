/** 
 * @file   znfile.c
 * 
 * @brief  Open an existing file
 * 
 */

#include <string.h>

#include "co.h"
#include "bool.h"
#include "msg.h"

#include "errors.h"
#include "debug.h"

/** 
 * @param  MRECLB
 *    Length of a fixed record in bytes for "DATA" type files.
 */
#define	MRECLB	512

/** 
 * Opne an existing file or create a new one
 * 
 * @param nfu 
 *    Fortran file unit to open file on
 * @param kname 
 *    File to open
 * @param kname_s 
 *    Length of \p kname
 * @param ktype 
 *    Type of file to open
 *    - 'DATA' for unformatted direct-access data
 *    - 'TEXT' for formatted sequential access data (Unsupported)
 * @param ktype_s 
 *    Length of \ ktype
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
znfile(int  *nfu, 
       char *kname, 
       int   kname_s, 
       char *ktype, 
       int   ktype_s, 
       int  *nerr) {

	int lnewfl, lro;
	int noerr;

	*nerr = 0;

	/* - Open data file. */
	if( memcmp(ktype,"DATA",4) == 0 ){
	    lnewfl = TRUE;
	    lro = FALSE;
	    zopenc( (int *)nfu, kname, &lnewfl, &lro, (int*) &noerr, kname_s );
	    if( noerr != 0 ){
		*nerr = ERROR_OPENING_FILE;
		setmsg( "ERROR", *nerr );
		apcmsg( kname,kname_s );
		if( noerr == 1 )
		    apcmsg( "(Insufficient access rights.)",30 );
		else
		    apcmsg( "(System error occurred.)",25 );
		goto L_8888;
	    }
	}

	/* - Open text file. */
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

void
znfilef(int  *nfu,
	char *kname,     /* File name to open */
	char *ktype,     /* 'DATA' or Error */
	int  *nlen,      /* Unused, historical from Fortran on MASSCOMP */
	int  *nerr,      /* Error return Flag */
	int   kname_s,   /* Length of string kname */
	int   ktype_s) { /* Length of string ktype */
  UNUSED(nlen);
  znfile(nfu, kname, kname_s, ktype, ktype_s, nerr);
}

void
znfilef_(int  *nfu,
	 char *kname,
	 char *ktype,
	 int  *nlen,
	 int  *nerr,
	 int   kname_s,
	 int   ktype_s) {
  UNUSED(nlen);
  znfile(nfu, kname, kname_s, ktype, ktype_s, nerr);
}
void
znfilef__(int  *nfu,  
	  char *kname,
	  char *ktype,
	  int  *nlen,
	  int  *nerr,
	  int   kname_s,
	  int   ktype_s) {
  UNUSED(nlen);
  znfile(nfu, kname, kname_s, ktype, ktype_s, nerr);
}
