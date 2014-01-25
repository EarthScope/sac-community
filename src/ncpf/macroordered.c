
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mach.h"
#include "ncpf.h"
#include "cpf.h"

#include "string_utils.h"


#include "co.h"
#include "msg.h"
#include "bot.h"
#include "ucf.h"
#include "vars.h"

void /*FUNCTION*/ macroordered(kmacroargs, kmacroargs_s, nun, kline, 
	 kline_s, nerr)
char *kmacroargs;   int kmacroargs_s;
FILE *nun;
char *kline;   int kline_s;
int *nerr;
{
	char kdef[MCMSG+1], key[MCMSG+1], kmacroname[MCPFN+1], ktoken[9], 
	 kval[MCMSG+1];
  Token *t;
	int ic1, ic2, itype, jline, nc, 
	 ncargs, nkey, nline, nval, numsave;
        char *s1;
        char *p;
        memset(key, 0, sizeof(key));
        memset(kdef, 0, sizeof(kdef));
        memset(kmacroname, 0, sizeof(kmacroname));
        memset(ktoken, 0, sizeof(ktoken));
        
	/*=====================================================================
	 * PURPOSE: To process an "ordered" SAC macro file preamble.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    kmacroargs:  Arguments from macro execute line. [c]
	 *    nun:         Fortran file unit that macro file is open on. [i]
	 *    kline:       Next line from command file. [c]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error return flag.  Set to 0 if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  cpf/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    MCMSG, MCPW, MCPFN
	 *    cpf:     knoval, kvarsname
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900322:  Fixed bug so that quoted strings are treated as
	 *             a single argument and pass through accordingly.
	 *    870915:  Added ability to skip over blank or comment lines.
	 *    870402:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870402
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Store the entire argument line in the vars section under the keyword "all". */
	ncargs = indexb( kmacroargs,kmacroargs_s );
	if( ncargs > 0 )
		putvvstring( kmcpf.kvarsname,9, "all",4, ncargs, kmacroargs
		 ,kmacroargs_s, nerr );

	/* - Process argument line.  Each token is stored in numerical order.
	 *   The jth token is stored as the keyword "j". */

	nkey = 1;

  while((t = arg())) {
    sprintf(key, "%d", nkey++);
    token_to_var(t, kmcpf.kvarsname, key);
    arg_next();
  }

	/* - For each default card in macro preamble, starting with the one
	 *   passed in by the calling subroutine:
	 *   (Format of a default card is: $DEFAULT keyword value)
	 *   If keyword already has a definition do nothing.
	 *   Otherwise store the default value.
	 * - If not a default card, return. */

	nline = indexb( kline,kline_s );
L_2000:
	jline = 0;
  ic1 = 0;
  ic2 = 0;
	poptok( kline, nline, &jline, &ic1, &ic2, &itype );
  
  if( ic2 >= ic1 ) {
    s1 = strcut(kline, ic1, ic2);
    upcase( s1, ic2 - ic1 + 1, ktoken,9 );
    free(s1);
	  if( strcmp(ktoken,"$DEFAULT") == 0 ){
      poptok( kline, nline, &jline, &ic1, &ic2, &itype );
      fstrncpy( kdef, MCMSG, kline+ic1 - 1,ic2 - ic1 + 1);
      nval = nline - jline + 1;
      fstrncpy( kval, MCMSG, kline+jline - 1,nline - jline + 1);
      rstrip(kdef);
      rstrip(kval);
      {
        Token *t;
        char *p;
        t = string_to_token_list(kval);
        if(t) {
          p = upcase_dup(kdef);
          sac_vars_put_var(kmcpf.kvarsname, p, VAR_LIST, t);
        }
        if(p) { free(p); p= NULL;}
      }
    L_3000:
      if(fgetsp( kline,kline_s,nun)==NULL){
        if(feof(nun))goto L_8888;
        goto L_9000;
      }
      if(kline[(numsave=strlen(kline)-1)] == '\n') kline[numsave] = ' ';
      
      nline = indexb( kline,kline_s );
      if( nline <= 0 || kline[0] == '*' )
        goto L_3000;
      p = lstrip(&kline[0]);
      if(*p == '\0') {
        goto L_3000;
      }
      goto L_2000;
    }
  }
 L_8888:
	return;
  
L_9000:
	*nerr = 114;
	setmsg( "ERROR", *nerr );
	apcmsg( "macro ordered preamble for",27 );
	getvvstring( kmcpf.kvarsname,9, "macroname",10, &nc, kmacroname
	 ,MCPFN+1, nerr );
	apcmsg( kmacroname,MCPFN+1 );
	goto L_8888;


} /* end of function */

