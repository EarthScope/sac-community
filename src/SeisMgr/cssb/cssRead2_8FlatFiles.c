#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cssb.h"
#include "timefuncs.h"
#define NULL_ORIGIN
#define NULL_ARRIVAL
#define NULL_WFDISC
#include "cssListOps/cssStrucs.h"
#include "cssListOps/cssListStrucs.h"
#include "cssListOps/dblUserData.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "cssListOps/dblErrors.h"
#include "smDataIO.h"
#include "stringfun.h"
#include "smMemory/smMemory.h"
#include "string_utils.h"


char *cssConstructFilename(char *Root, char *Suffix);
static int Verbose;
static int TracesInExistingTree;
static double MaxPhysMemToUse;


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



static char* ExtractPath(char* s)
{
   char* p = strrchr(s, '/');
   *p = '\0';
   return s;
}
/* ------------------------------------------------------------------------- */




static int InSpecList(char* sta, char* chan, char** Station, int Nstation, 
                       char** Channel, int Nchannel, char** Band, int Nband, 
                       char** Code, int Ncode, char** Orientation, 
                       int Norientation)
{
   int position;
   
   if(Station && Nstation)
      if(!InList(sta, Station, Nstation) ) return 0;

   if(Channel && Nchannel)
      if(!InList(chan, Channel, Nchannel) ) return 0;
      
   if(Band && Nband){
      position = 0;
      if(!InOneCharList(chan, Band, Nband, position) ) return 0;
   }
      
   if(Code && Ncode){
      position = 1;
      if(!InOneCharList(chan, Code, Ncode, position) ) return 0;
   }
      
   if(Orientation && Norientation){
      position = 2;
      if(!InOneCharList(chan, Orientation, Norientation, position) ) return 0;
   }
      
   return 1;

}
/* ------------------------------------------------------------------------ */







static int TimeStaChanMatch(double time, char *sta, char *chan, DBlist tree)
{

   struct wfdiscList *w = 0;
   do{
      if(!( w = (struct wfdiscList *)
            dblNextTableInstance(w, tree, dbl_LIST_WFDISC) ) )break;
      if(!strcmp(sta, w->element->sta) )
         if(!strcmp(chan, w->element->chan) )
            if(time >= w->element->time && time <= w->element->endtime)
               return w->element->wfid ;
   }while(w);
   return 0;
}
/* ------------------------------------------------------------------------ */              


                       

/* Read version 2.8 wfdisc file. */
static int AddToWfdisc(char* buffer, DBlist tree, char** Station, int Nstation, 
                       char** Channel, int Nchannel, char** Band, int Nband, 
                       char** Code, int Ncode, char** Orientation, 
                       int Norientation, char** FileList, int Nfiles,
                       char* WfdiscPath)
                      
{
   struct wfdisc w;
   char AlternateDir[256];
   struct wfdiscList *wf;
   int wOff[] = {  0,   9,  25,  32,  35,  44,  56,  66,  74,
                  81,  83,  86,  88,  97, 106, 137, 158, 169, 178};

   w = wfdisc_null;


   
   strncpy(w.sta,   buffer + wOff[2], 6 ); Deblank(w.sta, 7);
   strncpy(w.chan , buffer + wOff[3], 2 ); Deblank(w.chan, 3);
   if(! InSpecList(w.sta, w.chan, Station, Nstation, Channel, Nchannel, 
                    Band, Nband, Code, Ncode, Orientation, Norientation) )
      return 0;
   
   w.time     = atof(buffer + wOff[1]);
   w.wfid     = atol(buffer + wOff[13]);
   w.chanid     = atol(buffer + wOff[12]);
   w.jdate     = atol(buffer + wOff[0]);
   w.nsamp    = atol(buffer + wOff[4]);
   w.samprate   = atof(buffer + wOff[5]);
   w.calib    = atof(buffer + wOff[6]);
   w.calper   = atof(buffer + wOff[7]);

                            
   strncpy(w.instype , buffer + wOff[8], 6 ); Deblank(w.instype, 7);
   strncpy(w.segtype , buffer + wOff[9], 1 ); Deblank(w.segtype, 2);
   strncpy(w.dattype , buffer + wOff[10], 2 ); Deblank(w.dattype, 3);
   strncpy(w.clip    , buffer + wOff[11], 1 ); Deblank(w.clip, 2);
   strncpy(w.dfile   , buffer + wOff[15], 20); Deblank(w.dfile, 21);

   if(!FileExists(w.dir, w.dfile) ){
      strcpy(AlternateDir,WfdiscPath);
      strcat(AlternateDir, "/");
      strcat(AlternateDir, w.dir);
      if(!FileExists(AlternateDir, w.dfile) ){ 
         strcpy(w.dir, WfdiscPath); /* Use data file path for wfdisc file. */
         if(!FileExists(w.dir, w.dfile) ){
            printf("Cannot open waveform file(%s). Skipping trace...\n",w.dfile);
            return 0;
	 }
      }
      else
	 strcpy(w.dir, AlternateDir);
   }


   w.foff     = atol(buffer + wOff[16]);
   strcpy(w.lddate, tmListEpochTime( tmGetEpochTime(), 18 ) );

   if( FileList && Nfiles)
      if(! InList(w.dfile, FileList, Nfiles) )
	 return 0;

   if(Verbose)printf("Adding station (%s) channel (%s)...\n",w.sta, w.chan);
   
   wf = (struct wfdiscList *) dblCreateTableInstance(tree, dbl_LIST_WFDISC);
   if(!wf)return 0;
   dblCopyTableElement(dbl_LIST_WFDISC,&w,wf);
   if(!dblGetSeismograms( wf , 0 , 0 ) ){
      printf("Problem reading seismogram. Freeing wfdisc struct...\n");
      dblDeleteTableInstance(dbl_LIST_WFDISC, tree, wf);
   }
   
   return 1;
}
/* ------------------------------------------------------------------------ */              






static int ReadWfdiscFile(char* Root,  char *WorkSetName, DBlist tree, char** Station, 
                          int Nstation, char** Channel, int Nchannel,
                          char** Band, int Nband,char** Code, int Ncode,
                          char** Orientation, int Norientation, int Replace,
                          char** FileList, int Nfiles,
                          int MaxAllowableWaveforms)
{
   int LinesUsed = 0;
   FILE *ptr;
   char *FileName;
   char *WfdiscPath;
   char WfSuffix[] = ".wfdisc";

   char buffer[300];   /* Holds lines read from CSS files */
   int  buflen = 300;
   int  Lines;


   int WaveformsInMemory = dblGetNumWaveformsInMemory(tree) +
                           TracesInExistingTree;


   /* Open the wfdisc file */

   FileName = cssConstructFilename(Root, WfSuffix);
   if(! (ptr = fopen( FileName, "r" ) ) ){
      printf("ERROR: Unable to open file (%s).\n",FileName);
      if(Replace)smDeleteWorksetByName( (char*)WorkSetName );
      smFree(FileName);
      return 0;
   }
   
   WfdiscPath = ExtractPath(FileName); /* may need this to read seismograms. */
   /* Read the wfdisc file... */
   while( fgetsp(buffer, buflen, ptr) ){
      if( !strlen(buffer) || *buffer == '\n' )continue;
      Lines = AddToWfdisc(buffer, tree, Station, Nstation, Channel, Nchannel, 
                               Band, Nband, Code, Ncode, Orientation, 
                               Norientation, FileList, Nfiles, WfdiscPath);
      LinesUsed         += Lines;
      WaveformsInMemory += Lines;
      if(WaveformsInMemory == MaxAllowableWaveforms){
         printf("There are now %d waveforms in memory. Remainder will be skipped.\n",
                WaveformsInMemory);
         break;
      } 

      if(smFracPhysMemUsed() > MaxPhysMemToUse){
         printf("Waveforms in SeisMgr memory are using more than %5.2f%% of physical memory.\n",
                MaxPhysMemToUse * 100);
         printf("No more waveforms will be read.\n");
         printf("To utilize a higher percentage of physical memory use the MAXMEM option.\n");                
         break;
      }
      
      
   }
   
   fclose(ptr);
   smFree(FileName);
   return LinesUsed;
}
/* ------------------------------------------------------------------------ */              








static void AddOriginLine(char* buffer, int* orOffset, DBlist tree)
{
   struct eventList *ev = 0 ;
   struct wftagList *wt = 0 ;
   struct originList *o = (struct originList *)
                          dblCreateTableInstance(tree, dbl_LIST_ORIGIN);
   if(!o)return;

   *(o->element) = nullOrigin;

   o->element->lat    = atof(buffer + orOffset[2]);
   o->element->lon    = atof(buffer + orOffset[3]);
   o->element->depth  = atof(buffer + orOffset[4]);
   o->element->time   = atof(buffer + orOffset[1]);

   o->element->orid   =  dblNextAvailableOrid(tree);
   o->element->evid   =  dblNextAvailableEvid(tree);

   o->element->jdate  = atol(buffer + orOffset[0]);
   o->element->nass   = atol(buffer + orOffset[9]);
   o->element->ndef   = atol(buffer + orOffset[10]);
   o->element->ndp    = atol(buffer + orOffset[11]);
   o->element->grn    = atol(buffer + orOffset[17]);
   o->element->srn    = atol(buffer + orOffset[18]);
   strncpy(o->element->etype, buffer + orOffset[21], 7);
   Deblank(o->element->etype, 8); 
   o->element->depdp  = atof(buffer + orOffset[14]);
   strncpy(o->element->dtype, buffer + orOffset[20], 1);
   Deblank(o->element->dtype, 2); 

   o->element->mb     = atof(buffer + orOffset[5]);
   o->element->ms     = atof(buffer + orOffset[6]);
   strncpy(o->element->auth, buffer + orOffset[22], 15);
   Deblank(o->element->auth, 16); 

   ev = (struct eventList *) dblCreateTableInstance(tree, dbl_LIST_EVENT ) ;
   if( ev ) {
      struct wfdiscList *w = 0 ;

      ev->element->evid = o->element->evid ;
      ev->element->prefor = o->element->orid ;
      strcpy( ev->element->auth , o->element->auth ) ;

      do{
         w = (struct wfdiscList *) dblNextTableInstance(w, tree,  dbl_LIST_WFDISC);
         if( !w )
            break ;

         wt = (struct wftagList *) dblCreateTableInstance(tree, dbl_LIST_WFTAG);
         if( wt ) {
            wt->element->wfid = w->element->wfid ;
            strcpy( wt->element->tagname , "evid" ) ;
            wt->element->tagid = o->element->evid  ;
            strcpy( wt->element->lddate , w->element->lddate ) ;
         }
      }while( w ) ;

      if( wt ) 
         strcpy( ev->element->lddate , wt->element->lddate ) ;
   }
}
/* ---------------------------------------------------------------------------------- */              




         

static void ReadOriginFile(char* Root, DBlist tree)
{
   FILE *ptr;
   char *FileName;
   char OrSuffix[] = ".origin";
   char buffer1[300], buffer2[300];   
   int  buflen     = 300;
   int orOff[] = { 0,   9,  25,  35,  45,  55,  62,  69,  77,  80,
                  85,  90,  95, 100, 105, 115, 124, 133, 137, 141,
                 146, 148, 156, 172, 188, 190};

   FileName = cssConstructFilename(Root, OrSuffix);
   if(! (ptr = fopen( FileName, "r" ) ) ){
      if(Verbose)printf("Missing schema file \t(%s).\n",FileName);
      smFree(FileName);
      return;
   }

   /* Version 2.8 has no obvious way to connect origins to wfdiscs, so the
      strategy is to read the origin if and only if there is exactly one,
      origin record in the origin file, and make event and wftag on the fly. */

    if( !fgetsp(buffer1, buflen, ptr) ){
       fclose(ptr);
       smFree(FileName);
       return ;
    }

    if( fgetsp(buffer2, buflen, ptr) ){
       fclose(ptr);
       smFree(FileName);
       return ;
    }

    AddOriginLine(buffer1, orOff, tree);
            
    fclose(ptr);
    smFree(FileName);

}
/* ------------------------------------------------------------------------ */              





         

/* static void ReadStructureFile(char* Root, DBlist tree, char *Suffix, */
/*                               char *StructType, void (*fp)(char*, void*)) */
/* { */
/*    FILE *ptr; */
/*    char *FileName; */
/*    char buffer[300];    */
/*    int  buflen     = 300; */
 
/*    FileName = cssConstructFilename(Root, Suffix); */
/*    if(! (ptr = fopen( FileName, "r" ) ) ){ */
/*       if(Verbose)printf("Missing schema file \t(%s).\n",FileName); */

/*       smFree(FileName); */
/*       return; */
/*    } */
/*    else{ */
/*       while( fgetsp(buffer, buflen, ptr) ){ */
/*          if( !strlen(buffer) || *buffer == '\n' )continue; */
/*          fp(buffer, tree); */
/*       } */
            
/*       fclose(ptr); */
/*       smFree(FileName); */
/*    } */

/* } */
/* ------------------------------------------------------------------------ */              








static void AddArrivalLine(char* buffer, int* Off, DBlist tree, int wfid)
{
   struct arrivalList *a = (struct arrivalList *)
                            dblCreateTableInstance(tree, dbl_LIST_ARRIVAL);
   struct wftagList *wt = 0 ;
   if(!a)return;

   *(a->element) = nullArrival;
   strncpy(a->element->sta, buffer + Off[2], 6);
   Deblank(a->element->chan, 7);
   a->element->time    = atof(buffer + Off[1]);
   a->element->arid    = atol(buffer + Off[2]);
   a->element->jdate   = atol(buffer + Off[0]);
   a->element->chanid  = atol(buffer + Off[20]);
   strncpy(a->element->chan, buffer  + Off[3], 2);
   Deblank(a->element->chan, 3);
   strncpy(a->element->iphase, buffer+ Off[6], 8);
   Deblank(a->element->iphase, 9);
   strncpy(a->element->stype, buffer + Off[16], 1);
   Deblank(a->element->stype, 2);
   a->element->slow    = atof(buffer + Off[13]);
   a->element->ema     = atof(buffer + Off[14]);
   a->element->amp     = atof(buffer + Off[8]);
   a->element->per     = atof(buffer + Off[9]);
   a->element->logat   = atof(buffer + Off[10]);
   strncpy(a->element->clip, buffer  + Off[17], 1);
   Deblank(a->element->clip, 2);
   strncpy(a->element->fm, buffer    + Off[7], 2);
   Deblank(a->element->fm, 3);
   strncpy(a->element->qual, buffer  + Off[5], 1);
   Deblank(a->element->qual, 2);
   strncpy(a->element->auth, buffer  + Off[22], 15);
   Deblank(a->element->auth, 16);

   /* now make an arid wftag */
   wt = (struct wftagList *) dblCreateTableInstance(tree, dbl_LIST_WFTAG ) ;
   if( wt ) {
      wt->element->wfid = wfid ;
      strcpy( wt->element->tagname, "arid" ) ;
      wt->element->tagid = a->element->arid ;
   }

}
/* ------------------------------------------------------------------------ */              







static int InAuthorList(char *auth, char** Author, int Nauthor)
{
   int j;
   if(!Nauthor || !Author ) return 1; /* No List means match anything! */

   for(j=0;j<Nauthor;j++)
      if(!strcmp(auth, Author[j]) ) return 1;

   return 0;
}
/* ------------------------------------------------------------------------ */              





static int InPhaseList(char *iphase, char** Phaselist, int Nphases)
{
   int j;
   if(!Phaselist || !Nphases ) return 1; /* No List means match anything! */

   for(j=0;j<Nphases;j++)
      if(!strcmp(iphase, Phaselist[j]) ) return 1;

   return 0;
}
/* ------------------------------------------------------------------------ */              





static void AddMatchingArrivalStruct(char *buffer, DBlist tree, char** Author,
                                     int Nauthor, char** Phaselist, int Nphases)
{
  int arOff[] = { 0,   9,  25,  32,  35,  37,  39,  48,  51,  62,
                 70,  78,  85,  93, 101, 109, 117, 119, 121, 130,
                139, 148, 157, 173};
   int wfid = 0 ;
   double time;
   char sta[7];
   char chan[3];
   char auth[16];
   char iphase[9];

   time = atof(buffer + arOff[1]);
   strncpy(sta, buffer + arOff[2], 6);    Deblank(sta, 7);
   strncpy(chan, buffer + arOff[3], 2);   Deblank(chan, 3);
   strncpy(auth, buffer + arOff[22], 15); Deblank(auth, 16);
   strncpy(iphase, buffer + arOff[6], 8); Deblank(iphase, 9);
   if(!InAuthorList(auth, Author, Nauthor) ) return;
   if(!InPhaseList(iphase, Phaselist, Nphases) ) return;

   wfid = TimeStaChanMatch(time, sta, chan, tree) ;
   if( wfid > 0 ) {
      AddArrivalLine(buffer, arOff, tree, wfid);
      return;
   }
}
/* ------------------------------------------------------------------------ */              


         
      

static void ReadArrivalFile(char* Root, DBlist tree, char** Author,
                            int Nauthor, char** Phaselist, int Nphases)
{
   FILE *ptr;
   char *FileName;
   char ArSuffix[] = ".arrival";
   char buffer[300];   
   int  buflen     = 300;
 

   FileName = cssConstructFilename(Root, ArSuffix);
   if(! (ptr = fopen( FileName, "r" ) ) ){
      if(Verbose)printf("Missing schema file \t(%s).\n",FileName);
      smFree(FileName);
      return;
   }
   else{
      while( fgetsp(buffer, buflen, ptr) ){
         if( !strlen(buffer) || *buffer == '\n' )continue;
         AddMatchingArrivalStruct(buffer, tree, Author, Nauthor,
                                  Phaselist, Nphases);
      }
            
      fclose(ptr);
      smFree(FileName);
   }

}
/* ------------------------------------------------------------------------ */              
         

         















int cssRead2_8FlatFiles(char* Root, char *WorkSetName, int Replace, 
                        char** Station, int Nstation, 
                        char** Channel, int Nchannel, 
                        char** Band, int Nband, 
                        char** Code, int Ncode,
                        char** Orientation, int Norientation, 
                        char** Author, int Nauthor,
                        char** Filelist, int Nfiles,
		        char** Phaselist, int Nphases,
                        int MaxAllowableWaveforms, int verbose,
                        double MaxPhysMem);



/*    
    
      Root is the basename of the CSS flat files to be read.  
      
      WorkSetName is the name of the Workset to which structures will be added. If
      WorkSetName is NULL, then a default workset is created.
      
      If Replace is zero, then the new structures are appended to the tree in
      WorkSetName. Otherwise, The tree is cleared before adding the new structures.
      
      It is possible to restrict the lines read from the wfdisc file by means of
      the Station, Channel, Band, Code, and Orientation arrays. If these arrays
      are empty no restrictions are applied, and all lines are read. Otherwise
      restrictions are applied in the following manner:
          Station -------- Only lines whose sta matches one orig more strings from Station
                           are used. Station[j] may contain the wildcards '*' and '?'
                           which are interpreted in the usual manner.
                           
          Channel -------- Only lines whose chan matches one orig more of the strings in
                           Channel are used. Channel may also contain wildcards.
                           
          Band ----------- Each string in this array should be one-character long.
                           The first letter of chan is compared to each of these strings,
                           and if it matches, the line is used. Band[j] may also be
                           a wildcard, which gives the result of matching any band.
                           This can be accomplished more easily by setting Band and Nband
                           to 0. The usual band codes are:
                           E		Extremely Short Period
                           S		Short Period
                           H		High Broad Band
                           B		Broad Band
                           M		Mid Period
                           L		Long Period
                           V		Very Long Period
                           U		Ultra Long Period
                           R		Extremely Long Period
                           
          Code ----------- Strings in the Code array should also be one-character
                           long. These are compared to the second character of the
                           chan string from the current. As with Band, a wildcard may
                           be used, although it is simpler to set Code and NCode to 0.
                           The usual codes are:
                           H		High Gain
                           L		Low Gain
                           G		Gravimeter/Accelerometer
                           M		Mass Position Seismometer
                           
          Orientation ---- Orientation strings should be one-character long, and are
                           compared to the 3rd letter of chan. The usual codes are:
                           Z N E	(Vertical North East)
                           A B C	(Triaxial along edges of cube standing on corner)
                           1 2 3	Orthogonal but non-standard orientation
		    
	  Filelist ------- If Filelist is non-NULL then its elements (which may contain
	                   wildcards) are compared to the dfile member of each wfdisc line
			   as it is read. If the dfile does not match one of the list
			   elements, the line is not used.

          Phaselist ------ If Phaselist is non-NULL then its elements are compared to the 
	                   iphase field of any arrival record being read. Only if there is an
			   exact match will a new arrival struct be created and populated
			   from the record.
*/                           

int cssRead2_8FlatFiles(char* Root, char *WorkSetName, int Replace, 
                        char** Station, int Nstation, char** Channel,
                        int Nchannel, char** Band, int Nband, char** Code,
                        int Ncode, char** Orientation, int Norientation,
                        char** Author, int Nauthor, char** FileList,
                        int Nfiles, char** Phaselist, int Nphases,
                        int MaxAllowableWaveforms, int verbose,
                        double MaxPhysMem )
{
   DBlist tree;
   DBlist tree2;
   int LinesUsed;
   Verbose = verbose;    /* Set static variable to argument. */
   MaxPhysMemToUse = MaxPhysMem;

   if(!Root || ! strlen(Root)){
      printf("Invalid root name!\n");
      return 0;
   }


   if(!WorkSetName || !strlen(WorkSetName) ){
      printf("Invalid worksetname! Cannot add CSS ASCII data to workset.\n");
      return 0;
   }

   if(Replace){
      smDeleteWorksetByName( (char*)WorkSetName );
      smCreateEmptyWorkset( (char*)WorkSetName );  /* This workset now default*/
   }
   else
      if(!smChangeDefaultWorksetByName( (char*)WorkSetName ))
         smCreateEmptyWorkset((char*) WorkSetName );  /* workset now default. */

   tree2 = smGetDefaultTree();
   if(!tree2) tree2 = smMakeDefaultTree(); /* This is the returned tree. */
   TracesInExistingTree = dblGetNumWaveformsInMemory(tree2);

   tree = dblNewTree(); /* This is a temp tree to be merged after filling. */


   LinesUsed = ReadWfdiscFile(Root, WorkSetName, tree, Station, Nstation, Channel, Nchannel,
                              Band, Nband, Code, Ncode, Orientation,
                              Norientation, Replace, FileList, Nfiles,
                              MaxAllowableWaveforms);
   if( !LinesUsed )
      return 0 ;

   /* If there is exactly one origin record, take it, and make event and 
      wftag to go with it.  Otherwize, skip origin. */
   ReadOriginFile(Root, tree); 


   ReadArrivalFile(Root, tree, Author, Nauthor, Phaselist, Nphases);


   /*

   ReadStructureFile(Root, tree, ".assoc", "Assoc", AddMatchingAssocStruct);


   ReadStructureFile(Root, tree, ".site", "Site", AddMatchingSiteStruct);

   ReadStructureFile(Root, tree, ".origerr", "Origerr", AddMatchingOrigerrStruct);

   ReadStructureFile(Root, tree, ".event", "Event", AddMatchingEventStruct);

   ReadStructureFile(Root, tree, ".sensor", "Sensor", AddMatchingSensorStruct);

   ReadStructureFile(Root, tree, ".instrument", "Instrument", AddMatchingInstrumentStruct);

   ReadStructureFile(Root, tree, ".gregion", "Gregion", AddMatchingGregionStruct);

   ReadStructureFile(Root, tree, ".remark", "Remark", AddMatchingRemarkStruct);
   */


   tree2 = dblMergeTrees(tree2, tree , FALSE );
   if(tree2 != tree) {
       dblDeleteTree(tree);
   }

   if(LinesUsed && Verbose){
      printf("%d traces read into CSStree\n", LinesUsed);
      dblTableOfContents(tree2, stdout);
   }


   return LinesUsed;
}
/* ------------------------------------------------------------------------ */   
