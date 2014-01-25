#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>


#ifndef FALSE
#       define FALSE 0
#       define TRUE !FALSE
#endif /* FALSE */

#define DBL_MOD_FILE TRUE
#include "cssListOps/cssListStrucs.h"
#include "timefuncs.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "cssListOps/dblErrors.h"


#define AF_LIST (struct affiliationList *)
#define AR_LIST (struct arrivalList *)
#define OR_LIST (struct originList *)
#define RE_LIST (struct remarkList *)
#define WF_LIST (struct wfdiscList *)
#define WT_LIST (struct wftagList *)
#define AS_LIST (struct assocList *)
#define EV_LIST (struct eventList *)
#define GR_LIST (struct gregionList *)
#define IN_LIST (struct instrumentList *)
#define OE_LIST (struct origerrList *)
#define SE_LIST (struct sensorList *)
#define SL_LIST (struct siteList *)
#define SC_LIST (struct sitechanList *)
#define UD_LIST (userData *)
#define SA_LIST (struct stassocList *)
#define SD_LIST (struct sacdataList *)



#define NULL_AF_LIST AF_LIST NULL  
#define NULL_AR_LIST AR_LIST NULL
#define NULL_OR_LIST OR_LIST NULL
#define NULL_RE_LIST RE_LIST NULL
#define NULL_WF_LIST WF_LIST NULL
#define NULL_WT_LIST WT_LIST NULL
#define NULL_AS_LIST AS_LIST NULL
#define NULL_EV_LIST EV_LIST NULL
#define NULL_GR_LIST GR_LIST NULL
#define NULL_IN_LIST IN_LIST NULL
#define NULL_OE_LIST OE_LIST NULL
#define NULL_SE_LIST SE_LIST NULL
#define NULL_SL_LIST SL_LIST NULL
#define NULL_SC_LIST SC_LIST NULL
#define NULL_UD_LIST UD_LIST NULL
#define NULL_SD_LIST SD_LIST NULL



static void dblPrintAFvalsToString(struct affiliationList *afPntr, int * attribute,
				   int nCols, char *tmpstr);
static void dblPrintARvalsToString(struct arrivalList *arPntr, int * attribute,
				   int nCols,char *tmpstr);
static void dblPrintASvalsToString(struct assocList *asPntr, int * attribute,
				   int nCols,char *tmpstr);
static void dblPrintEVvalsToString(struct eventList *evPntr, int * attribute,
				   int nCols,char *tmpstr);
static void dblPrintGRvalsToString(struct gregionList *grPntr, int * attribute,
				   int nCols, char *tmpstr);
static void dblPrintINvalsToString(struct instrumentList *inPntr, int * attribute,
				   int nCols, char *tmpstr);
static void dblPrintOEvalsToString(struct origerrList *oePntr, int * attribute,
				   int nCols, char *tmpstr);
static void dblPrintORvalsToString(struct originList *orPntr, int * attribute,
				   int nCols, char *tmpstr);
static void dblPrintREvalsToString(struct remarkList *rePntr, int * attribute,
				   int nCols, char *tmpstr);
static void dblPrintSEvalsToString(struct sensorList *sePntr, int * attribute,
				   int nCols, char *tmpstr);
static void dblPrintSLvalsToString(struct siteList *slPntr, int * attribute,
				   int nCols, char *tmpstr);
static void dblPrintSCvalsToString(struct sitechanList *scPntr, int * attribute,
				   int nCols, char *tmpstr);
static void dblPrintSAvalsToString(struct stassocList *saPntr, int * attribute,
				   int nCols, char *tmpstr);
static void dblPrintWFvalsToString(struct wfdiscList *wfPntr, int * attribute,
				   int nCols, char *tmpstr);
static void dblPrintWTvalsToString(struct wftagList *wtPntr, int * attribute,
				   int nCols, char *tmpstr);
static void dblPrintSDvalsToString(struct sacdataList *sdPntr, int * attribute,
				   int nCols, char *tmpstr);



static void dblPrintAFheaderToString(int *attribute, int *nCols, char *tmpstr);
static void dblPrintARheaderToString(int *attribute, int *nCols, char *tmpstr);
static void dblPrintASheaderToString(int *attribute, int *nCols, char *tmpstr);
static void dblPrintEVheaderToString(int *attribute, int *nCols, char *tmpstr);
static void dblPrintGRheaderToString(int *attribute, int *nCols, char *tmpstr);
static void dblPrintINheaderToString(int *attribute, int *nCols, char *tmpstr);
static void dblPrintOEheaderToString(int *attribute, int *nCols, char *tmpstr);
static void dblPrintORheaderToString(int *attribute, int *nCols, char *tmpstr);
//static void dblPrintREheaderToString(int *attribute, int *nCols, char *tmpstr);
static void dblPrintSEheaderToString(int *attribute, int *nCols, char *tmpstr);
static void dblPrintSLheaderToString(int *attribute, int *nCols, char *tmpstr);
static void dblPrintSCheaderToString(int *attribute, int *nCols, char *tmpstr);
static void dblPrintSAheaderToString(int *attribute, int *nCols, char *tmpstr);
static void dblPrintWFheaderToString(int *attribute, int *nCols, char *tmpstr);
static void dblPrintWTheaderToString(int *attribute, int *nCols, char *tmpstr);
static void dblPrintSDheaderToString(int *attribute, int *nCols, char *tmpstr);

static int Device = 0;   /* Output device for status messages (0 is stdout) */




/* Format and print one row from the specified table */
void dblPrintTableInstance(DBtable tableInstance,  dblObject specifier, FILE *unit)
{
   struct affiliationList *afPntr;
   struct arrivalList     *arPntr;
   struct assocList       *asPntr;
   struct eventList       *evPntr;
   struct gregionList     *grPntr;
   struct instrumentList  *inPntr;
   struct origerrList     *oePntr;
   struct originList      *orPntr;
   struct remarkList      *rePntr;
   struct sensorList      *sePntr;
   struct siteList        *slPntr;
   struct sitechanList    *scPntr;
   struct stassocList     *saPntr;
   struct wfdiscList      *wfPntr;
   struct wftagList       *wtPntr;
   struct sacdataList     *sdPntr;
   char timestr[25], timestr2[25];
   char fmt[600];
   struct UD *sdu;	

   switch (specifier){
      case dbl_LIST_AFFILIATION:{
         afPntr = AF_LIST tableInstance;
         strcpy(fmt,"(net = %8s) \n(sta = %6s) \n(lddate = %17s) \n(index = %5d)\n");
         if(fprintf(unit,fmt,
            afPntr->element->net, 
	    afPntr->element->sta,
            afPntr->element->lddate,
	    afPntr->index) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }

      case dbl_LIST_ARRIVAL:{
         arPntr = AR_LIST tableInstance;
         strcpy(fmt,"(sta = %6s) \n(time = %24s) \n(arid = %8d) \n(jdate = %8d) ");
         strcat(fmt,
		"\n(stassid = %8d) \n(chanid = %8d) \n(chan = %8s) \n(iphase = %8s) ");
	 strcat(fmt,
		"\n(stype = %2s) \n(deltim = %8f) \n(azimuth = %8f) \n(delaz = %8f) ");
	 strcat(fmt,
		"\n(slow = %8f) \n(delslo = %8f) \n(ema = %8f) \n(rect = %8f) ");
	 strcat(fmt,
		"\n(amp = %8f) \n(per = %8f) \n(logat = %8f) \n(clip = %2s) ");
	 strcat(fmt,"\n(fm = %3s) \n(snr = %8f)  \n(qual = %2s) \n(auth = %15s) ");
	 strcat(fmt,"\n(commid = %8d) \n(lddate = %17s)\n");
         if(fprintf(unit,fmt,
 	    arPntr->element->sta,
	    tmListEpochTime(arPntr->element->time, 14),
	    arPntr->element->arid,
	    arPntr->element->jdate,
	    arPntr->element->stassid,
	    arPntr->element->chanid,
	    arPntr->element->chan,
	    arPntr->element->iphase,
	    arPntr->element->stype,
	    arPntr->element->deltim,
	    arPntr->element->azimuth,
	    arPntr->element->delaz,
	    arPntr->element->slow,
	    arPntr->element->delslo,
	    arPntr->element->ema,
	    arPntr->element->rect,
	    arPntr->element->amp,
	    arPntr->element->per,
	    arPntr->element->logat,
	    arPntr->element->clip,
	    arPntr->element->fm,
	    arPntr->element->snr,
	    arPntr->element->qual,
	    arPntr->element->auth,
	    arPntr->element->commid,
	    arPntr->element->lddate) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }
 

      case dbl_LIST_ASSOC:{
         asPntr = AS_LIST tableInstance;
         strcpy(fmt,"(arid = %8d) \n(orid = %8d) \n(sta = %7s) \n(phase = %9s) ");
         strcat(fmt,
		"\n(belief = %8f) \n(delta = %8f) \n(seaz = %8f) \n(esaz = %8f) ");
	 strcat(fmt,
		"\n(timeres = %8f) \n(timedef = %2s) \n(azres = %8f) \n(azdef = %2s) ");
	 strcat(fmt,
		"\n(slores = %8f) \n(slodef = %2s) \n(emares = %8f) \n(wgt = %8f) ");
	 strcat(fmt,
		"\n(vmodel = %16s) \n(commid = %8d) \n(lddate = %18s) \n");
         if(fprintf(unit,fmt,
 	    asPntr->element->arid,
 	    asPntr->element->orid,
 	    asPntr->element->sta,
 	    asPntr->element->phase,
 	    asPntr->element->belief,
 	    asPntr->element->delta,
 	    asPntr->element->seaz,
 	    asPntr->element->esaz,
 	    asPntr->element->timeres,
 	    asPntr->element->timedef,
 	    asPntr->element->azres,
 	    asPntr->element->azdef,
 	    asPntr->element->slores,
 	    asPntr->element->slodef,
 	    asPntr->element->emares,
 	    asPntr->element->wgt,
 	    asPntr->element->vmodel,
 	    asPntr->element->commid,
 	    asPntr->element->lddate) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }

      case dbl_LIST_EVENT:{
         evPntr = EV_LIST tableInstance;
         strcpy(fmt,"(evid = %8d) \n(evname = %16s) \n(prefor = %8d) \n(auth = %16s) ");
         strcat(fmt,"\n(commid = %8d) \n(lddate = %18s) \n");
         if(fprintf(unit,fmt,
 	    evPntr->element->evid,
 	    evPntr->element->evname,
 	    evPntr->element->prefor,
 	    evPntr->element->auth,
 	    evPntr->element->commid,
 	    evPntr->element->lddate) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }

      case dbl_LIST_GREGION:{
         grPntr = GR_LIST tableInstance;
         strcpy(fmt,"(grn = %8d) \n(grname = %41s) \n(lddate = %18s) \n ");
         if(fprintf(unit,fmt,
 	    grPntr->element->grn,
 	    grPntr->element->grname,
 	    grPntr->element->lddate) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }


      case dbl_LIST_INSTRUMENT:{
         inPntr = IN_LIST tableInstance;
         strcpy(fmt,"(inid = %8d) \n(insname = %51s) \n(instype = %7s) ");
         strcat(fmt,"\n(band = %2s) \n(digital = %2s) \n(samprate = %8f) ");
         strcat(fmt,"\n(ncalib = %8f) \n(ncalper = %8f) \n(dir = %65s) ");
         strcat(fmt,"\n(dfile = %33s) \n(rsptype = %7s) \n(lddate = %18s) \n");
         if(fprintf(unit,fmt,
 	    inPntr->element->inid,
 	    inPntr->element->insname,
 	    inPntr->element->instype,
 	    inPntr->element->band,
 	    inPntr->element->digital,
 	    inPntr->element->samprate,
 	    inPntr->element->ncalib,
 	    inPntr->element->ncalper,
 	    inPntr->element->dir,
 	    inPntr->element->dfile,
 	    inPntr->element->rsptype,
 	    inPntr->element->lddate) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }


      case dbl_LIST_ORIGERR:{
         oePntr = OE_LIST tableInstance;
         strcpy(fmt,"(orid = %8d) \n(sxx = %8f) \n(syy = %8f) ");
         strcat(fmt,"\n(szz = %8f) \n(stt = %8f) \n(sxy = %8f) ");
         strcat(fmt,"\n(sxz = %8f) \n(syz = %8f) \n(stx = %8f) ");
         strcat(fmt,"\n(sty = %8f) \n(stz = %8f) \n(sdobs = %8f) ");
         strcat(fmt,"\n(smajax = %8f) \n(sminax = %8f) \n(strike = %8f) ");
         strcat(fmt,"\n(sdepth = %8f) \n(stime = %8f) ");
         strcat(fmt,"\n(conf = %8f) \n(commid = %8d) \n(lddate = %18s) \n");
         if(fprintf(unit,fmt,
 	    oePntr->element->orid,
 	    oePntr->element->sxx,
 	    oePntr->element->syy,
 	    oePntr->element->szz,
 	    oePntr->element->stt,
 	    oePntr->element->sxy,
 	    oePntr->element->sxz,
 	    oePntr->element->syz,
 	    oePntr->element->stx,
 	    oePntr->element->sty,
 	    oePntr->element->stz,
 	    oePntr->element->sdobs,
 	    oePntr->element->smajax,
 	    oePntr->element->sminax,
 	    oePntr->element->strike,
 	    oePntr->element->sdepth,
 	    oePntr->element->stime,
 	    oePntr->element->conf,
 	    oePntr->element->commid,
 	    oePntr->element->lddate) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }


      case dbl_LIST_ORIGIN:{
         orPntr = OR_LIST tableInstance;
         strcpy(fmt,"(lat = %8f) \n(lon = %8f) \n(depth = %8f) ");
         strcat(fmt,"\n(time = %24s) \n(orid = %8d) \n(evid = %8d) ");
         strcat(fmt,"\n(jdate = %8d) \n(nass = %8d) \n(ndef = %8d) ");
         strcat(fmt,"\n(ndp = %8d) \n(grn = %8d) \n(srn = %8d) ");
         strcat(fmt,"\n(etype = %8s) \n(depdp = %8f) \n(dtype = %2s) ");
         strcat(fmt,"\n(mb = %8f) \n(mbid = %8d) \n(ms = %8f) ");
         strcat(fmt,"\n(msid = %8d) \n(ml = %8f) \n(mlid = %8d) ");
         strcat(fmt,"\n(algorithm = %16s) ");
         strcat(fmt,"\n(auth = %16s) \n(commid = %8d) \n(lddate = %18s) \n");
         if(fprintf(unit,fmt,
 	    orPntr->element->lat,
 	    orPntr->element->lon,
 	    orPntr->element->depth,
 	    tmListEpochTime(orPntr->element->time, 14),
 	    orPntr->element->orid,
 	    orPntr->element->evid,
 	    orPntr->element->jdate,
 	    orPntr->element->nass,
 	    orPntr->element->ndef,
 	    orPntr->element->ndp,
 	    orPntr->element->grn,
 	    orPntr->element->srn,
 	    orPntr->element->etype,
 	    orPntr->element->depdp,
 	    orPntr->element->dtype,
 	    orPntr->element->mb,
 	    orPntr->element->mbid,
 	    orPntr->element->ms,
 	    orPntr->element->msid,
 	    orPntr->element->ml,
 	    orPntr->element->mlid,
 	    orPntr->element->algorithm,
 	    orPntr->element->auth,
 	    orPntr->element->commid,
 	    orPntr->element->lddate) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }


      case dbl_LIST_REMARK:{
         rePntr = RE_LIST tableInstance;
         strcpy(fmt,"(commid = %8d) \n(lineno = %8d) \n(remark = %s) ");
         strcat(fmt,"\n(lddate = %18s) \n");
         if(fprintf(unit,fmt,
 	    rePntr->element->commid,
 	    rePntr->element->lineno,
 	    rePntr->element->remark,
 	    rePntr->element->lddate) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }


      case dbl_LIST_SENSOR:{
         sePntr = SE_LIST tableInstance;
         strcpy(fmt,"(sta = %7s) \n(chan = %9s) \n(time = %24s) ");
         strcat(fmt,"\n(endtime = %24s) \n(inid = %8d) \n(chanid = %8d) ");
         strcat(fmt,"\n(jdate = %8d) \n(calratio = %8f) \n(calper = %8f) ");
         strcat(fmt,"\n(tshift = %8f) \n(instant = %2s) \n(lddate = %18s) \n");
         if(fprintf(unit,fmt,
 	    sePntr->element->sta,
 	    sePntr->element->chan,
 	    tmListEpochTime(sePntr->element->time, 14 ),
 	    tmListEpochTime(sePntr->element->endtime, 14 ),
 	    sePntr->element->inid,
 	    sePntr->element->chanid,
 	    sePntr->element->jdate,
 	    sePntr->element->calratio,
 	    sePntr->element->calper,
 	    sePntr->element->tshift,
 	    sePntr->element->instant,
 	    sePntr->element->lddate) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }


      case dbl_LIST_SITE:{
         slPntr = SL_LIST tableInstance;
         strcpy(fmt,"(sta = %7s) \n(ondate = %8d) \n(offdate = %8d) ");
         strcat(fmt,"\n(lat = %8f) \n(lon = %8f) \n(elev = %8f) ");
         strcat(fmt,"\n(staname = %51s) \n(statype = %5s) \n(refsta = %7s) ");
         strcat(fmt,"\n(dnorth = %8f) \n(deast = %8f) \n(lddate = %18s) \n");
         if(fprintf(unit,fmt,
 	    slPntr->element->sta,
 	    slPntr->element->ondate,
 	    slPntr->element->offdate,
 	    slPntr->element->lat,
 	    slPntr->element->lon,
 	    slPntr->element->elev,
 	    slPntr->element->staname,
 	    slPntr->element->statype,
 	    slPntr->element->refsta,
 	    slPntr->element->dnorth,
 	    slPntr->element->deast,
 	    slPntr->element->lddate) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }


      case dbl_LIST_SITECHAN:{
         scPntr = SC_LIST tableInstance;
         strcpy(fmt,"(sta = %7s) \n(chan = %9s) \n(ondate = %8d) ");
         strcat(fmt,"\n(chanid = %8d) \n(offdate = %8d) \n(ctype = %5s) ");
         strcat(fmt,"\n(edepth = %8f) \n(hang = %8f) ");
         strcat(fmt,"\n(vang = %8f) \n(descrip = %51s) \n(lddate = %18s) \n");
         if(fprintf(unit,fmt,
 	    scPntr->element->sta,
 	    scPntr->element->chan,
 	    scPntr->element->ondate,
 	    scPntr->element->chanid,
 	    scPntr->element->offdate,
 	    scPntr->element->ctype,
 	    scPntr->element->edepth,
 	    scPntr->element->hang,
 	    scPntr->element->vang,
 	    scPntr->element->descrip,
 	    scPntr->element->lddate) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }


      case dbl_LIST_STASSOC:{
         saPntr = SA_LIST tableInstance;
         strcpy(fmt,"(stassid = %8d) \n(sta = %7s) \n(etype = %8s) ");
         strcat(fmt,"\n(location = %33s) \n(dist = %8f) \n(azimuth = %8f) ");
         strcat(fmt,"\n(lat = %8f) \n(lon = %8f) \n(depth = %8f) ");
         strcat(fmt,"\n(time = %24s) \n(imb = %8f) ");
         strcat(fmt,"\n(ims = %8f) \n(iml = %8f) ");
         strcat(fmt,"\n(auth = %16s) \n(commid = %8d) \n(lddate = %18s) \n");
         if(fprintf(unit,fmt,
 	    saPntr->element->stassid,
 	    saPntr->element->sta,
 	    saPntr->element->etype,
 	    saPntr->element->location,
 	    saPntr->element->dist,
 	    saPntr->element->azimuth,
 	    saPntr->element->lat,
 	    saPntr->element->lon,
 	    saPntr->element->depth,
 	    tmListEpochTime(saPntr->element->time, 14),
 	    saPntr->element->imb,
 	    saPntr->element->ims,
 	    saPntr->element->iml,
 	    saPntr->element->auth,
 	    saPntr->element->commid,
 	    saPntr->element->lddate) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }


      case dbl_LIST_WFDISC:{
         wfPntr = WF_LIST tableInstance;
         strcpy(fmt,"(sta = %8s) \n(chan = %9s) \n(time = %24s) ");
         strcat(fmt,"\n(wfid = %8d) \n(chanid = %8d) \n(jdate = %8d) ");
         strcat(fmt,"\n(endtime = %24s) \n(nsamp = %8d) \n(samprate = %8f) ");
         strcat(fmt,"\n(calib = %8f) \n(calper = %8f) \n(instype = %7s) ");
         strcat(fmt,"\n(segtype = %2s) \n(dattype = %3s) \n(clip = %2s) ");
         strcat(fmt,"\n(dir = %65s) \n(dfile = %33s) ");
         strcat(fmt,"\n(foff = %8d) \n(commid = %8d) \n(lddate = %18s) \n");
         strcpy(timestr, tmListEpochTime(wfPntr->element->time, 14) );
	 strcpy(timestr2 , tmListEpochTime(wfPntr->element->endtime, 14) );
         if(fprintf(unit,fmt,
 	    wfPntr->element->sta,
 	    wfPntr->element->chan,
 	    timestr,
 	    wfPntr->element->wfid,
 	    wfPntr->element->chanid,
 	    wfPntr->element->jdate,
 	    timestr2,
 	    wfPntr->element->nsamp,
 	    wfPntr->element->samprate,
 	    wfPntr->element->calib,
 	    wfPntr->element->calper,
 	    wfPntr->element->instype,
 	    wfPntr->element->segtype,
 	    wfPntr->element->dattype,
 	    wfPntr->element->clip,
 	    wfPntr->element->dir,
 	    wfPntr->element->dfile,
 	    wfPntr->element->foff,
 	    wfPntr->element->commid,
 	    wfPntr->element->lddate) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }


      case dbl_LIST_WFTAG:{
         wtPntr = WT_LIST tableInstance;
         strcpy(fmt,"(tagname = %s) \n(tagid = %8d) \n(wfid = %d) ");
         strcat(fmt,"\n(lddate = %18s) \n");
         if(fprintf(unit,fmt,
 	    wtPntr->element->tagname,
 	    wtPntr->element->tagid,
 	    wtPntr->element->wfid,
 	    wtPntr->element->lddate) == EOF){
            dblSetError(1, "ERROR: Error writing file instance in dblPrintTableInstance.\n");
            return;
         }
         break;
      }


      case dbl_LIST_SACDATA:{
         sdPntr = SD_LIST tableInstance;
         sdu    = &(sdPntr->element->userdata);
         fprintf(unit,"USER0: %-8s --- %f\n", sdu->label[0], sdu->value[0]);
         fprintf(unit,"USER1: %-8s --- %f\n", sdu->label[1], sdu->value[1]);
         fprintf(unit,"USER2: %-8s --- %f\n", sdu->label[2], sdu->value[2]);
         fprintf(unit,"USER3:          --- %f\n",sdu->value[3]);
         fprintf(unit,"USER4:          --- %f\n",sdu->value[4]);
         fprintf(unit,"USER5:          --- %f\n",sdu->value[5]);
         fprintf(unit,"USER6:          --- %f\n",sdu->value[6]);
         fprintf(unit,"USER7:          --- %f\n",sdu->value[7]);
         fprintf(unit,"USER8:          --- %f\n",sdu->value[8]);
         fprintf(unit,"USER9:          --- %f\n",sdu->value[9]);
         fprintf(unit,"synthflag --- %d\n",sdPntr->element->synthflag);
         fprintf(unit,"lpspol    --- %d\n",sdPntr->element->lpspol);
         fprintf(unit,"iztype    --- %d\n",sdPntr->element->iztype);
         fprintf(unit,"idep      --- %d\n",sdPntr->element->idep);
         fprintf(unit,"iftype    --- %d\n",sdPntr->element->iftype);
         fprintf(unit,"wfid      --- %d\n",sdPntr->element->wfid);
         break;
      }
   case dbl_DBLIST:
   case dbl_LIST_USERDATA:
   AFFI_CASES
   ARRIVAL_CASES
   ASSOC_CASES
   EVENT_CASES
   GR_CASES
   INS_CASES
   NET_CASES
   ORIGERR_CASES
   ORIGIN_CASES
   REMA_CASES
   SENS_CASES
   SITE_CASES
   SITECHAN_CASES
   SR_CASES
   STASSOC_CASES
   WFDISC_CASES
   WFTAG_CASES
   SACD_CASES
       break;
   }   

}
/*----------------------------------------------------------------------*/







/* Dump selected columns from all the rows of the table specified by StrucType. */
void dblDumpTable(DBlist list, dblObject StrucType, FILE *unit,...)
{

	struct affiliationList * afPntr;
	struct arrivalList     * arPntr;
	struct assocList       * asPntr;
	struct eventList       * evPntr;
	struct gregionList     * grPntr;
	struct instrumentList  * inPntr;
	struct origerrList     * oePntr;
	struct originList      * orPntr;
	struct remarkList      * rePntr;
	struct sensorList      * sePntr;
	struct siteList        * slPntr;
	struct sitechanList    * scPntr;
	struct stassocList     * saPntr;
	struct wfdiscList      * wfPntr;	
	struct wftagList       * wtPntr;	
	struct sacdataList     * sdPntr;	

	struct CSStree *tree;
        va_list ap;
        int arg;
	int nCols = 0;
	int attribute[100];
	char tmpstr[400];


/* First take care of getting the variable args using stdarg macros. */
        va_start(ap,unit);
        while( (arg = va_arg(ap,int)) ){
	    attribute[nCols++] = arg;
	}   
        va_end(ap);



		
	tree = (struct CSStree *) list;
        if(!tree) return;
	switch (StrucType){
	   case dbl_LIST_AFFILIATION:{
	      afPntr = tree->afHead;
	      if(!afPntr)break;
	      dblPrintAFheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(afPntr){
		 dblPrintAFvalsToString(afPntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 afPntr = afPntr->next;
	      }
              if(fprintf(unit,"\n") == EOF)break;
	      return;
	   }
	
	   case dbl_LIST_ARRIVAL:{
	      arPntr = tree->arHead;
	      if(!arPntr)break;
	      dblPrintARheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(arPntr){
		 dblPrintARvalsToString(arPntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 arPntr = arPntr->next;
	      }
              if(fprintf(unit,"\n") == EOF)break;
	      return;
	   }
	
	   case dbl_LIST_ASSOC:{
	      asPntr = tree->asHead;
	      if(!asPntr)break;
	      dblPrintASheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(asPntr){
		 dblPrintASvalsToString(asPntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 asPntr = asPntr->next;
	      }
	      return;
	   }
	
	   case dbl_LIST_EVENT:{
	      evPntr = tree->evHead;
	      if(!evPntr)break;
	      dblPrintEVheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(evPntr){
		 dblPrintEVvalsToString(evPntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 evPntr = evPntr->next;
	      }
	      return;
	   }
	
	   case dbl_LIST_GREGION:{
	      grPntr = tree->grHead;
	      if(!grPntr)break;
	      dblPrintGRheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(grPntr){
		 dblPrintGRvalsToString(grPntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 grPntr = grPntr->next;
	      }
	      return;
	   }
	
	   case dbl_LIST_INSTRUMENT:{
	      inPntr = tree->inHead;
	      if(!inPntr)break;
	      dblPrintINheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(inPntr){
		 dblPrintINvalsToString(inPntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 inPntr = inPntr->next;
	      }
	      return;
	   }
	
	   case dbl_LIST_ORIGERR:{
	      oePntr = tree->oeHead;
	      if(!oePntr)break;
	      dblPrintOEheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(oePntr){
		 dblPrintOEvalsToString(oePntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 oePntr = oePntr->next;
	      }
	      return;
	   }
	
	   case dbl_LIST_ORIGIN:{
	      orPntr = tree->orHead;
	      if(!orPntr)break;
	      dblPrintORheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(orPntr){
		 dblPrintORvalsToString(orPntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 orPntr = orPntr->next;
	      }
	      return;
	   }
	
	   case dbl_LIST_REMARK:{
	      rePntr = tree->reHead;
	      if(!rePntr)break;
	      dblPrintSEheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(rePntr){
		 dblPrintREvalsToString(rePntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 rePntr = rePntr->next;
	      }
	      return;
	   }
	
	   case dbl_LIST_SENSOR:{
	      sePntr = tree->seHead;
	      if(!sePntr)break;
	      dblPrintSEheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(sePntr){
		 dblPrintSEvalsToString(sePntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 sePntr = sePntr->next;
	      }
	      return;
	   }
	
	   case dbl_LIST_SITE:{
	      slPntr = tree->slHead;
	      if(!slPntr)break;
	      dblPrintSLheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(slPntr){
		 dblPrintSLvalsToString(slPntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 slPntr = slPntr->next;
	      }
	      return;
	   }
	
	   case dbl_LIST_SITECHAN:{
	      scPntr = tree->scHead;
	      if(!scPntr)break;
	      dblPrintSCheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(scPntr){
		 dblPrintSCvalsToString(scPntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 scPntr = scPntr->next;
	      }
	      return;
	   }
	
	   case dbl_LIST_STASSOC:{
	      saPntr = tree->saHead;
	      if(!saPntr)break;
	      dblPrintSAheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(saPntr){
		 dblPrintSAvalsToString(saPntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 saPntr = saPntr->next;
	      }
	      return;
	   }
	
	   case dbl_LIST_WFDISC:{
	      wfPntr = tree->wfHead;
	      if(!wfPntr)break;
	      dblPrintWFheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(wfPntr){
		 dblPrintWFvalsToString(wfPntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 wfPntr = wfPntr->next;
	      }
	      return;
	   }
	
	   case dbl_LIST_WFTAG:{
	      wtPntr = tree->wtHead;
	      if(!wtPntr)break;
	      dblPrintWTheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(wtPntr){
		 dblPrintWTvalsToString(wtPntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 wtPntr = wtPntr->next;
	      }
	      return;
	   }
	
	   case dbl_LIST_SACDATA:{
	      sdPntr = tree->sdHead;
	      if(!sdPntr)break;
	      dblPrintSDheaderToString(attribute,&nCols,tmpstr);
	      if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
	      while(sdPntr){
		 dblPrintSDvalsToString(sdPntr, attribute,nCols,tmpstr);
		 if(fprintf(unit,"%s\n",tmpstr) == EOF) break;
		 sdPntr = sdPntr->next;
	      }
	      return;
	   }
	
	   default: {
	      return;
	   }
	}
        dblSetError(1, "ERROR: Error writing file instance in dblDumpTable.\n");
        return;


}
/*----------------------------------------------------------------------*/






/* Format and print selected attributes from the arrivals into a string. */
static void dblPrintARvalsToString(struct arrivalList *arPntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_ARRIV_AMP:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(arPntr, dbl_ARRIV_AMP) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_ARID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(arPntr, dbl_ARRIV_ARID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_AUTH:{
           sprintf(buffer,"%16s", (char *) dblGetTableObject(arPntr, dbl_ARRIV_AUTH) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_AZIMUTH:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(arPntr, dbl_ARRIV_AZIMUTH) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_CHAN:{
           sprintf(buffer,"%9s", (char *) dblGetTableObject(arPntr, dbl_ARRIV_CHAN) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_CLIP:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(arPntr, dbl_ARRIV_CLIP) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_COMMID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(arPntr, dbl_ARRIV_COMMID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_CHANID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(arPntr, dbl_ARRIV_CHANID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_DELAZ:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(arPntr, dbl_ARRIV_DELAZ) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_DELSLO:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(arPntr, dbl_ARRIV_DELSLO) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_EMA:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(arPntr, dbl_ARRIV_EMA) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_DELTIM:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(arPntr, dbl_ARRIV_DELTIM) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_FM:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(arPntr, dbl_ARRIV_FM) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_IPHASE:{
           sprintf(buffer,"%9s", (char *) dblGetTableObject(arPntr, dbl_ARRIV_IPHASE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_JDATE:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(arPntr, dbl_ARRIV_JDATE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_LOGAT:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(arPntr, dbl_ARRIV_LOGAT) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_PER:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(arPntr, dbl_ARRIV_PER) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_QUAL:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(arPntr, dbl_ARRIV_QUAL) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_RECT:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(arPntr, dbl_ARRIV_RECT) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_SLOW:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(arPntr, dbl_ARRIV_SLOW) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_SNR:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(arPntr, dbl_ARRIV_SNR) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_STA:{
           sprintf(buffer,"%7s", (char *) dblGetTableObject(arPntr, dbl_ARRIV_STA) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_STASSID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(arPntr, dbl_ARRIV_STASSID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_STYPE:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(arPntr, dbl_ARRIV_STYPE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_TIME:{
           sprintf(buffer,"%24s", 
		   tmListEpochTime( *(double *) dblGetTableObject(arPntr, dbl_ARRIV_TIME), 14) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_LDDATE:{
           sprintf(buffer,"%18s", (char *) dblGetTableObject(arPntr, dbl_ARRIV_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/






/* Format and print selected attribute descriptions from the arrival table. */
static void dblPrintARheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numARvals = 26;
  char buffer[30];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numARvals;j++)attribute[j] = dbl_ARRIV_AMP +j;
    *nCols = numARvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_ARRIV_AMP:{
	   sprintf(buffer,"%16s","amp");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_ARID:{
           sprintf(buffer,"%12s","arid");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_AUTH:{
	   sprintf(buffer,"%16s","auth");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_AZIMUTH:{
	   sprintf(buffer,"%16s","azimuth");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_CHAN:{
	   sprintf(buffer,"%9s","chan");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_CLIP:{
	   sprintf(buffer,"%8s","clip");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_COMMID:{
	   sprintf(buffer,"%12s","commid");
           strcat(tmpstr,buffer);
           break;
        }
     case dbl_ARRIV_CHANID:{
	   sprintf(buffer,"%12s","chanid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_DELAZ:{
	   sprintf(buffer,"%16s","delaz");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_DELSLO:{
	   sprintf(buffer,"%16s","delslo");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_EMA:{
	   sprintf(buffer,"%16s","ema");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_DELTIM:{
	   sprintf(buffer,"%16s","deltim");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_FM:{
	   sprintf(buffer,"%8s","fm");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_IPHASE:{
	   sprintf(buffer,"%9s","iphase");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_JDATE:{
	   sprintf(buffer,"%12s","jdate");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_LOGAT:{
	   sprintf(buffer,"%16s","logat");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_PER:{
	   sprintf(buffer,"%16s","per");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_QUAL:{
	   sprintf(buffer,"%8s","qual");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_RECT:{
	   sprintf(buffer,"%16s","rect");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_SLOW:{
	   sprintf(buffer,"%16s","slow");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_SNR:{
	   sprintf(buffer,"%16s","snr");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_STA:{
           sprintf(buffer,"%7s","sta");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_STASSID:{
	   sprintf(buffer,"%12s","stassid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_STYPE:{
	   sprintf(buffer,"%8s","stype");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_TIME:{
	   sprintf(buffer,"%24s","time");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ARRIV_LDDATE:{
	   sprintf(buffer,"%18s","lddate");
           strcat(tmpstr,buffer);
           break;
        }

     }
  }

}
/*----------------------------------------------------------------------*/







static void dblPrintAFvalsToString(struct affiliationList *afPntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_AFFI_NET:{
           sprintf(buffer,"%9s", (char *) dblGetTableObject(afPntr, dbl_AFFI_NET) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_AFFI_STA:{
           sprintf(buffer,"%7s", (char *) dblGetTableObject(afPntr, dbl_AFFI_STA) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_AFFI_LDDATE:{
           sprintf(buffer,"%18s", (char *) dblGetTableObject(afPntr, dbl_AFFI_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/










static void dblPrintAFheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numAFvals = 3;
  char buffer[30];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numAFvals;j++)attribute[j] = dbl_AFFI_NET +j;
    *nCols = numAFvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_AFFI_NET:{
	   sprintf(buffer,"%9s","net");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_AFFI_STA:{
           sprintf(buffer,"%7s","sta");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_AFFI_LDDATE:{
	   sprintf(buffer,"%18s","lddate");
           strcat(tmpstr,buffer);
           break;
        }

     }
  }

}
/*----------------------------------------------------------------------*/





static void dblPrintASheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numASvals = 19;
  char buffer[30];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numASvals;j++)attribute[j] = dbl_ASSOC_ARID +j;
    *nCols = numASvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_ASSOC_ARID:{
	   sprintf(buffer,"%12s","arid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_AZDEF:{
           sprintf(buffer,"%12s","orid");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_AZRES:{
	   sprintf(buffer,"%7s","sta");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_BELIEF:{
	   sprintf(buffer,"%9s","phase");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_COMMID:{
	   sprintf(buffer,"%16s","belief");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_DELTA:{
	   sprintf(buffer,"%16s","delta");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_EMARES:{
	   sprintf(buffer,"%16s","emares");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_ESAZ:{
	   sprintf(buffer,"%16s","esaz");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_ORID:{
	   sprintf(buffer,"%12s","orid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_PHASE:{
	   sprintf(buffer,"%9s","phase");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_SEAZ:{
	   sprintf(buffer,"%16s","seaz");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_SLODEF:{
	   sprintf(buffer,"%8s","slodef");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_SLORES:{
	   sprintf(buffer,"%16s","slores");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_STA:{
	   sprintf(buffer,"%7s","sta");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_TIMEDEF:{
	   sprintf(buffer,"%8s","timedef");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_TIMERES:{
	   sprintf(buffer,"%16s","timeres");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_VMODEL:{
	   sprintf(buffer,"%16s","vmodel");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_WGT:{
	   sprintf(buffer,"%8s","wgt");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_LDDATE:{
	   sprintf(buffer,"%18s","lddate");
           strcat(tmpstr,buffer);
           break;
        }

     }
  }

}
/*----------------------------------------------------------------------*/







static void dblPrintASvalsToString(struct assocList *asPntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_ASSOC_ARID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(asPntr, dbl_ASSOC_ARID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_AZDEF:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(asPntr, dbl_ASSOC_AZDEF) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_AZRES:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(asPntr, dbl_ASSOC_AZRES) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_BELIEF:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(asPntr, dbl_ASSOC_BELIEF) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_COMMID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(asPntr, dbl_ASSOC_COMMID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_DELTA:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(asPntr, dbl_ASSOC_DELTA) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_EMARES:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(asPntr, dbl_ASSOC_EMARES) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_ESAZ:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(asPntr, dbl_ASSOC_ESAZ) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_ORID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(asPntr, dbl_ASSOC_ORID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_PHASE:{
           sprintf(buffer,"%9s", (char *) dblGetTableObject(asPntr, dbl_ASSOC_PHASE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_SEAZ:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(asPntr, dbl_ASSOC_SEAZ) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_SLODEF:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(asPntr, dbl_ASSOC_SLODEF) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_SLORES:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(asPntr, dbl_ASSOC_SLORES) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_STA:{
           sprintf(buffer,"%7s", (char *) dblGetTableObject(asPntr, dbl_ASSOC_STA) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_TIMEDEF:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(asPntr, dbl_ASSOC_TIMEDEF) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_TIMERES:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(asPntr, dbl_ASSOC_TIMERES) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_VMODEL:{
           sprintf(buffer,"%16s", (char *) dblGetTableObject(asPntr, dbl_ASSOC_VMODEL) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_WGT:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(asPntr, dbl_ASSOC_WGT) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ASSOC_LDDATE:{
           sprintf(buffer,"%18s", (char *) dblGetTableObject(asPntr, dbl_ASSOC_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/




static void dblPrintEVheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numEVvals = 6;
  char buffer[30];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numEVvals;j++)attribute[j] = dbl_EVENT_AUTH +j;
    *nCols = numEVvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_EVENT_AUTH:{
	   sprintf(buffer,"%16s","auth");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_EVENT_COMMID:{
           sprintf(buffer,"%12s","commid");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_EVENT_EVID:{
	   sprintf(buffer,"%12s","evid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_EVENT_EVNAME:{
	   sprintf(buffer,"%16s","evname");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_EVENT_PREFOR:{
	   sprintf(buffer,"%12s","prefor");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_EVENT_LDDATE:{
	   sprintf(buffer,"%18s","lddate");
           strcat(tmpstr,buffer);
           break;
        }

     }
  }

}
/*----------------------------------------------------------------------*/







static void dblPrintEVvalsToString(struct eventList *evPntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_EVENT_AUTH:{
           sprintf(buffer,"%16s", (char *) dblGetTableObject(evPntr, dbl_EVENT_AUTH) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_EVENT_COMMID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(evPntr, dbl_EVENT_COMMID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_EVENT_EVID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(evPntr, dbl_EVENT_EVID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_EVENT_EVNAME:{
           sprintf(buffer,"%16s", (char *) dblGetTableObject(evPntr, dbl_EVENT_EVNAME) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_EVENT_PREFOR:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(evPntr, dbl_EVENT_PREFOR) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_EVENT_LDDATE:{
           sprintf(buffer,"%18s", (char *) dblGetTableObject(evPntr, dbl_EVENT_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/






static void dblPrintGRheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numGRvals = 3;
  char buffer[30];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numGRvals;j++)attribute[j] = dbl_GR_GRN +j;
    *nCols = numGRvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_GR_GRN:{
	   sprintf(buffer,"%12s","grn");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_GR_GRNAME:{
           sprintf(buffer,"%41s","grname");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_GR_LDDATE:{
	   sprintf(buffer,"%18s","lddate");
           strcat(tmpstr,buffer);
           break;
        }
     }
  }

}
/*----------------------------------------------------------------------*/







static void dblPrintGRvalsToString(struct gregionList *grPntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_GR_GRN:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(grPntr, dbl_GR_GRN) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_GR_GRNAME:{
           sprintf(buffer,"%41s", (char *) dblGetTableObject(grPntr, dbl_GR_GRNAME) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_GR_LDDATE:{
           sprintf(buffer,"%18s", (char *) dblGetTableObject(grPntr, dbl_GR_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/







static void dblPrintINheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numINvals = 12;
  char buffer[30];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numINvals;j++)attribute[j] = dbl_INS_BAND +j;
    *nCols = numINvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_INS_BAND:{
	   sprintf(buffer,"%8s","band");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_DFILE:{
           sprintf(buffer,"%33s","dfile");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_DIGITAL:{
	   sprintf(buffer,"%8s","digital");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_DIR:{
	   sprintf(buffer,"%65s","dir");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_INID:{
	   sprintf(buffer,"%12s","inid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_INSNAME:{
	   sprintf(buffer,"%51s","insname");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_INSTYPE:{
	   sprintf(buffer,"%7s","instype");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_NCALIB:{
           sprintf(buffer,"%16s","ncalib");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_NCALPER:{
	   sprintf(buffer,"%16s","ncalper");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_RSPTYPE:{
	   sprintf(buffer,"%7s","rsptype");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_SAMPRATE:{
	   sprintf(buffer,"%16s","samprate");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_LDDATE:{
	   sprintf(buffer,"%18s","lddate");
           strcat(tmpstr,buffer);
           break;
        }

     }
  }

}
/*----------------------------------------------------------------------*/







static void dblPrintINvalsToString(struct instrumentList *inPntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_INS_BAND:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(inPntr, dbl_INS_BAND) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_DFILE:{
           sprintf(buffer,"%33s", (char *) dblGetTableObject(inPntr, dbl_INS_DFILE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_DIGITAL:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(inPntr, dbl_INS_DIGITAL) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_DIR:{
           sprintf(buffer,"%65s", (char *) dblGetTableObject(inPntr, dbl_INS_DIR) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_INID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(inPntr, dbl_INS_INID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_INSNAME:{
           sprintf(buffer,"%51s", (char *) dblGetTableObject(inPntr, dbl_INS_INSNAME) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_INSTYPE:{
           sprintf(buffer,"%7s", (char *) dblGetTableObject(inPntr, dbl_INS_INSTYPE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_NCALIB:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(inPntr, dbl_INS_NCALIB) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_NCALPER:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(inPntr, dbl_INS_NCALPER) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_RSPTYPE:{
           sprintf(buffer,"%7s", (char *) dblGetTableObject(inPntr, dbl_INS_RSPTYPE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_SAMPRATE:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(inPntr, dbl_INS_SAMPRATE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_INS_LDDATE:{
           sprintf(buffer,"%18s", (char *) dblGetTableObject(inPntr, dbl_INS_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/







static void dblPrintOEheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numOEvals = 20;
  char buffer[30];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numOEvals;j++)attribute[j] = dbl_ORIGE_COMMID +j;
    *nCols = numOEvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_ORIGE_COMMID:{
	   sprintf(buffer,"%12s","commid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_CONF:{
           sprintf(buffer,"%16s","conf");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_ORID:{
	   sprintf(buffer,"%12s","orid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SDEPTH:{
	   sprintf(buffer,"%16s","sdepth");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SDOBS:{
	   sprintf(buffer,"%16s","sdobs");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SMAJAX:{
	   sprintf(buffer,"%16s","smajax");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SMINAX:{
	   sprintf(buffer,"%16s","sminax");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_STIME:{
	   sprintf(buffer,"%16s","stime");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_STRIKE:{
	   sprintf(buffer,"%16s","strike");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_STX:{
	   sprintf(buffer,"%16s","stx");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_STY:{
	   sprintf(buffer,"%16s","sty");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_STZ:{
	   sprintf(buffer,"%16s","stz");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SXX:{
	   sprintf(buffer,"%16s","sxx");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SXY:{
	   sprintf(buffer,"%16s","sxy");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SXZ:{
	   sprintf(buffer,"%16s","sxz");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SYY:{
	   sprintf(buffer,"%16s","syy");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SYZ:{
	   sprintf(buffer,"%16s","syz");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_STT:{
	   sprintf(buffer,"%16s","stt");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SZZ:{
	   sprintf(buffer,"%16s","szz");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_LDDATE:{
	   sprintf(buffer,"%18s","lddate");
           strcat(tmpstr,buffer);
           break;
        }

     }
  }

}
/*----------------------------------------------------------------------*/






static void dblPrintOEvalsToString(struct origerrList *oePntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_ORIGE_COMMID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(oePntr, dbl_ORIGE_COMMID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_CONF:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_CONF) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_ORID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(oePntr, dbl_ORIGE_ORID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SDEPTH:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_SDEPTH) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SDOBS:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_SDOBS) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SMAJAX:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_SMAJAX) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SMINAX:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_SMINAX) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_STIME:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_STIME) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_STRIKE:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_STRIKE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_STX:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_STX) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_STY:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_STY) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_STZ:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_STZ) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SXX:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_SXX) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SXY:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_SXY) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SXZ:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_SXZ) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SYY:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_SYY) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SYZ:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_SYZ) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_STT:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_STT) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_SZZ:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(oePntr, dbl_ORIGE_SZZ) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGE_LDDATE:{
           sprintf(buffer,"%18s", (char *) dblGetTableObject(oePntr, dbl_ORIGE_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/








static void dblPrintORheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numORvals = 25;
  char buffer[30];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numORvals;j++)attribute[j] = dbl_ORIGI_ALGORITHM +j;
    *nCols = numORvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_ORIGI_ALGORITHM:{
	   sprintf(buffer,"%16s","algorithm");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_AUTH:{
           sprintf(buffer,"%16s","auth");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_COMMID:{
	   sprintf(buffer,"%12s","commid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_DEPDP:{
	   sprintf(buffer,"%16s","depdp");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_DEPTH:{
	   sprintf(buffer,"%16s","depth");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_DTYPE:{
	   sprintf(buffer,"%8s","dtype");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_ETYPE:{
	   sprintf(buffer,"%8s","etype");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_EVID:{
	   sprintf(buffer,"%12s","evid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_GRN:{
	   sprintf(buffer,"%12s","grn");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_JDATE:{
	   sprintf(buffer,"%12s","jdate");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_LAT:{
	   sprintf(buffer,"%16s","lat");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_LON:{
	   sprintf(buffer,"%16s","lon");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_MB:{
	   sprintf(buffer,"%16s","mb");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_MBID:{
	   sprintf(buffer,"%12s","mbid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_ML:{
	   sprintf(buffer,"%16s","ml");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_MLID:{
	   sprintf(buffer,"%12s","mlid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_MS:{
	   sprintf(buffer,"%16s","ms");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_MSID:{
	   sprintf(buffer,"%12s","msid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_NASS:{
	   sprintf(buffer,"%12s","nass");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_NDEF:{
	   sprintf(buffer,"%12s","ndef");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_NDP:{
	   sprintf(buffer,"%12s","ndp");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_ORID:{
	   sprintf(buffer,"%12s","orid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_SRN:{
	   sprintf(buffer,"%12s","srn");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_TIME:{
	   sprintf(buffer,"%24s","time");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_LDDATE:{
	   sprintf(buffer,"%18s","lddate");
           strcat(tmpstr,buffer);
           break;
        }

     }
  }

}
/*----------------------------------------------------------------------*/







static void dblPrintORvalsToString(struct originList *orPntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_ORIGI_ALGORITHM:{
           sprintf(buffer,"%16s", (char *) dblGetTableObject(orPntr, dbl_ORIGI_ALGORITHM) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_AUTH:{
           sprintf(buffer,"%16s", (char *) dblGetTableObject(orPntr, dbl_ORIGI_AUTH) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_COMMID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(orPntr, dbl_ORIGI_COMMID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_DEPDP:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(orPntr, dbl_ORIGI_DEPDP) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_DEPTH:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(orPntr, dbl_ORIGI_DEPTH) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_DTYPE:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(orPntr, dbl_ORIGI_DTYPE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_ETYPE:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(orPntr, dbl_ORIGI_ETYPE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_EVID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(orPntr, dbl_ORIGI_EVID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_GRN:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(orPntr, dbl_ORIGI_GRN) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_JDATE:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(orPntr, dbl_ORIGI_JDATE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_LAT:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(orPntr, dbl_ORIGI_LAT) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_LON:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(orPntr, dbl_ORIGI_LON) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_MB:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(orPntr, dbl_ORIGI_MB) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_MBID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(orPntr, dbl_ORIGI_MBID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_ML:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(orPntr, dbl_ORIGI_ML) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_MLID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(orPntr, dbl_ORIGI_MLID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_MS:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(orPntr, dbl_ORIGI_MS) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_MSID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(orPntr, dbl_ORIGI_MSID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_NASS:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(orPntr, dbl_ORIGI_NASS) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_NDEF:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(orPntr, dbl_ORIGI_NDEF) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_NDP:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(orPntr, dbl_ORIGI_NDP) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_ORID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(orPntr, dbl_ORIGI_ORID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_SRN:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(orPntr, dbl_ORIGI_SRN) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_TIME:{
           sprintf(buffer,"%24s", tmListEpochTime( *(double *) 
		   dblGetTableObject(orPntr, dbl_ORIGI_TIME), 14) ) ;
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_ORIGI_LDDATE:{
           sprintf(buffer,"%18s", (char *) dblGetTableObject(orPntr, dbl_ORIGI_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/












/* static void dblPrintREheaderToString(int *attribute, int *nCols, char *tmpstr) */
/* { */
/*   int j; */
/*   int numREvals = 25; */
/*   char buffer[30]; */
/*   *tmpstr = 0; */

/*   if(*nCols < 1){ */
/*     for(j=0;j<numREvals;j++)attribute[j] = dbl_REMA_COMMID +j; */
/*     *nCols = numREvals; */
/*   } */

/*   for(j=0;j<*nCols;j++){ */
/*      switch (attribute[j]){ */
/*         case dbl_REMA_COMMID:{ */
/* 	   sprintf(buffer,"%8s","commid"); */
/*            strcat(tmpstr,buffer); */
/*            break; */
/*         } */
/*         case dbl_REMA_LINENO:{ */
/*            sprintf(buffer,"%8s","lineno"); */
/* 	   strcat(tmpstr,buffer); */
/*            break; */
/*         } */
/*         case dbl_REMA_REMARK:{ */
/* 	   sprintf(buffer,"%80s","remark"); */
/*            strcat(tmpstr,buffer); */
/*            break; */
/*         } */
/*         case dbl_REMA_LDDATE:{ */
/* 	   sprintf(buffer,"%17s","lddate"); */
/*            strcat(tmpstr,buffer); */
/*            break; */
/*         } */

/*      } */
/*   } */

/* } */
/*----------------------------------------------------------------------*/








static void dblPrintREvalsToString(struct remarkList *rePntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_REMA_COMMID:{
           sprintf(buffer,"%8d", *(int *) dblGetTableObject(rePntr, dbl_REMA_COMMID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_REMA_LINENO:{
           sprintf(buffer,"%8d", *(int *) dblGetTableObject(rePntr, dbl_REMA_LINENO) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_REMA_REMARK:{
           sprintf(buffer,"%80s", (char *) dblGetTableObject(rePntr, dbl_REMA_REMARK) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_REMA_LDDATE:{
           sprintf(buffer,"%17s", (char *) dblGetTableObject(rePntr, dbl_REMA_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/









static void dblPrintSEheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numSEvals = 12;
  char buffer[30];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numSEvals;j++)attribute[j] = dbl_SENS_CALPER +j;
    *nCols = numSEvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_SENS_CALPER:{
	   sprintf(buffer,"%16s","calper");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_CALRATIO:{
           sprintf(buffer,"%16s","calratio");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_CHAN:{
	   sprintf(buffer,"%9s","chan");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_CHANID:{
	   sprintf(buffer,"%12s","chanid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_ENDTIME:{
	   sprintf(buffer,"%24s","endtime");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_INID:{
	   sprintf(buffer,"%12s","inid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_INSTANT:{
	   sprintf(buffer,"%8s","instant");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_JDATE:{
	   sprintf(buffer,"%12s","jdate");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_STA:{
	   sprintf(buffer,"%7s","sta");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_TIME:{
	   sprintf(buffer,"%24s","time");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_TSHIFT:{
	   sprintf(buffer,"%16s","tshift");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_LDDATE:{
	   sprintf(buffer,"%18s","lddate");
           strcat(tmpstr,buffer);
           break;
        }

     }
  }

}
/*----------------------------------------------------------------------*/







static void dblPrintSEvalsToString(struct sensorList *sePntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_SENS_CALPER:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(sePntr, dbl_SENS_CALPER) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_CALRATIO:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(sePntr, dbl_SENS_CALRATIO) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_CHAN:{
           sprintf(buffer,"%9s", (char *) dblGetTableObject(sePntr, dbl_SENS_CHAN) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_CHANID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(sePntr, dbl_SENS_CHANID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_ENDTIME:{
           sprintf(buffer,"%24s", tmListEpochTime( *(double *) 
		   dblGetTableObject(sePntr, dbl_SENS_ENDTIME), 14) ) ;
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_INID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(sePntr, dbl_SENS_INID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_INSTANT:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(sePntr, dbl_SENS_INSTANT) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_JDATE:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(sePntr, dbl_SENS_JDATE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_STA:{
           sprintf(buffer,"%7s", (char *) dblGetTableObject(sePntr, dbl_SENS_STA) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_TIME:{
           sprintf(buffer,"%24s", tmListEpochTime( *(double *) 
		   dblGetTableObject(sePntr, dbl_SENS_TIME), 14) ) ;
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SENS_LDDATE:{
           sprintf(buffer,"%18s", (char *) dblGetTableObject(sePntr, dbl_SENS_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/







static void dblPrintSLheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numSEvals = 12;
  char buffer[30];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numSEvals;j++)attribute[j] = dbl_SITE_DEAST +j;
    *nCols = numSEvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_SITE_DEAST:{
	   sprintf(buffer,"%16s","deast");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_DNORTH:{
           sprintf(buffer,"%16s","dnorth");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_ELEV:{
	   sprintf(buffer,"%16s","elev");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_LAT:{
	   sprintf(buffer,"%16s","lat");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_LON:{
	   sprintf(buffer,"%16s","lon");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_OFFDATE:{
	   sprintf(buffer,"%12s","offdate");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_ONDATE:{
	   sprintf(buffer,"%12s","ondate");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_REFSTA:{
	   sprintf(buffer,"%7s","refsta");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_STA:{
	   sprintf(buffer,"%7s","sta");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_STANAME:{
	   sprintf(buffer,"%51s","staname");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_STATYPE:{
	   sprintf(buffer,"%8s","statype");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_LDDATE:{
	   sprintf(buffer,"%18s","lddate");
           strcat(tmpstr,buffer);
           break;
        }

     }
  }

}
/*----------------------------------------------------------------------*/








static void dblPrintSLvalsToString(struct siteList *slPntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_SITE_DEAST:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(slPntr, dbl_SITE_DEAST) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_DNORTH:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(slPntr, dbl_SITE_DNORTH) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_ELEV:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(slPntr, dbl_SITE_ELEV) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_LAT:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(slPntr, dbl_SITE_LAT) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_LON:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(slPntr, dbl_SITE_LON) ) ;
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_OFFDATE:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(slPntr, dbl_SITE_OFFDATE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_ONDATE:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(slPntr, dbl_SITE_ONDATE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_REFSTA:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(slPntr, dbl_SITE_REFSTA) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_STA:{
           sprintf(buffer,"%7s", (char *) dblGetTableObject(slPntr, dbl_SITE_STA) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_STANAME:{
           sprintf(buffer,"%51s", (char *) dblGetTableObject(slPntr, dbl_SITE_STANAME) ) ;
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITE_LDDATE:{
           sprintf(buffer,"%18s", (char *) dblGetTableObject(slPntr, dbl_SITE_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/






static void dblPrintSCheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numSCvals = 11;
  char buffer[30];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numSCvals;j++)attribute[j] = dbl_SITEC_CHAN +j;
    *nCols = numSCvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_SITEC_CHAN:{
	   sprintf(buffer,"%9s","chan");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_CHANID:{
           sprintf(buffer,"%12s","chanid");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_CTYPE:{
	   sprintf(buffer,"%8s","ctype");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_DESCRIP:{
	   sprintf(buffer,"%51s","descrip");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_EDEPTH:{
	   sprintf(buffer,"%16s","edepth");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_HANG:{
	   sprintf(buffer,"%16s","hang");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_OFFDATE:{
	   sprintf(buffer,"%12s","offdate");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_ONDATE:{
	   sprintf(buffer,"%12s","ondate");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_STA:{
	   sprintf(buffer,"%7s","sta");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_VANG:{
	   sprintf(buffer,"%16s","vang");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_LDDATE:{
	   sprintf(buffer,"%18s","lddate");
           strcat(tmpstr,buffer);
           break;
        }

     }
  }

}
/*----------------------------------------------------------------------*/






static void dblPrintSCvalsToString(struct sitechanList *scPntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_SITEC_CHAN:{
           sprintf(buffer,"%9s", (char *) dblGetTableObject(scPntr, dbl_SITEC_CHAN) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_CHANID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(scPntr, dbl_SITEC_CHANID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_CTYPE:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(scPntr, dbl_SITEC_CTYPE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_DESCRIP:{
           sprintf(buffer,"%51s", (char *) dblGetTableObject(scPntr, dbl_SITEC_DESCRIP) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_EDEPTH:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(scPntr, dbl_SITEC_EDEPTH) ) ;
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_HANG:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(scPntr, dbl_SITEC_HANG) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_OFFDATE:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(scPntr, dbl_SITEC_OFFDATE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_ONDATE:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(scPntr, dbl_SITEC_ONDATE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_STA:{
           sprintf(buffer,"%7s", (char *) dblGetTableObject(scPntr, dbl_SITEC_STA) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_VANG:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(scPntr, dbl_SITEC_VANG) ) ;
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SITEC_LDDATE:{
           sprintf(buffer,"%18s", (char *) dblGetTableObject(scPntr, dbl_SITEC_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/





static void dblPrintSAheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numSAvals = 16;
  char buffer[30];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numSAvals;j++)attribute[j] = dbl_STASS_AUTH +j;
    *nCols = numSAvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_STASS_AUTH:{
	   sprintf(buffer,"%16s","auth");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_AZIMUTH:{
           sprintf(buffer,"%16s","azimuth");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_COMMID:{
	   sprintf(buffer,"%12s","commid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_DEPTH:{
	   sprintf(buffer,"%16s","depth");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_DIST:{
	   sprintf(buffer,"%16s","dist");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_ETYPE:{
	   sprintf(buffer,"%8s","etype");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_IMB:{
	   sprintf(buffer,"%16s","imb");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_IML:{
	   sprintf(buffer,"%16s","iml");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_IMS:{
	   sprintf(buffer,"%16s","ims");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_LAT:{
	   sprintf(buffer,"%16s","lat");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_LOCATION:{
	   sprintf(buffer,"%33s","location");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_LON:{
	   sprintf(buffer,"%16s","lon");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_STA:{
	   sprintf(buffer,"%7s","sta");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_STASSID:{
	   sprintf(buffer,"%12s","stassid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_TIME:{
	   sprintf(buffer,"%24s","time");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_LDDATE:{
	   sprintf(buffer,"%18s","lddate");
           strcat(tmpstr,buffer);
           break;
        }

     }
  }

}
/*----------------------------------------------------------------------*/






static void dblPrintSAvalsToString(struct stassocList *saPntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_STASS_AUTH:{
           sprintf(buffer,"%16s", (char *) dblGetTableObject(saPntr, dbl_STASS_AUTH) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_AZIMUTH:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(saPntr, dbl_STASS_AZIMUTH) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_COMMID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(saPntr, dbl_STASS_COMMID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_DEPTH:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(saPntr, dbl_STASS_DEPTH) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_DIST:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(saPntr, dbl_STASS_DIST) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_ETYPE:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(saPntr, dbl_STASS_ETYPE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_IMB:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(saPntr, dbl_STASS_IMB) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_IML:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(saPntr, dbl_STASS_IML) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_IMS:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(saPntr, dbl_STASS_IMS) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_LAT:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(saPntr, dbl_STASS_LAT) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_LOCATION:{
           sprintf(buffer,"%33s", (char *) dblGetTableObject(saPntr, dbl_STASS_LOCATION) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_LON:{
           sprintf(buffer,"%16f", *(double *) dblGetTableObject(saPntr, dbl_STASS_LON) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_STA:{
           sprintf(buffer,"%7s", (char *) dblGetTableObject(saPntr, dbl_STASS_STA) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_STASSID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(saPntr, dbl_STASS_STASSID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_TIME:{
	   sprintf(buffer,"%24s", tmListEpochTime( *(double *)
		   dblGetTableObject(saPntr, dbl_STASS_TIME), 14) ) ;
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_STASS_LDDATE:{
           sprintf(buffer,"%18s", (char *) dblGetTableObject(saPntr, dbl_STASS_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/
            





static void dblPrintWFheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numWFvals = 20;
  char buffer[100];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numWFvals;j++)attribute[j] = dbl_WFDIS_CALIB +j;
    *nCols = numWFvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_WFDIS_CALIB:{
	   sprintf(buffer,"%12s","calib");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_CALPER:{
           sprintf(buffer,"%12s","calper");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_CHAN:{
	   sprintf(buffer,"%9s","chan");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_CHANID:{
	   sprintf(buffer,"%12s","chanid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_CLIP:{
	   sprintf(buffer,"%8s","clip");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_COMMID:{
	   sprintf(buffer,"%12s","commid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_DATATYPE:{
	   sprintf(buffer,"%8s","dattype");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_DFILE:{
	   sprintf(buffer,"%33s","dfile");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_DIR:{
	   sprintf(buffer,"%65s","dir");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_ENDTIME:{
	   sprintf(buffer,"%24s","endtime");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_FOFF:{
	   sprintf(buffer,"%12s","foff");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_INSTYPE:{
	   sprintf(buffer,"%7s","instype");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_JDATE:{
	   sprintf(buffer,"%12s","jdate");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_NSAMP:{
	   sprintf(buffer,"%12s","nsamp");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_SAMPRATE:{
	   sprintf(buffer,"%12s","samprate");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_SEGTYPE:{
	   sprintf(buffer,"%8s","segtype");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_STA:{
	   sprintf(buffer,"%8s","sta");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_TIME:{
	   sprintf(buffer,"%24s","time");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_WFID:{
	   sprintf(buffer,"%12s","wfid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_LDDATE:{
	   sprintf(buffer,"%18s","lddate");
           strcat(tmpstr,buffer);
           break;
        }

     }
  }

}
/*----------------------------------------------------------------------*/







static void dblPrintWFvalsToString(struct wfdiscList *wfPntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_WFDIS_CALIB:{
           sprintf(buffer,"%12f", *(float *) dblGetTableObject(wfPntr, dbl_WFDIS_CALIB) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_CALPER:{
           sprintf(buffer,"%12f", *(float *) dblGetTableObject(wfPntr, dbl_WFDIS_CALPER) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_CHAN:{
           sprintf(buffer,"%9s", (char *) dblGetTableObject(wfPntr, dbl_WFDIS_CHAN) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_CHANID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(wfPntr, dbl_WFDIS_CHANID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_CLIP:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(wfPntr, dbl_WFDIS_CLIP) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_COMMID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(wfPntr, dbl_WFDIS_COMMID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_DATATYPE:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(wfPntr, dbl_WFDIS_DATATYPE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_DFILE:{
           sprintf(buffer,"%33s", (char *) dblGetTableObject(wfPntr, dbl_WFDIS_DFILE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_DIR:{
           sprintf(buffer,"%65s", (char *) dblGetTableObject(wfPntr, dbl_WFDIS_DIR) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_ENDTIME:{
	   sprintf(buffer,"%24s", tmListEpochTime( *(double *)
		   dblGetTableObject(wfPntr, dbl_WFDIS_ENDTIME), 14) ) ;
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_FOFF:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(wfPntr, dbl_WFDIS_FOFF) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_INSTYPE:{
           sprintf(buffer,"%7s", (char *) dblGetTableObject(wfPntr, dbl_WFDIS_INSTYPE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_JDATE:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(wfPntr, dbl_WFDIS_JDATE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_NSAMP:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(wfPntr, dbl_WFDIS_NSAMP) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_SAMPRATE:{
           sprintf(buffer,"%12f", *(float *) dblGetTableObject(wfPntr, dbl_WFDIS_SAMPRATE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_SEGTYPE:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(wfPntr, dbl_WFDIS_SEGTYPE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_STA:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(wfPntr, dbl_WFDIS_STA) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_TIME:{
	   sprintf(buffer,"%24s", tmListEpochTime( *(double *)
		   dblGetTableObject(wfPntr, dbl_WFDIS_TIME), 14) ) ;
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_WFID:{
           sprintf(buffer,"%12d", *(int *) dblGetTableObject(wfPntr, dbl_WFDIS_WFID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFDIS_LDDATE:{
           sprintf(buffer,"%18s", (char *) dblGetTableObject(wfPntr, dbl_WFDIS_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/









static void dblPrintWTheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numWTvals = 20;
  char buffer[100];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numWTvals;j++)attribute[j] = dbl_WFTAG_TAGNAME +j;
    *nCols = numWTvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_WFTAG_TAGNAME:{
	   sprintf(buffer,"%8s","tagname");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFTAG_TAGID:{
           sprintf(buffer,"%8s","tagid");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFTAG_WFID:{
	   sprintf(buffer,"%8s","wfid");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFTAG_LDDATE:{
	   sprintf(buffer,"%17s","lddate");
           strcat(tmpstr,buffer);
           break;
        }

     }
  }

}
/*----------------------------------------------------------------------*/









static void dblPrintWTvalsToString(struct wftagList *wtPntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_WFTAG_TAGNAME:{
           sprintf(buffer,"%8s", (char *) dblGetTableObject(wtPntr, dbl_WFTAG_TAGNAME) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFTAG_TAGID:{
           sprintf(buffer,"%8d", *(int *) dblGetTableObject(wtPntr, dbl_WFTAG_TAGID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFTAG_WFID:{
           sprintf(buffer,"%8d", *(int *) dblGetTableObject(wtPntr, dbl_WFTAG_WFID) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_WFTAG_LDDATE:{
           sprintf(buffer,"%18s", (char *) dblGetTableObject(wtPntr, dbl_WFTAG_LDDATE) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/








static void dblPrintSDheaderToString(int *attribute, int *nCols, char *tmpstr)
{
  int j;
  int numSDvals = 6;
  char buffer[30];
  *tmpstr = 0;

  if(*nCols < 1){
    for(j=0;j<numSDvals;j++)attribute[j] = dbl_SACD_SYNTHFLG +j;
    *nCols = numSDvals;
  }

  for(j=0;j<*nCols;j++){
     switch (attribute[j]){
        case dbl_SACD_SYNTHFLG:{
	   sprintf(buffer,"%-8s","synthflag");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SACD_LPSPOL:{
           sprintf(buffer,"%-8s","lpspol");
	   strcat(tmpstr,buffer);
           break;
        }
        case dbl_SACD_IZTYPE:{
	   sprintf(buffer,"%-8s","iztype");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SACD_IDEP:{
	   sprintf(buffer,"%-8s","idep");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SACD_IFTYPE:{
	   sprintf(buffer,"%-8s","iftype");
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SACD_WFID:{
	   sprintf(buffer,"%-8s","wfid");
           strcat(tmpstr,buffer);
           break;
        }
     }
  }

}
/*----------------------------------------------------------------------*/







static void dblPrintSDvalsToString(struct sacdataList *sdPntr, int * attribute,
				   int nCols, char *tmpstr)
{
   char buffer[100];
   int j;

   *tmpstr = 0;
   for(j=0;j<nCols;j++){
     switch (attribute[j]){
        case dbl_SACD_SYNTHFLG:{
           sprintf(buffer,"%8d", *(int *) dblGetTableObject(sdPntr, dbl_SACD_SYNTHFLG) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SACD_LPSPOL:{
           sprintf(buffer,"%8d", *(int *) dblGetTableObject(sdPntr, dbl_SACD_LPSPOL) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SACD_IZTYPE:{
           sprintf(buffer,"%8d", *(int *) dblGetTableObject(sdPntr, dbl_SACD_IZTYPE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SACD_IDEP:{
           sprintf(buffer,"%8d", *(int *) dblGetTableObject(sdPntr, dbl_SACD_IDEP) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SACD_IFTYPE:{
           sprintf(buffer,"%8d", *(int *) dblGetTableObject(sdPntr, dbl_SACD_IFTYPE) );
           strcat(tmpstr,buffer);
           break;
        }
        case dbl_SACD_WFID:{
           sprintf(buffer,"%8d", *(int *) dblGetTableObject(sdPntr, dbl_SACD_WFID) );
           strcat(tmpstr,buffer);
           break;
        }
     }
   }
}
/*----------------------------------------------------------------------*/











void dbPrintToDevice(char *string)
{
  switch(Device){
  case 0:
    printf("%s",string);
    return;
  default:
    fprintf(stderr,"Unrecognised output device!\n");
    exit(-1);
  }
}
/*----------------------------------------------------------------------*/
