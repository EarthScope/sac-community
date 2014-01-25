#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NULL_SUDS 
#include "suds/sudsListOps.h"


void FreeSTList(stList *stHead)
{
   stList *ptr;
   ptr = stHead;
   while(ptr){
      stHead = ptr->next;
      free(ptr->st);
      free(ptr);
      ptr = stHead;
   }
}
/* ------------------------------------------------------- */





void FreeADList(adList *adHead)
{
   adList *ptr;
   ptr = adHead;
   while(ptr){
      adHead = ptr->next;
      free(ptr->ad);
      free(ptr);
      ptr = adHead;
   }
}
/* ------------------------------------------------------- */






void FreeCAList(caList *caHead)
{
   caList *ptr;
   ptr = caHead;
   while(ptr){
      caHead = ptr->next;
      free(ptr->ca);
      free(ptr);
      ptr = caHead;
   }
}
/* ------------------------------------------------------- */





void FreeCOList(coList *coHead)
{
   coList *ptr;
   ptr = coHead;
   while(ptr){
      coHead = ptr->next;
      if(ptr->text)free(ptr->text);
      free(ptr->co);
      free(ptr);
      ptr = coHead;
   }
}
/* ------------------------------------------------------- */





void FreeCSList(csList *csHead)
{
   csList *ptr;
   ptr = csHead;
   while(ptr){
      csHead = ptr->next;
      if(ptr->entry)free(ptr->entry);
      free(ptr->cs);
      free(ptr);
      ptr = csHead;
   }
}
/* ------------------------------------------------------- */





void FreeDTList(dtList *dtHead)
{
   dtList *ptr;
   ptr = dtHead;
   while(ptr){
      dtHead = ptr->next;
      if(ptr->i2data)free(ptr->i2data);
      if(ptr->i4data)free(ptr->i4data);
      if(ptr->r4data)free(ptr->r4data);
      if(ptr->r8data)free(ptr->r8data);
      free(ptr->dt);
      free(ptr);
      ptr = dtHead;
   }
}
/* ------------------------------------------------------- */






void FreeDEList(deList *deHead)
{
   deList *ptr;
   ptr = deHead;
   while(ptr){
      deHead = ptr->next;
      free(ptr->de);
      free(ptr);
      ptr = deHead;
   }
}
/* ------------------------------------------------------- */






void FreeEQList(eqList *eqHead)
{
   eqList *ptr;
   ptr = eqHead;
   while(ptr){
      eqHead = ptr->next;
      free(ptr->eq);
      free(ptr);
      ptr = eqHead;
   }
}
/* ------------------------------------------------------- */






void FreeERList(erList *erHead)
{
   erList *ptr;
   ptr = erHead;
   while(ptr){
      erHead = ptr->next;
      free(ptr->er);
      free(ptr);
      ptr = erHead;
   }
}
/* ------------------------------------------------------- */






void FreeEVList(evList *evHead)
{
   evList *ptr;
   ptr = evHead;
   while(ptr){
      evHead = ptr->next;
      free(ptr->ev);
      free(ptr);
      ptr = evHead;
   }
}
/* ------------------------------------------------------- */






void FreeESList(esList *Head)
{
   esList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->es);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */






void FreeEDList(edList *Head)
{
   edList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->ed);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */






void FreeFEList(feList *Head)
{
   feList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->fe);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */






void FreeFOList(foList *Head)
{
   foList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->fo);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */






void FreeINList(inList *Head)
{
   inList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->in);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */





void FreeLAList(laList *Head)
{
   laList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->la);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */





void FreeLOList(loList *Head)
{
   loList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->lo);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */





void FreeMOList(moList *Head)
{
   moList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->mo);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */





void FreeMUList(muList *Head)
{
   muList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      if(ptr->i2data)free(ptr->i2data);
      if(ptr->i4data)free(ptr->i4data);
      if(ptr->r4data)free(ptr->r4data);
      if(ptr->r8data)free(ptr->r8data);
      free(ptr->mu);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */





void FreeORList(orList *Head)
{
   orList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->orig);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */






void FreePRList(prList *Head)
{
   prList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->pr);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */






void FreeREList(reList *Head)
{
   reList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->re);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */






void FreeSHList(shList *Head)
{
   shList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->sh);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */






void FreeSCList(scList *Head)
{
   scList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->sc);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */






void FreeTEList(teList *Head)
{
   teList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->te);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */






void FreeTCList(tcList *Head)
{
   tcList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->tc);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */






void FreeTRList(trList *Head)
{
   trList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->tr);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */






void FreeTSList(tsList *Head)
{
   tsList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->ts);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */






void FreeVMList(vmList *Head)
{
   vmList *ptr = Head;
   while(ptr){
      Head = ptr->next;
      free(ptr->vm);
      free(ptr);
      ptr = Head;
   }
}
/* ------------------------------------------------------- */





SUDS *NewSudsList()
{
   SUDS *S;
   S = (SUDS *) calloc(1, sizeof(SUDS) );

   return S;
}
/* ------------------------------------------------------- */






void FreeSuds(SUDS *S)
{
   if(!S)return;

   if(S->stHead)FreeSTList(S->stHead);
   if(S->adHead)FreeADList(S->adHead);
   if(S->caHead)FreeCAList(S->caHead);
   if(S->coHead)FreeCOList(S->coHead);
   if(S->csHead)FreeCSList(S->csHead);
   if(S->dtHead)FreeDTList(S->dtHead);
   if(S->deHead)FreeDEList(S->deHead);
   if(S->eqHead)FreeEQList(S->eqHead);
   if(S->erHead)FreeERList(S->erHead);
   if(S->evHead)FreeEVList(S->evHead);
   if(S->esHead)FreeESList(S->esHead);
   if(S->edHead)FreeEDList(S->edHead);
   if(S->feHead)FreeFEList(S->feHead);
   if(S->foHead)FreeFOList(S->foHead);
   if(S->inHead)FreeINList(S->inHead);
   if(S->laHead)FreeLAList(S->laHead);
   if(S->loHead)FreeLOList(S->loHead);
   if(S->moHead)FreeMOList(S->moHead);
   if(S->muHead)FreeMUList(S->muHead);
   if(S->orHead)FreeORList(S->orHead);
   if(S->prHead)FreePRList(S->prHead);
   if(S->reHead)FreeREList(S->reHead);
   if(S->shHead)FreeSHList(S->shHead);
   if(S->scHead)FreeSCList(S->scHead);
   if(S->teHead)FreeTEList(S->teHead);
   if(S->tcHead)FreeTCList(S->tcHead);
   if(S->trHead)FreeTRList(S->trHead);
   if(S->tsHead)FreeTSList(S->tsHead);
   if(S->vmHead)FreeVMList(S->vmHead);
   free(S);
   S = 0;
   return;
}
/* ------------------------------------------------------- */






void *AddSudsListElement(SUDS *S, short StructID) 
{
   deList *newDE;
   adList *newAD;
   tsList *newTS;
   esList *newES;
   scList *newSC;
   dtList *newDT;
   feList *newFE;
   caList *newCA;
   coList *newCO;
   evList *newEV;
   trList *newTR;
   edList *newED;
   foList *newFO;
   inList *newIN;
   orList *newOR;
   tcList *newTC;
   stList *newST;
   csList *newCS;
   eqList *newEQ;
   erList *newER;
   laList *newLA;
   loList *newLO;
   moList *newMO;
   muList *newMU;
   prList *newPR;
   reList *newRE;
   shList *newSH;
   teList *newTE;
   vmList *newVM;

   switch (StructID){
   
   case DETECTOR:
      newDE = (deList *) malloc(sizeof(deList) );
      newDE->de = (SUDS_DETECTOR *) malloc( sizeof(SUDS_DETECTOR));
      *(newDE->de) = NullDE;
      newDE->next = 0;
      if(S->deTail){
         S->deTail->next = newDE;
         S->deTail = newDE;
      }
      else{
         S->deHead = newDE;
         S->deTail = newDE;
      }
      return (void *)(newDE->de);

   case ATODINFO:
      newAD = (adList *) malloc(sizeof(adList) );
      newAD->ad = (SUDS_ATODINFO *) malloc( sizeof(SUDS_ATODINFO));
      *(newAD->ad) = NullAD;
      newAD->next = 0;
      if(S->adTail){
         S->adTail->next = newAD;
         S->adTail = newAD;
      }
      else{
         S->adHead = newAD;
         S->adTail = newAD;
      }
      return (void *)(newAD->ad);


   case TRIGSETTING:
      newTS = (tsList *) malloc(sizeof(tsList) );
      newTS->ts = (SUDS_TRIGSETTING *) malloc( sizeof(SUDS_TRIGSETTING));
      *(newTS->ts) = NullTS;
      newTS->next = 0;
      if(S->tsTail){
         S->tsTail->next = newTS;
         S->tsTail = newTS;
      }
      else{
         S->tsHead = newTS;
         S->tsTail = newTS;
      }
      return (void *)(newTS->ts);


   case EVENTSETTING:
      newES = (esList *) malloc(sizeof(esList) );
      newES->es = (SUDS_EVENTSETTING *) malloc( sizeof(SUDS_EVENTSETTING));
      *(newES->es) = NullES;
      newES->next = 0;
      if(S->esTail){
         S->esTail->next = newES;
         S->esTail = newES;
      }
      else{
         S->esHead = newES;
         S->esTail = newES;
      }
      return (void *)(newES->es);


   case STATIONCOMP:
      newSC = (scList *) malloc(sizeof(scList) );
      newSC->sc = (SUDS_STATIONCOMP *) malloc( sizeof(SUDS_STATIONCOMP));
      *(newSC->sc) = NullSC;
      newSC->next = 0;
      if(S->scTail){
         S->scTail->next = newSC;
         S->scTail = newSC;
      }
      else{
         S->scHead = newSC;
         S->scTail = newSC;
      }
      return (void *)(newSC->sc);

   case DESCRIPTRACE:
      newDT = (dtList *) malloc(sizeof(dtList) );
      newDT->dt = (SUDS_DESCRIPTRACE *) malloc( sizeof(SUDS_DESCRIPTRACE));
      *(newDT->dt) = NullDT;
      newDT->next = 0;
      newDT->i2data = 0;
      newDT->i4data = 0;
      newDT->r4data = 0;
      newDT->r8data = 0;

      if(S->dtTail){
         S->dtTail->next = newDT;
         S->dtTail = newDT;
      }
      else{
         S->dtHead = newDT;
         S->dtTail = newDT;
      }
      return (void *)(newDT->dt);

   case FEATURE:
      newFE = (feList *) malloc(sizeof(feList) );
      newFE->fe = (SUDS_FEATURE *) malloc( sizeof(SUDS_FEATURE));
      *(newFE->fe) = NullFE;
      newFE->next = 0;
      if(S->feTail){
         S->feTail->next = newFE;
         S->feTail = newFE;
      }
      else{
         S->feHead = newFE;
         S->feTail = newFE;
      }
      return (void *)(newFE->fe);

   case CALIBRATION:
      newCA = (caList *) malloc(sizeof(caList) );
      newCA->ca = (SUDS_CALIBRATION *) malloc( sizeof(SUDS_CALIBRATION));
      *(newCA->ca) = NullCA;
      newCA->next = 0;
      if(S->caTail){
         S->caTail->next = newCA;
         S->caTail = newCA;
      }
      else{
         S->caHead = newCA;
         S->caTail = newCA;
      }
      return (void *)(newCA->ca);


   case COMMENT:
      newCO = (coList *) malloc(sizeof(coList) );
      newCO->co = (SUDS_COMMENT *) malloc( sizeof(SUDS_COMMENT));
      *(newCO->co) = NullCO;
      newCO->next = 0;
      if(S->coTail){
         S->coTail->next = newCO;
         S->coTail = newCO;
      }
      else{
         S->coHead = newCO;
         S->coTail = newCO;
      }
      return (void *)(newCO->co);


   case EVENT:
      newEV = (evList *) malloc(sizeof(evList) );
      newEV->ev = (SUDS_EVENT *) malloc( sizeof(SUDS_EVENT));
      *(newEV->ev) = NullEV;
      newEV->next = 0;
      if(S->evTail){
         S->evTail->next = newEV;
         S->evTail = newEV;
      }
      else{
         S->evHead = newEV;
         S->evTail = newEV;
      }
      return (void *)(newEV->ev);

   case TRIGGERS:
      newTR = (trList *) malloc(sizeof(trList) );
      newTR->tr = (SUDS_TRIGGERS *) malloc( sizeof(SUDS_TRIGGERS));
      *(newTR->tr) = NullTR;
      newTR->next = 0;
      if(S->trTail){
         S->trTail->next = newTR;
         S->trTail = newTR;
      }
      else{
         S->trHead = newTR;
         S->trTail = newTR;
      }
      return (void *)(newTR->tr);


   case EV_DESCRIPT:
      newED = (edList *) malloc(sizeof(edList) );
      newED->ed = (SUDS_EVDESCR *) malloc( sizeof(SUDS_EVDESCR));
      *(newED->ed) = NullED;
      newED->next = 0;
      if(S->edTail){
         S->edTail->next = newED;
         S->edTail = newED;
      }
      else{
         S->edHead = newED;
         S->edTail = newED;
      }
      return (void *)(newED->ed);

   case FOCALMECH:
      newFO = (foList *) malloc(sizeof(foList) );
      newFO->fo = (SUDS_FOCALMECH *) malloc( sizeof(SUDS_FOCALMECH));
      *(newFO->fo) = NullFO;
      newFO->next = 0;
      if(S->foTail){
         S->foTail->next = newFO;
         S->foTail = newFO;
      }
      else{
         S->foHead = newFO;
         S->foTail = newFO;
      }
      return (void *)(newFO->fo);

   case INSTRUMENT:
      newIN = (inList *) malloc(sizeof(inList) );
      newIN->in = (SUDS_INSTRUMENT *) malloc( sizeof(SUDS_INSTRUMENT));
      *(newIN->in) = NullIN;
      newIN->next = 0;
      if(S->inTail){
         S->inTail->next = newIN;
         S->inTail = newIN;
      }
      else{
         S->inHead = newIN;
         S->inTail = newIN;
      }
      return (void *)(newIN->in);


   case ORIGIN:
      newOR = (orList *) malloc(sizeof(orList) );
      newOR->orig = (SUDS_ORIGIN *) malloc( sizeof(SUDS_ORIGIN));
      *(newOR->orig) = NullOR;
      newOR->next = 0;
      if(S->orTail){
         S->orTail->next = newOR;
         S->orTail = newOR;
      }
      else{
         S->orHead = newOR;
         S->orTail = newOR;
      }
      return (void *)(newOR->orig);

   case TIMECORRECTION:
      newTC = (tcList *) malloc(sizeof(tcList) );
      newTC->tc = (SUDS_TIMECORRECTION *) malloc( sizeof(SUDS_TIMECORRECTION));
      *(newTC->tc) = NullTC;
      newTC->next = 0;
      if(S->tcTail){
         S->tcTail->next = newTC;
         S->tcTail = newTC;
      }
      else{
         S->tcHead = newTC;
         S->tcTail = newTC;
      }
      return (void *)(newTC->tc);

   case STAT_IDENT:
      newST = (stList *) malloc(sizeof(stList) );
      newST->st = (SUDS_STATIDENT *) malloc( sizeof(SUDS_STATIDENT));
      *(newST->st) = NullST;
      newST->next = 0;
      if(S->stTail){
         S->stTail->next = newST;
         S->stTail = newST;
      }
      else{
         S->stHead = newST;
         S->stTail = newST;
      }
      return (void *)(newST->st);

   case CHANSET:
      newCS = (csList *) malloc(sizeof(csList) );
      newCS->cs = (SUDS_CHANSET *) malloc( sizeof(SUDS_CHANSET));
      *(newCS->cs) = NullCS;
      newCS->next = 0;
      if(S->csTail){
         S->csTail->next = newCS;
         S->csTail = newCS;
      }
      else{
         S->csHead = newCS;
         S->csTail = newCS;
      }
      return (void *)(newCS->cs);

   case EQUIPMENT:
      newEQ = (eqList *) malloc(sizeof(eqList) );
      newEQ->eq = (SUDS_EQUIPMENT *) malloc( sizeof(SUDS_EQUIPMENT));
      *(newEQ->eq) = NullEQ;
      newEQ->next = 0;
      if(S->eqTail){
         S->eqTail->next = newEQ;
         S->eqTail = newEQ;
      }
      else{
         S->eqHead = newEQ;
         S->eqTail = newEQ;
      }
      return (void *)(newEQ->eq);

   case ERROR:
      newER = (erList *) malloc(sizeof(erList) );
      newER->er = (SUDS_ERROR *) malloc( sizeof(SUDS_ERROR));
      *(newER->er) = NullER;
      newER->next = 0;
      if(S->erTail){
         S->erTail->next = newER;
         S->erTail = newER;
      }
      else{
         S->erHead = newER;
         S->erTail = newER;
      }
      return (void *)(newER->er);


   case LAYERS:
      newLA = (laList *) malloc(sizeof(laList) );
      newLA->la = (SUDS_LAYERS *) malloc( sizeof(SUDS_LAYERS));
      *(newLA->la) = NullLA;
      newLA->next = 0;
      if(S->laTail){
         S->laTail->next = newLA;
         S->laTail = newLA;
      }
      else{
         S->laHead = newLA;
         S->laTail = newLA;
      }
      return (void *)(newLA->la);

   case LOCTRACE:
      newLO = (loList *) malloc(sizeof(loList) );
      newLO->lo = (SUDS_LOCTRACE *) malloc( sizeof(SUDS_LOCTRACE));
      *(newLO->lo) = NullLO;
      newLO->next = 0;
      if(S->loTail){
         S->loTail->next = newLO;
         S->loTail = newLO;
      }
      else{
         S->loHead = newLO;
         S->loTail = newLO;
      }
      return (void *)(newLO->lo);

   case MOMENT:
      newMO = (moList *) malloc(sizeof(moList) );
      newMO->mo = (SUDS_MOMENT *) malloc( sizeof(SUDS_MOMENT));
      *(newMO->mo) = NullMO;
      newMO->next = 0;
      if(S->moTail){
         S->moTail->next = newMO;
         S->moTail = newMO;
      }
      else{
         S->moHead = newMO;
         S->moTail = newMO;
      }
      return (void *)(newMO->mo);

   case MUXDATA:
      newMU = (muList *) malloc(sizeof(muList) );
      newMU->mu = (SUDS_MUXDATA *) malloc( sizeof(SUDS_MUXDATA));
      *(newMU->mu) = NullMU;
      newMU->i2data = 0;
      newMU->i4data = 0;
      newMU->r4data = 0;
      newMU->r8data = 0;
      newMU->next = 0;
      if(S->muTail){
         S->muTail->next = newMU;
         S->muTail = newMU;
      }
      else{
         S->muHead = newMU;
         S->muTail = newMU;
      }
      return (void *)(newMU->mu);

   case PROFILE:
      newPR = (prList *) malloc(sizeof(prList) );
      newPR->pr = (SUDS_PROFILE *) malloc( sizeof(SUDS_PROFILE));
      *(newPR->pr) = NullPR;
      newPR->next = 0;
      if(S->prTail){
         S->prTail->next = newPR;
         S->prTail = newPR;
      }
      else{
         S->prHead = newPR;
         S->prTail = newPR;
      }
      return (void *)(newPR->pr);

   case RESIDUAL:
      newRE = (reList *) malloc(sizeof(reList) );
      newRE->re = (SUDS_RESIDUAL *) malloc( sizeof(SUDS_RESIDUAL));
      *(newRE->re) = NullRE;
      newRE->next = 0;
      if(S->reTail){
         S->reTail->next = newRE;
         S->reTail = newRE;
      }
      else{
         S->reHead = newRE;
         S->reTail = newRE;
      }
      return (void *)(newRE->re);

   case SHOTGATHER:
      newSH = (shList *) malloc(sizeof(shList) );
      newSH->sh = (SUDS_SHOTGATHER *) malloc( sizeof(SUDS_SHOTGATHER));
      *(newSH->sh) = NullSH;
      newSH->next = 0;
      if(S->shTail){
         S->shTail->next = newSH;
         S->shTail = newSH;
      }
      else{
         S->shHead = newSH;
         S->shTail = newSH;
      }
      return (void *)(newSH->sh);

   case TERMINATOR:
      newTE = (teList *) malloc(sizeof(teList) );
      newTE->te = (SUDS_TERMINATOR *) malloc( sizeof(SUDS_TERMINATOR));
      *(newTE->te) = NullTE;
      newTE->next = 0;
      if(S->teTail){
         S->teTail->next = newTE;
         S->teTail = newTE;
      }
      else{
         S->teHead = newTE;
         S->teTail = newTE;
      }
      return (void *)(newTE->te);

   case VELMODEL:
      newVM = (vmList *) malloc(sizeof(vmList) );
      newVM->vm = (SUDS_VELMODEL *) malloc( sizeof(SUDS_VELMODEL));
      *(newVM->vm) = NullVM;
      newVM->next = 0;
      if(S->vmTail){
         S->vmTail->next = newVM;
         S->vmTail = newVM;
      }
      else{
         S->vmHead = newVM;
         S->vmTail = newVM;
      }
      return (void *)(newVM->vm);



   }

   return 0;
}
/* ------------------------------------------------------- */




void AllocateArray(dtList *dt)
{


   switch (dt->dt->datatype){

   case 's': case 'q': case 'u': case 'i':
      dt->i2data = (short *) malloc( dt->dt->length * sizeof(short) );
      if(!dt->i2data){
         printf("ERROR: Could not allocate data array in AllocateArray.\n");
         exit(1);
      }
      return;

   case '2': case 'l':
      dt->i4data = (int *) malloc( dt->dt->length * sizeof(int) );
      if(!dt->i4data){
         printf("ERROR: Could not allocate data array in AllocateArray.\n");
         exit(1);
      }
      return;

   case 'f':
      dt->r4data = (float *) malloc( dt->dt->length * sizeof(float) );
      if(!dt->r4data){
         printf("ERROR: Could not allocate data array in AllocateArray.\n");
         exit(1);
      }
      return;

   case 'd':
      dt->r8data = (double *) malloc( dt->dt->length * sizeof(double) );
      if(!dt->r8data){
         printf("ERROR: Could not allocate data array in AllocateArray.\n");
         exit(1);
      }
         return;
   }
}
/* ------------------------------------------------------- */





void Mux2Trace(SUDS *S, muList *mu, int Nsamples, int BlockNumber)
{
   int j = 0;
   int trcIndex = BlockNumber * mu->mu->blocksize;
   int blkIndex;
   dtList *dt = S->dtHead;
     

   while(dt){
     if(!BlockNumber){  /* first time through the dt structs... */
        dt->dt->begintime = mu->mu->begintime;
        dt->dt->localtime = mu->mu->loctime;
        dt->dt->datatype  = mu->mu->typedata;
        dt->dt->rate      = mu->mu->dig_rate;
        dt->dt->descriptor= mu->mu->descript;
        dt->dt->length = Nsamples;
        AllocateArray(dt);
     }
     blkIndex = (j++) * mu->mu->blocksize;
     if(mu->i2data){
         short *from = mu->i2data + blkIndex;
         short *to   = dt->i2data + trcIndex;
         if(to && from) {
             memcpy(to, from, mu->mu->blocksize * sizeof(short));
         }
     }
     else if(mu->i4data){
         int *from = mu->i4data + blkIndex;
         int *to   = dt->i4data + trcIndex;
         if(to && from) {
             memcpy( to, from, mu->mu->blocksize * sizeof(int));
         }
     }
     else if(mu->r4data){
         float *from = mu->r4data + blkIndex;
         float *to   = dt->r4data + trcIndex;
         if(to && from) {
             memcpy( to, from, mu->mu->blocksize * sizeof(float));
         }
     }
     else if(mu->r8data){
         double *from = mu->r8data + blkIndex;
         double *to   = dt->r8data + trcIndex;
         if(to && from) {
             memcpy( to, from, mu->mu->blocksize * sizeof(float));
         }
     }
     dt = dt->next;
   }



}
/* ------------------------------------------------------- */







int Demux(SUDS *S)
{
   int Nsamples;
   scList *sc;
   SUDS_DESCRIPTRACE *dt;
   muList *mu;
   int BlockNumber = 0;

   /* First free any pre-existing descriptrace struc list. */
   if(S->dtHead){
     FreeDTList(S->dtHead);
     S->dtHead = 0;
   }

   /* Make sure there are stationcomp structs... */
   sc =  S->scHead;
   if(!sc) {
      printf("ERROR: No stationcomp structs so cannot demux traces.\n");
      return 0;
   }

   /* Now get the total length of each dt array */
   Nsamples = 0;
   mu =  S->muHead;
   while(mu){
      Nsamples += mu->mu->blocksize;
      mu = mu->next;
   }
   if(!Nsamples){
      printf("ERROR: No data in muxdata structs so cannot demux traces.\n");
      return 0;
   }



   /* Now create a list of descriptrace structs from the stationcomp list. */
   while(sc){
      dt = (SUDS_DESCRIPTRACE *) AddSudsListElement(S, DESCRIPTRACE);
      dt->dt_name = sc->sc->sc_name;
      sc = sc->next;
   }


   /* Now step through muxdata list again, and for each struct copy its
      data into all the trace structs. */
   mu =  S->muHead;
   while(mu){
      Mux2Trace(S, mu, Nsamples, BlockNumber++);
      mu = mu->next;
   }


   /* Now destroy the muxdata list. */
   FreeMUList(S->muHead);
   S->muHead = 0;



   return 1;



}
/* ------------------------------------------------------- */





char *Instcode(short code)
{
   static char Inst[40];

   if(code == 0 )
         strcpy(Inst, "not specified");
   else if(code == 1)
         strcpy(Inst, "sp usgs");
   else if(code == 2)
         strcpy(Inst, "sp wwssn");
   else if(code == 3)
         strcpy(Inst, "lp wwssn");
   else if(code == 4)
         strcpy(Inst, "sp dwwssn");
   else if(code == 5)
         strcpy(Inst, "lp dwwssn");
   else if(code == 6)
         strcpy(Inst, "hglp lamont");
   else if(code == 7)
         strcpy(Inst, "lp hglp lamont");
   else if(code == 8)
         strcpy(Inst, "sp sro");
   else if(code == 9)
         strcpy(Inst, "lp sro");
   else if(code == 10)
         strcpy(Inst, "sp asro");
   else if(code == 11)
         strcpy(Inst, "lp asro");
   else if(code == 12)
         strcpy(Inst, "sp rstn");
   else if(code == 13)
         strcpy(Inst, "lp rstn");
   else if(code == 14)
         strcpy(Inst, "sp uofa U of Alaska");
   else if(code == 15)
         strcpy(Inst, "STS-1/UVBB");
   else if(code == 16)
         strcpy(Inst, "STS-1/VBB");
   else if(code == 17)
         strcpy(Inst, "STS-2");
   else if(code == 18)
         strcpy(Inst, "FBA-23");
   else if(code == 19)
         strcpy(Inst, "Wilcoxin");
   else if(code == 50)
         strcpy(Inst, "USGS cassette");
   else if(code == 51)
         strcpy(Inst, "GEOS");
   else if(code == 52)
         strcpy(Inst, "EDA");
   else if(code == 53)
         strcpy(Inst, "Sprengnether refraction");
   else if(code == 54)
         strcpy(Inst, "Teledyne refraction");
   else if(code == 55)
         strcpy(Inst, "Kinemetrics refraction");
   else if(code == 300)
         strcpy(Inst, "amplifier");
   else if(code == 301)
         strcpy(Inst, "amp/vco");
   else if(code == 302)
         strcpy(Inst, "filter");
   else if(code == 303)
         strcpy(Inst, "summing amp");
   else if(code == 304)
         strcpy(Inst, "transmitter");
   else if(code == 305)
         strcpy(Inst, "receiver");
   else if(code == 306)
         strcpy(Inst, "antenna");
   else if(code == 307)
         strcpy(Inst, "battery");
   else if(code == 308)
         strcpy(Inst, "solar cell");
   else if(code == 309)
         strcpy(Inst, "discriminator");
   else if(code == 310)
         strcpy(Inst, "discr, rack");
   else if(code == 311)
         strcpy(Inst, "paper recorder");
   else if(code == 312)
         strcpy(Inst, "film recorder");
   else if(code == 313)
         strcpy(Inst, "smoked glass recorder");
   else if(code == 314)
         strcpy(Inst, "atod convertor");
   else if(code == 315)
         strcpy(Inst, "computer");
   else if(code == 316)
         strcpy(Inst, "clock");
   else if(code == 317)
         strcpy(Inst, "time receiver");
   else if(code == 318)
         strcpy(Inst, "magnetic tape");
   else if(code == 319)
         strcpy(Inst, "magntic disk");
   else if(code == 320)
         strcpy(Inst, "optical disk");
   else
         strcpy(Inst,  "Unknown");

   return Inst;
}
/* ------------------------------------------------------- */










char *Phasecode(short code)
{
   static char PhaseName[20];

   if(code == 0 )
         strcpy(PhaseName, "none");
   else if(code == 1)
         strcpy(PhaseName, "window");
   else if(code == 2)
         strcpy(PhaseName, "f finis");
   else if(code == 3)
         strcpy(PhaseName, "MaxAmp");
   else if(code == 50)
         strcpy(PhaseName, "P-first");
   else if(code == 51)
         strcpy(PhaseName, "P");
   else if(code == 52)
         strcpy(PhaseName, "P*");
   else if(code == 53)
         strcpy(PhaseName, "PP");
   else if(code == 54)
         strcpy(PhaseName, "PPP");
   else if(code == 55)
         strcpy(PhaseName, "PPPP");
   else if(code == 56)
         strcpy(PhaseName, "PPS");
   else if(code == 57)
         strcpy(PhaseName, "Pg");
   else if(code == 58)
         strcpy(PhaseName, "Pn");
   else if(code == 59)
         strcpy(PhaseName, "Pdiff");
   else if(code == 60)
         strcpy(PhaseName, "PcP");
   else if(code == 61)
         strcpy(PhaseName, "PcPPKP");
   else if(code == 62)
         strcpy(PhaseName, "PcS");
   else if(code == 63)
         strcpy(PhaseName, "pP");
   else if(code == 64)
         strcpy(PhaseName, "pPP");
   else if(code == 65)
         strcpy(PhaseName, "PKP");
   else if(code == 66)
         strcpy(PhaseName, "PKPPKP");
   else if(code == 67)
         strcpy(PhaseName, "PKPPKS");
   else if(code == 68)
         strcpy(PhaseName, "PKPSKS");
   else if(code == 69)
         strcpy(PhaseName, "PKS");
   else if(code == 70)
         strcpy(PhaseName, "pPKS");
   else if(code == 71)
         strcpy(PhaseName, "PKKP");
   else if(code == 72)
         strcpy(PhaseName, "PKKS");
   else if(code == 73)
         strcpy(PhaseName, "PcPPKP");
   else if(code == 74)
         strcpy(PhaseName, "PcSPKP");


    else if(code == 100)
         strcpy(PhaseName, "S-first");
   else if(code == 101)
         strcpy(PhaseName, "S");
   else if(code == 102)
         strcpy(PhaseName, "S*");
   else if(code == 103)
         strcpy(PhaseName, "SS");
   else if(code == 104)
         strcpy(PhaseName, "SSS");
   else if(code == 105)
         strcpy(PhaseName, "SSSS");
   else if(code == 106)
         strcpy(PhaseName, "Sg");
   else if(code == 107)
         strcpy(PhaseName, "Sn");
   else if(code == 108)
         strcpy(PhaseName, "ScS");
   else if(code == 109)
         strcpy(PhaseName, "SPcS");
   else if(code == 110)
         strcpy(PhaseName, "sS");
   else if(code == 111)
         strcpy(PhaseName, "sSS");
   else if(code == 112)
         strcpy(PhaseName, "sSSS");
   else if(code == 113)
         strcpy(PhaseName, "SScS");
   else if(code == 114)
         strcpy(PhaseName, "ScSPKP");
   else if(code == 115)
         strcpy(PhaseName, "ScP");
   else if(code == 116)
         strcpy(PhaseName, "SKS");
   else if(code == 117)
         strcpy(PhaseName, "SKKS");
   else if(code == 118)
         strcpy(PhaseName, "SKKKS");
   else if(code == 119)
         strcpy(PhaseName, "SKSSKS");
   else if(code == 120)
         strcpy(PhaseName, "SKP");
   else if(code == 121)
         strcpy(PhaseName, "SKKP");
   else if(code == 122)
         strcpy(PhaseName, "SKKKP");

   else if(code == 201)
         strcpy(PhaseName, "Lg");
   else if(code == 202)
         strcpy(PhaseName, "Lr");
   else if(code == 203)
         strcpy(PhaseName, "Lr2");
   else if(code == 204)
         strcpy(PhaseName, "Lr3");
   else if(code == 205)
         strcpy(PhaseName, "Lr4");
   else if(code == 206)
         strcpy(PhaseName, "Lq");
   else if(code == 207)
         strcpy(PhaseName, "Lq2");
   else if(code == 208)
         strcpy(PhaseName, "Lq3");
   else if(code == 209)
         strcpy(PhaseName, "Lq4");
   else if(code == 301) 
         strcpy(PhaseName, "t");
   else
         strcpy(PhaseName,  "Unknown");

   return PhaseName;
}
/* ------------------------------------------------------- */





short Codephase(const char *phase)
{
   if(!strcmp(phase, "window") )
      return 1;
   else if(!strcmp(phase, "f finis") )
      return 2; 
   else if(!strcmp(phase, "MaxAmp") )
      return 3; 
   else if(!strcmp(phase, "P-first") )
      return 50; 
   else if(!strcmp(phase, "P") )
      return 51; 
   else if(!strcmp(phase, "P*") )
      return 52; 
   else if(!strcmp(phase, "PP") )
      return 53; 
   else if(!strcmp(phase, "PPP") )
      return 54; 
   else if(!strcmp(phase, "PPPP") )
      return 55; 
   else if(!strcmp(phase, "PPS") )
      return 56; 
   else if(!strcmp(phase, "Pg") )
      return 57 ;
   else if(!strcmp(phase, "Pn") )
      return 58; 
   else if(!strcmp(phase, "Pdiff") )
      return 59; 
   else if(!strcmp(phase, "PcP") )
      return 60; 
   else if(!strcmp(phase, "PcPPKP") )
      return 61; 
   else if(!strcmp(phase, "PcS") )
      return 62; 
   else if(!strcmp(phase, "pP") )
      return 63; 
   else if(!strcmp(phase, "pPP") )
      return 64; 
   else if(!strcmp(phase, "PKP") )
      return 65; 
   else if(!strcmp(phase, "PKPPKP") )
      return 66; 
   else if(!strcmp(phase, "PKPPKS") )
      return 67; 
   else if(!strcmp(phase, "PKPSKS") )
      return 68; 
   else if(!strcmp(phase, "PKS") )
      return 69; 
   else if(!strcmp(phase, "pPKS") )
      return 70; 
   else if(!strcmp(phase, "PKKP") )
      return 71; 
   else if(!strcmp(phase, "PKKS") )
      return 72; 
   else if(!strcmp(phase, "PcPPKP") )
      return 73; 
   else if(!strcmp(phase, "PcSPKP") )
      return 74; 
   else if(!strcmp(phase, "S-first") )
      return 100; 
   else if(!strcmp(phase, "S") )
      return 101; 
   else if(!strcmp(phase, "S*") )
      return 102; 
   else if(!strcmp(phase, "SS") )
      return 103; 
   else if(!strcmp(phase, "SSS") )
      return 104; 
   else if(!strcmp(phase, "SSSS") )
      return 105; 
   else if(!strcmp(phase, "Sg") )
      return 106; 
   else if(!strcmp(phase, "Sn") )
      return 107; 
   else if(!strcmp(phase, "ScS") )
      return 108; 
   else if(!strcmp(phase, "SPcS") )
      return 109; 
   else if(!strcmp(phase, "sS") )
      return 110; 
   else if(!strcmp(phase, "sSS") )
      return 111; 
   else if(!strcmp(phase, "sSSS") )
      return 112; 
   else if(!strcmp(phase, "SScS") )
      return 113; 
   else if(!strcmp(phase, "ScSPKP") )
      return 114; 
   else if(!strcmp(phase, "ScP") )
      return 115; 
   else if(!strcmp(phase, "SKS") )
      return 116; 
   else if(!strcmp(phase, "SKKS") )
      return 117; 
   else if(!strcmp(phase, "SKKKS") )
      return 118; 
   else if(!strcmp(phase, "SKSSKS") )
      return 119; 
   else if(!strcmp(phase, "SKP") )
      return 120; 
   else if(!strcmp(phase, "SKKP") )
      return 121; 
   else if(!strcmp(phase, "SKKKP") )
      return 122; 
   else if(!strcmp(phase, "Lg") )
      return 201; 
   else if(!strcmp(phase, "Lr") )
      return 202; 
   else if(!strcmp(phase, "Lr2") )
      return 203; 
   else if(!strcmp(phase, "Lr3") )
      return 204; 
   else if(!strcmp(phase, "Lr4") )
      return 205; 
   else if(!strcmp(phase, "Lq") )
      return 206; 
   else if(!strcmp(phase, "Lq2") )
      return 207; 
   else if(!strcmp(phase, "Lq3") )
      return 208; 
   else if(!strcmp(phase, "Lq4") )
      return 209; 
   else if(!strcmp(phase, "t") )
      return 301;
   else
      return 0;

}
/* ------------------------------------------------------- */




char *GetCharAuth(short authority)
{
   static char tmp[100];
   switch (authority) {
   case 1000:
      strcpy(tmp,"USGS, Menlo Park");
      break;
   case 1002:
      strcpy(tmp,"CALNET");
      break;
   case 1050:
      strcpy(tmp,"RTP, USGS, Menlo Park");
      break;
   case 2000:
      strcpy(tmp,"Geophysical Institute, U. of Alaska");
      break;
   case 3000:
      strcpy(tmp,"University of Washington");
      break;
   case 4000:
      strcpy(tmp,"Lamont Doherty Geological Observatory");
      break;
   case 5000:
      strcpy(tmp,"IRIS");
      break;
   case 5100:
      strcpy(tmp,"GSN");
      break;
   case 5200:
      strcpy(tmp,"ASRO");
      break;
   case 5300:
      strcpy(tmp,"PASSCAL");
      break;
   case 6000:
      strcpy(tmp,"LLNL");
      break;
   case 7000:
      strcpy(tmp,"LBL");
      break;
   case 8000:
      strcpy(tmp,"LANL");
      break;
      
   default:
      strcpy(tmp,"-");
   }
   return tmp;
}
/* -------------------------------------------------------------------- */





char *GetCharProg(char program)
{
   static char tmp[100];
   switch(program){
   case '7':
      strcpy(tmp,"Hypo-71");
      break;
   case 'h':
      strcpy(tmp,"HypoInverse");
      break;
   case 'l':
      strcpy(tmp,"HypoLayer");
      break;
   case 'c':
      strcpy(tmp,"Centroid");
      break;
   case 'v':
      strcpy(tmp,"Velest");
      break;
   default:
      strcpy(tmp,"-");
   }
   return tmp;
}
/* -------------------------------------------------------------------- */





char *GetCharMagType(short mag_type)
{
   static char tmp[100];
   switch (mag_type){
   case 'b':
      strcpy(tmp,"Mb");
      break;
   case 'c':
      strcpy(tmp,"Mc");
      break;
   case 'l':
      strcpy(tmp,"Ml");
      break;
   case 's':
      strcpy(tmp,"Ms");
      break;
   case 'w':
      strcpy(tmp,"Mw");
      break;
   default:
      strcpy(tmp,"Mb");
   }
   return tmp;
}
/* -------------------------------------------------------------------- */








