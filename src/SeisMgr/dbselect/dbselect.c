#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define DEFINE_DB_VARS
#include "dbDefaults.h"
#include "dbselect.h" 
#undef DEFINE_DB_VARS
#include "../stringfun.h"
#include "dbErrorHandler.h"
#include "../cssListOps/dblErrors.h"
int dbSelect(char * command);


/* Private function prototypes */

/* Read a set of values from a disk file and add tokens to the current option */
static void AddValsFromFile(char *token, int tokenID,enum DATATYPE dtype);

/* Convert the current token into a float and copy to current option. */
static int ParseToReal(char *token, int tokenID);

/* Copy token into the string array of the current option */
static void ParseToString(char *token, int tokenID );

/* Function to display a help screen for using the dbselect command */
static void DispDBselectHelp(char *option, char *subOption);

/* Determine if current token is a valid option and return index if so */ 
static int TokenInOptionList(char *token, int *tokenID);

/* Parse tokens interpreting them as parameters for the current option */
static int SetOptionParams(int tokenID, int *nextTokenID);

void dbPrintToDevice(char *string);
void DisplaySpecificHelp(char *option, char *subOption) ;

/* --------------------------------------------------------------------------- */



typedef struct A{
   char *newKey;
   char *origKey;
   struct A *next;
}alias;

static alias *AliasHead = 0;

void AddAlias( char *newKey, char *origKey)
{
   int j;
   int KeyOK;
   alias *newA, *curA;

   newKey = Upstring(newKey);
   origKey = Upstring(origKey);

   if(!newKey) return;
   if(!origKey) return;

   for(j=0;j<NUMOPTIONS;j++) 
     if( !strcmp(newKey,options[j]) ){
	printf("Alias (%s) is the name of a built-in option and cannot be used.\n"
	       ,newKey);
	return;
     }
   KeyOK = 0;
   for(j=0;j<NUMOPTIONS;j++) 
     if( !strcmp(origKey,options[j]) ){
	KeyOK = 1;
	break;
     }
   if(!KeyOK){
      printf("(%s) cannot be aliased to (%s) because (%s) is not a valid option.\n"
	     ,newKey,origKey,origKey);
      return;
   }

   curA = AliasHead;
   while(curA){
     if(!strcmp( curA->newKey, newKey) ){
	printf("Alias (%s) is already in use.\n" ,newKey);
	return;
     }
     curA = curA->next;
   }

   newA = (alias *) malloc( sizeof(alias) );
   newA->newKey = (char *) malloc(strlen(newKey) + 1);
   strcpy(newA->newKey,newKey);
   newA->origKey = (char *) malloc(strlen(origKey) + 1 );
   strcpy(newA->origKey, origKey);
   newA->next = 0;

   if(!AliasHead){
      AliasHead = newA;
      return;
   }

   curA = AliasHead;
   while(curA->next)curA = curA->next;
   curA->next = newA;     
}





char *CheckForAlias(char *token)
{
   alias *curA;

   curA = AliasHead;
   while(curA){
     if(!strcmp(curA->newKey, token) ){
        token = (char *) realloc(token, strlen(curA->origKey) + 1 );
        strcpy(token,curA->origKey); 
	return token;
     }
     curA = curA->next;
   }
   return token;
}




void DisplayAliasList(void)
{
   alias *curA;
   char spaces[] = "                  ";
   char line[100];

   curA = AliasHead;
   while(curA){
     sprintf(line,"(%s)",curA->newKey);
     dbPrintToDevice(line);
     sprintf(line,"%s",spaces + strlen(curA->newKey));
     dbPrintToDevice(line);
     sprintf(line,"is an alias for     (%s)\n", curA->origKey);
     dbPrintToDevice(line);
     curA = curA->next;
   }
   dbPrintToDevice("\n");
}









/* This function reads a set of values from a disk file and associates them with
   the current option. token is expected to contain the name of the file holding
   the data, tokenID identifies the current option, and dtype controls how the
   values are interpreted (string or float ). In case of error while processing 
   the file, the function prints an error message and returns. 
*/
static void AddValsFromFile(char *token, int tokenID,enum DATATYPE dtype)
{

int ipos;
char *filename;
FILE *paramFile;
float lat,lon;
char *lineBuffer;
char **lasts;
int tokenLength;

  /* token should be of the form file=filename. 
     Make sure it fits that form, then open file and read
     either pairs of real values, or strings (1 per line)  
     to end of file. We know there is an "=" 
     somewhere in token. 
  */

   filename = strchr(token,'=')+1;
   ipos = filename - token;
   if(ipos < 5){   /* allows constructs like file=... filename=... */
      sprintf(dbErrorString,
      "ERROR: Expected string like 'file=filename', but got %s \n",token);
      strcat(dbErrorString, "In: dbErrorString");
      dblSetError(1, dbErrorString);
   }

   if( (paramFile = fopen(filename,"r")) == (FILE *) NULL ){
      sprintf(dbErrorString,
      "ERROR: Could not open file ( %s ) \n",filename);
      strcat(dbErrorString, "In: dbErrorString");
      dblSetError(1, dbErrorString);
   }
   
   if(dtype == REAL){
      while( fscanf(paramFile, "%f %f", &lat,&lon) == 2){
         numParams[tokenID]+= 2;
         floatParams[tokenID] = (float *) realloc(floatParams[tokenID], 
         numParams[tokenID] * sizeof(float) );
         floatParams[tokenID][ numParams[tokenID] - 2 ] = lat;
         floatParams[tokenID][ numParams[tokenID] - 1 ] = lon;
      }
   }
   else if(dtype == CHAR){
      lineBuffer = (char *) malloc(MAXLINELENGTH);
      while( fgetsp(lineBuffer, MAXLINELENGTH, paramFile) != NULL){
         tokenLength = strlen(lineBuffer);
         numParams[tokenID]++;
         charParams[tokenID] = (char **)  realloc(charParams[tokenID], 
         numParams[tokenID] * sizeof(char *) );
         charParams[tokenID][ numParams[tokenID] - 1 ] = (char *) 
         malloc(tokenLength * sizeof(char) + 1 );
         strcpy(charParams[tokenID][ numParams[tokenID] - 1 ],lineBuffer);
      }
   free(lineBuffer);
   }

   
   fclose(paramFile); 
}
/* --------------------------------------------------------------------------- */






/* This function converts the current token into a float and copies that float into
   the array of floats associated with the current option. It does no error
   checking on the validity of the token. 
*/
static int ParseToReal(char *token, int tokenID)
{

   float *floatVal;
   int j;
   char errString[81];

   if(numParams[tokenID] >= MaxParamNum[tokenID] && MaxParamNum[tokenID] >= 0){
      sprintf(errString,"Error getting values for option: %s.\n",options[tokenID]);
      dbPrintToDevice(errString);
      sprintf(errString,"   Only %d values may be specified.\n",numParams[tokenID]);
      dbPrintToDevice(errString);
      sprintf(errString,"   Offending token is: %s.\n",token);
      dbPrintToDevice(errString);
      dblSetError(1, "Too many tokens for option.\n");
   }
   for(j=0;j<strlen(token);j++){
     if( isalpha(token[j]) ){
      sprintf(errString,"Error getting values for option: %s.\n",options[tokenID]);
      dbPrintToDevice(errString);
      sprintf(errString,"   The token (%s) is not a valid number. \n", token);
      dbPrintToDevice(errString);
      sprintf(errString,"   You may have misspelled an option name.\n\n");
      dbPrintToDevice(errString);
      dblSetError(1, "Invalid token for option.\n");
     }
   }
   floatVal = (float *) malloc(sizeof(float));
   *floatVal = 0.0;
   sscanf(token,"%f",floatVal);
   numParams[tokenID]++;
   floatParams[tokenID] = (float *) realloc(floatParams[tokenID], 
                           numParams[tokenID] * sizeof(float) );
   floatParams[tokenID][ numParams[tokenID] - 1 ] = *floatVal;
   free(floatVal);
   return 1;
}
/* --------------------------------------------------------------------------- */




/* This function copies the value in token into the array of strings associated
     with the current option (option is identified through tokenID)
*/
static void ParseToString(char *token, int tokenID )
{

   int tokenLength;
   char errString[81];

   tokenLength = strlen(token) + 1;
   if(numParams[tokenID] >= MaxParamNum[tokenID] && MaxParamNum[tokenID] >= 0){
      sprintf(errString,"Error getting values for option: %s.\n",options[tokenID]);
      dbPrintToDevice(errString);
      sprintf(errString,"   Only %d values may be specified.\n",numParams[tokenID]);
      dbPrintToDevice(errString);
      sprintf(errString,"   Offending token is: %s.\n",token);
      dbPrintToDevice(errString);
      dblSetError(1, "Too many tokens for option.\n");
   }
   numParams[tokenID]++;
   charParams[tokenID] = (char **)  realloc(charParams[tokenID], 
                          numParams[tokenID] * sizeof(char *) );
   charParams[tokenID][ numParams[tokenID] - 1 ] = (char *) 
                        malloc(tokenLength * sizeof(char) + 1 );
   strcpy(charParams[tokenID][ numParams[tokenID] - 1 ],token);
   if(!strcmp( dbGetCaseSensitivity() , "OFF" ) )
      charParams[tokenID][ numParams[tokenID] - 1 ] = 
	         Upstring(charParams[tokenID][ numParams[tokenID] - 1 ]);

   return;
}
/* --------------------------------------------------------------------------- */






/* Function to display a help screen for using the dbselect command */
static void DispDBselectHelp(char *option, char *subOption)
{

   int j;

   if(option)
      DisplaySpecificHelp(option, subOption);
   else{
      dbPrintToDevice(
      "Usage: dbselect option value [option value option value ...]\n");
      dbPrintToDevice(
      "Each option is followed by one or more values separated by spaces or commas.\n");
      dbPrintToDevice(
      "For help on specific options type HELP optionName, e.g. HELP HELP\n\n");
      dbPrintToDevice("Available options are:\n");

      dbPrintToDevice("GENERAL:\n");

      dbPrintToDevice("   HELP            This screen\n");
      dbPrintToDevice("   MAXROWS         MaxNumberofRows\n");
      dbPrintToDevice("   SHOWSQL         Show generated SQL for all queries\n");
      dbPrintToDevice("   CMDFILE         filename of file containing select commands\n");
      dbPrintToDevice("\nDATABASE:\n");

      dbPrintToDevice("   SCHEMA          Name of schema to access\n");
      dbPrintToDevice("   CONNECT         userID/password\n");
      dbPrintToDevice("   SID             Database Service Identifier (sid)\n");
      dbPrintToDevice("   SHOWDEFAULTS    Show current Oracle defaults\n");

      dbPrintToDevice("\nSOURCE RESTRICTIONS:\n");

      dbPrintToDevice("   SRCLAT          minLat maxLat\n");
      dbPrintToDevice("   SRCLON          minLon maxLon\n");
      dbPrintToDevice("   SRCDEP          minDepth maxDepth\n");
      dbPrintToDevice("   SRCBOX          file=filename or lat lon lat lon lat lon ...\n");
      dbPrintToDevice("   SRCTYPE         one or more of qb eq me ex o l r t\n");
      dbPrintToDevice("   SRCCIRCLE       latCenter, lonCenter, radius (km)\n");

      dbPrintToDevice("   MBMAG           minMag maxMag\n");
      dbPrintToDevice("   MLMAG           minMag maxMag\n");
      dbPrintToDevice("   MSMAG           minMag maxMag\n");
      dbPrintToDevice("   EVENTID         file=filename or ID ID ID ...\n");

      dbPrintToDevice("\nSTATION RESTRICTIONS:\n");

      dbPrintToDevice("   STANAME         file=filename or name name name ...\n");
      dbPrintToDevice("   COMPONENT       VHE VHN VHZ etc.\n");
      dbPrintToDevice("   STALAT          minLat maxLat\n");
      dbPrintToDevice("   STALON          minLon maxLon\n");
      dbPrintToDevice("   STABOX          file=filename or lat lon lat lon lat lon ...\n");
      dbPrintToDevice("   INSTDEPTH       minDepth maxDepth\n");
      dbPrintToDevice("   STARTTIME       YYYY/DDD/HH:MM:SS.SSS or YYYY/MM/DD/HH:MM:SS.SSS\n");
      dbPrintToDevice("   ENDTIME         YYYY/DDD/HH:MM:SS.SSS or YYYY/MM/DD/HH:MM:SS.SSS\n");

      dbPrintToDevice("\nSOURCE-STATION RESTRICTIONS: (must have 1 station)\n");
      dbPrintToDevice("   SRCDIST         minDist maxDist (in km), minDist defaults to 0\n");
      dbPrintToDevice("   SRCDELTA        minDelt maxDelt (in degrees), minDelt defaults to 0\n");
      dbPrintToDevice("   QUADRANT        minAzimuth maxAzimuth\n");


      dbPrintToDevice("\nOTHER RESTRICTIONS:\n");

      dbPrintToDevice("   PHASELIST       Phase1 Phase2 ...\n");
      dbPrintToDevice("   AUTHLIST        auth1 auth2 ...\n");
      dbPrintToDevice("   WFID            wfid1 wfid2 ...\n");

   }
   return;
}
/* --------------------------------------------------------------------------- */





static void ShowOracleDefaults(void)
{
   char string[200];
   if(!dbGetQueryMaxRows() )dbSetQueryDefaults();
  
   sprintf(string,"Max rows to return:    %d\n",dbGetQueryMaxRows());
   dbPrintToDevice(string);
   sprintf(string,"Oracle login name:     %s\n",dbGetQueryLogin());
   dbPrintToDevice(string);
   sprintf(string,"Database service id:   %s\n",Sid());
   dbPrintToDevice(string);
   sprintf(string,"Restrict phases to:    %s\n",dbGetQueryPhaselist());
   dbPrintToDevice(string);
   sprintf(string,"Restrict authors to:   %s\n",dbGetQueryAuthlist());
   dbPrintToDevice(string);
   sprintf(string,"Crop this from path:   %s\n",dbGetCropFromPath());
   dbPrintToDevice(string);
   sprintf(string,"Prepend this to path:  %s\n",dbGetPrependToPath());
   dbPrintToDevice(string);
   sprintf(string,"Case sensitivity is:   %s\n\n",dbGetCaseSensitivity());
   dbPrintToDevice(string);
   DisplayAliasList();

   return;
}
/* --------------------------------------------------------------------------- */




/* function to determine whether the current token is one of the valid dbselect 
   options. If token is valid the index number (tokenID) is returned. 
*/
static int TokenInOptionList(char *token, int *tokenID)
{

   int j;
   char *tempstr = (char *)NULL;

   tempstr = (char *) malloc(strlen(token) + 1);
   strcpy(tempstr,token);
   tempstr = Upstring(tempstr);
   tempstr = CheckForAlias(tempstr);

   for(j=0;j<NUMOPTIONS;j++){
     if( !strcmp(tempstr,options[j]) ){
	*tokenID = j;
        optionSelected[j] = TRUE;
        free(tempstr);
	return(TRUE);
     }
   }
   free(tempstr);
   return(FALSE);
}
/* --------------------------------------------------------------------------- */






/* This function is called after a valid option has been parsed. It parses tokens 
   interpreting them as parameters for the current option until the next valid 
   dbselect option is encountered. If the parameters are illegal or insufficient, 
   it issues an error message and returns. 
*/
static int SetOptionParams(int tokenID, int *nextTokenID)
{

   int returnVal;
   char *token;
   while ((token = strtok(NULL,"        ,")) != NULL) {
     if( TokenInOptionList(token, nextTokenID) ){
        return(TRUE);
     }
     else
        switch(tokenID){
	case srclat: case srclon: case srcdep: case srcdist: case srcdelta: case mbmag: 
	case mlmag: case msmag: case stalat: case stalon: case instdepth: case quadrant: 
	case maxrows: case srccircle: case wfid:
	  if(!ParseToReal(token,tokenID)){
             printf("Error getting values for option: %s.\n",options[tokenID]);
             printf("   The token (%s) is not a valid number. \n", token);
             printf("   You may have misspelled an option name.\n\n");
	  }
           break;

        case help: case schema: case connect: case srctype: case starttime:
	case endtime: case authlist: case component: case showsql: case cmdfile: case sid:
           ParseToString( token , tokenID ) ;
           break;

	case phaselist:
           dbSetCaseSensitivity( "ON" ) ;
	   ParseToString( token , tokenID ) ;
	   dbSetCaseSensitivity( "OFF" ) ;
	   break ;
    
        case srcbox: case stabox: 
	   if(!strchr(token,'=') ){
	  if(!ParseToReal(token,tokenID)){
             printf("Error getting values for option: %s.\n",options[tokenID]);
             printf("   The token (%s) is not a valid number. \n", token);
             printf("   You may have misspelled an option name.\n\n");
	  }
	   }
	   else{
	      AddValsFromFile(token,tokenID,REAL);
	   }
	   break;

        case staname: case eventid: 
	   if(!strchr(token,'=') ){
	      ParseToString( token , tokenID );
	   }
	   else{
	      AddValsFromFile(token,tokenID,CHAR);
	   }
	   break;

	}
   }
   return(FALSE);
}
/* --------------------------------------------------------------------------- */





void dbFreeTokenList()
{
   int j,k;
   for(j=0;j<NUMOPTIONS;j++){
     if(optionSelected[j]){
        switch(j){
	case srclat: case srclon: case srcdep: case srcdist: case srcdelta: 
	case mbmag: case mlmag: case msmag: case stalat: case stalon: case instdepth: 
	case quadrant: case maxrows: case srccircle: case srcbox: case stabox: case wfid:
           free(floatParams[j]);
           floatParams[j]=0;
           numParams[j] = 0;
           optionSelected[j] = FALSE;
           continue;

        case schema: case connect: case srctype: case starttime: case endtime: case component: 
	case authlist: case phaselist: case showsql: case cmdfile: case staname: case eventid:
        case sid:
           for(k=0;k<numParams[j];k++){
              if(charParams[j][k])free(charParams[j][k]);
              charParams[j][k] = 0;
	   }
           free(charParams[j]);
           charParams[j] = 0;
           numParams[j] = 0;
           optionSelected[j] = FALSE;
           continue;
	}
     }
   optionSelected[j] = FALSE;
   numParams[j] = 0;
   }
}
/* --------------------------------------------------------------------------- */




char *LoadCommandsFromFile(char *filename)
{
   FILE *fptr;
   char * cmd;
   int j, nbytes=0;
   int c;

   if(!( fptr = fopen(filename,"r") ) ){
      sprintf(dbErrorString,"ERROR: could not open file (%s)\n",filename);
      strcat(dbErrorString, "In: LoadCommandsFromFile");
      dblSetError(1, dbErrorString);
      return 0;
   }

   while( (c = getc(fptr)) != EOF){
      if(!c)break;
      nbytes++;
   }
   if(!nbytes){
      sprintf(dbErrorString,"ERROR: empty command file (%s)\n",filename);
      strcat(dbErrorString, "In: LoadCommandsFromFile");
      dblSetError(1, dbErrorString);
      return 0;
   }

   cmd = (char *) malloc(nbytes + 1);
   rewind(fptr);
   j = 0;
   while((c = getc(fptr)) != EOF){
      if(c == '\n') c = ' ';
      cmd[j++] = c;
   }
   fclose(fptr);
   return cmd;
}
/* --------------------------------------------------------------------------- */



void InitializeOptionList(void)
{
   int j;
   for(j=0;j<NUMOPTIONS;j++){
      optionSelected[j] = FALSE;
      numParams[j] = 0;
   } 
}
/* --------------------------------------------------------------------------- */

 


/* This function parses a command line which specifies the search parameters 
   required to extract a set of waveforms from the data base. It modifies the 
   arrays optionSelected, numParams, floatParams, charParams based on the
   options specified on the command line.
*/
int dbSelect(char * command)
{

   char * token;
   char *filename, *command2;
   int j, result;
   int tokenID, nextTokenID;
   
   dbFreeTokenList();


   /* begin parsing the command line. */
   token = strtok(command," 	,;");
   if( token == (char *) NULL)return 0;
   
   if(!TokenInOptionList(token, &tokenID)){
      sprintf(dbErrorString,"ERROR: illegal token (%s)\n",token);
      strcat(dbErrorString, "In: dbSelect");
      dbFreeTokenList();
      dblSetError(1, dbErrorString);
      return 0;
   }
   if(tokenID == cmdfile){
      filename = strtok(0," 	,");
      if(!filename){
         sprintf(dbErrorString,
         "ERROR: must specify file name for option %s.\n",token);
         strcat(dbErrorString, "In: dbSelect");
         dbFreeTokenList();
         dblSetError(1, dbErrorString);
         return 0;
      }
      command2 = LoadCommandsFromFile(filename);
      if(!command2){
         sprintf(dbErrorString,
         "ERROR: could not load commands from file %s for option %s.\n",
         filename, token);
         strcat(dbErrorString, "In: dbSelect");
         dbFreeTokenList();
         dblSetError(1, dbErrorString);
         return 0;
      }
      dbFreeTokenList();
      result = dbSelect(command2);
      free(command2);
      return result;
   }


   if(tokenID == help){
      DispDBselectHelp(strtok(0," 	,"), strtok(0," 	,"));
      return 0;
   }
   if(tokenID == showdefaults){
      ShowOracleDefaults();
      return 0;
   }
   while( SetOptionParams(tokenID,&nextTokenID) ){
      tokenID = nextTokenID;
      if(tokenID == 0 /* help */){
         DispDBselectHelp(strtok(0," 	,"), strtok(0," 	,"));
         return 0;
      }
   }
   return 1;
}
/* --------------------------------------------------------------------------- */

