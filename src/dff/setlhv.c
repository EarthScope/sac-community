/** 
 * @file   setlhv.c
 * 
 * @brief  Set a logical header value
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "bot.h"
#include "co.h"
#include "hdr.h"
#include "lhf.h"
#include "msg.h"
#include "bool.h"

#include "errors.h"

/** 
 * Set a logical header value in the current SAC file
 * 
 * @param kname 
 *    Header field to set
 * @param lvalue 
 *    Logical value to set
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
setlhv(char *kname, 
       int  *lvalue, 
       int  *nerr, 
       int   kname_s) {

	char ktest[9];
	int index, ntest;

	char *kname_c;

	kname_c = fstrdup(kname, kname_s);
	kname_s = strlen(kname_c) + 1;

	*nerr = 0;

	/* - Convert input name to uppercase and check versus list of legal names. */
  sacio_char_to_keyword(kname_c, ktest);
	index = nequal( ktest, (char*)kmlhf.klhdr,9, SAC_HEADER_LOGICALS );

	/* - Store value in appropriate header field. */
	if( index > 0 ){
	    Lhdr[index] = *lvalue;
	}
	else{
	    *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
	    Lhdr[index] = FALSE;
	}

	/* - Create error message and write to terminal. */
	if( *nerr != 0 ){
    sacio_message(*nerr, kname_c);
	}

 	free(kname_c); 

	return;
}




/* Wrapper to make the function more convenient for FORTRAN programmers. */

void setlhv_ (char      *kname, 
	      int       *lvalue, 
	      int       *nerr, 
	      int        kname_s) {
  setlhv ( kname , lvalue , nerr , kname_s ) ;
}
void setlhv__ (char      *kname, 
	       int       *lvalue, 
	       int       *nerr, 
	       int        kname_s) {
  setlhv ( kname , lvalue , nerr , kname_s ) ;
}
