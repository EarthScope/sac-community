#ifdef DB_BUILD_SQL_STRING

/* Module global variables */
#define MAX_ROWS_IN_PRIMARY_QUERY 2000

static int useOriginRestrict = FALSE;
static int useSiteRestrict = FALSE;
static int useSiteChanRestrict = FALSE;
static int useEvidRestriction = FALSE;
static int SrcBox = FALSE;
static int StaBox = FALSE;
static float MinDistValue;
static float MaxDistValue;
static char DistanceType[10];
static int DistRestrictionApplied;
static float SrcCircleLatC;
static float SrcCircleLonC;
static float SrcCircleRadius;
static int DoSrcCircleRestrict = FALSE;
static int BazRestrictionApplied;

/* ------------------------------------------------------------------ */





/* Public functions and data*/

/* list of possible restrictions */
enum Restriction {Origin, Site, SrcPoly, StaPoly, EvidList, SrcCircle, Quadrant};

/* Build the SQL string from user data */
char *dbBuildSQLstring(void);

/* Return the status of a search restriction */ 
int dbRestrictionApplied(enum Restriction Restrict);

int dbDistRestrictionApplied(void);
float dbMaxDist(void);
float dbMinDist(void);
char *dbDistanceType(void);

float GetSrcCircleLatC(void);
float GetSrcCircleLonC(void);
float GetSrcCircleRadius(void);

float dbGetMinBaz(void) {
    return MinBaz ;
}

float dbGetMaxBaz(void) {
    return MaxBaz ;
}






/* private function list */
/* Get centroid coords of polygon */ 
static void GetBoxCentroid(int index, float *LatCenter, float *LonCenter);

/* extract all the user-supplied stations and put in a comma-delimited list */
/* static char *AddStationList(int index); */

/* extract all the user-supplied components and put in a comma-delimited list */
/* static char *AddCompList(int index); */

/* put line breaks before the start of each clause and limit line length to 80 */
static char *FormatQueryString(char *QueryString);

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
enum Restriction {Origin, Site, SrcPoly, StaPoly, EvidList, SrcCircle, Quadrant};

/* Build the SQL string from user data */
extern char *dbBuildSQLstring(void);

/* Return the status of a search restriction */ 
extern int dbRestrictionApplied(enum Restriction Restrict);

extern int dbDistRestrictionApplied(void);
extern float dbMaxDist(void);
extern float dbMinDist(void);
extern char *dbDistanceType(void);
extern float GetSrcCircleLatC(void);
extern float GetSrcCircleLonC(void);
extern float GetSrcCircleRadius(void);
extern float dbGetMinBaz(void);
extern float dbGetMaxBaz(void);



#endif 
