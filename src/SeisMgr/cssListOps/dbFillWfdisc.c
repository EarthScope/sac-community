#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include "smMemory/smMemory.h"


#define DBL_MOD_FILE TRUE
#include "cssListOps/cssListStrucs.h"
#include "timefuncs.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "dbselect/dbDefaults.h"
#include "cssListOps/dblGetDataSubs.h"
#include "cssListOps/dblErrors.h"

static char errorStrg[300];
void dbPrintToDevice(char *string);
int dblGetRealImagFloats ( int NPTS , FILE * fptr , struct wfdiscList *wfStruc );


static void CropPath(char *dir)
{
   char tmp[80];
   int cl = strlen(dbGetCropFromPath());

   if(!strncmp(dir,dbGetCropFromPath(), cl)){
      strcpy(tmp,dir+cl);
      if(tmp[0] != '/'){
         strcpy(dir,"/");
         strcat(dir,tmp);
      }
      else{
         strcpy(dir,tmp);
      }
   }
   else{
      strcpy(errorStrg, "ERROR: Expecting to crop: \n");
      strcat(errorStrg,dbGetCropFromPath());
      strcat(errorStrg,"\nfrom path:\n");
      strcat(errorStrg,dir);
      strcat(errorStrg,"\nbut could not match.\n");
      dblSetError(1, errorStrg);
   }
} 

/*----------------------------------------------------------------------*/






static void PrependToPath(char *dir)
{
   char tmp[80];
   dbGetCropFromPath();
   strcpy(tmp,dbGetPrependToPath());
   strcat(tmp,"/");
   strcat(tmp,dir);
   strcpy(dir,tmp);

} 

/*----------------------------------------------------------------------*/





static void StripDuplicateSeparators(char *strg)
{
   char *ptr;

   while( (ptr = strstr(strg, "//")) ){
      *ptr = '\0';
      ptr++;
      strcat(strg, ptr);
   }

} 

/*----------------------------------------------------------------------*/







/* This function reads the raw seismogram data from a disk file using the */
/* information stored in the input wfStruc to identify the file name, */
/* location, and data format. The data is then attached to the wfStruc. */

/* 980825 modified to work with either the Oracle database or CSS flat files. *
 * kpath1 is an absolute path to the .w file, or NULL if the path is to be    *
 * determined by the contents of wfStruc.  kpath2 is the directory where the  *
 * .wfdisc file resides.  If the .w file cannot be found in kpath1, then the  *
 * code looks in kpath2.                                                      */


int dblGetSeismograms( struct wfdiscList *wfStruc , char *kpath1 , char * kpath2 )
{
   char  * filename = NULL ,	/* name of .w file; used iff kpath1 is NULL */
	 * pType ;		/* the header element: 'datatype' from .wfdisc*/
   int stringLen = 0;		/* length of path for filename + little buffer*/
   int NPTS ,		/* number of datapoints in a file. */
	    check ,		/* gets return value from dblGet* functions. */
	    byteOffset ;	/* where to begin reading the .w file. */
   FILE   *fptr;		/* .w file */
 

   /* if the .w pathname was not passed in:
	1. determine it from wfStruc 
	2. try to open it		    */

   if ( !kpath1 ) {
   	if(dbGetCropFromPath() && strlen(dbGetCropFromPath()) ){
	    CropPath(wfStruc->element->dir);
   	}
   
   	if(dbGetPrependToPath() && strlen(dbGetPrependToPath()) ){
	    PrependToPath(wfStruc->element->dir);
   	}


   	stringLen = strlen(wfStruc->element->dir) + strlen(wfStruc->element->dfile) + 10;
   	filename = (char *) smMalloc( stringLen * sizeof(char) );
   	strcpy(filename,wfStruc->element->dir);
   	strcat(filename,"/");
   	strcat(filename,wfStruc->element->dfile);
   	StripDuplicateSeparators(filename);
   	stringLen = strlen(filename);
   	if(filename[stringLen-1] == '/') { 
      filename[stringLen-1] = '\0';
    }
	fptr = fopen(filename,"rb") ;
   }


   /* if the pathname was passed in:
	1. try to open it
	2. if it won't open, try the alternate  */

   else {
	if ( !(fptr = fopen ( kpath1 , "rb" ) ) && kpath2 )
	    fptr = fopen ( kpath2 , "rb" ) ;
   }
	

   /* if the file didn't open, bow out gracefully */
			/* filename and kpath1: one has a value, one is NULL */
   if ( !fptr ) {
	sprintf ( errorStrg , "ERROR: Could not open seismogram file %s\n" ,
		  filename ? filename : kpath1 ) ; 
	if ( filename ) smFree(filename);
	dbPrintToDevice(errorStrg);
	return FALSE;
   }

   
   
   NPTS = wfStruc->element->nsamp;	      /* get number of samples from wfStruc */
   pType = wfStruc->element->dattype ;    /* get datatype from wfStruc */
   if( *pType >= 'A' && *pType <= 'Z' ) { /* make sure the first char in ... */
      *pType += 'a' - 'A' ;               /* ... pType is lower case. */
   }
   byteOffset = wfStruc->element->foff ;/* get offset ... */
   byteOffset = byteOffset < 0 ? 0 : byteOffset ; /* don't allow negative offset */

   /* offset the file pointer by the offset in the .wfdisc file. */
   if ( fseek ( fptr , byteOffset , SEEK_SET ) ) {
	/* error handling */
        sprintf ( errorStrg , "%s\nERROR: Can't offset file by %d.\n" ,
                  filename ? filename : kpath1 , byteOffset ) ;
        dbPrintToDevice(errorStrg);
        if ( filename ) {
          smFree(filename);
        }
        fclose ( fptr ) ;
        return FALSE;
   }


   /* the following if-tree matches the datatype to the appropriate algorithm
      for reading the data.  Datatypes are discussed on page 33 of CSS 3.0
      reference manual */
   /* at one time i2, f4, i4, and f8 data would not be read on Sun's even 
      thought the code was present.  Nor could e1, s2, s3, t4, s4, and t8
      be read on PC or DEC.  This is because these lables specify a particular
      byte order.  As needed, the code will be modified to byteswap when 
      appropriate.  As of 12/3/99, the sun should be able to handle f4. */


   if ( !strncmp ( pType , "e1" , 2 ) ) /* e1 compressed data */
	check = dblGetE1 ( NPTS , fptr , wfStruc ) ;

   else if ( !strncmp ( pType , "s2" , 2 ) ||	/* Sun IEEE short int */
	     !strncmp ( pType , "i2" , 2 ) ||	/* VAX IEEE short int */
	     !strncmp ( pType , "g2" , 2 ) )	/* NORESS gain-ranged */
	check = dblGetS2I2G2 ( NPTS , fptr , wfStruc , pType ) ;

   else if ( !strncmp ( pType , "s3" , 2 ) )	/* 3 byte integer data */
	check = dblGetS3 ( NPTS , fptr , wfStruc ) ;

   else if ( !strncmp ( pType , "t4" , 2 ) ||	/* Sun IEEE floating point */
	     !strncmp ( pType , "f4" , 2 ) )	/* VAX IEEE floating point */
	check = dblGetT4F4 ( NPTS , fptr , wfStruc , pType ) ;

   else if ( !strncmp ( pType , "s4" , 2 ) ||	/* Sun IEEE integer data */
	     !strncmp ( pType , "i4" , 2 ) )	/* VAX IEEE integer data */
	check = dblGetS4I4 ( NPTS , fptr , wfStruc , pType ) ;

   else if ( !strncmp ( pType , "t8" , 2 ) ||   /* Sun IEEE double precision */
             !strncmp ( pType , "f8" , 2 ) )    /* VAX IEEE double precision */
        check = dblGetT8F8 ( NPTS , fptr , wfStruc , pType ) ;

   else if ( !strncmp ( pType , "ri" , 2 ) )    /* realfloats followed by imag floats */
        check = dblGetRealImagFloats ( NPTS , fptr , wfStruc ) ;

   else	{					/* error */
	/* handle error */
	sprintf ( errorStrg , "%s\nERROR: Unknown data type (%s).\n" ,
		  filename ? filename : kpath1 , wfStruc->element->dattype ) ;
	dbPrintToDevice(errorStrg);
	if ( filename ) smFree(filename);
	fclose ( fptr ) ;
	return FALSE;
   }

   fclose ( fptr ) ;

   if ( check == 1 ) {
	/* handle error */
	strcpy(errorStrg, "ERROR: Could not allocate data in dblGetSeismogram.\n");
	if ( filename ) smFree(filename);
	dbPrintToDevice(errorStrg);
	return FALSE;
   }
   else if ( check == 2 ) {
	sprintf ( errorStrg , "ERROR: Problem reading seismogram file %s\n" ,
		  filename ) ;
        if ( filename ) smFree(filename);
        dbPrintToDevice(errorStrg);
        return FALSE ;
   }

   if ( filename ) {
	dbPrintToDevice(filename);
   	dbPrintToDevice("\n");
   	smFree(filename);
   }

   return TRUE;
}
/*----------------------------------------------------------------------*/









