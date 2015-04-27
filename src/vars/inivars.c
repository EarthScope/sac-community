/** 
 * @file   inivars.c
 * 
 * @brief  Initialize the VARS storage subsystem
 * 
 */

#include <stdio.h>
#include <stdlib.h>


#include "bool.h"
#include "vars.h"
#include "co.h"
#include "debug.h"

/** 
 * Initialize the VARS storage subsystem
 * 
 * @date   881114:  Original version.
 * @date   881114:  Documented/Reviewed
 */
void 
inivars()
{
  int i;


        if((vfilelist.filelist = 
	    (struct varsfile *)malloc(NVFILELIST*sizeof(struct varsfile)))
	     != NULL )    {
           vfilelist.nallocated = NVFILELIST;
           vfilelist.nentries   = 0;
           for(i = 0; i < vfilelist.nallocated; i++) {
             vfilelist.filelist[i].varsname = NULL;
             vfilelist.filelist[i].variable = NULL;
           }
        } else {
           fprintf(stderr,"error initializing varsfile list--quitting\n");
           exit(1);
	}

  sac_vars_init();

	return;
}

void
vfilelist_free() {
  int i;
  if(vfilelist.nallocated > 0) {
    for(i = 0; i < vfilelist.nallocated; i++) {
      FREE(vfilelist.filelist[i].varsname);
      FREE(vfilelist.filelist[i].variable);
    }
    FREE(vfilelist.filelist);
  }
}
