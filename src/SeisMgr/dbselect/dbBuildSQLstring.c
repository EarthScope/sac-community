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
#include "dbBuildSQLstring.h"
#undef DB_BUILD_SQL_STRING
#include "dbErrorHandler.h"
#include "../sacIO/dbConversions.h"



/* static float MinBaz, MaxBaz ; */



float GetSrcCircleLatC(void){return SrcCircleLatC;}
float GetSrcCircleLonC(void){return SrcCircleLonC;}
float GetSrcCircleRadius(void){return SrcCircleRadius;}
int TablesAvailable(int  useOriginRestrict, int useSiteRestrict, int useSiteChanRestrict,
                      int EvidAvailable);

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





static int DBnameSpecifiedInQuery(void)
{
   int j;

   for(j=0;j<NUMOPTIONS;j++){
     if(optionSelected[j] && !strcmp(options[j],"DBNAME") )
        return 1;
   }
   return 0;
}
/* ------------------------------------------------------------------ */







/* Return the status of a search restriction */ 
int dbRestrictionApplied(enum Restriction Restrict)
{
  switch (Restrict){
     case Origin:
         return useOriginRestrict;
	 break;

     case Site:
         return useSiteRestrict;
	 break;

     case SrcPoly:
         return SrcBox;
	 break;

     case StaPoly:
         return StaBox;
	 break;

     case EvidList:
         return useEvidRestriction;
	 break;

     case SrcCircle:
         return DoSrcCircleRestrict;
	 break;

     case Quadrant:
         return BazRestrictionApplied;
	 break;
  }

}
/* ------------------------------------------------------------------ */





static char *AddEtypeList(int index)
{
int j;
char * string;
int stringLen;

   if( numParams[index] < 1){
      sprintf(dbErrorString,
      "ERROR: SRCTYPE option requires at least one source type. \n");
      strcat(dbErrorString, "In: AddEtypeList in dbBuildSQLstring");
      dblSetError(1, dbErrorString);
   }
   
   stringLen = 0;
   for(j=0;j<numParams[index];j++){
      stringLen += strlen( charParams[index][j] );
   }
   stringLen += j*3 +13;
   string = (char *) malloc(stringLen *sizeof(char));
   strcpy(string,"O.ETYPE IN (");
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







static char *BuildWfidRestriction(int index)
{
   int j;
   char * string;
   int stringLen;
   char Tmp[20];
   char WfidRestriction[] = "W.WFID IN (";

   if( numParams[index] < 1){
      sprintf(dbErrorString,
      "ERROR: WFID option requires at least one wfid. \n");
      strcat(dbErrorString, "In: BuildWfidRestriction in dbBuildSQLstring");
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



static void AddPhaseList(int index)
{
int j;
char * string;
int stringLen;

   if( numParams[index] < 1){
      sprintf(dbErrorString,
      "ERROR: PHASELIST option requires at least one phase name. \n");
      strcat(dbErrorString, "In: AddPhaseList in dbBuildSQLstring");
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




static void AddAuthList(int index)
{
int j;
char * string;
int stringLen;

   if( numParams[index] < 1){
      sprintf(dbErrorString,
      "ERROR: AUTHLIST option requires at least one author name. \n");
      strcat(dbErrorString, "In: AddAuthList in dbBuildSQLstring");
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







static void ChangeMaxRows(int index)
{
int j;
int value;

   if( numParams[index] != 1){
      sprintf(dbErrorString,
      "ERROR: MAXROWS option requires exactly one value. \n");
      strcat(dbErrorString, "In: ChangeMaxRows in dbBuildSQLstring");
      dblSetError(1, dbErrorString);
   }
   
   value = floatParams[index][0];
   if(value < 1)return;
   dbSetQueryMaxRows(value);

}
/* ------------------------------------------------------------------ */






static void SetDistanceLimits(int index, const char *type)
{
   float tmp;

   if( numParams[index] < 1 || numParams[index] > 2){
      sprintf(dbErrorString,
      "ERROR: Distance limits options require 1 or 2 values. \n");
      strcat(dbErrorString, "In: SetDistanceLimits in dbBuildSQLstring");
      dblSetError(1, dbErrorString);
   }


   if( numParams[15] != 1){
      sprintf(dbErrorString,
      "ERROR: Distance limits options require that 1 station be specified. \n");
      strcat(dbErrorString, "In: SetDistanceLimits in dbBuildSQLstring");
      dblSetError(1, dbErrorString);
      return;
   }


   if( numParams[index] == 1){
      MaxDistValue = floatParams[index][0];
      MinDistValue = 0;
   }
   else{
      MinDistValue = floatParams[index][0];
      MaxDistValue = floatParams[index][1];
      if(MinDistValue > MaxDistValue){
         tmp = MinDistValue;
         MinDistValue = MaxDistValue;
         MaxDistValue = MinDistValue;
      }
   }
   strcpy(DistanceType, type);
   DistRestrictionApplied = 1;

}
/* ------------------------------------------------------------------ */




static void SetQuadrantLimits(int index)
{
   float tmp;

   if( numParams[index] != 2){
      sprintf(dbErrorString,
      "ERROR: Quadrant option requires 2 values. \n");
      strcat(dbErrorString, "In: SetQuadrantLimits in dbBuildSQLstring");
      dblSetError(1, dbErrorString);
      return;
   }

   if( numParams[15] != 1){
      sprintf(dbErrorString,
      "ERROR: Quadrant option requires that 1 station be specified. \n");
      strcat(dbErrorString, "In: SetQuadrantLimits in dbBuildSQLstring");
      dblSetError(1, dbErrorString);
      return;
   }

   MinBaz = floatParams[index][0];
   MaxBaz = floatParams[index][1];
   if(MinBaz < 180 &&  MaxBaz > 180){
         tmp = MinBaz;
         MinBaz = MaxBaz;
         MaxBaz = MinBaz;
   }
   BazRestrictionApplied = TRUE;
}
/* ------------------------------------------------------------------ */





int dbDistRestrictionApplied(void){ return DistRestrictionApplied;}
float dbMaxDist(void){return MaxDistValue;}
float dbMinDist(void){return MinDistValue;}
char *dbDistanceType(void){return DistanceType;}




static void SetSQLONOFF(int index)
{
   int state;
   if( numParams[index] > 1){
      sprintf(dbErrorString,
      "ERROR: ShowSQL option requires 1 or no values. \n");
      strcat(dbErrorString, "In: SetSQLONOFF in dbBuildSQLstring");
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
      strcat(dbErrorString, "In: AddStationList in dbBuildSQLstring");
      dblSetError(1, dbErrorString);
   }
   
   stringLen = 0;
   for(j=0;j<numParams[index];j++){
      stringLen += strlen( charParams[index][j] );
   }
   stringLen += j*3 +12;
   string = (char *) malloc(stringLen *sizeof(char));
   strcpy(string,"W.STA IN (");
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
      strcat(dbErrorString, "In: AddCompList in dbBuildSQLstring");
      dblSetError(1, dbErrorString);
   }
   
   stringLen = 0;
   for(j=0;j<numParams[index];j++){
      stringLen += strlen( charParams[index][j] );
   }
   stringLen += j*3 +13;
   string = (char *) malloc(stringLen *sizeof(char));
   strcpy(string,"W.CHAN IN (");
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





/* put line breaks before the start of each clause and limit line length to 80 */
static char *FormatQueryString(char *QueryString)
{
   int LineLen = 80;
   char *fromPos;
   char *wherePos;
   char *andPos;
   int selectClauseLen;
   int j;
   int commaPos,oldCommaPos;
   int placeInLine, oldPlaceInLine;

   fromPos = strstr(QueryString,"FROM");
   *(fromPos -1) = '\n';
   selectClauseLen = fromPos - QueryString;

   wherePos = strstr(QueryString,"WHERE");
   if(wherePos != (char *) NULL) *(wherePos -1) = '\n';

   andPos = strstr(QueryString,"AND");
   while(andPos != (char *) NULL){
      *(andPos - 1) = '\n';
      andPos = strstr(andPos +3, "AND");
   }



   oldPlaceInLine = 0;
   oldCommaPos = 0;
   for(j=0;j< selectClauseLen;j++){
      if(QueryString[j] == ','){
         placeInLine = j % LineLen;
         commaPos = j;
         if(placeInLine < oldPlaceInLine)QueryString[oldCommaPos+1] = '\n';
	 oldCommaPos = commaPos;
         oldPlaceInLine = placeInLine;
      }   
   }     

   return QueryString;

}
/* ------------------------------------------------------------------ */





/* Add a clause to the SELECT string */
char *AddToSelectString(char *SelectString, char * string)
{
   return strAllocCat(SelectString, string);
}
/* ------------------------------------------------------------------ */






/* Add a clause to the FROM string */
char *AddToFromString(char *FromString, char * string)
{
   return strAllocCat(FromString, string);
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
   the returned data are in the polygon. This function determines the coordinates
   of that rectangle. 
*/
static char *GetBoxExtrema(int index, char *TableDot, char *latlon)
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
      strcat(dbErrorString, "In: GetBoxExtrema in dbBuildSQLstring");
      dblSetError(1, dbErrorString);
   }

   min = FLT_MAX;
   max = (-min);

   if( ! strcmp(latlon,"LAT") ){
     for(j=0;j<numParams[index];j+=2){
        if( floatParams[index][j] < min)min = floatParams[index][j];
        if( floatParams[index][j] > max)max = floatParams[index][j];
     }
   }
   else{
     for(j=1;j<numParams[index];j+=2){
        if( floatParams[index][j] < min)min = floatParams[index][j];
        if( floatParams[index][j] > max)max = floatParams[index][j];
     }
   }
   sprintf(minC,"%9.4f",min);
   sprintf(maxC,"%9.4f",max);


   string = stringcat(TableDot, latlon, " >= ", minC,
                      " AND ", TableDot, latlon, " <= ",
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
      strcat(dbErrorString, "In: GetSrcCircleBoxExtrema in dbBuildSQLstring");
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


   string = stringcat("O.LAT >= ", minC1,
                      " AND ", "O.LAT <= ", maxC1, " ",
                      " AND ", "O.LON >= ", minC2, " ",
                      " AND ", "O.LON <= ", maxC2, " ",0);

   return string;
}
/* ------------------------------------------------------------------ */









static char *GetSrcDistBoxExtrema(void)
{
   char minC1[10];
   char maxC1[10];
   char minC2[10];
   char maxC2[10];
   float latc,lonc,radius;
   float minLat, maxLat, minLon, maxLon;
   char * string;


   if( !dbGetStationCoords(charParams[15][0],&latc, &lonc) ){
      sprintf(dbErrorString,
      "ERROR: Could not get station coords from database. \n");
      strcat(dbErrorString, "In: GetSrcDistBoxExtrema in dbBuildSQLstring");
      dblSetError(1, dbErrorString);
   }


   if(!strcmp(DistanceType,"DIST"))
      dbGetEnclosingBox(latc, lonc, MaxDistValue, &minLat, &maxLat,&minLon, &maxLon);
   else{
      minLat = latc - MaxDistValue;
      maxLat = latc + MaxDistValue;
      minLon = lonc - MaxDistValue;
      maxLon = lonc + MaxDistValue;
   }

   sprintf(minC1,"%9.4f",minLat);
   sprintf(maxC1,"%9.4f",maxLat);
   sprintf(minC2,"%9.4f",minLon);
   sprintf(maxC2,"%9.4f",maxLon);


   string = stringcat("O.LAT >= ", minC1,
                      " AND ", "O.LAT <= ", maxC1, " ",
                      " AND ", "O.LON >= ", minC2, " ",
                      " AND ", "O.LON <= ", maxC2, " ",0);

   return string;
}
/* ------------------------------------------------------------------ */






/* This function forms clauses of the form " item1 <= value1 and item2 >= value2" */
static char *BuildMinMaxRestrict(int index, char *TableDot, char * option, char *latlon)
{
   char minC[10];
   char maxC[10];
   float min,max,temp;
   char * string;
   int MaxStringLen = 50;

   if( numParams[index] != 2){
      sprintf(dbErrorString,
      "ERROR: %s option selected. Both min%s and max%s must be specified. \n",
	              option,latlon,latlon);
      strcat(dbErrorString, "In: BuildMinMaxRestrict in dbBuildSQLstring");
      dblSetError(1, dbErrorString);
   }

   min = floatParams[index][0];
   max = floatParams[index][1];
   if(min > max){
      if(min > 180)
         min -=360;
      else{
         temp = max;
         max = min;
         min = temp;
      }
   }
   sprintf(minC,"%9.4f",min);
   sprintf(maxC,"%9.4f",max);
   

   string = stringcat(TableDot, latlon, " BETWEEN ", minC,
                      " AND ", maxC, " ", 0);
   return string;
}
/* ------------------------------------------------------------------ */






/* Build the SQL string from user data */
char *dbBuildSQLstring(void)
{
   char *tmp;
   int j, k;
   char *p;
   char baseString[ ] = "SELECT DISTINCT W.STA, W.CHAN, W.TIME, W.WFID, W.CHANID, "
                        "W.JDATE, W.ENDTIME, W.NSAMP, W.SAMPRATE, W.CALIB, W.CALPER, "
                        "W.INSTYPE, W.SEGTYPE, W.DATATYPE, W.CLIP, W.DIR, W.DFILE, "
                        "W.FOFF, W.COMMID, W.LDDATE ";


   char originFields[]          = ", O.LAT, O.LON, O.DEPTH, O.ORID, E.EVID OEVID ";
   char originFromClause[200]   = "";
   char originWhereClause[]     = " UPPER(WT.TAGNAME) = 'EVID' AND E.EVID = WT.TAGID AND E.PREFOR = O.ORID ";

   char siteFields[]            = ", S.LAT SLAT, S.LON SLON, S.ELEV SELEV ";
   char siteFromClause[100]     = "";
   char siteWhereClause[]       = ", W.STA = S.STA \nAND W.JDATE BETWEEN S.ONDATE AND S.OFFDATE ";

   char siteChanFields[]        = ", SC.EDEPTH SCEDEPTH ";
   char siteChanFromClause[100] = "";
   char siteChanWhereClause[]   = ", SC.STA = W.STA AND SC.CHAN = W.CHAN\nAND W.JDATE "
                                  "BETWEEN SC.ONDATE AND SC.OFFDATE ";
   
   char EvidRestriction[]       = " WT.TAGNAME = 'evid' AND W.WFID = WT.WFID AND "
                                  "WT.TAGID IN (SELECT EVID FROM SAC_TMP_EVIDLIST) ";

   char *SelectString           = (char *) NULL;
   char *FromString             = (char *) NULL;
   char *WhereString            = (char *) NULL;
   char *QueryString            = (char *) NULL;
   char FromClauseBase[100]     = "";
   char eventFromClause[100]    = "";

   char *RequestTimeC           = (char *) NULL;
   double RequestTime;
   char *timeError;
   char timeString[80];


   DistRestrictionApplied       = FALSE;
   useEvidRestriction           = FALSE;
   useOriginRestrict            = FALSE;
   useSiteRestrict              = FALSE;
   useSiteChanRestrict          = FALSE;
   SrcBox                       = FALSE;
   StaBox                       = FALSE;
   DoSrcCircleRestrict          = FALSE;
   BazRestrictionApplied        = FALSE;




   /* First process options that do not go in main query. */
   for(j=0;j<NUMOPTIONS;j++){    

     if(optionSelected[j] && !strcmp(options[j],"PHASELIST") ){
        AddPhaseList(j);
     }

     if(optionSelected[j] && !strcmp(options[j],"AUTHLIST") ){
        AddAuthList(j);
     }

     if(optionSelected[j] && !strcmp(options[j],"MAXROWS") ){
        ChangeMaxRows(j);
     }

     if(optionSelected[j] && !strcmp(options[j],"SRCDIST") ){
        SetDistanceLimits(j, "DIST");
     }

     if(optionSelected[j] && !strcmp(options[j],"SRCDELTA") ){
        SetDistanceLimits(j, "DELTA");
     }

     if(optionSelected[j] && !strcmp(options[j],"QUADRANT") ){
        SetQuadrantLimits(j);
     }

     if(optionSelected[j] && !strcmp(options[j],"SHOWSQL") ){
        SetSQLONOFF(j);
     }

   }



   /* Set the initial part of from clause for each type of restriction.
      These may or may not get used depending on options selected by user. */
   sprintf( FromClauseBase , " FROM %s W ", dbGetWfdiscTableName() );
   sprintf( eventFromClause  , ", %s WT " , dbGetWftagTableName() );
   sprintf( originFromClause , ", %s O , %s E " ,
                               dbGetOriginTableName() , dbGetEventTableName() );
   sprintf( siteFromClause , ", %s S " , dbGetSiteTableName() ) ;
   sprintf( siteChanFromClause , ", %s SC " , dbGetSitechanTableName() ) ;

   SelectString = (char *) malloc( strlen(baseString) + 1 );
   strcpy(SelectString, baseString);

   FromString = (char *) malloc( strlen(FromClauseBase) + 1);
   strcpy(FromString, FromClauseBase);




   /* Check each option to see if selected */
   for(j=0;j<NUMOPTIONS;j++){

     if(optionSelected[j] && !strcmp(options[j],"SRCLAT") ){
       if( !useOriginRestrict){
	  useOriginRestrict = TRUE;
          WhereString  = AddToWhereString (WhereString,  originWhereClause  );
       }
       tmp = BuildMinMaxRestrict(j,  "O.", "SRCLAT", "LAT");
       WhereString = AddToWhereString(WhereString, tmp );
       free(tmp); 
     }

     if(optionSelected[j] && !strcmp(options[j],"SRCLON") ){
       if( !useOriginRestrict){
	  useOriginRestrict = TRUE;
          WhereString  = AddToWhereString (WhereString,  originWhereClause  );
       }
       tmp = BuildMinMaxRestrict(j,  "O.", "SRCLON", "LON");
       WhereString = AddToWhereString(WhereString, tmp); 
       free(tmp);
     }

     if(optionSelected[j] && !strcmp(options[j],"SRCDEP") ){
       if( !useOriginRestrict){
	  useOriginRestrict = TRUE;
          WhereString  = AddToWhereString (WhereString,  originWhereClause  );
       }
       tmp = BuildMinMaxRestrict(j,  "O.", "SRCDEP", "DEPTH");
       WhereString = AddToWhereString(WhereString, tmp);
       free(tmp); 
     }

     if(optionSelected[j] && !strcmp(options[j],"MBMAG") ){
       if( !useOriginRestrict){
	  useOriginRestrict = TRUE;
          WhereString  = AddToWhereString (WhereString,  originWhereClause  );
       }
       tmp = BuildMinMaxRestrict(j,  "O.", "MBMAG", "MB");
       WhereString = AddToWhereString(WhereString, tmp);
       free(tmp); 
     }

     if(optionSelected[j] && !strcmp(options[j],"MLMAG") ){
       if( !useOriginRestrict){
	  useOriginRestrict = TRUE;
          WhereString  = AddToWhereString (WhereString,  originWhereClause  );
       }
       tmp = BuildMinMaxRestrict(j,  "O.", "MLMAG", "ML");
       WhereString = AddToWhereString(WhereString, tmp);
       free(tmp); 
     }

     if(optionSelected[j] && !strcmp(options[j],"MSMAG") ){
       if( !useOriginRestrict){
	  useOriginRestrict = TRUE;
          WhereString  = AddToWhereString (WhereString,  originWhereClause  );
       }
       tmp = BuildMinMaxRestrict(j,  "O.", "MSMAG", "MS");
       WhereString = AddToWhereString(WhereString, tmp ); 
       free(tmp);
     }

     if(optionSelected[j] && !strcmp(options[j],"SRCTYPE") ){
       if( !useOriginRestrict){
	  useOriginRestrict = TRUE;
       }
       tmp = AddEtypeList(j);
       WhereString  = AddToWhereString (WhereString, tmp );
       free(tmp);
     }

     if(optionSelected[j] && !strcmp(options[j],"STANAME") ){
        tmp = AddStationList(j);
	WhereString = AddToWhereString(WhereString, tmp );
        free(tmp); 
     }

     if(optionSelected[j] && !strcmp(options[j],"COMPONENT") ){
        tmp = AddCompList(j);
	WhereString = AddToWhereString(WhereString, tmp );
        free(tmp); 
     }

     if(optionSelected[j] && !strcmp(options[j],"SRCBOX") ){
       if( !useOriginRestrict){
	  useOriginRestrict = TRUE;
          WhereString  = AddToWhereString (WhereString,  originWhereClause  );
       }
       tmp = GetBoxExtrema(j, "O.", "LAT");
       WhereString = AddToWhereString(WhereString, tmp );
       free(tmp);
       tmp = GetBoxExtrema(j, "O.", "LON");
       WhereString = AddToWhereString(WhereString, tmp );
       free(tmp);
       SrcBox = TRUE;
     }

     if(optionSelected[j] && !strcmp(options[j],"STALAT") ){
       if( !useSiteRestrict){
          useSiteRestrict = TRUE;
          WhereString  = AddToWhereString (WhereString,  siteWhereClause  );
       }
       tmp = BuildMinMaxRestrict(j, "S.", "STALAT", "LAT");
       WhereString = AddToWhereString(WhereString, tmp );
       free(tmp); 
     }

     if(optionSelected[j] && !strcmp(options[j],"STALON") ){
       if( !useSiteRestrict){
          useSiteRestrict = TRUE;
          WhereString  = AddToWhereString (WhereString,  siteWhereClause  );
       }
       tmp = BuildMinMaxRestrict(j, "S.", "STALON", "LON");
       WhereString = AddToWhereString(WhereString, tmp  ); 
       free(tmp);
     }

     if(optionSelected[j] && !strcmp(options[j],"STABOX") ){
       if( !useSiteRestrict){
          useSiteRestrict = TRUE;
          WhereString  = AddToWhereString (WhereString,  siteWhereClause  );
       }
       tmp = GetBoxExtrema(j, "S.", "LAT");
       WhereString = AddToWhereString(WhereString, tmp );
       free(tmp);
       tmp = GetBoxExtrema(j, "S.", "LON");
       WhereString = AddToWhereString(WhereString, tmp );
       free(tmp);
       StaBox = TRUE;
     }


     if(optionSelected[j] && !strcmp(options[j],"EVENTID") ){
       useEvidRestriction = TRUE;
       WhereString = AddToWhereString(WhereString, EvidRestriction ) ;
       FromString  = AddToFromString (FromString , eventFromClause ) ;
     }

     if(optionSelected[j] && !strcmp(options[j],"INSTDEPTH") ){
       if( !useSiteChanRestrict){
	  useSiteChanRestrict = TRUE;
          WhereString  = AddToWhereString (WhereString,  siteChanWhereClause  );
       }
       tmp = BuildMinMaxRestrict(j,  "SC.", "INSTDEPTH", "EDEPTH");
       WhereString = AddToWhereString(WhereString, tmp );
       free(tmp); 
     }

     if(optionSelected[j] && !strcmp(options[j],"SRCCIRCLE") ){
       if( !useOriginRestrict){
	  useOriginRestrict = TRUE;
          WhereString  = AddToWhereString (WhereString,  originWhereClause  );
       }
       tmp = GetSrcCircleBoxExtrema(j);
       WhereString = AddToWhereString(WhereString, tmp );
       free(tmp);
     }
     if(optionSelected[j] && (!strcmp(options[j],"SRCDIST") || 
                              !strcmp(options[j],"SRCDELTA") ) ){
       if( !useOriginRestrict){
	  useOriginRestrict = TRUE;
          WhereString  = AddToWhereString (WhereString,  originWhereClause  );
       }
       tmp = GetSrcDistBoxExtrema();
       WhereString = AddToWhereString(WhereString, tmp );
       free(tmp);

     }


     if(optionSelected[j] && !strcmp(options[j],"STARTTIME") ){
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
	     sprintf(timeString," W.TIME >= %f ",RequestTime);
	     WhereString = AddToWhereString(WhereString, timeString);
	  }
          free(RequestTimeC); RequestTimeC = 0;
       }
      
     }

     if(optionSelected[j] && !strcmp(options[j],"ENDTIME") ){
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
	     sprintf(timeString," W.TIME <= %f ",RequestTime);
	     WhereString = AddToWhereString(WhereString, timeString);
	  }
          free(RequestTimeC); RequestTimeC = 0;
       }
     }


     if(optionSelected[j] && !strcmp(options[j],"WFID") ){
       WhereString = AddToWhereString(WhereString, BuildWfidRestriction(j) );
     }




   }

   tmp = SetRowRestriction();
   WhereString = AddToWhereString(WhereString, tmp );
   free(tmp);





   if(useOriginRestrict){
     SelectString = AddToSelectString(SelectString, originFields );
     FromString   = AddToFromString( FromString,   originFromClause   );
   }

   if( useSiteRestrict ){
     SelectString = AddToSelectString(SelectString, siteFields );
     FromString   = AddToFromString(  FromString,   siteFromClause   );
   }

   if( useSiteChanRestrict ){
     SelectString = AddToSelectString(SelectString, siteChanFields );
     FromString   = AddToFromString(  FromString,   siteChanFromClause   );
   }

   QueryString = stringcat(SelectString, FromString, WhereString, 0 );
   QueryString = FormatQueryString(QueryString);
   free(SelectString);
   free(FromString);
   if(WhereString)free(WhereString);

   if(!TablesAvailable( useOriginRestrict, useSiteRestrict, useSiteChanRestrict, 0 ) ){
      free(QueryString);
      QueryString = 0;
   }

   return QueryString;
}
/* ------------------------------------------------------------------ */



