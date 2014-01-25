#include <string.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include "../cssListOps/dblErrors.h"
#include "dbselect.h" 
#include "../time/timefuncs.h"
#include "../stringfun.h"
#include "dbDefaults.h"
#define DB_BUILD_SQL_STRING
static float MinBaz, MaxBaz ;
static float MinDep, MaxDep ;
#include "dbBuildSearchlink.h"
#undef DB_BUILD_SQL_STRING
#include "dbBuildSQLstring.h"
#include "dbErrorHandler.h"
#include "../sacIO/dbConversions.h"



/* static float MinBaz, MaxBaz ; */



float GetSrcCircleLatCSL(void){return SrcCircleLatC;}
float GetSrcCircleLonCSL(void){return SrcCircleLonC;}
float GetSrcCircleRadiusSL(void){return SrcCircleRadius;}

int dbGetStationCoords(const char *sta, float *latc, float *lonc ) ;





/* Set the maximum number of rows for db to return */
static char *SetRowRestriction(void)
{
   int maxRows;
   char restrictor[] = " rownum <= ";
   char value[10];
   char * clause;


   maxRows = MAX_ROWS_IN_PRIMARY_QUERY; 
   sprintf(value,"%d",maxRows);
   clause = stringcat(restrictor, value, 0 );
   return clause;
}
/* ------------------------------------------------------------------ */







/* Return the status of a search restriction */ 
int dbRestrictionAppliedSL(enum RestrictionSL Restrict)
{
  switch (Restrict){
     case SrcPolySL:
         return SrcBox;
	 break;

     case StaPolySL:
         return StaBox;
	 break;

     case SrcCircleSL:
         return DoSrcCircleRestrict;
	 break;

     case Edepth:
         return Edepth;
         break ;
  }

}
/* ------------------------------------------------------------------ */





static char *AddEtypeListSL(int index)
{
   int j;
   char * string;
   int stringLen;

   if( numParams[index] < 1){
      sprintf(dbErrorString,
      "ERROR: SRCTYPE option requires at least one source type. \n");
      strcat(dbErrorString, "In: AddEtypeListSL in dbBuildSearchlink");
      dblSetError(1, dbErrorString);
   }
   
   stringLen = 0;
   for(j=0;j<numParams[index];j++){
      stringLen += strlen( charParams[index][j] );
   }
   stringLen += j*3 + 22 ;
   string = (char *) malloc(stringLen *sizeof(char));
   strcpy(string,"UPPER( SL.ETYPE ) IN (");
   for(j=0;j<numParams[index]-1;j++){
      strcat(string,"'");
      strcat(string,charParams[index][j]);
      strcat(string,"',");
   }
   strcat(string,"'");
   strcat(string,charParams[index][numParams[index]-1]);
   strcat(string,"')");
   return string;
}
/* ------------------------------------------------------------------ */







static char *BuildWfidRestrictionSL(int index)
{
   int j;
   char * string;
   int stringLen;
   char Tmp[20];
   char WfidRestriction[] = "SL.WFID IN (";

   if( numParams[index] < 1){
      sprintf(dbErrorString,
      "ERROR: WFID option requires at least one wfid. \n");
      strcat(dbErrorString, "In: BuildWfidRestrictionSL in dbBuildSearchlink");
      dblSetError(1, dbErrorString);
   }
   
   stringLen = 0;
   for(j=0;j<numParams[index];j++){
      sprintf(Tmp, "%d, ", (int) floatParams[index][j] );
      stringLen += strlen(Tmp);
   }
   stringLen += 2 + strlen(WfidRestriction);
   string = (char *) malloc(stringLen *sizeof(char));
   strcpy(string, WfidRestriction);
   for(j=0;j<numParams[index]-1;j++){
      sprintf(Tmp, "%d, ", (int) floatParams[index][j] );
      strcat(string, Tmp);
   }
   sprintf(Tmp, "%d)", (int) floatParams[index][numParams[index]-1] );
   strcat(string, Tmp);
   return string;
}
/* ------------------------------------------------------------------ */



static void AddPhaseListSL(int index)
{
int j;
char * string;
int stringLen;

   if( numParams[index] < 1){
      sprintf(dbErrorString,
      "ERROR: PHASELIST option requires at least one phase name. \n");
      strcat(dbErrorString, "In: AddPhaseListSL in dbBuildSearchlink");
      dblSetError(1, dbErrorString);
   }

   stringLen = 0;
   for(j=0;j<numParams[index];j++){
      stringLen += strlen( charParams[index][j] );
   }
   stringLen += numParams[index];
   string = (char *) malloc(stringLen *sizeof(char));
   strcpy(string,charParams[index][0]);
   for(j=1;j<numParams[index];j++){
      strcat(string,",");
      strcat(string,charParams[index][j]);
   }
   dbSetQueryPhaselist(string);

   free(string);
}
/* ------------------------------------------------------------------ */




static void AddAuthListSL(int index)
{
int j;
char * string;
int stringLen;

   if( numParams[index] < 1){
      sprintf(dbErrorString,
      "ERROR: AUTHLIST option requires at least one author name. \n");
      strcat(dbErrorString, "In: AddAuthListSL in dbBuildSearchlink");
      dblSetError(1, dbErrorString);
   }

   stringLen = 0;
   for(j=0;j<numParams[index];j++){
      stringLen += strlen( charParams[index][j] );
   }
   stringLen += numParams[index];
   string = (char *) malloc(stringLen *sizeof(char));
   strcpy(string,charParams[index][0]);
   for(j=1;j<numParams[index];j++){
      strcat(string,",");
      strcat(string,charParams[index][j]);
   }
   dbSetQueryAuthlist(string);

   free(string);
}
/* ------------------------------------------------------------------ */







static void ChangeMaxRowsSL(int index)
{
int j;
int value;

   if( numParams[index] != 1){
      sprintf(dbErrorString,
      "ERROR: MAXROWS option requires exactly one value. \n");
      strcat(dbErrorString, "In: ChangeMaxRowsSL in dbBuildSearchlink");
      dblSetError(1, dbErrorString);
   }
   
   value = floatParams[index][0];
   if(value < 1)return;
   dbSetQueryMaxRows(value);

}
/* ------------------------------------------------------------------ */








static void SetEdepthLimits(int index)
{
   float tmp;

   if( numParams[index] != 2){
      sprintf(dbErrorString,
      "ERROR: instdepth option requires 2 values. \n");
      strcat(dbErrorString, "In: SetEdepthLimits in dbBuildSearchlink");
      dblSetError(1, dbErrorString);
      return;
   }

   if( numParams[15] != 1){
      sprintf(dbErrorString,
      "ERROR: Instdepth option requires that 1 station be specified. \n");
      strcat(dbErrorString, "In: SetEdepthLimits in dbBuildSearchlink");
      dblSetError(1, dbErrorString);
      return;
   }

   MinDep = floatParams[index][0];
   MaxDep = floatParams[index][1];
/*   DepRestrictionApplied = TRUE; */
}
/* ------------------------------------------------------------------ */





float dbMaxDistSL(void){return MaxDistValue;}
float dbMinDistSL(void){return MinDistValue;}
char *dbDistanceTypesSL(void){return DistanceType;}




static void SetSQLONOFFSL(int index)
{
   int state;
   if( numParams[index] > 1){
      sprintf(dbErrorString,
      "ERROR: ShowSQL option requires 1 or no values. \n");
      strcat(dbErrorString, "In: SetSQLONOFFSL in dbBuildSearchlink");
      dblSetError(1, dbErrorString);
   }
   if( numParams[index] == 0){
      state = dbGetShowSQL();
      if(state)
         dbSetShowSQL("OFF");
      else
         dbSetShowSQL("ON");
   }
   else 
      dbSetShowSQL(charParams[index][0]);


}
/* ------------------------------------------------------------------ */







/* extract all the user-supplied stations and put in a comma-delimited list */
static char *AddStationList(int index)
{
int j;
char * string;
int stringLen;

   if( numParams[index] < 1){
      sprintf(dbErrorString,
      "ERROR: STANAME option requires at least one station name. \n");
      strcat(dbErrorString, "In: AddStationList in dbBuildSearchlink");
      dblSetError(1, dbErrorString);
   }
   
   stringLen = 0;
   for(j=0;j<numParams[index];j++){
      stringLen += strlen( charParams[index][j] );
   }
   stringLen += j*3 +12;
   string = (char *) malloc(stringLen *sizeof(char));
   strcpy(string,"SL.STA IN (");
   for(j=0;j<numParams[index]-1;j++){
      strcat(string,"'");
      strcat(string,charParams[index][j]);
      strcat(string,"',");
   }
   strcat(string,"'");
   strcat(string,charParams[index][numParams[index]-1]);
   strcat(string,"')");
   return string;
}
/* ------------------------------------------------------------------ */






/* extract all the user-supplied components and put in a comma-delimited list */
static char *AddCompList(int index)
{
int j;
char * string;
int stringLen;

   if( numParams[index] < 1){
      sprintf(dbErrorString,
      "ERROR: COMPONENT option requires at least one component name. \n");
      strcat(dbErrorString, "In: AddCompList in dbBuildSearchlink");
      dblSetError(1, dbErrorString);
   }
   
   stringLen = 0;
   for(j=0;j<numParams[index];j++){
      stringLen += strlen( charParams[index][j] );
   }
   stringLen += j*3 +13;
   string = (char *) malloc(stringLen *sizeof(char));
   strcpy(string,"SL.CHAN IN (");
   for(j=0;j<numParams[index]-1;j++){
      strcat(string,"'");
      strcat(string,charParams[index][j]);
      strcat(string,"',");
   }
   strcat(string,"'");
   strcat(string,charParams[index][numParams[index]-1]);
   strcat(string,"')");
   return string;
}
/* ------------------------------------------------------------------ */






/* Add a clause to the WHERE string */
static char *AddToWhereString(char *WhereString, char *string)
{
   int len1, len2;
   len2 = strlen(string);
   if(WhereString == (char *) NULL){
      WhereString = (char *) malloc(len2 + 7);
      strcpy( WhereString, "WHERE ");
      strcat( WhereString, string);
   }
   else{
      len1 = strlen(WhereString);
      WhereString = (char *) realloc(WhereString, len1 + len2 + 6);
      strcat( WhereString, " AND ");
      strcat( WhereString, string);
   }

   return WhereString;
}
/* ------------------------------------------------------------------ */





/* The srcbox and stabox options allow specification of a polygon. This is
   done in a 2-step process. First the data for the smallest enclosing rect.
   are extracted. Then functions in dbPtsInside are used to find which of
   the returned data are in the polygon. This function determines the
   coordinates of that rectangle. 
*/
static char *GetBoxExtremaSL(int index, char *latlon)
{
   char minC[10];
   char maxC[10];
   float min,max,temp;
   char * string;
   int j;
   int MaxStringLen = 50;

   if( numParams[index] < 6){
      sprintf(dbErrorString,
      "ERROR: polygon option selected. Need at least 3 Lat - Lon pairs. \n");
      strcat(dbErrorString, "In: GetBoxExtremaSL in dbBuildSearchlink");
      dblSetError(1, dbErrorString);
   }

   min = FLT_MAX;
   max = (-min);

   if( strstr(latlon,"LAT") )
      j = 0 ;
   else
      j = 1 ;

     for( ;j<numParams[index];j+=2){
        if( floatParams[index][j] < min)min = floatParams[index][j];
        if( floatParams[index][j] > max)max = floatParams[index][j];
     }

   sprintf(minC,"%9.4f",min);
   sprintf(maxC,"%9.4f",max);


   string = stringcat( latlon, " >= ", minC,
                      " AND ", latlon, " <= ",
                      maxC, " ", 0);
   return string;
}
/* ------------------------------------------------------------------ */





static char *GetSrcCircleBoxExtrema(int index)
{
   char minC1[10];
   char maxC1[10];
   char minC2[10];
   char maxC2[10];
   float latc,lonc,radius;
   float minLat, maxLat, minLon, maxLon;
   char * string;

   if( numParams[index] < 3){
      sprintf(dbErrorString,
      "ERROR: SRCCIRCLE option selected. Need lat, lon, radius (km) \n");
      strcat(dbErrorString, "In: GetSrcCircleBoxExtrema in dbBuildSearchlink");
      dblSetError(1, dbErrorString);
   }


   latc   = floatParams[index][0];
   lonc   = floatParams[index][1];
   radius = (float) fabs( (double) floatParams[index][2]);
   SrcCircleLatC   = latc;
   SrcCircleLonC   = lonc;
   SrcCircleRadius = radius;
   DoSrcCircleRestrict = TRUE;

   dbGetEnclosingBox(latc, lonc, radius, &minLat, &maxLat,&minLon, &maxLon);

   sprintf(minC1,"%9.4f",minLat);
   sprintf(maxC1,"%9.4f",maxLat);
   sprintf(minC2,"%9.4f",minLon);
   sprintf(maxC2,"%9.4f",maxLon);


   string = stringcat("SL.OLAT >= ", minC1,
                      " AND ", "SL.OLAT <= ", maxC1, " ",
                      " AND ", "SL.OLON >= ", minC2, " ",
                      " AND ", "SL.OLON <= ", maxC2, " ",0);

   return string;
}
/* ------------------------------------------------------------------ */









/* This function forms clauses of the form " item1 <= value1 and item2 >= value2" */
static char *BuildMinMaxRestrictSL(int index, char * option, char *latlon)
{
   char minC[10];
   char maxC[10];
   char junk[ 7 ] , *junk1 , *junk2 ;
   float min,max,temp;
   char * string;
   int MaxStringLen = 50;

   if( numParams[index] != 2){
      sprintf(dbErrorString,
      "ERROR: %s option selected. Both min%s and max%s must be specified. \n",
	              option,latlon,latlon);
      strcat(dbErrorString, "In: BuildMinMaxRestrictSL in dbBuildSearchlink");
      dblSetError(1, dbErrorString);
   }

   min = floatParams[index][0];
   max = floatParams[index][1];
   if(min > max){
      if(min > 180 && !strstr( latlon , "DEPTH" ) )
         min -=360;
      else{
         temp = max;
         max = min;
         min = temp;
      }
   }


   string = (char *) malloc( strlen( latlon ) + 15 + 20 ) ;

   sprintf( string , "%s BETWEEN %9.4f AND %9.4f " , latlon , min , max ) ;


   return string;
}
/* ------------------------------------------------------------------ */






char *dbBuildSearchlink(void)
{
   /* Declare Variables. */
   char *tmp;
   int j, k, jdx ;
   char baseString[ ] = "SELECT DISTINCT SL.EVID, SL.ORID, SL.OLAT, SL.OLON, SL.DEPTH, "
                        "SL.MB, SL.MS, SL.ML, SL.TIME, SL.ETYPE, SL.STA, SL.CHAN, "
                        "SL.SLAT, SL.SLON, SL.DEGDIST, SL.SEAZ, SL.ESAZ, SL.WFID " ;


   char siteChanWhereClause[]   = "SC.STA = SL.STA AND SC.CHAN = SL.CHAN\n"
                                "AND SL.JDATE BETWEEN SC.ONDATE AND SC.OFFDATE ";  

   char FromString[ 140 ]       = "\0" ;
   char *WhereString            = (char *) NULL;
   char *QueryString            = (char *) NULL;

   char *RequestTimeC           = (char *) NULL;
   double RequestTime;
   char *timeError;
   char timeString[80];
   char *EvidRestriction        = NULL ;

   useSiteChanRestrict     = FALSE;

   SrcBox                       = FALSE;
   StaBox                       = FALSE;
   DoSrcCircleRestrict          = FALSE;


   /* Handle select criteria:  develope where clause */
   /* First process certain options. */
   for(j=0;j<NUMOPTIONS;j++){
      if( optionSelected[ j ] ) {

         if( !strcmp(options[j],"PHASELIST") ){
            AddPhaseListSL(j);
         }

         if( !strcmp(options[j],"AUTHLIST") ){
            AddAuthListSL(j);
         }

         if( !strcmp(options[j],"MAXROWS") ){
            ChangeMaxRowsSL(j);
         }

         if( !strcmp(options[j],"SHOWSQL") ){
            SetSQLONOFFSL(j);
         }
      } /* end if( optionSelected[ j ] ) */
   }


   /* Check each option to see if selected */
   for( j = 0 ; j < NUMOPTIONS ; j++ ) {

      if( optionSelected[j] ) {
         if( !strcmp(options[j],"SRCLAT") ){
           tmp = BuildMinMaxRestrictSL(j, "SRCLAT", "SL.OLAT");
           WhereString = AddToWhereString(WhereString, tmp );
           free(tmp);
         }

         if( !strcmp(options[j],"SRCLON") ){
           tmp = BuildMinMaxRestrictSL(j, "SRCLON", "SL.OLON");
           WhereString = AddToWhereString(WhereString, tmp);
           free(tmp);
         }

         if( !strcmp(options[j],"SRCDEP") ){
           tmp = BuildMinMaxRestrictSL(j, "SRCDEP", "SL.DEPTH");
           WhereString = AddToWhereString(WhereString, tmp);
           free(tmp);
         }

         if( !strcmp(options[j],"MBMAG") ){
           tmp = BuildMinMaxRestrictSL(j, "MBMAG", "SL.MB");
           WhereString = AddToWhereString(WhereString, tmp);
           free(tmp);
         }

         if( !strcmp(options[j],"MLMAG") ){
           tmp = BuildMinMaxRestrictSL(j, "MLMAG", "SL.ML");
           WhereString = AddToWhereString(WhereString, tmp);
           free(tmp);
         }

         if( !strcmp(options[j],"MSMAG") ){
           tmp = BuildMinMaxRestrictSL(j, "MSMAG", "SL.MS");
           WhereString = AddToWhereString(WhereString, tmp );
           free(tmp);
         }

         if( !strcmp(options[j],"SRCTYPE") ){
           tmp = AddEtypeListSL(j);
           WhereString  = AddToWhereString (WhereString, tmp );
           free(tmp);
         }

         if( !strcmp(options[j],"STANAME") ){
            tmp = AddStationList(j);
            WhereString = AddToWhereString(WhereString, tmp );
            free(tmp);
         }

         if( !strcmp(options[j],"COMPONENT") ){
            tmp = AddCompList(j);
            WhereString = AddToWhereString(WhereString, tmp );
            free(tmp);
         }

         if( !strcmp(options[j],"SRCBOX") ){
           tmp = GetBoxExtremaSL(j, "SL.OLAT");
           WhereString = AddToWhereString(WhereString, tmp );
           free(tmp);
           tmp = GetBoxExtremaSL(j, "SL.OLON");
           WhereString = AddToWhereString(WhereString, tmp );
           free(tmp);
           SrcBox = TRUE;
         }

         if( !strcmp(options[j],"STALAT") ){
           tmp = BuildMinMaxRestrictSL(j, "STALAT", "SL.SLAT");
           WhereString = AddToWhereString(WhereString, tmp );
           free(tmp);
         }

         if( !strcmp(options[j],"STALON") ){
           tmp = BuildMinMaxRestrictSL(j, "STALON", "SL.SLON");
           WhereString = AddToWhereString(WhereString, tmp  );
           free(tmp);
         }

         if( !strcmp(options[j],"STABOX") ){
           tmp = GetBoxExtremaSL(j, "SL.SLAT");
           WhereString = AddToWhereString(WhereString, tmp );
           free(tmp);
           tmp = GetBoxExtremaSL(j, "SL.SLON");
           WhereString = AddToWhereString(WhereString, tmp );
           free(tmp);
           StaBox = TRUE;
         }

         if( !strcmp(options[j],"EVENTID") ){
           EvidRestriction = (char *) malloc( sizeof( "SL.EVID IN (" ) + 1 ) ;
           strcpy( EvidRestriction , "SL.EVID IN (" ) ;
           for( jdx = 0 ; jdx < numParams[ 24 ] ; jdx++ ) {
              int temp = strlen( charParams[ 24 ][ jdx ] ) + strlen( EvidRestriction ) + 2 ;

              EvidRestriction = (char *) realloc( EvidRestriction , temp ) ;

              if( jdx ) strcat( EvidRestriction , ", " ) ;
              strcat( EvidRestriction , charParams[ 24 ][ jdx ] ) ;
           } /* end for */

           EvidRestriction = (char *) realloc( EvidRestriction , strlen( EvidRestriction ) + 3 );
           strcat( EvidRestriction , " ) " ) ;
           WhereString = AddToWhereString(WhereString, EvidRestriction );
           free( EvidRestriction ) ;
           EvidRestriction = NULL ;
         }
    
         if( !strcmp(options[j],"INSTDEPTH") ){
           if( !useSiteChanRestrict){
              useSiteChanRestrict = TRUE;
              WhereString  = AddToWhereString (WhereString,  siteChanWhereClause  );
           }
           tmp = BuildMinMaxRestrictSL(j, "INSTDEPTH", "SC.EDEPTH");
           WhereString = AddToWhereString(WhereString, tmp );
           free(tmp);
         }

         if( !strcmp(options[j],"SRCCIRCLE") ){
           tmp = GetSrcCircleBoxExtrema(j);
           WhereString = AddToWhereString(WhereString, tmp );
           free(tmp);
         }
         if( !strcmp(options[j],"SRCDIST") ){
           floatParams[j][0] /= 111.1 ;
           floatParams[j][1] /= 111.1 ;

           tmp = BuildMinMaxRestrictSL(j, "SRCDIST", "SL.DEGDIST");
           WhereString = AddToWhereString(WhereString, tmp );
           free(tmp);
         }
         if( !strcmp(options[j],"SRCDELTA") ){
           tmp = BuildMinMaxRestrictSL(j, "SRCDELTA", "SL.DEGDIST");
           WhereString = AddToWhereString(WhereString, tmp );
           free(tmp);
         }

         if( !strcmp(options[j],"STARTTIME") ){
           if( numParams[j] > 0){
              RequestTimeC = strAllocCat( RequestTimeC, charParams[j][0]);
              for(k=1;k < numParams[j]; k++){
                 RequestTimeC = strAllocCat( RequestTimeC, "/");
                 RequestTimeC = strAllocCat( RequestTimeC, charParams[j][k]);
              }
              RequestTime = tmStrToEpochTime(RequestTimeC);
    
              if( warningLevel == SevereTimeError){
                 timeError = tmGetLastError();
                 printf("%s\n",timeError);
              }
              else{
                 sprintf(timeString," SL.TIME >= %f ",RequestTime);
                 WhereString = AddToWhereString(WhereString, timeString);
              }
              free(RequestTimeC); RequestTimeC = 0;
           }
    
         }

         if( !strcmp(options[j],"ENDTIME") ){
           if( numParams[j] > 0){
              RequestTimeC = strAllocCat( RequestTimeC, charParams[j][0]);
              for(k=1;k < numParams[j]; k++){
                 RequestTimeC = strAllocCat( RequestTimeC, "/");
                 RequestTimeC = strAllocCat( RequestTimeC, charParams[j][k]);
              }
              RequestTime = tmStrToEpochTime(RequestTimeC);
              if( warningLevel == SevereTimeError){
                 timeError = tmGetLastError();
                 printf("%s\n",timeError);
              }
              else{
                 sprintf(timeString," SL.TIME <= %f ",RequestTime);
                 WhereString = AddToWhereString(WhereString, timeString);
              }
              free(RequestTimeC); RequestTimeC = 0;
           }
         }

         if( !strcmp(options[j],"QUADRANT") ){
           tmp = BuildMinMaxRestrictSL(j, "QUADRANT", "SL.SEAZ");
           WhereString = AddToWhereString(WhereString, tmp );
           free(tmp);
         }

         if( !strcmp(options[j],"WFID") ){
           tmp = BuildWfidRestrictionSL(j) ;
           WhereString = AddToWhereString(WhereString, tmp );
           free( tmp ) ;
         }
      } /* end if( optionSelected[ j ] ) */
   } /* end for( j ) */

   tmp = SetRowRestriction();
   WhereString = AddToWhereString(WhereString, tmp );
   free(tmp);



   /* Put it all in one string, and return it. */
   if( useSiteChanRestrict )
      sprintf( FromString , "FROM %s SL, %s SC " ,
               dbGetSearchlinkTableName() , dbGetSitechanTableName() ) ;
   else
      sprintf( FromString , "FROM %s SL " , dbGetSearchlinkTableName() ) ;

   QueryString = (char *) malloc( 3 + strlen( baseString ) + 
                 strlen( FromString ) + strlen( WhereString ) ) ;

   sprintf( QueryString , "%s\n%s\n%s" , baseString , FromString , 
            WhereString ) ;

   return QueryString ;
} /* end second dbBuildSearchlink() */
