/** 
 * @file   makeuniq.c
 * 
 * @brief  Make files in a list unique
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dfm.h"
#include "bool.h"

#include "errors.h"


#include "clf.h"
#include "bot.h"

/** 
 * Make sure all the names in a filelist are unique
 * 
 * @param filelist 
 *    Input and Output filelist
 * @param lenfilelist 
 *    Length of \p filelit
 * @param nfiles 
 *    Number of files in \p filelist
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - 
 */
void 
makeuniq(char *filelist,
	 int   lenfilelist,
	 int   nfiles,
	 int  *nerr) {

	int ic1, ic2, jdfl;
        char *filename = NULL;
        char *buf1     = NULL;
        char *buf2     = NULL;
        char *listptr;
        char char1, char2;
        int lunique = FALSE;
        int lenfilename, bufcount, numchars;
        int counter;
        char *field, *temp;
        int lenlist;
	/*=====================================================================
	 * PURPOSE:  To make sure the all the names in a filelist are unique.
	 *         
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error return flag.  Set to zero if no error occurs.
	 *         
	 *         
	 *=====================================================================
	 * MODULE/LEVEL:  DFM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *    HDR:     IFTYPE, ITIME, IXY, LEVEN
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  GTOUTM
	 *===================================================================== */
	/* PROCEDURE: */

      *nerr = 0;

      if((filename = (char *)malloc(lenfilelist+1)) == NULL){
        *nerr = ERROR_OUT_OF_MEMORY;
        goto L_8888;
      }

      if((buf1 = (char *)malloc(lenfilelist+1)) == NULL){
        *nerr = ERROR_OUT_OF_MEMORY;
        goto L_8888;
      }

      if((buf2 = (char *)malloc(lenfilelist+1)) == NULL){
        *nerr = ERROR_OUT_OF_MEMORY;
        goto L_8888;
      }

      /* get the actual length of the list */
      lenlist = indexb(filelist,lenfilelist);
      if( lenlist == lenfilelist ){
	/* this is an error because we need room enough for one blank at the 
	 * end of the list for lnumcl to work properly. 
	 */
        *nerr = ERROR_SAC_LOGIC_ERROR;
        goto L_8888;
      }

      /* increment the length so we copy one blank past the last entry */
      lenlist++;

      strncpy(buf1,filelist,lenlist);
      buf1[lenlist] = '\0';

      strcpy(buf2,buf1);

      counter = 0;

      while(!lunique){
        lunique = TRUE;
        counter++;

	/* - For each file in DFL: */

	for( jdfl = 1; jdfl <= nfiles; jdfl++ ){
          memset(filename,' ',lenfilelist);
          filename[lenfilelist] = '\0';

          lnumcl(buf1,lenfilelist, jdfl, &ic1, &ic2 );

          strncpy(filename,buf1+ic1-1,ic2-ic1+1);
          filename[ic2-ic1+1] = '\0';
          lenfilename = ic2-ic1+1;

          if((listptr = strstr(buf1,filename)) == NULL){
            *nerr = ERROR_SAC_LOGIC_ERROR;
            goto L_8888;
	  }

          listptr += lenfilename;

          bufcount = listptr-buf1;
          strncpy(buf2,buf1,bufcount);


          char1 = '0';
          char2 = '1';

          /* look for identical filenames in the list */
          while((field = strstr(listptr,filename)) != NULL){
              numchars = field-listptr+lenfilename; 
              strncpy(buf2+bufcount,listptr,numchars);

              bufcount += numchars;
              listptr +=  numchars;
              if(field[lenfilename] == ' '){
                lunique = FALSE;
                buf2[bufcount] = char1;
                buf2[bufcount+1] = char2;
                bufcount += 2;
                if( char2 == '9' ) char2 = '0';
                else char2++;
                if( char2 == '0' ){
                  if( char1 == '9' ){
                    printf("too many output files-makeuniq\n");
                    *nerr = ERROR_SAC_LOGIC_ERROR;
                    goto L_8888;
		  }else{
                    char1++;
	          } 
	        } 
	      }
	  } 
         
          strcpy(buf2+bufcount,listptr);

          temp = buf1;
          buf1 = buf2;
          buf2 = temp;
        } 
      } 


L_8888:
        
        strcpy(filelist,buf1);

        if(filename != NULL) free(filename);
        if(buf1     != NULL) free(buf1);
        if(buf2     != NULL) free(buf2);
   
	return;
}

