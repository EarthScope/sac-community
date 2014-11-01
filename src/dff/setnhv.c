/** 
 * @file   setnhv.c
 * 
 * @brief  Set an integer header value
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "amf.h"
#include "dff.h"
#include "bot.h"
#include "co.h"
#include "hdr.h"
#include "msg.h"
#include "lhf.h"
#include "bool.h"
#include "SacHeader.h"
#include "errors.h"

/** 
 * Set an integer header value for the current SAC file
 * 
 * @param kname 
 *    Header field to set
 * @param nvalue 
 *    Integer value to set
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_ILLEGAL_HEADER_FIELD_NAME
 * @param kname_s 
 *    Length of \p kname
 *
 * @date   870902:  Original version.
 *
 */
void 
setnhv(char *kname, 
       int  *nvalue, 
       int  *nerr, 
       int   kname_s) {

	char ktest[9];
	int index;
	char *kname_c;
  sac *s;
	kname_c = fstrdup(kname, kname_s);
	kname_s = strlen(kname_c) + 1;

	*nerr = 0;
  s = sacget_current();
	/* - Convert input name to uppercase and check versus list of legal names. */
  sacio_char_to_keyword(kname_c, ktest);
	index = nequal( ktest, (char*)kmlhf.knhdr,9, SAC_HEADER_INTEGERS );

	/* - Store value in appropriate header field. */
	if( index > 0 ){
    NHDR(s)[index-1] = *nvalue;
	}
	else{
	    *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
	}

	/* - Create error message and write to terminal. */

	if( *nerr != 0 ){
    sacio_message(*nerr, kname_c);
	}
	
	free(kname_c);
	kname_c = NULL;

	return;
}



/* Wrapper to make the function more convenient for FORTRAN programmers. */

void setnhv_ (char     *kname,
              int      *nvalue, 
	      int      *nerr, 
	      int       kname_s) {
  setnhv ( kname , nvalue , nerr , kname_s ) ;
}

void setnhv__ (char     *kname,
	       int      *nvalue, 
	       int      *nerr, 
	       int       kname_s) {
  setnhv ( kname , nvalue , nerr , kname_s ) ;
}
