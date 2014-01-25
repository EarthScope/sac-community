/** 
 * @file   getihv.c
 * 
 * @brief  Get an enumerated header value
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "bot.h"
#include "msg.h"
#include "hdr.h"
#include "co.h"
#include "lhf.h"
#include "bool.h"

#include "errors.h"

/** 
 * Get an enumerated header value from the current SAC file
 * 
 * @param kname 
 *    Name of the header field to get
 * @param kvalue 
 *    Value of heade field from the current SAC data file
 *    Each value represents a specific condition
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_UNDEFINED_HEADER_FIELD_VALUE
 *    - 1337
 * @param kname_s 
 *    Length of \p kname
 * @param kvalue_s 
 *    Length of \p kvalye
 *
 * @date   870902:  Original version.
 *
 */
void 
getihv(char *kname, 
       char *kvalue, 
       int  *nerr, 
       int   kname_s, 
       int   kvalue_s) {

	char ktest[9];
	int index, ivalue, ntest;

	char *kname_c;
	int callFromC = 0;

	if(kname_s < 0) {
	  callFromC = 1;
	}

	kname_c = fstrdup(kname, kname_s);
	kname_s = strlen(kname_c) + 1;
	

	*nerr = 0;

	/* - Convert input name to uppercase and 
	 *   check versus list of legal names. */
	ntest = min( indexb( kname_c,kname_s ), SAC_HEADER_STRING_LENGTH_FILE );
	strcpy( ktest, "        " );
	modcase( TRUE, kname_c, ntest, ktest );
	index = nequal( ktest, (char*)kmlhf.kihdr,9, SAC_HEADER_ENUMS );

	/* - If legal name, return current value.
	 *   Otherwise, set error condition. */

	if( index > 0 ){
	    ivalue = Ihdr[index];
	    if( ivalue == cmhdr.iundef ){
    	        fstrncpy( kvalue, kvalue_s-1, "UNDEFINED", 9);
		*nerr = ERROR_UNDEFINED_HEADER_FIELD_VALUE;
	    }
	    else{
    	        fstrncpy( kvalue, kvalue_s-1, kmlhf.kiv[ivalue - 1],
                          strlen(kmlhf.kiv[ivalue - 1]) );
	    }
	}
	else{
    	    fstrncpy( kvalue, kvalue_s-1, "ILLEGAL", 7);
	    *nerr = 1337;
	}

	/* - Create error message and write to terminal. */

	if( *nerr != 0 ){
	    setmsg( "WARNING", *nerr );
	    apcmsg( kname_c,kname_s );
	    outmsg();
      clrmsg();
	}

	if(callFromC) { /* C String Termination */
        kvalue[ max(0,min(kvalue_s,8))] = 0;
        } else {        /* Fortran String Non-Termination by Spaces */
          if(kvalue_s > 8) {
            memset(kvalue + 8, ' ', kvalue_s - 8);
          }
        }

	free(kname_c);

	return;

}




/* Wrapper to make the function more convenient for FORTRAN programmers. */

void getihv_ (char *kname, 
	      char *kvalue, 
	      int  *nerr, 
	      int   kname_s, 
	      int   kvalue_s) {
  getihv ( kname , kvalue , nerr , kname_s , kvalue_s ) ;
}
void getihv__ (char *kname, 
	       char *kvalue, 
	       int  *nerr, 
	       int   kname_s, 
	       int   kvalue_s) {
  getihv ( kname , kvalue , nerr , kname_s , kvalue_s ) ;
}
