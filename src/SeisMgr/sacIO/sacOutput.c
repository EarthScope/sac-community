#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <float.h>
#include <unistd.h>
#include <ctype.h>

#include "hdr.h"
#include "timefuncs.h"
#include "cssListOps/cssStrucs.h"
#include "cssListOps/dblUserData.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "cssListOps/dblErrors.h"
#include "stringfun.h"

#define SAC_NULL_HEADER_REQUIRED /* This is hackish */
#include "SacHeader.h"

#include "sacIO/sacIO.h"
#include "sacIO/dbConversions.h"

#include "smDataIO.h"
#include "debug.h"

char *
sac_header_kt_number(struct SACheader *h, int i) {
    switch(i) {
    case 0: return h->kt0; break;
    case 1: return h->kt1; break;
    case 2: return h->kt2; break;
    case 3: return h->kt3; break;
    case 4: return h->kt4; break;
    case 5: return h->kt5; break;
    case 6: return h->kt6; break;
    case 7: return h->kt7; break;
    case 8: return h->kt8; break;
    case 9: return h->kt9; break;
    }
    return NULL;
}

struct sacdataList *findSacData(DBlist tree, int wfid);
int CSSchrDefined(char *value);
int lngDefined(int value);

/* ------------------------------------------------------------------ */

static int Round(float Val)
{
   int tmp = Val;
   float d1 = fabs(Val - tmp);
   float d2 = fabs(Val - tmp - 1);
   return (d1 < d2) ? tmp : tmp + 1;
}
/* ------------------------------------------------------------------ */





/* Make sure the specified directory exists and is writable. */
/* static int dirOK(char *dir) */
/* { */
/*    if(access(dir, F_OK | X_OK))return FALSE; */
/*    return TRUE; */
/* } */
/* ------------------------------------------------------------------ */





/* Make a filename from the station name, component, and reference time. */
/* static char *sacMakeFileName(struct wfdiscList *w, char *kstnm, char *kcmpnm) */
/* { */
/*    char *string; */
/*    string = (char *) malloc(25 + strlen(kstnm) + strlen(kcmpnm) ); */
/*    if(strcmp(kstnm, SAC_CHAR_UNDEFINED) ){ */
/*       strcpy(string,kstnm); */
/*       strcat(string,"."); */
/*    } */
/*    if(strcmp(kcmpnm,SAC_CHAR_UNDEFINED) ){ */
/*       strcat(string,kcmpnm); */
/*       strcat(string,"."); */
/*    } */
/*    strcat(string,tmListEpochTime(w->element->time, 0)); */
/*    strcat(string,".SAC"); */
/*    return string; */


/* } */
/* ------------------------------------------------------------------ */






/* Make complete filename with directory. */
/* static char *sacMakeCompleteName(struct wfdiscList *w, char *dir, char *fname,  */
/* 				 char *kstnm, char *kcmpnm) */
/* { */
/*    char * tmpFname, *tmpDir, *CompleteFileName; */
/*    int dirLen; */


/*    if(!fname || !strlen(fname)) */
/*       tmpFname = sacMakeFileName(w, kstnm, kcmpnm); */
/*    else{ */
/*       tmpFname = (char *) malloc(strlen(fname) + 1); */
/*       strcpy(tmpFname,fname); */
/*    } */

/*    if(!dir || !strlen(dir) || !dirOK(dir) ){ */
/*       tmpDir = (char *) malloc(2); /\* enough space for "." *\/ */
/*       strcpy(tmpDir,"."); */
/*    } */
/*    else{ */
/*       tmpDir = (char *) malloc(strlen(dir) + 1); */
/*       strcpy(tmpDir,dir); */
/*    } */
   
/*    dirLen = strlen(tmpDir); */
/*    CompleteFileName = (char *) malloc(strlen(tmpFname) + dirLen + 2); */
/*    if(tmpDir[dirLen -1] == '/')tmpDir[dirLen -1] = '\0'; */
/*    strcpy(CompleteFileName, tmpDir); */
/*    strcat(CompleteFileName,"/"); */
/*    strcat(CompleteFileName,tmpFname); */
/*    free(tmpDir); */
/*    free(tmpFname); */
/*    return CompleteFileName; */
/* } */
/* ------------------------------------------------------------------ */






/* Copy character strings from 9-byte header to 8-byte header for output */
/* static void FillCharHeader(struct SACheader *header, struct SACFileCharHeader   */
/* 		                                              *charHeader) */
/* { */

/*    strcpy(charHeader->kstnm,header->kstnm);  */
/*    strcpy(charHeader->kevnm,header->kevnm);  */
/*    strcpy(charHeader->khole,header->khole);  */
/*    strcpy(charHeader->ko,header->ko);  */
/*    strcpy(charHeader->ka,header->ka);  */
/*    strcpy(charHeader->kt0,header->kt0);  */
/*    strcpy(charHeader->kt1,header->kt1);  */
/*    strcpy(charHeader->kt2,header->kt2);  */
/*    strcpy(charHeader->kt3,header->kt3);  */
/*    strcpy(charHeader->kt4,header->kt4);  */
/*    strcpy(charHeader->kt5,header->kt5);  */
/*    strcpy(charHeader->kt6,header->kt6);  */
/*    strcpy(charHeader->kt7,header->kt7);  */
/*    strcpy(charHeader->kt8,header->kt8);  */
/*    strcpy(charHeader->kt9,header->kt9);  */
/*    strcpy(charHeader->kf,header->kf);  */
/*    strcpy(charHeader->kuser0,header->kuser0);  */
/*    strcpy(charHeader->kuser1,header->kuser1);  */
/*    strcpy(charHeader->kuser2,header->kuser2);  */
/*    strcpy(charHeader->kcmpnm,header->kcmpnm);  */
/*    strcpy(charHeader->knetwk,header->knetwk);  */
/*    strcpy(charHeader->kdatrd,header->kdatrd);  */
/*    strcpy(charHeader->kinst,header->kinst);  */
/* } */
/* ------------------------------------------------------------------ */


/* return an arrival that matches criteria and matches SACfield. */
static struct arrivalList *MatchingArrival(DBlist tree, char *sta, char *chan, double time, 
                                           double end, char *SACfield, int wfid)
{
   struct arrivalList *ar = 0;
   UNUSED(end);
   UNUSED(time);
   UNUSED(sta);
   UNUSED(chan);

   do{
      ar = (struct arrivalList *) dblNextTableInstance(ar, tree, dbl_LIST_ARRIVAL);
      if(!ar)break;
      if(! strcmp(ar->element->SACfield, SACfield) )
         if( AridJoinsWithWfid(tree, ar->element->arid, wfid) )
               return ar;
   }while(ar);


   return ar;
}
/* ------------------------------------------------------------------ */



/* return next arrival that matches criteria and has no SACfield set. */
/* This function looks from the current place in the arrivalList, so if it is
   called repeatedly, each time updating the list pointer to the new arrival,
   it will skip all arrivals with SACfield set, and will not repeat any arrivals.
*/
struct arrivalList *sacFindNextMatchingArrival(struct arrivalList *ar, 
					       DBlist tree, char *sta, char *chan,
                                               double time, double end, int wfid)

{
   struct arrivalList *result;
   UNUSED(end);
   UNUSED(time);
   UNUSED(chan);
   UNUSED(sta);

   result = ar;
   do{
      result = (struct arrivalList *) dblNextTableInstance(result, tree, dbl_LIST_ARRIVAL);
      if(!result)break;
      if(! strcmp(result->element->SACfield, "-") )
         if( AridJoinsWithWfid(tree, result->element->arid, wfid) )
               return result;
   }while(result);


   return 0;
}
/* ------------------------------------------------------------------ */





struct arrivalList *sacFindNextMatchingPick(struct arrivalList *ar,
                                            DBlist tree, int wfid)

{
   struct arrivalList *result;

   result = ar;
   do{
      result = (struct arrivalList *) dblNextTableInstance(result, tree, dbl_LIST_ARRIVAL);
      if(!result)break;
      if( AridJoinsWithWfid(tree, result->element->arid, wfid) )
          return result;
   }while(result);


   return 0;
}
/* ------------------------------------------------------------------ */







static struct sitechanList *sacFindSiteChan(DBlist tree, char *sta, char *chan,
                                            int jdate, int wfid)
{
   struct sitechanList *sc = 0;
   UNUSED(wfid);

      do{
      sc = (struct sitechanList *) dblNextTableInstance(sc, tree, dbl_LIST_SITECHAN);
      if(sc){
        if( CSSchrDefined(sta) && CSSchrDefined(chan) )
	   if(!strcmp(sta, sc->element->sta) && !strcmp(chan, sc->element->chan) )
              if(sc->element->ondate <= jdate || jdate < 0) 
                 if(sc->element->offdate >= jdate || sc->element->offdate < 0) 
                   return sc;
      }
   }while(sc); 
   return 0;
}
/* ------------------------------------------------------------------ */




static struct siteList *sacFindSite(DBlist tree, char *sta, int jdate, int wfid)
{
   struct siteList *si = 0;

   UNUSED(wfid);


   /* First try the normal join using sta, jdate. */
   DEBUG("Looking for station: %s\n", sta);
   if( CSSchrDefined(sta) ) {
      do{
         si = (struct siteList *) dblNextTableInstance(si, tree, dbl_LIST_SITE);
         DEBUG("%p [%p]\n", si, tree);
         if(!si)break;
         if(! strcmp(si->element->sta, sta) )
            if(si->element->ondate <= jdate)
               if(si->element->offdate >= jdate || si->element->offdate < 0)
                  return si;
      }while(si);
   } else {
     DEBUG("warning: Station undefined: %s\n", sta);
   }

   return 0;
}
/* ------------------------------------------------------------------ */


static struct originList *sacFindMatchingOrigin(DBlist tree, int orid)
{
   struct originList *orig = 0;
   do{
      orig = (struct originList *) dblNextTableInstance(orig, tree, dbl_LIST_ORIGIN);
      if(orig && orig->element->orid == orid) return orig;
   }while(orig);

   return 0;
}
/* ------------------------------------------------------------------ */




/* prototype for use here. */
static struct eventList *sacFindEvent(DBlist tree, int evid);

static struct originList *sacFindOrigin(DBlist tree, struct wfdiscList *w)
{
   struct originList *orig = 0 ;
   struct eventList  *ev   = 0 ;
   struct wftagList  *wt   = 0 ;
   char Tmp[10];
   do{
      if(!(wt = (struct wftagList *) dblNextTableInstance(wt, tree, dbl_LIST_WFTAG) ) )break;
      strcpy(Tmp, wt->element->tagname);
      Upstring(Tmp);
      if( !strcmp( Tmp, "EVID" ) ){
	 if(wt->element->wfid == w->element->wfid){
            ev = sacFindEvent(tree, wt->element->tagid);
            if( !ev ) continue ;
            orig = sacFindMatchingOrigin( tree, ev->element->prefor ) ;
            if( orig )
               return orig ;
	 }
      }
   }while(wt);


   return 0;
}
/* ------------------------------------------------------------------ */







static struct assocList *sacFindAssoc(DBlist tree, int orid, char* sta)
{
   struct assocList *as = 0;

   do{
      as = (struct assocList *) dblNextTableInstance(as, tree, dbl_LIST_ASSOC);
      if(as && as->element->orid == orid)
         if(!strcmp(as->element->sta, sta) ) return as;
   }while(as);
   return 0;
}
/* ------------------------------------------------------------------ */




static struct assocList *sacFindAssocFromArid(DBlist tree, int arid)
{
   struct assocList *as = 0;

   do{
      as = (struct assocList *) dblNextTableInstance(as, tree, dbl_LIST_ASSOC);
      if(as && as->element->arid == arid)
         return as;
   }while(as);
   return 0;
}
/* ------------------------------------------------------------------ */







static struct affiliationList *sacFindAffiliation(DBlist tree, char *kstnm)
{
   struct affiliationList *af = 0;

   do{
      af = (struct affiliationList *) dblNextTableInstance(af, tree, dbl_LIST_AFFILIATION);
      if(af && !strcmp(af->element->sta, kstnm) ) return af;
   }while(af);
   return 0;
}
/* ------------------------------------------------------------------ */






static struct eventList *sacFindEvent(DBlist tree, int evid)
{
   struct eventList *ev = 0;

   do{
      ev = (struct eventList *) dblNextTableInstance(ev, tree, dbl_LIST_EVENT);
      if(ev && (ev->element->evid == evid) ) return ev;
   }while(ev);
   return 0;
}
/* ------------------------------------------------------------------ */





static struct wftagList *sacFindWftag(DBlist tree, int wfid, char* tagname )
{
   struct wftagList *wt = 0;

   do{
      wt = (struct wftagList *) dblNextTableInstance(wt, tree, dbl_LIST_WFTAG);
      if(wt && !strcmp( wt->element->tagname, tagname ) &&
         (wt->element->wfid == wfid) ) return wt;
   }while(wt);
   return 0;
}
/* ------------------------------------------------------------------ */






int CSSfltDefined(float value)
{
  return (value == -999.0) ? FALSE : TRUE;
}
/* ------------------------------------------------------------------ */






int CSSdblDefined(double value)
{
  return (value == -9999999999.999) ? FALSE : TRUE;
}
/* ------------------------------------------------------------------ */






int CSSlngDefined(int value)
{
  return (value == -1) ? FALSE : TRUE;
}
/* ------------------------------------------------------------------ */






int CSSchrDefined(char *value)
{
  return strcmp(value,"-") ? TRUE : FALSE;
}
/* ------------------------------------------------------------------ */






static int sacSetEvType(char *etype)
{
   if(!strcmp(etype, "qb"))return IQB;
   if(!strcmp(etype, "qb+"))return IQB1;
   if(!strcmp(etype, "qb++"))return IQB2;
   if(!strcmp(etype, "qbx"))return IQBX;
   if(!strcmp(etype, "qd"))return IEQ3 ;
   if(!strcmp(etype, "qf"))return IEQ2 ;
   if(!strcmp(etype, "qp"))return IEQ0 ;
   if(!strcmp(etype, "qt"))return IEQ  ;
   if(!strcmp(etype, "qmt"))return IQMT;
   if(!strcmp(etype, "eq"))return IEQ;
   if(!strcmp(etype, "eq+"))return IEQ1;
   if(!strcmp(etype, "eq++"))return IEQ2;
   if(!strcmp(etype, "mb"))return  IQMT ;
   if(!strcmp(etype, "mc"))return IQC ;
   if(!strcmp(etype, "me"))return IQB;
   if(!strcmp(etype, "mp"))return IQB0 ;
   if(!strcmp(etype, "ec"))return ICHEM ;
   if(!strcmp(etype, "en"))return INU ;
   if(!strcmp(etype, "ep"))return IEX0 ;
   if(!strcmp(etype, "ex"))return IEX;
   if(!strcmp(etype, "nu"))return INU;
   if(!strcmp(etype, "nc"))return INC;
   if(!strcmp(etype, "ge"))return IGEY ;
   if(!strcmp(etype, "xl"))return ILIT ;
   if(!strcmp(etype, "xm"))return IMET ;
   if(!strcmp(etype, "xo"))return IODOR ;
   if(!strcmp(etype, "o"))return IO_;
   if(!strcmp(etype, "l"))return IL;
   if(!strcmp(etype, "r"))return IR;
   if(!strcmp(etype, "t"))return IT;
   if(!strcmp(etype, "Unk"))return IUNKNOWN;
   if(!strcmp(etype, "PreN"))return IPREN;
   if(!strcmp(etype, "PostN"))return IPOSTN;
   if(!strcmp(etype, "ForeShk"))return IPREQ;
   if(!strcmp(etype, "AftrShk"))return IPOSTQ;
   if(!strcmp(etype, "ChemExp"))return ICHEM;
   if(!strcmp(etype, "Other"))return IOTHER;


   return IUNKNOWN;
}
/* ------------------------------------------------------------------ */




void SetMagnitudeSource(struct SACheader *h,  struct originList *orig)
{
  if(!CSSchrDefined(orig->element->auth) ){h->imagsrc = SAC_INT_UNDEFINED; return;}
  if(!strcmp(CpUpstring(orig->element->auth), "UNKNOWN")){h->imagsrc = IUNKNOWN; return;}
  if(!strcmp(CpUpstring(orig->element->auth), "NEIC")){h->imagsrc = INEIC;return;}
  if(!strncmp(CpUpstring(orig->element->auth), "PDE",3)){h->imagsrc = IPDE;return;}
  if(!strncmp(CpUpstring(orig->element->auth), "PDE-Q",5)){h->imagsrc = IPDEQ;return;}
  if(!strncmp(CpUpstring(orig->element->auth), "PDE-W",5)){h->imagsrc = IPDEW;return;}
  if(!strcmp(CpUpstring(orig->element->auth), "ISC")){h->imagsrc = IISC;return;}
  if(!strcmp(CpUpstring(orig->element->auth), "REB")){h->imagsrc = IREB;return;}
  if(!strncmp(CpUpstring(orig->element->auth), "USGS",4)){h->imagsrc = IUSGS;return;}
  if(!strcmp(CpUpstring(orig->element->auth), "BRK")){h->imagsrc = IBRK;return;}
  if(!strcmp(CpUpstring(orig->element->auth), "CALTECH")){h->imagsrc = ICALTECH;return;}
  if(!strcmp(CpUpstring(orig->element->auth), "LLNL")){h->imagsrc = ILLNL;return;}
  if(!strcmp(CpUpstring(orig->element->auth), "EVLOC")){h->imagsrc = IEVLOC;return;}
  if(!strcmp(CpUpstring(orig->element->auth), "JSOP")){h->imagsrc = IJSOP;return;}
  if(!strcmp(CpUpstring(orig->element->auth), "USER")){h->imagsrc = IUSER;return;}
  
  h->imagsrc = SAC_INT_UNDEFINED;
  return;

}
/* ------------------------------------------------------------------ */






void SetMagnitudeInHeader(struct SACheader *h,  struct originList *orig, MagType Mtype)
{


  if(Mtype == Any){
    if( CSSfltDefined(orig->element->ms) && orig->element->ms >= 6.6 ){
       h->mag = orig->element->ms;
       h->imagtyp = IMS;
       return;
    }
    if( CSSfltDefined(orig->element->mb)){
       h->mag = orig->element->mb;
       h->imagtyp = IMB;
       return;
    }
    if( CSSfltDefined(orig->element->ms)){
       h->mag = orig->element->ms;
       h->imagtyp = IMS;
       return;
    }
    if( CSSfltDefined(orig->element->ml)){
       h->mag = orig->element->ml;
       h->imagtyp = IML;
       return;
    }
  }
        
  if( Mtype == MsMag && CSSfltDefined(orig->element->ms)){
     h->mag = orig->element->ms;
     h->imagtyp = IMS;
     return;
  }

  if( Mtype == MbMag && CSSfltDefined(orig->element->mb)){
     h->mag = orig->element->mb;
     h->imagtyp = IMB;
     return;
  }

  if( Mtype == MlMag && CSSfltDefined(orig->element->ml)){
     h->mag = orig->element->ml;
     h->imagtyp = IML;
     return;
  }

}
/* ------------------------------------------------------------------ */






static void SetDistAzValues(struct SACheader *header)
{
   float delt,dist,azim,bazim;


   if(header->stla == SAC_FLOAT_UNDEFINED)return;
   if(header->stlo == SAC_FLOAT_UNDEFINED)return;
   if(header->evla == SAC_FLOAT_UNDEFINED)return;
   if(header->evlo == SAC_FLOAT_UNDEFINED)return;

   delt = dist = azim = bazim = 0.0;
   dbDelaz(&(header->stla), &(header->stlo), &(header->evla), &(header->evlo), 
           &delt, &dist, &azim, &bazim);
   header->dist = dist;
   if(header->gcarc == SAC_FLOAT_UNDEFINED)header->gcarc  = delt;
   if(header->baz == SAC_FLOAT_UNDEFINED)header->baz = bazim;
   if(header->az == SAC_FLOAT_UNDEFINED)header->az = azim;
   return;
}
/* ------------------------------------------------------------------ */






void sacHeaderFromCSS(DBlist tree, struct SACheader *header, struct wfdiscList *w, int RefTimeType,
                                   double *RefTime, MagType Mtype)
{
   struct sitechanList *sc;
   struct siteList *si;
   struct originList *orig;
   struct eventList *ev;
   struct assocList *aso;
   struct assocList *as;
   struct affiliationList *af;
   struct arrivalList *ar, *arm;
   struct sacdataList *sd;
   struct wftagList *wt = 0 ;

   int year, month, day, hour, min;
   float second;
   int i;
   char SACfield[3];
   float TmpMilliSeconds;


   *header = NullSacHeader;


   header->npts = w->element->nsamp;
   if( CSSchrDefined(w->element->instype) )
      strcpy(header->kinst,w->element->instype);

   header->nwfid = w->element->wfid; 

/* Find an origin struct matched to w and copy its info to header. */
   if( (orig = sacFindOrigin(tree, w) ) ){
      if( CSSfltDefined(orig->element->lat) )   header->evla   = orig->element->lat;
      if( CSSfltDefined(orig->element->lon) )   header->evlo   = orig->element->lon;
      if( CSSfltDefined(orig->element->depth) ) header->evdp   = orig->element->depth * 1000.0;
      if( CSSlngDefined(orig->element->grn) )   header->ievreg = orig->element->grn + REGCONV ;
      if( CSSlngDefined(orig->element->orid)    )header->norid = orig->element->orid;
      if( CSSlngDefined(orig->element->evid) ){
         header->nevid = orig->element->evid;
      }
      header->ievtyp = sacSetEvType(orig->element->etype);
      if( (as = sacFindAssoc(tree, orig->element->orid, w->element->sta) ) ){
	 if(isValidFloat(dbl_LIST_ASSOC, dbl_ASSOC_DELTA, as->element->delta) )
	     header->gcarc = as->element->delta;
	 if( CSSfltDefined(as->element->seaz) )   header->baz = as->element->seaz;
	 if( CSSfltDefined(as->element->esaz)     )header->az = as->element->esaz;
      }
      SetMagnitudeInHeader(header, orig, Mtype);
      SetMagnitudeSource(header, orig);
   }

   if(orig && RefTimeType == IO && CSSdblDefined(orig->element->time)){
      header->iztype = IO;
      *RefTime = orig->element->time;
   }
   else{
      header->iztype = IB;
      *RefTime = w->element->time;
   }
      
      
   header->b = (float) (w->element->time - *RefTime);
   if(orig && CSSdblDefined(orig->element->time) ){
      header->o = orig->element->time - *RefTime;
      strcpy(header->ko, "O");
   }

   if(w->element->samprate)
	header->delta = 1.0 / w->element->samprate;
   else
        header->delta = 1.0;

   header->e = header->b + (float) ( (header->npts -1) * header->delta);

   tmDecodeEpochTime(*RefTime, &year, &month, &day, &hour, &min, &second);
   header->nzyear  = year;
   header->nzjday  = yrday( month, day, isleap( year) );
   header->nzhour  = hour;
   header->nzmin   = min;
   header->nzsec   = (int) second;
   TmpMilliSeconds = (second - (int) second) * 1000;
   header->nzmsec  = Round(TmpMilliSeconds);

   if(w->seis->Cmplx){
      header->iftype = IXY;
      header->leven = FALSE;
   }
   else{
      header->iftype = ITIME;
      header->leven  = TRUE;
      header->depmin = FLT_MAX;
      header->depmax = -header->depmin;
      header->depmen = 0.0;
      for(i=0;i<header->npts;i++){
	 header->depmen += *(w->seis->i + i);
	 if(header->depmin > *(w->seis->i + i) ) header->depmin = *(w->seis->i + i);
	 if(header->depmax < *(w->seis->i + i) ) header->depmax = *(w->seis->i + i);
      }
      if(header->npts) header->depmen /= header->npts;
   }



   /* Get A variable if any are marked as such. */

   strcpy(SACfield, "A");
   arm = MatchingArrival(tree, w->element->sta, w->element->chan, w->element->time,
                         w->element->endtime, SACfield, w->element->wfid);
   if(arm){
      header->a = arm->element->time - *RefTime;
      aso = sacFindAssocFromArid( tree , arm->element->arid ) ;
      if ( aso && isValidString ( dbl_LIST_ASSOC , dbl_ASSOC_PHASE , aso->element->phase ) )
         strncpy(header->ka, aso->element->phase,8);
      else
         strncpy(header->ka, arm->element->iphase,8);
   }

   /* Get T0 - T9 */
   ar = 0;
   for(i=0;i<10;i++){
      sprintf(SACfield,"T%d",i);
      SACfield[2] = '\0';
      arm = MatchingArrival(tree, w->element->sta, w->element->chan, w->element->time,
                           w->element->endtime, SACfield, w->element->wfid);
      if(!arm){  /* If you can't find an arrival marked for Ti, take the next unmarked arrival. */
         ar = sacFindNextMatchingArrival(ar, tree, w->element->sta, w->element->chan,
                                         w->element->time, w->element->endtime, w->element->wfid);
         if ( !ar ) continue ;
         arm = ar;
      }
      aso = sacFindAssocFromArid( tree , arm->element->arid ) ;
      if ( aso && isValidString ( dbl_LIST_ASSOC , dbl_ASSOC_PHASE , aso->element->phase ) ) {
         if ( toupper(aso->element->phase[0]) == 'F' ) continue;
         char *kt = sac_header_kt_number(header, i);
         if(kt) {
             strncpy(kt, aso->element->phase,8);
         }
      }
      else {
         if( toupper(arm->element->iphase[0]) == 'F' ) continue;
         char *kt = sac_header_kt_number(header, i);
         if(kt) {
             strncpy(kt, arm->element->iphase,8);
         }
      }
      *(&(header->t0) + i) = arm->element->time - *RefTime;
      strcpy(arm->element->SACfield, SACfield);
   }



/* Now get the Fini arrival if present. */
   strcpy(SACfield, "F");
   arm = MatchingArrival(tree, w->element->sta, w->element->chan, w->element->time,
                         w->element->endtime, SACfield, w->element->wfid);
   if(arm){
	 header->f = arm->element->time - *RefTime;
         aso = sacFindAssocFromArid( tree , arm->element->arid ) ;
         if ( aso && isValidString ( dbl_LIST_ASSOC , dbl_ASSOC_PHASE , aso->element->phase ) )
            strncpy(header->kf, aso->element->phase,8);
         else
	    strcpy( header->kf, arm->element->iphase);
   }
   else{
      ar = 0;
      do {
         ar = sacFindNextMatchingArrival(ar, tree, w->element->sta, w->element->chan,
                                         w->element->time, w->element->endtime, w->element->wfid);
         if(!ar)break;
         aso = sacFindAssocFromArid( tree , ar->element->arid ) ;
         if ( aso && isValidString ( dbl_LIST_ASSOC , dbl_ASSOC_PHASE , aso->element->phase ) ) {
            if( toupper(aso->element->phase[0]) == 'F' ){
               header->f = ar->element->time - *RefTime;
               strcpy( header->kf, aso->element->phase);
            }
         }
         else {
            if( toupper(ar->element->iphase[0]) == 'F' ){
	       header->f = ar->element->time - *RefTime;
	       strcpy( header->kf, ar->element->iphase);
            }
         }
      }while(ar);
   }

   if( CSSchrDefined(w->element->sta) )
      strncpy(header->kstnm, w->element->sta, 8);

   if( CSSchrDefined(w->element->chan) )
      strncpy(header->kcmpnm, w->element->chan, 8);


   sc = sacFindSiteChan(tree, w->element->sta, w->element->chan, w->element->jdate,
                        w->element->wfid );
   if(sc ){
       if ( isValidFloat ( dbl_LIST_SITECHAN , dbl_SITEC_VANG , sc->element->vang ) )
          header->cmpinc   = sc->element->vang;
       else
          header->cmpinc = -12345.0 ;
       if ( isValidFloat ( dbl_LIST_SITECHAN , dbl_SITEC_HANG , sc->element->hang ) )
          header->cmpaz    = sc->element->hang;
       else
          header->cmpaz = -12345.0 ;
       if(!header->cmpinc) 
          header->cmpaz = 0.0;
       header->stdp     = sc->element->edepth * 1000.0 ;
   }

   if( (si = sacFindSite(tree, w->element->sta, w->element->jdate, w->element->wfid) ) ){
       if( CSSfltDefined(si->element->lat) )  header->stla = si->element->lat;
       if( CSSfltDefined(si->element->lon) )  header->stlo = si->element->lon;
       if( CSSfltDefined(si->element->elev) ) header->stel = si->element->elev * 1000;
       strncpy(header->kuser1 , si->element->refsta , 6 ) ; 
	header->kuser1[6] = '\0' ;
   } else {
     DEBUG("warning: Could not find Site: %s [%d]\n", w->element->sta, w->element->wfid);
   }
   SetDistAzValues(header);


  /* lpspol - true if station components have positive polarity */
   header->lpspol = FALSE;
  
   /* lcalca - true if event info is to be calculated from position */
   header->lcalda = TRUE;

   /* lovrok - true if the file can be overwritten */
   header->lovrok = TRUE;

   /* SAC header version number */
   header->nvhdr = 6;


   sd = findSacData(tree, w->element->wfid);
   if( sd ) {
      strcpy(header->kuser0, sd->element->userdata.label[0]);
      strcpy(header->kuser1, sd->element->userdata.label[1]);
      strcpy(header->kuser2, sd->element->userdata.label[2]);

      header->user0    = sd->element->userdata.value[0];
      header->user1    = sd->element->userdata.value[1];
      header->user2    = sd->element->userdata.value[2];
      header->user3    = sd->element->userdata.value[3];
      header->user4    = sd->element->userdata.value[4];
      header->user5    = sd->element->userdata.value[5];
      header->user6    = sd->element->userdata.value[6];
      header->user7    = sd->element->userdata.value[7];
      header->user8    = sd->element->userdata.value[8];
      header->user9    = sd->element->userdata.value[9];

      header->isynth   = sd->element->synthflag;
      header->lpspol   = sd->element->lpspol;
      header->idep     = sd->element->idep;
      header->iftype   = sd->element->iftype;
      header->nsnpts   = sd->element->nsnpts;
      header->nxsize   = sd->element->nxsize;
      header->nysize   = sd->element->nysize;
      header->leven    = sd->element->leven;
      header->fmt      = sd->element->fmt;
      header->sb       = sd->element->sb;
      header->sdelta   = sd->element->sdelta;
      header->xminimum = sd->element->xminimum;
      header->xmaximum = sd->element->xmaximum;
      header->yminimum = sd->element->yminimum;
      header->ymaximum = sd->element->ymaximum;
      header->odelta   = sd->element->odelta;
      header->resp0    = sd->element->resp[0];
      header->resp1    = sd->element->resp[1];
      header->resp2    = sd->element->resp[2];
      header->resp3    = sd->element->resp[3];
      header->resp4    = sd->element->resp[4];
      header->resp5    = sd->element->resp[5];
      header->resp6    = sd->element->resp[6];
      header->resp7    = sd->element->resp[7];
      header->resp8    = sd->element->resp[8];
      header->resp9    = sd->element->resp[9];
      header->evel     = sd->element->evel;
      header->az       = sd->element->az;
      header->baz      = sd->element->baz;
      header->gcarc    = sd->element->gcarc;
      header->dist     = sd->element->dist;
      header->iinst    = sd->element->iinst;
      if ( lngDefined  ( sd->element->istreg ) )
        header->istreg = sd->element->istreg + REGCONV ;
      header->iqual    = sd->element->iqual;
      header->lovrok   = sd->element->lovrok;
      header->lcalda   = sd->element->lcalda;
      strcpy(header->khole, sd->element->khole);
      strcpy(header->ko, sd->element->ko);
      strcpy(header->kdatrd, sd->element->kdatrd );
   }







   strncpy(header->kdatrd, tmListEpochTime( tmGetEpochTime(), 18 ) , 8 );
   header->kdatrd[ 8 ] = '\0' ;

   if( (af = sacFindAffiliation(tree, header->kstnm) ) ){
      strcpy(header->knetwk, af->element->net);
   }

   if( ( wt = sacFindWftag( tree, w->element->wfid, "evid" ) ) ) {
      if( (ev = sacFindEvent(tree, wt->element->tagid) ) ){
         if( CSSchrDefined(ev->element->evname) )
            strcpy(header->kevnm, ev->element->evname);
      }
   }


}
/* ------------------------------------------------------------------ */







/* int sacWriteSacFile(DBlist tree, struct wfdiscList *w, char  */
/* 		                                       *dir, char *fname) */
/* { */
/*    char *CompleteFileName; */
/*    struct SACheader *header; */
/*    int bytesToWrite; */
/*    struct SACFileCharHeader charHeader; */
/*    int numStrings=23; */
/*    double RefTime; */
/*    MagType Mtype = Any; */

/*    header = (struct SACheader *) malloc( sizeof(struct SACheader) ); */
/*    if(!header){ */
/*       printf("Error allocating header in sacWriteSacFile.\n"); */
/*       return 0; */
/*    } */
/*    sacHeaderFromCSS( tree, header, w, IO, &RefTime, Mtype); */
/*    CompleteFileName = sacMakeCompleteName(w, dir, fname, header->kstnm,  */
/* 					                header->kcmpnm); */
   
   
/*    if( (sacfile=fopen(CompleteFileName, "wb") ) == (FILE *) NULL ){ */
/*       strcpy(sacErrorStrg,"Error opening SAC file: "); */
/*       strcat(sacErrorStrg,CompleteFileName); */
/*       free(CompleteFileName); */
/*       free(header); */
/*       dblSetError(0, sacErrorStrg); */
/*       return FALSE; */
/*   } */

   
/*    bytesToWrite = sizeof(struct SACheader) -  */
/*                   sizeof(struct SACFileCharHeader) - numStrings -1; */
/*    if( fwrite( (char *) header, bytesToWrite, 1, sacfile) != 1){ */
/*       strcpy(sacErrorStrg,"Error writing SAC file header: "); */
/*       strcat(sacErrorStrg,CompleteFileName); */
/*       free(header); */
/*       free(CompleteFileName); */
/*       fclose(sacfile); */
/*       dblSetError(0, sacErrorStrg); */
/*       return FALSE; */
/*    } */


/*    FillCharHeader(header, &charHeader); */
/*    if( fwrite( (char *) &charHeader, sizeof(struct SACFileCharHeader),  */
/* 	                                               1, sacfile) != 1){ */
/*       strcpy(sacErrorStrg,"Error writing SAC file header: "); */
/*       strcat(sacErrorStrg,CompleteFileName); */
/*       free(CompleteFileName); */
/*       free(header); */
/*       fclose(sacfile); */
/*       dblSetError(0, sacErrorStrg); */
/*       return FALSE; */
/*    } */



/*    if(w->seis->Cmplx && w->seis->r){ */
/*       if( fwrite( (char *) (w->seis->r), sizeof(float),header->npts,   */
/*                                                 sacfile) != header->npts){ */
/*          strcpy(sacErrorStrg,"Error writing SAC file independent data: "); */
/*          strcat(sacErrorStrg,CompleteFileName); */
/*          free(CompleteFileName); */
/*          free(header); */
/*          fclose(sacfile); */
/*          dblSetError(0, sacErrorStrg); */
/*          return FALSE; */
/*       } */

/*    } */


/*    if( fwrite( (char *) (w->seis->i), sizeof(float),header->npts, sacfile)  */
/*                                                          != header->npts){ */
/*       strcpy(sacErrorStrg,"Error writing SAC file dependent data: "); */
/*       strcat(sacErrorStrg,CompleteFileName); */
/*       free(CompleteFileName); */
/*       free(header); */
/*       fclose(sacfile); */
/*       dblSetError(0, sacErrorStrg); */
/*       return FALSE; */
/*    } */



 
/*     fclose(sacfile); */
/*     free(header); */
/*     free(CompleteFileName); */
/*     return TRUE; */

/* } */
/* ------------------------------------------------------------------ */


