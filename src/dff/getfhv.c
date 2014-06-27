/** 
 * @file   getfhv.c
 * 
 * @brief  Get a floating point header value
 * 
 */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "dff.h"
#include "bot.h"
#include "bool.h"
#include "hdr.h"
#include "co.h"
#include "msg.h"
#include "lhf.h"
#include "SacHeader.h"
#include "errors.h"

static char *env_on[]  = {"on",  "true",  "yes", "1" };
static char *env_off[] = {"off", "false", "no",  "0" };

int
sacio_message_control() {
  int i, n;
  char *env_string;
  static int flag = -1;
  if(flag == -1) {
    if((env_string = getenv("SACIO_MESSAGES"))) {
      n = strlen(env_string);
      flag = FALSE;
      for(i = 0; i < (int)(sizeof(env_on)/sizeof(char *)); i++) {
        if(strncasecmp(env_string, env_on[i], min(n, strlen(env_on[i]))) == 0) {
          flag = TRUE;
        }
      }
      for(i = 0; i < (int)(sizeof(env_off)/sizeof(char *)); i++) {
        if(strncasecmp(env_string, env_off[i], min(n, strlen(env_off[i]))) == 0) {
          flag = FALSE;
        }
      }
    } else {
      flag = FALSE;
    }
  }
  return flag;
}

void
sacio_message(int nerr, char *name) {
  if(sacio_message_control()) {
    error(nerr, "%s", name);
    outmsg();
    clrmsg();
  }
}

void
sacio_char_to_keyword(char *in, char out[SAC_HEADER_STRING_LENGTH]) {
  int k = 0;
  memset(out, ' ', SAC_HEADER_STRING_LENGTH);
  out[SAC_HEADER_STRING_LENGTH-1] = 0;
  while(k <= SAC_HEADER_STRING_LENGTH && in[k] && ! isspace(in[k]) ) {
    out[k] = toupper(in[k]);
    k++;
  }
}

extern sac *CURRENT;


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
	int index;
	char *kname_c;

	kname_c = fstrdup(kname, kname_s);
	kname_s = strlen(kname_c) + 1;

	*nerr = 0;

	/* - Convert input name to uppercase and check versus 
	 *   list of legal names. */
  sacio_char_to_keyword(kname_c, ktest);
	index = nequal( ktest, (char*)kmlhf.kfhdr,9, SAC_HEADER_FLOATS );

	/* - If legal name, return current value.
	 *   Otherwise, set error condition. */
	if( index > 0 ){
    *fvalue = VALUE(fhdr(CURRENT,index));
    if( *fvalue == SAC_FLOAT_UNDEFINED )
      *nerr = ERROR_UNDEFINED_HEADER_FIELD_VALUE;
	}
	else{
	    *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
	    *fvalue = SAC_FLOAT_UNDEFINED;
	}

	/* - Create error message and write to terminal. */
	if( *nerr != 0 ){
    sacio_message(*nerr, kname_c);
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
