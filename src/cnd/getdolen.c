/** 
 * @file   getdolen.c
 * 
 * @brief  Get the length of a do clause
 * 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cnd.h"
#include "mach.h"
#include "bool.h"
#include "bot.h"
#include "vars.h"
#include "co.h"
#include "msg.h"
#include "ucf.h"

#include "errors.h"

#include "string_utils.h"

/** 
 * Get the length of a "DO" clause
 * 
 * @param nlines 
 *    Number of lines in the "DO" clause
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_READING_MACRO_FILE
 *    - ERROR_SEARCHING_MACRO_FILE_FOR
 *
 * @note Local Variables
 *    numdos:  Number of dos nested in the clause.
 *    nun:     Fortran file unit used in opening command file. [i]
 *    kiline:  Input (raw) line from command file. [c]
 *    niline:  Length of kiline without trailing blanks. [i]
 *    ktemp1:  Used to store prompt sent to terminal when in
 *             interactive mode. Also used when creating prompt. [c]
 *    ktemp2:  Used when creating prompt.  Return from call to getdir
 *             is the directory and filename parts of a pathname. [c]
 *
 * @date   870817:  Original version.
 *
 */
void 
getdolen(int *nlines, 
	 int *nerr) {

	char kiline[MCMSG+1], kname[9], ktemp1[MCPFN+1], ktoken[9];
	int ic, ic1, ic2, itype, nc, niline, numdos, nchars;
        int numchar;
        FILE *nun;
        char *strtemp;

	*nerr = 0;
	numdos = 0;
	*nlines = 0;
        ic = 0;
        ic1 = 0;
        ic2 = 0;

    memset(kiline, 0, MCMSG+1);
	/* - Get the fortran file unit. */
	getclun( &nun, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Read next input line.  End-of-file or error terminates macro. */
L_1000:
	if( nun != MUNINP ){
                if(fgetsp(kiline,MCMSG,nun) == NULL) {
                  if(feof(nun)) goto L_9100;
                  goto L_9000;
		}
                if(kiline[(numchar=strlen(kiline)-1)] == '\n')kiline[numchar] = '\0';
	}
	else{
		getvvstring( kname,9, "prompt",7, &nchars, ktemp1,MCPFN+1, nerr);
		if( *nerr != 0 )
			goto L_8888;
		zgpmsg( ktemp1,MCPFN+1, kiline,MCMSG+1 );
	}
	niline = indexb( kiline,MCMSG+1 );
	*nlines = *nlines + 1;

	/* - Check for nested dos. */
	ic = 0;
	poptok( kiline, niline, &ic, &ic1, &ic2, &itype );
	nc = min( MCPW, ic2 - ic1 + 1 );

        strtemp = malloc(nc+1);
        strncpy(strtemp,kiline+ic1 - 1,nc);
        strtemp[nc] = '\0';
	modcase( TRUE, strtemp, nc, ktoken );
        free(strtemp);

	/* -- If "WHILE", increment number of nested dos  */
	if( memcmp(ktoken,"WHILE",5) == 0 )
		numdos = numdos + 1;
	if( memcmp(ktoken,"DO",2) == 0 )
		numdos = numdos + 1;
	if( memcmp(ktoken,"ENDDO",5) == 0 )
		numdos = numdos - 1;
	if( (memcmp(ktoken,"ENDDO",5) == 0) && (numdos < 0) )
		goto L_2000;

	goto L_1000;

       /* -- Backspace to Starting do. */
L_2000:
        backspace(nun,*nlines);

L_8888:
	return;

L_9000:
	*nerr = ERROR_READING_MACRO_FILE;
	setmsg( "ERROR", *nerr );
	goto L_8888;

L_9100:
	*nerr = ERROR_SEARCHING_MACRO_FILE_FOR;
	setmsg( "ERROR", *nerr );
	apcmsg( "\"enddo\"",8 );
	goto L_8888;

}

