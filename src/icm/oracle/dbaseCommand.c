#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "complex.h"
#include "proto.h"
#include "mach.h"

#include "hdr.h"
#include "timefuncs.h"
#include "EVRESPnames.h"
#include "../dbFuncs.h"

char *GetRESPfileNameFromDB(char *station, char *component, char *locid, 
                            double EpochTime , char * rtypeOut );
void mnday( int d, int lp, int *pm, int *pd );
int isleap( int yr) ;



void dbaseResponse(int nfreq, double delfrq, double* xre, double* xim, 
                   float* nmScale, int* nerr)
{

   char * file ;
   char resptype[ 8 ] ;
   int year , jday , month , day , hour , minute ;
   float second ;
   double EpochTime ;
   char station[ 10 ] , component[ 10 ], locid[ 10 ] ;

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
      char check[ 10 ] ;

      /* get locid from khole if it's a two letter string */
      strcpy( check , khole ) ;
      deblank( check ) ;
      if( strlen( check ) == 2 && isalnum( check[ 0 ] ) &&
                	 isalnum( check[ 1 ] ) )
	 strcpy( locid , check ) ;
      else
	 strcpy( locid , "*" ) ;
   }



   setUseDBName(1, getTransferDirection());

   if( isSet( TIME , getTransferDirection() ) ) {
      hour = getHour( getTransferDirection() ) ;
      minute = getMinute( getTransferDirection() ) ;
      second = getSecond( getTransferDirection() ) ;
   }
   else {
      hour = *nzhour ;
      minute = *nzmin ;
      second = *nzsec + ( (float) (*nzmsec) ) / 1000.0;
   }

   if( isSet( DATE , getTransferDirection() ) ) {
      year = getYear( getTransferDirection() ) ;
      jday = getJday( getTransferDirection() ) ;
   }
   else {
      year = *nzyear ;
      jday = *nzjday ;
   }

   mnday( jday, isleap(year), &month, &day );
   EpochTime = tmMakeEpochTime( year, month, day, hour, minute, second);
   file      = GetRESPfileNameFromDB(station, component, locid,
                                     EpochTime , resptype );
   if(! file) {
       char staComp[ 81 ] ;

       sprintf ( staComp , "for %s, %s." , kstnm, kcmpnm ) ;
       setmsg ( "WARNING" , 2115 ) ;
       apcmsg ( staComp , strlen ( staComp ) + 1 ) ;
       outmsg () ;
       clrmsg () ;
       return ;
   }

   if ( !strcmp ( resptype , "evresp" ) )
       *nerr=EvrespGateway( nfreq, delfrq, xre, xim , nmScale , file);
   else if ( !strcmp ( resptype , "sacpzf" ) )
       polezero( nfreq, delfrq, xre, xim, file, strlen( file ), nerr ); 
   else if ( !strcmp ( resptype , "fap" ) ||
             !strcmp ( resptype , "paz" ) ||
             !strcmp ( resptype , "fir" ) ||
             !strcmp ( resptype , "pazfir" ) )
   {
       /* most of this code was added 020710 when simple fap was replaced with
          code from the NDC that handles fap, paz, fir, and pazfir. maf */
       char dfile[MCPFN+1], dir[MCPFN+1] = "." ;
       char *lastSlash ;

       /* check if a path is present in filename. */
       lastSlash = strrchr( file , KDIRDL ) ;
       if( lastSlash ){
           strncpy( dir, file, lastSlash - file ) ;
           strcpy( dfile, lastSlash + 1 ) ;
       }
       else
          strcpy( dfile, file ) ;

/*     fap( nfreq, delfrq, xre, xim, file, strlen( file ), nerr ) ; */
       ndcTransfer( dir, dfile, delfrq, nfreq, xre, xim, nerr ) ;
   }
   else {
       *nerr = 2116 ;
       setmsg ( "ERROR" , 2116 ) ;
       outmsg () ;
       return ;
   }
}
/* ---------------------------------------------------------------------- */
