/** 
 * @file   readcfl.c
 * 
 * @brief  Read a CSS File list
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dfm.h"

#include "cssb.h"

#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "smDataIO.h"
#include "smMemory/smMemory.h"
#include "msg.h"
#include "errors.h"

#include "wild.h"
#include "co.h"
#include "clf.h"
#include "ssi.h"
#include "ncpf.h"

/** 
 * Read a File list of CSS files 
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
 *    - ERROR_OUT_OF_MEMORY
 *    - ERROR_NO_DATA_FILES_READ_IN
 *    - ERROR_READING_FILE
 *    - ERROR_SAC_LOGIC_ERROR
 *
 * @date   980922:  New version using SeisMgr API.
 *
 */
void 
readcfl(int    lmore, 
        char  *kdirin, 
        int    kdirin_s, 
        string_list *list,
        int    Verbose, 
        int    isASCII, 
        float  MaxMem, 
        int   *nerr) {

   char *kwfdisc;
   char tmpBandString[2];
   char tmpOrientString[2];
   char kstationsel[7] ;
   char kchannelsel[9] ;
   int  wflexpnd;
   int  takeEvid = TRUE ;
   int  filesReturned = 0;
   DBlist tree;

   string_list *files_out, *wfdiscs;

   /* These are arguments to cssReadFlatFiles function */
   char** Station     = 0;
   int Nstation       = 0;
   char** Channel     = 0;
   int Nchannel       = 0;
   char** Band        = 0;
   int Nband          = 0;
   char** Code        = 0;
   int NCode          = 0;
   char** Orientation = 0;
   int Norientation   = 0;
   char** Author      = 0;
   int Nauthor        = 0;
   char** Filelist    = 0;
   int Nfiles         = 0;
   char** Phaselist   = 0;
   int Nphases        = 0;
   int Replace        = 0;
   char *WorkSetName;
   int MaxTraces      = MDFL;
   int i, j;

	*nerr = 0;


	if( lmore ) {
	   if( !cmdfm.ltrust )
		cmdfm.nreadflag = LOW ;
	   else if( cmdfm.nreadflag == RDB )
		cmdfm.nreadflag = HIGH ;
	}
	else {
	   cmdfm.nreadflag = HIGH ;
           smClearDefaultTree( );
	}
  files_out = NULL;
  wfdiscs = NULL;
    
        WorkSetName = smGetDefaultWorksetName();

        if(isASCII){
           /* pull the wfdisc file names out of the input file list--store in
              wfdiscroots and remove them from input file list (kdflin)
              wfdiscroots should be a char * and a blank delimited string of
              names will be returned (allocated in this routine) */
            wfdiscs = getwfdiscs(list, nerr);
            if( *nerr != 0 ) 
                goto L_8888;
     
           /* Process wildcards if any in wfdisc name(s). Process wfdiscroots
              through wildfl, returning wfdisclist and notfound. Allocate some
              temporary memory to get the returned file list in */

           files_out = wildfl(kdirin, kdirin_s, wfdiscs, &wflexpnd);

           if(string_list_length(files_out) <= 0){
              printf("No wfdisc file(s) found in the specified directory\n");
              *nerr = ERROR_NO_DATA_FILES_READ_IN;
              goto L_8888;
    	   }
        }

        /* set up to read file list from .wfdiscs selecting on stationsel, bandwsel,
           and orientsel. At this point Directory to read from is in (NULL-terminated)
           wfdiscdir */
        if( kmdfm.lstation ){
            strcpy ( kstationsel , kmdfm.kstation ) ;
            if(strcmp(kstationsel, "*") )  /* Null Station acts as * */
                Station = TokenLineToStrgList(kstationsel, &Nstation, 0);
        }

        if( kmdfm.lchannel ){   /* channel selection.  maf 970403 */
            strcpy ( kchannelsel , kmdfm.kchannel ) ;
            if(strcmp(kchannelsel, "*") )
               Channel = TokenLineToStrgList(kchannelsel, &Nchannel, 0);
        }
        
        if( kmdfm.lbandw ){
            tmpBandString[0] = toupper(kmdfm.kbandw[0]);
            tmpBandString[1] = '\0';
            if(kmdfm.kbandw[0] != '*') 
                Band = TokenLineToStrgList(tmpBandString, &Nband, 0);
        }

        if( kmdfm.lorient ){
            tmpOrientString[0] = toupper(kmdfm.korient[0]);
            tmpOrientString[1] = '\0';
            if(kmdfm.korient[0] != '*')
                Orientation = TokenLineToStrgList(tmpOrientString,
                                                  &Norientation, 0);
        }

        /* determine value of takeEvid */
        if( cmdfm.nreadflag != LOW && cmdfm.ltrust == TRUE )
            takeEvid = TRUE ;
        else
            takeEvid = FALSE ;

        

        if(isASCII){

           for( i = 0; i < string_list_length(files_out); i++) {
               kwfdisc = string_list_get(files_out, i);

               filesReturned += cssReadFlatFiles(kwfdisc,     WorkSetName,
                                                 Replace,     MaxTraces,  
                                                 Station,     Nstation, 
                                                 Channel,     Nchannel, 
                                                 Band,        Nband, 
                                                 Code,        NCode, 
                                                 Orientation, Norientation, 
                                                 Author,      Nauthor,
                                                 Filelist,    Nfiles, 
                                                 Phaselist,   Nphases,
                                                 Verbose,     MaxMem,
                                                 &takeEvid );
               
               if( !takeEvid ) {
                   cmdfm.nreadflag = LOW ;
               }
           }
           FreeStringArray(Station,     Nstation);
           FreeStringArray(Channel,     Nchannel);
           FreeStringArray(Band,        Nband);
           FreeStringArray(Code,        Nchannel);
           FreeStringArray(Orientation, Norientation);
           FreeStringArray(Author,      Nauthor);
           FreeStringArray(Filelist,    Nfiles);
           FreeStringArray(Phaselist,   Nphases);
        } else{
            files_out = wildfl(kdirin, kdirin_s, list, &wflexpnd);
            for(j = 0; j < string_list_length(files_out); j++) {
                kwfdisc = string_list_get(files_out, j);
                filesReturned += ReadCSSBfile(kwfdisc, WorkSetName,
                                              Replace, MaxTraces, Verbose,
                                              MaxMem, takeEvid );
                tree = smGetDefaultTree();
                if(dblGetNumWaveformsInMemory(tree) >= MaxTraces)
                    break;
                if(smFracPhysMemUsed() > MaxMem)
                    break;
            }
            if(Verbose) {
                printf("\n");
            }
        }
        
        tree = smGetDefaultTree();
        if(filesReturned){
           if(Verbose)
              dblTableOfContents(tree, stdout);
        } else {
            //printf("PROBLEM: No traces returned from SeisMgr!\n");
            *nerr = ERROR_READING_FILE;
            error(*nerr, ": No traces returned from SeisMgr");
            goto L_8888;
        }
        
        tree = smGetDefaultTree();
        cmdfm.lread = TRUE ;
        SeisMgrToSac ( tree , !lmore , nerr, Verbose , TRUE, TRUE ) ;
        cmdfm.lread = FALSE ;
        
        
 L_8888:
        if(files_out) {
          string_list_free(files_out);
          files_out = NULL;
        }
        return;
}

