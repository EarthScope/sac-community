/** 
 * @file   getfhv.c
 * 
 * @brief  Get a floating point header value
 * 
 */

#include <string.h>
#include <stdlib.h>

#include "dff.h"
#include "bot.h"
#include "bool.h"
#include "hdr.h"
#include "co.h"
#include "msg.h"
#include "lhf.h"

#include "errors.h"

/** 
 * Get a floating point header value from the current sac file
 * 
 * @param kname 
 *    Name of header field to get
 * @param fvalue 
 *    Floating point header field on return
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_UNDEFINED_HEADER_FIELD_VALUE
 *    - ERROR_ILLEGAL_HEADER_FIELD_NAME
 * @param kname_s 
 *    Length of \p kname
 *
 * @date   870918:  Added conversion of kname to uppercase.
 * @date   870902:  Original version.
 *
 */
void 
getfhv(char  *kname, 
       float *fvalue, 
       int   *nerr, 
       int    kname_s ) {

	char ktest[9];
	int index, ntest;

	char *kname_c;

	kname_c = fstrdup(kname, kname_s);
	kname_s = strlen(kname_c) + 1;

	*nerr = 0;

	/* - Convert input name to uppercase and check versus 
	 *   list of legal names. */
	ntest = min( indexb( kname_c,kname_s ), SAC_HEADER_STRING_LENGTH_FILE );
	strcpy( ktest, "        " );
	modcase( TRUE, kname_c, ntest, ktest );
	index = nequal( ktest, (char*)kmlhf.kfhdr,9, SAC_HEADER_FLOATS );

	/* - If legal name, return current value.
	 *   Otherwise, set error condition. */
	if( index > 0 ){
	    *fvalue = Fhdr[index];
	    if( *fvalue == cmhdr.fundef )
		*nerr = ERROR_UNDEFINED_HEADER_FIELD_VALUE;
	}
	else{
	    *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
	    *fvalue = cmhdr.fundef;
	}

	/* - Create error message and write to terminal. */
	if( *nerr != 0 ){
	    setmsg( "WARNING", *nerr );
	    apcmsg( kname_c,kname_s );
	    outmsg( );
      clrmsg();
	}
	
	free(kname_c);

	return;
}



/* The following is a wrapper to make the code more convenient for 
   FORTRAN programmers.  */

void getfhv_( char      *kname, 
	      float     *fvalue, 
	      int       *nerr, 
	      int        kname_s ) {
  getfhv ( kname , fvalue , nerr , kname_s ) ;
}
void getfhv__( char      *kname, 
	       float     *fvalue, 
	       int       *nerr, 
	       int        kname_s ) {
  getfhv ( kname , fvalue , nerr , kname_s ) ;
}
