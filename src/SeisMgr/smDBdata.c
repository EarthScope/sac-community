#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dbselect/dbselect.h" 

#include "dbselect/dbDefaults.h"
#include "dbselect/dbBuildSQLstring.h"
#include "dbselect/dbBuildSearchlink.h"

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

int dbSelect(char * command);
int dbExecuteOracleQuery(char *SQLstring,int SkipData, DBlist tree) ;
void dbPrintToDevice(char *string);



int smLoadOracleData(int takeEvid, char *specifier,...)
{
   va_list ap;
   int j;
   int arg;
   int Mode = OverWrite;
   int SkipData = FALSE;
   char *carg;
   char *WorkSet = 0;
   char * SQLstring = 0 ;
   int linesReturned = 0;
   char results[100];
   int searchlinkFlag ;
   struct CSStree *tree, *tempTree ;
   
/* Begin by initializing error handling */
   dblClearErrorList();
   if(setjmp(dblJmpBuf) != 0){
     printf("There were %d errors. Error listing follows: \n",dblGetNumErrors());
     printf(dblGetErrorMessage() );
     linesReturned = 0;
     goto CLEANUP;
   }

/*  fill a parameter array used to build SQL string */
   if(!specifier) return FALSE;
   if(!dbSelect(specifier))return FALSE;
   if(!strlen(dbGetQueryLoginString()) )dbSetQueryDefaults();



   for(j=0;j<NUMOPTIONS;j++){

     /* Need to extract connect and sid from command line if present 
	before connecting to database */
     if(optionSelected[j] && !strcmp(options[j],"SCHEMA") ){
        dbSetSchema(charParams[j][0]);
     }

     if(optionSelected[j] && !strcmp(options[j],"CONNECT") ){
        dbSetQueryLoginString(charParams[j][0]);
     }

     if(optionSelected[j] && !strcmp(options[j],"SID") ){
        SetSid(charParams[j][0]);
     }
   }
     

   if(!strlen(Sid()))
     {
      printf("Cannot process database request because \n"
             "database service identifier (SID) is not set. \n"
             "Check your TWO_TASK environmental variable. \n");
      linesReturned = 0;
      goto CLEANUP;
     }


   ConnectToOracleSilently();   /* do this once before any oracle calls */

   searchlinkFlag = ObjectExists( dbGetSearchlinkTableName() ) ;
   if( searchlinkFlag )
      SQLstring = dbBuildSearchlink();
   else {
      printf(
       "Note: There is no named searchlink table, the search may be slowed considerably\n" ) ;
      SQLstring = dbBuildSQLstring();
   }

   if(!SQLstring) {
      DisconnectFromOracle() ;
      linesReturned = 0;
      goto CLEANUP;
   }
  
/* Then take care of getting the variable args using stdarg macros. */
   va_start(ap,specifier);
   while( (arg = va_arg(ap,int)) != NULL){
     if(arg == CreateMode){
	arg = va_arg(ap,int);
	if(arg) Mode = arg;
     }
     else if(arg == NoData){
        arg = va_arg(ap,int);
	SkipData = arg;
     }
     else if(arg == WorkSetName){
	carg = va_arg(ap,char *);
	if(carg){
	   WorkSet = (char *) malloc( strlen(carg) + 1 );
	   strcpy(WorkSet, carg);
	}
     }

   }
  
   va_end(ap);

   if(Mode == OverWrite){
      smDeleteAllWorkSets();
      smCreateEmptyWorkset(WorkSet);
   }
   else if(Mode == NewWorkSet){
      if(WorkSet)smDeleteWorksetByName(WorkSet);
      smCreateEmptyWorkset(WorkSet);
   }
   else{
     if(WorkSet){
	smChangeDefaultWorksetByName(WorkSet);
     }
     else if ( !smGetDefaultWorkset () ) {
         smChangeDefaultWorksetByIndex ( smCreateEmptyWorkset ( WorkSet ) ) ;
     }
   }
   
   tree = smMakeDefaultTree();   /* Create a Tree if necessary or just return current tree. */
   tempTree = dblNewTree();
 
   if( searchlinkFlag )
      linesReturned = dbExecuteSearchlinkQuery( SQLstring, SkipData , tempTree );
   else
      linesReturned = dbExecuteOracleQuery( SQLstring,SkipData , tempTree );

   tree = dblMergeTrees( tree, tempTree, takeEvid ) ;
   if(tree != tempTree) {
       dblDeleteTree(tempTree);
   }

   if(WorkSet)free(WorkSet);  /* free string */

   DisconnectFromOracle();    /* do this once, after all the oracle calls */

CLEANUP:
   dbFreeTokenList();
   if( SQLstring != NULL ) ;
      free(SQLstring);
   SQLstring = 0;
   if(!linesReturned){
      smDeleteDefaultTree();
   }
   sprintf(results,"Lines returned = %d\n",linesReturned);
   dbPrintToDevice(results);

   return linesReturned;
  
}
/*----------------------------------------------------------------------*/




