/** 
 * @file   setfhv.c
 * 
 * @brief  Set a floating point header value
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "hdr.h"
#include "msg.h"
#include "co.h"
#include "bot.h"
#include "lhf.h"
#include "bool.h"

#include "errors.h"

/** 
 * Set a floating point header value in the current SAC file
 * 
 * @param kname 
 *    Name of the header field
 * @param fvalue 
 *    New Value 
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
setfhv(char  *kname, 
       float *fvalue, 
       int   *nerr, 
       int    kname_s) {

	char ktest[9];
	int index, ntest;
	char *kname_c;

	kname_c = fstrdup(kname, kname_s);
	kname_s = strlen(kname_c) + 1;

	*nerr = 0;

	/* - Convert input name to uppercase and 
	 *   check versus list of legal names. */

	ntest = min( indexb( kname_c,kname_s ), SAC_HEADER_STRING_LENGTH_FILE );
	strcpy( ktest, "        " );
	modcase( TRUE, kname_c, ntest, ktest );
	index = nequal( ktest, (char*)kmlhf.kfhdr,9, SAC_HEADER_FLOATS );

	/* - Store value in appropriate header field. */
	if( index > 0 ){
	    Fhdr[index] = *fvalue;
	}
	else{
	    *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
	    Fhdr[index] = cmhdr.fundef;
	}

	/* - Create error message and write to terminal. */

	if( *nerr != 0 ){
	    setmsg( "ERROR", *nerr );
	    apcmsg( kname_c,kname_s );
	    outmsg();
      clrmsg();
	}

	free(kname_c);
	kname_c = NULL;

	return;
}



/* Wrapper to make the function more convenient for FORTRAN programmers. */

void 
setfhv_ (char      *kname, 
	 float     *fvalue, 
	 int       *nerr, 
	 int        kname_s) {
  setfhv ( kname , fvalue , nerr , kname_s ) ;
}
void 
setfhv__ (char      *kname, 
	  float     *fvalue, 
	  int       *nerr, 
	  int        kname_s) {
  setfhv ( kname , fvalue , nerr , kname_s ) ;
}
