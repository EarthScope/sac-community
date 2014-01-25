/** 
 * @file   getkhv.c
 * 
 * @brief  Get a characer header value
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "msg.h"
#include "bot.h"
#include "hdr.h"
#include "co.h"
#include "lhf.h"
#include "bool.h"

#include "errors.h"

/** 
 * Get a character header value from the current SAC file
 * 
 * @param kname 
 *    Header variable to get
 * @param kvalue 
 *    Characeter header variable
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_UNDEFINED_HEADER_FIELD_VALUE
 *    - ERROR_ILLEGAL_HEADER_FIELD_NAME 
 * @param kname_s 
 *    Length of \p kname
 * @param kvalue_s 
 *    Length of \p kvalue
 *
 * @date   870902:  Original version.
 *
 */
void 
getkhv(char *kname, 
       char *kvalue, 
       int  *nerr, 
       int   kname_s, 
       int   kvalue_s) {

	char ktest[9];
	int index, ntest;
	
	char *kname_c;
	int callFromC = 0;

	if(kname_s < 0) {
	  callFromC = 1;
          kvalue_s = kvalue_s + 1; /* This +1 will be removed later on */
          if(kvalue_s <= 1) {
            *nerr = SAC_OK;
            return ;
          }
	}


	kname_c = fstrdup(kname, kname_s);
	kname_s = strlen(kname_c) + 1;

	*nerr = 0;
    memset(kvalue, 0, kvalue_s);
	/* - Convert input name to uppercase and 
	 *   check versus list of legal names. */
	ntest = min( indexb( kname_c,kname_s ), SAC_HEADER_STRING_LENGTH_FILE );
	strcpy( ktest, "        " );
	modcase( TRUE, kname_c, ntest, ktest );
	index = nequal( ktest, (char*)kmlhf.kkhdr,9, SAC_HEADER_STRINGS );

	/* - If legal name, return current value.
	 *   Otherwise, set error condition. */
	if( index > 0 ){
          fstrncpy( kvalue, kvalue_s-1, kmhdr.khdr[index - 1],  strlen(kmhdr.khdr[index - 1]) );
        if( memcmp(kvalue,kmhdr.kundef,min(strlen(kvalue), strlen(kmhdr.kundef))) == 0 ){
          *nerr = ERROR_UNDEFINED_HEADER_FIELD_VALUE;
	    }
	}
	else{
	    *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
	    fstrncpy( kvalue, kvalue_s-1, kmhdr.kundef, strlen(kmhdr.kundef) );
	    index = 1;
	}

	/* - Create error message and write to terminal. */

	if( *nerr != 0 ){
	    setmsg( "WARNING", *nerr );
	    apcmsg( kname_c,kname_s );
      outmsg();
      clrmsg();
	}
	if(callFromC) {
          /* Null Terminate the String at the approproiate Length */
          kvalue[min(((index == 2) ? 16 : 8), kvalue_s-1)] = 0; 
	} else {
      if(index == 2 && kvalue_s > 16) {
        memset(kvalue + 16, ' ', kvalue_s-16);
      }
      if(index != 2 && kvalue_s > 8) {
        memset(kvalue + 8, ' ', kvalue_s - 8);
      }
    }

	free(kname_c);

	return;
}




/* Wrapper to make fuction more convenient for FORTRAN programmers. */

void getkhv_ (char      *kname, 
	      char      *kvalue, 
	      int       *nerr, 
	      int        kname_s,
	      int        kvalue_s) {
  getkhv ( kname , kvalue , nerr , kname_s , kvalue_s ) ;
}

void getkhv__ (char      *kname, 
	       char      *kvalue, 
	       int       *nerr, 
	       int        kname_s,
	       int        kvalue_s) {
  getkhv ( kname , kvalue , nerr , kname_s , kvalue_s ) ;
}
