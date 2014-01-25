
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../stringfun.h"
#include "dbDefaults.h"

static int MaxRowsInQuery;
static char ConnectString[123]      = "";
static char logname[50]             = "";
static char passwd[50]              = "";
static char sid[20]                 = "";
static char Schema[50]              = "";
static char dbname[]                = "sacdb"; /* Actual name is arbitrary. Used by precompiler as an alias for the connection. */
static char Phaselist[200]          = "";
static char Authlist[200]           = "";
static int  ShowSQL                 = 0;
static char CropFromPath[80]        = "";
static char PrependToPath[80]       = "";
static char CaseSensitivity[80]     = "ON";
static char wfdiscTableName[80]     = "WFDISC";
static char wftagTableName[80]      = "WFTAG";
static char originTableName[80]     = "ORIGIN";
static char eventTableName[80]      = "EVENT";
static char arrivalTableName[80]    = "ARRIVAL";
static char assocTableName[80]      = "ASSOC";
static char siteTableName[80]       = "SITE";
static char sitechanTableName[80]   = "SITECHAN";
static char searchlinkTableName[80] = "SEARCH_LINK";
static char instrumentTableName[80] = "INSTRUMENT";
static char sensorTableName[80]     = "SENSOR";



void dbPrintToDevice(char *string);
void AddAlias( char *newKey, char *origKey);


const char *dbName(void)
{  
   return dbname;
}
/* ------------------------------------------------------------------ */


const char *Sid(void)
{  
   return sid;
}
/* ------------------------------------------------------------------ */

void SetSid(const char *s)
{
  if(s && strlen(s)) strcpy(sid,s);
}
/* ------------------------------------------------------------------ */


char  *dbGetQueryLogin(void)
{
   return logname;
}
/* ------------------------------------------------------------------ */



char  *dbGetQueryPasswd(void)
{
   return passwd;
}
/* ------------------------------------------------------------------ */

void dbSetSchema( const char* sc )
{
   if( sc && strlen( sc ) )
      strcpy( Schema, sc );
}
/* ------------------------------------------------------------------ */






void dbSetQueryDefaults(void)
{

   char *ConFigFileName;

   char *defsid = getenv( "TWO_TASK" );
   if( defsid ) strcpy(sid,defsid);

  /* First set some defaults to use if no config file is found */
   MaxRowsInQuery = 100;
   ShowSQL = 0;
   cuserid(logname);
   strcpy(passwd,logname);
   strcpy(Phaselist,"");
   strcpy(Authlist,"");
   strcpy(ConnectString,logname);
   strcat(ConnectString,"/");
   strcat(ConnectString,passwd);
   strcpy(CropFromPath, "");
   strcpy(PrependToPath, "");
   strcpy(CaseSensitivity, "ON");
   strcpy( wfdiscTableName     , "WFDISC" ) ;
   strcpy( wftagTableName      , "WFTAG" ) ;
   strcpy( originTableName     , "ORIGIN" ) ;
   strcpy( eventTableName      , "EVENT" ) ;
   strcpy( arrivalTableName    , "ARRIVAL" ) ;
   strcpy( assocTableName      , "ASSOC" ) ;
   strcpy( siteTableName       , "SITE" ) ;
   strcpy( sitechanTableName   , "SITECHAN" ) ;
   strcpy( searchlinkTableName , "SEARCH_LINK" ) ;
   strcpy( instrumentTableName , "INSTRUMENT" ) ;
   strcpy( sensorTableName     , "SENSOR" ) ;

   /* Now see if there is a configuration file specified in environment */

   if( ( ConFigFileName = getenv("DBSELECT_CONFIG_FILE") ) ){
      dbUpdateDefaultsFromFile(ConFigFileName);
   }

   /* If user did not specify schema name, try to make the schema name
      same as the user's name. */
   if( !strlen( Schema ) && strlen( logname ) )
      strcpy( Schema, logname );

}
/* ------------------------------------------------------------------ */




static void DeBlank(char *buffer)
{
   char *tmp;
   char * c;
   if(!(c = strchr(buffer,' ')) )return;
   tmp = c+1;
   *c = 0;
   strcat(buffer,tmp);
   DeBlank(buffer);
}
/* ------------------------------------------------------------------ */






int dbGetQueryMaxRows(void)
{
   return MaxRowsInQuery;
}
/* ------------------------------------------------------------------ */




void dbSetQueryMaxRows(int N)
{
   if(N > 0)MaxRowsInQuery = N;
}
/* ------------------------------------------------------------------ */




int dbGetShowSQL(void)
{
   return ShowSQL;
}
/* ------------------------------------------------------------------ */




void dbSetShowSQL(const char *state)
{
   if(!strcmp(state, "ON") || !strcmp(state,"on"))
      ShowSQL = 1;
   else
      ShowSQL = 0;
}
/* ------------------------------------------------------------------ */





char * dbGetQueryLoginString(void)
{
   return ConnectString;
}
/* ------------------------------------------------------------------ */



void dbSetQueryLoginString(const char * string)
{
   char * charptr;

   /* Step 1.  Copy string into ConnectString 
      Check for @ sign 
       If present 
            check that length of sid > 0
            If sid ok copy to internal sid string, else print error and return. 
	    Terminate ConnectString at @ sign
       end 
   */

   if(!string) 
     {
       printf("Null string passed to dbSetQueryLoginString.\n");
       return;
     }

   if(strlen(string) < 3) 
     {
       printf("String passed to dbSetQueryLoginString is too short.\n");
       return;
     }

   strcpy(ConnectString,string);



   /* Step 2.  Check for /  
       If not present print error and return
       If present, 
           check length > 0  
           copy from posit of / + 1  into passwd
           null term at posit of /  
       end
   */

   charptr = strrchr(ConnectString, '/');
   if (charptr) 
     {
     int length = strlen(charptr + 1);   
     if (length < 1) 
       {
	 printf("Improperly formatted connect string.\n");
	 printf("/ sign present without an Password. \n");
	 return;
       }
     else
       {
	 strcpy(passwd,charptr+1);
	 *charptr = '\0';
       }
     }
   else
     {
	 printf("Improperly formatted connect string.\n");
	 printf("No / sign present in connect string. \n");
	 return;
     }


   /* Stemp 3.  Check that length of string > 0
        If not print error and return 0
        If OK, 
           copy to username
           copy string back to ConnectString
        end
   */
   if (strlen(ConnectString)<1)
     {
       printf("Improperly formatted connect string.\n");
       printf("No login name present in connect string. \n");
       return;
     }
   else
     {
       strcpy(logname, ConnectString);
       strcpy(ConnectString,string);
     }

}

/* ------------------------------------------------------------------ */



void dbSetQueryPhaselist(char *string)
{
   if(!string) return;
   if(strlen(string) < 1) return;
   strcpy(Phaselist,string);
}
/* ------------------------------------------------------------------ */




char * dbGetQueryPhaselist(void)
{
   return Phaselist;
}
/* ------------------------------------------------------------------ */






void dbSetQueryAuthlist(char *string)
{
   if(!string) return;
   if(strlen(string) < 1) return;
   strcpy(Authlist,string);
}
/* ------------------------------------------------------------------ */




char * dbGetQueryAuthlist(void)
{
   return Authlist;
}
/* ------------------------------------------------------------------ */





void dbSetCropFromPath(char *string)
{
   if(!string) return;
   if(strlen(string) < 1) return;
   strcpy(CropFromPath,string);
}
/* ------------------------------------------------------------------ */




char * dbGetCropFromPath(void)
{
   return CropFromPath;
}
/* ------------------------------------------------------------------ */





void dbSetPrependToPath(char *string)
{
   if(!string) return;
   if(strlen(string) < 1) return;
   strcpy(PrependToPath,string);
}
/* ------------------------------------------------------------------ */




char * dbGetPrependToPath(void)
{
   return PrependToPath;
}
/* ------------------------------------------------------------------ */


static int NameHasSchema( const char* name )
{
   return strchr( name , '.' ) != 0;
}
/* ------------------------------------------------------------------ */






static char* FullyQualified( const char* TableName, char* fullname )
{
   if( NameHasSchema(TableName ) ){
      strcpy( fullname, TableName );
      return fullname;
   }
   else{
      sprintf( fullname, "%s.%s", Schema, TableName );
      return fullname;
   }
}
/* ------------------------------------------------------------------ */






void dbSetCaseSensitivity(char *string)
{
   if(!string || !strlen(string) ) return;
   strcpy(CaseSensitivity,string);
}
/* ------------------------------------------------------------------ */




char * dbGetCaseSensitivity(void)
{
   return CaseSensitivity;
}
/* ------------------------------------------------------------------ */



void dbSetWfdiscTableName(char *string)
{
   if(!string || !strlen(string) ) return;
   strcpy(wfdiscTableName,string);
}
/* ------------------------------------------------------------------ */




const char * dbGetWfdiscTableName(void)
{
   static char fullname[200];
   return FullyQualified( wfdiscTableName, fullname );
}
/* ------------------------------------------------------------------ */

void dbSetWftagTableName(char *string)
{
   if(!string || !strlen(string) ) return;
   strcpy(wftagTableName,string);
}
/* ------------------------------------------------------------------ */




const char * dbGetWftagTableName(void)
{
   static char fullname[200];
   return FullyQualified( wftagTableName, fullname );
}
/* ------------------------------------------------------------------ */

void dbSetOriginTableName(char *string)
{
   if(!string || !strlen(string) ) return;
   strcpy(originTableName,string);
}
/* ------------------------------------------------------------------ */




const char * dbGetOriginTableName(void)
{
   static char fullname[200];
   return FullyQualified( originTableName, fullname );
}
/* ------------------------------------------------------------------ */

void dbSetEventTableName(char *string)
{
   if(!string || !strlen(string) ) return;
   strcpy(eventTableName,string);
}
/* ------------------------------------------------------------------ */




const char * dbGetEventTableName(void)
{
   static char fullname[200];
   return FullyQualified( eventTableName, fullname );
}
/* ------------------------------------------------------------------ */

void dbSetArrivalTableName(char *string)
{
   if(!string || !strlen(string) ) return;
   strcpy(arrivalTableName,string);
}
/* ------------------------------------------------------------------ */




const char * dbGetArrivalTableName(void)
{
   static char fullname[200];
   return FullyQualified( arrivalTableName, fullname );
}
/* ------------------------------------------------------------------ */

void dbSetAssocTableName(char *string)
{
   if(!string || !strlen(string) ) return;
   strcpy(assocTableName,string);
}
/* ------------------------------------------------------------------ */




const char * dbGetAssocTableName(void)
{
   static char fullname[200];
   return FullyQualified( assocTableName, fullname );
}
/* ------------------------------------------------------------------ */

void dbSetSiteTableName(char *string)
{
   if( !string || !strlen(string) < 1 ) return;
   strcpy(siteTableName,string);
}
/* ------------------------------------------------------------------ */




const char * dbGetSiteTableName(void)
{
   static char fullname[200];
   return FullyQualified( siteTableName, fullname );
}
/* ------------------------------------------------------------------ */

void dbSetSitechanTableName(char *string)
{
   if(!string || !strlen(string) ) return;
   strcpy(sitechanTableName,string);
}
/* ------------------------------------------------------------------ */




const char * dbGetSitechanTableName(void)
{
   static char fullname[200];
   return FullyQualified( sitechanTableName, fullname );
}
/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */

void dbSetSearchlinkTableName(char *string)
{
   if(!string || !strlen(string) ) return;
   strcpy(searchlinkTableName,string);
}
/* ------------------------------------------------------------------ */


const char * dbGetSearchlinkTableName(void)
{
   static char fullname[200];
   return FullyQualified( searchlinkTableName, fullname );
}
/* ------------------------------------------------------------------ */


void dbSetInstrumentTableName(char *string)
{
   if(!string || !strlen(string) ) return;
   strcpy(instrumentTableName,string);
}
/* ------------------------------------------------------------------ */


const char * dbGetInstrumentTableName(void)
{
   static char fullname[200];
   return FullyQualified( instrumentTableName, fullname );
}
/* ------------------------------------------------------------------ */


void dbSetSensorTableName(char *string)
{
   if(!string || !strlen(string) ) return;
   strcpy(sensorTableName,string);
}
/* ------------------------------------------------------------------ */


const char * dbGetSensorTableName(void)
{
   static char fullname[200];
   return FullyQualified( sensorTableName, fullname );
}
/* ------------------------------------------------------------------ */




void dbListTableNames()
{
      printf( "  wfdiscTable:     %s\n" , wfdiscTableName ) ;
      printf( "  wftagTable:      %s\n" , wftagTableName ) ;
      printf( "  originTable:     %s\n" , originTableName ) ;
      printf( "  eventTable:      %s\n" , eventTableName ) ;
      printf( "  arrivalTable:    %s\n" , arrivalTableName ) ;
      printf( "  assocTable:      %s\n" , assocTableName ) ;
      printf( "  siteTable:       %s\n" , siteTableName ) ;
      printf( "  sitechanTable:   %s\n" , sitechanTableName ) ;
      printf( "  searchlinkTable: %s\n" , searchlinkTableName ) ;
}
/* ------------------------------------------------------------------ */




void  dbUpdateDefaultsFromFile(char * ConFigFileName)
{
   FILE *fptr;
   char buffer[100];
   char *c;
   char *keyword;
   char * value;

   if(! (fptr = fopen(ConFigFileName,"r")) )return;
   while(fgetsp(buffer,99,fptr) ){
      if( c = strchr(buffer,'!') ) *c = 0;
      if( c = strchr(buffer,'#') ) *c = 0;
      if( c = strchr(buffer,';') ) *c = 0;

      if(!buffer) continue;
      if(!strchr(buffer,'=') ){
         keyword = Upstring(strtok(buffer," \t"));
	 if(!keyword)continue;
	 if(!strcmp(keyword,"ALIAS"))AddAlias(strtok(0," \t\n"),strtok(0," \t\n"));
	 continue;
      }
      DeBlank(buffer);
      keyword = strtok(buffer,"=");
      value = strtok(0,"=\n");
      if(!value)continue;
      if(!strcmp(Upstring(keyword),"MAXROWSINQUERY") )
          sscanf(value,"%d",&MaxRowsInQuery);

      if(!strcmp(Upstring(keyword),"ORACLELOGINNAME") )
          strcpy(logname,value);

      if(!strcmp(Upstring(keyword),"ORACLEPASSWORD") )
          strcpy(passwd,value);

      if(!strcmp(Upstring(keyword),"SID") )
          strcpy(sid,value);

      if(!strcmp(Upstring(keyword),"SCHEMA") )
         strcpy(Schema,value);

      if(!strcmp(Upstring(keyword),"PHASELIST") )
         strcpy(Phaselist,value);

      if(!strcmp(Upstring(keyword),"AUTHLIST") )
         strcpy(Authlist,value);

      if(!strcmp(Upstring(keyword),"CROPFROMPATH") )
         strcpy(CropFromPath,value);

      if(!strcmp(Upstring(keyword),"PREPENDTOPATH") )
         strcpy(PrependToPath,value);

      if(!strcmp(Upstring(keyword),"CASESENSITIVITY") ){
         if(!strcmp(Upstring(value),"ON") )
            strcpy(CaseSensitivity,"ON");
         else if(!strcmp(Upstring(value),"OFF") )
            strcpy(CaseSensitivity,"OFF");
         else
	    dbPrintToDevice("WARNING: Bad value for CaseSensitivity in config file.\n");
      }

      else if( !strcmp( Upstring(    keyword ) , "WFDISCTABLE" ) )
         strcpy( wfdiscTableName     , value ) ;
      else if( !strcmp( Upstring(    keyword ) , "WFTAGTABLE" ) )
         strcpy( wftagTableName      , value ) ;
      else if( !strcmp( Upstring(    keyword ) , "ORIGINTABLE" ) )
         strcpy( originTableName     , value ) ;
      else if( !strcmp( Upstring(    keyword ) , "EVENTTABLE" ) )
         strcpy( eventTableName      , value ) ;
      else if( !strcmp( Upstring(    keyword ) , "ARRIVALTABLE" ) )
         strcpy( arrivalTableName    , value ) ;
      else if( !strcmp( Upstring(    keyword ) , "ASSOCTABLE" ) )
         strcpy( assocTableName      , value ) ;
      else if( !strcmp( Upstring(    keyword ) , "SITETABLE" ) )
         strcpy( siteTableName       , value ) ;
      else if( !strcmp( Upstring(    keyword ) , "SITECHANTABLE" ) )
         strcpy( sitechanTableName   , value ) ;
      else if( !strcmp( Upstring(    keyword ) , "SEARCHLINKTABLE" ) )
         strcpy( searchlinkTableName , value ) ;
      else if( !strcmp( Upstring(    keyword ) , "INSTRUMENTTABLE" ) )
         strcpy( instrumentTableName , value ) ;
      else if( !strcmp( Upstring(    keyword ) , "SENSORTABLE" ) )
         strcpy( sensorTableName     , value ) ;


      strcpy(ConnectString,logname);
      strcat(ConnectString,"/");
      strcat(ConnectString,passwd);

   }


   fclose(fptr);
}
/* ------------------------------------------------------------------ */




