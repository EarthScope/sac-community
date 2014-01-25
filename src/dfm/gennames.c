/** 
 * @file   gennames.c
 * 
 * @brief  Generate a list of filename
 * 
 */

#include <stdio.h>
#include <string.h>

#include "dfm.h"
#include "bool.h"

#include "errors.h"

#include "clf.h"
#include "dff.h"
#include "debug.h"

/** 
 * Generate a list of filenames
 * 
 * @param headerfield 
 *    Header field to generate filenames from
 * @param lenheaderfield 
 *    Length of \p headerfield
 * @param filelist 
 *    Output list of filenames
 * @param lenfilelist 
 *    Length of \p filelist
 * @param nfiles 
 *    Number of files to generate 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_SAC_LOGIC_ERROR    
 *
 * @date   850322:  Fixed logic in checking for spectral files.
 * @date   820622:  Original version.
 *
 */
void
gennames(char *headerfield, 
         int   lenheaderfield, 
         string_list *list,
         int   nfiles, 
         int  *nerr) {

	int jdfl, ndx1, ndx2, nlen;
  char nameout[41];
  char procname[41];
  char procnametmp[100];
  char *field;
  int flength;
  int id;
	*nerr = 0;

	/* - For each file in DFL: */
	for( jdfl = 1; jdfl <= nfiles; jdfl++ ){

		/* -- Get header from memory manager. */
		getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );
		if( *nerr != 0 )
			goto L_8888;
        
        strcpy(nameout,"                                        ");
        flength = 0;
        
        formhv(headerfield,lenheaderfield,3,nameout,41,nerr);
        if(*nerr != 0) 
            goto L_8888;
        DEBUG("name: %s '%s'\n", nameout, headerfield);
        /* replace interior blanks with a . */
        if((field = strtok(nameout," ")) == NULL){
            *nerr = ERROR_SAC_LOGIC_ERROR;
            goto L_8888;
		}
        
        strcpy(procname,field);
        flength += strlen(field);
        while((field = strtok(NULL," ")) != NULL){
            procname[flength] = '.';
            strcpy(procname+flength+1,field);
            flength += strlen(field) + 1; 
		}
        
        procname[flength] = ' ';
        procname[flength+1] = '\0';
        
        flength += 1;
        
        id = 0;
        strncpy(procnametmp, procname, strlen(procname)+1);
        while(string_list_find(list, procnametmp, strlen(procnametmp)) >= 0) {
            id++;
            sprintf(procnametmp, "%s%03d", procname, id);
        }
        string_list_put(list, procnametmp, flength);
        if(*nerr != 0)
            goto L_8888;
	}

L_8888:
	return;
}
