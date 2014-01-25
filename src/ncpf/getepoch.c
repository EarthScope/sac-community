
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ncpf.h"
#include "bool.h"
#include "hdr.h"


#include "msg.h"
#include "bot.h"
#include "cpf.h"
#include "dff.h"

#include "debug.h"

void /*FUNCTION*/ getepoch(kfunction, nc, ic, kvalue, nerr)
char *kfunction;
char * kvalue ;
int *nc, ic;
int *nerr;
{
	int  fileNumber = 1 ;
	int zyear, zdoy, zhour, zmin ;
	float zsec ;
	int unused1, unused2, unused3 ;
	float rvalue;
	double value ;

	char buff[18]; /* 17 is standard number of spaces for epochtime */
  char *pBuff;

  UNUSED(ic);
  UNUSED(nc);
  UNUSED(kfunction);
  
  rvalue = 0.0;
	/*=====================================================================
	 * PURPOSE: Returns the epoch time of the given relative value.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    kfunction:  The full gettime command string [c].
	 *    nc:         Length of command line string [i].
	 *    ic:         Pointer to next token in command [i].
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:       Nonzero if error occurs.
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    rvalue:     Relative time to be converted to epoch time.
	 *    value:      Located data point offset relative to begin time [f].
	 *=====================================================================
	 * MODULE/LEVEL:  SERVICE/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    980225:  Original version, plagerized from gettime(). maf
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  
	 *===================================================================== */
	*nerr = 0;

	/* CHECKING PHASE: */

	    

	/* PROCEDURE: */

	/* Get the header of the appropriate file. */
	getfil ( fileNumber , FALSE , &unused1 , &unused2 , &unused3 , nerr ) ;

	zyear = *nzyear ;
	zdoy  = *nzjday ;
	zhour = *nzhour ;
	zmin  = *nzmin ;
	zsec  = (float) (*nzsec) + (float) (*nzmsec) / 1000.0 ;

	/* Find epoch time of reference time and add rvalue */
	value = hrToEpoch ( zyear, (int) 0, zdoy, zhour, zmin, zsec, nerr );
	
	value += (double) rvalue ;

	/* Convert value to a string */
	sprintf ( buff , "%17.5f" , value ) ;
	
	/* Move beyond blank spaces */
	for ( pBuff = buff ; *pBuff == ' ' ; pBuff++ )
	{ /* do nothing */ }

	/* Fill kvalue with dummy string which disguises epochtime as
	   a string so SAC's command line interpreter won't convert
	   it back to a single precision float before handing it off */
	sprintf ( kvalue , "epoch:%s" , pBuff ) ;

} /* end of function */



/* Time intervals */
#define SECPERMIN             (60)
#define SECPERHOUR            (60L*SECPERMIN)
#define SECPERDAY             (24L*SECPERHOUR)

#define MAXFORM 15
#define DATE_STRING_LEN 40


/* Macros ------------------------------------------------------------- */
/* floor(x/y), where x, y>0 are integers, using integer arithmetic */
#define qfloor( x, y ) ( x>0 ? (x)/y : -((y-1-(x))/y) )

/* Globals ------------------------------------------------------------ */
int  eomnth[2][15] = {
   { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365, 396, 424 },
   { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366, 397, 425 },
};
/*char *mon[]={ "", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };*/




double hrToEpoch( int year, int month, int day, int hour,
                    int min, float second , int * nerr ) {
   int *et = eomnth[isLeapYear(year)];
   int basedate = 2440588; /* astronomical julian day for 01/01/1970 */
   int julianDay;   /* astronomical julian day */
   int DeltaDays;   /* number of days relative to basedate */

   *nerr = 0 ;

   if( year == 0 ) {
      *nerr = 908 ;
      setmsg ( "ERROR" , 908 ) ;
      apcmsg ( "year = 0" , 9 ) ;
      outmsg () ;
      clrmsg () ;
   }

   if( month == 0 )
      doyToMD( day, isLeapYear( year), &month, &day );

   if( month < 1 || month > 12 ) {
      *nerr = 908 ;
      setmsg ( "ERROR" , 908 ) ;
      apcmsg ( "month out of range" , 19 ) ;
      outmsg () ;
      clrmsg () ;
   }


   if( day < 1 || day > et[month]-et[month-1] ) {
      *nerr = 908 ;
      setmsg ( "ERROR" , 908 ) ;
      apcmsg ( "day out of range" , 17 ) ;
      outmsg () ;
      clrmsg () ;
   }

   if( *nerr )
      return( 0.0 );


  julianDay = mdyToJul(month, day, year); /* get astronomical Julian day */
  DeltaDays = julianDay - basedate;

  return (double) DeltaDays*SECPERDAY + (double)hour*SECPERHOUR +
         (double)min*SECPERMIN + (double)second;

}




/* Calculate month, day from day of year  */
void doyToMD( int d, int lp, int *pm, int *pd )
{
   int  i;
   int  *et = eomnth[lp];
   for( i=1; d>et[i]; i++ );
   *pm = i;
   *pd = d - et[i-1];

   return;
}



/* Calculate day of year from month, day. */
int MDtoDoy ( int month , int day , int leap )
{
    int *et = eomnth[ leap ] ;

    return ( et[ month - 1 ] + day ) ;
}
    




/* Recognize leap years  */
int isLeapYear( int yr)
{
   int l;

   if( yr < 0 )
      yr++;
   l = ( yr%4 == 0 );
   l = l && ( yr%100 != 0 || yr%400 == 0 );

   return( l );
}




/* get astronomical Julian day from month,day,year */
int mdyToJul(int mm, int id, int iyyy)
{
        int jul;
        int ja,jy=iyyy,jm;
        int IGREG = 588829;

        if (jy == 0){
           setmsg("WARNING" , 1030 ) ;
	   apcmsg ( " Setting year to 1." , 15 );
	   outmsg () ;
	   clrmsg () ;
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

