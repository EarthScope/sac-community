#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dbselect/dbselect.h" 

#include "dbselect/dbDefaults.h"
#include "dbselect/dbBuildSQLstring.h"

#define DB_ERROR_HANDLER
#include "dbselect/dbErrorHandler.h"
#undef DB_ERROR_HANDLER

#include "cssListOps/cssStrucs.h"
#include "cssListOps/dblUserData.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "cssListOps/dblErrors.h"
#include "dbselect/dbDefaults.h"
#include "smDataIO.h"
#include "sacIO/sacIO.h"



void dbPrintToDevice(char *string);




/* int smLoadSACData(int takeEvid, char *specifier,...) */
/* { */
/*    va_list ap; */
/*    int arg; */
/*    int Mode = OverWrite; */
/*    int SkipData = FALSE; */
/*    char *carg; */
/*    char *WorkSet = 0; */
/*    int filesReturned = 0; */
/*    char results[100]; */


/* /\* Begin by initializing error handling *\/ */
/*    dblClearErrorList(); */
/*    if(setjmp(dblJmpBuf) != 0){ */
/*      printf("There were %d errors. Error listing follows: \n",dblGetNumErrors()); */
/*      printf("%s", dblGetErrorMessage() ); */
/*      return 0; */
/*    } */


/*    if(!specifier) return 0; */
  
/* /\* Then take care of getting the variable args using stdarg macros. *\/ */
/*    va_start(ap,specifier); */
/*    while( (arg = va_arg(ap,int)) ){ */
/*      if(arg == CreateMode){ */
/* 	arg = va_arg(ap,int); */
/* 	if(arg) Mode = arg; */
/*      } */
/*      else if(arg == NoData){ */
/*         arg = va_arg(ap,int); */
/* 	SkipData = arg; */
/*      } */
/*      else if(arg == WorkSetNameArg){ */
/* 	carg = va_arg(ap,char *); */
/* 	if(carg){ */
/* 	   WorkSet = (char *) malloc( strlen(carg) + 1 ); */
/* 	   strcpy(WorkSet, carg); */
/* 	} */
/*      } */

/*    } */
  
/*    va_end(ap); */

/*    if(Mode == OverWrite){ */
/*       smDeleteAllWorkSets(); */
/*       smCreateEmptyWorkset(WorkSet); */
/*    } */
/*    else if(Mode == NewWorkSet){ */
/*       if(WorkSet)smDeleteWorksetByName(WorkSet); */
/*       smCreateEmptyWorkset(WorkSet); */
/*    } */
/*    else{ */
/*      if(!WorkSet){ */
/*         smCreateEmptyWorkset(WorkSet); */
/*      } */
/*      smChangeDefaultWorksetByName(WorkSet); */
/*    } */
/*    smMakeDefaultTree();   /\* Create a Tree if necessary or just return current tree. *\/ */
/*    filesReturned = sacLoadDataFromFiles(specifier,SkipData, smGetDefaultWorksetName(), */
/*                                         takeEvid ); */
   
/*    if(!filesReturned)smDeleteDefaultTree(); */
/*    sprintf(results,"Files returned = %d\n",filesReturned); */
/*    dbPrintToDevice(results); */

/*    return filesReturned; */
  
/* } */

/*----------------------------------------------------------------------*/







/* int smWriteSACData(char *WorkSet, ... ) */
/* { */
/*    va_list ap; */
/*    char *directory = 0;  */
/*    char **fileList = 0; */
/*    int NumFiles = 0; */
/*    int arg; */
/*    DBlist tree; */
/*    struct wfdiscList *w; */
/*    int NumFilesWritten = 0; */


/*    if(!WorkSet) return 0; */

/* /\* Initializing error handling *\/ */
/*    dblClearErrorList(); */
/*    if(setjmp(dblJmpBuf) != 0){ */
/*      printf("There were %d errors. Error listing follows: \n",dblGetNumErrors()); */
/*      printf("%s", dblGetErrorMessage() ); */
/*      return 0; */
/*    } */


/* /\* Then take care of getting the variable args using stdarg macros. *\/ */
/*    va_start(ap,WorkSet); */
/*    while( (arg = va_arg(ap,int)) ){ */
/*      if(arg == Directory){ */
/* 	directory = va_arg(ap,char *); */
/*      } */
/*      else if(arg == Filelist){ */
/*         fileList = va_arg(ap,char **); */
/*      } */
/*      else if(arg == Nfiles){ */
/* 	NumFiles = va_arg(ap,int); */
/*      } */
/*    } */
/*    va_end(ap); */


/*    smChangeDefaultWorksetByName(WorkSet); */
/*    tree = smGetDefaultTree(); */
/*    if(!tree) return 0; */

/*    w = 0; */
/*    do { */
/*       if( !(w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC) ) )break; */
/*       if( NumFilesWritten < NumFiles && fileList[NumFilesWritten] ) */
/*          sacWriteSacFile(tree, w, directory, fileList[NumFilesWritten]); */
/*       else */
/*          sacWriteSacFile(tree, w, directory, 0); */
/*       NumFilesWritten++; */
/*    }while(w); */

/*    return NumFilesWritten; */
/* } */
/*----------------------------------------------------------------------*/
