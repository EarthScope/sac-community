
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mach.h"
#include "gdm.h"
#include "co.h"
#include "msg.h"
#include "bot.h"
#include "ucf.h"

#include "string_utils.h"
#include "debug.h"
/** 
 * Set the color table by name
 * 
 * @param name
 *    Name of the color table to set
 *     - 'default' for default color table.
 *     - 'grays' for a gray-scale table.
 *     - 'rainbow' for an interesting color table.
 * @param name_s
 *    Length of \p name
 * @param max_
 *    Maximum length of color arrays passed
 * @param red
 *    Array of red values on output
 * @param green
 *    Array of green values on output
 * @param blue
 *    Array of blue values on output
 * @param cnames
 *    Array of color names on output
 * @param cnames_s
 *    Length of \p cnames
 * @param nentry
 *    Number of entries in the color table
 * @param nerr
 *    Error return flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   970129:  Add parameter (0) to cnvatf.  0 means that if a string
 *                  of digits is too long, let it slide by.  maf 
 * @date   870923:  Deleted ".saf" from aux file names.
 * @date   861020:  Original version.

 */
void 
readctable(char *name,
           int name_s,
           int max_,
           float red[],
           float green[],
           float blue[],
           char *cnames,
           int cnames_s,
           int *nentry,
           int *nerr)
{

#define CNAMES(I_,J_)	(cnames+(I_)*(cnames_s)+(J_))

	char ctable[MCPFN+1], line[MCMSG+1];
	int  idx ;
	int ic, ic1, ic2, itype, nc, numsave;
  FILE *nun;
	float bluev, greenv, redv;
  char *s1;
  UNUSED(max_);
        for( idx = 0 ; idx < MCPFN ; idx++ )
            ctable[ idx ] = ' ' ;
        ctable[ MCPFN ] = '\0' ;

	/* - Build name of color table file. */
	zbasename( ctable,MCPFN+1 );
	crname( ctable,MCPFN+1, KSUBDL, "ctables",8, nerr );
	if( *nerr != 0 )
		goto L_8888;
	crname( ctable,MCPFN+1, KDIRDL, name,name_s, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Open color table file. */

	zopens( &nun, ctable,MCPFN+1, "TEXT",5, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Initialize entry number counter. */

	*nentry = 0;

	/* - Read each line from disk file. */

L_1000:
        if(fgetsp( line,MCMSG+1,nun)==NULL) {
          if(feof(nun)) goto L_5000;
          goto L_9000;
        }
        if(line[(numsave=strlen(line)-1)] == '\n') line[numsave] = ' ';

	nc = numsave + 1;;
	ic = 0;

	/* -- Pop first three tokens.  They are the red, green, and blue values. */
	poptok( line, nc, &ic, &ic1, &ic2, &itype );
	if( itype == 1 ){
                strncpy((s1=malloc(ic2-ic1+2)),line+ic1 - 1,ic2-ic1+1);
                s1[ic2-ic1+1] = '\0';
		cnvatf( s1, ic2-ic1 + 2, &redv, 0, nerr ); /* add 0 before nerr. maf 970129 */
		free(s1);
		}
	else{
		*nerr = 2201;
		setmsg( "ERROR", *nerr );
		aplmsg( line, MCMSG+1 );
		}
	if( *nerr != 0 )
		goto L_8888;

	poptok( line, nc, &ic, &ic1, &ic2, &itype );
	if( itype == 1 ){
                strncpy((s1=malloc(ic2-ic1+2)),line+ic1 - 1,ic2-ic1+1);
                s1[ic2-ic1+1] = '\0';
		cnvatf( s1, ic2-ic1 + 2, &greenv, 0, nerr ); /* add 0 before nerr. maf 970129 */
		free(s1);
		}
	else{
		*nerr = 2201;
		setmsg( "ERROR", *nerr );
		aplmsg( line, nc);
		}
	if( *nerr != 0 )
		goto L_8888;

	poptok( line, nc, &ic, &ic1, &ic2, &itype );
	if( itype == 1 ){
                strncpy((s1=malloc(ic2-ic1+2)),line+ic1 - 1,ic2-ic1+1);
                s1[ic2-ic1+1] = '\0';
		cnvatf( s1, ic2- ic1 + 2, &bluev, 0, nerr ); /* add 0 before nerr. maf 970129 */
		free(s1);
		}
	else{
		*nerr = 2201;
		setmsg( "ERROR", *nerr );
		aplmsg( line, nc);
		}
	if( *nerr != 0 )
		goto L_8888;

	/* -- Store values in color table if everything is okay. */
	red[*nentry] = redv;
	green[*nentry] = greenv;
	blue[*nentry] = bluev;

	/* -- Pop next token.  It is an optional color name. */
	poptok( line, nc, &ic, &ic1, &ic2, &itype );
	if( itype == 1 ){
		fstrncpy( CNAMES(*nentry,0), cnames_s-1, line+ic1 - 1,min(ic2,MCMSG) - 
		 ic1 + 1);
		}
	else{
		fstrncpy( CNAMES(*nentry,0), cnames_s-1, "UNKNOWN", 7 );
		}

	/* -- Increment color table entry pointer and loop until end-of-file */
	*nentry = *nentry + 1;
	goto L_1000;

	/* - Come to here on end-of-file */

L_5000:
	;

L_8888:
	zcloses( &nun, nerr );
	return;

	/* - Come to here on errors during read. */

L_9000:
	*nerr = 114;
	setmsg( "ERROR", *nerr );
	apcmsg( ctable,MCPFN+1 );
	goto L_8888;

#undef	CNAMES
}

