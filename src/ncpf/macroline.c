
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "co.h"
#include "mach.h"
#include "ncpf.h"
#include "cpf.h"
#include "bool.h"

#include "string_utils.h"


#include "msg.h"
#include "wild.h"
#include "bot.h"
#include "ucf.h"
#include "vars.h"

char * process_line(char *in);

int /*FUNCTION*/ macroline(kmacroline, kmacroline_s, ncmacroline, nerr)
char *kmacroline;   int kmacroline_s;
int *ncmacroline, *nerr;
{
	char kiline[MCMSG+1], ktemp1[MCPFN+1], ktemp2[MCPFN+1],
	     ktoken[9] = "        ", runfile[MCPFN+1];
	int lrunmode, macroline_v;
	int ic, ic1, ic2, idx, itype, nc, niline, ntemp1, 
	         ntemp2, nchars, numsave;
	FILE *nrfun;
        FILE *nun;
        char *cattemp;
        char *s1, *s2;

	/*=====================================================================
	 * PURPOSE: To return the next line in a SAC macro (command) file.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    kmacroline:   Next (processed) line of text from macro file. [c]
	 *    ncmacroline:  Length of kmacroline excluding trailing blanks. [i]
	 *    nerr:         Error return flag.  0 if no error occurred. [i]
	 *=====================================================================
	 * FUNCTION VALUE:  Set to .TRUE. if a text line is returned.
	 *                  Set to .FALSE. if macro has been completely read.
	 *=====================================================================
	 * MODULE/LEVEL:  cpf/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    MCPFN, MCMSG, MCPW, MUNOUT
	 *    cpf:     kvarsname
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  getvvinteger, getvvstring, zgpmsg, poptok, modcase, 
	 *             putvvinteger, getdir, putvvstring, zrunname, zrun, 
	 *             zruntext, setmsg, apcmsg, apcmsg, outmsg
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    nun:     File descriptor used in opening macro file. [i]
	 *    kiline:  Input (raw) line from macro file. [c]
	 *    niline:  Length of kiline without trailing blanks. [i]
	 *    ktemp1:  Used to store prompt sent to terminal when in
	 *             interactive mode. Also used when creating prompt. [c]
	 *    ktemp2:  Used when creating prompt.  Return from call to getdir
	 *             is the directory and filename parts of a pathname. [c]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900129:  Now passing nesting level not macro name.
	 *    900122:  Fixed bug in $KILL option.
	 *    890106:  Now sending macro lines to message subsystem.
	 *    871110:  Fixed bug when $ keywords didn't start in column 1.
	 *    871014:  Added option to run other programs from a macro.
	 *    870915:  Added option to skip over blank and comment lines.
	 *    870514:  Changed keyword $PAUSE to $TERMINAL.
	 *    870424:  Added ability to pause, resume, and kill macro file.
	 *    870402:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  871014
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;
	lrunmode = FALSE;

  macroline_v = FALSE;

  memset(&kiline[0], ' ', sizeof(kiline));
	kiline[ MCMSG ] = '\0' ;

  memset(kmacroline, 0, kmacroline_s);

	/* - Determine vars list name and get the file descripter. */

	getvFILEptr( kmcpf.kvarsname,9, "fileunit",9, &nun, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Read next input line.  End-of-file or error terminates macro. */

L_1000:
	if( nun != MUNINP ){
	    if(fgetsp( kiline,MCMSG+1,nun)==NULL){
		if(feof(nun))goto L_9000;
		goto L_9100;
	    }
	    if(kiline[(numsave=strlen(kiline)-1)] == '\n') {
        kiline[numsave] = '\0';
      }
      if(strncasecmp(kiline, "SAC> ", 5) == 0) {
        memmove(&kiline[0], &kiline[5], sizeof(kiline)-5);
      }
	    setmsg( "MACRO", 99 );
	    apcmsg( kiline,MCMSG+1 );
	    outmsg();
	    clrmsg();
	}
	else{
	    getvvstring( kmcpf.kvarsname , 9 , "prompt" , 7 , &nchars ,
			 ktemp1 , MCPFN+1, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	    zgpmsg( ktemp1,MCPFN+1, kiline,MCMSG+1 );
	}
	niline = indexb( kiline,MCMSG+1 );

	/* - Check for macro commands to pause, resume, or kill macro execution:
	 *   (1) Pop first token and check first character for a $.
	 *   (2) If $ found, upcase rest of token and move to local storage. */

	ic = 0;
	poptok( kiline, niline, &ic, &ic1, &ic2, &itype );
	if( kiline[ic1 - 1] == '$' ){
	    nc = min( MCPW, ic2 - ic1 );
	    strncpy((s1=malloc(ic2-(ic1+1)+2)),kiline+ic1,ic2-(ic1+1)+1);
	    s1[ic2-(ic1+1)+1] = '\0';
	    modcase( TRUE, s1, nc, ktoken );
	    free(s1);
	    /* -- If "TERMINAL", save macro file unit, send rest of message
	     *    to terminal, encode macro name in prompt, and start reading
	     *    from terminal. */
	    if( memcmp(ktoken,"TERMINAL",8) == 0 ){
        putvFILEptr( kmcpf.kvarsname,9, "saveunit",9, nun, nerr );
        if( *nerr != 0 )
          goto L_8888;
        nun = MUNINP;
        putvFILEptr( kmcpf.kvarsname,9, "fileunit",9, nun, nerr );
        if( *nerr != 0 )
          goto L_8888;
        getdir( kmcpf.kvarsname,9, ktemp1,MCPFN+1, ktemp2,MCPFN+1 );
        ntemp2 = indexb( ktemp2,MCPFN+1 );
        fstrncpy( ktemp1, MCPFN, "SAC(", 4);
        fstrncpy( ktemp1+4, MCPFN-4, ktemp2, min(ntemp2,MCPFN));
        fstrncpy( ktemp1+4+min(ntemp2,MCPFN),MCPFN-4-min(ntemp2,MCPFN), ")> $", 4);
        ntemp1 = indexb( ktemp1,MCPFN+1 );
        putvvstring ( kmcpf.kvarsname, 9, "prompt", 7, ntemp1, ktemp1, MCPFN+1, nerr);
        if( *nerr != 0 )
          goto L_8888;
        if( ic < niline )
          {
            s1 = strcut(kiline, ic1, niline);
            fprintf(stdout," %s\n",s1);
            free(s1);
          }
        goto L_1000;
	    }

	    /* -- If "RESUME", restore saved file unit and read next line
		  from macro. */
	    else if( memcmp(ktoken,"RESUME",6) == 0 ){
        getvFILEptr( kmcpf.kvarsname,9, "saveunit",9, &nun, nerr );
        if( *nerr != 0 )
          goto L_8888;
        putvFILEptr( kmcpf.kvarsname,9, "fileunit",9, nun, nerr );
        if( *nerr != 0 )
          goto L_8888;
        goto L_1000;
	    }

	    /* -- If "KILL", treat like end-of-file (after restoring fileunit)*/
	    else if( memcmp(ktoken,"KILL",4) == 0 ){
        getvFILEptr( kmcpf.kvarsname,9, "saveunit",9, &nun, nerr );
        if( *nerr != 0 )
          goto L_8888;
        putvFILEptr( kmcpf.kvarsname,9, "fileunit",9, nun, nerr );
        if( *nerr != 0 )
          goto L_8888;
        goto L_9000;
	    }

	    /* -- If "RUN", initialize runfile. */
	    else if( memcmp(ktoken,"RUN",3) == 0 ){
        lrunmode = TRUE;
        poptok( kiline, niline, &ic, &ic1, &ic2, &itype );
        
        s1 = strcut(kiline, ic1, ic2);
        if(ic < niline) {
          s2 = strcut(kiline, ic, niline);
        } else {
          s2 = strdup("");
        }

        zrunname ( s1 , ic2 - ic1 + 2 , s2 , niline - ic + 2 ,
                   &nrfun , runfile , MCPFN+1 , nerr ) ;
        
        free(s1);
        free(s2);
        if( *nerr != 0 )
          goto L_8888;
        goto L_1000;
	    }

	    /* -- If "ENDRUN", execute runfile. */
	    else if( memcmp(ktoken,"ENDRUN",6) == 0 ){
        lrunmode = FALSE;
        zrun( &nrfun, runfile,MCPFN+1, nerr );
        if( *nerr != 0 )
          goto L_8888;
        goto L_1000;
	    }

	    /* -- If in runmode, process line, and write it to runfile.
	     *    This is necessary even though line begins with a $ because
	     *    it might be a macro argument or just text for the program.
	     */
	    else if( lrunmode ){
        char *out;
        if(!(out = process_line(kiline))) {
          out = strdup(kiline);
        }
        zruntext( out, strlen(out), nrfun, nerr );
        if( *nerr != 0 )
          goto L_8888;
        goto L_1000;
	    }

	    /* -- Unrecognized macro command. */
	    else{
        *nerr = 1017;
        setmsg( "ERROR", *nerr );
        cattemp = malloc(1+strlen(ktoken)+1);
        strcpy(cattemp,"$");
        strcat(cattemp,ktoken);
        apcmsg( cattemp, 1+strlen(ktoken)+1 );
        outmsg();
        free(cattemp);
        goto L_8888;
	    }
	}

	/* - If in runmode, process line, write it to runfile, and loop. */
	else if( lrunmode ){
    char *out;
    if(!(out = process_line(kiline))) {
      out = strdup(kiline);
    }
    zruntext( out, strlen(out), nrfun, nerr );
    if( *nerr != 0 )
      goto L_8888;
    goto L_1000;
	}

	/* - If blank or comment line, loop back and read another line. */
	else if( niline <= 0 || kiline[0] == '*' ){
    goto L_1000;
	}

	/* - Otherwise, process the line, expanding arguments, evaluating 
	 *   blackboard references, and evaluating header references.
	 * - Set return function value and return line to calling program. */
	else{
    strncpy(kmacroline, kiline, strlen(kiline));
    *ncmacroline = strlen(kmacroline);
    macroline_v = TRUE;
	}

L_8888:

	return( macroline_v );

	/* - Process end-of-file and read errors here. */

L_9000:
	macroline_v = FALSE;
	fstrncpy( kmacroline, kmacroline_s, " ", 1 );
	*ncmacroline = 0;
	goto L_8888;

L_9100:
	*nerr = 114;
	setmsg( "ERROR", *nerr );
	apcmsg( "macro body for",15 );
	getvvstring( kmcpf.kvarsname,9, "macroname",10, &nc, ktemp1,MCPFN+1, 
	 nerr );
	apcmsg( ktemp1,MCPFN+1 );
	macroline_v = FALSE;
	fstrncpy( kmacroline, kmacroline_s, " ", 1 );
	*ncmacroline = 0;
	goto L_8888;

} /* end of function */

