
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mach.h"
#include "gdm.h"
#include "gam.h"
#include "co.h"
#include "msg.h"
#include "bot.h"

#include "color.h"
#include "sgfcolor.h"

#include "string_utils.h"

#define NPSCIMAGE 237

/** 
 * Set the color table by name. 
 *
 * This routine must fill in both the SGF colors and the X11 colors
 *
 * @param name
 *    Name of the color table to set
 * @param directory
 *    Directory where the color table file exists
 *    Default is the SACAUX directory
 * @param netry
 *    Number of entries in the color table on output
 * @param nerr
 *    Error return flag
 *    - 0 on Success
 *    - Non-Zero on error
 *
 * Notes:
 *   - psred:     Array of red values. [ra]  
 *   - psgreen:   Array of green values. [ra]
 *   - psblue:    Array of blue values. [ra]
 *   - sgfred:     Array of red values. [ra]  
 *   - sgfgreen:   Array of green values. [ra]
 *   - sgfblue:    Array of blue values. [ra]
 *   - npscolors:  Number of entries in the color table. [i]
 *
 * @date   950306:  Original version.
 *
 */
void 
loadctable(char *name,
           char *directory,
           int *nentry,
           int *nerr)
{

	char ctable[MCPFN+1], line[MCMSG+1];
	int idx, name_s;
        FILE *nun;
        char *field;

        for( idx = 0 ; idx < MCPFN ; idx++ )
            ctable[ idx ] = ' ' ;
        ctable[ MCPFN ] = '\0' ;

	/* PROCEDURE: */

        name_s = strlen(name);

	/* - Build name of color table file. */
        /* If input directory is NULL, look for file in SACAUX */
        if((directory == NULL) || !strcmp(directory,"")){
  	  zbasename( ctable,MCPFN+1 );
 	  crname( ctable,MCPFN+1, KSUBDL, "ctables",8, nerr );
	  if( *nerr != 0 )
		goto L_8888;
	}
        else{
          strcpy(ctable,directory);
	}

	crname( ctable,MCPFN+1, KDIRDL, name,name_s+1, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Open color table file. */
        /* - If no input directory, try to open the file in the current directory */
        if((directory == NULL) || !strcmp(directory,"")){
           zopens(&nun, name, name_s, "TEXT", 5, nerr);
           if( *nerr != 0) {
        /* - Try SACAUX or input directory */
	     zopens( &nun, ctable,MCPFN+1, "TEXT",5, nerr );
	     if( *nerr != 0 )
		goto L_8888;
	   }
        }else{
	  zopens( &nun, ctable,MCPFN+1, "TEXT",5, nerr );
	  if( *nerr != 0 )
		goto L_8888;
	}

	/* - Initialize entry number counter. */

	*nentry = 0;

	/* - Read each line from disk file. */

L_1000:
        if(fgetsp( line,MCMSG+1,nun)==NULL) {
          if(feof(nun)) goto L_5000;
          goto L_9000;
        }
 
        if((field = strtok(line," \n\0\t")) == NULL){
          *nerr = 1;
          goto L_8888;
        } 
         
        sgfred[*nentry] = atol(field);
        sred[*nentry]   = sgfred[*nentry];
        psred[*nentry] = ((float)sgfred[*nentry]/255.)*65535.0;

        if((field = strtok(NULL," \n\0\t")) == NULL){
          *nerr = 2;
          goto L_8888;
        } 
         
        sgfgreen[*nentry] = atol(field);
        sgreen[*nentry]   = sgfgreen[*nentry];
        psgreen[*nentry] = ((float)sgfgreen[*nentry]/255.)*65535.0;

        if((field = strtok(NULL," \n\0\t")) == NULL){
          *nerr = 3;
          goto L_8888;
        } 
         
        sgfblue[*nentry] = atol(field);
        sblue[*nentry]   = sgfblue[*nentry];
        psblue[*nentry] = ((float)sgfblue[*nentry]/255.)*65535.0;

	/* -- Increment color table entry pointer and loop until end-of-file */
	*nentry = *nentry + 1;
        if(*nentry < NPSCIMAGE) goto L_1000;

	/* - Come to here on end-of-file */

L_5000:
	;

        if( cmgam.cmap != MDEFAULT ){
          setpsctable(nerr);
	}

	zcloses( &nun, nerr );

L_8888:
	return;

	/* - Come to here on errors during read. */

L_9000:
	*nerr = 114;
	setmsg( "ERROR", *nerr );
	apcmsg( ctable,MCPFN+1 );
	goto L_8888;

} /* end of function */

