#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#ifdef POSIX
#include <fnmatch.h>
#endif
#include <math.h>

#define NULL_SITE
#define NULL_WFDISC
#define NULL_SITECHAN
#define NULL_ARRIVAL
#include "timefuncs.h"
#include "cssListOps/cssStrucs.h"
#include "cssListOps/cssListStrucs.h"
#include "cssListOps/dblUserData.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "cssListOps/dblErrors.h"
#include "smDataIO.h"
#include "stringfun.h"
#include "smMemory/smMemory.h"
#include "gse.h"
#include "string_utils.h"

#include "debug.h"

int ReadINTdata(FILE *ptr, int Nsamp, int *Data);


static int Verbose;
static int TracesInExistingTree;
static double MaxPhysMemToUse;


#define MAX_LINE 1025


static int IsBlank(char *line)
{
   char Tmp[MAX_LINE];
   char delimit[] = " \t\n";
   strcpy(Tmp, line);
   if(strtok(Tmp, delimit))return 0;
   return 1;
}
/* -------------------------------------------------------------- */



static float GetFloat(char *line, int *Off, float NullVal)
{
   char Tmp[MAX_LINE];
   int Start = *Off;
   int End   = *(Off + 1) - 1;
   int Len   = End - Start + 1;
   if(End > (int)strlen(line) - 1) return NullVal;


   strncpy(Tmp, line + Start, Len);
   Tmp[Len] = '\0';
   if(IsBlank(Tmp) ) return NullVal;
   return atof(Tmp);
}
/* -------------------------------------------------------------- */




static int GetInt(char *line, int *Off, float NullVal)
{
   char Tmp[MAX_LINE];
   int Start = *Off;
   int End   = *(Off + 1) - 1;
   int Len   = End - Start + 1;
   if(End > (int)strlen(line) - 1) return NullVal;


   strncpy(Tmp, line + Start, Len);
   Tmp[Len] = '\0';
   if(IsBlank(Tmp) ) return NullVal;
   return atol(Tmp);
}
/* -------------------------------------------------------------- */




int GetCompleteLine(char *line, int Maxlen, FILE *ptr)
{
   int N;
   char Tmp[MAX_LINE];
   *line = '\0';
   UNUSED(Maxlen);
   while( strlen(line) < MAX_LINE){
      if(!fgetsp(Tmp, MAX_LINE, ptr) )return 0;
      N = strlen(Tmp);
      Tmp[N - 1] = '\0';
      N = strlen(Tmp);
      if(Tmp[N - 1] == '\\'){
         Tmp[N - 1] = '\0';
         strcat(line, Tmp);
      }
      else{
         strcat(line, Tmp);
         return 1;
      }
   }
   
   return 1;


}
/* --------------------------------------------------------------------- */




FILE *OpenAndValidate(char *fileName)
{
   FILE *ptr;
   
   if(!fileName || ! strlen(fileName)){
      printf("Invalid (empty) GSE filename!\n");
      return 0;
   }

   ptr = fopen(fileName, "rt");
   if(!ptr){
      printf("ERROR: Could not open (%s).\n", fileName);
      return 0;
   }
   return ptr;
   /*
   if(!GetCompleteLine(line, MAX_LINE, ptr)){
      printf("ERROR: Failed to read message header.\n");
      fclose(ptr);
      return 0;
   }
   if(strncmp(line, "BEGIN GSE2.0", 12) ){
      printf("ERROR: Expected (BEGIN GSE2.0).\n");
      fclose(ptr);
      return 0;
   }
   if(!GetCompleteLine(line, MAX_LINE, ptr)){
      printf("ERROR: Failed to read message type.\n");
      fclose(ptr);
      return 0;
   }
   if(strncmp(line, "MSG_TYPE DATA", 13) ){
      printf("ERROR: Expected a DATA message.\n");
      fclose(ptr);
      return 0;
   }

   return ptr;
   */
}
/* --------------------------------------------------------------------- */








static void Deblank(char* c, int l)
{
   int i, j, k;
   c[l-1] = '\0';
   for(j=l-2;j>=0;j--) {
      if(c[j] == ' ')
         c[j] = '\0';
      else
         break ;
   }

   /* Left-justify the string */

   /* Count leading spaces */
   for ( k = 0 ; k < j ; k++ ) {
      if ( c[ k ] != ' ' )
         break ;
   }

   /* if there are leading spaces, shift the string */
   if ( k )
      for ( ++j , i = k ; i <= j ; i++ )
         c[ i - k ] = c[ i ] ;
}
/* ------------------------------------------------------------------------- */



int GetJdate(double time)
{
   int year, month, day, hour, min;
   float second;

   tmDecodeEpochTime( time, &year, &month, &day, &hour, &min, &second);

   return 1000 *year + yrday( month, day, isleap( year) );

}
/* --------------------------------------------------------------------------- */




static float *GetSamples(FILE *ptr, int Nsamp, char *datatype, struct wfdisc *w, DBlist tree)
{
   char line[MAX_LINE];
   float *Data;
   int   *TmpData;
   int j;
   struct siteList *si;
   struct site s = nullSite;

   if(Nsamp < 1) return 0;

   if(!GetCompleteLine(line, MAX_LINE, ptr) ){
      printf("ERROR: Expected DAT2 Line, but read failed!\n");
      return 0;
   }
   /* Added the following block to read the nonstandard 
      STA2 line in some GSE2.0 files  1/18/01 */
   if(!strncmp(line, "STA2", 4) ){
     char hdr[15], net[15], coord[15];
     float depth;
     hdr[0] = 0;
     net[0] = 0;
     coord[0] = 0;
     /* Assign lat and lon from the STA2 line */
     if(sscanff(line, "%4s %9s %9.5lf %10.5lf %12s %5.3lf %5.3lf",
                &hdr[0], &net[0], &s.lat, &s.lon, &coord[0], &s.elev, &depth) != 7) {
       fprintf(stderr, "gseRead2.x: Error reading STA2 line\n");
       return 0;
     }
     strncpy(s.sta, w->sta, strlen(w->sta));
     if( !( si = (struct siteList *) dblCreateTableInstance(tree, dbl_LIST_SITE)) ) {
       return 0;
     }

     dblCopyTableElement(dbl_LIST_SITE, &s, si);

     /* Add logic to assign lat and lon from the sta2 line */
     if(!GetCompleteLine(line, MAX_LINE, ptr) ){
       printf("ERROR: Read on GSE file failed\n");
       return 0;
     }
   }


   if(strncmp(line, "DAT2", 4) ){
      printf("ERROR: Expected DAT2 Line, but got (%s)!\n", line);
      return 0;
   }
   TmpData = (int*)   smMalloc(Nsamp * sizeof(int) );
   Data    = (float*) smMalloc(Nsamp * sizeof(float) );

   if(!strcmp(datatype, "INT") ){
      if( ReadINTdata(ptr, Nsamp, TmpData) ){
         for(j=0;j<Nsamp;j++)
            Data[j] = TmpData[j];
      } else {
	 smFree(Data);
	 Data = 0;
      }
   } else if(!strcmp(datatype, "CMP6") || !strcmp(datatype, "CM6") ||
             !strcmp(datatype, "CMP7") || !strcmp(datatype, "CMP8") ) {
     if(ReadCompData(ptr, Nsamp, TmpData, datatype) ){
         for(j=0;j<Nsamp;j++)
           Data[j] = TmpData[j];
     } else { 
       smFree(Data);
       Data = 0;
     }
   }

   smFree(TmpData);
   return Data;
}
/* --------------------------------------------------------------------------- */



static int MakeJdate(char *Date)
{
   char FullDate[30];
   double EpochTime;
   int year, month, day, hour, min;
   float second;

   strcpy(FullDate, Date);
   strcat(FullDate, "/00:00:00.000");
   EpochTime = tmStrToEpochTime(FullDate);
   if( tmDecodeEpochTime( EpochTime, &year, &month, &day, &hour, &min, &second ) == 0 )
      return( 0 );

   return year * 1000 + yrday( month, day, isleap( year) );
}
/* --------------------------------------------------------------------------- */



static void GetStationData(FILE *ptr, DBlist tree)
{
   char line[MAX_LINE];
   int Off[] = {0, 11, 21, 32, 40, 51};
   char OnDate[11];
   char OffDate[11];
   struct siteList *si;
   struct site s = nullSite;

   if(!GetCompleteLine(line, MAX_LINE, ptr) ){
      printf("ERROR: Expected Station header Line, but read failed!\n");
      return ;
   }

   while(GetCompleteLine(line, MAX_LINE, ptr) ){
      if(IsBlank(line))return;
      if(strlen(line) < 38) return; /* Too incomplete to use */
      strncpy(s.sta, line, 5);  Deblank(s.sta, 6);
      s.lat  = atof(line + Off[1]);
      s.lon  = atof(line + Off[2]);
      s.elev = atof(line + Off[3]);
      if(strlen(line) >= 49){
         strncpy(OnDate, line + Off[4],10); Deblank(OnDate, 11);
	 if ( !strlen ( OnDate ) )
             printf("WARNING: Station ondate not available.  Set to Jan. 1, 1970\n") ;
	 s.ondate = MakeJdate(OnDate);
      }
      if(strlen(line) >= 60){
         strncpy(OffDate, line + Off[5],10); Deblank(OffDate, 11);
	 if ( strlen ( OffDate ) )
	     s.offdate = MakeJdate(OffDate);
	 else
	     s.offdate = -1 ;
      }

      si = (struct siteList *) dblCreateTableInstance(tree, dbl_LIST_SITE);
      if(!si)return;

      dblCopyTableElement(dbl_LIST_SITE,&s,si);

   }

      

}
/* --------------------------------------------------------------------------- */




static int GetWaveformData(char *line, FILE *ptr, DBlist tree, int MaxWaveforms)
{
   struct wfdiscList *wf;
   struct wfdisc w = wfdisc_null;
   int wOff[] = {5, 29, 35, 44, 48, 57, 69, 80, 88};
   char TimeString[24];
   char datatype[4];
   float *Data;

   int WaveformsInMemory = dblGetNumWaveformsInMemory(tree) + TracesInExistingTree;


   if(WaveformsInMemory >= MaxWaveforms)return 0;


   strncpy(TimeString,line + wOff[0], 23); Deblank(TimeString, 24);
   w.time = tmStrToEpochTime(TimeString);

   strncpy(w.sta,   line + wOff[1], 5 ); Deblank(w.sta, 6);
   strncpy(w.chan , line + wOff[2], 3 ); Deblank(w.chan, 4);
   strncpy(datatype, line + wOff[3], 3); Deblank(datatype, 4);
   w.nsamp    = atol(line + wOff[4]);
   w.samprate = atof(line + wOff[5]);
   if(w.samprate <= 0.0){
      printf("ERROR: Negative sample rate. Ignoring trace.\n");
      return 0;
   }
   
   w.jdate    = GetJdate(w.time);

   w.endtime  = w.time + (w.nsamp - 1) / w.samprate;

   w.calib    = atof(line + wOff[6]);
   w.calper   = atof(line + wOff[7]);

   w.wfid     = dblNextAvailableWfid(tree);

   strncpy(w.instype , line + wOff[8], 6 ); Deblank(w.instype, 7);

   Data = GetSamples(ptr, w.nsamp, datatype, &w, tree);
   if(!Data) return 0;


   if(Verbose)printf("Adding station (%s) channel (%s)...\n",w.sta, w.chan);
   wf = (struct wfdiscList *) dblCreateTableInstance(tree, dbl_LIST_WFDISC);
   if(!wf)return 0;

   dblCopyTableElement(dbl_LIST_WFDISC,&w,wf);
   wf->seis->i = Data;

   WaveformsInMemory++;
   if(WaveformsInMemory == MaxWaveforms){
      printf("There are now %d waveforms in memory. Remainder will be skipped.\n",
                WaveformsInMemory);
      return 1;
   }
      
   if(smFracPhysMemUsed() > MaxPhysMemToUse){
      printf("Waveforms in SeisMgr memory are using more than %5.2f%% of physical memory.\n",
                MaxPhysMemToUse * 100);
      printf("No more waveforms will be read.\n");
      printf("To utilize a higher percentage of physical memory use the MAXMEM option.\n");                
      return 1;
   }

   return 1;
}
/* -------------------------------------------------------------------- */  



static void GetChannelData(FILE *ptr, DBlist tree)
{
   char line[MAX_LINE];
   int Off[] = {0, 6, 44, 51, 58, 84, 95};
   char OnDate[11];
   char OffDate[11];
   struct sitechanList *sc;
   struct sitechan s = nullSitechan;

   if(!GetCompleteLine(line, MAX_LINE, ptr) ){
      printf("ERROR: Expected Channel header Line, but read failed!\n");
      return ;
   }

   while(GetCompleteLine(line, MAX_LINE, ptr) ){
      if(IsBlank(line))return;
      if(strlen(line) < 9) return; /* Too incomplete to use */
      strncpy(s.sta, line, 5);  Deblank(s.sta, 6);
      strncpy(s.chan, line + Off[1], 3); Deblank(s.chan, 4);
      s.edepth  = atof(line + Off[2]);
      s.hang    = atof(line + Off[3]);
      s.vang    = atof(line + Off[4]);
      if(strlen(line) >= 93){
         strncpy(OnDate, line + Off[5],10); Deblank(OnDate, 11);
         if ( !strlen ( OnDate ) )
             printf("WARNING: Channel ondate not available.  Set to Jan. 1, 1970\n") ;
	 s.ondate = MakeJdate(OnDate);
      }
      if(strlen(line) >= 104){
         strncpy(OffDate, line + Off[6],10); Deblank(OffDate, 11);
	 if ( strlen ( OffDate ) )
	     s.offdate = MakeJdate(OffDate);
	 else
	     s.offdate = -1 ;
      }

      sc = (struct sitechanList *) dblCreateTableInstance(tree, dbl_LIST_SITECHAN);
      if(!sc)return;

      dblCopyTableElement(dbl_LIST_SITECHAN,&s,sc);

   }
}
/* -------------------------------------------------------------------- */  




static void GetArrivalData(FILE *ptr, DBlist tree)
{
   char line[MAX_LINE];
   int Off[] = {0, 6, 13, 19, 20, 21, 23, 31, 42, 53, 59, 65, 72, 78, 84, 85, 86, 
                88, 94, 104, 110, 112, 117, 119, 124, 132};
   struct arrivalList *ar;
   struct arrival a = nullArrival;
   char DateTime[22];

   if(!GetCompleteLine(line, MAX_LINE, ptr) ){
      printf("ERROR: Expected Arrival header Line, but read failed!\n");
      return ;
   }

   while(GetCompleteLine(line, MAX_LINE, ptr) ){
      if(IsBlank(line))return;
      if(strlen(line) < 51) return; /* Too incomplete to use (not even a time) */
      strncpy(a.sta, line, 5);  Deblank(a.sta, 6);
      if(line[ Off[4] ] != ' '){
         a.fm[0] = line[ Off[4] ];
	 a.fm[1] = '\0';
      }
      if(line[ Off[5] ] != ' '){
         a.qual[0] = line[ Off[5] ];
	 a.qual[1] = '\0';
      }
      if(line[ Off[6] ] != ' '){
	 strncpy(a.iphase, line + Off[6], 7); 
	 Deblank(a.iphase, 8);
      }
      strncpy(DateTime, line + Off[7], 21); Deblank(DateTime, 22);
      if ( !strlen ( DateTime ) ) {
	 /* if there is no time, there is no arrival */
	 printf ( "Error:  Arrival skipped because it had no time.\n" ) ;
	 return ;
      }
      a.time = tmStrToEpochTime(DateTime);
      DateTime[10] = '\0';
      a.jdate = MakeJdate(DateTime);


      a.azimuth    = GetFloat(line, Off + 10, a.azimuth);
      a.delaz      = GetFloat(line, Off + 11, a.delaz);
      a.slow       = GetFloat(line, Off + 12, a.slow);
      a.delslo     = GetFloat(line, Off + 13, a.delslo);
      a.snr        = GetFloat(line, Off + 17, a.snr);
      a.amp        = GetFloat(line, Off + 18, a.amp);
      a.per        = GetFloat(line, Off + 19, a.per);
      a.arid       = GetInt  (line, Off + 24, a.arid);

      
      ar = (struct arrivalList *) dblCreateTableInstance(tree, dbl_LIST_ARRIVAL);
      if(!ar)return;

      dblCopyTableElement(dbl_LIST_ARRIVAL, &a, ar);
   }

}
/* -------------------------------------------------------------------- */  



/* static void GetOriginData(FILE *ptr, DBlist tree) */
/* { */
/*    char line[MAX_LINE]; */
/*    char line2[MAX_LINE]; */
/*    char line3[MAX_LINE]; */
/*    int Off1[] = {0, 11, 22, 25, 34, 44, 47, 53, 56, 61, 66, 71, 73, 78, 82,  */
/*                  84, 89, 93, 95, 100, 104, 114, 121}; */
/*    struct originList *orig; */
/*    struct origin o = nullOrigin; */
/*    char DateTime[22]; */
/*    char *token; */
/*    fpos_t pos; */

/*    /\* First header line is required. Second is optional. *\/ */
/*    if(!GetCompleteLine(line, MAX_LINE, ptr) ){ */
/*       printf("ERROR: Expected Origin header Line, but read failed!\n"); */
/*       return ; */
/*    } */
/*    token = strtok(line, " \t"); */
/*    if(strcmp(token, "Date") ){ */
/*       printf("PROBLEM: Expected first origin header line to start with 'Date'!\n"); */
/*       return; */
/*    } */

/*    if(!GetCompleteLine(line, MAX_LINE, ptr) )return; */
/*    token = strtok(line, " \t"); */
/*    if(token && !strcmp(token, "rms"))   /\* This was header 2, so waste a line. *\/ */
/*       GetCompleteLine(line, MAX_LINE, ptr);    */

/*    while(1){ */
/*       fgetpos(ptr, &pos); */
/*       GetCompleteLine(line, MAX_LINE, ptr); */
/*       if(!strncmp(line, "DATA", 4) ){ */
/*          fsetpos(ptr, &pos); */
/* 	 return; */
/*       } */
/*       GetCompleteLine(line2, MAX_LINE, ptr); */
/*       strcpy(line3, line); */
/*       token = strtok(line3, " \t\n"); */
/*       if(!token)break; */

/*       /\* Make sure this looks like a date-time "####/##/##"... *\/ */
/*       strncpy(DateTime, line + Off1[0], 21); Deblank(DateTime, 22); */
/*       if(DateTime[4] != '/' || DateTime[7] != '/')return; */
/*       o.time   = tmStrToEpochTime(DateTime); */
/*       o.lat    = GetFloat(line, Off1 + 3, o.lat); */
/*       o.lon    = GetFloat(line, Off1 + 4, o.lon); */
/*       o.depth  = GetFloat(line, Off1 + 6, o.depth); */
/*       o.ndef   = GetInt  (line, Off1 + 8, o.ndef); */



/*       o.orid  = dblNextAvailableOrid(tree); */
/*       o.evid  = dblNextAvailableEvid(tree); */
/*       orig = (struct originList *) dblCreateTableInstance(tree, dbl_LIST_ORIGIN); */
/*       if(!orig)return; */

/*       dblCopyTableElement(dbl_LIST_ORIGIN, &o, orig); */
/*       strcpy(orig->element->lddate, tmListEpochTime( tmGetEpochTime(), 18 ) ); */

      
/*       GetCompleteLine(line, MAX_LINE, ptr); /\* blank line following origin *\/ */
/*    }       */
   



/* } */
/* -------------------------------------------------------------------- */  





static void AssociateTables(DBlist tree)
{
   struct wfdiscList *w    = 0 ;
   struct wftagList *wt    = 0 ;
   struct eventList *ev    = 0 ;
   struct originList *orig = 0 ;



   orig = (struct originList *)dblNextTableInstance(orig, tree, dbl_LIST_ORIGIN);
   if( !orig ) return ;

   do{
      w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC);
      if(!w)break;
      dblAddComment(tree, dbl_LIST_WFDISC, w, "Converted from GSE format");

      ev = (struct eventList *) dblCreateTableInstance(tree, dbl_LIST_EVENT ) ;

      if(ev){   /* Fill ev and add a wftag struct linking event to wfdisc... */
         ev->element->evid = orig->element->evid ;
         ev->element->prefor = orig->element->orid ;
         strcpy( ev->element->evname , "-" ) ;
         strcpy( ev->element->auth , orig->element->auth ) ;
         ev->element->commid = orig->element->commid ;
         strcpy( ev->element->lddate , orig->element->lddate ) ;

         wt = (struct wftagList *) dblCreateTableInstance(tree, dbl_LIST_WFTAG);
         strcpy(wt->element->tagname, "evid");
         wt->element->tagid = ev->element->evid;
         wt->element->wfid  = w->element->wfid;
         strcpy(wt->element->lddate, w->element->lddate);
      }

   }while(w);

}
/* -------------------------------------------------------------------- */  




 
   










   
static int ReadGSEFile(FILE *ptr, DBlist tree, int MaxWaveforms)
{
   char line[MAX_LINE];
   int Ntraces = 0;
      
   while(GetCompleteLine(line, MAX_LINE, ptr) ){
   
      if(!strncmp(line, "WID2", 4) && smFracPhysMemUsed() <= MaxPhysMemToUse &&
         Ntraces + TracesInExistingTree < MaxWaveforms)
         Ntraces += GetWaveformData(line, ptr, tree, MaxWaveforms);
         
      else if(!strncmp(line, "DATA_TYPE STATION", 17) )
         GetStationData(ptr, tree);
         
      else if(!strncmp(line, "DATA_TYPE CHANNEL", 17) )
         GetChannelData(ptr, tree);
         
      else if(!strncmp(line, "DATA_TYPE ARRIVAL GSE2.0", 24) )
         GetArrivalData(ptr, tree);
         
      /* There is no way to relate origin information to a waveform
	 in GSE 2.0, so for now we don't even read it.  When this
	 problem is fixed, we can make use of this code again. */
/*      else if(!strncmp(line, "DATA_TYPE ORIGIN GSE2.0", 23) )
         GetOriginData(ptr, tree); */
   
   }

   return Ntraces;
}
/* -------------------------------------------------------------------------------- */






int gseRead20(char *fileName, char *WorkSetName, int Replace, int MaxWaveforms, int verbose,
	      double MaxPhysMem )
{
   DBlist tree;
   DBlist tree2;
   FILE *ptr;
   int TracesRead  = 0;
   MaxPhysMemToUse = MaxPhysMem;
   Verbose         = verbose;

   if(!WorkSetName || !strlen(WorkSetName) ){
      printf("Invalid worksetname! Cannot add GSE data to workset.\n");
      return 0;
   }

   if(Replace){
      smDeleteWorksetByName( (char*)WorkSetName );
      smCreateEmptyWorkset( (char*)WorkSetName );  /* This workset is now the default. */
   }
   else
      if(!smChangeDefaultWorksetByName( (char*)WorkSetName ))
         smCreateEmptyWorkset((char*) WorkSetName );  /* This workset is now the default. */

   tree2 = smGetDefaultTree();
   if(!tree2) tree2 = smMakeDefaultTree(); /* This is the returned tree. */
   
   /* Check for too many traces in memory... */
   TracesInExistingTree = dblGetNumWaveformsInMemory(tree2);
   if(MaxWaveforms <= TracesInExistingTree){
      if(Verbose)
         printf("%d traces already in memory! No traces will be read.\n",
                dblGetNumWaveformsInMemory(tree2));
      return 0;
   }


   /* Check for not enough memory ... */
   if(smFracPhysMemUsed() > MaxPhysMemToUse){
      if(Verbose){
         printf("Waveforms already in SeisMgr memory are using more than %5.2f%% of physical memory.\n",
                MaxPhysMemToUse * 100);
         printf("No waveforms will be read from this file.\n");
         printf("To utilize a higher percentage of physical memory use the MAXMEM option.\n");
      }
      return 0;                
   }



   ptr = OpenAndValidate(fileName);
   if(!ptr)return 0;

   tree = dblNewTree(); /* This is a temp tree which will be merged after filling. */


   TracesRead = ReadGSEFile(ptr, tree, MaxWaveforms); 
   fclose(ptr);
   AssociateTables(tree);

   tree2 = dblMergeTrees(tree2, tree, FALSE );
   if(tree2 != tree) {
       dblDeleteTree(tree);
   }


   if(TracesRead && Verbose){
      printf("%d traces read into CSStree\n", TracesRead);
   }

   return TracesRead;
}
/* -------------------------------------------------------------------------------------------- */   
