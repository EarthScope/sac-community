/** 
 * @file   zload.c
 * 
 * @brief  Load an external command
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef POSIX
#include <dlfcn.h>
#endif

#include "mach.h"
#include "co.h"
#include "bool.h"

#include "dload.h"

#include "errors.h"

/** 
 * Dynamically load an external command
 * 
 * @param kfile 
 *    File containing external command
 * @param index 
 *    Index used to access this command, Output
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_EXCEEDED_NUMEBER_OF_COMMANDS
 *    - ERROR_COMMAND_DOES_NOT_EXIST
 *
 * @note Environment variable SACSOLIST holds the location of the 
 *       external commands
 *
 * @date   920409:  Added include file dload. Moved common to dload. Moved
 *             data initialization of nfiles to initcommon block data.
 * @date   900804:  Original version.
 *
 */
void 
zload(char *kfile, 
      int  *index, 
      int  *nerr) {
#ifdef POSIX
	  
         char solist[300], *env_solist, *soname, *funcname;
         char kfiletemp[MCPFN+1];
         void *handle;
         int (*fptr)();
         int found;

	*nerr = 0;

	if((cmextcom.nfiles = cmextcom.nfiles + 1) > MEXTCOMS){
          *nerr = ERROR_EXCEEDED_NUMEBER_OF_COMMANDS;
          return;
        }

        if((env_solist = getenv("SACSOLIST")) != NULL ){
          strcpy(solist,env_solist);
	}else{
          strcpy(solist,"libsac.so");
	}


        strcpy(kfiletemp,kfile);
        funcname = strtok(kfiletemp," \t\n\0");

        soname = strtok(solist," \t\n\0");
        found = FALSE;

        while( soname != NULL ){
	  char *dumbError ;
          if((handle = dlopen(soname, RTLD_LAZY)) != NULL){
            if((fptr = (int (*)())dlsym(handle,funcname)) != NULL){
              cmextcom.extfuncs[cmextcom.nfiles-1] = fptr;
              found = TRUE;
              break;
	    }
          }          
	  dumbError = dlerror() ;
	  fprintf(stderr, "Load Error: %s\n", dumbError);
          soname = strtok(NULL," \t\n\0");
	}
        
        if( found ){
	  *index = cmextcom.nfiles;
	}else{
          *nerr = ERROR_COMMAND_DOES_NOT_EXIST;
          cmextcom.nfiles--;
          *index = -1;
	}

	return;
#else 
	fprintf(stderr, "opening of shared libraries is not supported\n");
#endif
} 

