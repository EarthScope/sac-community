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
zrunname(char *name, int name_s, char *args, int args_s, FILE ** nfun,
         char *runfile, int runfile_s, int *nerr) {

    /* - Create a new file to contain the shell script. */
    memset(runfile, (int) ' ', runfile_s - 1);
    runfile[runfile_s - 1] = '\0';
    memcpy(runfile, "sacrunfile", 10);

    znfiles(nfun, runfile, runfile_s, "TEXT", 5, nerr);
    if (*nerr != 0)
        goto L_8888;


    fprintf(*nfun, "%s %s << endrun\n", name, args);

  L_8888:
    return;

}
