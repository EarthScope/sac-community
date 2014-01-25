/** 
 * @file   skipdo.c
 * 
 * @brief  Skip a "DO" Statement
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "cnd.h"
#include "bool.h"
#include "vars.h"
#include "bot.h"
#include "co.h"
#include "msg.h"

#include "errors.h"

#include "string_utils.h"


#include "ucf.h"

/** 
 * Skip over a clause of a "DO" Statement
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - ERROR_READING_MACRO_FILE
 *   - ERROR_SEARCHING_MACRO_FILE_FOR
 *
 * @note Local Variables
 *   - numdos:  Number of dos nested in the clause.
 *   - nun:     Fortran file unit used in opening command file. [i]
 *   - kiline:  Input (raw) line from command file. [c]
 *   - niline:  Length of kiline without trailing blanks. [i]
 *   - ktemp1:  Used to store prompt sent to terminal when in
 *             interactive mode. Also used when creating prompt. [c]
 *   - ktemp2:  Used when creating prompt.  Return from call to getdir
 *             is the directory and filename parts of a pathname. [c]
 *
 * @date   870817:  Original version.
 *
 */
void 
skipdo(int *nerr) {
	char kiline[MCMSG+1], kname[9], ktemp1[MCPFN+1], ktoken[9];
	int ic, ic1, ic2, itype, nc, niline, numdos;
        FILE *nun;
        int nchars;
        int numchar;
        char *strtemp;

	*nerr = 0;
	numdos = 0;
    memset(kiline, 0, sizeof(kiline));
    memset(kname, 0, sizeof(kname));
    memset(ktemp1, 0, sizeof(ktemp1));
    memset(ktoken, 0, sizeof(ktoken));
	/* - Get the fortran file unit. */
	getclun( &nun, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Read next input line.  End-of-file or error terminates macro. */
L_1000:
	if( nun != MUNINP ){
                if(fgetsp(kiline,MCMSG,nun) == NULL) {
                  if(feof(nun)) goto L_9100;
                  else goto L_9000;
		}
                if(kiline[(numchar=strlen(kiline)-1)] == '\n')kiline[numchar] = '\0';
	}
	else{
		getvvstring( kname,9, "prompt",7, &nchars, ktemp1,MCPFN+1, 
		 nerr );
		if( *nerr != 0 )
			goto L_8888;
		zgpmsg( ktemp1,MCPFN+1, kiline,MCMSG+1 );
	}
	niline = indexb( kiline,MCMSG+1 );

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
	if( memcmp(ktoken,"IF",2) == 0 )
		cnd.niflevel = cnd.niflevel + 1;
	if( memcmp(ktoken,"ENDIF",5) == 0 )
		cnd.niflevel = cnd.niflevel - 1;
	if( (memcmp(ktoken,"ENDDO",5) == 0) && (numdos < 0) )
		goto L_8000;

	goto L_1000;

L_8000:
	if(cnd.ndolevel > 0) cnd.ndolevel = cnd.ndolevel - 1;

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

