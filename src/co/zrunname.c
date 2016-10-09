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
#include "debug.h"
#include "errors.h"

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
    UNUSED(args_s);
    UNUSED(name_s);

    strlcpy(runfile, "sacrunfile", runfile_s);

    if((*nfun = fopen(runfile, "wb")) == NULL) {
        *nerr = ERROR_OPENING_FILE;
        goto L_8888;
    }

    fprintf(*nfun, "%s %s << endrun\n", name, args);

  L_8888:
    return;

}
