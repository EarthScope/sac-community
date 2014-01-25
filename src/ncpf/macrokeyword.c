
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mach.h"
#include "ncpf.h"
#include "cpf.h"
#include "bool.h"

#include "string_utils.h"


#include "co.h"
#include "msg.h"
#include "bot.h"
#include "ucf.h"
#include "vars.h"

#include "debug.h"

Token *token_dup(Token *t);

void /*FUNCTION*/ macrokeyword(kmacroargs, kmacroargs_s, nun, keys, 
	 keys_s, nerr)
char *kmacroargs;   int kmacroargs_s;
FILE *nun;
char *keys;   int keys_s;
int *nerr;
{
	char kdef[MCMSG+1], key[MCMSG+1], kline[MCMSG+1], kmacroname[MCPFN+1], 
	 ktemp[MCMSG+1], ktoken[9], kval[MCMSG+1];
	int lfirst;
	int ic1, ic2, ick1, ick2,
	 iktype, itype, jkeys, jline, nc, 
	 nkey, nkeys, nline, nval, numsave;
  char *s1, *p;
        Token *t;
        Token *list, *last;
        memset(kdef, 0, sizeof(kdef));
        memset(key, 0, sizeof(key));
        memset(kline, 0, sizeof(kline));
        memset(kmacroname, 0, sizeof(kmacroname));
        memset(ktemp, 0, sizeof(ktemp));
        memset(ktoken, 0, sizeof(ktoken));
        memset(kval, 0, sizeof(kval));
	/*=====================================================================
	 * PURPOSE: To process a "keyworded"  SAC macro file preamble.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    kmacroargs:   Arguments from macro execute line. [c]
	 *    nun:          Fortran file unit that macro file is open on. [i]
	 *    keys:         List of keywords from macro preamble. [c]
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
	 *    880404:  Now lowercasing each token into a temporary string
	 *             before comparing to keywords.
	 *    870915:  Added option to skip over blank and comment lines.
	 *    870724:  Fixed bug that occurred  when there were no arguments.
	 *    870402:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870402
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;
  UNUSED(kmacroargs);
  UNUSED(kmacroargs_s);
	/* - Store the keyword list in the vars section. */

	nkeys = indexb( keys,keys_s );
	putvvstring( kmcpf.kvarsname,9, "keys",5, nkeys, keys,keys_s, 
	 nerr );

	/* - For each keyword in list, store it's name and give it
	 *   a "pseudo value" indicating that is does not have a "real value". */

	jkeys = 0;
L_1000:
	poptok( keys, nkeys, &jkeys, &ic1, &ic2, &itype );
	if( itype > 0 ){
		nkey = ic2 - ic1 + 1;
		fstrncpy( key, MCMSG, keys+ic1 - 1,ic2 - ic1 + 1);
    s1 = strcut(key, 1, nkey);
    p = upcase_dup(s1);
		putvvstring( kmcpf.kvarsname,9, p, nkey + 1, MCPW, kmcpf.knoval,9, nerr );
		free(s1);
    free(p);
		if( *nerr != 0 )
			goto L_8888;
		goto L_1000;
		}

	/* - For each default card in macro preamble:
	 *   (Format of a default card is: $DEFAULT keyword value)
	 *   If it is a legal keyword, store the default value in vars section.
	 *   Otherwise raise an error. */

L_2000:

        if(fgetsp( kline,MCMSG+1,nun)==NULL){
          if(feof(nun))goto L_8888;
          goto L_9000;
	}
        if(kline[(numsave=strlen(kline)-1)] == '\n') kline[numsave] = ' ';

	nline = indexb( kline,MCMSG+1 );
	if( nline <= 0 || kline[0] == '*' )
		goto L_2000;
  /* Find First Non-Whitespace character*/
  p = lstrip(&kline[0]);
  if(*p == '\0') {
    goto L_2000;
  }
	jline = 0;
	poptok( kline, nline, &jline, &ic1, &ic2, &itype );
    s1 = strcut(kline, ic1, ic2);
	upcase( s1, ic2-ic1 + 1, ktoken,9 );
    free(s1);

	if( strcmp(ktoken,"$DEFAULT") == 0 ){
		poptok( kline, nline, &jline, &ic1, &ic2, &itype );
		fstrncpy( kdef, MCMSG, kline+ic1 - 1,min(ic2,MCMSG)-ic1+1);
		nval = nline - jline + 1;
		fstrncpy( kval, MCMSG, kline+jline - 1,min(nline,MCMSG)-jline+1);
		jkeys = 0;
L_3000:
		poptok( keys, nkeys, &jkeys, &ic1, &ic2, &itype );
		if( itype > 0 ){
			if( strncasecmp(kdef,keys+ic1 - 1,ic2 - ic1 + 1) == 0 ){
        s1 = strcut(keys, ic1, ic2);
        {
          Token *t;
          t = string_to_token_list(kval);
          if(t) {
            p = upcase_dup(s1);
            sac_vars_put_var(kmcpf.kvarsname, p, VAR_LIST, t);
          }
          FREE(p);
        }
				free(s1);
				if( *nerr != 0 )
					goto L_8888;
				}
			else{
				goto L_3000;
				}
			}
		goto L_2000;
		}

	/* - Process argument line if any.  Each token here is either a keyword 
	 *   or part of the "value" of another keyword. */

	//nargs = indexb( kmacroargs,kmacroargs_s );
	//if( nargs > 0 ){
    char ckey[1000];
    char cval[10000];
    memset(ckey, 0, sizeof(ckey));
    memset(cval, 0, sizeof(cval));
		lfirst = TRUE;
    list = last = NULL;

    while((t = arg())) {
			jkeys = 0;
			poptok( keys, nkeys, &jkeys, &ick1, &ick2, &iktype );
      while(iktype > 0) {
        if(token_strncasecmp(t, keys+ick1-1, ick2-ick1+1)) { /* Keyword found */
					if( !lfirst ){
            sac_vars_put_var(kmcpf.kvarsname, ckey, VAR_LIST, list);
          }
					lfirst = FALSE;
          strcpy(ckey, t->str);
          modcase(TRUE, ckey, strlen(ckey), ckey);
          list = last = NULL;
          break;
        }
        poptok( keys, nkeys, &jkeys, &ick1, &ick2, &iktype );
      }
      /* If lastest keyword is defined and current token does not match a defined keyword */
      if(strlen(ckey) > 0 && iktype <= 0) { 
        if(last) {
          last->next = token_dup(t);
          last = last->next;
          last->next = NULL;
        } else {
          list = last = token_dup(t);
          last->next = NULL;
        }
      }
      arg_next();
    }
    if(strlen(ckey) > 0 && list) {
      sac_vars_put_var(kmcpf.kvarsname, ckey, VAR_LIST, list);
    }
    list = last = NULL;
    //  }
L_8888:

	return;

L_9000:
	*nerr = 114;
	setmsg( "ERROR", *nerr );
	apcmsg( "macro keyword preamble for",27 );
	getvvstring( kmcpf.kvarsname,9, "macroname",10, &nc, kmacroname
	 ,MCPFN+1, nerr );
	apcmsg( kmacroname,MCPFN+1 );
	goto L_8888;


} /* end of function */

