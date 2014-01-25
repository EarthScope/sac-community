#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <ctype.h>



#define DBL_MOD_FILE TRUE
#define NULL_AFFILIATION
#define NULL_ARRIVAL
#define NULL_ASSOC
#define NULL_EVENT
#define NULL_GREGION
#define NULL_INSTRUMENT
#define NULL_ORIGIN_ERROR
#define NULL_ORIGIN
#define NULL_REMARK
#define NULL_SENSOR
#define NULL_SITE
#define NULL_SITECHAN
#define NULL_STASSOC
#define NULL_WFDISC
#define NULL_WFTAG
#define NULL_SAC_DATA
#include "cssListOps/cssListStrucs.h"
#include "timefuncs.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "dbselect/dbDefaults.h"
#include "smMemory/smMemory.h"
#include "cssListOps/dblErrors.h"

#include "debug.h"
/* Each table instance in the tree has associated with it a unique index number
   (not necessarily sequential) by which it may be identified. This could be
   useful as a means of accessing sets of data whose indices have been stored.
   However, at this time (8/4/1997) no such functions exist. The index number
   is assigned when the table instance is created by the function 
   dblNextIndex below.
*/

/* +++++++++++++++ PRIVATE FUNCTION PROTOTYPES FOR THIS FILE ++++++++++++++ */
//static void dblDeleteTrace(struct trace *trce );
static struct trace *dblCopyTraceData(struct trace *old, int nsamp);

void * smGetDefaultTree ( void ) ;
/* ++++++++++++++ END FUNCTION PROTOTYPES FOR THIS FILE ++++++++++++ */




static int dblIndex = 0;

static int dblNextIndex(void)
{
   dblIndex++;
   return dblIndex;
}  
/* --------------------------------------------------------------- */







/* Utility routine to free a trace struct associated with a wfdiscList struct */
/* static void dblDeleteTrace(struct trace *trce ) */
/* { */
/*    if(!trce) return; */
/*    if(trce->r) smFree(trce->r); */
/*    if(trce->i) smFree(trce->i); */
/*    smFree(trce); */
/*    return; */
/* } */
/* --------------------------------------------------------------- */





/* This function is called while copying a tree.  It copies the trace struct */
/* associated with a wfdiscList struct and all associated data.  */
/* A pointer to the new trace struct is returned. */
static struct trace *dblCopyTraceData(struct trace *old, int nsamp)
{
   struct trace *fresh;
   int i;

   if(!old){
      dblSetError(1, "ERROR: Null input trace struct in dblCopyTraceData");
      return (struct trace *)NULL;
   }   
   fresh = (struct trace *) smMalloc( sizeof(struct trace) );
   fresh->Cmplx = old->Cmplx;
   fresh->r = (float *) smMalloc(nsamp *sizeof(float) );
   if(fresh->r == (float *)NULL){
      dblSetError(1, "ERROR: Could not allocate memory in dblCopyTraceData.\n");
      return (struct trace *)NULL;
   }
   for(i=0;i<nsamp;i++) *(fresh->r++) = *(old->r++);

   if(fresh->Cmplx){
      fresh->i = (float *) smMalloc(nsamp *sizeof(float) );
      if(fresh->i == (float *)NULL){
         dblSetError(1, "ERROR: Could not allocate memory in dblCopyTraceData.\n");
         return (struct trace *)NULL;
      }
      for(i=0;i<nsamp;i++) *(fresh->i++) = *(old->i++);
   }
   return fresh;
}
/* --------------------------------------------------------------- */




/* Given and index number, return a pointer to the data from the indicated
   waveform in the default workset. */

struct trace * dblGetTrace ( int index ) 
{
	int idx ;
	DBlist tree = smGetDefaultTree();
	struct CSStree *Tree = (struct CSStree *) tree ;
	struct wfdiscList *wfL = Tree->wfHead ;
    
    idx = 0;
    while(wfL && idx < index) {
        idx++;
        wfL = wfL->next;
    }

	if ( wfL )
	    return wfL->seis ;
	else
	    return NULL ;
}

/* --------------------------------------------------------------- */




/* Called to create and initialize a new CSS tree. */
DBlist dblNewTree(void)
{
	struct CSStree *newT;
	newT = (struct CSStree *) smCalloc(1,sizeof(struct CSStree));
	if(newT == (struct CSStree *) NULL){
           dblSetError(1, "ERROR: Could not allocate new tree in dblNewTree.\n");
	   return (void *) NULL;
	}
	newT->afHead = NULL_AF_LIST;
	newT->afTail = NULL_AF_LIST;
	newT->arHead = NULL_AR_LIST;
	newT->arTail = NULL_AR_LIST;
	newT->orHead = NULL_OR_LIST;
	newT->orTail = NULL_OR_LIST;
	newT->reHead = NULL_RE_LIST;
	newT->reTail = NULL_RE_LIST;
	newT->wfHead = NULL_WF_LIST;
	newT->wfTail = NULL_WF_LIST;
	newT->wtHead = NULL_WT_LIST;
	newT->wtTail = NULL_WT_LIST;
	newT->asHead = NULL_AS_LIST;
	newT->asTail = NULL_AS_LIST;
	newT->evHead = NULL_EV_LIST;
	newT->evTail = NULL_EV_LIST;
	newT->grHead = NULL_GR_LIST;
	newT->grTail = NULL_GR_LIST;
	newT->inHead = NULL_IN_LIST;
	newT->inTail = NULL_IN_LIST;
	newT->oeHead = NULL_OE_LIST;
	newT->oeTail = NULL_OE_LIST;
	newT->seHead = NULL_SE_LIST;
	newT->seTail = NULL_SE_LIST;
	newT->slHead = NULL_SL_LIST;
	newT->slTail = NULL_SL_LIST;
	newT->scHead = NULL_SC_LIST;
	newT->scTail = NULL_SC_LIST;
	newT->saHead = NULL_SA_LIST;
	newT->saTail = NULL_SA_LIST;
	newT->sdHead = NULL_SD_LIST;
	newT->sdTail = NULL_SD_LIST;
        newT->UserData = NULL_UD_LIST;
	
	return (DBlist) newT;
}
/* --------------------------------------------------------------- */





/* This function creates a new instance of one of the CSS List structs and attaches the */
/* new struct to its proper list. The new table is filled with default values. */
DBtable dblCreateTableInstance(DBlist list, dblObject StrucType)
{

	struct affiliationList *newafPntr;
	struct arrivalList     *newarPntr;
	struct assocList       *newasPntr;
	struct eventList       *newevPntr;
	struct gregionList     *newgrPntr;
	struct instrumentList  *newinPntr;
	struct origerrList     *newoePntr;
	struct originList      *neworPntr;
	struct remarkList      *newrePntr;
	struct sensorList      *newsePntr;
	struct siteList        *newslPntr;
	struct sitechanList    *newscPntr;
	struct stassocList     *newsaPntr;
	struct wfdiscList      *newwfPntr;	
	struct wftagList       *newwtPntr;	
	struct sacdataList     *newsdPntr;	

	struct CSStree *tree;
	
		
	tree = (struct CSStree *) list;
	if(!tree){
           dblSetError(1, "ERROR: Null input tree in dblCreateTableInstance.\n");
	   return (void *)NULL;
	}

	switch (StrucType){
	   case dbl_LIST_AFFILIATION:{
	      newafPntr = AF_LIST smMalloc(sizeof(struct affiliationList));
	      if(newafPntr == NULL_AF_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate affiliation struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      newafPntr->element = (struct affiliation *) 
	                            smMalloc(sizeof(struct affiliation));
	      if(newafPntr->element == (struct affiliation *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate affiliation element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }

	      *(newafPntr->element) = nullAffiliation;
              newafPntr->index = dblNextIndex(); 
	      newafPntr->next = NULL_AF_LIST;
	      if(tree->afHead == NULL_AF_LIST){
	         tree->afHead = newafPntr;
	         tree->afTail = newafPntr;
	         newafPntr->prev = NULL_AF_LIST;
	         
	      }
	      else{
	         newafPntr->prev = tree->afTail;
	         tree->afTail->next = newafPntr;
	         tree->afTail = newafPntr;
	      }
	      return (DBtable) newafPntr;
	   }
	
	   case dbl_LIST_ARRIVAL:{
	      newarPntr = AR_LIST smMalloc(sizeof(struct arrivalList ));
	      if(newarPntr == NULL_AR_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate arrival struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      newarPntr->element = (struct arrival *) 
	                            smMalloc(sizeof(struct arrival ));
	      if(newarPntr->element == (struct arrival *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate arrival element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      *(newarPntr->element) = nullArrival; 
              newarPntr->index = dblNextIndex(); 
	      newarPntr->next = NULL_AR_LIST;
	      if(tree->arHead == NULL_AR_LIST){
	         tree->arHead = newarPntr;
	         tree->arTail = newarPntr;
	         newarPntr->prev = NULL_AR_LIST;
	         
	      }
	      else{
	         newarPntr->prev = tree->arTail;
	         tree->arTail->next = newarPntr;
	         tree->arTail = newarPntr;
	      }
	      return (DBtable) newarPntr;
	   }
	
	   case dbl_LIST_ASSOC:{
	      newasPntr = AS_LIST smMalloc(sizeof(struct assocList ));
	      if(newasPntr == NULL_AS_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate assoc struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      newasPntr->element = (struct assoc *) 
	      smMalloc(sizeof(struct arrival ));
	      if(newasPntr->element == (struct assoc *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate assoc element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      *(newasPntr->element) = nullAssoc; 
              newasPntr->index = dblNextIndex(); 
	      newasPntr->next = NULL_AS_LIST;
	      if(tree->asHead == NULL_AS_LIST){
	         tree->asHead = newasPntr;
	         tree->asTail = newasPntr;
	         newasPntr->prev = NULL_AS_LIST;
	         
	      }
	      else{
	         newasPntr->prev = tree->asTail;
	         tree->asTail->next = newasPntr;
	         tree->asTail = newasPntr;
	      }
	      return (DBtable) newasPntr;
	   }
	
	   case dbl_LIST_EVENT:{
	      newevPntr = EV_LIST smMalloc(sizeof(struct eventList ));
	      if(newevPntr == NULL_EV_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate event struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      newevPntr->element = (struct event *) 
	                            smMalloc(sizeof(struct arrival ));
	      if(newevPntr->element == (struct event *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate event element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      *(newevPntr->element) = nullEvent; 
              newevPntr->index = dblNextIndex(); 
	      newevPntr->next = NULL_EV_LIST;
	      if(tree->evHead == NULL_EV_LIST){
	         tree->evHead = newevPntr;
	         tree->evTail = newevPntr;
	         newevPntr->prev = NULL_EV_LIST;
	         
	      }
	      else{
	         newevPntr->prev = tree->evTail;
	         tree->evTail->next = newevPntr;
	         tree->evTail = newevPntr;
	      }
	      return (DBtable) newevPntr;
	   }
	
	   case dbl_LIST_GREGION:{
	      newgrPntr = GR_LIST smMalloc(sizeof(struct gregionList ));
	      if(newgrPntr == NULL_GR_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate gregion struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      newgrPntr->element = (struct gregion *) 
	                            smMalloc(sizeof(struct gregion ));
	      if(newgrPntr->element == (struct gregion *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate gregion element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      *(newgrPntr->element) = nullGregion; 
              newgrPntr->index = dblNextIndex(); 
	      newgrPntr->next = NULL_GR_LIST;
	      if(tree->grHead == NULL_GR_LIST){
	         tree->grHead = newgrPntr;
	         tree->grTail = newgrPntr;
	         newgrPntr->prev = NULL_GR_LIST;
	         
	      }
	      else{
	         newgrPntr->prev = tree->grTail;
	         tree->grTail->next = newgrPntr;
	         tree->grTail = newgrPntr;
	      }
	      return (DBtable) newgrPntr;
	   }
	
	   case dbl_LIST_INSTRUMENT:{
	      newinPntr = IN_LIST smMalloc(sizeof(struct instrumentList ));
	      if(newinPntr == NULL_IN_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate instrument struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      newinPntr->element = (struct instrument *) 
	                            smMalloc(sizeof(struct instrument ));
	      if(newinPntr->element == (struct instrument *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate instrument element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      *(newinPntr->element) = nullInstrument; 
              newinPntr->index = dblNextIndex(); 
	      newinPntr->next = NULL_IN_LIST;
	      if(tree->inHead == NULL_IN_LIST){
	         tree->inHead = newinPntr;
	         tree->inTail = newinPntr;
	         newinPntr->prev = NULL_IN_LIST;
	         
	      }
	      else{
	         newinPntr->prev = tree->inTail;
	         tree->inTail->next = newinPntr;
	         tree->inTail = newinPntr;
	      }
	      return (DBtable) newinPntr;
	   }
	
	   case dbl_LIST_ORIGERR:{
	      newoePntr = OE_LIST  smMalloc(sizeof(struct origerrList ));
	      if(newoePntr == NULL_OE_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate origerr struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      newoePntr->element = (struct origerr *) 
	                            smMalloc(sizeof(struct origerr ));
	      if(newoePntr->element == (struct origerr *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate origerr element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      *(newoePntr->element) = nullOrigerr; 
              newoePntr->index = dblNextIndex(); 
	      newoePntr->next = NULL_OE_LIST;
	      if(tree->oeHead == NULL_OE_LIST){
	         tree->oeHead = newoePntr;
	         tree->oeTail = newoePntr;
	         newoePntr->prev = NULL_OE_LIST;
	         
	      }
	      else{
	         newoePntr->prev = tree->oeTail;
	         tree->oeTail->next = newoePntr;
	         tree->oeTail = newoePntr;
	      }
	      return (DBtable) newoePntr;
	   }
	
	   case dbl_LIST_ORIGIN:{
	      neworPntr = OR_LIST smMalloc(sizeof(struct originList));
	      if(neworPntr == NULL_OR_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate origin struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      neworPntr->element = (struct origin *) 
	                            smMalloc(sizeof(struct origin));
	      if(neworPntr->element == (struct origin *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate origin element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      *(neworPntr->element) = nullOrigin; 
              neworPntr->index = dblNextIndex(); 
	      neworPntr->next = NULL_OR_LIST;
	      if(tree->orHead == NULL_OR_LIST){
	         tree->orHead = neworPntr;
	         tree->orTail = neworPntr;
	         neworPntr->prev = NULL_OR_LIST;
	         
	      }
	      else{
	         neworPntr->prev = tree->orTail;
	         tree->orTail->next = neworPntr;
	         tree->orTail = neworPntr;
	      }
	      return (DBtable) neworPntr;
	   }
	
	   case dbl_LIST_REMARK:{
	      newrePntr = RE_LIST smMalloc(sizeof(struct remarkList));
	      if(newrePntr == NULL_RE_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate remark struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      newrePntr->element = (struct remark *) 
	                            smMalloc(sizeof(struct remark));
	      if(newrePntr->element == (struct remark *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate remark element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      *(newrePntr->element) = nullRemark; 
              newrePntr->index = dblNextIndex(); 
	      newrePntr->next = NULL_RE_LIST;
	      if(tree->reHead == NULL_RE_LIST){
	         tree->reHead = newrePntr;
	         tree->reTail = newrePntr;
	         newrePntr->prev = NULL_RE_LIST;
	         
	      }
	      else{
	         newrePntr->prev = tree->reTail;
	         tree->reTail->next = newrePntr;
	         tree->reTail = newrePntr;
	      }
	      return (DBtable) newrePntr;
	   }
	
	   case dbl_LIST_SENSOR:{
	      newsePntr = SE_LIST smMalloc(sizeof(struct sensorList));
	      if(newsePntr == NULL_SE_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate sensor struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      newsePntr->element = (struct sensor *) 
	                            smMalloc(sizeof(struct sensor));
	      if(newsePntr->element == (struct sensor *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate sensor element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      *(newsePntr->element) = nullSensor; 
              newsePntr->index = dblNextIndex(); 
	      newsePntr->next = NULL_SE_LIST;
	      if(tree->seHead == NULL_SE_LIST){
	         tree->seHead = newsePntr;
	         tree->seTail = newsePntr;
	         newsePntr->prev = NULL_SE_LIST;
	         
	      }
	      else{
	         newsePntr->prev = tree->seTail;
	         tree->seTail->next = newsePntr;
	         tree->seTail = newsePntr;
	      }
	      return (DBtable) newsePntr;
	   }
	
	   case dbl_LIST_SITE:{
	      newslPntr = SL_LIST smMalloc(sizeof(struct siteList));
	      if(newslPntr == NULL_SL_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate site struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      newslPntr->element = (struct site *) 
	                            smMalloc(sizeof(struct site));
	      if(newslPntr->element == (struct site *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate site element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      *(newslPntr->element) = nullSite; 
              newslPntr->index = dblNextIndex(); 
	      newslPntr->next = NULL_SL_LIST;
	      if(tree->slHead == NULL_SL_LIST){
	         tree->slHead = newslPntr;
	         tree->slTail = newslPntr;
	         newslPntr->prev = NULL_SL_LIST;
	         
	      }
	      else{
	         newslPntr->prev = tree->slTail;
	         tree->slTail->next = newslPntr;
	         tree->slTail = newslPntr;
	      }
	      return (DBtable) newslPntr;
	   }
	
	   case dbl_LIST_SITECHAN:{
	      newscPntr = SC_LIST smMalloc(sizeof(struct sitechanList));
	      if(newscPntr == NULL_SC_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate sitechan struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      newscPntr->element = (struct sitechan *) 
	                            smMalloc(sizeof(struct sitechan));
	      if(newscPntr->element == (struct sitechan *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate sitechan element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      *(newscPntr->element) = nullSitechan; 
              newscPntr->index = dblNextIndex(); 
	      newscPntr->next = NULL_SC_LIST;
	      if(tree->scHead == NULL_SC_LIST){
	         tree->scHead = newscPntr;
	         tree->scTail = newscPntr;
	         newscPntr->prev = NULL_SC_LIST;
	         
	      }
	      else{
	         newscPntr->prev = tree->scTail;
	         tree->scTail->next = newscPntr;
	         tree->scTail = newscPntr;
	      }
	      return (DBtable) newscPntr;
	   }
	
	   case dbl_LIST_STASSOC:{
	      newsaPntr = SA_LIST smMalloc(sizeof(struct stassocList));
	      if(newsaPntr == NULL_SA_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate stassoc struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      newsaPntr->element = (struct stassoc *) 
	                            smMalloc(sizeof(struct stassoc));
	      if(newsaPntr->element == (struct stassoc *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate stassoc element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      *(newsaPntr->element) = nullStassoc; 
              newsaPntr->index = dblNextIndex(); 
	      newsaPntr->next = NULL_SA_LIST;
	      if(tree->saHead == NULL_SA_LIST){
	         tree->saHead = newsaPntr;
	         tree->saTail = newsaPntr;
	         newsaPntr->prev = NULL_SA_LIST;
	         
	      }
	      else{
	         newsaPntr->prev = tree->saTail;
	         tree->saTail->next = newsaPntr;
	         tree->saTail = newsaPntr;
	      }
	      return (DBtable) newsaPntr;
	   }
	
	   case dbl_LIST_WFDISC:{
	      newwfPntr = WF_LIST smMalloc(sizeof(struct wfdiscList));
	      if(newwfPntr == NULL_WF_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate wfdisc struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      newwfPntr->element = (struct wfdisc *) 
	                            smCalloc(1, sizeof(struct wfdisc));
	      if(newwfPntr->element == (struct wfdisc *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate wfdisc element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      
	      *(newwfPntr->element) = wfdisc_null;

              newwfPntr->element->nsamp = 0;
	      newwfPntr->seis = (struct trace *) smMalloc(sizeof(struct trace) );
	      newwfPntr->seis->r = (float *)NULL;
	      newwfPntr->seis->i = (float *)NULL;
	      newwfPntr->seis->Cmplx = 0;
              newwfPntr->index = dblNextIndex(); 
	      newwfPntr->next = NULL_WF_LIST;
	      if(tree->wfHead == NULL_WF_LIST){
	         tree->wfHead = newwfPntr;
	         tree->wfTail = newwfPntr;
	         newwfPntr->prev = NULL_WF_LIST;
	         
	      }
	      else{
	         newwfPntr->prev = tree->wfTail;
	         tree->wfTail->next = newwfPntr;
	         tree->wfTail = newwfPntr;
	      }
   /* Why doesn't list get updated before we return? PG 2-26-01 */
	      return (DBtable) newwfPntr;
	   }

	
	   case dbl_LIST_WFTAG:{
	      newwtPntr = WT_LIST smMalloc(sizeof(struct wftagList));
	      if(newwtPntr == NULL_WT_LIST){
                 dblSetError(1, 
                 "ERROR: Could not allocate wftag struct in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      newwtPntr->element = (struct wftag *) 
	                            smMalloc(sizeof(struct wftag));
	      if(newwtPntr->element == (struct wftag *) NULL){
                 dblSetError(1, 
                 "ERROR: Could not allocate wftag element in dblCreateTableInstance.\n");
	         return (void *)NULL;
              }
	      *(newwtPntr->element) = nullWftag; 
              newwtPntr->index = dblNextIndex(); 
	      newwtPntr->next = NULL_WT_LIST;
	      if(tree->wtHead == NULL_WT_LIST){
	         tree->wtHead = newwtPntr;
	         tree->wtTail = newwtPntr;
	         newwtPntr->prev = NULL_WT_LIST;
	         
	      }
	      else{
	         newwtPntr->prev = tree->wtTail;
	         tree->wtTail->next = newwtPntr;
	         tree->wtTail = newwtPntr;
	      }
	      return (DBtable) newwtPntr;
	   }
	

	
	   case dbl_LIST_SACDATA:{
	      newsdPntr = SD_LIST smMalloc(sizeof(struct sacdataList));
	      if(!newsdPntr){
                 dblSetError(1, 
                 "ERROR: Could not allocate sacdata struct in dblCreateTableInstance.\n");
	         return 0;
              }
	      newsdPntr->element = (struct sacdata *) 
	                            smMalloc(sizeof(struct sacdata));
	      if(!newsdPntr->element){
                 dblSetError(1, 
                 "ERROR: Could not allocate sacdata element in dblCreateTableInstance.\n");
	         return 0;
              }
	      *(newsdPntr->element) = nullSacdata; 
              newsdPntr->index      = dblNextIndex(); 
	      newsdPntr->next       = 0;
	      newsdPntr->prev       = 0;
	      if(tree->sdHead == 0){
	         tree->sdHead       = newsdPntr;
	         tree->sdTail       = newsdPntr;
	      }
	      else{
	         newsdPntr->prev    = tree->sdTail;
	         tree->sdTail->next = newsdPntr;
	         tree->sdTail       = newsdPntr;
	      }
	      return (DBtable) newsdPntr;
	   }
	

	
	   default: {
	      return (DBtable) NULL;
	   }
	}
}
/*----------------------------------------------------------------------*/








void dblTableOfContents(DBlist dblList, FILE *unit)
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
	struct CSStree *tree;
        DataComment  *curDCpntr;
   

	int NumRows;
	int NumXdata, NumYdata;
	int NumDC;
	
	tree = (struct CSStree *) dblList;
	if(!tree)return;


	afPntr = tree->afHead;
	NumRows = 0;
	while(afPntr){
	   NumRows++;
	   afPntr = afPntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \taffiliation structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}

	arPntr = tree->arHead;
	NumRows = 0;
	while(arPntr){
	   NumRows++;
	   arPntr = arPntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \tarrival     structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}

	asPntr = tree->asHead;
	NumRows = 0;
	while(asPntr){
	   NumRows++;
	   asPntr = asPntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \tassoc       structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}

	evPntr = tree->evHead;
	NumRows = 0;
	while(evPntr){
	   NumRows++;
	   evPntr = evPntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \tevent       structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}

	grPntr = tree->grHead;
	NumRows = 0;
	while(grPntr){
	   NumRows++;
	   grPntr = grPntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \tgregion     structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}

	inPntr = tree->inHead;
	NumRows = 0;
	while(inPntr){
	   NumRows++;
	   inPntr = inPntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \tinstrument  structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}


	oePntr = tree->oeHead;
	NumRows = 0;
	while(oePntr){
	   NumRows++;
	   oePntr = oePntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \torigerr     structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}

	orPntr = tree->orHead;
	NumRows = 0;
	while(orPntr){
	   NumRows++;
	   orPntr = orPntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \torigin      structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}

	rePntr = tree->reHead;
	NumRows = 0;
	while(rePntr){
	   NumRows++;
	   rePntr = rePntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \tremark      structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}

	sePntr = tree->seHead;
	NumRows = 0;
	while(sePntr){
	   NumRows++;
	   sePntr = sePntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \tsensor      structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}

	slPntr = tree->slHead;
	NumRows = 0;
	while(slPntr){
	   NumRows++;
	   slPntr = slPntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \tsite        structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}

	scPntr = tree->scHead;
	NumRows = 0;
	while(scPntr){
	   NumRows++;
	   scPntr = scPntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \tsitechan    structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}

	saPntr = tree->saHead;
	NumRows = 0;
	while(saPntr){
	   NumRows++;
	   saPntr = saPntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \tstassoc     structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}


	wtPntr = tree->wtHead;
	NumRows = 0;
	while(wtPntr){
	   NumRows++;
	   wtPntr = wtPntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \twftag       structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}


	sdPntr = tree->sdHead;
	NumRows = 0;
	while(sdPntr){
	   NumRows++;
	   sdPntr = sdPntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \tsacdata     structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}


	NumXdata = NumYdata = 0;
	wfPntr = tree->wfHead;
	NumRows = 0;
	while(wfPntr){
	   NumRows++;
	   if(wfPntr->seis->r) NumXdata++;
	   if(wfPntr->seis->i) NumYdata++;
	   wfPntr = wfPntr->next;
	}
	if(NumRows){
	  if( fprintf(unit,"%3d \twfdisc      structs. \n",NumRows) == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	  if(NumXdata){
	     if( fprintf(unit,"\t-----%3d have X data. \n",NumXdata) == EOF ){
	        dblSetError(1, 
			    "ERROR: Error writing file instance in dblTableOfContents.\n");
                return;
	     }
	  }
	  if(NumYdata){
	     if( fprintf(unit,"\t-----%3d have Y data. \n",NumYdata) == EOF ){
	        dblSetError(1, 
			    "ERROR: Error writing file instance in dblTableOfContents.\n");
                return;
	     }
	  }
	}


        if(!tree->UserData){
	  if( fprintf(unit,"No UserData struct. \n") == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	  return;
	}

        if(tree->UserData->comment){
	  if( fprintf(unit,"  1 UserData Comment struct. \n") == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}

	if(tree->UserData->matrix){
	  if( fprintf(unit,"  1 UserData Matrix struct. \n") == EOF ){
	      dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
              return;
	  }
	}

	if(tree->UserData->dataComment){
	     NumDC = 0;
             curDCpntr = tree->UserData->dataComment;
             while(curDCpntr){
	         NumDC++;
	         curDCpntr = curDCpntr->next;
	     }
	     if( fprintf(unit,"%3d UserData DataComment structs. \n",NumDC) == EOF ){
	        dblSetError(1, "ERROR: Error writing file instance in dblTableOfContents.\n");
                return;
	     }
	}
}
/*----------------------------------------------------------------------*/






/* This function deletes the CSS liststruct pointed to by pntr from the tree pointed */
/* to by dbList. Pointers are rearranged as necessary and memory is freed */
void dblDeleteTableInstance(dblObject StrucType, DBlist dblList,  DBtable pntr)
{
/* Delete a CSS struct from its list */
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
	struct CSStree *tree;
	
	tree = (struct CSStree *) dblList;
	if(!tree)return;
	if(!pntr)return;

	switch (StrucType){
	   case dbl_LIST_AFFILIATION:{
	      afPntr = AF_LIST pntr;
              if(afPntr == tree->afHead && afPntr == tree->afTail){
                 tree->afHead = 0; tree->afTail = 0;
	      }
              else if(afPntr == tree->afHead){
                 tree->afHead = afPntr->next; tree->afHead->prev = 0;
	      }
              else if(afPntr == tree->afTail){
		 tree->afTail = afPntr->prev; tree->afTail->next = 0;
	      }
	      else{
                 afPntr->prev->next = afPntr->next;
                 afPntr->next->prev = afPntr->prev;
	      }
	      smFree(afPntr->element);
	      smFree( afPntr );
	      return;
	   }

	   case dbl_LIST_ARRIVAL:{
	      arPntr = (struct arrivalList *) pntr;
              if(arPntr == tree->arHead && arPntr == tree->arTail){
                 tree->arHead = 0; tree->arTail = 0;
	      }
              else if(arPntr == tree->arHead){
                 tree->arHead = arPntr->next; tree->arHead->prev = 0;
	      }
              else if(arPntr == tree->arTail){
		 tree->arTail = arPntr->prev; tree->arTail->next = 0;
	      }
	      else{
                 arPntr->prev->next = arPntr->next;
                 arPntr->next->prev = arPntr->prev;
	      }
	      smFree(arPntr->element);
	      smFree( arPntr );
	      return;
	   }


	   case dbl_LIST_ASSOC:{
	      asPntr = AS_LIST pntr;
              if(asPntr == tree->asHead && asPntr == tree->asTail){
                 tree->asHead = 0; tree->asTail = 0;
	      }
              else if(asPntr == tree->asHead){
                 tree->asHead = asPntr->next; tree->asHead->prev = 0;
	      }
              else if(asPntr == tree->asTail){
		 tree->asTail = asPntr->prev; tree->asTail->next = 0;
	      }
	      else{
                 asPntr->prev->next = asPntr->next;
                 asPntr->next->prev = asPntr->prev;
	      }
	      smFree(asPntr->element);
	      smFree( asPntr );
	      return;
	   }

	   case dbl_LIST_EVENT:{
	      evPntr = EV_LIST pntr;
              if(evPntr == tree->evHead && evPntr == tree->evTail){
                 tree->evHead = 0; tree->evTail = 0;
	      }
              else if(evPntr == tree->evHead){
                 tree->evHead = evPntr->next; tree->evHead->prev = 0;
	      }
              else if(evPntr == tree->evTail){
		 tree->evTail = evPntr->prev; tree->evTail->next = 0;
	      }
	      else{
                 evPntr->prev->next = evPntr->next;
                 evPntr->next->prev = evPntr->prev;
	      }
	      smFree(evPntr->element);
	      smFree( evPntr );
	      return;
	   }

	   case dbl_LIST_GREGION:{
	      grPntr = GR_LIST pntr;
              if(grPntr == tree->grHead && grPntr == tree->grTail){
                 tree->grHead = 0; tree->grTail = 0;
	      }
              else if(grPntr == tree->grHead){
                 tree->grHead = grPntr->next; tree->grHead->prev = 0;
	      }
              else if(grPntr == tree->grTail){
		 tree->grTail = grPntr->prev; tree->grTail->next = 0;
	      }
	      else{
                 grPntr->prev->next = grPntr->next;
                 grPntr->next->prev = grPntr->prev;
	      }
	      smFree(grPntr->element);
	      smFree( grPntr );
	      return;
	   }

	   case dbl_LIST_INSTRUMENT:{
	      inPntr = IN_LIST pntr;
              if(inPntr == tree->inHead && inPntr == tree->inTail){
                 tree->inHead = 0; tree->inTail = 0;
	      }
              else if(inPntr == tree->inHead){
                 tree->inHead = inPntr->next; tree->inHead->prev = 0;
	      }
              else if(inPntr == tree->inTail){
		 tree->inTail = inPntr->prev; tree->inTail->next = 0;
	      }
	      else{
                 inPntr->prev->next = inPntr->next;
                 inPntr->next->prev = inPntr->prev;
	      }
	      smFree(inPntr->element);
	      smFree( inPntr );
	      return;
	   }

	   case dbl_LIST_ORIGERR:{
	      oePntr = OE_LIST pntr;
              if(oePntr == tree->oeHead && oePntr == tree->oeTail){
                 tree->oeHead = 0; tree->oeTail = 0;
	      }
              else if(oePntr == tree->oeHead){
                 tree->oeHead = oePntr->next; tree->oeHead->prev = 0;
	      }
              else if(oePntr == tree->oeTail){
		 tree->oeTail = oePntr->prev; tree->oeTail->next = 0;
	      }
	      else{
                 oePntr->prev->next = oePntr->next;
                 oePntr->next->prev = oePntr->prev;
	      }
	      smFree(oePntr->element);
	      smFree( oePntr );
	      return;
	   }
	

	   case dbl_LIST_ORIGIN:{
	      orPntr = OR_LIST pntr;
              if(orPntr == tree->orHead && orPntr == tree->orTail){
                 tree->orHead = 0; tree->orTail = 0;
	      }
              else if(orPntr == tree->orHead){
                 tree->orHead = orPntr->next; tree->orHead->prev = 0;
	      }
              else if(orPntr == tree->orTail){
		 tree->orTail = orPntr->prev; tree->orTail->next = 0;
	      }
	      else{
                 orPntr->prev->next = orPntr->next;
                 orPntr->next->prev = orPntr->prev;
	      }
	      smFree(orPntr->element);
	      smFree( orPntr );
	      return;
	   }
	

	   case dbl_LIST_REMARK:{
	      rePntr = RE_LIST pntr;
              if(rePntr == tree->reHead && rePntr == tree->reTail){
                 tree->reHead = 0; tree->reTail = 0;
	      }
              else if(rePntr == tree->reHead){
                 tree->reHead = rePntr->next; tree->reHead->prev = 0;
	      }
              else if(rePntr == tree->reTail){
		 tree->reTail = rePntr->prev; tree->reTail->next = 0;
	      }
	      else{
                 rePntr->prev->next = rePntr->next;
                 rePntr->next->prev = rePntr->prev;
	      }
	      smFree(rePntr->element);
	      smFree( rePntr );
	      return;
	   }

	   case dbl_LIST_SENSOR:{
	      sePntr = SE_LIST pntr;
              if(sePntr == tree->seHead && sePntr == tree->seTail){
                 tree->seHead = 0; tree->seTail = 0;
	      }
              else if(sePntr == tree->seHead){
                 tree->seHead = sePntr->next; tree->seHead->prev = 0;
	      }
              else if(sePntr == tree->seTail){
		 tree->seTail = sePntr->prev; tree->seTail->next = 0;
	      }
	      else{
                 sePntr->prev->next = sePntr->next;
                 sePntr->next->prev = sePntr->prev;
	      }
	      smFree(sePntr->element);
	      smFree( sePntr );
	      return;
	   }

	   case dbl_LIST_SITE:{
	      slPntr = SL_LIST pntr;
              if(slPntr == tree->slHead && slPntr == tree->slTail){
                 tree->slHead = 0; tree->slTail = 0;
	      }
              else if(slPntr == tree->slHead){
                 tree->slHead = slPntr->next; tree->slHead->prev = 0;
	      }
              else if(slPntr == tree->slTail){
		 tree->slTail = slPntr->prev; tree->slTail->next = 0;
	      }
	      else{
                 slPntr->prev->next = slPntr->next;
                 slPntr->next->prev = slPntr->prev;
	      }
	      smFree(slPntr->element);
	      smFree( slPntr );
	      return;
	   }

	   case dbl_LIST_SITECHAN:{
	      scPntr = SC_LIST pntr;
              if(scPntr == tree->scHead && scPntr == tree->scTail){
                 tree->scHead = 0; tree->scTail = 0;
	      }
              else if(scPntr == tree->scHead){
                 tree->scHead = scPntr->next; tree->scHead->prev = 0;
	      }
              else if(scPntr == tree->scTail){
		 tree->scTail = scPntr->prev; tree->scTail->next = 0;
	      }
	      else{
                 scPntr->prev->next = scPntr->next;
                 scPntr->next->prev = scPntr->prev;
	      }
	      smFree(scPntr->element);
	      smFree( scPntr );
	      return;
	   }

	   case dbl_LIST_STASSOC:{
	      saPntr = SA_LIST pntr;
              if(saPntr == tree->saHead && saPntr == tree->saTail){
                 tree->saHead = 0; tree->saTail = 0;
	      }
              else if(saPntr == tree->saHead){
                 tree->saHead = saPntr->next; tree->saHead->prev = 0;
	      }
              else if(saPntr == tree->saTail){
		 tree->saTail = saPntr->prev; tree->saTail->next = 0;
	      }
	      else{
                 saPntr->prev->next = saPntr->next;
                 saPntr->next->prev = saPntr->prev;
	      }
	      smFree(saPntr->element);
	      smFree( saPntr );
	      return;
	   }
	
	   case dbl_LIST_WFDISC:{
	      wfPntr = WF_LIST pntr;
              if(wfPntr == tree->wfHead && wfPntr == tree->wfTail){
                 tree->wfHead = 0; tree->wfTail = 0;
	      }
              else if(wfPntr == tree->wfHead){
                 tree->wfHead = wfPntr->next; tree->wfHead->prev = 0;
	      }
              else if(wfPntr == tree->wfTail){
		 tree->wfTail = wfPntr->prev; tree->wfTail->next = 0;
	      }
	      else{
                 wfPntr->prev->next = wfPntr->next;
                 wfPntr->next->prev = wfPntr->prev;
	      }
	      smFree(wfPntr->element);
	      smFree( wfPntr );
	      return;
	   }


	   case dbl_LIST_WFTAG:{
	      wtPntr = WT_LIST pntr;
              if(wtPntr == tree->wtHead && wtPntr == tree->wtTail){
                 tree->wtHead = 0; tree->wtTail = 0;
	      }
              else if(wtPntr == tree->wtHead){
                 tree->wtHead = wtPntr->next; 
                 if(tree && tree->wtHead) {
                     tree->wtHead->prev = 0;
                 }
	      }
              else if(wtPntr == tree->wtTail){
		 tree->wtTail = wtPntr->prev; tree->wtTail->next = 0;
	      }
	      else{
              if(wtPntr && wtPntr->prev) {
                 wtPntr->prev->next = wtPntr->next;
              }
              if(wtPntr && wtPntr->next) {
                 wtPntr->next->prev = wtPntr->prev;
              }
	      }
	      smFree(wtPntr->element);
	      smFree( wtPntr );
	      return;
	   }


	   case dbl_LIST_SACDATA:{
	      sdPntr = SD_LIST pntr;
               if(sdPntr == tree->sdHead && sdPntr == tree->sdTail){
                 tree->sdHead = 0; tree->sdTail = 0;
	      }
              else if(sdPntr == tree->sdHead){
                 tree->sdHead = sdPntr->next; 
                 if(tree && tree->sdHead) {
                     tree->sdHead->prev = 0;
                 }
	      }
              else if(sdPntr == tree->sdTail){
		 tree->sdTail = sdPntr->prev; tree->sdTail->next = 0;
	      }
	      else{
              if(sdPntr && sdPntr->prev) {
                  sdPntr->prev->next = sdPntr->next;
              }
              if(sdPntr && sdPntr->next) {
                 sdPntr->next->prev = sdPntr->prev;
              }
	      }
	      smFree(sdPntr->element);
	      smFree( sdPntr );
	      return;
	   }



	
	   default: return;
	}
}
/*----------------------------------------------------------------------*/








/* Return an index to the current structure */
int dblGetTableIndex(dblObject StrucType,  DBtable pntr)
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
	
	if(!pntr)return 0;

	switch (StrucType){
	   case dbl_LIST_AFFILIATION:{
	      afPntr = AF_LIST pntr;
	      return afPntr->index;
	   }

	   case dbl_LIST_ARRIVAL:{
	      arPntr = (struct arrivalList *) pntr;
	      return arPntr->index;
	   }

	   case dbl_LIST_ASSOC:{
	      asPntr = AS_LIST pntr;
	      return asPntr->index;
	   }

	   case dbl_LIST_EVENT:{
	      evPntr = EV_LIST pntr;
	      return evPntr->index;
	   }

	   case dbl_LIST_GREGION:{
	      grPntr = GR_LIST pntr;
	      return grPntr->index;
	   }

	   case dbl_LIST_INSTRUMENT:{
	      inPntr = IN_LIST pntr;
	      return inPntr->index;
	   }

	   case dbl_LIST_ORIGERR:{
	      oePntr = OE_LIST pntr;
	      return oePntr->index;
	   }

	   case dbl_LIST_ORIGIN:{
	      orPntr = OR_LIST pntr;
	      return orPntr->index;
	   }

	   case dbl_LIST_REMARK:{
	      rePntr = RE_LIST pntr;
	      return rePntr->index;
	   }

	   case dbl_LIST_SENSOR:{
	      sePntr = SE_LIST pntr;
	      return sePntr->index;
	   }

	   case dbl_LIST_SITE:{
	      slPntr = SL_LIST pntr;
	      return slPntr->index;
	   }

	   case dbl_LIST_SITECHAN:{
	      scPntr = SC_LIST pntr;
	      return scPntr->index;
	   }

	   case dbl_LIST_STASSOC:{
	      saPntr = SA_LIST pntr;
	      return saPntr->index;
	   }
	
	   case dbl_LIST_WFDISC:{
	      wfPntr = WF_LIST pntr;
	      return wfPntr->index;
	   }
	
	   case dbl_LIST_WFTAG:{
	      wtPntr = WT_LIST pntr;
	      return wtPntr->index;
	   }
	
	   case dbl_LIST_SACDATA:{
	      sdPntr = SD_LIST pntr;
	      return sdPntr->index;
	   }
	
	   default: return 0;
	}
}
/*----------------------------------------------------------------------*/






/* This function sets trace data into a wfdiscList struct. It is called with tableInstance */
/* pointing to a wfdiscList struct which may or may not have allocated trace data */
/* attached. You can set the real part only by specifying dtype to be REALDATA and */
/* having data point to an array of floats which are the real-data to be set. Similarly, */
/* you can set the imaginary part only by specifying dtype to be IMAGDATA and having data  */
/* point to an array of floats which are the imaginary-data to be set. You can also set */
/* the real and imaginary parts simultaneously by specifying dtype to be COMPLEXDATA and  */
/* having data point to an array of ComplexFloats. */

void dblSetTraceData(DBtable tableInstance, DataType dtype, void *data, int npts)
{
     struct wfdiscList      *wfPntr;	
     float *inPntr, *copyPntr, *copyImagPntr;
     ComplexFloat *inpntr;
     int j;



     if(!data)return;
     if(npts < 1 )return;
     wfPntr = WF_LIST tableInstance;
     if(!wfPntr){
        dblSetError(1, "ERROR: Empty struc pointer passed to dblSetTraceData.\n");
        return;
     }

     switch(dtype){
       case REALDATA:{
	  inPntr = (float *)data;
	  copyPntr = (float *) smMalloc(npts * sizeof(float) );
	  for(j=0;j<npts;j++) *(copyPntr +j) = *(inPntr +j);
	  if(wfPntr->seis->r) smFree(wfPntr->seis->r);
	  wfPntr->seis->r = copyPntr;
	  wfPntr->element->nsamp = npts;
	  return;
       }

       case IMAGDATA:{
	  inPntr = (float *)data;
	  copyPntr = (float *) smMalloc(npts * sizeof(float) );
	  for(j=0;j<npts;j++) *(copyPntr +j) = *(inPntr +j);
	  if(wfPntr->seis->i) smFree(wfPntr->seis->i);
	  wfPntr->seis->i = copyPntr;
	  wfPntr->seis->Cmplx = 1;
	  wfPntr->element->nsamp = npts;
	  return;
       }

       case COMPLEXDATA:{
	  inpntr = (ComplexFloat *)data;
	  copyPntr = (float *) smMalloc(npts * sizeof(float) );
	  copyImagPntr = (float *) smMalloc(npts * sizeof(float) );
	  for(j=0;j<npts;j++){
 	     *(copyPntr +j) = inpntr[j].real;
 	     *(copyImagPntr +j) = inpntr[j].imag; 
	  }
	  if(wfPntr->seis->r) smFree(wfPntr->seis->r);
	  if(wfPntr->seis->i) smFree(wfPntr->seis->i);
	  wfPntr->seis->r = copyPntr;
	  wfPntr->seis->i = copyImagPntr;
	  wfPntr->seis->Cmplx = 1;
	  wfPntr->element->nsamp = npts;
	  return;
       }

     }

}
/*----------------------------------------------------------------------*/






/* This function deletes an entire CSS tree and all its associated data. All memory */
/* used in the tree is reclaimed. */
void dblDeleteTree(DBlist Tree)
{
/* for each non-empty list in the tree, traverse the list deleting
   each list element. When all the lists are empty, delete the tree. */

	struct CSStree *tree;	



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
	
	tree = (struct CSStree *) Tree;
        if(!tree)return;
	
	afPntr = tree->afTail;   
	while (afPntr != NULL_AF_LIST){
	   dblDeleteTableInstance(dbl_LIST_AFFILIATION, 
	                          (DBlist) tree, (DBtable) afPntr);
	   afPntr = tree->afTail;
	}
	
	arPntr = tree->arTail;   
	while (arPntr != NULL_AR_LIST){
	   dblDeleteTableInstance(dbl_LIST_ARRIVAL, 
	                          (DBlist) tree, (DBtable) arPntr);
	   arPntr = tree->arTail;
	}
	
	asPntr = tree->asTail;   
	while (asPntr != NULL_AS_LIST){
	   dblDeleteTableInstance(dbl_LIST_ASSOC, 
	                          (DBlist) tree, (DBtable) asPntr);
	   asPntr = tree->asTail;
	}
	
	evPntr = tree->evTail;   
	while (evPntr != NULL_EV_LIST){
	   dblDeleteTableInstance(dbl_LIST_EVENT, 
	                          (DBlist) tree, (DBtable) evPntr);
	   evPntr = tree->evTail;
	}
	
	grPntr = tree->grTail;   
	while (grPntr != NULL_GR_LIST){
	   dblDeleteTableInstance(dbl_LIST_GREGION, 
	                          (DBlist) tree, (DBtable) grPntr);
	   grPntr = tree->grTail;
	}
	
	inPntr = tree->inTail;   
	while (inPntr != NULL_IN_LIST){
	   dblDeleteTableInstance(dbl_LIST_INSTRUMENT, 
	                          (DBlist) tree, (DBtable) inPntr);
	   inPntr = tree->inTail;
	}
	
	oePntr = tree->oeTail;   
	while (oePntr != NULL_OE_LIST){
	   dblDeleteTableInstance(dbl_LIST_ORIGERR, 
	                          (DBlist) tree, (DBtable) oePntr);
	   oePntr = tree->oeTail;
	}

	orPntr = tree->orTail;   
	while (orPntr != NULL_OR_LIST){
	   dblDeleteTableInstance(dbl_LIST_ORIGIN, 
	                          (DBlist) tree, (DBtable) orPntr);
	   orPntr = tree->orTail;
	}

	rePntr = tree->reTail;   
	while (rePntr != NULL_RE_LIST){
	   dblDeleteTableInstance(dbl_LIST_REMARK, 
	                          (DBlist) tree, (DBtable) rePntr);
	   rePntr = tree->reTail;
	}

	sePntr = tree->seTail;   
	while (sePntr != NULL_SE_LIST){
	   dblDeleteTableInstance(dbl_LIST_SENSOR, 
	                          (DBlist) tree, (DBtable) sePntr);
	   sePntr = tree->seTail;
	}

	slPntr = tree->slTail;   
	while (slPntr != NULL_SL_LIST){
	   dblDeleteTableInstance(dbl_LIST_SITE, 
	                          (DBlist) tree, (DBtable) slPntr);
	   slPntr = tree->slTail;
	}

	scPntr = tree->scTail;   
	while (scPntr != NULL_SC_LIST){
	   dblDeleteTableInstance(dbl_LIST_SITECHAN, 
	                          (DBlist) tree, (DBtable) scPntr);
	   scPntr = tree->scTail;
	}

	saPntr = tree->saTail;   
	while (saPntr != NULL_SA_LIST){
	   dblDeleteTableInstance(dbl_LIST_STASSOC, 
	                          (DBlist) tree, (DBtable) saPntr);
	   saPntr = tree->saTail;
	}

        wfPntr = tree->wfTail;
	while (wfPntr != NULL_WF_LIST){
	   dblDeleteTableInstance(dbl_LIST_WFDISC, 
	                          (DBlist) tree, (DBtable) wfPntr);
	   wfPntr = tree->wfTail;
	}

        wtPntr = tree->wtTail;
	while (wtPntr != NULL_WT_LIST){
	   dblDeleteTableInstance(dbl_LIST_WFTAG, 
	                          (DBlist) tree, (DBtable) wtPntr);
	   wtPntr = tree->wtTail;
	}

        sdPntr = tree->sdTail;
	while (sdPntr){
	   dblDeleteTableInstance(dbl_LIST_SACDATA, 
	                          (DBlist) tree, (DBtable) sdPntr);
	   sdPntr = tree->sdTail;
	}


	dblDestroyUserDataStruc(Tree);


        smFree(tree);
        Tree = NULL;	     
}
/*----------------------------------------------------------------------*/





/* This function copies the contents of origPntr to copyPntr. It assumes
   that both struct pointers have been allocated and makes no assumptions
   about their connections within a dbtree. It does not modify the next
   or prev elements of either structure. For most structs, this means that
   only the element structs are copied. However structs with associated
   data such as wfdiscList strucs will have space allocated and their data 
   copied as necessary. 
*/
void dblCopyTable(dblObject StrucType,  DBtable origPntr, DBtable copyPntr)
{

	struct affiliationList *afPntr, *copyAfPntr;
	struct arrivalList     *arPntr, *copyArPntr;
	struct assocList       *asPntr, *copyAsPntr;
	struct eventList       *evPntr, *copyEvPntr;
	struct gregionList     *grPntr, *copyGrPntr;
	struct instrumentList  *inPntr, *copyInPntr;
	struct origerrList     *oePntr, *copyOePntr;
	struct originList      *orPntr, *copyOrPntr;
	struct remarkList      *rePntr, *copyRePntr;
	struct sensorList      *sePntr, *copySePntr;
	struct siteList        *slPntr, *copySlPntr;
	struct sitechanList    *scPntr, *copyScPntr;
	struct stassocList     *saPntr, *copySaPntr;
	struct wfdiscList      *wfPntr, *copyWfPntr;	
	struct wftagList       *wtPntr, *copyWtPntr;	
	struct sacdataList     *sdPntr, *copySdPntr;	


        if(!origPntr){
           dblSetError(1, "ERROR: Empty source pointer passed to dblCopyTable.\n");
           return;
        }

        if(!copyPntr){
           dblSetError(1, "ERROR: Empty target pointer passed to dblCopyTable.\n");
           return;
        }

	switch (StrucType){
	   case dbl_LIST_AFFILIATION:{
	      afPntr = AF_LIST origPntr;
	      copyAfPntr = AF_LIST copyPntr;
	       *(copyAfPntr->element) =  *(afPntr->element);   
	      return;
	   }
	
	   case dbl_LIST_ARRIVAL:{
	      arPntr = AR_LIST origPntr;
	      copyArPntr = AR_LIST copyPntr;
	       *(copyArPntr->element) =  *(arPntr->element);   
	      return;
	   }
	
	   case dbl_LIST_ASSOC:{
	      asPntr = AS_LIST origPntr;
	      copyAsPntr = AS_LIST copyPntr;
	      *(copyAsPntr->element) = *(asPntr->element);   
	      return;
	   }
	
	   case dbl_LIST_EVENT:{
	      evPntr = EV_LIST origPntr;
	      copyEvPntr = EV_LIST copyPntr;
	       *(copyEvPntr->element) =  *(evPntr->element);   
	      return;
	   }
	
	   case dbl_LIST_GREGION:{
	      grPntr = GR_LIST origPntr;
	      copyGrPntr = GR_LIST copyPntr;
	       *(copyGrPntr->element) =  *(grPntr->element);   
	      return;
	   }
	
	   case dbl_LIST_INSTRUMENT:{
	      inPntr = IN_LIST origPntr;
	      copyInPntr = IN_LIST copyPntr;
	       *(copyInPntr->element) =  *(inPntr->element);   
	      return;
	   }
	
	   case dbl_LIST_ORIGERR:{
	      oePntr = OE_LIST origPntr;
	      copyOePntr = OE_LIST copyPntr;
	       *(copyOePntr->element) =  *(oePntr->element);   
	      return;
	   }
	
	   case dbl_LIST_ORIGIN:{
	      orPntr = OR_LIST origPntr;
	      copyOrPntr = OR_LIST copyPntr;
	       *(copyOrPntr->element) =  *(orPntr->element);   
	      return;
	   }
	
	   case dbl_LIST_REMARK:{
	      rePntr = RE_LIST origPntr;
	      copyRePntr = RE_LIST copyPntr;
	       *(copyRePntr->element) =  *(rePntr->element);   
	      return;
	   }
	
	   case dbl_LIST_SENSOR:{
	      sePntr = SE_LIST origPntr;
	      copySePntr = SE_LIST copyPntr;
	       *(copySePntr->element) =  *(sePntr->element);   
	      return;
	   }
	
	   case dbl_LIST_SITE:{
	      slPntr = SL_LIST origPntr;
	      copySlPntr = SL_LIST copyPntr;
	       *(copySlPntr->element) =  *(slPntr->element);   
	      return;
	   }
	
	   case dbl_LIST_SITECHAN:{
	      scPntr = SC_LIST origPntr;
	      copyScPntr = SC_LIST copyPntr;
	       *(copyScPntr->element) =  *(scPntr->element);   
	      return;
	   }
	
	   case dbl_LIST_STASSOC:{
	      saPntr = SA_LIST origPntr;
	      copySaPntr = SA_LIST copyPntr;
	       *(copySaPntr->element) =  *(saPntr->element);   
	      return;
	   }
	
	   case dbl_LIST_WFDISC:{
	      wfPntr = WF_LIST origPntr; 
	      copyWfPntr = WF_LIST copyPntr;
	         
	      *(copyWfPntr->element) =  *(wfPntr->element);
	      if(copyWfPntr->element->nsamp >0){
		if(copyWfPntr->seis->r){
		    smFree(copyWfPntr->seis->r);
		    copyWfPntr->seis->r = 0;
		}
		if(copyWfPntr->seis->i){
		    smFree(copyWfPntr->seis->i);
		    copyWfPntr->seis->i = 0;
		}
                 copyWfPntr->seis = dblCopyTraceData(wfPntr->seis, 
                                    wfPntr->element->nsamp);
	      }  
	      return;
	   }
	
	   case dbl_LIST_WFTAG:{
	      wtPntr = WT_LIST origPntr;
	      copyWtPntr = WT_LIST copyPntr;
	       *(copyWtPntr->element) =  *(wtPntr->element);   
	      return;
	   }
	
	   case dbl_LIST_SACDATA:{
	      sdPntr = SD_LIST origPntr;
	      copySdPntr = SD_LIST copyPntr;
	       *(copySdPntr->element) =  *(sdPntr->element);   
	      return;
	   }
	
	   default: return;
	}
}
/*----------------------------------------------------------------------*/





/* This function copies the actual CSS struct into its corresponding CSS tableList */
/* struct. Its primary use is for instances where you have obtained a CSS struct */
/* from a source outside the SeisMgr library and want to include the struct into */
/* a CSS tree. */
void dblCopyTableElement(dblObject StrucType, DBelement origPntr, DBtable copyPntr)
{

	struct affiliation *afPntr;
	struct arrival     *arPntr;
	struct assoc       *asPntr;
	struct event       *evPntr;
	struct gregion     *grPntr;
	struct instrument  *inPntr;
	struct origerr     *oePntr;
	struct origin      *orPntr;
	struct remark      *rePntr;
	struct sensor      *sePntr;
	struct site        *slPntr;
	struct sitechan    *scPntr;
	struct stassoc     *saPntr;
	struct wfdisc      *wfPntr;	
	struct wftag       *wtPntr;	
	struct sacdata     *sdPntr;	

	struct affiliationList *copyAfPntr;
	struct arrivalList     *copyArPntr;
	struct assocList       *copyAsPntr;
	struct eventList       *copyEvPntr;
	struct gregionList     *copyGrPntr;
	struct instrumentList  *copyInPntr;
	struct origerrList     *copyOePntr;
	struct originList      *copyOrPntr;
	struct remarkList      *copyRePntr;
	struct sensorList      *copySePntr;
	struct siteList        *copySlPntr;
	struct sitechanList    *copyScPntr;
	struct stassocList     *copySaPntr;
	struct wfdiscList      *copyWfPntr;	
	struct wftagList       *copyWtPntr;	
	struct sacdataList     *copySdPntr;	




        if(!origPntr){
           dblSetError(1, 
           "ERROR: Empty source pointer passed to dblCopyTableElement.\n");
           return;
        }

        if(!copyPntr){
           dblSetError(1, 
           "ERROR: Empty target pointer passed to dblCopyTableElement.\n");
           return;
        }

	switch (StrucType){
	   case dbl_LIST_AFFILIATION:{
	      afPntr = (struct affiliation *) origPntr;
	      copyAfPntr = AF_LIST copyPntr;
	       *(copyAfPntr->element) =  *(afPntr);   
	      return;
	   }
	
	   case dbl_LIST_ARRIVAL:{
	      arPntr = (struct arrival *) origPntr;
	      copyArPntr = AR_LIST copyPntr;
	       *(copyArPntr->element) =  *(arPntr);   
	      return;
	   }
	
	   case dbl_LIST_ASSOC:{
	      asPntr = (struct assoc *) origPntr;
	      copyAsPntr = AS_LIST copyPntr;
	      *(copyAsPntr->element) = *(asPntr);   
	      return;
	   } 
	
	   case dbl_LIST_EVENT:{
	      evPntr = (struct event *) origPntr;
	      copyEvPntr = EV_LIST copyPntr;
	       *(copyEvPntr->element) =  *(evPntr);   
	      return;
	   }
	
	   case dbl_LIST_GREGION:{
	      grPntr = (struct gregion *) origPntr;
	      copyGrPntr = GR_LIST copyPntr;
	       *(copyGrPntr->element) =  *(grPntr);   
	      return;
	   }
	
	   case dbl_LIST_INSTRUMENT:{
	      inPntr = (struct instrument *) origPntr;
	      copyInPntr = IN_LIST copyPntr;
	       *(copyInPntr->element) =  *(inPntr);   
	      return;
	   }
	
	   case dbl_LIST_ORIGERR:{
	      oePntr = (struct origerr *) origPntr;
	      copyOePntr = OE_LIST copyPntr;
	       *(copyOePntr->element) =  *(oePntr);   
	      return;
	   }
	
	   case dbl_LIST_ORIGIN:{
	      orPntr = (struct origin *) origPntr;
	      copyOrPntr = OR_LIST copyPntr;
	       *(copyOrPntr->element) =  *(orPntr);   
	      return;
	   }
	
	   case dbl_LIST_REMARK:{
	      rePntr = (struct remark *) origPntr;
	      copyRePntr = RE_LIST copyPntr;
	       *(copyRePntr->element) =  *(rePntr);   
	      return;
	   }
	
	   case dbl_LIST_SENSOR:{
	      sePntr = (struct sensor *) origPntr;
	      copySePntr = SE_LIST copyPntr;
	       *(copySePntr->element) =  *(sePntr);   
	      return;
	   }
	
	   case dbl_LIST_SITE:{
	      slPntr = (struct site *) origPntr;
	      copySlPntr = SL_LIST copyPntr;
	       *(copySlPntr->element) =  *(slPntr);   
	      return;
	   }
	
	   case dbl_LIST_SITECHAN:{
	      scPntr = (struct sitechan *) origPntr;
	      copyScPntr = SC_LIST copyPntr;
	       *(copyScPntr->element) =  *(scPntr);   
	      return;
	   }
	
	   case dbl_LIST_STASSOC:{
	      saPntr = (struct stassoc *) origPntr;
	      copySaPntr = SA_LIST copyPntr;
	       *(copySaPntr->element) =  *(saPntr);   
	      return;
	   }
	
	   case dbl_LIST_WFDISC:{
	      wfPntr = (struct wfdisc *) origPntr; 
	      copyWfPntr = WF_LIST copyPntr;
	      *(copyWfPntr->element) =  *(wfPntr);
	      return;
	   }
	
	   case dbl_LIST_WFTAG:{
	      wtPntr = (struct wftag *) origPntr; 
	      copyWtPntr = WT_LIST copyPntr;
	      *(copyWtPntr->element) =  *(wtPntr);
	      return;
	   }
	
	   case dbl_LIST_SACDATA:{
	      sdPntr = (struct sacdata *) origPntr; 
	      copySdPntr = SD_LIST copyPntr;
	      *(copySdPntr->element) =  *(sdPntr);
	      return;
	   }
	
	   default: return;
	}
}
/*----------------------------------------------------------------------*/






/* This function copies the CSS tree pointed to by OldTree into a new tree struct and */
/* returns a pointer to that struct. */
DBlist dblCopyTree(DBlist OldTree)
{
	struct affiliationList *afPntr, *newAfPntr;
	struct arrivalList     *arPntr, *newArPntr;
	struct assocList       *asPntr, *newAsPntr;
	struct eventList       *evPntr, *newEvPntr;
	struct gregionList     *grPntr, *newGrPntr;
	struct instrumentList  *inPntr, *newInPntr;
	struct origerrList     *oePntr, *newOePntr;
	struct originList      *orPntr, *newOrPntr;
	struct remarkList      *rePntr, *newRePntr;
	struct sensorList      *sePntr, *newSePntr;
	struct siteList        *slPntr, *newSlPntr;
	struct sitechanList    *scPntr, *newScPntr;
	struct stassocList     *saPntr, *newSaPntr;
	struct wfdiscList      *wfPntr, *newWfPntr;	
	struct wftagList       *wtPntr, *newWtPntr;	
	struct sacdataList     *sdPntr, *newSdPntr;	

	struct CSStree *oldTree;	
	struct CSStree *newTree;

	DataComment *DCpntr;
	DataComment *newDCpntr;
	int Nrows, Ncols;
	
	oldTree = (struct CSStree *) OldTree;

/* Make sure oldTree is not empty */	
   	if(oldTree == (struct CSStree *) NULL)
           return (DBlist) NULL;

/* initialize a new tree */
	newTree = (struct CSStree *) dblNewTree();
        if(!newTree){
           dblSetError(1, "ERROR: Empty newTree pointer in dblCopyTree.\n");
           return (DBlist) NULL;
        }

/* copy the affiliation list */	      
      	afPntr = oldTree->afHead;
	while (afPntr != NULL_AF_LIST){
           newAfPntr = (struct affiliationList *) dblCreateTableInstance(newTree, dbl_LIST_AFFILIATION);
	   dblCopyTable(dbl_LIST_AFFILIATION, (DBtable) afPntr, (DBtable) newAfPntr);
	   afPntr = afPntr->next;
	}

/* copy the arrival list */	      
      	arPntr = oldTree->arHead;
	while (arPntr != NULL_AR_LIST){
           newArPntr = (struct arrivalList*) dblCreateTableInstance(newTree, dbl_LIST_ARRIVAL);
	   dblCopyTable(dbl_LIST_ARRIVAL, (DBtable) arPntr, (DBtable) newArPntr);
	   arPntr = arPntr->next;
	}

/* copy the assoc list */	      
      	asPntr = oldTree->asHead;
	while (asPntr != NULL_AS_LIST){
           newAsPntr = (struct assocList*) dblCreateTableInstance(newTree, dbl_LIST_ASSOC);
	   dblCopyTable(dbl_LIST_ASSOC, (DBtable) asPntr, (DBtable) newAsPntr);
	   asPntr = asPntr->next;
	}

/* copy the event list */	      
      	evPntr = oldTree->evHead;
	while (evPntr != NULL_EV_LIST){
           newEvPntr = (struct eventList*) dblCreateTableInstance(newTree, dbl_LIST_EVENT);
	   dblCopyTable(dbl_LIST_EVENT, (DBtable) evPntr, (DBtable) newEvPntr);
	   evPntr = evPntr->next;
	}

/* copy the gregion list */	      
      	grPntr = oldTree->grHead;
	while (grPntr != NULL_GR_LIST){
           newGrPntr = (struct gregionList*) dblCreateTableInstance(newTree, dbl_LIST_GREGION);
	   dblCopyTable(dbl_LIST_GREGION, (DBtable) grPntr, (DBtable) newGrPntr);
	   grPntr = grPntr->next;
	}

/* copy the instrument list */	      
      	inPntr = oldTree->inHead;
	while (inPntr != NULL_IN_LIST){
           newInPntr = (struct instrumentList*) dblCreateTableInstance(newTree, dbl_LIST_INSTRUMENT);
	   dblCopyTable(dbl_LIST_INSTRUMENT, (DBtable) inPntr, (DBtable) newInPntr);
	   inPntr = inPntr->next;
	}

/* copy the origerr list */	      
      	oePntr = oldTree->oeHead;
	while (oePntr != NULL_OE_LIST){
           newOePntr = (struct origerrList*) dblCreateTableInstance(newTree, dbl_LIST_ORIGERR);
	   dblCopyTable(dbl_LIST_ORIGERR, (DBtable) oePntr, (DBtable) newOePntr);
	   oePntr = oePntr->next;
	}

/* copy the origin list */	      
      	orPntr = oldTree->orHead;
	while (orPntr != NULL_OR_LIST){
           newOrPntr = (struct originList*) dblCreateTableInstance(newTree, dbl_LIST_ORIGIN);
	   dblCopyTable(dbl_LIST_ORIGIN, (DBtable) orPntr, (DBtable) newOrPntr);
	   orPntr = orPntr->next;
	}


/* copy the remark list */	      
      	rePntr = oldTree->reHead;
	while (rePntr != NULL_RE_LIST){
           newRePntr = (struct remarkList*) dblCreateTableInstance(newTree, dbl_LIST_REMARK);
	   dblCopyTable(dbl_LIST_REMARK, (DBtable) rePntr, (DBtable) newRePntr);
	   rePntr = rePntr->next;
	}

/* copy the sensor list */	      
      	sePntr = oldTree->seHead;
	while (sePntr != NULL_SE_LIST){
           newSePntr = (struct sensorList*) dblCreateTableInstance(newTree, dbl_LIST_SENSOR);
	   dblCopyTable(dbl_LIST_SENSOR, (DBtable) sePntr, (DBtable) newSePntr);
	   sePntr = sePntr->next;
	}

/* copy the site list */	      
      	slPntr = oldTree->slHead;
	while (slPntr != NULL_SL_LIST){
           newSlPntr = (struct siteList*) dblCreateTableInstance(newTree, dbl_LIST_SITE);
	   dblCopyTable(dbl_LIST_SITE, (DBtable) slPntr, (DBtable) newSlPntr);
	   slPntr = slPntr->next;
	}

/* copy the sitechan list */	      
      	scPntr = oldTree->scHead;
	while (scPntr != NULL_SC_LIST){
           newScPntr = (struct sitechanList*) dblCreateTableInstance(newTree, dbl_LIST_SITECHAN);
	   dblCopyTable(dbl_LIST_SITECHAN, (DBtable) scPntr, (DBtable) newScPntr);
	   scPntr = scPntr->next;
	}

/* copy the stassoc list */	      
      	saPntr = oldTree->saHead;
	while (saPntr != NULL_SA_LIST){
           newSaPntr = (struct stassocList*) dblCreateTableInstance(newTree, dbl_LIST_STASSOC);
	   dblCopyTable(dbl_LIST_STASSOC, (DBtable) saPntr, (DBtable) newSaPntr);
	   saPntr = saPntr->next;
	}

/* copy the wfdisc list */	      
      	wfPntr = oldTree->wfHead;
	while (wfPntr != NULL_WF_LIST){
           newWfPntr = (struct wfdiscList*) dblCreateTableInstance(newTree, dbl_LIST_WFDISC);
	   dblCopyTable(dbl_LIST_WFDISC, (DBtable) wfPntr, (DBtable) newWfPntr);
	   wfPntr = wfPntr->next;
	}

/* copy the wftag list */	      
      	wtPntr = oldTree->wtHead;
	while (wtPntr != NULL_WT_LIST){
           newWtPntr = (struct wftagList*) dblCreateTableInstance(newTree, dbl_LIST_WFTAG);
	   dblCopyTable(dbl_LIST_WFTAG, (DBtable) wtPntr, (DBtable) newWtPntr);
	   wtPntr = wtPntr->next;
	}

/* copy the sacdata list */	      
      	sdPntr = oldTree->sdHead;
	while (sdPntr){
           newSdPntr = (struct sacdataList*) dblCreateTableInstance(newTree, dbl_LIST_SACDATA);
	   dblCopyTable(dbl_LIST_SACDATA, (DBtable) sdPntr, (DBtable) newSdPntr);
	   sdPntr = sdPntr->next;
	}


/* copy the UserData structure */

   if(oldTree->UserData){
      dblCreateUserDataStruc(newTree);
      if(oldTree->UserData->comment){
	 dblSetUserDataComment(newTree, oldTree->UserData->comment);
      }
      if(oldTree->UserData->matrix->comment){
	 dblSetUserDataMatrixComment(newTree, oldTree->UserData->matrix->comment);
      }
      if(oldTree->UserData->matrix->matrix){
	 Nrows = oldTree->UserData->matrix->nrows;
	 Ncols = oldTree->UserData->matrix->ncols;
	 dblSetUserDataMatrixData(newTree, Nrows, Ncols, 
			      oldTree->UserData->matrix->matrix);
      }

      DCpntr = (struct DataPlusComment*) dblGetNextDataComment(0,oldTree);
      while(DCpntr){
	 newDCpntr = (struct DataPlusComment*) dblCreateDataComment(newTree);
	 dblSetDCIndex(newDCpntr, ((DataComment *) DCpntr)->reference);
	 if(DCpntr->comment){
            dblSetUserDataComComment(newDCpntr, DCpntr->comment);
	 }
	 if(DCpntr->data){
	    dblSetUserDataComData(newDCpntr, DCpntr->data, DCpntr->dataLen);
	 }
         DCpntr = (struct DataPlusComment*) dblGetNextDataComment(DCpntr, oldTree);
      }
   }
   return (DBlist) newTree;
}
/*----------------------------------------------------------------------*/







/* This function returns a pointer to a list struct in the tree pointed to by targetObj */
/* of the type specified by specifier, and with index matching index. */
DBtable dblGetTableInstance(DBobj targetObj, dblObject specifier, int index)
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
   struct CSStree *tree;

   tree = (struct CSStree *) targetObj;
   if(!tree){
      dblSetError(1, "ERROR: Empty tree pointer passed to  dblGetTableInstance.\n");
      return (DBlist) NULL;
   }

   switch (specifier){
   case dbl_LIST_AFFILIATION:{
      afPntr = tree->afHead;
      while(afPntr != NULL_AF_LIST){
         if(index == afPntr->index)return (DBtable) afPntr;
	 afPntr = afPntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_ARRIVAL:{
      arPntr = tree->arHead;
      while(arPntr != NULL_AR_LIST){
         if(index == arPntr->index)return (DBtable) arPntr;
	 arPntr = arPntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_ASSOC:{
      asPntr = tree->asHead;
      while(asPntr != NULL_AS_LIST){
         if(index == asPntr->index)return (DBtable) asPntr;
	 asPntr = asPntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_EVENT:{
      evPntr = tree->evHead;
      while(evPntr != NULL_EV_LIST){
         if(index == evPntr->index)return (DBtable) evPntr;
	 evPntr = evPntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_GREGION:{
      grPntr = tree->grHead;
      while(grPntr != NULL_GR_LIST){
         if(index == grPntr->index)return (DBtable) grPntr;
	 grPntr = grPntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_INSTRUMENT:{
      inPntr = tree->inHead;
      while(inPntr != NULL_IN_LIST){
         if(index == inPntr->index)return (DBtable) inPntr;
	 inPntr = inPntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_ORIGERR:{
      oePntr = tree->oeHead;
      while(oePntr != NULL_OE_LIST){
         if(index == oePntr->index)return (DBtable) oePntr;
	 oePntr = oePntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_ORIGIN:{
      orPntr = tree->orHead;
      while(orPntr != NULL_OR_LIST){
         if(index == orPntr->index)return (DBtable) orPntr;
	 orPntr = orPntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_REMARK:{
      rePntr = tree->reHead;
      while(rePntr != NULL_RE_LIST){
         if(index == rePntr->index)return (DBtable) rePntr;
	 rePntr = rePntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_SENSOR:{
      sePntr = tree->seHead;
      while(sePntr != NULL_SE_LIST){
         if(index == sePntr->index)return (DBtable) sePntr;
	 sePntr = sePntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_SITE:{
      slPntr = tree->slHead;
      while(slPntr != NULL_SL_LIST){
         if(index == slPntr->index)return (DBtable) slPntr;
	 slPntr = slPntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_SITECHAN:{
      scPntr = tree->scHead;
      while(scPntr != NULL_SC_LIST){
         if(index == scPntr->index)return (DBtable) scPntr;
	 scPntr = scPntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_STASSOC:{
      saPntr = tree->saHead;
      while(saPntr != NULL_SA_LIST){
         if(index == saPntr->index)return (DBtable) saPntr;
	 saPntr = saPntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_WFDISC:{
      wfPntr = tree->wfHead;
      while(wfPntr != NULL_WF_LIST){
         if(index == wfPntr->index)return (DBtable) wfPntr;
	 wfPntr = wfPntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_WFTAG:{
      wtPntr = tree->wtHead;
      while(wtPntr != NULL_WT_LIST){
         if(index == wtPntr->index)return (DBtable) wtPntr;
	 wtPntr = wtPntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
   }

   case dbl_LIST_SACDATA:{
      sdPntr = tree->sdHead;
      while(sdPntr){
         if(index == sdPntr->index)return (DBtable) sdPntr;
	 sdPntr = sdPntr->next;
      }
      dblSetError(1, "ERROR: Invalid table index in dblGetTableInstance.\n");
      return 0;
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

   return 0;
}
/*----------------------------------------------------------------------*/





/* This function returns pointers to specific (usually scalar) objects in the list struct */
/* pointed to by targetObj. The object returned is determined by the value of specifier */
/* which is a member of the enumeration defined in dblPublicDefs.h. All returned pointers */
/* are cast to void *, and should be cast to the proper type before dereferencing. This  */
/* function can be used to retrieve pointers to trace data by setting targetObj to point */
/* to a wfdiscList struct and specifying one of the keywords: dbl_WFDIS_REAL_TRC, or */
/* dbl_WFDIS_IMAG_TRC. In that case the result should be cast to float *. */
DBobj dblGetTableObject(DBobj targetObj, dblObject specifier)
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



   if(!targetObj){
      dblSetError(1, "ERROR: Empty struct pointer passed  to dblGetTableObject.\n");
      return (void *)NULL;
   }
   
   switch (specifier){
/* Return attributes of the affiliation relation */
	case dbl_AFFI_NET:{	  /* (affiliation) Network Identifier (attribute) */
	   afPntr = (struct affiliationList *) targetObj;
 	   return (DBresult) &(afPntr->element->net);
	}
	case dbl_AFFI_STA:{	  /* (affiliation) Station Code (attribute) */
	   afPntr = (struct affiliationList *) targetObj;
 	   return (DBresult) &(afPntr->element->sta); 
	}
	case dbl_AFFI_LDDATE:{	  /* (affiliation) Load Date (attribute) */
	   afPntr = (struct affiliationList *) targetObj;
 	   return (DBresult) &(afPntr->element->lddate); 
	}
	   
/* Return attributes of the arrival relation */
	case dbl_ARRIV_AMP:{	  /* (arrival) signal amplitude (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->amp);
	}
	case dbl_ARRIV_ARID:{		/* (arrival) Arrival identifier (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->arid);
	}
	case dbl_ARRIV_AUTH:{		/* (arrival) Author (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) arPntr->element->auth;
	}
	case dbl_ARRIV_AZIMUTH:{	/* (arrival) Observed Azimuth (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->azimuth);
	}
	case dbl_ARRIV_CHAN:{		/* (arrival) channel identifier (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) arPntr->element->chan;
	}
	case dbl_ARRIV_CLIP:{		/* (arrival) clipped data flag (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) arPntr->element->clip;
	}
	case dbl_ARRIV_COMMID:{		/* (arrival) comment ID (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->commid);
	}
	case dbl_ARRIV_CHANID:{		/* (arrival) Chan Recording ID (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->chanid);
	}
	case dbl_ARRIV_DELAZ:{		/* (arrival) Delta Azimuth (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->delaz);
	}
	case dbl_ARRIV_DELSLO:{		/* (arrival) Delta Slowness (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->delslo);
	}
	case dbl_ARRIV_EMA:{		/* (arrival) Emergence Angle (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->ema);
	}
	case dbl_ARRIV_DELTIM:{		/* (arrival) Delta time (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->deltim);
	}
	case dbl_ARRIV_FM:{		/* (arrival) First Motion (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) arPntr->element->fm;
	}
	case dbl_ARRIV_IPHASE:{		/* (arrival) Reported Phase (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) arPntr->element->iphase;
	}
	case dbl_ARRIV_JDATE:{		/* (arrival) Julian Date (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->jdate);
	}
	case dbl_ARRIV_LOGAT:{		/* (arrival) Log(amplitude)/period (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->logat);
	}
	case dbl_ARRIV_PER:{		/* (arrival) Signal Period (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->per);
	}
	case dbl_ARRIV_QUAL:{		/* (arrival) Onset Quality (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) arPntr->element->qual;
	}
	case dbl_ARRIV_RECT:{		/* (arrival) Rectilinearity (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->rect);
	}
	case dbl_ARRIV_SLOW:{		/* (arrival) Observed Slowness (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->slow);
	}
	case dbl_ARRIV_SNR:{		/* (arrival) Signal-noise Ratio (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->snr);
	}
	case dbl_ARRIV_STA:{		/* (arrival) Station Code (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) arPntr->element->sta;
	}
	case dbl_ARRIV_STASSID:{	/* (arrival) Sta-assoc ID (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->stassid);
	}
	case dbl_ARRIV_STYPE:{		/* (arrival) Signal Type (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) arPntr->element->stype;
	}
	case dbl_ARRIV_TIME:{		/* (arrival) Epoch Time (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) &(arPntr->element->time);
	}
	case dbl_ARRIV_LDDATE:{		/* (arrival) Load Date (attribute) */
	   arPntr = (struct arrivalList *) targetObj;
	   return (DBresult) arPntr->element->lddate;
	}
	   
	case dbl_ASSOC_ARID:{		/* (assoc) Arrival identifier (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) &(asPntr->element->arid);
	}
	case dbl_ASSOC_AZDEF:{		/* (assoc) Azimuth defining code (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) asPntr->element->azdef;
	}
	case dbl_ASSOC_AZRES:{		/* (assoc) Azimuth residual (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) &(asPntr->element->azres);
	}
	case dbl_ASSOC_BELIEF:{		/* (assoc) Phase ID confidence (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) &(asPntr->element->belief);
	}
	case dbl_ASSOC_COMMID:{		/* (assoc) comment ID (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) &(asPntr->element->commid);
	}
	case dbl_ASSOC_DELTA:{		/* (assoc) Src-Receiver dist (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) &(asPntr->element->delta);
	}
	case dbl_ASSOC_EMARES:{		/* (assoc) Emergence Angle Residual (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) &(asPntr->element->emares);
	}
	case dbl_ASSOC_ESAZ:{		/* (assoc) Event-station azimuth (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) &(asPntr->element->esaz);
	}
	case dbl_ASSOC_ORID:{		/* (assoc) Origin Identification (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) &(asPntr->element->orid);
	}
	case dbl_ASSOC_PHASE:{		/* (assoc) Associated Phase (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) asPntr->element->phase;
	}
	case dbl_ASSOC_SEAZ:{		/* (assoc) Sta-Event Azimuth (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) &(asPntr->element->seaz);
	}
	case dbl_ASSOC_SLODEF:{		/* (assoc) Slowness Defining code (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) asPntr->element->slodef;
	}
	case dbl_ASSOC_SLORES:{		/* (assoc) Slowness Residual (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) &(asPntr->element->slores);
	}
	case dbl_ASSOC_STA:{		/* (assoc) Station Code (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) asPntr->element->sta;
	}
	case dbl_ASSOC_TIMEDEF:{	/* (assoc) Time-defining Code (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) asPntr->element->timedef;
	}
	case dbl_ASSOC_TIMERES:{	/* (assoc) Time Residual (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) &(asPntr->element->timeres);
	}
	case dbl_ASSOC_VMODEL:{		/* (assoc) Velocity Model (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) asPntr->element->vmodel;
	}
	case dbl_ASSOC_WGT:{		/* (assoc) Location Weight (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) &(asPntr->element->wgt);
	}
	case dbl_ASSOC_LDDATE:{		/* (assoc) Load Date (attribute) */
	   asPntr = (struct assocList *) targetObj;
	   return (DBresult) asPntr->element->lddate;
	}

	case dbl_EVENT_AUTH:{		/* (event) Author (attribute) */
	   evPntr = (struct eventList *) targetObj;
 	   return (DBresult) evPntr->element->auth;
	}
	case dbl_EVENT_COMMID:{		/* (event) comment ID (attribute) */
	   evPntr = (struct eventList *) targetObj;
 	   return (DBresult) &(evPntr->element->commid);
	}
	case dbl_EVENT_EVID:{		/* (event) Event Identifier (attribute) */
	   evPntr = (struct eventList *) targetObj;
 	   return (DBresult) &(evPntr->element->evid);
	}
	case dbl_EVENT_EVNAME:{		/* (event) Event Name (attribute) */
	   evPntr = (struct eventList *) targetObj;
 	   return (DBresult) evPntr->element->evname;
	}
	case dbl_EVENT_PREFOR:{		/* (event) Preferred Origin (attribute) */
	   evPntr = (struct eventList *) targetObj;
 	   return (DBresult) &(evPntr->element->prefor);
	}
	case dbl_EVENT_LDDATE:{		/* (event) Load Date (attribute) */
	   evPntr = (struct eventList *) targetObj;
 	   return (DBresult) evPntr->element->lddate;
	}

	case dbl_GR_GRN:{		/* (gregion) Geographic region number (attribute) */
	   grPntr = (struct gregionList *) targetObj;
 	   return (DBresult) &(grPntr->element->grn);
	}
	case dbl_GR_GRNAME:{		/* (gregion) Geographic region name (attribute) */
	   grPntr = (struct gregionList *) targetObj;
 	   return (DBresult) grPntr->element->grname;
	}
	case dbl_GR_LDDATE:{		/* (gregion) Load Date (attribute) */
	   grPntr = (struct gregionList *) targetObj;
 	   return (DBresult) grPntr->element->lddate;
	}

	case dbl_INS_BAND:{		/* (instrument) Frequency Band (attribute) */
	   inPntr = (struct instrumentList *) targetObj;
 	   return (DBresult) inPntr->element->band;
	}
	case dbl_INS_DFILE:{	      	/* (instrument) Data file (attribute) */
	   inPntr = (struct instrumentList *) targetObj;
 	   return (DBresult) inPntr->element->dfile;
	}
	case dbl_INS_DIGITAL:{		/* (instrument) Digital/Analog flag (attribute) */
	   inPntr = (struct instrumentList *) targetObj;
 	   return (DBresult) inPntr->element->digital;
	}
	case dbl_INS_DIR:{		/* (instrument) directory (attribute) */
	   inPntr = (struct instrumentList *) targetObj;
 	   return (DBresult) inPntr->element->dir;
	}
	case dbl_INS_INID:{		/* (instrument) Instrument Identifier (attribute) */
	   inPntr = (struct instrumentList *) targetObj;
 	   return (DBresult) &(inPntr->element->inid);
	}
	case dbl_INS_INSNAME:{		/* (instrument) Instrument Name (attribute) */
	   inPntr = (struct instrumentList *) targetObj;
 	   return (DBresult) inPntr->element->insname;
	}
	case dbl_INS_INSTYPE:{		/* (instrument) Instrument Type (attribute) */
	   inPntr = (struct instrumentList *) targetObj;
 	   return (DBresult) inPntr->element->instype;
	}
	case dbl_INS_NCALIB:{		/* (instrument) Nominal Calib. Factor (attribute) */
	   inPntr = (struct instrumentList *) targetObj;
 	   return (DBresult) &(inPntr->element->ncalib);
	}
	case dbl_INS_NCALPER:{		/* (instrument) Calibration Period (attribute) */
	   inPntr = (struct instrumentList *) targetObj;
 	   return (DBresult) &(inPntr->element->ncalper);
	}
	case dbl_INS_RSPTYPE:{		/* (instrument) Response Type (attribute) */
	   inPntr = (struct instrumentList *) targetObj;
 	   return (DBresult) inPntr->element->rsptype;
	}
	case dbl_INS_SAMPRATE:{		/* (instrument) Sampling Rate (attribute) */
	   inPntr = (struct instrumentList *) targetObj;
 	   return (DBresult) &(inPntr->element->samprate);
	}
	case dbl_INS_LDDATE:{		/* (instrument) Load Date (attribute) */
	   inPntr = (struct instrumentList *) targetObj;
 	   return (DBresult) inPntr->element->lddate;
	}

	case dbl_ORIGE_COMMID:{		/* (origerr) comment ID (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->commid);
	}
	case dbl_ORIGE_CONF:{		/* (origerr) error confidence (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->conf);
	}
	case dbl_ORIGE_ORID:{		/* (origerr) Origin Identification (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->orid);
	}
	case dbl_ORIGE_SDEPTH:{		/* (origerr) Depth Error (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->sdepth);
	}
	case dbl_ORIGE_SDOBS:{		/* (origerr) Std. Error of Observation (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->sdobs);
	}
	case dbl_ORIGE_SMAJAX:{		/* (origerr) Semi-major axis of Err. ellipse (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->smajax);
	}
	case dbl_ORIGE_SMINAX:{		/* (origerr) Semi-minor axis of Err. ellipse (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->sminax);
	}
	case dbl_ORIGE_STIME:{		/* (origerr) Origin Time error (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->stime);
	}
	case dbl_ORIGE_STRIKE:{		/* (origerr) Strike of major axis of ellipse (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->strike);
	}
	case dbl_ORIGE_STX:{		/* (origerr) Element of Covariance matrix (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->stx);
	}
	case dbl_ORIGE_STY:{		/* (origerr) Element of Covariance matrix (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->sty);
	}
	case dbl_ORIGE_STZ:{		/* (origerr) Element of Covariance matrix (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->stz);
	}
	case dbl_ORIGE_SXX:{		/* (origerr) Element of Covariance matrix (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->sxx);
	}
	case dbl_ORIGE_SXY:{		/* (origerr) Element of Covariance matrix (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->sxy);
	}
	case dbl_ORIGE_SXZ:{		/* (origerr) Element of Covariance matrix (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->sxz);
	}
	case dbl_ORIGE_SYY:{		/* (origerr) Element of Covariance matrix (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->syy);
	}
	case dbl_ORIGE_SYZ:{		/* (origerr) Element of Covariance matrix (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->syz);
	}
	case dbl_ORIGE_STT:{		/* (origerr) Element of Covariance matrix (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->stt);
	}
	case dbl_ORIGE_SZZ:{		/* (origerr) Element of Covariance matrix (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) &(oePntr->element->szz);
	}
	case dbl_ORIGE_LDDATE:{		/* (origerr) Load Date (attribute) */
	   oePntr = (struct origerrList *) targetObj;
 	   return (DBresult) oePntr->element->lddate;
	}

	case dbl_ORIGI_ALGORITHM:{	/* (origin) location algorithm (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) orPntr->element->algorithm;
	}
	case dbl_ORIGI_AUTH:{		/* (origin) Author (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) orPntr->element->auth;
	}
	case dbl_ORIGI_COMMID:{		/* (origin) comment ID (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->commid);
	}
	case dbl_ORIGI_DEPDP:{		/* (origin) Depth from phases (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->depdp);
	}
	case dbl_ORIGI_DEPTH:{		/* (origin) Source Depth (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->depth);
	}
	case dbl_ORIGI_DTYPE:{		/* (origin) Depth Determination flag (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) orPntr->element->dtype;
	}
	case dbl_ORIGI_ETYPE:{		/* (origin) Event type (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) orPntr->element->etype;
	}
	case dbl_ORIGI_EVID:{		/* (origin) Event Identifier (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->evid);
	}
	case dbl_ORIGI_GRN:{		/* (origin) Geographic region number (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->grn);
	}
	case dbl_ORIGI_JDATE:{		/* (origin) Julian Date (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->jdate);
	}
	case dbl_ORIGI_LAT:{		/* (origin) Latitude (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->lat);
	}
	case dbl_ORIGI_LON:{		/* (origin) Longitude (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->lon);
	}
	case dbl_ORIGI_MB:{		/* (origin) Body Wave Mag. (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->mb);
	}
	case dbl_ORIGI_MBID:{		/* (origin) Mag. ID for Mb (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->mbid);
	}
	case dbl_ORIGI_ML:{		/* (origin) Local Magnitude (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->ml);
	}
	case dbl_ORIGI_MLID:{		/* (origin) Mad. ID for Ml (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->mlid);
	}
	case dbl_ORIGI_MS:{		/* (origin) Surface Wave Mag. (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->ms);
	}
	case dbl_ORIGI_MSID:{		/* (origin) Mag. ID for MS (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->msid);
	}
	case dbl_ORIGI_NASS:{		/* (origin) Number of Associated Arrivals (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->nass);
	}
	case dbl_ORIGI_NDEF:{		/* (origin) Number of arrival used (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->ndef);
	}
	case dbl_ORIGI_NDP:{		/* (origin) Number of Depth Phases (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->ndp);
	}
	case dbl_ORIGI_ORID:{		/* (origin) Origin Identification (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->orid);
	}
	case dbl_ORIGI_SRN:{		/* (origin) Region Number (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->srn);
	}
	case dbl_ORIGI_TIME:{		/* (origin) Epoch Time (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) &(orPntr->element->time);
	}
	case dbl_ORIGI_LDDATE:{		/* (origin) Load Date (attribute) */
	   orPntr = (struct originList *) targetObj;
 	   return (DBresult) orPntr->element->lddate;
	}

	case dbl_REMA_COMMID:{		/* (remark) comment ID (attribute) */
	   rePntr = (struct remarkList *) targetObj;
 	   return (DBresult) &(rePntr->element->commid);
	}
	case dbl_REMA_LINENO:{		/* (remark) comment line number (attribute) */
	   rePntr = (struct remarkList *) targetObj;
 	   return (DBresult) &(rePntr->element->lineno);
	}
	case dbl_REMA_REMARK:{		/* (remark) comment string (attribute) */
	   rePntr = (struct remarkList *) targetObj;
 	   return (DBresult) rePntr->element->remark;
	}
	case dbl_REMA_LDDATE:{		/* (remark) load date (attribute) */
	   rePntr = (struct remarkList *) targetObj;
 	   return (DBresult) rePntr->element->lddate;
	}

	
	case dbl_SENS_CALPER:{		/* (sensor) Calibration Period (attribute) */
	   sePntr = (struct sensorList *) targetObj;
 	   return (DBresult) &(sePntr->element->calper);
	}
	case dbl_SENS_CALRATIO:{	/* (sensor) Calib conversion ratio (attribute) */
	   sePntr = (struct sensorList *) targetObj;
 	   return (DBresult) &(sePntr->element->calratio);
	}
	case dbl_SENS_CHAN:{		/* (sensor) channel identifier (attribute) */
	   sePntr = (struct sensorList *) targetObj;
 	   return (DBresult) sePntr->element->chan;
	}
	case dbl_SENS_CHANID:{		/* (sensor) Chan Recording ID (attribute) */
	   sePntr = (struct sensorList *) targetObj;
 	   return (DBresult) &(sePntr->element->chanid);
	}
	case dbl_SENS_ENDTIME:{		/* (sensor) Time of last datum (attribute) */
	   sePntr = (struct sensorList *) targetObj;
 	   return (DBresult) &(sePntr->element->endtime);
	}
	case dbl_SENS_INID:{		/* (sensor) Instrument Identifier (attribute) */
	   sePntr = (struct sensorList *) targetObj;
 	   return (DBresult) &(sePntr->element->inid);
	}
	case dbl_SENS_INSTANT:{		/* (sensor) Snapshot indicator (attribute) */
	   sePntr = (struct sensorList *) targetObj;
 	   return (DBresult) sePntr->element->instant;
	}
	case dbl_SENS_JDATE:{		/* (sensor) Julian Date (attribute) */
	   sePntr = (struct sensorList *) targetObj;
 	   return (DBresult) &(sePntr->element->jdate);
	}
	case dbl_SENS_STA:{		/* (sensor) Station Code (attribute) */
	   sePntr = (struct sensorList *) targetObj;
 	   return (DBresult) sePntr->element->sta;
	}
	case dbl_SENS_TIME:{		/* (sensor) Epoch Time (attribute) */
	   sePntr = (struct sensorList *) targetObj;
 	   return (DBresult) &(sePntr->element->time);
	}
	case dbl_SENS_TSHIFT:{		/* (sensor) Correction for Clock Errors (attribute) */
	   sePntr = (struct sensorList *) targetObj;
 	   return (DBresult) &(sePntr->element->tshift);
	}
	case dbl_SENS_LDDATE:{		/* (sensor) Load Date (attribute) */
	   sePntr = (struct sensorList *) targetObj;
 	   return (DBresult) sePntr->element->lddate;
	}

	case dbl_SITE_DEAST:{		/* (site) Distance East (attribute) */
	   slPntr = (struct siteList *) targetObj;
 	   return (DBresult) &(slPntr->element->deast);
	}
	case dbl_SITE_DNORTH:{		/* (site) Distance North (attribute) */
	   slPntr = (struct siteList *) targetObj;
 	   return (DBresult) &(slPntr->element->dnorth);
	}
	case dbl_SITE_ELEV:{		/* (site) Elevation (attribute) */
	   slPntr = (struct siteList *) targetObj;
 	   return (DBresult) &(slPntr->element->elev);
	}
	case dbl_SITE_LAT:{		/* (site) Latitude (attribute) */
	   slPntr = (struct siteList *) targetObj;
 	   return (DBresult) &(slPntr->element->lat);
	}
	case dbl_SITE_LON:{		/* (site) Longitude (attribute) */
	   slPntr = (struct siteList *) targetObj;
 	   return (DBresult) &(slPntr->element->lon);
	}
	case dbl_SITE_OFFDATE:{		/* (site) Turn-off Date (attribute) */
	   slPntr = (struct siteList *) targetObj;
 	   return (DBresult) &(slPntr->element->offdate);
	}
	case dbl_SITE_ONDATE:{		/* (site) Turn-on Date (attribute) */
	   slPntr = (struct siteList *) targetObj;
 	   return (DBresult) &(slPntr->element->ondate);
	}
	case dbl_SITE_REFSTA:{		/* (site) Reference Station (attribute) */
	   slPntr = (struct siteList *) targetObj;
 	   return (DBresult) slPntr->element->refsta;
	}
	case dbl_SITE_STA:{		/* (site) Station Code (attribute) */
	   slPntr = (struct siteList *) targetObj;
 	   return (DBresult) slPntr->element->sta;
	}
	case dbl_SITE_STANAME:{		/* (site) Station Name (attribute) */
	   slPntr = (struct siteList *) targetObj;
 	   return (DBresult) slPntr->element->staname;
	}
	case dbl_SITE_STATYPE:{		/* (site) Station Type (attribute) */
	   slPntr = (struct siteList *) targetObj;
 	   return (DBresult) slPntr->element->statype;
	}
	case dbl_SITE_LDDATE:{		/* (site) Load Date (attribute) */
	   slPntr = (struct siteList *) targetObj;
 	   return (DBresult) slPntr->element->lddate;
	}

	case dbl_SITEC_CHAN:{		/* (sitechan) channel identifier (attribute) */
	   scPntr = (struct sitechanList *) targetObj;
 	   return (DBresult) scPntr->element->chan;
	}
	case dbl_SITEC_CHANID:{		/* (sitechan) Chan Recording ID (attribute) */
	   scPntr = (struct sitechanList *) targetObj;
 	   return (DBresult) &(scPntr->element->chanid);
	}
	case dbl_SITEC_CTYPE:{		/* (sitechan) Channel Type (attribute) */
	   scPntr = (struct sitechanList *) targetObj;
 	   return (DBresult) scPntr->element->ctype;
	}
	case dbl_SITEC_DESCRIP:{	/* (sitechan) Channel Description (attribute) */
	   scPntr = (struct sitechanList *) targetObj;
 	   return (DBresult) scPntr->element->descrip;
	}
	case dbl_SITEC_EDEPTH:{		/* (sitechan) Emplacement Depth (attribute) */
	   scPntr = (struct sitechanList *) targetObj;
 	   return (DBresult) &(scPntr->element->edepth);
	}
	case dbl_SITEC_HANG:{		/* (sitechan) Horizontal orientation (attribute) */
	   scPntr = (struct sitechanList *) targetObj;
 	   return (DBresult) &(scPntr->element->hang);
	}
	case dbl_SITEC_OFFDATE:{	/* (sitechan) Turn-off Date (attribute) */
	   scPntr = (struct sitechanList *) targetObj;
 	   return (DBresult) &(scPntr->element->offdate);
	}
	case dbl_SITEC_ONDATE:{		/* (sitechan) Turn-on Date (attribute) */
	   scPntr = (struct sitechanList *) targetObj;
 	   return (DBresult) &(scPntr->element->ondate);
	}
	case dbl_SITEC_STA:{		/* (sitechan) Station Code (attribute) */
	   scPntr = (struct sitechanList *) targetObj;
 	   return (DBresult) scPntr->element->sta;
	}
	case dbl_SITEC_VANG:{		/* (sitechan) Vert. Orientation of Seis.  (attribute) */
	   scPntr = (struct sitechanList *) targetObj;
 	   return (DBresult) &(scPntr->element->vang);
	}
	case dbl_SITEC_LDDATE:{		/* (sitechan) Load Date (attribute) */
	   scPntr = (struct sitechanList *) targetObj;
 	   return (DBresult) scPntr->element->lddate;
	}

	case dbl_STASS_AUTH:{		/* (stassoc) Author (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) saPntr->element->auth;
	}
	case dbl_STASS_AZIMUTH:{	/* (stassoc) Observed Azimuth (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) &(saPntr->element->azimuth);
	}
	case dbl_STASS_COMMID:{		/* (stassoc) comment ID (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) &(saPntr->element->commid);
	}
	case dbl_STASS_DEPTH:{		/* (stassoc) Source Depth (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) &(saPntr->element->depth);
	}
	case dbl_STASS_DIST:{		/* (stassoc) Estimated Distance (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) &(saPntr->element->dist);
	}
	case dbl_STASS_ETYPE:{		/* (stassoc) Event type (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) saPntr->element->etype;
	}
	case dbl_STASS_IMB:{		/* (stassoc) Initial Body wave Mag. (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) &(saPntr->element->imb);
	}
	case dbl_STASS_IML:{		/* (stassoc) Initial Local Mag. (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) &(saPntr->element->iml);
	}
	case dbl_STASS_IMS:{		/* (stassoc) Initial Surface wave Mag. (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) &(saPntr->element->ims);
	}
	case dbl_STASS_LAT:{		/* (stassoc) Latitude (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) &(saPntr->element->lat);
	}
	case dbl_STASS_LOCATION:{	/* (stassoc) Location Description. (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) saPntr->element->location;
	}
	case dbl_STASS_LON:{		/* (stassoc) Longitude (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) &(saPntr->element->lon);
	}
	case dbl_STASS_STA:{		/* (stassoc) Station Code (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) saPntr->element->sta;
	}
	case dbl_STASS_STASSID:{	/* (stassoc) Sta-assoc ID (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) &(saPntr->element->stassid);
	}
	case dbl_STASS_TIME:{		/* (stassoc) Epoch Time (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) &(saPntr->element->time);
	}
	case dbl_STASS_LDDATE:{		/* (stassoc) Load Date (attribute) */
	   saPntr = (struct stassocList *) targetObj;
 	   return (DBresult) saPntr->element->lddate;
	}
	
	case dbl_WFDIS_CALIB:{		/* (wfdisc) Calibration Factor (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) &(wfPntr->element->calib);
	}
	case dbl_WFDIS_CALPER:{		/* (wfdisc) Calibration Period (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) &(wfPntr->element->calper);
	}
	case dbl_WFDIS_CHAN:{		/* (wfdisc) channel identifier (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) wfPntr->element->chan;
	}
	case dbl_WFDIS_CHANID:{		/* (wfdisc) Chan Recording ID (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) &(wfPntr->element->chanid);
	}
	case dbl_WFDIS_CLIP:{		/* (wfdisc) clipped data flag (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) wfPntr->element->clip;
	}
	case dbl_WFDIS_COMMID:{		/* (wfdisc) comment ID (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) &(wfPntr->element->commid);
	}
	case dbl_WFDIS_DATATYPE:{	/* (wfdisc) Numeric storage (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) wfPntr->element->dattype;
	}
	case dbl_WFDIS_DFILE:{		/* (wfdisc) Data file (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) wfPntr->element->dfile;
	}
	case dbl_WFDIS_DIR:{		/* (wfdisc) directory (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) wfPntr->element->dir;
	}
	case dbl_WFDIS_ENDTIME:{	/* (wfdisc) Time of last datum (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) &(wfPntr->element->endtime);
	}
	case dbl_WFDIS_FOFF:{		/* (wfdisc) File offset (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) &(wfPntr->element->foff);
	}
	case dbl_WFDIS_INSTYPE:{	/* (wfdisc) Instrument Type (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) wfPntr->element->instype;
	}
	case dbl_WFDIS_JDATE:{		/* (wfdisc) Julian Date (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) &(wfPntr->element->jdate);
	}
	case dbl_WFDIS_NSAMP:{		/* (wfdisc) Number of Samples (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) &(wfPntr->element->nsamp);
	}
	case dbl_WFDIS_SAMPRATE:{	/* (wfdisc) Sampling Rate (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) &(wfPntr->element->samprate);
	}
	case dbl_WFDIS_SEGTYPE:{	/* (wfdisc) Segment Type (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) wfPntr->element->segtype;
	}
	case dbl_WFDIS_STA:{		/* (wfdisc) Station Code (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) wfPntr->element->sta;
	}
	case dbl_WFDIS_TIME:{		/* (wfdisc) Epoch Time (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) &(wfPntr->element->time);
	}
	case dbl_WFDIS_WFID:{		/* (wfdisc) Waveform Identifier (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) &(wfPntr->element->wfid);
	}
	case dbl_WFDIS_LDDATE:{		/* (wfdisc) Load Date (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) wfPntr->element->lddate;
	}
	case dbl_WFDIS_REAL_TRC:{		/* (wfdisc) pointer to real trace (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) wfPntr->seis->r;
	}
	case dbl_WFDIS_IMAG_TRC:{		/* (wfdisc) pointer to imag trace (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) wfPntr->seis->i;
	}
	case dbl_WFDIS_COMPLEX:{		/* (wfdisc) complex flag (attribute) */
	   wfPntr = (struct wfdiscList *) targetObj;
 	   return (DBresult) &(wfPntr->seis->Cmplx);
	}

	case dbl_WFTAG_TAGNAME:{		/* (wftag) key (arid, evid,...) (attribute) */
	   wtPntr = (struct wftagList *) targetObj;
 	   return (DBresult) wtPntr->element->tagname;
	}
	case dbl_WFTAG_TAGID:{		/* (wftag) tagname value (attribute) */
	   wtPntr = (struct wftagList *) targetObj;
 	   return (DBresult) &(wtPntr->element->tagid);
	}
	case dbl_WFTAG_WFID:{			/* (wftag) waveform ID (attribute) */
	   wtPntr = (struct wftagList *) targetObj;
 	   return (DBresult) &(wtPntr->element->wfid);
	}
	case dbl_WFTAG_LDDATE:{ 		/* (wftag) load date (attribute) */
	   wtPntr = (struct wftagList *) targetObj;
 	   return (DBresult) wtPntr->element->lddate;
	}


	case 	dbl_SACD_UDATA:{		/* (sacdata) key (wfid) (attribute) */
	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->userdata);
	}
	case 	dbl_SACD_SYNTHFLG:{		/* (sacdata) tagname value (attribute) */
	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->synthflag);
	}
        case dbl_SACD_LPSPOL:{		       /* (sacdata) waveform ID (attribute) */
	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->lpspol);
	}
	case dbl_SACD_IZTYPE:{		       /* (sacdata) load date (attribute) */
	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->iztype);
	}
	case dbl_SACD_IDEP:{		       /* (sacdata) tagname value (attribute) */
	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->idep);
	}
	case dbl_SACD_IFTYPE:{		       /* (sacdata) waveform ID (attribute) */
	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->iftype);
	}
	case dbl_SACD_WFID:{		       /* (sacdata) load date (attribute) */
	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->wfid);
	}
	case dbl_SACD_ODELTA:{
	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->odelta);
	}
	case dbl_SACD_EVEL:{
 	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->evel);
	}
       case dbl_SACD_IINST:{
 	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->iinst);
	}
       case dbl_SACD_ISTREG:{
 	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->istreg);
	}
       case dbl_SACD_IQUAL:{
	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->iqual);
	}
        case dbl_SACD_LOVROK:{
	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->lovrok);
	}
        case dbl_SACD_LCALDA:{
 	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->lcalda);
	}
       case dbl_SACD_KHOLE:{
	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->khole);
	}
        case dbl_SACD_KO:{
 	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->ko);
	}
       case dbl_SACD_KDATRD:{
	   sdPntr = (struct sacdataList *) targetObj;
 	   return (DBresult) &(sdPntr->element->kdatrd);
	}
   case dbl_DBLIST:
   LIST_CASES
   NET_CASES
   SR_CASES
       break;
	
   }
   return NULL;
}
/*----------------------------------------------------------------------*/


/* This function returns a pointer to the next table in a list belonging to the tree */
/* pointed to by DBlist. tableInstance is a pointer to a struct in that list. If */
/* tableInstance in NULL then this function returns a pointer to the first table in the */
/* tree. Otherwise, a pointer to the table following tableInstance is returned. */
/* When the end of the list is encountered, the function returns NULL. The specifier */
/* argument is a member of the enumeration in dblPublicDefs.h */
DBtable dblNextTableInstance(DBtable tableInstance, DBlist dblList, dblObject specifier)
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
   struct CSStree *tree;

   tree = (struct CSStree *) dblList;
   DEBUG("spec: %d [%d]\n", specifier, dbl_LIST_SITE);
   DEBUG("    tableInstance: %p\n", tableInstance);
   if(!tree){
       dblSetError(1, 
       "ERROR: Attempt to access element of null tree in dblNextTableInstance.\n");
       return (void *) NULL;
   }

   switch (specifier){
      case dbl_LIST_AFFILIATION:{
	if(!tableInstance)return (DBtable) tree->afHead;
	else{
	   afPntr = AF_LIST tableInstance;
           return (DBtable) afPntr->next;
	}
	break;
      }

      case dbl_LIST_ARRIVAL:{
	if(!tableInstance)return (DBtable) tree->arHead;
	else{
	   arPntr = AR_LIST tableInstance;
           return (DBtable) arPntr->next;
	}
	break;
      }

      case dbl_LIST_ASSOC:{
	if(!tableInstance)return (DBtable) tree->asHead;
	else{
	   asPntr = AS_LIST tableInstance;
           return (DBtable) asPntr->next;
	}
	break;
      }

      case dbl_LIST_EVENT:{
	if(!tableInstance)return (DBtable) tree->evHead;
	else{
	   evPntr = EV_LIST tableInstance;
           return (DBtable) evPntr->next;
	}
	break;
      }

      case dbl_LIST_GREGION:{
	if(!tableInstance)return (DBtable) tree->grHead;
	else{
	   grPntr = GR_LIST tableInstance;
           return (DBtable) grPntr->next;
	}
	break;
      }

      case dbl_LIST_INSTRUMENT:{
	if(!tableInstance)return (DBtable) tree->inHead;
	else{
	   inPntr = IN_LIST tableInstance;
           return (DBtable) inPntr->next;
	}
	break;
      }

      case dbl_LIST_ORIGERR:{
	if(!tableInstance)return (DBtable) tree->oeHead;
	else{
	   oePntr = OE_LIST tableInstance;
           return (DBtable) oePntr->next;
	}
	break;
      }

      case dbl_LIST_ORIGIN:{
	if(!tableInstance)return (DBtable) tree->orHead;
	else{
	   orPntr = OR_LIST tableInstance;
           return (DBtable) orPntr->next;
	}
	break;
      }

      case dbl_LIST_REMARK:{
	if(!tableInstance)return (DBtable) tree->reHead;
	else{
	   rePntr = RE_LIST tableInstance;
           return (DBtable) rePntr->next;
	}
	break;
      }

      case dbl_LIST_SENSOR:{
	if(!tableInstance)return (DBtable) tree->seHead;
	else{
	   sePntr = SE_LIST tableInstance;
           return (DBtable) sePntr->next;
	}
	break;
      }

      case dbl_LIST_SITE:{
        DEBUG("LIST_SITE: %p [%p/%p]\n", tableInstance, tree->slHead, tree->scHead);
        if(!tableInstance)return (DBtable) tree->slHead;
	else{
	   slPntr = SL_LIST tableInstance;
           return (DBtable) slPntr->next;
	}
	break;
      }

      case dbl_LIST_SITECHAN:{
	if(!tableInstance)return (DBtable) tree->scHead;
	else{
	   scPntr = SC_LIST tableInstance;
           return (DBtable) scPntr->next;
	}
	break;
      }

      case dbl_LIST_STASSOC:{
	if(!tableInstance)return (DBtable) tree->saHead;
	else{
	   saPntr = SA_LIST tableInstance;
           return (DBtable) saPntr->next;
	}
	break;
      }

      case dbl_LIST_WFDISC:{
	if(!tableInstance)return (DBtable) tree->wfHead;
	else{
	   wfPntr = WF_LIST tableInstance;
           return (DBtable) wfPntr->next;
	}
	break;
      }

      case dbl_LIST_WFTAG:{
	if(!tableInstance)return (DBtable) tree->wtHead;
	else{
	   wtPntr = WT_LIST tableInstance;
           return (DBtable) wtPntr->next;
	}
	break;
      }

      case dbl_LIST_SACDATA:{
	if(!tableInstance)return (DBtable) tree->sdHead;
	else{
	   sdPntr = SD_LIST tableInstance;
           return (DBtable) sdPntr->next;
	}
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
   return NULL;
}
/*----------------------------------------------------------------------*/



/* Used internally by dblWriteUserData to write a comment from a UserData  */
/* struct to disk */
static void StreamComment(char *comment, char *type, FILE *fptr)
{
   int stringLen;
   int markerLen = strlen(type);
   int itemsWritten = 0;

   if(!comment)
      stringLen = 0;
   else
      stringLen  = strlen(comment);
   
   itemsWritten += fwrite( type, sizeof(char), markerLen, fptr);
   itemsWritten += fwrite( (char *) &stringLen, sizeof(int), 1, fptr);
   if(comment) itemsWritten += fwrite( comment, sizeof(char), stringLen, fptr);

   if( itemsWritten < stringLen + markerLen + 1 ){
       dblSetError(1, "ERROR: Problem writing comments to UserData file. \n");
   }
}
/*----------------------------------------------------------------------*/






/* Used internally by dblWriteUserData to write a data from a UserData  */
/* struct to disk */

static void StreamData(ComplexFloat *data, int dataLen, FILE *fptr)
{
   int itemsWritten = 0;

   if(!data)
      dataLen = 0;
   itemsWritten += fwrite( (char *) &dataLen, sizeof(int), 1, fptr);
   if(dataLen)itemsWritten += 
		fwrite( (char *) data, sizeof(ComplexFloat), dataLen, fptr);
   
   if( itemsWritten < dataLen + 1 ){
       dblSetError(1, "ERROR: Problem writing data to UserData file. \n");
   }
}
/*----------------------------------------------------------------------*/





/* Used internally by dblReadUserData to get a comment from disk. */
static void ReadComment(FILE *fptr, char *comment, int dataLen)
{
   int itemsRead = 0;

   itemsRead += fread( (char *) comment, sizeof(char), dataLen, fptr);
   
   if( itemsRead < dataLen ){
       dblSetError(1, "ERROR: Problem reading comment from UserData file. \n");
   }


}
/*----------------------------------------------------------------------*/






/* Used internally by dblReadUserData to get the label for the upcoming
   data item and the number of bytes to follow. */

static void ReadLabelAndBytes(FILE *fptr, char *label, int *dataLen )
{
   int itemsRead = 0;

   itemsRead += fread( (char *) label, sizeof(char), 3, fptr);
   itemsRead +=	fread( (char *) dataLen, sizeof(int), 1, fptr);
   
   if( itemsRead < 4 ){
       dblSetError(1, "ERROR: Problem reading data from UserData file. \n");
   }
   label[3] = '\0';
}
/*----------------------------------------------------------------------*/






/* Used internally by dblReadUserData to get the number of ComplexFloats to read. */
static void ReadNumberOfCplxFloats(FILE *fptr,int *dataLen)
{
   int itemsRead = 0;

   itemsRead += fread( (char *) dataLen, sizeof(int), 1, fptr);
   if( itemsRead < 1 ){
       dblSetError(1, "ERROR: Problem reading data from UserData file. \n");
   }
}
/*----------------------------------------------------------------------*/






static void ReadIndex(FILE *fptr, int *index)
{
   int itemsRead = 0;

   itemsRead += fread( (char *) index, sizeof(int), 1, fptr);
   if( itemsRead < 1 ){
       dblSetError(1, "ERROR: Problem reading data from UserData file. \n");
   }
}
/*----------------------------------------------------------------------*/







/* Used internally by dblReadUserData to read a stram of ComplexFloats from disk */
static void ReadComplexFloats(FILE *fptr, ComplexFloat *data, int dataLen)
{
   int itemsRead = 0;

   itemsRead += fread( (char *) data, sizeof(ComplexFloat), dataLen, fptr);
   if( itemsRead < dataLen ){
       dblSetError(1, "ERROR: Problem reading data from UserData file. \n");
   }
}
/*----------------------------------------------------------------------*/






/* This function writes the entire UserData structure to a binary disk file. */
int dblWriteUserData(DBlist list, char * filename)
{

   struct CSStree *tree;
   char *comment;
   FILE *fptr;
   char errorStrg[300];
   DataComment *DCstruct;
   ComplexFloat *data;
   ComplexFloat **matrixPtr;
   int dataLen;
   int Nrows, Ncols, j;
   int index;
   DataComment * DC;
   
   tree = (struct CSStree *) list;
   if(!tree){
       dblSetError(1, "ERROR: Null input tree in dblWriteUserData.\n");
       return FALSE;
   }


   if(!tree->UserData)return FALSE;

   if(!filename){
       dblSetError(1, "ERROR: No filename specified in dblWriteUserData.\n");
       return FALSE;
   }


   if( !(fptr = fopen(filename,"wb")) ){
       strcpy(errorStrg, "ERROR: Could not open UserData file ");
       strcat(errorStrg,filename);
       strcat(errorStrg,"\n");
       dblSetError(1, errorStrg);
       return FALSE;
   }

   /* write global userData comment. */
   comment = dblGetUserDataComment(list);
   StreamComment(comment,"COM",fptr);


   /* Write out UserData DataComment comments and data. */
   DCstruct = (DataComment *)NULL;
   do{
      DCstruct = (struct DataPlusComment*) dblGetNextDataComment(DCstruct, list);
      if(DCstruct){
	 DC = (DataComment *) DCstruct;
	 comment = DCstruct->comment;
	 StreamComment(comment,"UDC",fptr);
	 data = DCstruct->data;
	 dataLen = DCstruct->dataLen;
	 StreamData(data, dataLen,fptr);
	 index = DC->reference;
	 fwrite( (char *) &index, sizeof(int), 1, fptr);
      }
   }while(DCstruct);

   /* Finally set UserData Matrix comment and data */
   comment = dblGetUserDataMatrixComment(list);
   StreamComment(comment,"UDM",fptr);
   matrixPtr = dblGetUserDataMatrixData(list, &Nrows, &Ncols);
   if(matrixPtr) {
       fwrite( (char *) &Nrows, sizeof(int), 1, fptr);
       for(j=0;j<Nrows;j++) {
           if(matrixPtr[j]) {
               StreamData(matrixPtr[j], Ncols,fptr);
           }
       }
   }


   fclose(fptr);
   return TRUE;

}
/*----------------------------------------------------------------------*/






/* This function reads a binary image of the UserData structure from disk and */
/* either adds it to an existing UserData struct attached to list, or else */
/* overwrites the existing list struct. */
int dblReadUserData(DBlist list, char * filename, int OverWrite)
{

   struct CSStree *tree;
   char *comment;
   FILE *fptr;
   char errorStrg[300];
   char label[4];
   DBdataComm dataComPtr;
   ComplexFloat *data;
   ComplexFloat **matrixPtr;
   int dataLen;
   int Nrows, Ncols, j;
   int index;

   Ncols = 0;

   
   tree = (struct CSStree *) list;
   if(!tree){
       dblSetError(1, "ERROR: Null CSS tree in dblReadUserData.\n");
       return FALSE;
   }



   if(!filename){
       dblSetError(1, "ERROR: No filename specified in dblReadUserData.\n");
       return FALSE;
   }


   if( !(fptr = fopen(filename,"rb")) ){
       strcpy(errorStrg, "ERROR: Could not open UserData file ");
       strcat(errorStrg,filename);
       strcat(errorStrg,"\n");
       dblSetError(1, errorStrg);
       return FALSE;
   }


   if(OverWrite)dblDestroyUserDataStruc( list);



   /* read global userData comment. */
   ReadLabelAndBytes(fptr,label,&dataLen);
   if(dataLen){
      comment = (char *) smMalloc( (dataLen +1) * sizeof(char) );
      ReadComment(fptr, comment, dataLen);
      dblSetUserDataComment(list, comment);
      smFree(comment);
   }


  /*  Read UserData DataComments */
   do{
      ReadLabelAndBytes(fptr,label,&dataLen);
      if(!strcmp("UDC",label) ){
         dataComPtr = dblCreateDataComment(list);
         if(dataLen){
            comment = (char *) smMalloc( (dataLen +1) * sizeof(char) );
            ReadComment(fptr, comment, dataLen);
	    dblSetUserDataComComment(dataComPtr,comment);
	    smFree(comment);
         }
         ReadNumberOfCplxFloats(fptr, &dataLen);
         if(dataLen){
	    data = (ComplexFloat *) smMalloc( dataLen * sizeof(ComplexFloat) );
	    ReadComplexFloats(fptr, data, dataLen);
	    dblSetUserDataComData(dataComPtr, data, dataLen);
	    smFree(data);
         }
	 ReadIndex(fptr, &index);
	 dblSetDCIndex(dataComPtr, index);
      }

   }while(!strcmp("UDC",label) );

   if(!strcmp("UDM",label) ){
      if(dataLen){
         comment = (char *) smMalloc( (dataLen +1) * sizeof(char) );
         ReadComment(fptr, comment, dataLen);
         dblSetUserDataMatrixComment(list, comment);
	 smFree(comment);
      }
      ReadNumberOfCplxFloats(fptr, &Nrows);
      if(Nrows){
	 matrixPtr = (ComplexFloat **) smMalloc(Nrows * sizeof(ComplexFloat *) );
	 for(j=0;j<Nrows;j++){
	    ReadNumberOfCplxFloats(fptr, &Ncols);
	    matrixPtr[j] = (ComplexFloat *) smMalloc( Ncols * sizeof(ComplexFloat) );
	    ReadComplexFloats(fptr, matrixPtr[j], Ncols);
	 }
	 dblSetUserDataMatrixData(list, Nrows, Ncols, matrixPtr);
	 for(j=0;j<Nrows;j++){
	    smFree(matrixPtr[j]);
	 }
	 smFree(matrixPtr);
      }
	    
   }
   fclose(fptr);
   return TRUE;

}
/*----------------------------------------------------------------------*/





/* Sort the Wfdisc list in an existing tree into the order given by the
   Input array Index. It returns 1 on success and 0 otherwise.*/
int dblSortWfdiscList(DBlist TreePtr, int *Index, int Nitems)
{
	struct wfdiscList      *wfPntr, **PntrArray;	
	struct CSStree *Tree;
	int Count = 0;
	Tree = (struct CSStree *) TreePtr;

   	if(Tree == (struct CSStree *) NULL) return 0;
	if(!Index || Nitems < 1) return 0;
	if(Nitems < 2) return 1;   /* List with 1 element is automatically sorted! */

/* First make sure the number of items in the list = Nitems */	      
      	wfPntr = Tree->wfHead;
	while (wfPntr != NULL_WF_LIST){
           Count++;
	   wfPntr = wfPntr->next;
	}
        if(Count != Nitems){
           printf("Error in dblSortWfdiscList: Nitems != List Count!\n");
	   return 0;
	}

	/* Make sure none of the elements of Index has a value > Nitems - 1. */
	for(Count=0;Count<Nitems;Count++)
	  if(Index[Count] > Nitems -1 || Index[Count] < 0){
	     printf("Error in dblSortWfdiscList: One or more Index values out of bounds!\n");
	     return 0;
	  }

        PntrArray = (struct wfdiscList**) smMalloc(Nitems * sizeof(struct wfdiscList*) );
        if(!PntrArray){
           printf("Error in dblSortWfdiscList: Could not allocate PntrArray!\n");
	   return 0;
	}

	/* Fill the pointer array... */
	Count = 0;
      	wfPntr = Tree->wfHead;
	while (wfPntr != NULL_WF_LIST){
	   PntrArray[Count] = wfPntr;
           Count++;
	   wfPntr = wfPntr->next;
	}


	/* Now sort the list... */
	for(Count=1;Count<Nitems-1;Count++){
	   wfPntr = PntrArray[ Index[Count] ];
	   wfPntr->prev = PntrArray[ Index[Count - 1] ]; 
	   wfPntr->next = PntrArray[ Index[Count + 1] ]; 
	}

        Tree->wfHead = PntrArray[ Index[0] ];
	Tree->wfHead->prev = 0;
	Tree->wfHead->next = PntrArray[ Index[1] ];

	Tree->wfTail = PntrArray[ Index[Nitems - 1] ];
	Tree->wfTail->next = 0;
	Tree->wfTail->prev = PntrArray[ Index[Nitems - 2] ];

	smFree(PntrArray);
	return 1;
}
/*----------------------------------------------------------------------*/







static int NextCommentLineNum(struct CSStree *tree, int commid)
{
   struct remarkList *rePntr;
   int lineNum = -1;
   rePntr = tree->reHead;
   while(rePntr){
      if(rePntr->element->commid == commid)
         if(rePntr->element->lineno >= lineNum)
            lineNum = rePntr->element->lineno + 1;
      rePntr = rePntr->next;
   }
   return lineNum;
}
/* -------------------------------------------------------------------- */





static int NextCommid(struct CSStree *tree)
{
   struct remarkList *rePntr;
   int commid = 1;
   rePntr = tree->reHead;
   while(rePntr){
      if(rePntr->element->commid >= commid)commid = rePntr->element->commid + 1;
      rePntr = rePntr->next;
   }
   return commid;
}
/* -------------------------------------------------------------------- */




static void AddComment(struct CSStree *tree, int commid, int lineNum, const char* comment)
{
   struct remarkList *rePntr;
   
   rePntr = (struct remarkList *) dblCreateTableInstance(tree, dbl_LIST_REMARK);
   if(rePntr) {
       rePntr->element->commid = commid;
       rePntr->element->lineno = lineNum;
       strncpy(rePntr->element->remark, comment, 80);
       strcpy(rePntr->element->lddate, tmListEpochTime( tmGetEpochTime(), 18 ) );
   }
}
/* -------------------------------------------------------------------- */






int dblAddComment(DBlist list, dblObject StrucType, DBtable rowPntr, const char* comment)
{

	struct arrivalList     *arPntr;
	struct assocList       *asPntr;
	struct eventList       *evPntr;
	struct origerrList     *oePntr;
	struct originList      *orPntr;
	struct stassocList     *saPntr;
	struct wfdiscList      *wfPntr;	
        int lineNum;
	struct CSStree *tree;
	
		
	tree = (struct CSStree *) list;
	if(!tree){
           dblSetError(1, "ERROR: Null input tree in dblAddComment.\n");
	   return 0;
	}

        if(!rowPntr){
           dblSetError(1, "ERROR: Null row pointer in dblAddComment.\n");
	   return 0;
	}


        
	switch (StrucType){
	   case dbl_LIST_ARRIVAL:{
	      arPntr = (struct arrivalList*) rowPntr;
              lineNum = NextCommentLineNum(tree, arPntr->element->commid);
              if(lineNum < 0){
                 arPntr->element->commid = NextCommid(tree);
                 lineNum = 1;
	      }   
              AddComment(tree, arPntr->element->commid, lineNum, comment);   
	      return 1;
	   }
	
	   case dbl_LIST_ASSOC:{
	      asPntr = (struct assocList*) rowPntr;
              lineNum = NextCommentLineNum(tree, asPntr->element->commid);
              if(lineNum < 0){
                 asPntr->element->commid = NextCommid(tree);
                 lineNum = 1;
	      }   
              AddComment(tree, asPntr->element->commid, lineNum, comment);   
	      return 1;
	   }
	
	   case dbl_LIST_EVENT:{
	      evPntr = (struct eventList*) rowPntr;
              lineNum = NextCommentLineNum(tree, evPntr->element->commid);
              if(lineNum < 0){
                 evPntr->element->commid = NextCommid(tree);
                 lineNum = 1;
	      }   
              AddComment(tree, evPntr->element->commid, lineNum, comment);   
	      return 1;
	   }
	
	   case dbl_LIST_ORIGERR:{
	      oePntr = (struct origerrList*) rowPntr;
              lineNum = NextCommentLineNum(tree, oePntr->element->commid);
              if(lineNum < 0){
                 oePntr->element->commid = NextCommid(tree);
                 lineNum = 1;
	      }   
              AddComment(tree, oePntr->element->commid, lineNum, comment);   
	      return 1;
	   }
	
	   case dbl_LIST_ORIGIN:{
	      orPntr = (struct originList*) rowPntr;
              lineNum = NextCommentLineNum(tree, orPntr->element->commid);
              if(lineNum < 0){
                 orPntr->element->commid = NextCommid(tree);
                 lineNum = 1;
	      }   
              AddComment(tree, orPntr->element->commid, lineNum, comment);   
	      return 1;
	   }
	
	   case dbl_LIST_STASSOC:{
	      saPntr = (struct stassocList*) rowPntr;
              lineNum = NextCommentLineNum(tree, saPntr->element->commid);
              if(lineNum < 0){
                 saPntr->element->commid = NextCommid(tree);
                 lineNum = 1;
	      }   
              AddComment(tree, saPntr->element->commid, lineNum, comment);   
	      return 1;
	   }
	
	   case dbl_LIST_WFDISC:{
	      wfPntr = (struct wfdiscList*) rowPntr;
              lineNum = NextCommentLineNum(tree, wfPntr->element->commid);
              if(lineNum < 0){
                 wfPntr->element->commid = NextCommid(tree);
                 lineNum = 1;
	      }   
              AddComment(tree, wfPntr->element->commid, lineNum, comment);   
	      return 1;
	   }
	
	   default: {
	      return 0;
	   }
	}
}
/*----------------------------------------------------------------------*/


static char** GetAllComments(struct CSStree *tree, int commid, int* Nlines)
{
   char ** comment;
   struct remarkList *rePntr;

   *Nlines = 0;
   rePntr = tree->reHead;
   while(rePntr){
     if(rePntr->element->commid == commid)(*Nlines)++;
      rePntr = rePntr->next;
   }
   if(!(*Nlines) ) return 0;
   comment = (char**) smMalloc(*Nlines * sizeof(char*));

   *Nlines = 0;
   rePntr = tree->reHead;
   while(rePntr){
     if(rePntr->element->commid == commid){
        comment[ *Nlines ] = (char*) smMalloc( strlen(rePntr->element->remark) + 1);
        strcpy(comment[ *Nlines ], rePntr->element->remark); 
        (*Nlines)++;
     }       
      rePntr = rePntr->next;
   }
        
   return comment;
}
/* -------------------------------------------------------------------- */





char** dblGetComments(DBlist list, dblObject StrucType, DBtable rowPntr, int *Nlines)
{

	struct arrivalList     *ar = 0;
	struct assocList       *as = 0;
	struct eventList       *ev = 0;
	struct origerrList     *oe = 0;
	struct originList      *orig = 0;
	struct stassocList     *sa = 0;
	struct wfdiscList      *wf = 0;	
        char** comments;
	struct CSStree *tree;
	
	*Nlines = 0; 	
	tree    = (struct CSStree *) list;
	if(!tree){
           dblSetError(1, "ERROR: Null input tree in dblGetComments.\n");
	   return 0;
	}

        if(!rowPntr){
           dblSetError(1, "ERROR: Null row pointer in dblGetComments.\n");
	   return 0;
	}


        
	switch (StrucType){
	   case dbl_LIST_ARRIVAL:{
	      ar = (struct arrivalList*) rowPntr;
              comments = GetAllComments(tree, ar->element->commid, Nlines);
	      return comments;
	   }
	
	   case dbl_LIST_ASSOC:{
	      as = (struct assocList*) rowPntr;
              comments = GetAllComments(tree, as->element->commid, Nlines);
	      return comments;
	   }
	
	   case dbl_LIST_EVENT:{
	      ev = (struct eventList*) rowPntr;
              comments = GetAllComments(tree, ev->element->commid, Nlines);
	      return comments;
	   }
	
	   case dbl_LIST_ORIGERR:{
	      oe = (struct origerrList*) rowPntr;
              comments = GetAllComments(tree, oe->element->commid, Nlines);
	      return comments;
	   }
	
	   case dbl_LIST_ORIGIN:{
	      orig = (struct originList*) rowPntr;
              comments = GetAllComments(tree, orig->element->commid, Nlines);
	      return comments;
	   }
	
	   case dbl_LIST_STASSOC:{
	      sa = (struct stassocList*) rowPntr;
              comments = GetAllComments(tree, sa->element->commid, Nlines);
	      return comments;
	   }
	
	   case dbl_LIST_WFDISC:{
	      wf = (struct wfdiscList*) rowPntr;
              comments = GetAllComments(tree, wf->element->commid, Nlines);
	      return comments;
	   }
	
	   default: {
	      return 0;
	   }
	}
}
/*----------------------------------------------------------------------*/








/* static int InDeleteList(int idx, int *index,int Nitems) */
/* { */
/*    int j; */
/*    for(j = 0; j < Nitems; j++) */
/*       if(idx == index[j] ) return 1; */
   
/*    return 0; */
/* } */
/* ------------------------------------------------------------------ */




int  DeleteWfdiscByIndex(DBlist tree, int idx )
{
   struct wfdiscList *w = 0;
   int k = 0;
   do{
      w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC);
      if(!w)break;
      if(k++ == idx){
         dblDeleteTableInstance(dbl_LIST_WFDISC, tree, w);
         return 1;
      }
   }while(w);
   
   fprintf(stderr, "Failed to delete wfdisc with index %d.\n", idx);
   return 0;
}
/* ------------------------------------------------------------------ */


/* index is an array holding indices of traces to be deleted in sorted
   order (smallest to largest)
   Nitems is the number of items to delete
*/   
int dblDeleteWfdiscs(DBlist tree, int *index, int Nitems)
{
   int idx;
   if(!index || Nitems < 1) return 0;
   

   for ( idx = 0 ; idx < Nitems ; idx++ )
      if(!DeleteWfdiscByIndex(tree, index[idx] ) )
         return 0;
 

   return 1;
}



/*

int dblDeleteWfdiscs(DBlist tree, int *index, int Nitems)
{
   struct wfdiscList *w, **wl;
   int idx = 0;
   
   if(!index || Nitems < 1) return 0;
   wl = (struct wfdiscList **) smMalloc(Nitems * sizeof( struct wfdiscList * ) );
   w = 0;
   do{
      w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC);
      if(!w)break;
      if( InDeleteList(idx, index, Nitems) )
         wl[idx] = w;
      else
         wl[idx] = 0;
      idx++;
   }while(w);

   for(idx = 0; idx < Nitems; idx++)
      dblDeleteTableInstance(dbl_LIST_WFDISC, tree, wl[idx]);

   smFree(wl);
   return 1;
}

*/

/* ------------------------------------------------------------------ */





int dblWfidInUse(DBlist tree, int wfid)
{
   struct wfdiscList *w = 0;

   do{
      w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC);
      if(!w)break;
      if(w->element->wfid == wfid) return 1;
   }while(w);

   return 0;
}
/* ------------------------------------------------------------------ */




int dblNextAvailableWfid(DBlist tree)
{
   struct wfdiscList *w = 0;
   int wfid = 0;
   do{
      w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC);
      if(!w)break;
      if(w->element->wfid > wfid) wfid = w->element->wfid;
   }while(w);

   return ++wfid;
}
/* ------------------------------------------------------------------ */


void dblReplaceWfid(DBlist tree, int OldWfid, int NewWfid)
{
   struct affiliationList *af = 0;
   struct arrivalList     *ar = 0;
   struct assocList       *as = 0;
   struct eventList       *ev = 0;
   struct gregionList     *gr = 0;
   struct instrumentList  *in = 0;
   struct origerrList     *oe = 0;
   struct originList      *orig = 0;
   struct sensorList      *se = 0;
   struct siteList        *sl = 0;
   struct sitechanList    *sc = 0;
   struct stassocList     *sa = 0;
   struct wfdiscList      *w  = 0;
   struct wftagList       *wt = 0;
   struct sacdataList     *sd = 0;


   /* First replace wfids in affiliationList... */
   do{
      af = (struct affiliationList *) dblNextTableInstance(af, tree, dbl_LIST_AFFILIATION);
      if(!af)break;
      if(af->element->wfid == OldWfid){
         af->element->wfid = NewWfid;
         break;
      }   
   }while(af);

   /* Now replace wfids in arrivalList... */
   do{
      ar = (struct arrivalList *) dblNextTableInstance(ar, tree, dbl_LIST_ARRIVAL);
      if(!ar)break;
      if(ar->element->wfid == OldWfid){
         ar->element->wfid = NewWfid;
         break;
      }   
   }while(ar);

   /* Now replace wfids in assocList... */
   do{
      as = (struct assocList *) dblNextTableInstance(as, tree, dbl_LIST_ASSOC);
      if(!as)break;
      if(as->element->wfid == OldWfid){
         as->element->wfid = NewWfid;
         break;
      }   
   }while(as);

   /* Now replace wfids in eventList... */
   do{
      ev = (struct eventList *) dblNextTableInstance(ev, tree, dbl_LIST_EVENT);
      if(!ev)break;
      if(ev->element->wfid == OldWfid){
         ev->element->wfid = NewWfid;
         break;
      }   
   }while(ev);

   /* Now replace wfids in gregionList... */
   do{
      gr = (struct gregionList *) dblNextTableInstance(gr, tree, dbl_LIST_GREGION);
      if(!gr)break;
      if(gr->element->wfid == OldWfid){
         gr->element->wfid = NewWfid;
         break;
      }   
   }while(gr);

   /* Now replace wfids in instrumentList... */
   do{
      in = (struct instrumentList *) dblNextTableInstance(in, tree, dbl_LIST_INSTRUMENT);
      if(!in)break;
      if(in->element->wfid == OldWfid){
         in->element->wfid = NewWfid;
         break;
      }   
   }while(in);

   /* Now replace wfids in origerrList... */
   do{
      oe = (struct origerrList *) dblNextTableInstance(oe, tree, dbl_LIST_ORIGERR);
      if(!oe)break;
      if(oe->element->wfid == OldWfid){
         oe->element->wfid = NewWfid;
         break;
      }   
   }while(oe);

   /* Now replace wfids in originList... */
   do{
      orig = (struct originList *) dblNextTableInstance(orig, tree, dbl_LIST_ORIGIN);
      if(!orig)break;
      if(orig->element->wfid == OldWfid){
         orig->element->wfid = NewWfid;
         break;
      }   
   }while(orig);

   /* Now replace wfids in sensorList... */
   do{
      se = (struct sensorList *) dblNextTableInstance(se, tree, dbl_LIST_SENSOR);
      if(!se)break;
      if(se->element->wfid == OldWfid){
         se->element->wfid = NewWfid;
         break;
      }   
   }while(se);

   /* Now replace wfids in siteList... */
   do{
      sl = (struct siteList *) dblNextTableInstance(sl, tree, dbl_LIST_SITE);
      if(!sl)break;
      if(sl->element->wfid == OldWfid){
         sl->element->wfid = NewWfid;
         break;
      }   
   }while(sl);

   /* Now replace wfids in sitechanList... */
   do{
      sc = (struct sitechanList *) dblNextTableInstance(sc, tree, dbl_LIST_SITECHAN);
      if(!sc)break;
      if(sc->element->wfid == OldWfid){
         sc->element->wfid = NewWfid;
         break;
      }   
   }while(sc);

   /* Now replace wfids in stassocList... */
   do{
      sa = (struct stassocList *) dblNextTableInstance(sa, tree, dbl_LIST_STASSOC);
      if(!sa)break;
      if(sa->element->wfid == OldWfid){
         sa->element->wfid = NewWfid;
         break;
      }   
   }while(sa);


   /* Now replace wfids in wfdiscList... */
   do{
      w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC);
      if(!w)break;
      if(w->element->wfid == OldWfid){
         w->element->wfid = NewWfid;
         break;
      }   
   }while(w);

   /* Now replace wfids in wftagList... */
   do{
      wt = (struct wftagList *) dblNextTableInstance(wt, tree, dbl_LIST_WFTAG);
      if(!wt)break;
      if(wt->element->wfid == OldWfid){
         wt->element->wfid = NewWfid;
         break;
      }   
   }while(wt);

   /* Finally replace wfids in sacdataList... */
   do{
      sd = (struct sacdataList *) dblNextTableInstance(sd, tree, dbl_LIST_SACDATA);
      if(!sd)break;
      if(sd->element->wfid == OldWfid){
         sd->element->wfid = NewWfid;
         break;
      }   
   }while(sd);


}
/* ------------------------------------------------------------------ */






struct originList *dblOridInUse(DBlist tree, int orid)
{
   struct originList *orig = 0;

   do{
      orig = (struct originList *) dblNextTableInstance(orig, tree, dbl_LIST_ORIGIN);
      if(!orig)break;
      if(orig->element->orid == orid) return orig;
   }while(orig);

   return 0;
}
/* ------------------------------------------------------------------ */




int dblNextAvailableOrid(DBlist tree)
{
   struct originList *orig = 0;
   int orid = 0;
   do{
      orig = (struct originList *) dblNextTableInstance(orig, tree, dbl_LIST_ORIGIN);
      if(!orig)break;
      if(orig->element->orid > orid) orid = orig->element->orid;
   }while(orig);

   return ++orid;
}
/* ------------------------------------------------------------------ */


void dblReplaceOrid(DBlist tree, int OldOrid, int NewOrid)
{
   struct assocList       *as = 0;
   struct origerrList     *oe = 0;
   struct originList      *orig = 0;
   struct eventList       *ev = 0 ;


   /* First replace orids in assocList... */
   do{
      as = (struct assocList *) dblNextTableInstance(as, tree, dbl_LIST_ASSOC);
      if(!as)break;
      if(as->element->orid == OldOrid)
         as->element->orid = NewOrid;
   }while(as);

   /* Next replace orids in origerrList... */
   do{
      oe = (struct origerrList *) dblNextTableInstance(oe, tree, dbl_LIST_ORIGERR);
      if(!oe)break;
      if(oe->element->orid == OldOrid){
         oe->element->orid = NewOrid;
         break;
      }   
   }while(oe);

   /* Then replace orids in originList... */
   do{
      orig = (struct originList *) dblNextTableInstance(orig, tree, dbl_LIST_ORIGIN);
      if(!orig)break;
      if(orig->element->orid == OldOrid){
         orig->element->orid = NewOrid;
         break;
      }   
   }while(orig);

   /* Finally replace orids in prefors of eventList */
   do{
      ev = (struct eventList *) dblNextTableInstance(ev, tree, dbl_LIST_EVENT);
      if(!ev)break;
      if(ev->element->prefor == OldOrid){
         ev->element->prefor = NewOrid;
         break;
      }
   }while(ev);

}
/* ------------------------------------------------------------------ */





struct eventList *dblEvidInUse(DBlist tree, int evid)
{
   struct eventList       *ev = 0;

   do{
      ev = (struct eventList *) dblNextTableInstance(ev, tree, dbl_LIST_EVENT);
      if(!ev)break;
      if(ev->element->evid == evid) return ev;
   }while(ev);

   return 0;
}
/* ------------------------------------------------------------------ */




int dblNextAvailableEvid(DBlist tree)
{
   struct eventList       *ev = 0;
   int evid = 0;
   do{
      ev = (struct eventList *) dblNextTableInstance(ev, tree, dbl_LIST_EVENT);
      if(!ev)break;
      if(ev->element->evid > evid) evid = ev->element->evid;
   }while(ev);

   return ++evid;
}
/* ------------------------------------------------------------------ */






void dblReplaceEvid(DBlist tree, int OldEvid, int NewEvid)
{
   struct eventList       *ev   = 0;
   struct originList      *orig = 0;
   struct wftagList       *wt   = 0;

   /* First replace evids in eventList... */
   do{
      ev = (struct eventList *) dblNextTableInstance(ev, tree, dbl_LIST_EVENT);
      if(!ev)break;
      if(ev->element->evid == OldEvid)
         ev->element->evid = NewEvid;
   }while(ev);

   /* Next replace evids in originList... */
   do{
      orig = (struct originList *) dblNextTableInstance(orig, tree, dbl_LIST_ORIGIN);
      if(!orig)break;
      if(orig->element->evid == OldEvid)
         orig->element->evid = NewEvid;
   }while(orig);


   /* Finally replace evids in wftagList... */
   do{
      wt = (struct wftagList *) dblNextTableInstance(wt, tree, dbl_LIST_WFTAG);
      if(!wt)break;
      if(wt->element->tagid == OldEvid && !strcmp( wt->element->tagname , "evid" ) )
         wt->element->tagid = NewEvid;
   }while(wt);
}
/* ------------------------------------------------------------------ */







int dblAridInUse(DBlist tree, int arid)
{
   struct arrivalList     *ar = 0;

   do{
      ar = (struct arrivalList *) dblNextTableInstance(ar, tree, dbl_LIST_ARRIVAL);
      if(!ar)break;
      if(ar->element->arid == arid) return 1;
   }while(ar);

   return 0;
}
/* ------------------------------------------------------------------ */




int dblNextAvailableArid(DBlist tree)
{
   struct arrivalList     *ar = 0;
   int arid = 0;
   do{
      ar = (struct arrivalList *) dblNextTableInstance(ar, tree, dbl_LIST_ARRIVAL);
      if(!ar)break;
      if(ar->element->arid > arid) arid = ar->element->arid;
   }while(ar);

   return ++arid;
}
/* ------------------------------------------------------------------ */





void dblReplaceArid(DBlist tree, int OldArid, int NewArid)
{
   struct arrivalList     *ar = 0;
   struct assocList       *as = 0;
   struct wftagList       *wt = 0;


   /* First replace arids in arrivalList... */
   do{
      ar = (struct arrivalList *) dblNextTableInstance(ar, tree, dbl_LIST_ARRIVAL);
      if(!ar)break;
      if(ar->element->arid == OldArid)
         ar->element->arid = NewArid;
   }while(ar);


   /* Next replace arids in assocList... */
   do{
      as = (struct assocList *) dblNextTableInstance(as, tree, dbl_LIST_ASSOC);
      if(!as)break;
      if(as->element->arid == OldArid)
         as->element->arid = NewArid;
   }while(as);

   /* Finally replace arids in wftagList... */
   do{
      wt = (struct wftagList *) dblNextTableInstance(wt, tree, dbl_LIST_WFTAG);
      if(!wt)break;
      if(wt->element->tagid == OldArid)
         if(!strcmp(wt->element->tagname, "arid") )
            wt->element->tagid = NewArid;
   }while(wt);
}
/* ------------------------------------------------------------------ */







int dblChanidInUse(DBlist tree, int chanid)
{
   struct wfdiscList      *w  = 0;

   do{
      w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC);
      if(!w)break;
      if(w->element->chanid == chanid) return 1;
   }while(w);

   return 0;
}
/* ------------------------------------------------------------------ */




int dblNextAvailableChanid(DBlist tree)
{
   struct wfdiscList      *w  = 0;
   int chanid = 0;
   do{
      w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC);
      if(!w)break;
      if(w->element->chanid > chanid) chanid = w->element->chanid;
   }while(w);

   return ++chanid;
}
/* ------------------------------------------------------------------ */



void dblReplaceChanid(DBlist tree, int OldChanid, int NewChanid)
{
   struct arrivalList     *ar = 0;
   struct sensorList      *se = 0;
   struct sitechanList    *sc = 0;
   struct wfdiscList      *w  = 0;


   /* First replace chanids in arrivalList... */
   do{
      ar = (struct arrivalList *) dblNextTableInstance(ar, tree, dbl_LIST_ARRIVAL);
      if(!ar)break;
      if(ar->element->chanid == OldChanid)
         ar->element->chanid = NewChanid;
   }while(ar);

   /* Then replace chanids in sensorList... */
   do{
      se = (struct sensorList *) dblNextTableInstance(se, tree, dbl_LIST_SENSOR);
      if(!se)break;
      if(se->element->chanid == OldChanid)
         se->element->chanid = NewChanid;
   }while(se);

   /* Then replace chanids in sitechanList... */
   do{
      sc = (struct sitechanList *) dblNextTableInstance(sc, tree, dbl_LIST_SITECHAN);
      if(!sc)break;
      if(sc->element->chanid == OldChanid)
         sc->element->chanid = NewChanid;
   }while(sc);

   /* Finally replace chanids in wfdiscList... */
   do{
      w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC);
      if(!w)break;
      if(w->element->chanid == OldChanid)
         w->element->chanid = NewChanid;
   }while(w);
}
/* ------------------------------------------------------------------ */










int dblInidInUse(DBlist tree, int inid)
{
   struct instrumentList      *in  = 0;

   do{
      in = (struct instrumentList *) dblNextTableInstance(in, tree, dbl_LIST_INSTRUMENT);
      if(!in)break;
      if(in->element->inid == inid) return 1;
   }while(in);

   return 0;
}
/* ------------------------------------------------------------------ */




int dblNextAvailableInid(DBlist tree)
{
   struct instrumentList      *in  = 0;
   int inid = 0;
   do{
      in = (struct instrumentList *) dblNextTableInstance(in, tree, dbl_LIST_INSTRUMENT);
      if(!in)break;
      if(in->element->inid > inid) inid = in->element->inid;
   }while(inid);

   return ++inid;
}
/* ------------------------------------------------------------------ */



void dblReplaceInid(DBlist tree, int OldInid, int NewInid)
{
   struct instrumentList     *in = 0;
   struct sensorList         *se = 0;


   /* First replace inids in instrumentList... */
   do{
      in = (struct instrumentList *) dblNextTableInstance(in, tree, dbl_LIST_INSTRUMENT);
      if(!in)break;
      if(in->element->inid == OldInid)
         in->element->inid = NewInid;
   }while(in);

   /* Then replace inids in sensorList... */
   do{
      se = (struct sensorList *) dblNextTableInstance(se, tree, dbl_LIST_SENSOR);
      if(!se)break;
      if(se->element->inid == OldInid)
         se->element->inid = NewInid;
   }while(se);

}
/* ------------------------------------------------------------------ */

















static struct siteList *findMatchingSite(DBlist Tree, struct siteList *sIn)
{
    struct siteList *s    = 0;

   do{
      s = (struct siteList *) dblNextTableInstance(s, Tree, dbl_LIST_SITE);
      if(!s)break;
      if(!strcmp(sIn->element->sta, s->element->sta) )  
         if(sIn->element->ondate  == s->element->ondate)   
            if(sIn->element->lat     == s->element->lat)     
               if(sIn->element->lon     == s->element->lon)     
                  if(sIn->element->elev    == s->element->elev)     
                     if(sIn->element->offdate == s->element->offdate)  
                        return s;
   }while(s);

   return 0;
}
/* ------------------------------------------------------------------ */



static struct sitechanList *findMatchingSitechan(DBlist Tree, struct sitechanList *scIn)
{
    struct sitechanList *sc    = 0;

   do{
      sc = (struct sitechanList *) dblNextTableInstance(sc, Tree, dbl_LIST_SITECHAN);
      if(!sc)break;
      if(!strcmp(scIn->element->sta, sc->element->sta) )  
         if(!strcmp(scIn->element->chan, sc->element->chan) )  
            if(scIn->element->ondate  == sc->element->ondate)
               if(scIn->element->offdate == sc->element->offdate)   
                  if(scIn->element->edepth     == sc->element->edepth)     
                     if(scIn->element->hang     == sc->element->hang)     
                        if(scIn->element->vang    == sc->element->vang)     
                           return sc;
   }while(sc);

   return 0;
}
/* ------------------------------------------------------------------ */




static struct arrivalList *findMatchingArrival(DBlist Tree, struct arrivalList *arIn)
{
    struct arrivalList *a    = 0;

   do{
      a = (struct arrivalList *) dblNextTableInstance(a, Tree, dbl_LIST_ARRIVAL);
      if(!a)break;
      if(!strcmp(arIn->element->sta, a->element->sta) )  
         if(!strcmp(arIn->element->chan, a->element->chan) )  
            if(!strcmp(arIn->element->iphase, a->element->iphase) )
               if(!strcmp(arIn->element->auth, a->element->auth))   
                  if(fabs(arIn->element->time - a->element->time) < .001)     
                     return a;
   }while(a);

   return 0;
}
/* ------------------------------------------------------------------ */




static struct eventList *findMatchingEvent(DBlist Tree, struct eventList *evIn)
{
   struct eventList *ev    = 0;

   if( !evIn || !strcmp( evIn->element->evname , "-" ) )
      return 0 ;

   do{
      ev = (struct eventList *) dblNextTableInstance(ev, Tree, dbl_LIST_EVENT);
      if(!ev)break;
      if(!strcmp(evIn->element->evname, ev->element->evname) )
         return ev;
   }while(ev);

   return 0;
}
/* ------------------------------------------------------------------ */




static struct gregionList *findMatchingGregion(DBlist Tree, struct gregionList *grIn)
{
   struct gregionList *gr    = 0;

   do{
      gr = (struct gregionList *) dblNextTableInstance(gr, Tree, dbl_LIST_GREGION);
      if(!gr)break;
      if( grIn->element->grn == gr->element->grn )
         return gr;
   }while(gr);

   return 0;
}
/* ------------------------------------------------------------------ */




static struct wftagList *findMatchingWftag(DBlist Tree, struct wftagList *wtIn)
{
   struct wftagList *wt    = 0;

   do{
      wt = (struct wftagList *) dblNextTableInstance(wt, Tree, dbl_LIST_WFTAG);
      if(!wt)break;
      if(!strcmp(wtIn->element->tagname, wt->element->tagname) )
         if( wtIn->element->tagid == wt->element->tagid )
            if( wtIn->element->wfid == wt->element->wfid ) 
               return wt;
   }while(wt);

   return 0;
}
/* ------------------------------------------------------------------ */




/* Add the contents of tree to tree2 emptying tree in the process. i.e.
   dis-attach pointers from tree and attach to tree2. 
*/
DBlist dblMergeTrees(DBlist tree2, DBlist tree, int takeEvid )
{
    struct CSStree *Tree;	
    struct CSStree *Tree2;

    struct wfdiscList *w    = 0 ;
    struct originList *o    = 0 ;
    struct eventList  *ev   = 0 , *ev2 = 0 ;
    struct arrivalList *a   = 0 ;
    struct siteList *s      = 0 ;
    struct siteList *s2 ;
    struct sitechanList *sc = 0 ;
    struct sitechanList *sc2 ;
    struct arrivalList  *ar2 ;
    struct gregionList   *gr = 0 , *gr2 = 0 ;
    struct wftagList    *wt  = 0 , *wt2 = 0 ;
	
    Tree  = (struct CSStree *) tree;
    Tree2 = (struct CSStree *) tree2;

/* Make sure Tree is not empty */	
    if(!Tree) {
       if( !Tree2 ) {
          /* error */
          dblSetError(1, "ERROR: Both tree and tree2 are empty in dblMergeTrees");
          return (DBlist) NULL ;
       }
       else {
          return tree2;
       }
    }

    if(!Tree2)    
       return tree ;

    if(Tree2->UserData)
       dblDestroyUserDataStruc(Tree2);

    Tree2->UserData = Tree->UserData;
    Tree->UserData = 0;


/* Eliminate any duplicate site or sitechan table instances... */
   do{
      s = (struct siteList *) dblNextTableInstance(s, Tree, dbl_LIST_SITE);
      if(!s)break;
      s2 = findMatchingSite(Tree2, s);
      if(s2)
         dblDeleteTableInstance(dbl_LIST_SITE, Tree2, s2);
   }while(s);

   do{
      sc = (struct sitechanList *) dblNextTableInstance(sc, Tree, dbl_LIST_SITECHAN);
      if(!sc)break;
      sc2 = findMatchingSitechan(Tree2, sc);
      if(sc2)
         dblDeleteTableInstance(dbl_LIST_SITECHAN, Tree2, sc2);
   }while(sc);

   /* Eliminate any duplicate arrival instances... */
   do{
      a = (struct arrivalList *) dblNextTableInstance(a, Tree, dbl_LIST_ARRIVAL);
      if(!a)break;
      ar2 = findMatchingArrival(Tree2, a);
      if(ar2)
         dblDeleteTableInstance(dbl_LIST_ARRIVAL, Tree2, ar2);
   }while(a);

   /* Eliminate any Event instances with identical event names.  */
   do{
      ev = (struct eventList *) dblNextTableInstance( ev, Tree, dbl_LIST_EVENT ) ;
      if( !ev ) break ;
      ev2 = findMatchingEvent( Tree2 , ev ) ;
      if( ev2 )
         dblDeleteTableInstance(dbl_LIST_EVENT, Tree2, ev2 ) ;
   }while( ev ) ;

   /* Eliminate any gregion instances with identical grn.  */
   do{
      gr = (struct gregionList *) dblNextTableInstance( gr, Tree, dbl_LIST_GREGION ) ;
      if( !gr ) break ;
      gr2 = findMatchingGregion( Tree2 , gr ) ;
      if( gr2 )
         dblDeleteTableInstance(dbl_LIST_GREGION, Tree2, gr2 ) ;
   }while( gr ) ;

   /* Eliminate any identical wftag instances.  */
   do{
      wt = (struct wftagList *) dblNextTableInstance( wt, Tree, dbl_LIST_WFTAG ) ;
      if( !wt ) break ;
      wt2 = findMatchingWftag( Tree2 , wt ) ;
      if( wt2 )
         dblDeleteTableInstance(dbl_LIST_WFTAG, Tree2, wt2 ) ;
   }while( wt ) ;


/* Before moving list elements need to make sure there are no conflicts between
   table keys in four lists... */

   do{
      w = (struct wfdiscList *) dblNextTableInstance(w, Tree, dbl_LIST_WFDISC);
      if(!w)break;
      if(dblWfidInUse(Tree2, w->element->wfid) ){
         int id1, id2 ;
   
         id1 = dblNextAvailableWfid(Tree) ;
         id2 = dblNextAvailableWfid(Tree2);
         id2 = id1 > id2 ? id1 : id2 ;

         dblReplaceWfid(Tree2, w->element->wfid, id2 ) ;
      }
   }while(w);
 

   do{
      struct originList *o2 = 0 ;

      o = (struct originList *) dblNextTableInstance(o, Tree, dbl_LIST_ORIGIN);
      if(!o)break;
      if((o2 = dblOridInUse(Tree2, o->element->orid))){
         if( takeEvid ) {
            dblDeleteTableInstance(dbl_LIST_ORIGIN, Tree2, o2 ) ;
         }
         else{
            int id1, id2 ;
   
            id1 = dblNextAvailableOrid(Tree) ;
            id2 = dblNextAvailableOrid(Tree2);
            id2 = id1 > id2 ? id1 : id2 ;

            dblReplaceOrid(Tree2, o->element->orid, id2 ) ;
         }
      }
   }while(o);


   do{
      ev = ( struct eventList *) dblNextTableInstance(ev, Tree, dbl_LIST_EVENT ) ;
      if( !ev ) break ;
      if((ev2 = dblEvidInUse(Tree2, ev->element->evid)) ) { 
         if( takeEvid ) {
            dblDeleteTableInstance(dbl_LIST_EVENT, Tree2, ev2 ) ;
         }
         else{
            int id1, id2 ;
   
            id1 = dblNextAvailableEvid(Tree) ;
            id2 = dblNextAvailableEvid(Tree2);
            id2 = id1 > id2 ? id1 : id2 ;

            dblReplaceEvid(Tree2, ev->element->evid, id2 ) ;
         }
      }
   }while(ev);

   do{
      a = (struct arrivalList *) dblNextTableInstance(a, Tree, dbl_LIST_ARRIVAL);
      if(!a)break;
      if(dblAridInUse(Tree2, a->element->arid) ){
         int id1, id2 ;

         id1 = dblNextAvailableArid(Tree) ;
         id2 = dblNextAvailableArid(Tree2);
         id2 = id1 > id2 ? id1 : id2 ;

         dblReplaceArid(Tree2, a->element->arid, id2 ) ;
      }
   }while(a);





    /* Move any affiliation structs... */
    if(Tree2->afTail && Tree->afHead){
       Tree2->afTail->next = Tree->afHead;
       Tree->afHead->prev  = Tree2->afTail;
       Tree2->afTail       = Tree->afTail;
       Tree->afHead        = 0;
       Tree->afTail        = 0;
    }
    else if(Tree->afHead){
       Tree2->afHead = Tree->afHead;
       Tree2->afTail = Tree->afTail;
       Tree->afHead  = 0;
       Tree->afTail  = 0;
    }

    /* Move any arrival structs... */
    if(Tree2->arTail && Tree->arHead){
       Tree2->arTail->next = Tree->arHead;
       Tree->arHead->prev  = Tree2->arTail;
       Tree2->arTail       = Tree->arTail;
       Tree->arHead        = 0;
       Tree->arTail        = 0;
    }
    else if(Tree->arHead){
       Tree2->arHead = Tree->arHead;
       Tree2->arTail = Tree->arTail;
       Tree->arHead  = 0;
       Tree->arTail  = 0;
    }

    /* Move any assoc structs... */
    if(Tree2->asTail && Tree->asHead){
       Tree2->asTail->next = Tree->asHead;
       Tree->asHead->prev  = Tree2->asTail;
       Tree2->asTail       = Tree->asTail;
       Tree->asHead        = 0;
       Tree->asTail        = 0;
    }
    else if(Tree->asHead){
       Tree2->asHead = Tree->asHead;
       Tree2->asTail = Tree->asTail;
       Tree->asHead  = 0;
       Tree->asTail  = 0;
    }

    /* Move any event structs... */
    if(Tree2->evTail && Tree->evHead){
       Tree2->evTail->next = Tree->evHead;
       Tree->evHead->prev  = Tree2->evTail;
       Tree2->evTail       = Tree->evTail;
       Tree->evHead        = 0;
       Tree->evTail        = 0;
    }
    else if(Tree->evHead){
       Tree2->evHead = Tree->evHead;
       Tree2->evTail = Tree->evTail;
       Tree->evHead  = 0;
       Tree->evTail  = 0;
    }

    /* Move any gregion structs... */
    if(Tree2->grTail && Tree->grHead){
       Tree2->grTail->next = Tree->grHead;
       Tree->grHead->prev  = Tree2->grTail;
       Tree2->grTail       = Tree->grTail;
       Tree->grHead        = 0;
       Tree->grTail        = 0;
    }
    else if(Tree->grHead){
       Tree2->grHead = Tree->grHead;
       Tree2->grTail = Tree->grTail;
       Tree->grHead  = 0;
       Tree->grTail  = 0;
    }

    /* Move any instrument structs... */
    if(Tree2->inTail && Tree->inHead){
       Tree2->inTail->next = Tree->inHead;
       Tree->inHead->prev  = Tree2->inTail;
       Tree2->inTail       = Tree->inTail;
       Tree->inHead        = 0;
       Tree->inTail        = 0;
    }
    else if(Tree->inHead){
       Tree2->inHead = Tree->inHead;
       Tree2->inTail = Tree->inTail;
       Tree->inHead  = 0;
       Tree->inTail  = 0;
    }

    /* Move any origerr structs... */
    if(Tree2->oeTail && Tree->oeHead){
       Tree2->oeTail->next = Tree->oeHead;
       Tree->oeHead->prev  = Tree2->oeTail;
       Tree2->oeTail       = Tree->oeTail;
       Tree->oeHead        = 0;
       Tree->oeTail        = 0;
    }
    else if(Tree->oeHead){
       Tree2->oeHead = Tree->oeHead;
       Tree2->oeTail = Tree->oeTail;
       Tree->oeHead  = 0;
       Tree->oeTail  = 0;
    }

    /* Move any origin structs... */
    if(Tree2->orTail && Tree->orHead){
       Tree2->orTail->next = Tree->orHead;
       Tree->orHead->prev  = Tree2->orTail;
       Tree2->orTail       = Tree->orTail;
       Tree->orHead        = 0;
       Tree->orTail        = 0;
    }
    else if(Tree->orHead){
       Tree2->orHead = Tree->orHead;
       Tree2->orTail = Tree->orTail;
       Tree->orHead  = 0;
       Tree->orTail  = 0;
    }

    /* Move any remark structs... */
    if(Tree2->reTail && Tree->reHead){
       Tree2->reTail->next = Tree->reHead;
       Tree->reHead->prev  = Tree2->reTail;
       Tree2->reTail       = Tree->reTail;
       Tree->reHead        = 0;
       Tree->reTail        = 0;
    }
    else if(Tree->reHead){
       Tree2->reHead = Tree->reHead;
       Tree2->reTail = Tree->reTail;
       Tree->reHead  = 0;
       Tree->reTail  = 0;
    }

    /* Move any sensor structs... */
    if(Tree2->seTail && Tree->seHead){
       Tree2->seTail->next = Tree->seHead;
       Tree->seHead->prev  = Tree2->seTail;
       Tree2->seTail       = Tree->seTail;
       Tree->seHead        = 0;
       Tree->seTail        = 0;
    }
    else if(Tree->seHead){
       Tree2->seHead = Tree->seHead;
       Tree2->seTail = Tree->seTail;
       Tree->seHead  = 0;
       Tree->seTail  = 0;
    }

    /* Move any site structs... */
    if(Tree2->slTail && Tree->slHead){
       Tree2->slTail->next = Tree->slHead;
       Tree->slHead->prev  = Tree2->slTail;
       Tree2->slTail       = Tree->slTail;
       Tree->slHead        = 0;
       Tree->slTail        = 0;
    }
    else if(Tree->slHead){
       Tree2->slHead = Tree->slHead;
       Tree2->slTail = Tree->slTail;
       Tree->slHead  = 0;
       Tree->slTail  = 0;
    }

    /* Move any sitechan structs... */
    if(Tree2->scTail && Tree->scHead){
       Tree2->scTail->next = Tree->scHead;
       Tree->scHead->prev  = Tree2->scTail;
       Tree2->scTail       = Tree->scTail;
       Tree->scHead        = 0;
       Tree->scTail        = 0;
    }
    else if(Tree->scHead){
       Tree2->scHead = Tree->scHead;
       Tree2->scTail = Tree->scTail;
       Tree->scHead  = 0;
       Tree->scTail  = 0;
    }

    /* Move any stassoc structs... */
    if(Tree2->saTail && Tree->saHead){
       Tree2->saTail->next = Tree->saHead;
       Tree->saHead->prev  = Tree2->saTail;
       Tree2->saTail       = Tree->saTail;
       Tree->saHead        = 0;
       Tree->saTail        = 0;
    }
    else if(Tree->saHead){
       Tree2->saHead = Tree->saHead;
       Tree2->saTail = Tree->saTail;
       Tree->saHead  = 0;
       Tree->saTail  = 0;
    }

    /* Move any wfdisc structs... */
    if(Tree2->wfTail && Tree->wfHead){
       Tree2->wfTail->next = Tree->wfHead;
       Tree->wfHead->prev  = Tree2->wfTail;
       Tree2->wfTail       = Tree->wfTail;
       Tree->wfHead        = 0;
       Tree->wfTail        = 0;
    }
    else if(Tree->wfHead){
       Tree2->wfHead = Tree->wfHead;
       Tree2->wfTail = Tree->wfTail;
       Tree->wfHead  = 0;
       Tree->wfTail  = 0;
    }

    /* Move any wftag structs... */
    if(Tree2->wtTail && Tree->wtHead){
       Tree2->wtTail->next = Tree->wtHead;
       Tree->wtHead->prev  = Tree2->wtTail;
       Tree2->wtTail       = Tree->wtTail;
       Tree->wtHead        = 0;
       Tree->wtTail        = 0;
    }
    else if(Tree->wtHead){
       Tree2->wtHead = Tree->wtHead;
       Tree2->wtTail = Tree->wtTail;
       Tree->wtHead  = 0;
       Tree->wtTail  = 0;
    }

    /* Move any sacdata structs... */
    if(Tree2->sdTail && Tree->sdHead){
       Tree2->sdTail->next = Tree->sdHead;
       Tree->sdHead->prev  = Tree2->sdTail;
       Tree2->sdTail       = Tree->sdTail;
       Tree->sdHead        = 0;
       Tree->sdTail        = 0;
    }
    else if(Tree->sdHead){
       Tree2->sdHead = Tree->sdHead;
       Tree2->sdTail = Tree->sdTail;
       Tree->sdHead  = 0;
       Tree->sdTail  = 0;
    }


    return (DBlist) Tree2;
} /* end dblMergeTrees() */
/* ------------------------------------------------------------------------ */




int dblGetNumWaveformsInMemory(DBlist tree)
{         
   struct wfdiscList  *wf = 0;
   int Num = 0;

   do{
      if(!( wf = (struct wfdiscList  *) dblNextTableInstance(wf, tree, dbl_LIST_WFDISC) ) )break;
      Num++;
   }while(wf);
   return Num;
}
/* ---------------------------------------------------------------------------------- */            



static int CHRlen(char *str)
{
   int j;
   int L;
   if(!str)return 0;
   L = strlen(str);
   for(j=0;j<L;j++)
      if(isdigit(str[j]))return j;
   
   return L;
}
/* ------------------------------------------------------------------ */




static char *MkTmpName(char *OldName, int Num)
{
   static char Name[10];
   char tmp[10];
   if(!OldName) {
       return NULL;
   }
   int L = CHRlen(OldName);
   sprintf(tmp, "%02X", Num);

   strcpy(Name, OldName);
   if(L < 5){
      strcpy(Name + L, tmp);
   }
   else
      strcpy(Name + 4, tmp);
   return Name;
}
/* ------------------------------------------------------------------ */



static int UniqueSiteName(DBlist tree, char *name)
{
   struct siteList *si = 0;

   do{
      si = (struct siteList *) dblNextTableInstance(si, tree, dbl_LIST_SITE);
      if(!si)break;
      if(! CSSstrcmp(si->element->sta, name) ){
         return 0;
      }
   }while(si);
   return 1;
}
/* ------------------------------------------------------------------ */





char *MakeUniqueSiteName(DBlist tree, char *NewName)
{
   int NameInList      = 0;
   struct siteList *si = 0;
   static int SiNum    = 0;

   /* First see if proposed name is already in site list... */
   do{
      si = (struct siteList *) dblNextTableInstance(si, tree, dbl_LIST_SITE);
      if(!si)break;
      if(! CSSstrcmp(si->element->sta, NewName) ){
         NameInList = 1;
         break;
      }
   }while(si);

   if(!NameInList) return NewName;

   while( !UniqueSiteName(tree,  MkTmpName(NewName, ++SiNum) ) );

   return MkTmpName(NewName, SiNum);
}
/* ------------------------------------------------------------------ */

   




static int UniqueChanName(DBlist tree, char *sta, char *chan)
{
   struct sitechanList *sc = 0;

   do{
      sc = (struct sitechanList *) dblNextTableInstance(sc, tree, dbl_LIST_SITECHAN);
      if(!sc)break;
      if(! CSSstrcmp(sc->element->sta, sta) ) 
	 if(!CSSstrcmp(sc->element->chan, chan) )
            return 0;
   }while(sc);
   return 1;
}
/* ------------------------------------------------------------------ */





char *MakeUniqueChanName(DBlist tree, char *sta, char *NewChan)
{
   int ChanInList          = 0;
   struct sitechanList *sc = 0;
   static int ScNum        = 0;

   /* First see if proposed sta, chan is already in sitechan list... */
   do{
      sc = (struct sitechanList *) dblNextTableInstance(sc, tree, dbl_LIST_SITECHAN);
      if(!sc)break;
      if(! CSSstrcmp(sc->element->sta, sta) )
	 if(!CSSstrcmp(sc->element->chan, NewChan) ){
            ChanInList = 1;
            break;
         }
   }while(sc);

   if(!ChanInList) return NewChan;

   while( !UniqueChanName(tree,  sta, MkTmpName(NewChan, ++ScNum) ) );

   return MkTmpName(NewChan, ScNum);
}
/* ------------------------------------------------------------------ */





int  GetCalibCalperFromWfdisc(DBlist tree, int wfid, double *calib, double* calper )
{
   struct wfdiscList *w = 0;
   *calib  = CALIB_UNDEF;
   *calper = CALPER_UNDEF;
   do{
      w = (struct wfdiscList *) dblNextTableInstance(w, tree, dbl_LIST_WFDISC);
      if(!w)break;
      if(w->element->wfid == wfid){
         *calib  = w->element->calib;
         *calper = w->element->calper;
         return 1;
      }
   }while(w);

   return 0;
}
/* ------------------------------------------------------------------ */

   
