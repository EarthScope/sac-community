/** 
 * @file   getlhv.c
 * 
 * @brief  Get a logical header value
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "bot.h"
#include "co.h"
#include "msg.h"
#include "hdr.h"
#include "lhf.h"
#include "bool.h"
#include "SacHeader.h"
#include "errors.h"
extern sac *CURRENT;
/** 
 * Get a logical header value from the current SAC file
 * 
 * @param kname 
 *    Header variable to get
 * @param lvalue 
 *    Logical value to get
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
getlhv(char *kname, 
       int  *lvalue, 
       int  *nerr, 
       int   kname_s) {

	char ktest[9];
	int index;
	char *kname_c;

	kname_c = fstrdup(kname, kname_s);
	kname_s = strlen(kname_c) + 1;
	
	*nerr = 0;

	/* - Convert input name to uppercase and 
	 *   check versus list of legal names. */
  sacio_char_to_keyword(kname_c, ktest);
	index = nequal( ktest, (char*)kmlhf.klhdr,9, SAC_HEADER_LOGICALS );

	/* - If legal name, return current value.
	 *   Otherwise, set error condition. */

	if( index > 0 ){
    *lvalue = LHDR(CURRENT)[index-1];
	}
	else{
	    *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
	    *lvalue = FALSE;
	}

	/* - Create error message and write to terminal. */

	if( *nerr != 0 ){
    sacio_message(*nerr, kname_c);
	}

	free(kname_c);

	return;
} 




/* Wrapper to make code more convenient for FORTRAN programmers. */

void getlhv_ (char      *kname, 
	      int       *lvalue, 
	      int       *nerr, 
	      int        kname_s) {
  getlhv ( kname , lvalue , nerr , kname_s ) ;
}
void getlhv__ (char      *kname, 
	       int       *lvalue, 
	       int       *nerr, 
	       int        kname_s) {
  getlhv ( kname , lvalue , nerr , kname_s ) ;
}
