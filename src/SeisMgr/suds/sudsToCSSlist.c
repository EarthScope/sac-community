#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "suds/suds.h"
#include "suds/sudsListOps.h"
#include "suds/sudsIO.h"

#include "timefuncs.h"
#include "cssListOps/cssStrucs.h"
#include "cssListOps/dblUserData.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "cssListOps/cssListStrucs.h"
#include "cssListOps/dblErrors.h"
#include "smDataIO.h"
#include "smMemory/smMemory.h"
#include "sacIO/dbConversions.h"

#include "debug.h"

static char StaticString[100];

static int GlobalOrid;
static int GlobalEvid;
static int Verbose;
static int TracesInExistingTree;
static int MaxWaveforms;
SUDS *ReadSUDSfile(char *, int Verbose);



char *sudsGetEventType(SUDS *S, int ev_num)
{
   evList *ev = S->evHead;
   while(ev){
      if(ev->ev->number == ev_num){
         switch(ev->ev->ev_type){
         case 'e':
         strcpy(StaticString, "eq");return StaticString;
         case 'E':
         strcpy(StaticString, "qb");return StaticString;
         case 'n':
         strcpy(StaticString, "ex");return StaticString;
         case 'i':
         strcpy(StaticString, "o");return StaticString;
         case 'r':
         strcpy(StaticString, "r");return StaticString;
         case 't':
         strcpy(StaticString, "t");return StaticString;
         }
       }
       ev = ev->next;
    }
    strcpy(StaticString, "-");
    return StaticString;
    
}
/* -------------------------------------------------------------- */





float* DataFromSuds(dtList *dt)
{
   int j;
   float *Data;
  
   if(!dt || !dt->dt)return 0;
   if(dt->dt->length < 1)return 0;

   Data = (float*) smMalloc( dt->dt->length * sizeof(float) );
   if(!Data){
      printf("Error allocating Data in DataFromSuds!\n");
      return 0;
   }
  
   switch (dt->dt->datatype){
   case 's': case 'q': case 'u': case 'i': case 'r':
     if(! dt->i2data){
        printf("ERROR: Short datatype specified, but none found!\n");
        return 0;
     }
     for(j=0;j<dt->dt->length;j++)
        *(Data + j) = *(dt->i2data + j);
     return Data;
     
   case 'l': case '2':
     if(! dt->i4data){
        printf("ERROR: Long datatype specified, but none found!\n");
        return 0;
     }
     for(j=0;j<dt->dt->length;j++)
        *(Data + j) = *(dt->i4data + j);
     return Data;
     
   case 'f':
     if(! dt->r4data){
        printf("ERROR: Float datatype specified, but none found!\n");
        return 0;
     }
     memcpy(Data, dt->r4data, dt->dt->length * sizeof(float) );
     return Data;
     
   default:
      printf("ERROR: Unrecognized Suds data format code.\n");
      return NULL ;
   } 
}
/* ------------------------------------------------------------------ */



static struct originList *sudsAddOriginStruct(SUDS *S, DBlist tree, orList *orS)
{
   struct originList *orig;
   struct eventList *ev = 0 ;
   int year, month, day, hour, min;
   float second;

   orig = (struct originList *) dblCreateTableInstance(tree,dbl_LIST_ORIGIN);
   if(!orig)return 0;
   
   
   if(orS->orig->or_lat  != NODATA)  orig->element->lat   = orS->orig->or_lat;
   if(orS->orig->or_long != NODATA)  orig->element->lon   = orS->orig->or_long;
   if(orS->orig->depth   != NODATA)  orig->element->depth = orS->orig->depth;
   if(orS->orig->orgtime != NOTIME){
      orig->element->time  = orS->orig->orgtime;
      tmDecodeEpochTime(orS->orig->orgtime, &year, &month, &day,&hour, &min, &second);
      orig->element->jdate = year*1000 + yrday( month, day, isleap( year) );
   }
      
      
   orig->element->orid  = GlobalOrid;
   orig->element->evid  = GlobalEvid;
   if(orS->orig->number != NODATA){
      strcpy(orig->element->etype, sudsGetEventType(S, orS->orig->number) );
   }
   if(orS->orig->region != NODATA) orig->element->grn = orS->orig->region;

   strncpy(orig->element->auth, GetCharAuth(orS->orig->authority), 15);
   strncpy(orig->element->algorithm, GetCharProg(orS->orig->program), 15);
   strcpy(orig->element->lddate, tmListEpochTime( tmGetEpochTime(), 18 ) );
   


   if(orS->orig->magnitude != NODATA){
     switch(orS->orig->mag_type){
     case 'b':
        orig->element->mb = orS->orig->magnitude;
        break;
     case 's':
        orig->element->ms = orS->orig->magnitude;
        break;
     case 'l':
        orig->element->ml = orS->orig->magnitude;
        break;
     default:
        orig->element->mb = orS->orig->magnitude;
     }
   }

   /* Produce accompanying event file. */
   ev = (struct eventList *) dblCreateTableInstance(tree,dbl_LIST_EVENT ) ;
   if( !ev ) return orig ;

   ev->element->evid = GlobalEvid ;
   ev->element->prefor = GlobalOrid ;

   strcpy( ev->element->auth , orig->element->auth ) ;
   ev->element->commid = orig->element->commid ;
   strcpy( ev->element->lddate , orig->element->lddate ) ;

   return orig;

}
/* ------------------------------------------------------------------ */





static int sudsAddAffiliationStruct( DBlist tree, char *sta, char *net)
{
   struct affiliationList *af = 0;
   
   do{
      af = (struct affiliationList *)dblNextTableInstance(af, tree, dbl_LIST_AFFILIATION);
      if(af && !strcmp(sta, af->element->sta) && !strcmp(net, af->element->net) ){
         return 1;
      }
   }while(af);

   af = (struct affiliationList *) dblCreateTableInstance(tree, dbl_LIST_AFFILIATION);
   if(!af)return 0;
   strcpy(af->element->sta, sta);
   strcpy(af->element->net, net);
   strcpy(af->element->lddate, tmListEpochTime( tmGetEpochTime(), 18 ) );
   return 1;
}
/* ------------------------------------------------------------------ */






static int sudsAddAssoc(DBlist tree, char* sta, int arid, char* iphase)
{
   struct siteList *si        = 0;
   struct siteList *siMatch   = 0;
   struct originList *orig      = 0;

   struct assocList *as;
   float delt, dist, azim, bazim;

   orig = (struct originList *) dblNextTableInstance(orig, tree, dbl_LIST_ORIGIN);
   if(!orig)
      return 1; /* need an orid to form a valid assoc entry. */



   as = (struct assocList *) dblCreateTableInstance(tree,dbl_LIST_ASSOC);
   if(!as)return 0;
   as->element->arid = arid;
   as->element->orid = orig->element->orid;
   strcpy(as->element->sta, sta);
   strcpy(as->element->phase, iphase);
   strcpy(as->element->lddate, orig->element->lddate);
   
   do{
      si = (struct siteList *) dblNextTableInstance(si, tree, dbl_LIST_SITE);
      if(si && !strcmp(sta, si->element->sta) ){
         siMatch = si;
         break;
      }
   }while(si);

   if(siMatch){
      dbDelaz((float*) &(si->element->lat), (float*) &(si->element->lat),
	      (float*) &(orig->element->lat), (float*) &(orig->element->lon),
	      &delt, &dist, &azim, &bazim);
      as->element->delta = delt;
      as->element->seaz  = azim;
      as->element->esaz  = bazim;
   }
   return 1;
}
/* ------------------------------------------------------------------ */





int InSiteList(scList *sc , DBlist tree )
{
   struct CSStree *Tree = (struct CSStree *) tree ;
   struct siteList *si = Tree->slHead ;

   while(si){
      if(!strncmp(sc->sc->sc_name.st_name, si->element->sta, 3) )
         if(sc->sc->st_lat == si->element->lat)
            if(sc->sc->st_long == si->element->lon)
              if(sc->sc->elev == si->element->elev * 1000)
                 if(sc->sc->effective == (int) julianToEpoch ( si->element->ondate , 1 ) ) {
                    return 1;
                 }
      si = si->next;
   }
   return 0;
}
/* -------------------------------------------------- */





static int sudsAddSiteStruct(SUDS *S,  DBlist tree, scList *sc)
{
   struct siteList *si;
   double Time;
   int year, month, day, hour, min;
   float second;
   
   UNUSED(S);

   if( !strlen(sc->sc->sc_name.st_name) )
      return 1; /* Without station name, no way to reference, so don't create. */
   if(sc->sc->st_lat == NODATA && sc->sc->st_long == NODATA && sc->sc->elev == NODATA)
      return 1;  /* Site not useful without these data. */
   if( InSiteList ( sc , tree ) )
      return 1; /* This site is already in the list */


   si = (struct siteList *) dblCreateTableInstance(tree,dbl_LIST_SITE);
   if(!si)return 0;

   strcpy(si->element->sta, sc->sc->sc_name.st_name);
   
   if ( sc->sc->st_lat != NODATA )
      si->element->lat   = sc->sc->st_lat;
   if ( sc->sc->st_long != NODATA )
      si->element->lon   = sc->sc->st_long;
   if ( sc->sc->elev != NODATA )
      si->element->elev  = sc->sc->elev / 1000.0;
   if(sc->sc->effective != NODATA && sc->sc->effective != NOTIME){
      Time = sc->sc->effective;
      tmDecodeEpochTime(Time, &year, &month, &day,&hour, &min, &second);
      si->element->ondate = year*1000 + yrday( month, day, isleap( year) );
   }
   strcpy(si->element->lddate, tmListEpochTime( tmGetEpochTime(), 18 ) );


   return 1;
}
/* ------------------------------------------------------------------ */



static int sudsAddArrivalStruct(SUDS *S, DBlist tree, feList *fe)
{
   struct arrivalList *ar;
   int year, month, day, hour, min;
   float second;
   UNUSED(S);
   
   if(! fe->fe->obs_phase) return 1;
   if( !strlen(fe->fe->fe_name.st_name) ){
      if(Verbose)printf("SUDS FEATURE with no station name --- Cannot associate!\n");
      return 1; /* Can't associate without a station name. */
   }

   if( fe->fe->fe_name.component == NOCHAR){
      if(Verbose)printf("SUDS FEATURE with no channel name --- Cannot associate!\n");
      return 1; /* Also need a component to associate. */
   }

   if( fe->fe->obs_phase == NODATA){
      if(Verbose)printf("SUDS FEATURE with no phase code --- Cannot associate!\n");
      return 1; /* Also need a phase code to associate. */
   }


   ar = (struct arrivalList *) dblCreateTableInstance(tree,dbl_LIST_ARRIVAL);
   
   strcpy(ar->element->sta,fe->fe->fe_name.st_name);
   ar->element->chan[0] = fe->fe->fe_name.component;
   ar->element->chan[1] = '\0';

   ar->element->time = fe->fe->time;
   strncpy(ar->element->iphase,Phasecode(fe->fe->obs_phase), 8);
   strcpy(ar->element->lddate, tmListEpochTime( tmGetEpochTime(), 18 ) );
   ar->element->arid = dblNextAvailableArid(tree);

   tmDecodeEpochTime(fe->fe->time, &year, &month, &day,&hour, &min, &second);
   ar->element->jdate = year*1000 + yrday( month, day, isleap( year) );
   strcpy(ar->element->lddate, tmListEpochTime( tmGetEpochTime(), 18 ) );
   strcpy(ar->element->auth, "Unknown");

   if(!sudsAddAssoc(tree, ar->element->sta, ar->element->arid, ar->element->iphase) )
      return 0;

   return 1;
}
/* ------------------------------------------------------------------ */




static int sudsAddSiteChanStruct(SUDS *S,  DBlist tree, scList *sc)
{
   struct sitechanList *si;
   double Time;
   int year, month, day, hour, min;
   float second;
   
   UNUSED(S);
   if(! strlen(sc->sc->sc_name.st_name) )
      return 1; /*Can't reference without station so don't create. */

   if( sc->sc->sc_name.component == NOCHAR)
      return 1; /*Can't reference without component so don't create. */

   si = (struct sitechanList *) dblCreateTableInstance(tree, dbl_LIST_SITECHAN);
   if(!si)return 0;

      
   strcpy(si->element->sta, sc->sc->sc_name.st_name);
   si->element->chan[0] = sc->sc->sc_name.component;
   si->element->chan[1] = '\0';

   if(sc->sc->channel != NODATA){
      if(dblChanidInUse(tree, sc->sc->channel) )
         dblReplaceChanid(tree, sc->sc->channel, dblNextAvailableChanid(tree) );

      si->element->chanid = sc->sc->channel;
   }

   if(sc->sc->azim != NODATA){
      si->element->hang = sc->sc->azim;
   }

   if(sc->sc->incid != NODATA){
      si->element->vang = sc->sc->incid;
   }
   
   if(sc->sc->effective != NODATA && sc->sc->effective != NOTIME){
      Time = sc->sc->effective;
      tmDecodeEpochTime(Time, &year, &month, &day,&hour, &min, &second);
      si->element->ondate = year*1000 + yrday( month, day, isleap( year) );
   }
   strcpy(si->element->lddate, tmListEpochTime( tmGetEpochTime(), 18 ) );
   si->element->edepth = 0; /* No SUDS field for this */


   return 1;
}
/* ------------------------------------------------------------------ */










static int sudsAddWfdiscStruct( DBlist tree, dtList *dt, struct originList *orig, 
                                const char *fname)
{
   struct wfdiscList *w;
   struct wftagList  *wt;
   int year, month, day, hour, min;
   float second;
   const char *SlashPtr = strrchr(fname, '/');
   
   if(SlashPtr)
      SlashPtr++;
   else
      SlashPtr = fname;
   
   w = (struct wfdiscList *) dblCreateTableInstance(tree,dbl_LIST_WFDISC);
   if(!w) return 0;
   
   w->element->nsamp    = dt->dt->length; 
   w->element->samprate = dt->dt->rate + dt->dt->rate_correct;
   w->element->time     = dt->dt->begintime + dt->dt->time_correct;
   w->element->wfid     = dblNextAvailableWfid(tree);
   w->element->endtime  = w->element->time + (w->element->nsamp) / w->element->samprate;
   w->element->calib    = CALIB_UNDEF;
   w->element->chan[0]  = dt->dt->dt_name.component;
   w->element->chan[1]  = '\0';
   strncpy(w->element->dfile, SlashPtr, 32);
   w->seis->r           = 0;
   w->seis->Cmplx       = 0;
   w->seis->i           = DataFromSuds(dt);
   if(!w->seis->i) return 0;
   
   strcpy(w->element->dattype , "t4" );
   strcpy(w->element->lddate, tmListEpochTime( tmGetEpochTime(), 18 ) );
   
   if( strlen(dt->dt->dt_name.st_name) )
      strcpy(w->element->sta, dt->dt->dt_name.st_name);
  
   tmDecodeEpochTime(w->element->time, &year, &month, &day,&hour, &min, &second);
   w->element->jdate = year*1000 + yrday( month, day, isleap( year) );
   if(dt->dt->dt_name.inst_type != NODATA)
      strncpy( w->element->instype, Instcode(dt->dt->dt_name.inst_type), 6 );   


   /* There is no way to link a descriptrace with an origin in SUDS
      So assume that every descriptrace in file is associated with
      the hopefully one origin! 
   */

   dblAddComment(tree, dbl_LIST_WFDISC, w, "Converted from SUDS format");


   if(orig){   /* Add a wftag struct linking origin to wfdisc... */
      wt = (struct wftagList *) dblCreateTableInstance(tree, dbl_LIST_WFTAG);
      strcpy(wt->element->tagname, "evid");
      wt->element->tagid = GlobalEvid;
      wt->element->wfid  = w->element->wfid;
      strcpy(wt->element->lddate, w->element->lddate);
   }
      

   if( strlen( dt->dt->dt_name.st_name ) && strlen( dt->dt->dt_name.network ) )
      if(!sudsAddAffiliationStruct(tree, dt->dt->dt_name.st_name, dt->dt->dt_name.network ) )
         return 0; 
   return 1;
}
/* ------------------------------------------------------------------ */








int sudsToCSSlist(const char* filename, const char *WorkSetName, int Replace, int Maxfiles, 
                  int verbose, double MaxPhysMem)
{
   DBlist tree;
   DBlist tree2;
   SUDS *S;
   dtList *dt;
   orList *orS;
   scList *staComp;
   feList *fe;
   double MaxPhysMemToUse;
   
   struct originList *orig = 0;   
   int NTraces           = 0;
   MaxWaveforms          = Maxfiles;
   Verbose               = verbose;
   MaxPhysMemToUse       = MaxPhysMem;
   
   
   if(!filename || ! strlen(filename)){
      printf("Invalid file name!\n");
      return 0;
   }


   if(!WorkSetName || !strlen(WorkSetName) ){
      printf("Invalid worksetname! Cannot add SUDS data to workset.\n");
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
   
   
   /* Make sure that there are not already too many traces to allow any further read... */
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



   tree = dblNewTree(); /* This is a temp tree which will be merged after filling. */


   /* No easy surefire way to link a suds origin to other suds data members, so make the
      assumption that there is only one origin in the suds file, and it relates to all
      the other data.
   */
   GlobalOrid  = dblNextAvailableOrid(tree);
   GlobalEvid  = dblNextAvailableEvid(tree);

   
   S = ReadSUDSfile((char*)filename, Verbose);
   if(!S){
      printf("Unable to fill SUDS structs from (%s).\n", filename);
      dblDeleteTree(tree);
      return 0;
   }


   /* Only allowing 1 origin from suds file since no easy way to associate origins with other tables.*/
   orS = S->orHead;
   if(orS){
      if(!(orig = sudsAddOriginStruct(S, tree, orS)) ){
         FreeSuds(S);
         dblDeleteTree(tree);
         return 0;
      }
   }

   
   dt = S->dtHead;
   while(dt){
      if(!sudsAddWfdiscStruct(tree, dt, orig, filename) ){
         FreeSuds(S);
         dblDeleteTree(tree);
         return 0;
      }
      NTraces++;
      if(NTraces + TracesInExistingTree >= MaxWaveforms){
         printf("There are now %d waveforms in memory. Remainder will be skipped.\n",
                NTraces + TracesInExistingTree);
         break;
      } 
      
    
      if(smFracPhysMemUsed() > MaxPhysMemToUse){
         printf("Waveforms in SeisMgr memory are using more than %5.2f%% of physical memory.\n",
                MaxPhysMemToUse * 100);
         printf("No more waveforms will be read.\n");
         printf("To utilize a higher percentage of physical memory use the MAXMEM option.\n");
         break;
      }
      
      dt = dt->next;
   }
   
   
   staComp = S->scHead;
   while(staComp){
      if(!sudsAddSiteStruct(S, tree, staComp) ){
         FreeSuds(S);
         dblDeleteTree(tree);
         return 0;
      }
      staComp = staComp->next;
   }


   staComp = S->scHead;
   while(staComp){
      if(!sudsAddSiteChanStruct(S, tree, staComp) ){
         FreeSuds(S);
         dblDeleteTree(tree);
         return 0;
      }
      staComp = staComp->next;
   }
   
   fe = S->feHead;
   while(fe){
      if(!sudsAddArrivalStruct(S, tree, fe) ){
         FreeSuds(S);
         dblDeleteTree(tree);
         return 0;
      }
      fe = fe->next;
   }
   
   FreeSuds(S);


   tree2 = dblMergeTrees(tree2, tree, FALSE );
   if(tree2 != tree) {
       dblDeleteTree(tree);
   }

   return NTraces;
}

