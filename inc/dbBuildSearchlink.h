static int useSiteChanRestrict = FALSE ;

#ifdef DB_BUILD_SQL_STRING

/* Module global variables */
#define MAX_ROWS_IN_PRIMARY_QUERY 2000

static int SrcBox = FALSE;
static int StaBox = FALSE;
static float MinDistValue;
static float MaxDistValue;
static char DistanceType[10];
static float SrcCircleLatC;
static float SrcCircleLonC;
static float SrcCircleRadius;
static int DoSrcCircleRestrict = FALSE;

/* ------------------------------------------------------------------ */





/* Public functions and data*/

/* list of possible restrictions */
enum RestrictionSL { SrcPolySL, StaPolySL, SrcCircleSL, Edepth };

/* Build the SQL string from user data */
char *dbBuildSearchlink(void);

/* Return the status of a search restriction */ 
int dbRestrictionAppliedSL(enum RestrictionSL Restrict);

float dbMaxDistSL(void);
float dbMinDistSL(void);
char *dbDistanceTypeSL(void);

float GetSrcCircleLatCSL(void);
float GetSrcCircleLonCSL(void);
float GetSrcCircleRadiusSL(void);

/* accessors for MinBaz and MaxBaz */
float dbGetMinBazSL(void){
   return MinBaz ;
}

float dbGetMaxBazSL(void){
   return MaxBaz ;
}

float dbGetMinDepSL(void){
   return MinDep ;
}

float dbGetMaxDepSL(void){
   return MaxDep ;
}

void SetEdepthLimits(int index) ;




/* private function list */
/* Get centroid coords of polygon */ 
static void GetBoxCentroid(int index, float *LatCenter, float *LonCenter);

/* extract all the user-supplied stations and put in a comma-delimited list */
/* static char *AddStationList(int index); */

/* extract all the user-supplied components and put in a comma-delimited list */
/* static char *AddCompList(int index); */

/* Add a clause to the WHERE string */
/* static char *AddToWhereString(char *WhereString, char *string); */

/* determines the coordinates of smallest rectangle enclosing polygon. */
static char *GetBoxExtrema(int index, char *TableDot, char *latlon);

/* This function forms clauses of the form " item1 <= value1 and item2 >= value2" */
static char *BuildMinMaxRestrict(int index, char *TableDot, char * option, 
                                 char *latlon);

/* Set the maximum number of rows for db to return */
/* static char *SetRowRestriction(void); */

/* ------------------------------------------------------------------ */



#else

/* Public functions and data*/

/* list of possible restrictions */
enum RestrictionSL {SrcPolySL, StaPolySL, SrcCircleSL, Edepth};

/* Build the SQL string from user data */
extern char *dbBuildSearchlink(void);

/* Return the status of a search restriction */ 
extern int dbRestrictionAppliedSL(enum RestrictionSL Restrict);

extern float dbMaxDistSL(void);
extern float dbMinDistSL(void);
extern char *dbDistanceTypeSL(void);
extern float GetSrcCircleLatCSL(void);
extern float GetSrcCircleLonCSL(void);
extern float GetSrcCircleRadiusSL(void);
extern float dbGetMinBazSL(void);
extern float dbGetMaxBazSL(void);



#endif 
