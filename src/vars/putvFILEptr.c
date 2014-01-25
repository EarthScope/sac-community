/** 
 * @file   putvFILEptr.c
 * 
 * @brief  Store a vars File pointer
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "debug.h"

/** 
 * Store a vars FILE poiner
 * 
 * @param vars 
 *    Name of vars list
 * @param vars_s 
 *    Length of \p vars
 * @param name 
 *    Name of vars entry
 * @param name_s 
 *    Length of \p name
 * @param value 
 *    FILE pointer to store
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @bug This routine returns an error code of 1, an undefined error code
 *
 * @date   121493:  Original version.
 *
 */
void 
putvFILEptr(char *vars, 
	    int   vars_s, 
	    char *name, 
	    int   name_s, 
	    FILE *value, 
	    int  *nerr)
{
	int i;
    struct varsfile *tempptr;
    struct varsfile *flist;
  UNUSED(vars_s);
  UNUSED(name_s);

	*nerr = 0;
        flist = vfilelist.filelist;

	/* search for existing entry to replace */
        for (i=0; i<vfilelist.nentries; i++, flist++){
          if(strcmp(flist->varsname,vars) == 0) {
            if(strcmp(flist->variable,name) == 0) {
               /* replace existing entry */
              flist->value = value;
              goto L_8888;
            }
          }
        }

	/* is there room for another entry? */
        if((vfilelist.nentries + 1) > vfilelist.nallocated) {
           tempptr = realloc(vfilelist.filelist,
                             (vfilelist.nentries+NVFILEINC)*sizeof(struct varsfile));
           if(tempptr==NULL){
             *nerr = 1;
             goto L_8888;
	   }
           else {
             vfilelist.filelist = (struct varsfile *)tempptr;
             vfilelist.nallocated += NVFILEINC;
           }           
        }

	/* store the new entry */
        flist = vfilelist.filelist + vfilelist.nentries;
        flist->varsname = strdup(vars);
        flist->variable = strdup(name);
        flist->value = value;
        vfilelist.nentries += 1;

L_8888:
	return;
} 

