
#include "ncpf.h"

int  eom[2][15] = {
   { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365, 396, 424 },
   { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366, 397, 425 },
};
char *mon[]={ "", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };


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


  julianDay = julday(month, day, year); /* get astronomical Julian day */
  DeltaDays = julianDay - basedate;

  return (double) DeltaDays*SECPERDAY + (double)hour*SECPERHOUR +
         (double)min*SECPERMIN + (double)second;

}
/* ------------------------------------------------------------------ */


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


