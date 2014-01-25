#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "suds/suds.h"
#include "suds/sudsListOps.h"
#include "suds/sudsDataConversions.h"
#include "suds/sudsReading.h"

#include "dff.h"

#include "proto.h"

#define TagLen 12


static int ReadStatIdent(SUDS_STATIDENT *si, FILE *ptr)
{
   int bytesRead;

   bytesRead = fread(si->network,1, 4, ptr);
   si->network[4] = '\0';
   bytesRead += fread(si->st_name,1, 5, ptr);
   si->st_name[4] = '\0';
   if(bytesRead < 9)bytesRead += fread(&(si->component),1, 1, ptr);
   bytesRead += fread(&(si->component),1, 1, ptr);
   bytesRead += fread(&(si->inst_type),1, 2, ptr);
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(si->inst_type));
   }
   return bytesRead;
}
/* ----------------------------------------------------------------------- */







int FillDetectorStruct(SUDS_DETECTOR *de, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!de) return 0;
   bytesRead = fread( de, 1, 12, ptr );
   de->net_node_id[10] = '\0';
   bytesRead += fread( &(de->versionnum), 1, 4, ptr );
   bytesRead += fread( &(de->event_number), 1, 4, ptr );
   bytesRead += fread( &(de->spareL), 1, 4, ptr );
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(de->versionnum));
     Convert4(&(de->event_number));
     Convert4(&(de->spareL));
   }
   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */





int FillAtoDStruct(SUDS_ATODINFO *ad, SUDS_STRUCTTAG Tag, FILE *ptr)
{

   if(!ad) return 0;
      
   if( fread( ad, 1, Tag.len_struct, ptr ) != (size_t)Tag.len_struct)return 0;
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(ad->base_address));
     Convert2(&(ad->device_id));
     Convert2(&(ad->device_flags));
     Convert2(&(ad->extended_bufs));
     Convert2(&(ad->external_mux));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */





int FillTrigSettingStruct(SUDS_TRIGSETTING *ts, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!ts) return 0;
   bytesRead = fread(ts->netwname,1, 4,ptr);
   ts->netwname[4] = '\0';
   bytesRead += fread(&(ts->beginttime),1, 8,ptr);
   bytesRead += fread(&(ts->const1),1, 2,ptr);
   bytesRead += fread(&(ts->const2),1, 2,ptr);
   bytesRead += fread(&(ts->threshold),1, 2,ptr);
   bytesRead += fread(&(ts-> const3),1, 2,ptr);
   bytesRead += fread(&(ts->const4),1, 2,ptr);
   bytesRead += fread(&(ts->wav_inc),1, 2,ptr);
   bytesRead += fread(&(ts->sweep),1, 4,ptr);
   bytesRead += fread(&(ts->aperture),1, 4,ptr);
   bytesRead += fread(&(ts->algorithm),1,1,ptr);
   bytesRead += fread(&(ts->spareJ),1,1,ptr);
   bytesRead += fread(&(ts->spareI),1, 2,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert8(&(ts->beginttime));
     Convert2(&(ts->const1));
     Convert2(&(ts->const2));
     Convert2(&(ts->threshold));
     Convert2(&(ts->const3));
     Convert2(&(ts->const4));
     Convert2(&(ts->wav_inc));
     Convert4(&(ts->sweep));
     Convert4(&(ts->aperture));
     Convert2(&(ts->spareI));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */





int FillEventSettingStruct(SUDS_EVENTSETTING *es, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!es) return 0;
   bytesRead = fread(es->netwname,1, 4,ptr);
   es->netwname[4] = '\0';
   bytesRead += fread(&(es->beginttime),1, 8,ptr);
   bytesRead += fread(&(es->const1),1, 2,ptr);
   bytesRead += fread(&(es->const2),1, 2,ptr);
   bytesRead += fread(&(es->threshold),1, 2,ptr);
   bytesRead += fread(&(es-> const3),1, 2,ptr);
   bytesRead += fread(&(es->minduration),1, 4,ptr);
   bytesRead += fread(&(es->maxduration),1, 4,ptr);
   bytesRead += fread(&(es->algorithm),1, 1,ptr);
   bytesRead += fread(&(es->spareK),1, 1,ptr);
   bytesRead += fread(&(es->spareI),1, 2,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert8(&(es->beginttime));
     Convert2(&(es->const1));
     Convert2(&(es->const2));
     Convert2(&(es->threshold));
     Convert2(&(es->const3));
     Convert4(&(es->minduration));
     Convert4(&(es->maxduration));
     Convert2(&(es->spareI));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */






int FillStationCompStruct(SUDS_STATIONCOMP *sc, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!sc) return 0;
   bytesRead = ReadStatIdent(&(sc->sc_name), ptr);


   bytesRead += fread(&(sc->azim),1, 2,ptr);
   bytesRead += fread(&(sc->incid),1, 2,ptr);
   bytesRead += fread(&(sc->st_lat),1, 8,ptr);
   bytesRead += fread(&(sc->st_long),1, 8,ptr);
   bytesRead += fread(&(sc->elev),1, 4,ptr);
   bytesRead += fread(&(sc->enclosure),1, 1,ptr);
   bytesRead += fread(&(sc->annotation),1, 1,ptr);
   bytesRead += fread(&(sc->recorder),1, 1,ptr);
   bytesRead += fread(&(sc->rockclass),1, 1,ptr);
   bytesRead += fread(&(sc->rocktype),1, 2,ptr);
   bytesRead += fread(&(sc->sitecondition),1, 1,ptr);
   bytesRead += fread(&(sc->sensor_type),1, 1,ptr);
   bytesRead += fread(&(sc->data_type),1, 1,ptr);
   bytesRead += fread(&(sc->data_units),1, 1,ptr);
   bytesRead += fread(&(sc->polarity),1, 1,ptr);
   bytesRead += fread(&(sc->st_status),1, 1,ptr);
   bytesRead += fread(&(sc->max_gain),1, 4,ptr);
   bytesRead += fread(&(sc->clip_value),1, 4,ptr);
   bytesRead += fread(&(sc->con_mvolts),1, 4,ptr);
   bytesRead += fread(&(sc->channel),1, 2,ptr);
   bytesRead += fread(&(sc->atod_gain),1, 2,ptr);
   bytesRead += fread(&(sc->effective),1, 4,ptr);
   bytesRead += fread(&(sc->clock_correct),1, 4,ptr);
   bytesRead += fread(&(sc->station_delay),1, 4,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(sc->azim));
     Convert2(&(sc->incid));
     Convert8(&(sc->st_lat));
     Convert8(&(sc->st_long));
     Convert4(&(sc->elev));
     Convert2(&(sc->rocktype));
     Convert4(&(sc->max_gain));
     Convert4(&(sc->clip_value));
     Convert4(&(sc->con_mvolts));
     Convert2(&(sc->channel));
     Convert2(&(sc->atod_gain));
     Convert4(&(sc->effective));
     Convert4(&(sc->clock_correct));
     Convert4(&(sc->station_delay));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */





int FillMuxDataStruct(SUDS *S, SUDS_MUXDATA *mu, 
                                  SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;
   int j;

   if(!mu) return 0;

   bytesRead  = fread(mu->netname,1, 4,ptr);
   mu->netname[3] = '\0';
   bytesRead += fread(&(mu->begintime),1, 8,ptr);
   bytesRead += fread(&(mu->loctime),1, 2,ptr);
   bytesRead += fread(&(mu->numchans),1, 2,ptr);
   bytesRead += fread(&(mu->dig_rate),1, 4,ptr);
   bytesRead += fread(&(mu->typedata),1, 1,ptr);
   bytesRead += fread(&(mu->descript),1, 1,ptr);
   bytesRead += fread(&(mu->spareG),1, 2,ptr);
   bytesRead += fread(&(mu->numsamps),1, 4,ptr);
   bytesRead += fread(&(mu->blocksize),1, 4,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert8(&(mu->begintime));
     Convert2(&(mu->loctime));
     Convert2(&(mu->numchans));
     Convert4(&(mu->dig_rate));
     Convert2(&(mu->spareG));
     Convert4(&(mu->numsamps));
     Convert4(&(mu->blocksize));
   }

   mu->numsamps = mu->numchans * mu->blocksize;

   if(Tag.len_data){
      switch (mu->typedata){

      case 's': case 'q': case 'u': case 'i':
         S->muTail->i2data = (short *) malloc( Tag.len_data );
         if(!S->muTail->i2data){
            printf("ERROR: Could not allocate data array in FillMuxDataStruct.\n");
            return 0;
	 }
         bytesRead = fread(S->muTail->i2data, 1, Tag.len_data, ptr);
         if(bytesRead != Tag.len_data){
            printf("ERROR: Expected to read %d bytes but got %d instead.\n",
                    Tag.len_data, bytesRead);
            return 0;
	 }
   if(CheckByteOrder() == ENDIAN_BIG) {
     for(j=0;j<mu->numsamps;j++)Convert2(S->muTail->i2data + j);
   }
         return 1;

      case '2': case 'l':
         S->muTail->i4data = (int *) malloc( Tag.len_data );
         if(!S->muTail->i4data){
            printf("ERROR: Could not allocate data array in FillMuxDataStruct.\n");
            return 0;
	 }
         bytesRead = fread(S->muTail->i4data, 1, Tag.len_data, ptr);
         if(bytesRead != Tag.len_data){
            printf("ERROR: Expected to read %d bytes but got %d instead.\n",
                    Tag.len_data, bytesRead);
            return 0;
	 }
   if(CheckByteOrder() == ENDIAN_BIG) {
     for(j=0;j<mu->numsamps;j++)Convert4(S->muTail->i4data + j);
   }
         return 1;

      case 'f':
         S->muTail->r4data = (float *) malloc( Tag.len_data );
         if(!S->muTail->r4data){
            printf("ERROR: Could not allocate data array in FillMuxDataStruct.\n");
            return 0;
	 }
         bytesRead = fread(S->muTail->r4data, 1, Tag.len_data, ptr);
         if(bytesRead != Tag.len_data){
            printf("ERROR: Expected to read %d bytes but got %d instead.\n",
                    Tag.len_data, bytesRead);
            return 0;
	 }
   if(CheckByteOrder() == ENDIAN_BIG) {
     for(j=0;j<mu->numsamps;j++)Convert4(S->muTail->r4data + j);
   }
         return 1;

      case 'd':
         S->muTail->r8data = (double *) malloc( Tag.len_data );
         if(!S->muTail->r8data){
            printf("ERROR: Could not allocate data array in FillMuxDataStruct.\n");
            return 0;
	 }
         bytesRead = fread(S->muTail->r8data, 1,  Tag.len_data, ptr);
         if(bytesRead != Tag.len_data){
            printf("ERROR: Expected to read %d bytes but got %d instead.\n",
                    Tag.len_data, bytesRead);
            return 0;
	 }
   if(CheckByteOrder() == ENDIAN_BIG) {
     /* for(j=0;j<mu->numsamps;j++)Convert8(S->muTail->r8data + j); */
   }
         return 1;
      }
   }


   /* default is to skip data if not a recognized type */
   fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */

















int FillDescripTraceStruct(SUDS *S, SUDS_DESCRIPTRACE *dt, 
                                  SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;
   int j;

   if(!dt) return 0;
   bytesRead = ReadStatIdent(&(dt->dt_name), ptr);
   bytesRead += fread(&(dt->begintime),1, 8,ptr);
   bytesRead += fread(&(dt->localtime),1, 2,ptr);
   bytesRead += fread(&(dt->datatype),1, 1,ptr);
   bytesRead += fread(&(dt->descriptor),1, 1,ptr);
   bytesRead += fread(&(dt->digi_by),1, 2,ptr);
   bytesRead += fread(&(dt->processed),1, 2,ptr);
   bytesRead += fread(&(dt->length),1, 4,ptr);
   bytesRead += fread(&(dt->rate),1, 4,ptr);
   bytesRead += fread(&(dt->mindata),1, 4,ptr);
   bytesRead += fread(&(dt->maxdata),1, 4,ptr);
   bytesRead += fread(&(dt->avenoise),1, 4,ptr);
   bytesRead += fread(&(dt->numclip),1, 4,ptr);
   bytesRead += fread(&(dt->time_correct),1, 8,ptr);
   bytesRead += fread(&(dt->rate_correct),1, 4,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert8(&(dt->begintime));
     Convert2(&(dt->localtime));
     Convert2(&(dt->digi_by));
     Convert2(&(dt->processed));
     Convert4(&(dt->length));
     Convert4(&(dt->rate));
     Convert4(&(dt->mindata));
     Convert4(&(dt->maxdata));
     Convert4(&(dt->avenoise));
     Convert4(&(dt->numclip));
     Convert8(&(dt->time_correct));
     Convert4(&(dt->rate_correct));
   }

   if(Tag.len_data){
      switch (dt->datatype){

      case 's': case 'q': case 'u': case 'i':
         S->dtTail->i2data = (short *) malloc( dt->length * sizeof(short) );
         if(!S->dtTail->i2data){
            printf("ERROR: Could not allocate data array in FillDescripTraceStruct.\n");
            return 0;
	 }
         bytesRead = fread(S->dtTail->i2data, 1, sizeof(short) * dt->length, ptr);
         if(bytesRead != Tag.len_data){
            printf("ERROR: Expected to read %d bytes but got %d instead.\n",
                    Tag.len_data, bytesRead);
            return 0;
	 }
   if(CheckByteOrder() == ENDIAN_BIG) {
     for(j=0;j<dt->length;j++)Convert2(S->dtTail->i2data + j);
   }
         return 1;

      case '2': case 'l':
         S->dtTail->i4data = (int *) malloc( dt->length * sizeof(int) );
         if(!S->dtTail->i4data){
            printf("ERROR: Could not allocate data array in FillDescripTraceStruct.\n");
            return 0;
	 }
         bytesRead = fread(S->dtTail->i4data, 1, sizeof(int) * dt->length, ptr);
         if(bytesRead != Tag.len_data){
            printf("ERROR: Expected to read %d bytes but got %d instead.\n",
                    Tag.len_data, bytesRead);
            return 0;
	 }
   if(CheckByteOrder() == ENDIAN_BIG) {
     for(j=0;j<dt->length;j++)Convert4(S->dtTail->i4data + j);
   }
         return 1;

      case 'f':
         S->dtTail->r4data = (float *) malloc( dt->length * sizeof(float) );
         if(!S->dtTail->r4data){
            printf("ERROR: Could not allocate data array in FillDescripTraceStruct.\n");
            return 0;
	 }
         bytesRead = fread(S->dtTail->r4data, 1, sizeof(float) * dt->length, ptr);
         if(bytesRead != Tag.len_data){
            printf("ERROR: Expected to read %d bytes but got %d instead.\n",
                    Tag.len_data, bytesRead);
            return 0;
	 }
   if(CheckByteOrder() == ENDIAN_BIG) {
     for(j=0;j<dt->length;j++)Convert4(S->dtTail->r4data + j);
   }
         return 1;

      case 'd':
         S->dtTail->r8data = (double *) malloc( dt->length * sizeof(double) );
         if(!S->dtTail->r8data){
            printf("ERROR: Could not allocate data array in FillDescripTraceStruct.\n");
            return 0;
	 }
         bytesRead = fread(S->dtTail->r8data, 1, sizeof(double) * dt->length, ptr);
         if(bytesRead != Tag.len_data){
            printf("ERROR: Expected to read %d bytes but got %d instead.\n",
                    Tag.len_data, bytesRead);
            return 0;
	 }
   if(CheckByteOrder() == ENDIAN_BIG) {
     for(j=0;j<dt->length;j++)Convert8(S->dtTail->r8data + j);
   }
         return 1;
      }
   }


   /* default is to skip data if not a recognized type */
   fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */





int FillFeatureStruct(SUDS_FEATURE *fe, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!fe) return 0;
   bytesRead = ReadStatIdent(&(fe->fe_name), ptr);
   bytesRead += fread(&(fe->obs_phase),1, 2,ptr);
   bytesRead += fread(&(fe->onset),1, 1,ptr);
   bytesRead += fread(&(fe->direction),1, 1,ptr);
   bytesRead += fread(&(fe->sig_noise),1, 2,ptr);
   bytesRead += fread(&(fe->data_source),1, 1,ptr);
   bytesRead += fread(&(fe->tim_qual),1, 1,ptr);
   bytesRead += fread(&(fe->amp_qual),1, 1,ptr);
   bytesRead += fread(&(fe->ampunits),1, 1,ptr);
   bytesRead += fread(&(fe->gain_range),1, 2,ptr);
   bytesRead += fread(&(fe->time),1, 8,ptr);
   bytesRead += fread(&(fe->amplitude),1, 4,ptr);
   bytesRead += fread(&(fe->period),1, 4,ptr);
   bytesRead += fread(&(fe->time_of_pick),1, 4,ptr);
   bytesRead += fread(&(fe->pick_authority),1, 2,ptr);
   bytesRead += fread(&(fe->pick_reader),1, 2,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(fe->obs_phase));
     Convert2(&(fe->sig_noise));
     Convert2(&(fe->gain_range));
     Convert8(&(fe->time));
     Convert4(&(fe->amplitude));
     Convert4(&(fe->period));
     Convert4(&(fe->time_of_pick));
     Convert2(&(fe->pick_authority));
     Convert2(&(fe->pick_reader));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */






int FillCalibStruct(SUDS_CALIBRATION *ca, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!ca) return 0;
   bytesRead = ReadStatIdent(&(ca->ca_name), ptr);
   bytesRead += fread(&(ca->maxgain),1, 4,ptr);
   bytesRead += fread(&(ca->normaliz),1, 4,ptr);
   bytesRead += fread(ca->cal,1, NOCALPTS * sizeof(SUDS_CALIBR),ptr);
   bytesRead += fread(&(ca->begint),1, 4,ptr);
   bytesRead += fread(&(ca->endt),1, 4,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(ca->maxgain));
     Convert4(&(ca->normaliz));
     ConvertPZArray(ca->cal, NOCALPTS);
     Convert4(&(ca->begint));
     Convert4(&(ca->endt));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */







int FillCommentStruct(SUDS *S, SUDS_COMMENT *co, 
                                  SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!co) return 0;
   bytesRead  = fread(&(co->refer),1, 2,ptr);
   bytesRead += fread(&(co->item),1, 2,ptr);
   bytesRead += fread(&(co->length),1, 2,ptr);
   bytesRead += fread(&(co->unused),1, 2,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(co->refer));
     Convert2(&(co->item));
     Convert2(&(co->length));
     Convert2(&(co->unused));
   }

   if(Tag.len_data){
      S->coTail->text = (char *) calloc(1, co->length * sizeof(char) );
      bytesRead = fread(S->coTail->text, 1, sizeof(char) * co->length, ptr);
      if(bytesRead != Tag.len_data){
         printf("ERROR: Expected to read %d bytes but got %d instead.\n",
                 Tag.len_data, bytesRead);
         return 0;
      }
      S->coTail->text[co->length - 1] = '\0';
      return 1;
   }
   return 1;
}
/* ----------------------------------------------------------------------- */







int FillChansetStruct(SUDS *S, SUDS_CHANSET *cs, 
                                  SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;
   int j;

   if(!cs) return 0;

   bytesRead  = fread(&(cs->type),1, 2,ptr);
   bytesRead += fread(&(cs->entries),1, 2,ptr);
   bytesRead += fread(cs->network,1, 4,ptr);
   bytesRead += fread(cs->name,1, 5,ptr);
   bytesRead += fread(&(cs->active),1, 4,ptr);
   bytesRead += fread(&(cs->inactive),1, 4,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else 
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(cs->type));
     Convert2(&(cs->entries));
     Convert4(&(cs->active));
     Convert4(&(cs->inactive));
   }



   if(Tag.len_data){
      S->csTail->entry = (CHANSETENTRY *) malloc( cs->entries * sizeof(CHANSETENTRY) );
      bytesRead = 0;
      for(j=0;j<cs->entries;j++){
         bytesRead += fread(&(S->csTail->entry[j].inst_num), 1, 4, ptr);
	 if(CheckByteOrder() == ENDIAN_BIG) {
	   Convert4(&(S->csTail->entry[j].inst_num));
	 }
	 bytesRead += fread(&(S->csTail->entry[j].stream_num), 1, 2, ptr);
	 if(CheckByteOrder() == ENDIAN_BIG) {
	   Convert2(&(S->csTail->entry[j].stream_num));
	 }
	 bytesRead += fread(&(S->csTail->entry[j].chan_num), 1, 2, ptr);
	 if(CheckByteOrder() == ENDIAN_BIG) {
	   Convert2(&(S->csTail->entry[j].chan_num));
	 }
	 bytesRead += ReadStatIdent(&(S->csTail->entry[j].st), ptr);
      }
      if(bytesRead != Tag.len_data){
	printf("ERROR: Expected to read %d bytes but got %d instead.\n",
	       Tag.len_data, bytesRead);
         return 0;
      }
      return 1;
   }
   return 1;
}
/* ----------------------------------------------------------------------- */






int FillEquipmentStruct(SUDS_EQUIPMENT *eq, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!eq) return 0;

   bytesRead  = ReadStatIdent(&(eq->thisSID), ptr);
   bytesRead += ReadStatIdent(&(eq->previous), ptr);
   bytesRead += ReadStatIdent(&(eq->next), ptr);
   bytesRead += fread(eq->serial,1, 8,ptr);
   bytesRead += fread(&(eq->model),1, 2,ptr);
   bytesRead += fread(&(eq->knob1),1, 2,ptr);
   bytesRead += fread(&(eq->knob2),1, 2,ptr);
   bytesRead += fread(&(eq->reason),1, 2,ptr);
   bytesRead += fread(&(eq->frequency),1, 4,ptr);
   bytesRead += fread(&(eq->effective),1, 4,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;


   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(eq->model));
     Convert2(&(eq->knob1));
     Convert2(&(eq->knob2));
     Convert2(&(eq->reason));
     Convert4(&(eq->frequency));
     Convert4(&(eq->effective));
    }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */






int FillErrorStruct(SUDS_ERROR *er, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;
   int j;

   if(!er) return 0;

   bytesRead  = fread(er->covarr,1, Tag.len_struct, ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;


   if(CheckByteOrder() == ENDIAN_BIG) {
     for(j=0;j<10;j++)
       Convert4(&(er->covarr[j]));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */






int FillEventStruct(SUDS_EVENT *ev, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!ev) return 0;
   bytesRead  = fread(&(ev->authority),1, 2,ptr);
   bytesRead += fread(&(ev->number),1, 4,ptr);
   bytesRead += fread(&(ev->felt),1, 2,ptr);
   bytesRead += fread(&(ev->mintensity),1, 1,ptr);
   bytesRead += fread(&(ev->ev_type),1, 1,ptr);
   bytesRead += fread(&(ev->tectonism),1, 1,ptr);
   bytesRead += fread(&(ev->waterwave),1, 1,ptr);
   bytesRead += fread(&(ev->mechanism),1, 1,ptr);
   bytesRead += fread(&(ev->medium),1, 1,ptr);
   bytesRead += fread(&(ev->size),1, 4,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;


   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(ev->authority));
     Convert4(&(ev->number));
     Convert2(&(ev->felt));
     Convert4(&(ev->size));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */







int FillTriggerStruct(SUDS_TRIGGERS *tr, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!tr) return 0;
   bytesRead = ReadStatIdent(&(tr->tr_name), ptr);
   bytesRead += fread(&(tr->sta),1, 2,ptr);
   bytesRead += fread(&(tr->lta),1, 2,ptr);
   bytesRead += fread(&(tr->abs_sta),1, 2,ptr);
   bytesRead += fread(&(tr->abs_lta),1, 2,ptr);
   bytesRead += fread(&(tr->trig_value),1, 2,ptr);
   bytesRead += fread(&(tr->num_triggers),1, 2,ptr);
   bytesRead += fread(&(tr->trig_time),1, 8,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;


   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(tr->sta));
     Convert2(&(tr->lta));
     Convert2(&(tr->abs_sta));
     Convert2(&(tr->abs_lta));
     Convert2(&(tr->trig_value));
     Convert2(&(tr->num_triggers));
     Convert8(&(tr->trig_time));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */






int FillEVDescriptStruct(SUDS_EVDESCR *ed, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!ed) return 0;
   bytesRead  = fread(&(ed->eqname),1, 20,ptr);
   bytesRead += fread(&(ed->country),1, 16,ptr);
   bytesRead += fread(&(ed->state),1, 16,ptr);
   bytesRead += fread(&(ed->localtime),1, 2,ptr);
   bytesRead += fread(&(ed->spareB),1, 2,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;


   ed->eqname[19] = '\0';
   ed->country[15] = '\0';
   ed->state[15] = '\0';
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(ed->localtime));
     Convert2(&(ed->spareB));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */






int FillFocalMechStruct(SUDS_FOCALMECH *fo, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!fo) return 0;
   bytesRead  = fread(&(fo->astrike),1, 4,ptr);
   bytesRead += fread(&(fo->adip),1, 4,ptr);
   bytesRead += fread(&(fo->arake),1, 4,ptr);
   bytesRead += fread(&(fo->bstrike),1, 4,ptr);
   bytesRead += fread(&(fo->bdip),1, 4,ptr);
   bytesRead += fread(&(fo->brake),1, 4,ptr);
   bytesRead += fread(&(fo->prefplane),1, 1,ptr);
   bytesRead += fread(&(fo->spareC),1, 3,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;


   fo->spareC[2] = '\0';
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(fo->astrike));
     Convert4(&(fo->adip));
     Convert4(&(fo->arake));
     Convert4(&(fo->bstrike));
     Convert4(&(fo->bdip));
     Convert4(&(fo->brake));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */







int FillInstrumentStruct(SUDS_INSTRUMENT *in, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!in) return 0;
   bytesRead  = ReadStatIdent(&(in->in_name), ptr);
   bytesRead += fread(&(in->in_serial),1, 2,ptr);
   bytesRead += fread(&(in->comps),1, 2,ptr);
   bytesRead += fread(&(in->channel),1, 2,ptr);
   bytesRead += fread(&(in->sens_type),1, 1,ptr);
   bytesRead += fread(&(in->datatype),1, 1,ptr);
   bytesRead += fread(&(in->void_samp),1, 4,ptr);
   bytesRead += fread(&(in->dig_con),1, 4,ptr);
   bytesRead += fread(&(in->aa_corner),1, 4,ptr);
   bytesRead += fread(&(in->aa_poles),1, 4,ptr);
   bytesRead += fread(&(in->nat_freq),1, 4,ptr);
   bytesRead += fread(&(in->damping),1, 4,ptr);
   bytesRead += fread(&(in-> mot_con),1, 4,ptr);
   bytesRead += fread(&(in->gain),1, 4,ptr);
   bytesRead += fread(&(in->local_x),1, 4,ptr);
   bytesRead += fread(&(in->local_y),1, 4,ptr);
   bytesRead += fread(&(in->local_z),1, 4,ptr);
   bytesRead += fread(&(in->effective),1, 4,ptr);
   bytesRead += fread(&(in->pre_event),1, 4,ptr);
   bytesRead += fread(&(in->trig_num),1, 2,ptr);
   bytesRead += fread(in->study,1, 6,ptr);
   in->study[5] = '\0';
   bytesRead += fread(&(in->sn_serial),1, 2,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;




   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(in->in_serial));
     Convert2(&(in->comps));
     Convert2(&(in->channel));
     Convert4(&(in->void_samp));
     Convert4(&(in->dig_con));
     Convert4(&(in->aa_corner));
     Convert4(&(in->aa_poles));
     Convert4(&(in->nat_freq));
     Convert4(&(in->damping));
     Convert4(&(in-> mot_con));
     Convert4(&(in->gain));
     Convert4(&(in->local_x));
     Convert4(&(in->local_y));
     Convert4(&(in->local_z));
     Convert4(&(in->effective));
     Convert4(&(in->pre_event));
     Convert2(&(in->trig_num));
     Convert2(&(in->sn_serial));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */






int FillLayersStruct(SUDS_LAYERS *la, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!la) return 0;

   bytesRead  = fread(&(la->thickness),1, 4,ptr);
   bytesRead += fread(&(la->pveltop),1, 4,ptr);
   bytesRead += fread(&(la->pvelbase),1, 4,ptr);
   bytesRead += fread(&(la->sveltop),1, 4,ptr);
   bytesRead += fread(&(la->svelbase),1, 4,ptr);
   bytesRead += fread(&(la->function),1, 2,ptr);
   bytesRead += fread(&(la->spareF),1, 2,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;



   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(la->thickness));
     Convert4(&(la->pveltop));
     Convert4(&(la->pvelbase));
     Convert4(&(la->sveltop));
     Convert4(&(la->svelbase));
     Convert2(&(la->function));
     Convert2(&(la->spareF));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */







int FillLoctraceStruct(SUDS_LOCTRACE *lo, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!lo) return 0;

   bytesRead  = ReadStatIdent(&(lo->lt_name), ptr);
   bytesRead += fread(&(lo->fileloc),1, 4,ptr);
   bytesRead += fread(&(lo->tapeloc),1, 4,ptr);
   bytesRead += fread(&(lo->beginloc),1, 4,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;


   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(lo->beginloc));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */







int FillMomentStruct(SUDS_MOMENT *mo, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;
   int j;

   if(!mo) return 0;
   bytesRead  = fread(&(mo->datatypes),1, 1,ptr);
   bytesRead += fread(&(mo->constraints),1, 1,ptr);
   bytesRead += fread(mo->spareD,1, 2,ptr);
   mo->spareD[1] = '\0';
   bytesRead += fread(&(mo->sc_moment),1, 4,ptr);
   bytesRead += fread(mo->norm_ten,1, 24,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;


   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(mo->sc_moment));
     for(j=0;j<6;j++)
       Convert4(&(mo->norm_ten[j]));
   }


   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */







int FillOriginStruct(SUDS_ORIGIN *orig, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!orig) return 0;
   bytesRead  = fread(&(orig->number),1, 4,ptr);
   bytesRead += fread(&(orig->authority),1, 2,ptr);
   bytesRead += fread(&(orig->version),1, 1,ptr);
   bytesRead += fread(&(orig->or_status),1, 1,ptr);
   bytesRead += fread(&(orig->preferred),1, 1,ptr);
   bytesRead += fread(&(orig->program),1, 1,ptr);
   bytesRead += fread(&(orig->depcontrl),1, 1,ptr);
   bytesRead += fread(&(orig->convergence),1, 1,ptr);
   bytesRead += fread(&(orig->region),1, 4,ptr);
   bytesRead += fread(&(orig->orgtime),1, 8,ptr);
   bytesRead += fread(&(orig->or_lat),1, 8,ptr);
   bytesRead += fread(&(orig->or_long),1, 8,ptr);
   bytesRead += fread(&(orig->depth),1, 4,ptr);
   bytesRead += fread(&(orig->err_horiz),1, 4,ptr);
   bytesRead += fread(&(orig->err_depth),1, 4,ptr);
   bytesRead += fread(&(orig->res_rms),1, 4,ptr);
   bytesRead += fread(orig->crustmodel,1, 6,ptr);
   orig->crustmodel[5] = '\0';
   bytesRead += fread(&(orig->gap),1, 2,ptr);
   bytesRead += fread(&(orig->nearstat),1, 4,ptr);
   bytesRead += fread(&(orig->num_stats),1, 2,ptr);
   bytesRead += fread(&(orig->rep_p),1, 2,ptr);
   bytesRead += fread(&(orig->used_p),1, 2,ptr);
   bytesRead += fread(&(orig->rep_s),1, 2,ptr);
   bytesRead += fread(&(orig-> used_s),1, 2,ptr);
   bytesRead += fread(&(orig->mag_type),1, 2,ptr);
   bytesRead += fread(&(orig->rep_m),1, 2,ptr);
   bytesRead += fread(&(orig->used_m),1, 2,ptr);
   bytesRead += fread(&(orig->magnitude),1, 4,ptr);
   bytesRead += fread(&(orig->weight),1, 4,ptr);
   bytesRead += fread(&(orig->mag_rms),1, 4,ptr);
   bytesRead += fread(&(orig->effective),1, 4,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;



   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(orig->number));
     Convert2(&(orig->authority));
     Convert4(&(orig->region));
     Convert8(&(orig->orgtime));
     Convert8(&(orig->or_lat));
     Convert8(&(orig->or_long));
     Convert4(&(orig->depth));
     Convert4(&(orig->err_horiz));
     Convert4(&(orig->err_depth));
     Convert4(&(orig->res_rms));
     Convert2(&(orig->gap));
     Convert4(&(orig->nearstat));
     Convert2(&(orig->num_stats));
     Convert2(&(orig->rep_p));
     Convert2(&(orig->used_p));
     Convert2(&(orig->rep_s));
     Convert2(&(orig->used_s));
     Convert2(&(orig->mag_type));
     Convert2(&(orig->rep_m));
     Convert2(&(orig->used_m));
     Convert4(&(orig->magnitude));
     Convert4(&(orig->weight));
     Convert4(&(orig->mag_rms));
     Convert4(&(orig->effective));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */






int FillProfileStruct(SUDS_PROFILE *pr, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!pr) return 0;
   bytesRead  = fread(&(pr->junk1),1, 2,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;



   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(pr->junk1));
   }
   
   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */





int FillResidualStruct(SUDS_RESIDUAL *re, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!re) return 0;

   bytesRead  = fread(&(re->event_num),1, 4,ptr);
   bytesRead += ReadStatIdent(&(re->re_name), ptr);
   bytesRead += fread(&(re->set_phase),1, 2,ptr);
   bytesRead += fread(&(re->set_tim_qual),1, 1,ptr);
   bytesRead += fread(&(re->set_amp_qual),1, 1,ptr);
   bytesRead += fread(&(re->residual),1, 4,ptr);
   bytesRead += fread(&(re->weight_used),1, 4,ptr);
   bytesRead += fread(&(re->delay),1, 4,ptr);
   bytesRead += fread(&(re->azimuth),1, 4,ptr);
   bytesRead += fread(&(re->distance),1, 4,ptr);
   bytesRead += fread(&(re->emergence),1, 4,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;


   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(re->event_num));
     Convert2(&(re->set_phase));
     Convert4(&(re->residual));
     Convert4(&(re->weight_used));
     Convert4(&(re->delay));
     Convert4(&(re->azimuth));
     Convert4(&(re->distance));
     Convert4(&(re->emergence));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */





int FillShotgatherStruct(SUDS_SHOTGATHER *sh, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!sh) return 0;
   bytesRead  = fread(&(sh->junk2),1, 2,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;



   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(sh->junk2));
   }

   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */





int FillTimeCorrectionStruct(SUDS_TIMECORRECTION *tc, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!tc) return 0;
   bytesRead = ReadStatIdent(&(tc->tm_name), ptr);
   bytesRead += fread(&(tc->time_correct),1, 8,ptr);
   bytesRead += fread(&(tc->rate_correct),1, 4,ptr);
   bytesRead += fread(&(tc->sync_code),1, 1,ptr);
   bytesRead += fread(&(tc->program),1, 1,ptr);
   bytesRead += fread(&(tc->effective_time),1, 4,ptr);
   bytesRead += fread(&(tc->spareM),1, 2,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;


   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert8(&(tc->time_correct));
     Convert4(&(tc->rate_correct));
     Convert4(&(tc->effective_time));
     Convert2(&(tc->spareM));
   }
   
   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */









int FillVelocityModelStruct(SUDS_VELMODEL *vm, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!vm) return 0;

   bytesRead  = fread(vm->netname,1, 4,ptr);
   vm->netname[3] = '\0';
   bytesRead += fread(vm->modelname,1, 6,ptr);
   vm->modelname[5] = '\0';
   bytesRead += fread(&(vm->spareE),1, 1,ptr);
   bytesRead += fread(&(vm->modeltype),1, 1,ptr);
   bytesRead += fread(&(vm->latA),1, 8,ptr);
   bytesRead += fread(&(vm->longA),1, 8,ptr);
   bytesRead += fread(&(vm->latB),1, 8,ptr);
   bytesRead += fread(&(vm->longB),1, 8,ptr);
   bytesRead += fread(&(vm->time_effective),1, 4,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;


   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert8(&(vm->latA));
     Convert8(&(vm->longA));
     Convert8(&(vm->latB));
     Convert8(&(vm->longB));
     Convert4(&(vm->time_effective));
   }


   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */






int FillTerminatorStruct(SUDS_TERMINATOR *te, SUDS_STRUCTTAG Tag, FILE *ptr)
{
   int bytesRead;

   if(!te) return 0;
   bytesRead  = fread(&(te->structid),1, 2,ptr);
   bytesRead += fread(&(te->spareA),1, 2,ptr);
   if(bytesRead > Tag.len_struct)
      return 0;
   else
      fseek ( ptr , Tag.len_struct - bytesRead , SEEK_CUR ) ;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(te->structid));
     Convert2(&(te->spareA));
   }
   
   if(Tag.len_data)fseek(ptr,Tag.len_data,SEEK_CUR);
   return 1;
}
/* ----------------------------------------------------------------------- */




