/* Prototypes, for external use --------------------------------------- */

/* Get system time as EpochTime */
extern double tmGetEpochTime( void );

/* Build EpochTime from components */
extern double tmMakeEpochTime( int year, int month, int day, int hour,
                        int min, float second);

/* Decode EpochTime into components */
extern int tmDecodeEpochTime( double time, int *year, int *month, int *day,
                       int *hour, int *min, float *second);

/* convert EpochTime to ASCII string  */
extern char *tmListEpochTime( double time, int fmtCode);

/* Get pointer to module error string */
extern char *tmGetLastError(void);

/* print last error */
extern void tmPrintLastError(void);

/* convert a formatted string into epoch time. */
extern double tmStrToEpochTime(char *string);

extern enum  warning { NoTimeError, ProbableTimeError, SevereTimeError} warningLevel;

/* convert julian date to epochtime 
   if begin is TRUE, get epochtime of the begining of the day,
   else get epochtime of the end of the day. */

extern double julianToEpoch ( int jDate , int begin ) ;

/* determine whether year is a leap year */
int    isleap( int year);

/* Calculate month, day from day of year  */
void mnday( int doy, int leap, int *mon, int *day );

/* Compute day of year from month, day, and whether it's a leap-year. */
int    yrday( int mo, int day, int lp);
