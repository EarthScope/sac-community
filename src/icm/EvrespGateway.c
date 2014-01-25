#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <ctype.h>

#include "complex.h"

#include "mach.h"
#include "../evalresp/evresp.h"
#include "hdr.h"
#include "extfunc.h" 
#include "EVRESPnames.h"
#include "timefuncs.h"


#include "ncpf.h"

/* #define MAX_FREQS 16384 */
#define MAX_FREQS 65536


void InterpolateArrays(double *freqs, int nfreqs, double *tmpRe, double *tmpIm, 
		       int nfreq, double *xre, double *xim )
{
   double F;
   double incr;
   int i, klo, khi;
   double ReSlope, ImSlope, df;

   incr = ( freqs[ nfreqs - 1] - freqs[0] ) / (nfreq - 1);

   F = freqs[0];
   klo = 0;
   khi = 1;
   ReSlope = (tmpRe[khi] - tmpRe[klo] ) / ( freqs[khi]-freqs[klo] );
   ImSlope = (tmpIm[khi] - tmpIm[klo] ) / ( freqs[khi]-freqs[klo] );
   for(i=0;i<nfreq;i++){
      df     = F - freqs[klo];
      xre[i] = tmpRe[klo] + df * ReSlope;
      xim[i] = tmpIm[klo] + df * ImSlope;

      F += incr;
      if( F > freqs[ nfreqs - 1] ) F = freqs[ nfreqs - 1];
      if( F > freqs[khi] ){
         klo++;
         khi++;
         ReSlope = (tmpRe[khi] - tmpRe[klo] ) / ( freqs[khi]-freqs[klo] );
         ImSlope = (tmpIm[khi] - tmpIm[klo] ) / ( freqs[khi]-freqs[klo] );
      }

   }
}
/* ----------------------------------------------------------------- */





void FillArrays(int nfreqs, struct response *first, double *xre, double *xim) 
{
   int i;
   struct response *resp;
   struct complex *output;

   resp = first;
   output = resp->rvec;
   for(i = 0; i < nfreqs; i++){
      *(xre + i) = output[i].real ;
      *(xim + i) = output[i].imag ;
   }
}
/* ----------------------------------------------------------------- */






static int notSet(arg) /* Check whether integer arg from sac header is set */
int arg;
{
   int INTNOTSET=-12345;
   if( arg == INTNOTSET)
      return 1;
   else
      return 0;   

}

static void setCurTime(t_o_day)
char * t_o_day ;
{
  struct tm *tod;
  time_t now;
  time(&now);
  tod = gmtime(&now);
  strftime(t_o_day, 12, "%T.000", tod);
}


static void getCurDate(curYear, dayOfYear)
int *curYear, *dayOfYear ;
{
   time_t now;
   struct tm *tmstruct;

   time(&now);
   tmstruct=localtime(&now);
   *curYear=tmstruct->tm_year+1900;
   *dayOfYear=tmstruct->tm_yday;

}




static void 
setTimeString(int   *hour, 
              int   *min, 
              int   *sec, 
              int   *msec,
              char  *t_o_day) {
  int Hour,Min, Sec, MSec;
   
   if(isSet(TIME, getTransferDirection())){
     Hour = getTime(getTransferDirection(), EV_HOUR);
     Min  = getTime(getTransferDirection(), EV_MIN);
     Sec  = getTime(getTransferDirection(), EV_SEC);
     MSec = getTime(getTransferDirection(), EV_MSEC);
     sprintf(t_o_day,"%02d:%02d:%02d.%03d",Hour,Min,Sec, MSec);
     return;
   }
   if( notSet(*hour) || notSet(*min) || notSet(*sec) ) {
      setCurTime(t_o_day);
   } else {
     sprintf(t_o_day,"%02d:%02d:%02d.%03d", *hour, *min, *sec, *msec);
   }
}



static void setDateString( year , jday , t_o_day , datime )  
int *year , *jday ;
char * t_o_day , *datime;
{
   int curYear,dayOfYear;

   if(isSet(DATE, getTransferDirection())){
      curYear=getYear(getTransferDirection());
      dayOfYear=getJday(getTransferDirection());
      sprintf(datime, "%4i,%03i,%s", curYear, dayOfYear, t_o_day);
      return;
   }


   if( notSet(*year) || notSet(*jday)  ){
      getCurDate(&curYear,&dayOfYear);
      sprintf(datime, "%4i,%03i,%s", curYear, dayOfYear, t_o_day);
   } 
   else   
      sprintf(datime, "%4i,%03i,%s", *year, *jday, t_o_day); 
   
} 
 
  

void deblank ( char *strg )
{
    int idx, jdx, len , iblank ;

    if ( !strg )
	return ;

    len = strlen ( strg ) ;

    /* count initial blanks */
    for ( iblank = 0 ; iblank < len ; iblank++ ) {
	if ( !isspace ( strg[ iblank ] ) )
	    break ;
    }

    /* fill in trailing blanks, decrement len as we go */
    while ( len > 0 && isspace ( strg[ len - 1 ] ) ) {
	strg[ len - 1 ] = '\0' ;
	len-- ;
    }

    /* if there are initial blanks, left-justify */
    if ( iblank ) {
	for ( idx = 0 , jdx = iblank ; jdx <= len ; idx++ , jdx++ ) {
	    strg[ idx ] = strg[ jdx ] ;
	}
    }
}

/*
void  deblank(char *strg)
{
  int i;
  int l;
  if( !strg ) return;
  l = strlen(strg);
  for (i=0;i<l;i++)
     if(strg[i] == ' ')
        strg[i]='\0';
}
* ---------------------------------------------------------------- */







int /*FUNCTION*/ EvrespGateway(int nfreq, double delfrq, double xre[], 
                               double xim[] , float *nmScale, char * inFile)
{
   int nfreqs, i ;
   char t_o_day[15];
   char datime[30];
   char net_code[10] = "         ";
   char units[4] = "   " ; 
   char locid[9] = "*";
   char check[ 9 ] = "        " ;
   int start_stage = -1, stop_stage = 0, stdio_flag = 0;
   double incr, freq_lims[2], *freqs;
   char *verbose = 0;
   struct response *first;
   char *file = 0;
   char rtype[] = "CS";
   double val;
   char station[10] = "         " , component[10] = "         " ;
   double *tmpRe, *tmpIm;
   int Interpolate;
   int FnameLen;
   struct stat status;

   strcpy ( net_code , "   " ) ;
   check[8] = 0;
   memset(t_o_day, 0, 15);
   memset(datime, 0, 30);
   setTimeString(nzhour,nzmin, nzsec, nzmsec ,t_o_day); 
   setDateString(nzyear,nzjday,t_o_day,datime); 

   if ( inFile ) {
      file = (char *) malloc ( strlen ( inFile ) ) ;
      if ( !file ) {
	 return 1301 ;
      }
      strcpy ( file , inFile ) ;
   }



 
  /* If number of points in trace is <= MAX_FREQS then just have evresp generate a
     transfer function for that number of points. However, if the number is larger,
     then have evresp generate a transfer function that is MAX_FREQS long and then
     interpolate the resulting transfer functions to the required number of points.
     This is because evresp gets quite slow when the number of points is > than a
     few thousand, but interpolation is relatively quick. The transfer functions 
     are very smooth so interpolation is not problematic.
     */
 
   freq_lims[0] = 0.0;
   freq_lims[1] = (nfreq-1)*delfrq;
   if( nfreq <= MAX_FREQS){
      nfreqs = nfreq;
      tmpRe = xre;
      tmpIm = xim;
      Interpolate = 0;
      incr = delfrq;
   }
   else{
      nfreqs = MAX_FREQS;
      tmpRe = (double *) malloc(nfreqs * sizeof( double ) );
      tmpIm = (double *) malloc(nfreqs * sizeof( double ) );
      Interpolate = 1;
      incr = ( freq_lims[1] - freq_lims[0] ) / (nfreqs - 1);
   }



/* allocate space for the frequencies and fill with appropriate values */

   freqs = alloc_double(nfreqs);
   for(i = 0, val = freq_lims[0]; i < nfreqs; i++) {
      freqs[i] = val;
      val += incr;
   }

   /* Get network name. */
   if(isSet(NETWORK, getTransferDirection())){
      strcpy(net_code,getNetworkName(getTransferDirection()));
   }
   else{
      if(strncmp(knetwk, "-12345",6) == 0){
         strcpy(net_code,"*"); 
      }
      else
         strcpy(net_code, knetwk);
   }
   deblank(net_code);


   strcpy(units,"DIS");
   *idep = IDISP ;
 

   /* Get station name. */
   if(isSet(STATION, getTransferDirection())){
      strcpy(station,getStationName(getTransferDirection()));
   }
   else{
      if(strncmp(kstnm, "-12345",6) == 0){
         strcpy(station,"*");
      }
      else
         strcpy(station, kstnm);
   }
   deblank(station);


   /* Get channel name. */
   if(isSet(CHANNEL, getTransferDirection())){
      strcpy(component,getChannelName(getTransferDirection()));
   }
   else{
      if(strncmp(kcmpnm, "-12345",6) == 0){
         strcpy(component,"*");
      }
      else
         strcpy(component, kcmpnm);
   }
   deblank(component);


   /* Get locid name. */
   if(isSet(LOCID, getTransferDirection())){
      strcpy(locid,getLocidName(getTransferDirection()));
   }
   else{
      /* get locid from khole if it's a two letter string */
      strcpy( check , khole ) ;
      deblank( check ) ;
      if( strlen( check ) == 2 && isalnum( check[ 0 ] ) &&
                                  isalnum( check[ 1 ] ) )
         strcpy( locid , check ) ;
      else
         strcpy( locid , "*" ) ;
   }
 


   if(isSet(FILENAME, getTransferDirection())){
      FnameLen = strlen( getFileName(getTransferDirection()) );
      if(file) free(file);
      file = (char *) malloc(FnameLen  + 1 );
      strcpy(file, getFileName(getTransferDirection()));
      if( stat( file, &status ) == -1){
         printf("User-specified file: (%s) not found.\n", file);
         printf("Looking in current directory for matching file...\n");
         free(file);
         file = 0;
      }
      else
         printf("Using response from user-specified file: (%s).\n",file);
   }

 
   printf(" Extracting evresp response for %s, %s...\n", station, component);
 /* 2010/02/03 added ,0*/
   first = evresp(station, component, net_code, locid, datime,units,file,freqs,nfreqs,
                  rtype,verbose,start_stage,stop_stage,stdio_flag,0);


   if( file){
      free(file);
      file = 0;
   }

   if( !first){
      free(freqs);
      if( Interpolate ){
         free(tmpRe);
         free(tmpIm);
      }
      return 2118 ;
   }

   /* set flag to correct data in transfer() */
   if( getTransferDirection() == FROM )
      (*nmScale) *= 1e09 ;
   else
      (*nmScale) /= 1e09 ;

   FillArrays(nfreqs, first, tmpRe, tmpIm);

   if( Interpolate ){
      InterpolateArrays(freqs, nfreqs, tmpRe, tmpIm, nfreq, xre, xim );
      free(tmpRe);
      free(tmpIm);
   }
   free(freqs);
   free_response(first);
   return 0;                 

}
