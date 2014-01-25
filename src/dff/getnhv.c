/** 
 * @file   getnhv.c
 * 
 * @brief  Get an integer header value
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "msg.h"
#include "hdr.h"
#include "bot.h"
#include "co.h"
#include "lhf.h"
#include "bool.h"

#include "errors.h"

/** 
 * Get an integer header value from the current SAC file
 * 
 * @param kname 
 *    Name of the header variable
 * @param nvalue 
 *    Integer value on return
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_UNDEFINED_HEADER_FIELD_VALUE
 *    - ERROR_ILLEGAL_HEADER_FIELD_NAME
 * @param kname_s 
 *    Length of \p kname
 *
 * @date   870902:  Original version.
 *
 */
void 
getnhv(char *kname, 
       int  *nvalue, 
       int  *nerr, 
       int   kname_s) {

	char ktest[9];
	int index, ntest;

	char *kname_c;

	kname_c = fstrdup(kname, kname_s);
	kname_s = strlen(kname_c) + 1;

	*nerr = 0;

	/* - Convert input name to uppercase and 
	 *   check versus list of legal names. */
	ntest = min( indexb( kname_c,kname_s ), MCPW );
	strcpy( ktest, "        " );
	modcase( TRUE, kname_c, ntest, ktest );
	index = nequal( ktest, (char*)kmlhf.knhdr,9, SAC_HEADER_INTEGERS );

	/* - If legal name, return current value.
	 *   Otherwise, set error condition. */

	if( index > 0 ){
	    *nvalue = Nhdr[index];
	    if( *nvalue == cmhdr.nundef )
		*nerr = ERROR_UNDEFINED_HEADER_FIELD_VALUE;
	}
	else{
	    *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
	    *nvalue = cmhdr.nundef;
	}

	/* - Create error message and write to terminal. */

	if( *nerr != 0 ){
	    setmsg( "WARNING", *nerr );
	    apcmsg( kname_c,kname_s );
	    outmsg();
      clrmsg();
	}

	free(kname_c);

	return;
}




/* Wrapper to make code more convenient for FORTRAN programmers.  */

void getnhv_ (char      *kname, 
	      int       *nvalue, 
	      int       *nerr, 
	      int        kname_s) {
  getnhv ( kname , nvalue , nerr , kname_s ) ;
}
void getnhv__ (char      *kname, 
	       int       *nvalue, 
	       int       *nerr, 
	       int        kname_s) {
  getnhv ( kname , nvalue , nerr , kname_s ) ;
}
