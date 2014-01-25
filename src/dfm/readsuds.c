/** 
 * @file   readsuds.c
 * 
 * @brief  Read a SUDS file
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dfm.h"
#include "clf.h"
#include "cssb.h"

#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "smDataIO.h"
#include "smMemory/smMemory.h"
#include "suds/sudsIO.h"
#include "errors.h"
#include "wild.h"
#include "ssi.h"
#include "debug.h"

/** 
 * Read a File list of GSE files 
 * 
 * @param lmore 
 *    - TRUE to append to the current file list
 *    - FALSE to replace files in the list
 * @param kdirin 
 *    Default input directory
 * @param kdirin_s 
 *    Length of \p kdirin
 * @param kdflin 
 *    Input file list
 * @param kdflin_s 
 *    Length of \p kdflin
 * @param ndflin 
 *    Number of entries in \p kdflin
 * @param Verbose 
 *    Be verbose during reading
 * @param isASCII 
 *    - TRUE the files are ascii format
 *    - FALSE the files are not ascii format
 * @param MaxMem 
 *    - ?
 * @param nerr 
 *    - 0 on Success
 *    - ERROR_READING_FILE
 *
 * @bug This routine looks a lot like readcfl() and readgse(), hmmmm.
 *
 * @date   980922:  New version using SeisMgr API.
 *
 */
void 
readsuds(int lmore, 
         char *kdirin, 
         int   kdirin_s, 
         string_list *list,
         int   Verbose, 
         int   isASCII, 
         float MaxMem, 
         int  *nerr) {

   DBlist tree;
   char *file, *WorkSetName;
   int j, expand;
   string_list *files;

   int filesReturned = 0;
   int Replace       = 0;
   int MaxTraces     = MDFL;

   UNUSED(isASCII);

	*nerr = 0;

	if( !lmore ) 
    smClearDefaultTree( );

  WorkSetName = smGetDefaultWorksetName();
  
  files = wildfl(kdirin, kdirin_s, list, &expand);
  for(j = 0; j < string_list_length(files); j++) {
    file = string_list_get(files,j);
    filesReturned += sudsToCSSlist(file, WorkSetName, Replace, 
                                   MaxTraces, Verbose, MaxMem);
    tree = smGetDefaultTree();
    if(dblGetNumWaveformsInMemory(tree) >= MaxTraces)
      break;
    if(smFracPhysMemUsed() > MaxMem)
      break;
 	}
  if(Verbose)
    printf("\n");
  
  tree = smGetDefaultTree();
  if(filesReturned){
    if(Verbose)
      dblTableOfContents(tree, stdout);
	}
	else{
    printf("PROBLEM: No traces returned from SeisMgr!\n");
    *nerr = ERROR_READING_FILE;
    goto L_8888;
	}
  
  tree = smGetDefaultTree();
	cmdfm.lread = TRUE ;
  SeisMgrToSac ( tree , !lmore , nerr, Verbose , FALSE, TRUE ) ;
	cmdfm.lread = FALSE ;
  
 L_8888:
  if(files) {
    string_list_clear(files);
    files = NULL;
  }
	return;
}

