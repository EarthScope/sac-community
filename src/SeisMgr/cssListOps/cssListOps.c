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

void * smGetDefaultTree ( void ) ;
/* ++++++++++++++ END FUNCTION PROTOTYPES FOR THIS FILE ++++++++++++ */




static int dblIndex = 0;

static int dblNextIndex(void)
{
   dblIndex++;
   return dblIndex;
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

   
