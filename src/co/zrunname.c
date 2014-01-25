/** 
 * @file   zrunname.c
 * 
 * @brief  Create a file to run a program
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "co.h"
#include "bot.h"

/** 
 * Create a file to run an external program
 * 
 * @param name 
 *    Name of the program to be run
 * @param name_s 
 *    Length of \p name
 * @param args 
 *    Arguments to the program
 * @param args_s 
 *    Length of \p args
 * @param nfun 
 *    Output File Descriptor
 * @param runfile 
 *    Output name of the run file
 * @param runfile_s 
 *    Length of \p runfile
 * @param nerr 
 *    Error Return Flag
 *   - 0 on Success
 *
 * @date   871014:  Original version.
 *
 */
void 
zrunname(char  *name, 
	 int    name_s, 
	 char  *args, 
	 int    args_s, 
	 FILE **nfun, 
	 char  *runfile, 
	 int    runfile_s, 
	 int   *nerr) {

	int ncargs, ncname ;
        char *strtemp1, *strtemp2;

	/* - Create a new file to contain the shell script. */
	ncname = indexb( name,name_s );

        memset(runfile,(int)' ',runfile_s - 1);
        runfile[runfile_s - 1] = '\0';
        memcpy(runfile,"sacrunfile",10);

	znfiles( nfun, runfile,runfile_s, "TEXT",5, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Write the first line to the file. */
	ncargs = max( 1, indexb( args,args_s ) );

        strtemp1 = malloc(ncname+1);
        strtemp2 = malloc(ncargs+1);
        strncpy(strtemp1,name,ncname);
        strtemp1[ncname] = '\0';
        strncpy(strtemp2,args,ncargs);
        strtemp2[ncargs] = '\0';

        fprintf(*nfun,"%s%c%s%s\n", strtemp1, ' ', strtemp2, " << endrun" );

        free(strtemp1);
        free(strtemp2);
L_8888:
	return;

}

