/** 
 * @file   ophelp.c
 * 
 * @brief  Open a help package
 * 
 */

#include <stdio.h>

#include "mach.h"
#include "ucf.h"

#include "string_utils.h"

#include "co.h"
#include "bot.h"
#include "debug.h"

/** 
 * Open a help package based on its name
 * 
 * @param kitem 
 *    Name of help package
 * @param kitem_s 
 *    Length of \p kitem
 * @param nun 
 *    Fortran file unit which was opened
 * @param khfile 
 *    Name of a help package file for an item
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   870923:  Deleted ".saf" from aux file names.
 *                  Changed location of "contents" file.
 *                  Changed error return logic.
 * @date   831110:  Generalized generation of pathname for HELP list.
 * @date   820310:  Fixed minor bug involving auxiliary help packages.
 * @date   800915:  Original version.
 * @date   810115:  Major revision for new help package format.
 * @date   810306:  Modified call to ZOPEN to open files read only.
 * @date   810316:  Added indexed search to find correct help file.
 *
 */
void 
ophelp(char  *kitem, 
       int    kitem_s, 
       FILE **nun, 
       char  *khfile, 
       int   *nerr) {

	char khitem[30], khname[100], klfile[MCPFN+1];
    char kiline[MCMSG+1];
	int ncerr, idx;
  UNUSED(kitem_s);

	*nerr = 0;
    memset(khname, 0, sizeof(khname));
        for( idx = 0 ; idx < MCPFN ; idx++ )
            klfile[ idx ] = ' ' ;
        klfile[ MCPFN ] = '\0' ;

	/* - Create pathname and open file that contains list of help packages. */

	zbasename( klfile,MCPFN+1 );
	crname( klfile,MCPFN+1, KSUBDL, "help",5, nerr );
	if( *nerr != 0 )
		goto L_8888;
	crname( klfile,MCPFN+1, KDIRDL, "contents",9, nerr );
	if( *nerr != 0 )
		goto L_8888;
	zopens( nun, klfile,MCPFN+1, "ROTEXT",7, nerr );
	if( *nerr != 0 ){
		goto L_8000;
		}

	/* - For each item in help list file: */

L_1000:
        if(fgetsp(kiline,MCMSG+1,*nun)==NULL) goto L_8000;
        if(sscanf(kiline," %s %s", khitem, khname) != 2){
          goto L_8000;
	}

	/* -- If it matches the requested help package name, close
	 *    the help list file, open the help package, and return. */

        
	if( strcmp(kitem,khitem) == 0 ){
		zcloses( nun, nerr );
		zbasename( khfile,MCPFN+1 );
		crname( khfile,MCPFN+1, KSUBDL, "help",5, nerr );
		if( *nerr != 0 )
			goto L_8888;
		crname( khfile,MCPFN+1, KDIRDL, khname,30, nerr );
		if( *nerr != 0 )
			goto L_8888;
		zopens( nun, khfile,MCPFN+1, "ROTEXT",7, nerr );
		goto L_8888;
		}

	/* -- If not loop back for next item in help list file. */

	goto L_1000;

	/* - Set error flag if no help file was found. */

L_8000:
	zcloses( nun, &ncerr );
	*nun = NULL ;

L_8888:
	return;

}

