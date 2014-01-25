/** 
 * @file   cfmt.c
 * 
 * @brief  Format and send an error message
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpf.h"
#include "com.h"
#include "bot.h"
#include "top.h"
#include "msg.h"
#include "co.h"
#include "debug.h"

/** 
 * Format and send a error message containing the current command
 * 
 * @param kmsg2 
 *    Second line of error message with a "$" appended to it
 *    A point to the bad token will be added to this line.
 *    The first line will contain a standard error message 
 *    with the current command appended to it.
 * @param kmsg2_s 
 *    Length of \p kmsg2
 *
 * @date   820615:  Added call to CERR to set error condition.
 * @date   820420:  Original version.
 *
 */
void 
cfmt(char *kmsg2, 
     int   kmsg2_s) {

	static char kmsg1[28] = "ERROR interpreting command:";

	/* - Raise command syntax error condition. */
	cerr( 1001 );
  
  char *s = strcut(kmsg2, 1, indexb(kmsg2, kmsg2_s));
  bell();
  fprintf(stdout, " %s  %s \n", kmsg1, lexer_input());
  if(arg()) {
    fprintf(stdout, " %*s  %*s\n", -(int)strlen(kmsg1), s, arg()->col, "^");
  } else {
    fprintf(stdout, " %*s  %*s\n", -(int)strlen(kmsg1), s, (int)strlen(lexer_input()), "^");
  }
  
  FREE(s);

  return;

	return;

} /* end of function */

void
arg_msg(char *msg) {
  fprintf(stdout, " %s %s \n", msg, lexer_input());
  fprintf(stdout, "  %*s\n", (int)strlen(msg)+arg()->col, "^");  
}
