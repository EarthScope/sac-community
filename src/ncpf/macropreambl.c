/** 
 * @file   macropreambl.c
 * 
 * @brief  Process Macro Preamble
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ncpf.h"
#include "cpf.h"

#include "string_utils.h"


#include "co.h"
#include "msg.h"
#include "bot.h"
#include "ucf.h"
#include "vars.h"

#define	MMSGDL	2

void 
macropreamble (char *kmacroargs, 
               int   kmacroargs_s, 
               FILE *nun, 
               int  *nerr) {

	char kiline[MCMSG+1], kmacroname[MCPFN+1], ktoken[9] = "        " ;
	int lkey;
	int ic, ic1, ic2, idx, itype, nc, niline, numsave;
	static char kmsgdl[MMSGDL]={'"','\''};
        char *s1;
        char *p;

	/*=====================================================================
	 * PURPOSE: To process the preamble of a SAC macro (command) file.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    kmacroargs:   Arguments from macro execute line. [c]
	 *    nun:          Fortran file unit that macro file is open on. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error return flag.  Set to 0 if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  cpf/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    MCMSG
	 *    cpf:     kvarsname
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900322:  Fixed bug so that quoted strings are treated as
	 *             a single argument and pass through accordingly.
	 *    900206:  Changed argument list.
	 *    870915:  Added option to skip over blank and comment lines.
	 *    870402:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870402
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;
	for( idx = 0 ; idx < MCMSG ; idx++ ){
	    kiline[ idx ] = ' ' ;
	}
	kiline[ MCMSG ] = '\0' ;

	/* - Set up message delimiters. */

	tokdel( " ",2, 0, kmsgdl ,1, MMSGDL );

	/* - Read and process the keyword list card if any.
	 *   This is a line that begins with "$keys" and is used when keyword driven 
	 *   input arguments is desired.
	 *   Otherwise the arguments are referred to by their order on the line. */

L_1000:
        if(fgetsp( kiline,MCMSG+1,nun)==NULL){
          if(feof(nun))goto L_8888;
          goto L_9000;
  	}
        if(kiline[(numsave=strlen(kiline)-1)] == '\n') kiline[numsave] = '\0';

	niline = indexb( kiline,MCMSG+1 );
	if( niline <= 0 || kiline[0] == '*' )
		goto L_1000;
  p = lstrip(&kiline[0]);
  if(*p == '\0') {
    goto L_1000;
  }
	ic = 0;
	poptok( kiline, niline, &ic, &ic1, &ic2, &itype );
	nc = min( ic2 - ic1 + 1, MCPW );

        strncpy((s1=malloc(nc+1)),kiline+ic1 - 1,nc);
        s1[nc] = '\0';
	upcase( s1, nc, ktoken, 9 );
        free(s1);

	lkey = memcmp(ktoken,"$KEYS",5) == 0;

	/* - Process the preamble in either keyword or ordered mode. */

	if( lkey ){

                strncpy((s1=malloc(niline-(ic2+1)+2)),kiline+ic2, niline-(ic2 + 1) + 1);
                s1[niline-(ic2 + 1) + 1] = '\0';
		macrokeyword( kmacroargs,kmacroargs_s, nun, s1, niline - (ic2 + 1) + 2, nerr );
		free(s1);
		}
	else{
		macroordered( kmacroargs,kmacroargs_s, nun, kiline,MCMSG+1, nerr );
		}
	if( *nerr != 0 )
		goto L_8888;

	/* - Backspace one line in file so we are at the first line of the body. */

        backspace( nun, 1L );

	/* - Unset message delimiters before returning. */

L_8888:
	tokdel( " ",2, 0, " ",2, 0 );
	return;

L_9000:
	*nerr = 114;
	setmsg( "ERROR", *nerr );
	apcmsg( "macro preamble for",19 );
	getvvstring( kmcpf.kvarsname,9, "macroname",10, &nc, kmacroname
	 ,MCPFN+1, nerr );
	apcmsg( kmacroname,MCPFN+1 );
	goto L_8888;

} /* end of function */

