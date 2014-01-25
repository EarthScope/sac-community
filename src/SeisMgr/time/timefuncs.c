#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>
#include <time.h>
#include <math.h>
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif


/* Prototypes, for external use --------------------------------------- */

/* Get system time as EpochTime */
double tmGetEpochTime( void );

/* Build EpochTime from components */
double tmMakeEpochTime( int year, int month, int day, int hour,
                        int min, float second);

/* Decode EpochTime into components */
int tmDecodeEpochTime( double time, int *year, int *month, int *day,
                       int *hour, int *min, float *second);

/* convert EpochTime to ASCII string  */
char *tmListEpochTime( double time, int fmtCode);

/* Get pointer to module error string */
char *tmGetLastError(void);

/* print last error */
void tmPrintLastError(void);

/* convert a formatted string into epoch time. */
double tmStrToEpochTime(char *string);

/* convert julian date to epochtime 
   if begin is TRUE, get epochtime of the begining of the day,
   else get epochtime of the end of the day. */

double julianToEpoch ( int jDate , int begin ) ;

enum  warning { NoTimeError, ProbableTimeError, SevereTimeError} warningLevel;



/* Prototypes, for internal use --------------------------------------- */
int    yrday( int, int, int );

/* Calculate month, day from day of year  */
void mnday( int doy, int leap, int *mon, int *day );

/* determine whether year is a leap year */
int    isleap( int year);

/* Set the module level warning string to string */
void SetWarningMessage(char *string);

/* return TRUE if token has non-numeric characters */
int NotNumericString(char *token);

/* make sure second is in range */
void ValidateSec(float *second);

/* make sure min is in range */
void ValidateMin(int *min);

/* make sure hour is in range */
void ValidateHour(int *hour);

/* make sure day is in range for month and year */
void ValidateDay(int year, int month, int *day);

/* make sure month is in range */
void ValidateMon(int *month);

/* make sure DayOfYear is in range for year */
void ValidateDOY(int year, int *DayOfYear);

/* calculate astronomical julian day from month,day,year */
int julday(int mon, int day, int year);

/* given astronomical Julian day, return month,day,year */
void caldat(int julian, int *mon, int *day, int *year);



/* Time intervals */
#define SECPERMIN             (60)
#define SECPERHOUR            (60L*SECPERMIN)
#define SECPERDAY             (24L*SECPERHOUR)

#define MAXFORM 19
#define DATE_STRING_LEN 40


/* Macros ------------------------------------------------------------- */
/* floor(x/y), where x, y>0 are integers, using integer arithmetic */
#define qfloor( x, y ) ( x>0 ? (x)/y : -((y-1-(x))/y) )

/* Globals ------------------------------------------------------------ */
static int  eom[2][15] = {
   { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365, 396, 424 },
   { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366, 397, 425 },
};
static char *mon[]={ "", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

static char *tmTimeErrorBuff;         /* Error message buffer */
static char outstr[40];        	      /* buffer for tmListEpochTime output */
static char buffer[80];               /* buffer for strToEpochTime input copy */


/* Recognize leap years  */
int isleap( int yr)
{
   int l;

   if( yr < 0 )
      yr++;
   l = ( yr%4 == 0 );
   l = l && ( yr%100 != 0 || yr%400 == 0 );

   return( l );
}


/* retrieve a pointer to string holding last error */
char *tmGetLastError(void)
{
   if( warningLevel == NoTimeError)return (char *)NULL;
   return (tmTimeErrorBuff);
}
/* ------------------------------------------------------------------ */



/* print last time error to stderr */
void tmPrintLastError(void)
{
   if( warningLevel == NoTimeError)return;
   if( warningLevel == ProbableTimeError){
      fprintf(stderr,"Probable Error in time: ");
      if( tmTimeErrorBuff != (char *)NULL)fprintf(stderr,"%s\n",tmTimeErrorBuff);
   }
   if( warningLevel == SevereTimeError){
      fprintf(stderr,"Severe Error in time: ");
      if( tmTimeErrorBuff != (char *)NULL)fprintf(stderr,"%s\n",tmTimeErrorBuff);
   }
}
/* ------------------------------------------------------------------ */



/* module functions use this to set error messages */
void SetWarningMessage(char *string)
{
   int len;
   len = strlen(string) +1;
   tmTimeErrorBuff = (char *) realloc(tmTimeErrorBuff, len*sizeof(char) );
   strcpy(tmTimeErrorBuff,string);
}
/* ------------------------------------------------------------------ */




/* make sure string contains only numbers and "." */
int NotNumericString(char *token)
{
   int j;
   for(j=0;j< (int)strlen(token);j++){
      if( !(isdigit(token[j])) ){
         if( !(token[j] == '.') ){
            warningLevel = SevereTimeError;
            SetWarningMessage("Token is not a number.");
            return(TRUE);
         }
      }
   }
   return(FALSE);
}
/* ------------------------------------------------------------------ */




/* make sure second is between 0 and 60  */
void ValidateSec(float *second)
{
   if( *second < 0){
      *second = 0;
      warningLevel = ProbableTimeError;
      SetWarningMessage("Second value out of range (0 <= s <= 60).");
   }
   if( *second > 60){
      *second = 60;
      warningLevel = ProbableTimeError;
      SetWarningMessage("Second value out of range (0 <= s <= 60).");
   }
}
/* ------------------------------------------------------------------ */




/* make sure minute is between 0 and 60  */
void ValidateMin(int *min)
{
   if( *min < 0){
      *min = 0;
      warningLevel = ProbableTimeError;
      SetWarningMessage("Minute value out of range (0 <= m <= 60).");
   }
   if( *min > 60){
      *min = 60;
      warningLevel = ProbableTimeError;
      SetWarningMessage("Minute value out of range (0 <= m <= 60).");
   }
}
/* ------------------------------------------------------------------ */




/* make sure hour is between 0 and 24  */
void ValidateHour(int *hour)
{
   if( *hour < 0){
      *hour = 0;
      warningLevel = ProbableTimeError;
      SetWarningMessage("Hour value out of range (0 <= h <= 24).");
   }
   if( *hour > 24){
      *hour = 24;
      warningLevel = ProbableTimeError;
      SetWarningMessage("Hour value out of range (0 <= h <= 24).");
   }
}
/* ------------------------------------------------------------------ */




/* make sure day is in correct range given year and month  */
void ValidateDay(int year, int month, int *day)
{
   int maxDay;
   if( *day < 1){
      *day = 1;
      warningLevel = ProbableTimeError;
      SetWarningMessage("Day value less than 1 (setting to 1).");
   }
   maxDay = eom[isleap(year)][month] - eom[isleap(year)][month-1];
   if( *day > maxDay){
      *day = maxDay;
      warningLevel = ProbableTimeError;
      SetWarningMessage("Day value out of range. Setting to max for month.");
   }
}
/* ------------------------------------------------------------------ */





/* make sure month is between 1 and 12  */
void ValidateMon(int *month)
{
   if( *month < 1){
      *month = 1;
      warningLevel = ProbableTimeError;
      SetWarningMessage("Month value out of range (1 <= m <= 12).");
   }
   if( *month > 12){
      *month = 12;
      warningLevel = ProbableTimeError;
      SetWarningMessage("Month value out of range (1 <= m <= 12).");
   }
}
/* ------------------------------------------------------------------ */




/* make sure day-of-year is correct  */
void ValidateDOY(int year, int *DayOfYear)
{
   int maxDay;
   if( *DayOfYear < 1){
      *DayOfYear = 1;
      warningLevel = ProbableTimeError;
      SetWarningMessage("DayOfYear value out of range.");
   }
   maxDay = eom[isleap(year)][12];
   if( *DayOfYear > maxDay){
      *DayOfYear = maxDay;
      warningLevel = ProbableTimeError;
      SetWarningMessage("DayOfYear value out of range.");
   }
}
/* ------------------------------------------------------------------ */



/* get astronomical Julian day from month,day,year */
int julday(int mm, int id, int iyyy)
{
        int jul;
        int ja,jy=iyyy,jm;
        int IGREG = 588829;

        if (jy == 0){
           SetWarningMessage("julday: there is no year zero. Setting to 1");
           jy = 1;
        }
        if (jy < 0) ++jy;
        if (mm > 2) {
                jm=mm+1;
        } else {
                --jy;
                jm=mm+13;
        }
        jul = (int) (floor(365.25*jy)+floor(30.6001*jm)+id+1720995);
        if (id+31L*(mm+12L*iyyy) >= IGREG) {
                ja=(int)(0.01*jy);
                jul += 2-ja+(int) (0.25*ja);
        }
        return jul;
}
/* ------------------------------------------------------------------ */




/* get month,day,year from astronomical julian day */
void caldat(int julian, int *mm, int *id, int *iyyy)
{
        int ja,jalpha,jb,jc,jd,je;
        int IGREG2 = 2299161;

        if (julian >= IGREG2) {
                jalpha=(int)(((float) (julian-1867216)-0.25)/36524.25);
                ja=julian+1+jalpha-(int) (0.25*jalpha);
        } else
                ja=julian;
        jb=ja+1524;
        jc=(int)(6680.0+((float) (jb-2439870)-122.1)/365.25);
        jd=(int)(365*jc+(0.25*jc));
        je=(int)((jb-jd)/30.6001);
        *id=jb-jd-(int) (30.6001*je);
        *mm=je-1;
        if (*mm > 12) *mm -= 12;
        *iyyy=jc-4715;
        if (*mm > 2) --(*iyyy);
        if (*iyyy <= 0) --(*iyyy);
}
/* ------------------------------------------------------------------ */







/* Compute day of year  */
int yrday( int mo, int day, int lp )
{
   return( eom[lp][mo-1] + day );
}
/* ------------------------------------------------------------------ */




/* Calculate month, day from day of year  */
void mnday( int d, int lp, int *pm, int *pd )
{
   int  i;
   int  *et = eom[lp];
   for( i=1; d>et[i]; i++ );
   *pm = i;
   *pd = d - et[i-1];

   return;
}
/* ------------------------------------------------------------------ */






/* Return system time as epoch time  */
double tmGetEpochTime( void )
{
   time_t t;
   double EpochTime;
   struct tm *tmptm;

   t = time(NULL);
   tmptm = localtime(&t);
   t = mktime(tmptm); 
   EpochTime = t;
   return EpochTime;
}
/* ------------------------------------------------------------------ */






/* Build EpochTime from components -------
   This is the number of seconds since 01/01/1970 00:00:00.0
*/

double tmMakeEpochTime( int year, int month, int day, int hour,
                    int min, float second) {
   int err;
   int *et = eom[isleap(year)];
   int basedate = 2440588; /* astronomical julian day for 01/01/1970 */
   int julianDay;   /* astronomical julian day */
   int DeltaDays;   /* number of days relative to basedate */

   err = 0;
   if( year == 0 ) {
      warningLevel = SevereTimeError;
      SetWarningMessage("Year value out of range.");
      err = 1;
   }

   if( month == 0 )
      mnday( day, isleap( year), &month, &day );

   if( month < 1 || month > 12 ) {
      warningLevel = SevereTimeError;
      SetWarningMessage("Month value out of range (1 <= m <= 12).");
      err = 1;
   }

   if( day < 1 || day > et[month]-et[month-1] ) {
      warningLevel = SevereTimeError;
      SetWarningMessage("Day value out of range.");
      err = 1;
   }

   if( err )
      return( 0.0 );


  julianDay = julday(month, day, year);	/* get astronomical Julian day */
  DeltaDays = julianDay - basedate;

  return (double) DeltaDays*SECPERDAY + (double)hour*SECPERHOUR +
         (double)min*SECPERMIN + (double)second;

}
/* ------------------------------------------------------------------ */



/* convert julian date to epochtime 
   if begin is TRUE, get epochtime of the begining of the day,
   else get epochtime of the end of the day. */

double julianToEpoch ( int jDate , int begin )
{
        int year , doy , month , day , hour , min ;
        float sec ;
        double epochtime ;


        year = jDate / 1000 ;

        doy = jDate - ( year * 1000 ) ;

        mnday ( doy , isleap ( year ) , &month , &day ) ;

        hour = begin ? 0 : 23 ;

        min = begin ? 0 : 59 ;

        sec = begin ? 0 : 59.999 ;

        epochtime = tmMakeEpochTime ( year , month , day , hour , min , sec ) ;

        return epochtime ;
}

/* ------------------------------------------------------------------ */





/* Decode EpochTime */
int tmDecodeEpochTime( double time, int *year, int *month, int *day,
                   int *hour, int *min, float *second)
{
   int ElapsedDays;
   double secondsRemaining;
   int basedate = 2440588; /* astronomical julian day for 01/01/1970 */
   int julianDay;   /* astronomical julian day */

   ElapsedDays = time/SECPERDAY;
   secondsRemaining = time - ((double)ElapsedDays)*SECPERDAY;
   julianDay = basedate + ElapsedDays;
   caldat(julianDay,month,day,year);

   if(secondsRemaining < 0){
      (*day)--;
      secondsRemaining += SECPERDAY;
   }

   *hour = secondsRemaining / SECPERHOUR;
   secondsRemaining = secondsRemaining - *hour * SECPERHOUR;
   *min = secondsRemaining /SECPERMIN;
   *second = secondsRemaining- *min * SECPERMIN;
      

   return( 1 );
}
/* ------------------------------------------------------------------ */










/* Display EpochTime  */
char *tmListEpochTime( double time, int form )
{
   int century, year, month, day, hour, min;
   float second;


   if( form < 0 || form > MAXFORM )
      form=0;

   if( tmDecodeEpochTime( time, &year, &month, &day, &hour, &min, &second ) == 0 )
      return( 0 );

   century = ( (int) ( year / 100 ) ) * 100 ;

   switch( form ) {
      case 0:
         sprintf( outstr, "%02d%02d%02d%02d%02d%06.3f", year-century, month,
                  day, hour, min, second );
         break;
      case 1:
         sprintf( outstr, "%02d%02d%02d%02d%02d%02d", year-century, month,
                  day, hour, min, (int)second );
         break;
      case 2:
         sprintf( outstr, "%02d %02d %02d% 02d %02d %06.3f", year-century,
                  month, day, hour, min, second );
         break;
      case 3:
         sprintf( outstr, "%02d %02d %02d% 02d %02d %02d", year-century,
                  month, day, hour, min, (int)second );
         break;
      case 4:
         sprintf( outstr, "%02d/%02d/%02d %02d:%02d:%06.3f", month, day,
                  year-century, hour, min, second );
         break;
      case 5:
         sprintf( outstr, "%02d/%02d/%02d %02d:%02d:%02d", month, day,
                  year-century, hour, min, (int)second );
         break;
      case 6:
         sprintf( outstr, "%s %d, %d %02d:%02d %06.3f GMT", mon[month],
                  day, year, hour, min, second );
         break;
      case 7:
         sprintf( outstr, "%s %d, %d %02d:%02d %02d GMT", mon[month], day,
                  year, hour, min, (int)second );
         break;
      case 8:
         sprintf( outstr, "%02d %03d %02d:%02d:%06.03f", year-century,
                 yrday( month, day, isleap( year) ), hour, min, second );
         break;
      case 9:
         sprintf( outstr, "%08d", (int)time );
         break;
      case 10:
         sprintf( outstr, "%02d*%03d+%02d:%02d:%06.03f", year-century,
                  yrday( month, day, isleap( year) ), hour, min, second );
         break;
      case 11:
         sprintf( outstr, "%02d/%02d/%02d %02d:%02d:%09.6f", month, day,
                  year-century, hour, min, second );
         break;
      case 12:
         sprintf( outstr, "%02d/%03d/%02d:%02d:%09.06f", year-century,
                  yrday( month, day, isleap( year) ), hour, min, second );
         break;
      case 13:
         sprintf( outstr, "%d/%03d/%02d:%02d:%06.03f", year,
                  yrday( month, day, isleap( year) ), hour, min, second );
         break;
      case 14:
         sprintf( outstr, "%04d/%02d/%02d/%02d:%02d:%06.03f", year, month,
                  day, hour, min, second );
         break;
      case 15:
         sprintf( outstr, "%02d-%s-%02d", day, mon[month], year % 100  );
         break;
      case 16:
         sprintf( outstr, "%04i,%03d,%02d:%02d:%06.04f", year,
                  yrday( month, day, isleap( year) ), hour, min, second );
         break;
      case 17:
         sprintf( outstr, "%04i,%03d,%02d:%02d:%02d", year,
                  yrday( month, day, isleap( year) ), hour, min, (int)second );
         break;
      case 18:
	 sprintf ( outstr , "%04i%02d%02d" , year , month , day ) ;
	 break ;
      case 19:
         sprintf( outstr, "%02d-%s-%04d", day, mon[month], year );
         break;
   }

   return( (char *)outstr );
}
/* ------------------------------------------------------------------ */





/* convert a formatted string into epoch time. Supported formats are:
   YYYY/DDD/HH:MM:SS.SSS and YYYY/MM/DD/HH:MM:SS.SSS legal separators
   between tokens are "/ - : " as well as the space and tab characters.
   Subsets of the format are also supported as long as they are not
   ambiguous. For instance YYYY DDD HH will be interpreted as
   YYYY/DDD/HH:00:00.000. Note that if day-of year format is used and
   if the day-of year is given with only 2 digits, the string will be
   ambiguous unless the remainder of the string is complete to the
   second and the second has a non-zero number of milliseconds. I.E.
   1997 12 22 13 will be ambiguous, but 1997 12 22 13.45 will not be.
*/

double tmStrToEpochTime(char *str)
{
   double EpochTime =  tmGetEpochTime( );
   int DayOfYear;
   int year, month, day, hour, min;
   float second;
   char *token;
   float tokVal[5];
   int ntokens;


   second = 0.0;
   min = 0;
   hour = 0;
   day = 1;
   month = 1;
   warningLevel = NoTimeError;
   
   if(!str || !strlen(str) ) return EpochTime;

   strcpy(buffer,str);

   /* Get the year from the buffer string. */
   token = strtok(buffer,": /-        "); 
   if(token == (char *)NULL)return EpochTime; 
   sscanf(token,"%d",&year); 
   if(NotNumericString(token) )return EpochTime; 
   if( year < 0 ){
      warningLevel = SevereTimeError;
      SetWarningMessage("Year is less than zero.");
      return EpochTime;
   }
   else if(year < 1900){
      warningLevel = ProbableTimeError;
      SetWarningMessage("Year is less than 1900.");
   }

      /* Get the next token and see if it is a day-of-year. */
   token = strtok(NULL,": /-        ");
   if(!token){      /* No more tokens so use defaults */
      EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
      return EpochTime;
   }
   else{
      if(strlen(token) == 3){         /* must be day of year */
         if(NotNumericString(token) )return EpochTime;
         sscanf(token,"%d",&DayOfYear);
         ValidateDOY(year, &DayOfYear);
         mnday(DayOfYear, isleap(year), &month, &day );
         token = strtok(NULL,": /-        "); /* try for hour */
         if(token == (char *)NULL){      /* No more tokens so use defaults */
            EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
            return EpochTime;
         }
         else{
            if(NotNumericString(token) )return EpochTime;
            sscanf(token,"%d",&hour);
            ValidateHour(&hour);
         }
         token = strtok(NULL,": /-        "); /* try for minute */
         if(token == (char *)NULL){      /* No more tokens so use defaults */
            EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
            return EpochTime;
         }
         else{
            if(NotNumericString(token) )return EpochTime;
            sscanf(token,"%d",&min);
            ValidateMin(&min);
         }
         token = strtok(NULL,": /-        "); /* try for second */
         if(token == (char *)NULL){      /* No more tokens so use defaults */
            EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
            return EpochTime;
         }
         else{
            if(NotNumericString(token) )return EpochTime;
            sscanf(token,"%f",&second);
            ValidateSec(&second);
            EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
            return EpochTime;
         }
      }
      else{        /* could be day-of-year format or month day format */
         ntokens = 1;
         while(token != (char *)NULL){
            if(NotNumericString(token) )return EpochTime;
            sscanf(token,"%f",tokVal + ntokens -1);
            token = strtok(NULL,": /-        ");
            ntokens++;
            if(ntokens > 6)break;
         }
         ntokens--;
         if(ntokens == 5){       /* must be month-day format */
            month = tokVal[0];
            ValidateMon(&month);
            day = tokVal[1];
            ValidateDay(year,month,&day);
            hour = tokVal[2];
            ValidateHour(&hour);
            min = tokVal[3];
            ValidateMin(&min);
            second = tokVal[4];
            ValidateSec(&second);
            EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
            return EpochTime;
         }

         month = day = 1;
         hour = min = second = 0.0;
         if(ntokens == 4){       /* may be DOY format */
            if( tokVal[0]  > 12){
               DayOfYear = tokVal[0];
               ValidateDOY(year, &DayOfYear);
               mnday(DayOfYear, isleap(year), &month, &day );
               hour = tokVal[1];
               ValidateHour(&hour);
               min = tokVal[2];
               ValidateMin(&min);
               second = tokVal[3];
               ValidateSec(&second);
               EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
               return EpochTime;
            }
	    else{              /* assuming MM-DD-HH-MM */
               month = tokVal[0];
               ValidateMon(&month);
               day = tokVal[1];
               ValidateDay(year,month,&day);
               hour = tokVal[2];
               ValidateHour(&hour);
               min = tokVal[3];
               ValidateMin(&min);
               EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
               return EpochTime;
            }
	 }

         if(ntokens == 3){       /* may be DOY format */
            if( tokVal[0]  > 12){
               DayOfYear = tokVal[0];
               ValidateDOY(year, &DayOfYear);
               mnday(DayOfYear, isleap(year), &month, &day );
               hour = tokVal[1];
               ValidateHour(&hour);
               min = tokVal[2];
               ValidateMin(&min);
               EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
               return EpochTime;
            }
	    else{              /* assuming MM-DD-HH-MM */
               month = tokVal[0];
               ValidateMon(&month);
               day = tokVal[1];
               ValidateDay(year,month,&day);
               hour = tokVal[2];
               ValidateHour(&hour);
               EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
               return EpochTime;
            }
	 }

         if(ntokens == 2){       /* may be DOY format */
            if( tokVal[0]  > 12){
               DayOfYear = tokVal[0];
               ValidateDOY(year, &DayOfYear);
               mnday(DayOfYear, isleap(year), &month, &day );
               hour = tokVal[1];
               ValidateHour(&hour);
               EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
               return EpochTime;
            }
	    else{              /* assuming MM-DD-HH-MM */
               month = tokVal[0];
               ValidateMon(&month);
               day = tokVal[1];
               ValidateDay(year,month,&day);
               EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
               return EpochTime;
            }
	 }

         if(ntokens == 1){       /* may be DOY format */
            if( tokVal[0]  > 12){
               DayOfYear = tokVal[0];
               ValidateDOY(year, &DayOfYear);
               mnday(DayOfYear, isleap(year), &month, &day );
               EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
               return EpochTime;
            }
	    else{              /* assuming MM-DD-HH-MM */
               month = tokVal[0];
               ValidateMon(&month);
               EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
               return EpochTime;
            }
	 }

         if(ntokens < 1){        
            EpochTime = tmMakeEpochTime(year, month, day, hour, min, second);
            return EpochTime;
         }

      }
   }

   return EpochTime;
}
/* ------------------------------------------------------------------ */



