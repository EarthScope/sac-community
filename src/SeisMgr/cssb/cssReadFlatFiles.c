#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#ifdef POSIX
#include <fnmatch.h>
#endif
#include <math.h>
#include "cssb.h"
#include "timefuncs.h"
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

#include "debug.h"

int CSSfltDefined(float value);
int CSSdblDefined(double value);
int CSSlngDefined(int value);
int CSSchrDefined(char *value);
static void AddMatchingEventStruct(char *buffer, DBlist tree);
static void ReadStructureFile(char* Root, DBlist tree, char *Suffix,
                              char *StructType, void (*fp)(char*,void*));

void css_warning(char *fmt, ...);
void css_error(char *fmt, ...);
static int Verbose;
static int TracesInExistingTree;
static double MaxPhysMemToUse;


int FileExists(char *dir, char *dfile)
{
   int stringLen;
   char *filename;
   FILE *fptr;
   
   stringLen = strlen(dir) + strlen(dfile) + 3;
   filename  = (char *) smMalloc( stringLen * sizeof(char) );
   strcpy(filename,dir);
   stringLen = strlen(filename);
   if(filename[stringLen - 1] != '/')
      strcat(filename,"/");
      
   strcat(filename,dfile);
   fptr = fopen(filename,"rb") ;
   smFree(filename);
   
   if(!fptr) return 0;
   
   fclose(fptr);
   return 1;
}
/* ----------------------------------------------------------------------- */ 




char *cssConstructFilename(char *Root, char *Suffix)
{
   char *FileName = (char*) smMalloc(strlen(Root) + strlen(Suffix) + 2 );
   strcpy(FileName, Root);
   strcat(FileName, Suffix);
   return FileName;
}
/* ----------------------------------------------------------------------- */




static char *StripSuffix( char *wfdiscName )
{
   char *ptr = strrchr(wfdiscName, '.');
   ptr[0]       = '\0';
   return wfdiscName;
}
/* ----------------------------------------------------------------------- */



int cssRead2_8FlatFiles(char* Root, char *WorkSetName, int Replace, 
                        char** Station, int Nstation, 
                        char** Channel, int Nchannel, 
                        char** Band, int Nband, 
                        char** Code, int Ncode,
                        char** Orientation, int Norientation, 
                        char** Author, int Nauthor,
                        char** Filelist, int Nfiles,
		        char** Phaselist, int Nphases, int MaxWaveforms, int Verbose, 
		        double MaxPhysMemToUse);


   /*
   This function parses a line containing multiple tokens into a character string array (char**)
   where each string contains one token from the input line. It returns the number of tokens
   in the Ntokens argument. The caller is responsible for freeing the memory of the character
   string when it is no longer needed. The third argument (Delimit) is a string containing the
   characters that may act as token delimiters. If D is set to 0, then the function will use
   the space, tab, and newline characters as delimiters.
   */

char **TokenLineToStrgList(char* line, int* Ntokens, char* Delimit)
{
   char *tok;
   char **list;
   char D[10];
   char *ptr;

   *Ntokens = 0;
   if(!line || !strlen(line))return 0;
   
   ptr = (char*) smMalloc(strlen(line) + 1);
   strcpy(ptr, line);

   if(!Delimit || !strlen(Delimit) )
      strcpy(D, " \t\n");
   else
      strncpy(D, Delimit, 9);

   tok = strtok(ptr, D);
   while(tok){
      (*Ntokens)++;
      tok = strtok(0, D);
   }
   if(! *Ntokens){
      smFree(ptr);
      return 0;
   }

   strcpy(ptr, line);
   list = (char**) smMalloc(*Ntokens * sizeof(char*) );

   *Ntokens = 0;
   tok = strtok(ptr, D);
   while(tok){
      list[*Ntokens] = (char*) smMalloc( strlen(tok) + 1);
      strcpy( list[*Ntokens], tok);
      (*Ntokens)++;
      tok = strtok(0, D);
   }
   smFree(ptr);
   return list;
}
/* ----------------------------------------------------------------------- */ 




/*
This function frees a string array (char**). The input arguments are S (the array to be
freed), and Nlines (the number of lines in the array). Nlines must be consistent with
the actual dimensions of S to avoid possible leaks or freeing of unallocated memory.
*/

void FreeStringArray(char** S, int Nlines)
{
   int j;
   if(!S || !Nlines) return;

   for(j=0; j<Nlines; j++)
      smFree(S[j]);

   smFree(S);
}
/* ----------------------------------------------------------------------- */ 







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
   char *path;
   char* p = strrchr(s, '/');
   if(!p){
      path = (char*) smMalloc(2);
      strcpy(path, ".");
      return path;
   }

   *p = '\0';
   path = (char*) smMalloc(strlen(s) + 1);
   strcpy(path, s);
   return path;
}
/* ------------------------------------------------------------------------- */


   





static int  GetWfdiscVersion(char *FileName)
{
   FILE *ptr;
   int testlong;
   double testdouble;

   char buffer[300];   /* Holds lines read from CSS files */
   int  buflen = 300;
   int Min30LineLength = 267;
   int Min28LineLength = 179;



   if(! (ptr = fopen( FileName, "rb" ) ) ){
     css_error("Unable to open file (%s) for version check.\n",FileName);
     return 0;
   }
   
   if(!fgetsp(buffer, buflen, ptr) ){
      fclose(ptr);
      return 0;
   }
   fclose(ptr);



   if((int)strlen(buffer) >= Min30LineLength){
    /*testdouble = atof(buffer + 16); * This should be start time. *
      if(testdouble < 0) return 0;    * Before 1970 */
      testlong   = atol(buffer + 52); /* Should be jdate. */
      if(testlong < 0 ) return 0;     /* before year 0 */
      testlong   = atol(buffer + 79); /* Should be nsamp.*/
      if(testlong < 0)  return 0;     /* Can't have negative nsamp. */
      testdouble = atof(buffer + 88); /* Should be samprate. */
      if(testdouble < 0) return 0;    /* Can't have negative samprate. */
      return 30;                      /* Almost certain to be version 3.0. */
   }
   else if((int)strlen(buffer) >= Min28LineLength){
      testlong   = atol(buffer + 0); /* Should be date. */
      if(testlong < 0 ) return 0;     /* before year 0 */
      testdouble = atof(buffer + 9); /* This should be start time. */
      if(testdouble < 0) return 0;    /* Before 1970 */
      testlong   = atol(buffer + 35); /* Should be nsamp.*/
      if(testlong < 0)  return 0;     /* Can't have negative nsamp. */
      testdouble = atof(buffer + 44); /* Should be smprat. */
      if(testdouble < 0) return 0;    /* Can't have negative samprate. */
      return 28;
   }

   
   return 0;
}
/* ---------------------------------------------------------------------------------- */              








int InList(char* str, char** List, int Nitems)
{
   #ifdef POSIX
   int j;
   for(j=0;j<Nitems;j++)
      if( !fnmatch(List[j], str, 0) ) return 1;
   #else 
   UNUSED(str);
   UNUSED(List);
   UNUSED(Nitems);
   #endif
   return 0;
}
/* ------------------------------------------------------------------------- */   



int InOneCharList(char* chan, char** List, int Nitems, int pos)
{
   int j;
   char c;

   /* Channel names should be of the form BIO where B is the band code,
      I is the instrument code, and O is the orientation code. Thus is pos is
      0 we are checking Band, if pos = 1 we are checking Instrument, and if pos
      = 2 we are checking Orientation. However, there are some 2-character codes like
      ie, in, iz so if pos == 2 and strlen(chan) == 2 assume we are doing an orientation
      comparison.
   */
   
   if(pos > (int)strlen(chan) - 1){
      pos = strlen(chan) - 1;
      c = toupper(chan[pos]);
   }
   else
      c = toupper(chan[pos]);
      
   for(j=0;j<Nitems;j++){
      if(List[j][0] == '*' || List[j][0] == '?' || c == toupper(List[j][0]) ) return 1;
   }
   return 0;
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



                       


static int AddToWfdisc(char* buffer, DBlist tree, char** Station, int Nstation, 
                       char** Channel, int Nchannel, char** Band, int Nband, 
                       char** Code, int Ncode, char** Orientation, 
                       int Norientation, char** FileList, int Nfiles, char* WfdiscPath)
{
   struct wfdisc w;
   struct wfdiscList *wf;
   char AlternateDir[256];
   int wOff[] = {0, 7, 16, 34, 43, 52, 61, 79, 88, 100, 117, 134, 141, 143, 146, 148, 
                 213, 246, 257, 266, 284};
   
   strncpy(w.sta,   buffer + wOff[0], 6 ); Deblank(w.sta, 7);
   strncpy(w.chan , buffer + wOff[1], 8 ); Deblank(w.chan, 9);
   if(! InSpecList(w.sta, w.chan, Station, Nstation, Channel, Nchannel, 
                    Band, Nband, Code, Ncode, Orientation, Norientation) ) return 0;
   
   w.time     = atof(buffer + wOff[2]);
   w.wfid     = atol(buffer + wOff[3]);
   w.chanid   = atol(buffer + wOff[4]);
   w.jdate    = atol(buffer + wOff[5]);
   w.endtime  = atof(buffer + wOff[6]);
   w.nsamp    = atol(buffer + wOff[7]);
   w.samprate = atof(buffer + wOff[8]);
   w.calib    = atof(buffer + wOff[9]);
   w.calper   = atof(buffer + wOff[10]);

                            
   strncpy(w.instype , buffer + wOff[11], 6 ); Deblank(w.instype, 7);
   strncpy(w.segtype , buffer + wOff[12], 1 ); Deblank(w.segtype, 2);
   strncpy(w.dattype , buffer + wOff[13], 2 ); Deblank(w.dattype, 3);
   strncpy(w.clip    , buffer + wOff[14], 1 ); Deblank(w.clip, 2);
   strncpy(w.dfile   , buffer + wOff[16], 32); Deblank(w.dfile, 33);
   strncpy(w.dir     , buffer + wOff[15], 64); Deblank(w.dir, 65);

   if(!FileExists(w.dir, w.dfile) ){
      strcpy(AlternateDir,WfdiscPath);
      strcat(AlternateDir, "/");
      strcat(AlternateDir, w.dir);
      if(!FileExists(AlternateDir, w.dfile) ){ 
        strcpy(w.dir, WfdiscPath); /* Use data file path for wfdisc file. */
        if(!FileExists(w.dir, w.dfile) ){
          css_warning("Cannot open waveform file(%s). Skipping trace... [Flat file]\n",w.dfile);
          return 0;
        }
      }
      else
        strcpy(w.dir, AlternateDir);
   }
   

   w.foff     = atol(buffer + wOff[17]);
   w.commid   = atol(buffer + wOff[18]);

   strncpy(w.lddate  , buffer + wOff[19], 17); Deblank(w.lddate, 18);


   if( FileList && Nfiles)
      if(! InList(w.dfile, FileList, Nfiles) )
	 return 0;


   if( !CSSlngDefined(w.wfid) )
      w.wfid = dblNextAvailableWfid(tree);

   if(Verbose){
     css_info("Adding station (%s) channel (%s)...\n",w.sta, w.chan);
   }
   wf = (struct wfdiscList *) dblCreateTableInstance(tree, dbl_LIST_WFDISC);
   if(!wf) {
     css_error("Cannot create wfdiscList Instance\n");
     return 0;
   }
   dblCopyTableElement(dbl_LIST_WFDISC,&w,wf);
   if(!dblGetSeismograms( wf , 0 , 0 ) ){
     css_error("Problem reading seismogram. Freeing wfdisc struct...\n");
     dblDeleteTableInstance(dbl_LIST_WFDISC, tree, wf);
   }
   return 1;
}
/* ---------------------------------------------------------------------------------- */              






static int ReadWfdiscFile(char* Root,  char* WorkSetName, DBlist tree, char** Station, 
                          int Nstation, char** Channel, int Nchannel, char** Band, 
                          int Nband,char** Code, int Ncode, char** Orientation, 
                          int Norientation, int Replace, char** FileList, int Nfiles,
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

   int WaveformsInMemory = dblGetNumWaveformsInMemory(tree) + TracesInExistingTree;


   /* Open the wfdisc file */
   FileName = cssConstructFilename(Root, WfSuffix);
   if(! (ptr = fopen( FileName, "rb" ) ) ){
     css_error("Unable to open file (%s).\n",FileName);
     if(Replace){ 
       smDeleteWorksetByName( (char*)WorkSetName );
     }
     smFree(FileName);
     return 0;
   }
   
   WfdiscPath = ExtractPath(FileName); /* may need this for reading seismograms. */
   /* Read the wfdisc file... */
   while( fgetsp(buffer, buflen, ptr) ){
      if( !strlen(buffer) || *buffer == '\n' )continue;
      Lines      = AddToWfdisc(buffer, tree, Station, Nstation, Channel, Nchannel, 
                               Band, Nband, Code, Ncode, Orientation, Norientation,
			       FileList, Nfiles, WfdiscPath);
      LinesUsed         += Lines;
      WaveformsInMemory += Lines;
      if(WaveformsInMemory == MaxAllowableWaveforms){
         css_warning("There are now %d waveforms in memory. Remainder will be skipped.\n",
                     WaveformsInMemory);
         break;
      }
      
      if(smFracPhysMemUsed() > MaxPhysMemToUse){
        css_warning("Waveforms in SeisMgr memory are using more than %5.2f%% of physical memory.\n",
                    MaxPhysMemToUse * 100);
        css_warning("No more waveforms will be read.\n");
        css_warning("To utilize a higher percentage of physical memory use the MAXMEM option.\n");                
        break;
      }
       
   }
   smFree(WfdiscPath);
   fclose(ptr);
   smFree(FileName);
   return LinesUsed;
}
/* ---------------------------------------------------------------------------------- */              




static int WfidInWfdiscList(int wfid, DBlist tree)
{
   struct wfdiscList *w = 0;
   do{
      if(!( w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC) ) )break;
      if(w->element->wfid == wfid)return 1;
   }while(w);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              





static void AddMatchingWftagStruct(char *buffer, DBlist tree)
{
   struct wftagList *wt;
   int wfid = atol(buffer + 18);

   if(!WfidInWfdiscList(wfid, tree) ) return;

   wt = (struct wftagList *) dblCreateTableInstance(tree, dbl_LIST_WFTAG);
   if(!wt)return;
   strncpy(wt->element->tagname, buffer, 8); Deblank(wt->element->tagname, 9);
   wt->element->tagid = atol(buffer + 9);
   wt->element->wfid = wfid;
   strncpy(wt->element->lddate, buffer + 27, 17); Deblank(wt->element->lddate, 18);
}
/* ---------------------------------------------------------------------------------- */              




static struct originList *findMatchingOrigin(DBlist tree, int evid , int orid )
{
   struct originList *orig = 0 ;
   
   /* Try to match by orid and evid. */
   do{
      orig = (struct originList *) dblNextTableInstance( (void *) orig , tree ,
                                                          dbl_LIST_ORIGIN ) ;
      if( !orig ) break ;

      if(CSSlngDefined(evid) && evid == orig->element->evid){
         if(CSSlngDefined(orid) && orid == orig->element->orid){
	    return orig;
	 }
      }
   }while(orig);
   
   /* Nothing worked. */
   return 0;
}
/* ------------------------------------------------------------------ */







static void AddOriginLine(char* buffer, int* orOffset, DBlist tree)
{
   int orid = atol(buffer + orOffset[4]) ;
   int evid = atol(buffer + orOffset[5]) ;

   struct originList *o = findMatchingOrigin( tree, evid, orid ) ;

   if(o)
      return; /* Don't duplicate existing origin. */


   o = (struct originList *) dblCreateTableInstance(tree, dbl_LIST_ORIGIN);
   if(!o)return;



   o->element->lat    = atof(buffer + orOffset[0]);
   o->element->lon    = atof(buffer + orOffset[1]);
   o->element->depth  = atof(buffer + orOffset[2]);
   o->element->time   = atof(buffer + orOffset[3]);


   o->element->orid   = orid ;
   o->element->evid   = evid ;
   o->element->jdate  = atol(buffer + orOffset[6]);
   o->element->nass   = atol(buffer + orOffset[7]);
   o->element->ndef   = atol(buffer + orOffset[8]);
   o->element->ndp    = atol(buffer + orOffset[9]);
   o->element->grn    = atol(buffer + orOffset[10]);
   o->element->srn    = atol(buffer + orOffset[11]);
   strncpy(o->element->etype, buffer + orOffset[12], 7);Deblank(o->element->etype, 8); 
   o->element->depdp  = atof(buffer + orOffset[13]);
   strncpy(o->element->dtype, buffer + orOffset[14], 1);Deblank(o->element->dtype, 2); 

   o->element->mb     = atof(buffer + orOffset[15]);
   o->element->mbid   = atol(buffer + orOffset[16]);
   o->element->ms     = atof(buffer + orOffset[17]);
   o->element->msid   = atol(buffer + orOffset[18]);
   o->element->ml     = atof(buffer + orOffset[19]);
   o->element->mlid   = atol(buffer + orOffset[20]);
   strncpy(o->element->algorithm, buffer + orOffset[21], 15);Deblank(o->element->algorithm, 16); 
   strncpy(o->element->auth, buffer + orOffset[22], 15);Deblank(o->element->auth, 16); 
   o->element->commid = atol(buffer + orOffset[23]);
   strncpy(o->element->lddate, buffer + orOffset[24], 17);Deblank(o->element->lddate, 18); 


}
/* ---------------------------------------------------------------------------------- */              




static int WfTagMatchEvid(int evid, DBlist tree)
{
   char Tmp[10];
   struct wftagList *wt = 0;
   do{
      if(!( wt = (struct wftagList *) dblNextTableInstance(wt, tree, dbl_LIST_WFTAG) ) )break;
      strcpy(Tmp, wt->element->tagname);
      Upstring(Tmp);
      if( !strcmp(Tmp, "EVID") )
         if(wt->element->tagid == evid) return 1;
   }while(wt);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              




static struct wftagList* WfTagMatchOrid(int orid, DBlist tree)
{
   char Tmp[10];
   struct wftagList *wt = 0;
   do{
      if(!( wt = (struct wftagList *) dblNextTableInstance(wt, tree, dbl_LIST_WFTAG) ) )break;
      strcpy(Tmp, wt->element->tagname);
      Upstring(Tmp);
      if( !strcmp(Tmp, "ORID") )
         if(wt->element->tagid == orid) return wt;
   }while(wt);
   return 0;
}
/* ---------------------------------------------------------------------------------- */




static struct wftagList* WfTagMatchWfid(int wfid, DBlist tree)
{
   struct wftagList *wt = 0;
   do{
      if(!( wt = (struct wftagList *) dblNextTableInstance(wt, tree, dbl_LIST_WFTAG) ) )break;
      if(wt->element->wfid == wfid) return wt;
   }while(wt);
   return 0;
}
/* ---------------------------------------------------------------------------------- */





static void AddWftagEntries(DBlist tree, int evid )
{
   struct wfdiscList *w = 0;
   struct wftagList *wt;
   do{
      if(!( w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC) ) ) break;

      wt = WfTagMatchWfid( w->element->wfid , tree ) ;
      if( wt ) {
         strcpy( wt->element->tagname , "evid" ) ;
         wt->element->tagid = evid ;
         continue ;
      }

      /* Make a new wftag struct and associate new origin with the wfdisc. */
      wt = (struct wftagList *) dblCreateTableInstance(tree, dbl_LIST_WFTAG);
      strcpy(wt->element->tagname, "evid");
      wt->element->tagid = evid;
      wt->element->wfid  = w->element->wfid;
      strcpy(wt->element->lddate, w->element->lddate);
   }while(w);
}
/* ---------------------------------------------------------------------------------- */              




static void BuildEventList( DBlist tree, int evid, int orid, int set )
{
   struct eventList  *ev = 0 ;

   do{
      ev = (struct eventList *) dblNextTableInstance(ev, tree, dbl_LIST_EVENT);
      if( ev && ev->element->evid == evid ) {
         if( set ) {
            ev->element->prefor = orid ;
            return ;
         }
         else {
            if( ev->element->prefor == orid ) 
               return ;
            else
               continue ;
         }
      }
   }while( ev ) ;

   ev = (struct eventList *) dblCreateTableInstance(tree, dbl_LIST_EVENT);
   ev->element->evid = evid;
   ev->element->prefor = orid ;
   /* tmListEpochTime's second argument (19) signifies that the string will
      be of the form dd-Mon-yyyy. */
   strcpy(ev->element->lddate, tmListEpochTime( tmGetEpochTime() , 19 ) );
}
/* ---------------------------------------------------------------------------------- */




/* static void BuildWftagList(DBlist tree, int evid) */
/* { */
/*    struct originList *orig = 0; */
/*    struct wfdiscList *w = 0; */
/*    struct wftagList  *wt; */

/*    orig = (struct originList *) dblNextTableInstance(orig, tree, dbl_LIST_ORIGIN); */
/*    if(!orig)return; */

/*    do{ */
/*       if(!( w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC) ) )break; */
/*       /\* Make a new wftag struct and associate new origin with the wfdisc. *\/ */
/*       wt = (struct wftagList *) dblCreateTableInstance(tree, dbl_LIST_WFTAG); */
/*       strcpy(wt->element->tagname, "evid"); */
/*       wt->element->tagid = evid; */
/*       wt->element->wfid  = w->element->wfid; */
/*       strcpy(wt->element->lddate, orig->element->lddate); */
/*    }while(w); */
/* } */
/* ---------------------------------------------------------------------------------- */              




static int GetPreforFromEvent( int evid , DBlist tree ) 
{
    struct eventList       *ev = 0;

    do{
      ev = (struct eventList*)dblNextTableInstance(ev, tree, dbl_LIST_EVENT);
      if( ev && ev->element->evid == evid )
         return ev->element->prefor ;
   }while(ev);

   return 0 ;
}
/* --------------------------------------------------------------------------------- */



int oneEvidInArray( int evid , int *evidArray , int cnt )
{
   int idx , nevids = 0 ;

   for( idx = 0 ; idx < cnt ; idx++ ) {
      if( evidArray[ idx ] == evid ) {
         ++nevids ;
      }
   }

   if( nevids > 1 )
      nevids = 0 ;

   return nevids ;
}
/* --------------------------------------------------------------------------------- */


/* don't assume one orid per evid */
static int AddMatchingOriginStruct(char* buffer, DBlist tree, int *orOffset)
{
   int evid, orid ;
   struct wftagList *wt = 0 ;

   orid = atoi(buffer + orOffset[4]);
   evid = atoi(buffer + orOffset[5]);

   if( WfTagMatchEvid(evid, tree) ){
      if( orid == GetPreforFromEvent( evid , tree ) ) {
         AddOriginLine(buffer, orOffset, tree);
         return 1;
      }
   }

   /* Updated to Convert wftag orid to wftag evid PG 2-26-01 */
   else {
       while((wt = WfTagMatchOrid(orid, tree)) ){
       strcpy( wt->element->tagname , "evid" ) ;
       wt->element->tagid = evid ;
       AddMatchingEventStruct( buffer + orOffset[5], tree ) ;
     }
     AddOriginLine( buffer, orOffset, tree );
     return 1;
   }



   return 0;
}
/* --------------------------------------------------------------------------------- */





static struct eventList* EventExists(int evid, DBlist tree) ;
         
#define ALLOCINC 10 

static void ReadOriginFile(char* Root, DBlist tree)
{
   FILE *ptr;
   char *FileName;
   char OrSuffix[] = ".origin";
   int  NumAdded   = 0 , cnt = 0 ;
   char buffer[300];   
   int  buflen     = 300;
   int *evidArray = NULL ;
   int orOffset[] = {0,10, 20, 30, 48, 57, 66, 75, 80, 85, 90, 99, 108, 116, 126, 128, 136, 
                     145, 153, 162, 170, 179, 195, 211, 220};
 
   FileName = cssConstructFilename(Root, OrSuffix);
   if(! (ptr = fopen( FileName, "rb" ) ) ){
      smFree(FileName);
      return;
   }

   evidArray = (int *) malloc( ALLOCINC * sizeof( int ) ) ;

   while( fgetsp(buffer, buflen, ptr) ){
      if( !strlen(buffer) || *buffer == '\n' )continue;
      cnt++ ;
      if( cnt % ALLOCINC == 0 )
         evidArray = (int *) realloc( evidArray , (cnt + ALLOCINC) * sizeof( int ) ) ;
      evidArray[ cnt - 1 ] = atoi( buffer + orOffset[ 5 ] ) ;
      NumAdded += AddMatchingOriginStruct(buffer, tree, orOffset);
   }

   if( !NumAdded ){
      int evid = -1 ;

      if( cnt == 1 ){        /* one and only one evid in evidArray */
         evid = evidArray[ 0 ] ;
         rewind(ptr);

         while( fgetsp(buffer, buflen, ptr) ){
            int orid ;

            if( !strlen(buffer) || *buffer == '\n' )continue;
            orid = atoi( buffer + orOffset[ 4 ] ) ;

            if ( Verbose ) {
              css_warning( "No origins match any prefor in the Event table.\n" ) ;
              css_warning( "One origin matched one and only one evid in Wftag table.\n" );
              css_warning( "That origin will be loaded.\n" ) ;
            }

            /* Handle event */
            if( !EventExists( evid , tree ) ) {
               ReadStructureFile( Root, tree, ".event", "Event", AddMatchingEventStruct ) ;
               BuildEventList( tree, evid , orid, 1 );
            }

            AddOriginLine(buffer, orOffset, tree);
            AddWftagEntries( tree , evid ) ;
            cnt = 2 ;
         }

      }


      if( cnt == 1 ) {
         if ( Verbose ) {
           css_warning("No origins were matched to wfdisc rows using either evids orig wftags.\n");
           css_warning("Attempting to add first line in origin file, but cannot guarantee ");
           css_warning("consistency.\n");
         }
         rewind(ptr);
         fgetsp(buffer, buflen, ptr);
         if(*buffer){
            int evid, orid ;
            struct originList *orig = 0 ;
            AddOriginLine(buffer, orOffset, tree);

            orid = atol( buffer + orOffset[ 4 ] ) ;
            evid = atol( buffer + orOffset[ 5 ] ) ;

            if( orid == 0 )
               orid = 1 ;
            if( evid == 0 )
               evid = 1 ;
            orig = dblNextTableInstance( orig , tree , dbl_LIST_ORIGIN ) ;
            if( orig ) {
               orig->element->evid = evid ;
               orig->element->orid = orid ;
               AddWftagEntries( tree , evid ) ;
               BuildEventList( tree, evid , orid , 0 );
            }
            else {
              css_error( "Manditory origin not found." ) ;
            }
         } /* end if(buffer) */
      } /* end if( cnt == 1 ) */
   }
            
   free( evidArray ) ;
   fclose(ptr);
   smFree(FileName);

}
/* ---------------------------------------------------------------------------------- */              




static int OridInOriginList(int orid, DBlist tree)
{
   struct originList *o = 0;
   do{
      if(!( o = (struct originList *) dblNextTableInstance(o, tree, dbl_LIST_ORIGIN) ) )break;
      if(o->element->orid == orid) return 1;
   }while(o);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              



static int AridInArrivalList(int arid, DBlist tree)
{
   struct arrivalList *ar = 0;
   do{
      if(!( ar = (struct arrivalList *)dblNextTableInstance(ar, tree, dbl_LIST_ARRIVAL) ) )break;
      if(ar->element->arid == arid) return 1;
   }while(ar);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              




static void AddMatchingAssocStruct(char *buffer, DBlist tree)
{
   int asOff[] = {0, 9, 18, 25, 34, 39, 48, 56, 64, 73, 75, 83, 85, 93, 95, 103, 110, 126, 135};
   int arid    = atol(buffer);
   int orid    = atol(buffer + 9);
   struct assocList *as;

   if(!OridInOriginList(orid, tree) ) return;
   if(!AridInArrivalList(arid, tree) ) return;

   as = (struct assocList *) dblCreateTableInstance(tree, dbl_LIST_ASSOC);
   as->element->arid    = arid;
   as->element->orid    = orid;
   strncpy(as->element->sta, buffer    + asOff[2], 6);     Deblank(as->element->sta, 7); 
   strncpy(as->element->phase, buffer  + asOff[3], 8);   Deblank(as->element->phase, 9);
   as->element->belief  = atof(buffer  + asOff[4]);
   as->element->delta   = atof(buffer  + asOff[5]);
   as->element->seaz    = atof(buffer  + asOff[6]);
   as->element->esaz    = atof(buffer  + asOff[7]);
   as->element->timeres = atof(buffer  + asOff[8]);
   strncpy(as->element->timedef, buffer + asOff[9], 1); Deblank(as->element->timedef, 2);
   as->element->azres   = atof(buffer  + asOff[10]);
   strncpy(as->element->azdef, buffer  + asOff[11], 1);  Deblank(as->element->azdef, 2);
   as->element->slores  = atof(buffer  + asOff[12]);
   strncpy(as->element->slodef, buffer + asOff[13], 1); Deblank(as->element->slodef, 2);
   as->element->emares  = atof(buffer  + asOff[14]);
   as->element->wgt     = atof(buffer  + asOff[15]);
   strncpy(as->element->vmodel, buffer + asOff[16], 15);Deblank(as->element->vmodel, 16);
   as->element->commid  = atol(buffer  + asOff[17]);
   strncpy(as->element->lddate, buffer + asOff[18], 17);Deblank(as->element->lddate, 18);
}
/* ---------------------------------------------------------------------------------- */              




static int TimeStaChanMatch(double time, char *sta, char *chan, DBlist tree)
{
   struct wfdiscList *w = 0;

   if(!CSSchrDefined(sta) )  return 0;
   if(!CSSchrDefined(chan) ) return 0;


   do{
      if(!( w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC) ) )break;
      if(!strcmp(sta, w->element->sta) )
         if(!strcmp(chan, w->element->chan) )
            if(time >= w->element->time && time <= w->element->endtime) 
               return w->element->wfid;
   }while(w);
   return wfdisc_null.wfid ;
}
/* ---------------------------------------------------------------------------------- */              



static void AddArrivalWftag(DBlist tree, int arid, int wfid)
{
   struct wftagList *wt = (struct wftagList *) dblCreateTableInstance(tree, dbl_LIST_WFTAG);
   if(!wt)return;
   strcpy(wt->element->tagname, "arid");
   wt->element->tagid = arid;
   wt->element->wfid = wfid;
   strcpy(wt->element->lddate, tmListEpochTime( tmGetEpochTime(), 18 ) );
}
/* ---------------------------------------------------------------------------------- */              




static void AddArrivalLine(char* buffer, int* Off, DBlist tree, int arid)
{
   struct arrivalList *a = (struct arrivalList *) dblCreateTableInstance(tree, dbl_LIST_ARRIVAL);
   if(!a)return;

   strncpy(a->element->sta, buffer, 6);              Deblank(a->element->sta, 7);
   a->element->time    = atof(buffer + Off[1]);
   a->element->arid    = arid;
   a->element->jdate   = atol(buffer + Off[3]);
   a->element->stassid = atol(buffer + Off[4]);
   a->element->chanid  = atol(buffer + Off[5]);
   strncpy(a->element->chan, buffer + Off[6], 8);    Deblank(a->element->chan, 9);
   strncpy(a->element->iphase, buffer + Off[7], 8);  Deblank(a->element->iphase, 9);
   strncpy(a->element->stype, buffer + Off[8], 1);   Deblank(a->element->stype, 2);
   a->element->deltim  = atof(buffer + Off[9]);
   a->element->azimuth = atof(buffer + Off[10]);
   a->element->delaz   = atof(buffer + Off[11]);
   a->element->slow    = atof(buffer + Off[12]);
   a->element->delslo  = atof(buffer + Off[13]);
   a->element->ema     = atof(buffer + Off[14]);
   a->element->rect    = atof(buffer + Off[15]);
   a->element->amp     = atof(buffer + Off[16]);
   a->element->per     = atof(buffer + Off[17]);
   a->element->logat   = atof(buffer + Off[18]);
   strncpy(a->element->clip, buffer + Off[19], 1);   Deblank(a->element->clip, 2);
   strncpy(a->element->fm, buffer + Off[20], 2);     Deblank(a->element->fm, 3);
   a->element->snr     = atof(buffer + Off[21]);
   strncpy(a->element->qual, buffer + Off[22], 1);   Deblank(a->element->qual, 2);
   strncpy(a->element->auth, buffer + Off[23], 15);  Deblank(a->element->auth, 16);
   a->element->commid  = atol(buffer + Off[24]);
   strncpy(a->element->lddate, buffer + Off[25], 17);Deblank(a->element->lddate, 18);

}
/* ---------------------------------------------------------------------------------- */              







static int InAuthorList(char *auth, char** Author, int Nauthor)
{
   int j;
   if(!Nauthor || !Author ) return 1; /* No List means match anything! */

   for(j=0;j<Nauthor;j++)
      if(!strcmp(auth, Author[j]) ) return 1;

   return 0;
}
/* ---------------------------------------------------------------------------------- */              





static int InPhaseList(char *iphase, char** Phaselist, int Nphases)
{
   int j;
   if(!Phaselist || !Nphases ) return 1; /* No List means match anything! */

   for(j=0;j<Nphases;j++)
      if(!strcmp(iphase, Phaselist[j]) ) return 1;

   return 0;
}
/* ---------------------------------------------------------------------------------- */              



static int WfidInList(DBlist tree, int wfid)
{
   struct wfdiscList *w = 0;

   do{
      w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC);
      if(!w)break;
      if(w->element->wfid == wfid )return 1;
   }while(w);

   return 0;


}
/* ---------------------------------------------------------------------------------- */              


static int WftagJoinExists(DBlist tree, int arid)
{
   char Tmp[10];
   struct wftagList *wt = 0;

   do{
      wt = (struct wftagList *) dblNextTableInstance(wt, tree, dbl_LIST_WFTAG);
      if(!wt)break;
      strcpy(Tmp, wt->element->tagname);
      Upstring(Tmp);
      if(! strcmp(Tmp, "ARID") )
         if(wt->element->tagid == arid)
            if( WfidInList(tree, wt->element->wfid) )
               return 1;
   }while(wt);

   return 0;

}
/* ---------------------------------------------------------------------------------- */              






static void AddMatchingArrivalStruct(char *buffer, DBlist tree, char** Author, int Nauthor,
				     char** Phaselist, int Nphases)
{
  int arOff[] = {0, 7, 25, 34, 43, 52, 61, 70, 79, 81, 88, 96, 104, 112, 120, 128, 136, 
                 147, 155, 163, 165, 168, 179, 181, 197, 206};
   int arid = atol(buffer + 25);
   double time;
   char sta[7];
   char chan[9];
   char auth[16];
   char iphase[9];
   int wfid;

   time = atof(buffer + arOff[1]);
   strncpy(sta, buffer, 6);               Deblank(sta, 7);
   strncpy(chan, buffer + arOff[6], 8);   Deblank(chan, 9);
   strncpy(auth, buffer + arOff[23], 15); Deblank(auth, 16);
   strncpy(iphase, buffer + arOff[7], 8); Deblank(iphase, 9);
   if(!InAuthorList(auth, Author, Nauthor) ) return;
   if(!InPhaseList(iphase, Phaselist, Nphases) ) return;

   if(arid < 1) arid = dblNextAvailableArid(tree);

   if( WftagJoinExists(tree, arid) ){
      AddArrivalLine(buffer, arOff, tree, arid);
      return;
   }

   wfid = TimeStaChanMatch(time, sta, chan, tree);
   if( isValidInt ( dbl_LIST_WFDISC, dbl_WFDIS_WFID, wfid ) ){
      AddArrivalLine(buffer, arOff, tree, arid);
      AddArrivalWftag(tree, arid, wfid);
      return;
   }
}
/* ---------------------------------------------------------------------------------- */              


         
      

static void ReadArrivalFile(char* Root, DBlist tree, 
                            char** Author, int Nauthor, char** Phaselist, int Nphases)
{
   FILE *ptr;
   char *FileName;
   char ArSuffix[] = ".arrival";
   char buffer[300];   
   int  buflen     = 300;
 

   FileName = cssConstructFilename(Root, ArSuffix);
   if(! (ptr = fopen( FileName, "rb" ) ) ){
      smFree(FileName);
      return;
   }
   else{
      while( fgetsp(buffer, buflen, ptr) ){
         if( !strlen(buffer) || *buffer == '\n' )continue;
         AddMatchingArrivalStruct(buffer, tree, Author, Nauthor, Phaselist, Nphases);
      }
            
      fclose(ptr);
      smFree(FileName);
   }

}
/* ---------------------------------------------------------------------------------- */              
         

         



static int SiteChanExists(char *sta, char *chan, int ondate, int offdate, DBlist tree)
{
   struct sitechanList *s = 0;
   do{
      if(!( s = (struct sitechanList *) dblNextTableInstance(s, tree, dbl_LIST_SITECHAN) ) )break;
      if(!strcmp(sta, s->element->sta) && !strcmp(chan, s->element->chan) &&
         ondate == s->element->ondate && offdate == s->element->offdate)return 1;
   }while(s);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              
         


static int MatchesWfdiscEntry(char* sta, char* chan, int ondate, int offdate, DBlist tree)
{
   struct wfdiscList *w = 0;
   do{
      if(!( w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC) ) )break;
      if(!strcmp(sta, w->element->sta) && !strcmp(chan, w->element->chan) &&
         ondate <= w->element->jdate && (offdate < 0 || offdate >= w->element->jdate))
         return 1;
   }while(w);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              
         




static void AddMatchingSiteChanStruct(char *buffer, DBlist tree)
{
   int Off[] = {0, 7, 16, 25, 34, 43, 48, 58, 65, 72, 123};
   struct sitechanList *s;
   char sta[6];
   char chan[9];
   int ondate, offdate;

   ondate  = atol(buffer+Off[2]);
   offdate = atol(buffer + Off[4]);
   strncpy(sta, buffer, 6);           Deblank(sta, 6);
   strncpy(chan, buffer + Off[1], 8); Deblank(chan, 9);
   if(SiteChanExists(sta, chan, ondate, offdate, tree) ) return;
   if(!MatchesWfdiscEntry(sta, chan, ondate, offdate, tree) ) return;

   s = (struct sitechanList *) dblCreateTableInstance(tree, dbl_LIST_SITECHAN);
   strcpy(s->element->sta, sta);
   strcpy(s->element->chan, chan);
   s->element->ondate  = ondate;
   s->element->chanid  = atol(buffer + Off[3]);
   s->element->offdate = offdate;
   strncpy(s->element->ctype, buffer + Off[5], 4);    Deblank(s->element->ctype, 5);
   s->element->edepth  = atof(buffer + Off[6]);
   s->element->hang    = atof(buffer + Off[7]);
   s->element->vang    = atof(buffer + Off[8]);
   strncpy(s->element->descrip, buffer + Off[9], 50); Deblank(s->element->descrip, 51);
   strncpy(s->element->lddate, buffer + Off[10], 17); Deblank(s->element->lddate, 18);
}
/* ---------------------------------------------------------------------------------- */              






static int SiteExists(char *sta, int ondate, int offdate, DBlist tree)
{
   struct siteList *s = 0;
   do{
      if(!( s = (struct siteList *) dblNextTableInstance(s, tree, dbl_LIST_SITE) ) )break;
      if(!strcmp(sta, s->element->sta)&& ondate == s->element->ondate && 
         offdate == s->element->offdate)return 1;
   }while(s);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              



static int StaTimeMatchWfdiscEntry(char* sta, int ondate, int offdate, DBlist tree)
{
   struct wfdiscList *w = 0;

   do{
      if(!( w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC) ) )break;
      if(!strcmp(sta, w->element->sta) && ondate <= w->element->jdate && 
         (offdate < 0 || offdate >= w->element->jdate)) return 1;
   }while(w);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              





static void AddMatchingSiteStruct(char *buffer, DBlist tree)
{
   int Off[] = {0, 7, 16, 25, 35, 45, 55, 106, 111, 118, 128, 138};
   struct siteList *s;
   char sta[6];
   int ondate, offdate;

   ondate  = atol(buffer+Off[1]);
   offdate = atol(buffer + Off[2]);
   strncpy(sta, buffer, 6);          Deblank(sta, 6);
   if(SiteExists(sta, ondate, offdate, tree) ) return;
   if(!StaTimeMatchWfdiscEntry(sta, ondate, offdate, tree) ) return;

   s = (struct siteList *) dblCreateTableInstance(tree, dbl_LIST_SITE);
   strcpy(s->element->sta, sta);
   s->element->ondate  = ondate;
   s->element->offdate = offdate;
   s->element->lat     = atof(buffer   + Off[3]);
   s->element->lon     = atof(buffer   + Off[4]);
   s->element->elev    = atof(buffer   + Off[5]);
   strncpy(s->element->staname, buffer + Off[6], 50); Deblank(s->element->staname, 51);
   strncpy(s->element->statype, buffer + Off[7], 4); Deblank(s->element->statype, 5);
   strncpy(s->element->refsta, buffer  + Off[8], 6); Deblank(s->element->refsta, 7);
   s->element->dnorth  = atof(buffer   + Off[9]);
   s->element->deast   = atof(buffer   + Off[10]);
   strncpy(s->element->lddate, buffer  + Off[11], 17); Deblank(s->element->lddate, 18);
}
/* ---------------------------------------------------------------------------------- */              

      

         




static int AffiliationExists(char *sta, DBlist tree)
{
   struct affiliationList *a = 0;
   do{
      if(!( a = (struct affiliationList *) dblNextTableInstance(a, tree, dbl_LIST_AFFILIATION) ) )break;
      if(!strcmp(sta, a->element->sta))return 1;
   }while(a);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              




static int StaInSiteList(char* sta, DBlist tree)
{
   struct siteList *s = 0;
   do{
      if(!( s = (struct siteList *) dblNextTableInstance(s, tree, dbl_LIST_SITE) ) )break;
      if(!strcmp(sta, s->element->sta))return 1;
   }while(s);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              






static void AddMatchingAffiliationStruct(char *buffer, DBlist tree)
{
   int Off[] = {0, 9, 16};
   struct affiliationList *a;
   char sta[6];

   strncpy(sta, buffer + Off[1], 6); Deblank(sta, 7);
   if(AffiliationExists(sta, tree) ) return;
   if(!StaInSiteList(sta, tree) ) return;

   a = (struct affiliationList *) dblCreateTableInstance(tree, dbl_LIST_AFFILIATION);
   strncpy(a->element->net, buffer, 8); Deblank(a->element->net, 9);
   strcpy(a->element->sta, sta);
   strncpy(a->element->lddate, buffer + Off[2], 17); Deblank(a->element->lddate, 18);
}
/* ---------------------------------------------------------------------------------- */              






static int OrigerrExists(int orid, DBlist tree)
{
   struct origerrList *oe = 0;
   do{
      if(!( oe = (struct origerrList *) dblNextTableInstance(oe, tree, dbl_LIST_ORIGERR) ) )break;
      if(orid == oe->element->orid)return 1;
   }while(oe);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              





static void AddMatchingOrigerrStruct(char *buffer, DBlist tree)
{
   int Off[] = {0, 9, 25, 41, 57, 73, 89, 105, 121, 137, 153, 169, 179, 189, 199, 
                206, 216, 225, 231, 240};
   struct origerrList *oe;
   int orid = atol(buffer);

   if(OrigerrExists(orid, tree) ) return;
   if(!OridInOriginList(orid, tree) ) return;

   oe = (struct origerrList *) dblCreateTableInstance(tree, dbl_LIST_ORIGERR);
   oe->element->orid     = orid;
   oe->element->sxx      = atof(buffer + Off[1]);
   oe->element->syy      = atof(buffer + Off[2]);
   oe->element->szz      = atof(buffer + Off[3]);
   oe->element->stt      = atof(buffer + Off[4]);
   oe->element->sxy      = atof(buffer + Off[5]);
   oe->element->sxz      = atof(buffer + Off[6]);
   oe->element->syz      = atof(buffer + Off[7]);
   oe->element->stx      = atof(buffer + Off[8]);
   oe->element->sty      = atof(buffer + Off[9]);
   oe->element->stz      = atof(buffer + Off[10]);
   oe->element->sdobs    = atof(buffer + Off[11]);
   oe->element->smajax   = atof(buffer + Off[12]);
   oe->element->sminax   = atof(buffer + Off[13]);
   oe->element->strike   = atof(buffer + Off[14]);
   oe->element->sdepth   = atof(buffer + Off[15]);
   oe->element->stime    = atof(buffer + Off[16]);
   oe->element->conf     = atof(buffer + Off[17]);
   oe->element->commid   = atol(buffer + Off[18]);
   strncpy(oe->element->lddate, buffer + Off[19], 17); Deblank(oe->element->lddate, 18);
}
/* ---------------------------------------------------------------------------------- */              







static struct eventList* EventExists(int evid, DBlist tree)
{
   struct eventList *e = 0;
   do{
      if( !( e = (struct eventList *) dblNextTableInstance(e, tree, dbl_LIST_EVENT) ) )break;
      if( evid == e->element->evid ) return e ;
   }while(e) ;
   return 0 ;
}
/* ---------------------------------------------------------------------------------- */              




static int EvidInWftagList(int evid, DBlist tree)
{
   struct wftagList *wt = 0;
   char   tag[ 9 ] ;
   int    idx ;

   do{
      int len ;
      if(!( wt = (struct wftagList *) dblNextTableInstance(wt, tree, dbl_LIST_WFTAG) ) )break;
      len = strlen(wt->element->tagname) >= 8 ? 8 : strlen(wt->element->tagname) ;
      for( idx = 0 ; idx < len ; idx++ )
         tag[ idx ] = toupper( wt->element->tagname[ idx ] ) ;
      tag[ idx ] = '\0' ;
      if( !strcmp( tag , "EVID" ) && evid == wt->element->tagid ) return 1;
   }while(wt);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              







static void AddMatchingEventStruct(char *buffer, DBlist tree)
{
   int Off[] = {0, 9, 25, 34, 50, 59};
   struct eventList *e;
   int evid = atol(buffer);

   if( EventExists(evid, tree) ) return ;
   if(!EvidInWftagList(evid, tree) ) return ;

   e = (struct eventList *) dblCreateTableInstance(tree, dbl_LIST_EVENT);
   e->element->evid        = evid;
   strncpy(e->element->evname, buffer + Off[1], 15); Deblank(e->element->evname, 16);
   e->element->prefor      = atol(buffer + Off[2]);
   strncpy(e->element->auth, buffer + Off[3], 15); Deblank(e->element->auth, 16);
   e->element->commid      = atol(buffer + Off[4]);
   strncpy(e->element->lddate, buffer + Off[5], 17); Deblank(e->element->lddate, 18);

   return ;
}
/* ---------------------------------------------------------------------------------- */              



         


         



static int SensorExists(char *sta, char *chan, double time, double endtime, DBlist tree)
{
   struct sensorList *s = 0;
   do{
      if(!( s = (struct sensorList *)dblNextTableInstance(s, tree, dbl_LIST_SENSOR) ) )break;
      if(!strcmp(sta, s->element->sta) && !strcmp(chan, s->element->chan) &&
         time == s->element->time && endtime == s->element->endtime)return 1;
   }while(s);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              
         


static int SensorInWfdiscList(char* sta, char* chan, double time, double endtime, DBlist tree)
{
   struct wfdiscList *w = 0;
   do{
      if(!( w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC) ) )break;
      if(!strcmp(sta, w->element->sta) && !strcmp(chan, w->element->chan) &&
         time <= w->element->time &&  endtime >= w->element->endtime)return 1;
   }while(w);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              
         




static void AddMatchingSensorStruct(char *buffer, DBlist tree)
{
   int Off[] = {0, 7, 16, 34, 52, 61, 70, 79, 96, 113, 120, 122};
   struct sensorList *s;
   char sta[6];
   char chan[9];
   double time, endtime;

   time    = atof(buffer + Off[2]);
   endtime = atof(buffer + Off[3]);
   strncpy(sta, buffer, 6);           Deblank(sta, 7);
   strncpy(chan, buffer + Off[1], 8); Deblank(chan, 9);
   if(SensorExists(sta, chan, time, endtime, tree) ) return;
   if(!SensorInWfdiscList(sta, chan, time, endtime, tree) ) return;

   s = (struct sensorList *) dblCreateTableInstance(tree, dbl_LIST_SENSOR);
   strcpy(s->element->sta, sta);
   strcpy(s->element->chan, chan);
   s->element->time      = time;
   s->element->endtime   = endtime;
   s->element->inid      = atol(buffer + Off[4]);
   s->element->chanid    = atol(buffer + Off[5]);
   s->element->jdate     = atol(buffer + Off[6]);
   s->element->calratio  = atof(buffer + Off[7]);
   s->element->calper    = atof(buffer + Off[8]);
   s->element->tshift    = atof(buffer + Off[9]);
   strncpy(s->element->instant, buffer + Off[10], 1);  Deblank(s->element->instant, 2);
   strncpy(s->element->lddate,  buffer + Off[11], 17); Deblank(s->element->lddate, 18);
}
/* ---------------------------------------------------------------------------------- */              








static int InstrumentExists(int inid, DBlist tree)
{
   struct instrumentList *i = 0;
   do{
      if(!( i = (struct instrumentList *) dblNextTableInstance(i, tree, dbl_LIST_INSTRUMENT) ) )break;
      if(inid == i->element->inid)return 1;
   }while(i);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              




static int InidInSensorList(int inid, DBlist tree)
{
   struct sensorList *s = 0;
   do{
      if(!( s = (struct sensorList *)dblNextTableInstance(s, tree, dbl_LIST_SENSOR) ) )break;
      if(inid == s->element->inid)return 1;
   }while(s);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              






static void AddMatchingInstrumentStruct(char *buffer, DBlist tree)
{
   int Off[] = {0, 9, 60, 67, 69, 71, 83, 100, 117, 182, 215, 222};
   struct instrumentList *i;
   int inid = atol(buffer);

   if(InstrumentExists(inid, tree) ) return;
   if(!InidInSensorList(inid, tree) ) return;
   i = (struct instrumentList *) dblCreateTableInstance(tree, dbl_LIST_INSTRUMENT);
   i->element->inid        = inid;
   strncpy(i->element->insname, buffer + Off[1], 50); Deblank(i->element->insname, 51);
   strncpy(i->element->instype, buffer + Off[2], 6);  Deblank(i->element->instype, 7);
   strncpy(i->element->band,    buffer + Off[3], 1);  Deblank(i->element->band, 2);
   strncpy(i->element->digital, buffer + Off[4], 1);  Deblank(i->element->digital, 2);
   i->element->samprate      = atof(buffer + Off[5]);
   i->element->ncalib        = atof(buffer + Off[6]);
   i->element->ncalper       = atof(buffer + Off[7]);
   strncpy(i->element->dir,     buffer + Off[8], 64); Deblank(i->element->dir, 65);
   strncpy(i->element->dfile,   buffer + Off[9], 32); Deblank(i->element->dfile, 33);
   strncpy(i->element->rsptype, buffer + Off[10], 6); Deblank(i->element->rsptype, 7);
   strncpy(i->element->lddate,  buffer + Off[11], 17);Deblank(i->element->lddate, 18);
}
/* ---------------------------------------------------------------------------------- */              







static int GregionExists(int grn, DBlist tree)
{
   struct gregionList *g = 0;
   do{
      if(!( g = (struct gregionList *) dblNextTableInstance(g, tree, dbl_LIST_GREGION) ) )break;
      if(grn == g->element->grn)return 1;
   }while(g);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              




static int GregionInOriginList(int grn, DBlist tree)
{
   struct originList *orig = 0;
   do{
      if(!( orig = (struct originList *) dblNextTableInstance(orig, tree, dbl_LIST_ORIGIN) ) )break;
      if(grn == orig->element->grn)return 1;
   }while(orig);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              



static void AddMatchingGregionStruct(char *buffer, DBlist tree)
{
   int Off[] = {0, 9, 50};
   struct gregionList *g;
   int grn = atol(buffer);

   if(GregionExists(grn, tree) ) return;
   if(!GregionInOriginList(grn, tree) ) return;

   g = (struct gregionList *) dblCreateTableInstance(tree, dbl_LIST_GREGION);
   g->element->grn     = grn;
   strncpy(g->element->grname, buffer + Off[1], 40); Deblank(g->element->grname, 41);
   strncpy(g->element->lddate, buffer + Off[2], 17); Deblank(g->element->lddate, 18);
}
/* ---------------------------------------------------------------------------------- */              







static int StassocExists(int stassid, DBlist tree)
{
   struct stassocList *s = 0;
   do{
      if(!( s = (struct stassocList *) dblNextTableInstance(s, tree, dbl_LIST_STASSOC) ) )break;
      if(stassid == s->element->stassid)return 1;
   }while(s);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              




static int StassidInArrivalList(int stassid, DBlist tree)
{
   struct arrivalList *a = 0;
   do{
      if(!( a = (struct arrivalList *)dblNextTableInstance(a, tree, dbl_LIST_ARRIVAL) ) )break;
      if(stassid == a->element->stassid)return 1;
   }while(a);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              






static void AddMatchingStassocStruct(char *buffer, DBlist tree)
{
   int Off[] = {0, 9, 16, 24, 57, 65, 73, 83, 93, 103, 121, 129, 137, 145, 161, 170};
   struct stassocList *s;
   int stassid = atol(buffer);

   if(StassocExists(stassid, tree) ) return;
   if(!StassidInArrivalList(stassid, tree) ) return;

   s = (struct stassocList *) dblCreateTableInstance(tree, dbl_LIST_STASSOC);
   s->element->stassid     = stassid;
	    
   strncpy(s->element->sta,      buffer + Off[1], 6);  Deblank(s->element->sta, 7);
   strncpy(s->element->etype,    buffer + Off[2], 7);  Deblank(s->element->etype, 8);
   strncpy(s->element->location, buffer + Off[3], 32); Deblank(s->element->location, 33);
   s->element->dist    = atof(buffer + Off[4]);
   s->element->azimuth = atof(buffer + Off[5]);
   s->element->lat     = atof(buffer + Off[6]);
   s->element->lon     = atof(buffer + Off[7]);
   s->element->depth   = atof(buffer + Off[8]);
   s->element->time    = atof(buffer + Off[9]);
   s->element->imb     = atof(buffer + Off[10]);
   s->element->ims     = atof(buffer + Off[11]);
   s->element->iml     = atof(buffer + Off[12]);
   strncpy(s->element->auth,  buffer + Off[13], 15); Deblank(s->element->auth, 16);
   s->element->commid  = atol(buffer + Off[14]);
   strncpy(s->element->lddate,buffer + Off[15], 17); Deblank(s->element->lddate, 18);
}
/* ---------------------------------------------------------------------------------- */              






static int CommidFound( int commid, DBlist tree)
{
   struct arrivalList *ar = 0;
   struct assocList   *as = 0;
   struct eventList   *ev = 0;
   struct origerrList *oe = 0;
   struct originList  *orig = 0;
   struct stassocList *sa = 0;
   struct wfdiscList  *wf = 0;

   do{
      if(!( ar = (struct arrivalList *)dblNextTableInstance(ar, tree, dbl_LIST_ARRIVAL) ) )break;
      if(commid == ar->element->commid)return 1;
   }while(ar);

   do{
      if(!( as = (struct assocList*) dblNextTableInstance(as, tree, dbl_LIST_ASSOC) ) )break;
      if(commid == as->element->commid)return 1;
   }while(as);

   do{
      if(!( ev = (struct eventList *) dblNextTableInstance(ev, tree, dbl_LIST_EVENT) ) )break;
      if(commid == ev->element->commid)return 1;
   }while(ev);

   do{
      if(!( oe = (struct origerrList *) dblNextTableInstance(oe, tree, dbl_LIST_ORIGERR) ) )break;
      if(commid == oe->element->commid)return 1;
   }while(oe);

   do{
      if(!( orig = (struct originList *) dblNextTableInstance(orig, tree, dbl_LIST_ORIGIN) ) )break;
      if(commid == orig->element->commid)return 1;
   }while(orig);

   do{
      if(!( sa = (struct stassocList *) dblNextTableInstance(sa, tree, dbl_LIST_STASSOC) ) )break;
      if(commid == sa->element->commid)return 1;
   }while(sa);

   do{
      if(!( wf = (struct wfdiscList *) dblNextTableInstance(wf, tree, dbl_LIST_WFDISC) ) )break;
      if(commid == wf->element->commid)return 1;
   }while(wf);



   return 0;
}
/* ---------------------------------------------------------------------------------- */              





static int RemarkExists(int commid, DBlist tree)
{
   struct remarkList *r = 0;
   do{
      if(!( r = (struct remarkList *)dblNextTableInstance(r, tree, dbl_LIST_REMARK) ) )break;
      if(commid == r->element->commid)return 1;
   }while(r);
   return 0;
}
/* ---------------------------------------------------------------------------------- */              





static void AddMatchingRemarkStruct(char *buffer, DBlist tree)
{
   int Off[] = {0, 9, 18, 99};
   struct remarkList *r;
   int commid = atol(buffer);

   if(RemarkExists(commid, tree) ) return;

   if(!CommidFound(commid, tree) ) return;
   
   r = (struct remarkList *) dblCreateTableInstance(tree, dbl_LIST_REMARK);
   r->element->commid   = commid;
   r->element->lineno   = atol(buffer + Off[1]);
   strncpy(r->element->remark, buffer + Off[2], 80); Deblank(r->element->remark, 81);
   strncpy(r->element->lddate, buffer + Off[3], 17); Deblank(r->element->lddate, 18);
}
/* ---------------------------------------------------------------------------------- */              



         




static void AddMatchingSacdataStruct(char *buffer, DBlist tree)
{
   int Off[] = {0, 9, 18, 27, 36, 45, 54, 63, 72, 81, 90, 99, 108, 117, 126, 135, 144, 153, 162,
                171, 180, 189, 198, 207, 224, 241, 258, 275, 292, 309,};
   struct sacdataList *sd;
   int wfid = atol(buffer + Off[18]);

   if(!WfidInWfdiscList(wfid, tree) ) return;
   

   sd = (struct sacdataList *) dblCreateTableInstance(tree, dbl_LIST_SACDATA);

   strncpy(sd->element->userdata.label[0], buffer + Off[0], 9); 
   Deblank(sd->element->userdata.label[0], 9);
   sd->element->userdata.value[0] = atol(buffer + Off[1]);

   strncpy(sd->element->userdata.label[1], buffer + Off[2], 9); 
   Deblank(sd->element->userdata.label[1], 9);
   sd->element->userdata.value[1] = atol(buffer + Off[3]);

   strncpy(sd->element->userdata.label[2], buffer + Off[4], 9); 
   Deblank(sd->element->userdata.label[2], 9);
   sd->element->userdata.value[2] = atol(buffer + Off[5]);
   
   sd->element->userdata.value[3] = atol(buffer + Off[6]);
   sd->element->userdata.value[4] = atol(buffer + Off[7]);
   sd->element->userdata.value[5] = atol(buffer + Off[8]);
   sd->element->userdata.value[6] = atol(buffer + Off[9]);
   sd->element->userdata.value[7] = atol(buffer + Off[10]);
   sd->element->userdata.value[8] = atol(buffer + Off[11]);
   sd->element->userdata.value[9] = atol(buffer + Off[12]);

   sd->element->synthflag         = atol(buffer + Off[13]);
   sd->element->lpspol            = atol(buffer + Off[14]);
   sd->element->iztype            = atol(buffer + Off[15]);
   sd->element->idep              = atol(buffer + Off[16]);
   sd->element->iftype            = atol(buffer + Off[17]);
   sd->element->wfid              = atol(buffer + Off[18]);
   sd->element->nsnpts            = atol(buffer + Off[19]);
   sd->element->nxsize            = atol(buffer + Off[20]);
   sd->element->nysize            = atol(buffer + Off[21]);
   sd->element->leven             = atol(buffer + Off[22]);
   sd->element->fmt               = atof(buffer + Off[23]);
   sd->element->sb                = atof(buffer + Off[24]);
   sd->element->sdelta            = atof(buffer + Off[25]);
   sd->element->xminimum          = atof(buffer + Off[26]);
   sd->element->xmaximum          = atof(buffer + Off[27]);
   sd->element->yminimum          = atof(buffer + Off[28]);
   sd->element->ymaximum          = atof(buffer + Off[29]);
 

}
/* ---------------------------------------------------------------------------------- */              


         

static void ReadStructureFile(char* Root, DBlist tree, char *Suffix,
                              char *StructType, void (*fp)(char*,void*))
{
   FILE *ptr;
   char *FileName;
   char buffer[400];   
   int  buflen     = 400;
 
   UNUSED(StructType);
   FileName = cssConstructFilename(Root, Suffix);
   if(! (ptr = fopen( FileName, "rb" ) ) ){

      smFree(FileName);
      return;
   }
   else{
      while( fgetsp(buffer, buflen, ptr) ){
         if( !strlen(buffer) || *buffer == '\n' )continue;
         fp(buffer, tree);
      }
            
      fclose(ptr);
      smFree(FileName);
   }

}
/* ---------------------------------------------------------------------------------- */              







static void RemoveOridWftags( DBlist tree )
{
   struct wftagList *wt = 0 ;
   struct CSStree *Tree = (struct CSStree *) tree ;

   do{
      if(!( wt = (struct wftagList *) dblNextTableInstance(wt, tree, dbl_LIST_WFTAG) ) ) break;

      if( !strcmp( wt->element->tagname , "orid" ) )
         dblDeleteTableInstance( dbl_LIST_WFTAG, tree, (DBtable) Tree->wfHead ) ;

   }while(wt);
}
/* --------------------------------------------------------------------------------- */




static void CheckPrefors( DBlist tree )
{
   struct eventList *ev = 0 ;

   do{
      struct originList * orig = 0 ;

      if( !( ev = (struct eventList *) dblNextTableInstance( ev , tree, dbl_LIST_EVENT ) ) ) break ;

      if( findMatchingOrigin( tree , ev->element->evid, ev->element->prefor ) )
         continue ;

      do{
         if( !( orig = (struct originList *) dblNextTableInstance( orig , tree, dbl_LIST_ORIGIN ) ) ) break ;

         if( orig->element->evid == ev->element->evid ){
            ev->element->prefor = orig->element->orid ;
            break ;
         }
      }while( orig ) ;      
   }while( ev ) ;
}
/* --------------------------------------------------------------------------------- */











int cssReadFlatFiles(char *wfdiscName, char *WorkSetName, int Replace, int MaxWaveforms,
                    char** Station, int Nstation, 
                    char** Channel, int Nchannel, 
                    char** Band, int Nband, 
                    char** Code, int Ncode,
                    char** Orientation, int Norientation, 
                    char** Author, int Nauthor,
                    char** Filelist, int Nfiles,
		    char** Phaselist, int Nphases, int verbose,
		    double MaxPhysMem, int *takeEvid );



/*   wfdiscName is expected to be a complete filename. 
    
      WorkSetName is the name of the Workset to which structures will be added. If
      WorkSetName is NULL, then a default workset is created.
      
      If Replace is zero, then the new structures are appended to the tree in
      WorkSetName. Otherwise, The tree is cleared before adding the new structures.
      
      It is possible to restrict the lines read from the wfdisc file by means of
      the Station, Channel, Band, Code, and Orientation arrays. If these arrays
      are empty no restrictions are applied, and all lines are read. Otherwise
      restrictions are applied in the following manner:
          Station -------- Only lines whose sta matches one or more strings from Station
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

int cssReadFlatFiles(char *wfdiscName, char *WorkSetName, int Replace, int MaxWaveforms, 
                    char** Station, int Nstation, char** Channel,
                    int Nchannel, char** Band, int Nband, char** Code, int Ncode,
                    char** Orientation, int Norientation, char** Author, int Nauthor,
		    char** FileList, int Nfiles, char** Phaselist, int Nphases, int verbose,
		    double MaxPhysMem , int *takeEvid )
{
   DBlist tree;
   DBlist tree2;
   int Version;
   char *Root;
   int LinesUsed   = 0;
   MaxPhysMemToUse = MaxPhysMem;
   Verbose         = verbose;

   if(!WorkSetName || !strlen(WorkSetName) ){
     css_error("Invalid worksetname! Cannot add CSS ASCII data to workset.\n");
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
     if(Verbose) {
       css_warning("%d traces already in memory! No traces will be read.\n",
                   dblGetNumWaveformsInMemory(tree2));
     }
     return 0;
   }


   /* Check for not enough memory ... */
   if(smFracPhysMemUsed() > MaxPhysMemToUse){
      if(Verbose){
        css_warning("Waveforms already in SeisMgr memory are using more than %5.2f%% of physical memory.\n",
                    MaxPhysMemToUse * 100);
        css_warning("No waveforms will be read from this file.\n");
        css_warning("To utilize a higher percentage of physical memory use the MAXMEM option.\n");
      }
      return 0;                
   }


   if(!wfdiscName || ! strlen(wfdiscName)){
     css_error("Invalid (empty) wfdisc filename\n");
     return 0;
   }

   if(!wfdiscName || ! strlen(wfdiscName)){
     css_error("Invalid (empty) wfdisc filename\n");
     return 0;
   }


   Version   = GetWfdiscVersion(wfdiscName);

   Root      = StripSuffix( wfdiscName );
   if(!Root){
      css_error("Badly formed wfdisc filename: %s\n", wfdiscName);
      return 0;
   }


   if(Version == 28){
      LinesUsed = cssRead2_8FlatFiles(Root, WorkSetName, Replace, Station, Nstation, Channel, 
                                      Nchannel, Band, Nband, Code, Ncode, Orientation, 
                                      Norientation,  Author, Nauthor, FileList, Nfiles, 
                                      Phaselist, Nphases, MaxWaveforms, Verbose, MaxPhysMemToUse );
      *takeEvid = FALSE ;
      return LinesUsed;
   }
   else if(Version > 0 && Version != 30){
     css_info("Version is %3.1f\n", Version / 10.0);
     return 0;
   }
   else if(Version <= 0){
     css_error("Failed to determine version number!\n");
     return 0;
   }




   tree = dblNewTree(); /* This is a temp tree which will be merged after filling. */


   LinesUsed = ReadWfdiscFile(Root, WorkSetName, tree, Station, Nstation, Channel, Nchannel, 
                              Band, Nband, Code, Ncode, Orientation, Norientation, Replace,
			      FileList, Nfiles, MaxWaveforms); 

   if(!LinesUsed){
     if(Verbose) {
       css_warning("No rows returned from wfdisc file. Other tables ignored.\n");
     }
     return 0;
   }

   ReadStructureFile(Root, tree, ".wftag", "Wftag", AddMatchingWftagStruct);

   ReadStructureFile(Root, tree, ".event", "Event", AddMatchingEventStruct);

   /* 2-26-01 There was a problem removing Orid wftags 
      because wtpntr->prev was nil and because the wftag wfids
      weren't getting linked to the evid.  
      Fixed this by changing AddMatchingOriginStruct.  PG 2-26-01 */
   ReadOriginFile(Root, tree); 

   RemoveOridWftags( tree ) ;
   CheckPrefors( tree ) ;

   ReadArrivalFile(Root, tree, Author, Nauthor, Phaselist, Nphases);

   ReadStructureFile(Root, tree, ".assoc", "Assoc", AddMatchingAssocStruct);

   ReadStructureFile( Root, tree, ".sitechan", "Sitechan", AddMatchingSiteChanStruct);

   ReadStructureFile(Root, tree, ".site", "Site", AddMatchingSiteStruct);

   ReadStructureFile(Root, tree, ".affiliation", "Affiliation", AddMatchingAffiliationStruct);

   ReadStructureFile(Root, tree, ".origerr", "Origerr", AddMatchingOrigerrStruct);

   ReadStructureFile(Root, tree, ".sensor", "Sensor", AddMatchingSensorStruct);

   ReadStructureFile(Root, tree, ".instrument", "Instrument", AddMatchingInstrumentStruct);

   ReadStructureFile(Root, tree, ".gregion", "Gregion", AddMatchingGregionStruct);

   ReadStructureFile(Root, tree, ".stassoc", "Stassoc", AddMatchingStassocStruct);

   ReadStructureFile(Root, tree, ".remark", "Remark", AddMatchingRemarkStruct);

   ReadStructureFile(Root, tree, ".sacdata", "Sacdata", AddMatchingSacdataStruct);

   tree2 = dblMergeTrees(tree2, tree, *takeEvid );
   if(tree2 != tree) {
       dblDeleteTree(tree);
   }


   if(LinesUsed && Verbose){
     css_info("%d traces read into CSStree\n", LinesUsed);
     dblTableOfContents(tree2, stdout);
   }

   return LinesUsed;
}
/* -------------------------------------------------------------------------------------------- */   
