#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "suds/suds.h"
#include "suds/sudsListOps.h"
#include "suds/sudsDataConversions.h"
#include "suds/sudsReading.h"
#include "suds/sudsWriting.h"

#include "dff.h"

#include "proto.h"


int FillMuxDataStruct(SUDS *S, SUDS_MUXDATA *mu,
				SUDS_STRUCTTAG Tag, FILE *ptr) ;
int FillShotgatherStruct(SUDS_SHOTGATHER *sh, SUDS_STRUCTTAG Tag, FILE *ptr) ;
int Demux(SUDS *S);
int WriteDTlist(dtList *dtHead, SUDS_STRUCTTAG *Tag, FILE *ptr) ;
int WriteVMlist(vmList *vmHead, SUDS_STRUCTTAG *Tag, FILE *ptr) ;



void ApplyRateCorrection(SUDS *S, double RateCorrect, int Verbose)
{
   dtList *dt;
   
   if ( ( (int) RateCorrect ) != NODATA){
      if(Verbose)
         printf("Applying rate correction of %f to all trace structs.\n",
                RateCorrect);
      dt = S->dtHead;
   
      while(dt){
         dt->dt->rate        += RateCorrect;
         dt->dt->rate_correct = 0.0;
         dt = dt->next;
      }
   }
}/* ---------------------------------------------------------------------- */




void ApplyTimeCorrection(SUDS *S, double TimeCorrect, int Verbose)
{
   dtList *dt;


   if ( ( (int) TimeCorrect ) != NODATA){
      if(Verbose)
         printf("Applying time correction of %f to all trace structs.\n",
                TimeCorrect);
      dt = S->dtHead;
   
      while(dt){
         dt->dt->begintime   += TimeCorrect;
         dt->dt->time_correct = 0.0;
         dt = dt->next;
      }
   }
}
/* -------------------------------------------------------------- */






void ApplyCorrectionsFromIrigStruct(SUDS *S, int Verbose)
{
   dtList *dt;
   double RateCorrect = 0;
   double TimeCorrect = 0;
   int TimeCodeFound  = 0;
   dt = S->dtHead;
   
   while(dt){
      if( toupper( dt->dt->dt_name.component ) == 'T' ||
          !strncmp( dt->dt->dt_name.st_name, "IRIG",4) ){
          RateCorrect = dt->dt->rate_correct;
          TimeCorrect = dt->dt->time_correct;
          TimeCodeFound = 1;
          break;
      }
      dt = dt->next;
   }
   if(!TimeCodeFound){
      if(Verbose)
         printf("No time code found! Trace times are uncorrected.\n");
      return;
   }

   ApplyRateCorrection(S, RateCorrect, Verbose);
   ApplyTimeCorrection(S, TimeCorrect, Verbose);
}
/* -------------------------------------------------------------- */







void ApplyExistingTimeCorrections(SUDS *S, int Verbose)
{
   tcList *tc;
   double RateCorrect    = 0;
   double TimeCorrect    = 0;
   int SuccessfulDecode  = 0;
   int NumTCstructs      = 0;
   tc = S->tcHead;
   
   if(!tc){
      if(Verbose){
         printf("No time-correction struct found! Trace times and rates are uncorrected.\n");
         printf("Attempting to extract information from IRIG descriptrace struct.\n");
      }
      ApplyCorrectionsFromIrigStruct(S, Verbose);
      return;
   }


   while(tc){
      NumTCstructs++;
      if(tc->tc->sync_code == '4' || tc->tc->sync_code == '5'){
          RateCorrect = tc->tc->rate_correct;
          TimeCorrect = tc->tc->time_correct;
          SuccessfulDecode = 1;
          break;
      }
      tc = tc->next;
   }
   if(!SuccessfulDecode){
      if(Verbose){
         printf("%d Time-correct structs found, but none showed successful sync.\n", 
               NumTCstructs );
         printf("Attempting to extract information from IRIG descriptrace struct.\n");
      }
      ApplyCorrectionsFromIrigStruct(S, Verbose);
      return;
   }

   ApplyRateCorrection(S, RateCorrect, Verbose);
   ApplyTimeCorrection(S, TimeCorrect, Verbose);
}
/* -------------------------------------------------------------- */



static int AddToSuds(SUDS *S, SUDS_STRUCTTAG Tag, FILE *ptr, int Verbose)
{
   SUDS_DETECTOR *de;
   SUDS_ATODINFO *ad;
   SUDS_TRIGSETTING *ts;
   SUDS_EVENTSETTING *es;
   SUDS_STATIONCOMP *sc;
   SUDS_DESCRIPTRACE *dt;
   SUDS_FEATURE *fe;
   SUDS_CALIBRATION *ca;
   SUDS_COMMENT *co;
   SUDS_EVENT *ev;
   SUDS_TRIGGERS *tr;
   SUDS_EVDESCR *ed;
   SUDS_FOCALMECH *fo;
   SUDS_INSTRUMENT *in;
   SUDS_ORIGIN *orig;
   SUDS_TIMECORRECTION *tc;
   SUDS_MUXDATA *mu;
   SUDS_VELMODEL *vm;
   SUDS_TERMINATOR *te;
   SUDS_LAYERS *la;
   SUDS_LOCTRACE *lo;
   SUDS_MOMENT *mo;
   SUDS_PROFILE *pr;
   SUDS_RESIDUAL *re;
   SUDS_SHOTGATHER *sh;
   SUDS_ERROR *er;
   SUDS_EQUIPMENT *eq;
   SUDS_CHANSET *cs;

   if(Tag.id_struct == DETECTOR){
      de = (SUDS_DETECTOR*)AddSudsListElement(S, Tag.id_struct);
      return FillDetectorStruct(de, Tag, ptr);
   }

   else if(Tag.id_struct == ATODINFO){
      ad = (SUDS_ATODINFO*)AddSudsListElement(S, Tag.id_struct);
      return FillAtoDStruct(ad, Tag, ptr);
   }

   else if(Tag.id_struct == TRIGSETTING){
      ts = (SUDS_TRIGSETTING*)AddSudsListElement(S, Tag.id_struct);
      return FillTrigSettingStruct(ts, Tag, ptr);
   }


   else if(Tag.id_struct == EVENTSETTING){
      es = (SUDS_EVENTSETTING*)AddSudsListElement(S, Tag.id_struct);
      return FillEventSettingStruct(es, Tag, ptr);
   }


   else if(Tag.id_struct == STATIONCOMP){
      sc = (SUDS_STATIONCOMP*)AddSudsListElement(S, Tag.id_struct);
      return FillStationCompStruct(sc, Tag, ptr);
  }

   else if(Tag.id_struct == DESCRIPTRACE){
      dt = (SUDS_DESCRIPTRACE*)AddSudsListElement(S, Tag.id_struct);
      return FillDescripTraceStruct(S, dt, Tag, ptr);
  }

   else if(Tag.id_struct == MUXDATA){
      mu = (SUDS_MUXDATA*)AddSudsListElement(S, Tag.id_struct);
      return FillMuxDataStruct(S, mu, Tag, ptr);
  }

   else if(Tag.id_struct == FEATURE){
      fe = (SUDS_FEATURE*) AddSudsListElement(S, Tag.id_struct);
      return FillFeatureStruct(fe, Tag, ptr);
  }

   else if(Tag.id_struct == CALIBRATION){
      ca = (SUDS_CALIBRATION*) AddSudsListElement(S, Tag.id_struct);
      return FillCalibStruct(ca, Tag, ptr);
  }

   else if(Tag.id_struct == COMMENT){
      co = (SUDS_COMMENT*) AddSudsListElement(S, Tag.id_struct);
      return FillCommentStruct(S, co, Tag, ptr);
  }

   else if(Tag.id_struct == CHANSET){
      cs = (SUDS_CHANSET*) AddSudsListElement(S, Tag.id_struct);
      return FillChansetStruct(S, cs, Tag, ptr);
  }

   else if(Tag.id_struct == EQUIPMENT){
      eq = (SUDS_EQUIPMENT*) AddSudsListElement(S, Tag.id_struct);
      return FillEquipmentStruct(eq, Tag, ptr);
  }

   else if(Tag.id_struct == ERROR){
      er = (SUDS_ERROR*) AddSudsListElement(S, Tag.id_struct);
      return FillErrorStruct(er, Tag, ptr);
  }

   else if(Tag.id_struct == EVENT){
      ev = (SUDS_EVENT*) AddSudsListElement(S, Tag.id_struct);
      return FillEventStruct(ev, Tag, ptr);
  }

   else if(Tag.id_struct == TRIGGERS){
      tr = (SUDS_TRIGGERS*) AddSudsListElement(S, Tag.id_struct);
      return FillTriggerStruct(tr, Tag, ptr);
  }

   else if(Tag.id_struct == EV_DESCRIPT){
      ed = (SUDS_EVDESCR*) AddSudsListElement(S, Tag.id_struct);
      return FillEVDescriptStruct(ed, Tag, ptr);
  }

   else if(Tag.id_struct == FOCALMECH){
      fo = (SUDS_FOCALMECH*) AddSudsListElement(S, Tag.id_struct);
      return FillFocalMechStruct(fo, Tag, ptr);
  }

   else if(Tag.id_struct == INSTRUMENT){
      in = (SUDS_INSTRUMENT*) AddSudsListElement(S, Tag.id_struct);
      return FillInstrumentStruct(in, Tag, ptr);
  }

   else if(Tag.id_struct == LAYERS){
      la = (SUDS_LAYERS*) AddSudsListElement(S, Tag.id_struct);
      return FillLayersStruct(la, Tag, ptr);
  }

   else if(Tag.id_struct == LOCTRACE){
      lo = (SUDS_LOCTRACE*) AddSudsListElement(S, Tag.id_struct);
      return FillLoctraceStruct(lo, Tag, ptr);
  }

   else if(Tag.id_struct == MOMENT){
      mo = (SUDS_MOMENT*) AddSudsListElement(S, Tag.id_struct);
      return FillMomentStruct(mo, Tag, ptr);
  }

   else if(Tag.id_struct == ORIGIN){
      orig = (SUDS_ORIGIN*) AddSudsListElement(S, Tag.id_struct);
      return FillOriginStruct(orig, Tag, ptr);
  }

   else if(Tag.id_struct == PROFILE){
      pr = (SUDS_PROFILE*) AddSudsListElement(S, Tag.id_struct);
      return FillProfileStruct(pr, Tag, ptr);
  }

   else if(Tag.id_struct == RESIDUAL){
      re = (SUDS_RESIDUAL*) AddSudsListElement(S, Tag.id_struct);
      return FillResidualStruct(re, Tag, ptr);
  }

   else if(Tag.id_struct == SHOTGATHER){
      sh = (SUDS_SHOTGATHER*) AddSudsListElement(S, Tag.id_struct);
      return FillShotgatherStruct(sh, Tag, ptr);
  }

   else if(Tag.id_struct == TIMECORRECTION){
      tc = (SUDS_TIMECORRECTION*) AddSudsListElement(S, Tag.id_struct);
      return FillTimeCorrectionStruct(tc, Tag, ptr);
  }

   else if(Tag.id_struct == VELMODEL){
      vm = (SUDS_VELMODEL*) AddSudsListElement(S, Tag.id_struct);
      return FillVelocityModelStruct(vm, Tag, ptr);
  }

   else if(Tag.id_struct == TERMINATOR){
      te = (SUDS_TERMINATOR*) AddSudsListElement(S, Tag.id_struct);
      return FillTerminatorStruct(te, Tag, ptr);
  }

  else{
      if(Verbose)
         printf("Unknown structure with tag ID = %d\n",Tag.id_struct);
      fseek(ptr,Tag.len_struct,SEEK_CUR);
      fseek(ptr,Tag.len_data,SEEK_CUR);
      return 1;
  }
 
}
/* ------------------------------------------------------- */





int TagFound(FILE *ptr, SUDS_STRUCTTAG *Tag)
{
  /* Read in the tag */
   if( fread( Tag, sizeof(SUDS_STRUCTTAG), 1, ptr ) != 1){
      return 0;
   }
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(Tag->id_struct));
     Convert4(&(Tag->len_struct));
     Convert4(&(Tag->len_data));
   }
   return 1;
}
/* ------------------------------------------------------- */







SUDS *ReadSUDSHeaders(char *fname, int Verbose)
{
   SUDS *S=0;
   FILE *ptr;
   SUDS_STRUCTTAG Tag;
   int offset;
   /* Open the file */
   if(! (ptr = fopen( fname, "rb" ) ) ){
      printf("ERROR: Unable to open file (%s).\n",fname);
      return S;
   }

   S = NewSudsList();
   /*printf("Reading SUDS...\n");*/
   while( TagFound( ptr, &Tag)){
      if(Tag.sync != 'S'){
         printf("File (%s) is out of sync orig is not a PC-Suds file!\n",fname);
         FreeSuds(S);
         fclose(ptr);
         return S;
      }
      if(Tag.machine != '6'){
         printf("File (%s) is not a PC-Suds file!\n",fname);
         FreeSuds(S);
         fclose(ptr);
         return S;
      }
      if(Tag.id_struct == DESCRIPTRACE || Tag.id_struct == MUXDATA){
         offset = Tag.len_struct + Tag.len_data; 
         fseek(ptr, offset, SEEK_CUR);
      }
      else if(! AddToSuds(S, Tag, ptr, Verbose) ){
         printf("Error reading data from (%s).\n",fname);
         FreeSuds(S);
         fclose(ptr);
         return S;
      }

   }


   fclose(ptr);
   return S;
}
/* ------------------------------------------------------- */






SUDS *ReadSUDSfile(char *fname, int Verbose)
{
   SUDS *S=0;
   FILE *ptr;
   SUDS_STRUCTTAG Tag;
   /* Open the file */
   if(! (ptr = fopen( fname, "rb" ) ) ){
      printf("ERROR: Unable to open file (%s).\n",fname);
      return S;
   }

   S = NewSudsList();
   if(Verbose)
      printf("Reading SUDS...\n"); 
   
   while( TagFound( ptr, &Tag)){
      if(Tag.sync != 'S'){
         printf("File (%s) is out of sync orig is not a PC-Suds file!\n",fname);
         FreeSuds(S);
         fclose(ptr);
         return 0;
      }
      if(Tag.machine != '6'){
         printf("File (%s) is not a PC-Suds file!\n",fname);
         FreeSuds(S);
         fclose(ptr);
         return 0;
      }
      
      
      
      if(! AddToSuds(S, Tag, ptr, Verbose) ){
         printf("Error reading data from (%s).\n",fname);
         FreeSuds(S);
         fclose(ptr);
         return 0;
      }

   }


   fclose(ptr);
   if(S->muHead){
      if(Verbose)
         printf("Demultiplexing ...\n"); 
      if(!Demux(S) )
         FreeSuds(S);
   }
   ApplyExistingTimeCorrections(S, Verbose);
   return S;
}
/* ------------------------------------------------------- */






int WriteSudsfile(SUDS *S, const char *fname)
{
   FILE *ptr;
   SUDS_STRUCTTAG Tag;


   if(!S)return 0;

   if(! (ptr = fopen( fname, "wb" ) ) ){
      printf("ERROR: Unable to open output file (%s)\n",fname);
      return 0;
   }

   if(S->stHead)printf("FreeSTList(S->stHead)\n");
   if(S->adHead)if(!WriteADlist(S->adHead, &Tag, ptr) ){fclose(ptr); return 0;} 
   if(S->caHead)if(!WriteCAlist(S->caHead, &Tag, ptr) ){fclose(ptr); return 0;} 
   if(S->coHead)if(!WriteCOlist(S->coHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->csHead)if(!WriteCSlist(S->csHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->dtHead)if(!WriteDTlist(S->dtHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->deHead)if(!WriteDElist(S->deHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->eqHead)if(!WriteEQlist(S->eqHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->erHead)if(!WriteERlist(S->erHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->evHead)if(!WriteEVlist(S->evHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->esHead)if(!WriteESlist(S->esHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->edHead)if(!WriteEDlist(S->edHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->feHead)if(!WriteFElist(S->feHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->foHead)if(!WriteFOlist(S->foHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->inHead)if(!WriteINlist(S->inHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->laHead)if(!WriteLAlist(S->laHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->loHead)if(!WriteLOlist(S->loHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->moHead)if(!WriteMOlist(S->moHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->muHead)printf("Muxdata not supported on output.\n");
   if(S->orHead)if(!WriteORlist(S->orHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->prHead)if(!WritePRlist(S->prHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->reHead)if(!WriteRElist(S->reHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->shHead)if(!WriteSHlist(S->shHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->scHead)if(!WriteSClist(S->scHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->teHead)if(!WriteTElist(S->teHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->tcHead)if(!WriteTClist(S->tcHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->trHead)if(!WriteTRlist(S->trHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->tsHead)if(!WriteTSlist(S->tsHead, &Tag, ptr) ){fclose(ptr); return 0;}
   if(S->vmHead)if(!WriteVMlist(S->vmHead, &Tag, ptr) ){fclose(ptr); return 0;}



   fclose(ptr);
   return 1;
}
/* ------------------------------------------------------- */

