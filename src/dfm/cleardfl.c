/** 
 * @file   cleardfl.c
 * 
 * @brief  Clear the data file list
 * 
 */

#include <string.h>

#include "clf.h"
#include "dfm.h"
#include "amf.h"
#include "debug.h"

#include "debug.h"

/** 
 * Clear pointers in the data set storage for current data sets
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   920406:  Update count of total files in SAC memory.
 * @date   911017:  Added reset to nflncds, number of files in the data-set.
 * @date   910827:  Adapted for multiple data-sets.
 * @date   910813:  Original version. Replaces part of the old cleardfl.
 *
 */
void 
cleardfl(int *nerr) {

	int jdfl, jdfl_, jcomp ;
    int i;

	*nerr = 0;
    DEBUG("files: %d\n", cmdfm.ndsflcnt);
	/* - For each file in storage */
	for( jdfl = 1; jdfl <= cmdfm.ndsflcnt; jdfl++ ){
	    jdfl_ = jdfl - 1;
        DEBUG("file: %d\n", jdfl);
	    /* -- If the data set index matches the current data set */
	    if( Ndsndx[jdfl] == 1 ){

		/* --- Release header block. */
		if( Ndxhdr[jdfl] > 0 ){
		    relamb( cmmem.sacmem, Ndxhdr[jdfl], nerr );
		    if( *nerr != 0 )
			goto L_8888;
		    Ndxhdr[jdfl] = 0;
		}

		/* --- Release SDD header block. */
		if( Nxsdd[jdfl] > 0 ){
		    relamb( cmmem.sacmem, Nxsdd[jdfl], nerr );
		    if( *nerr != 0 )
			goto L_8888;
		    Nxsdd[jdfl] = 0;
		}

		/* --- Release each data block. */
		for( jcomp = 0; jcomp < Ncomp[jdfl]; jcomp++ ){
		    if( cmdfm.ndxdta[jdfl_][jcomp] > 0 ){
			relamb( cmmem.sacmem, cmdfm.ndxdta[jdfl_][jcomp], nerr);
			if( *nerr != 0 )
			    goto L_8888;
			cmdfm.ndxdta[jdfl_][jcomp] = 0;
		    }
		}


		/* --- Reset the data set index, # of components
		 *     and data file length  
		 */
		Ndsndx[jdfl] = 0;
		Ncomp[jdfl] = 0;
		Nlndta[jdfl] = 0;

	    } 

	} 
    DEBUG("files: %d\n", cmdfm.ndfl);
    for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
        /* -- Initialize SDD header block pointer. */

		/* --- Release SDD header block. */
		if( Nxsdd[jdfl] > 0 ){
		    relamb( cmmem.sacmem, Nxsdd[jdfl], nerr );
		    if( *nerr != 0 )
			goto L_8888;
		    Nxsdd[jdfl] = 0;
		}

        /* Release Header */
        if(Ndxhdr[jdfl] > 0) {
            DEBUG("header: %d\n", jdfl);
            relamb(cmmem.sacmem, Ndxhdr[jdfl], nerr);
            if(*nerr != 0) {
                goto L_8888;
            }
            Ndxhdr[jdfl] = 0;
        }
        for(i = 0; i < Ncomp[jdfl]; i++) {
            DEBUG("data: %d\n", jdfl);
            if(cmdfm.ndxdta[jdfl-1][i] > 0) {
                relamb(cmmem.sacmem, cmdfm.ndxdta[jdfl-1][i], nerr);
                if(*nerr != 0) {
                    goto L_8888;
                }
                cmdfm.ndxdta[jdfl-1][i] = 0;
            }
        }
        Ncomp[jdfl] = 0;
    }
    
    /* - Clear DFL name storage. */
    string_list_clear(datafiles);
	/* - Zap the file count of files in memory. */
	cmdfm.ndsflcnt = 0 ;
	cmdfm.ndfl = 0;
L_8888:
    DEBUG("done: %d\n", *nerr);
	return;
}

void
clear_file(int i, int *nerr) {
  int j;
  if(Nxsdd[i] > 0) { /* SDD Header */
    relamb(cmmem.sacmem, Nxsdd[i], nerr); if(*nerr) { goto ERROR; }
    Nxsdd[i] = 0;
  }
  if(Ndxhdr[i] > 0) { /* Header Block */
    relamb(cmmem.sacmem, Ndxhdr[i], nerr); if(*nerr) { goto ERROR; }
    Ndxhdr[i] = 0;
  }
  for(j = 0; j < Ncomp[i]; j++) {
    if(cmdfm.ndxdta[i][j] > 0) {
      relamb(cmmem.sacmem, cmdfm.ndxdta[i][j], nerr); if(*nerr) { goto ERROR; }
      cmdfm.ndxdta[i][j] = 0;
    }
    Ncomp[i] = 0;
  }
  string_list_delete(datafiles, i-1);
  return;
 ERROR:
  fprintf(stderr, "Error releasing memory for file at position: %d\n", i);
  return;
}
