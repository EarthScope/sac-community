/** 
 * @file   setihv.c
 * 
 * @brief  Set an enumerated value
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "bot.h"
#include "msg.h"
#include "lhf.h"
#include "co.h"
#include "bool.h"

#include "errors.h"

/** 
 * Set an enumerated header value in the current SAC file
 * 
 * @param kname 
 *    Name of the header field to set
 * @param kvalue 
 *    Enumerated Value to set
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_ILLEGAL_HEADER_FIELD_NAME
 *    - ERROR_ILLEGAL_ENUMERATED_VALUE
 * @param kname_s 
 *    Length of \p kname
 * @param kvalue_s 
 *    Length of \p kvalue
 *
 * @date   870918:  Changed from integer to character input for value field.
 * @date   870902:  Original version.
 *
 */
void 
setihv(char *kname, 
       char *kvalue, 
       int  *nerr, 
       int   kname_s, 
       int   kvalue_s) {

	char ktest[9];
	int index, ivalue, ntest;

	char *kname_c;
	char *kvalue_c;

	kname_c  = fstrdup(kname, kname_s);
	kvalue_c = fstrdup(kvalue, kvalue_s);
	
	kname_s  = strlen(kname_c)  + 1;
	kvalue_s = strlen(kvalue_c) + 1;

	*nerr = 0;

	/* - Convert input value to uppercase and check versus list of allowed values. */
	ntest = min( indexb( kvalue_c,kvalue_s ), SAC_HEADER_STRING_LENGTH_FILE );
	strcpy( ktest, "        " );
	modcase( TRUE, kvalue_c, ntest, ktest );
	ivalue = nequal( ktest, (char*)kmlhf.kiv,9, SAC_ENUMS );

	/* - If not a match, set and report error condition. */
	if( ivalue <= 0 ){
	    *nerr = ERROR_ILLEGAL_ENUMERATED_VALUE;
	    setmsg( "ERROR", *nerr );
	    apcmsg( kvalue_c,kvalue_s );
	    outmsg();
      clrmsg();
	    ivalue = cmhdr.iundef;
	}

	/* - Convert input name to uppercase and check versus list of legal names. */
	ntest = min( indexb( kname_c,kname_s ), MCPW );
	strcpy( ktest, "        " );
	modcase( TRUE, kname_c, ntest, ktest );
	index = nequal( ktest, (char*)kmlhf.kihdr,9, SAC_HEADER_ENUMS );

	/* - If legal header name, store value in appropriate header field.
	 *   Otherwise, set and report error condition. */
	if( index > 0 ){
	    Ihdr[index] = ivalue;
	}
	else{
	    *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
	    setmsg( "ERROR", *nerr );
	    apcmsg( kname,kname_s );
	    outmsg();
	}

	free(kname_c);
	free(kvalue_c);

	return;
}




/* Wrapper to make the function more convenient for FORTRAN programmers. */

void setihv_ (char      *kname, 
	      char      *kvalue, 
	      int       *nerr, 
	      int        kname_s,
	      int        kvalue_s) {
  setihv ( kname , kvalue , nerr , kname_s , kvalue_s ) ;
}
void setihv__ (char     *kname, 
	      char      *kvalue, 
	      int       *nerr, 
	      int        kname_s,
	      int        kvalue_s) {
  setihv ( kname , kvalue , nerr , kname_s , kvalue_s ) ;
}
