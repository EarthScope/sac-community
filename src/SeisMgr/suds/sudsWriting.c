#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "suds/suds.h"
#include "suds/sudsListOps.h"
#include "suds/sudsDataConversions.h"
#include "suds/sudsWriting.h"

#include "dff.h"

#include "proto.h"

#define TagLen 12


static int WriteStatIdent(SUDS_STATIDENT *si, FILE *ptr)
{
  int bytesOut;
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(si->inst_type));
   }

   bytesOut = fwrite(si->network,1, 4, ptr);
   bytesOut += fwrite(si->st_name,1, 5, ptr);
   bytesOut += fwrite(&(si->component),1, 1, ptr);
   bytesOut += fwrite(&(si->inst_type),1, 2, ptr);
   return bytesOut;
}
/* ----------------------------------------------------------------------- */




static void FillTagIn(SUDS_STRUCTTAG *Tag, short id, int len, int dataLen)
{


   Tag->sync = 'S';   
   Tag->machine = '6';
   Tag->id_struct = id;        
   Tag->len_struct = len;           
   Tag->len_data = dataLen;            

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(Tag->id_struct));
     Convert4(&(Tag->len_struct));
     Convert4(&(Tag->len_data));
   }
}
/* ----------------------------------------------------------------------- */




static int WriteADstruct(SUDS_ATODINFO *ad, FILE *ptr, int length)
{
   SUDS_ATODINFO A;
   if(!ad) return 0;
   A = *ad;
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(A.base_address));
     Convert2(&(A.device_id));
     Convert2(&(A.device_flags));
     Convert2(&(A.extended_bufs));
     Convert2(&(A.external_mux));
   }
   
   if( fwrite( &A, 1, length, ptr ) != (size_t)length)return 0;
   return 1;
}
/* ----------------------------------------------------------------------- */





int WriteADlist(adList *adHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   adList *ad;
   short type = ATODINFO;
   int length = AD_LEN;
   FillTagIn(Tag, type, length , 0L);
   ad = adHead;
   while(ad){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteADstruct(ad->ad, ptr, length) ) return 0;
      ad = ad->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */





static int WriteDEstruct(SUDS_DETECTOR *de, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_DETECTOR D;
   if(!de) return 0;
   D = *de;
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(D.versionnum));
     Convert4(&(D.event_number));
     Convert4(&(D.spareL));
   }

   bytesOut = fwrite( &D, 1, 12, ptr ); /* 1st 12 bytes are characters */

   bytesOut += fwrite( &(D.versionnum), 1, 4, ptr );
   bytesOut += fwrite( &(D.event_number), 1, 4, ptr );
   bytesOut += fwrite( &(D.spareL), 1, 4, ptr );
   if(bytesOut != length) return 0;

   return 1;
}
/* ----------------------------------------------------------------------- */







int WriteDElist(deList *deHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   deList *de;
   short type = DETECTOR;
   int length = DE_LEN;
   FillTagIn(Tag, type, length , 0L);
   de = deHead;
   while(de){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteDEstruct(de->de, ptr, length) ) return 0;
      de = de->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */





static int WriteESstruct(SUDS_EVENTSETTING *es, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_EVENTSETTING E;

   if(!es) return 0;
   E = *es;
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert8(&(E.beginttime));
     Convert2(&(E.const1));
     Convert2(&(E.const2));
     Convert2(&(E.threshold));
     Convert2(&(E.const3));
     Convert4(&(E.minduration));
     Convert4(&(E.maxduration));
     Convert2(&(E.spareI));
   }

   bytesOut  = fwrite(E.netwname,1, 4,ptr);
   bytesOut += fwrite(&(E.beginttime),1, 8,ptr);
   bytesOut += fwrite(&(E.const1),1, 2,ptr);
   bytesOut += fwrite(&(E.const2),1, 2,ptr);
   bytesOut += fwrite(&(E.threshold),1, 2,ptr);
   bytesOut += fwrite(&(E.const3),1, 2,ptr);
   bytesOut += fwrite(&(E.minduration),1, 4,ptr);
   bytesOut += fwrite(&(E.maxduration),1, 4,ptr);
   bytesOut += fwrite(&(E.algorithm),1, 1,ptr);
   bytesOut += fwrite(&(E.spareK),1, 1,ptr);
   bytesOut += fwrite(&(E.spareI),1, 2,ptr);
   if(bytesOut != length) return 0;

   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteESlist(esList *esHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   esList *es;
   short type = EVENTSETTING;
   int length = ES_LEN;
   FillTagIn(Tag, type, length , 0L);
   es = esHead;
   while(es){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteESstruct(es->es, ptr, length) ) return 0;
      es = es->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */







static int WriteTRstruct(SUDS_TRIGGERS *tr, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_TRIGGERS T;
   if(!tr) return 0;
   T = *tr;
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(T.sta));
     Convert2(&(T.lta));
     Convert2(&(T.abs_sta));
     Convert2(&(T.abs_lta));
     Convert2(&(T.trig_value));
     Convert2(&(T.num_triggers));
     Convert8(&(T.trig_time));
   }

   bytesOut  = WriteStatIdent(&(T.tr_name), ptr);
   bytesOut += fwrite(&(T.sta),1, 2,ptr);
   bytesOut += fwrite(&(T.lta),1, 2,ptr);
   bytesOut += fwrite(&(T.abs_sta),1, 2,ptr);
   bytesOut += fwrite(&(T.abs_lta),1, 2,ptr);
   bytesOut += fwrite(&(T.trig_value),1, 2,ptr);
   bytesOut += fwrite(&(T.num_triggers),1, 2,ptr);
   bytesOut += fwrite(&(T.trig_time),1, 8,ptr);
   if(bytesOut != length) return 0;

   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteTRlist(trList *trHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   trList *tr;
   short type = TRIGGERS;
   int length = TR_LEN;
   FillTagIn(Tag, type, length , 0L);
   tr = trHead;
   while(tr){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteTRstruct(tr->tr, ptr, length) ) return 0;
      tr = tr->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */






static int WriteFEstruct(SUDS_FEATURE *fe, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_FEATURE F;
   if(!fe) return 0;
   F = *fe;
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(F.obs_phase));
     Convert2(&(F.sig_noise));
     Convert2(&(F.gain_range));
     Convert8(&(F.time));
     Convert4(&(F.amplitude));
     Convert4(&(F.period));
     Convert4(&(F.time_of_pick));
     Convert2(&(F.pick_authority));
     Convert2(&(F.pick_reader));
   }


   bytesOut = WriteStatIdent(&(F.fe_name), ptr);
   bytesOut += fwrite(&(F.obs_phase),1, 2,ptr);
   bytesOut += fwrite(&(F.onset),1, 1,ptr);
   bytesOut += fwrite(&(F.direction),1, 1,ptr);
   bytesOut += fwrite(&(F.sig_noise),1, 2,ptr);
   bytesOut += fwrite(&(F.data_source),1, 1,ptr);
   bytesOut += fwrite(&(F.tim_qual),1, 1,ptr);
   bytesOut += fwrite(&(F.amp_qual),1, 1,ptr);
   bytesOut += fwrite(&(F.ampunits),1, 1,ptr);
   bytesOut += fwrite(&(F.gain_range),1, 2,ptr);
   bytesOut += fwrite(&(F.time),1, 8,ptr);
   bytesOut += fwrite(&(F.amplitude),1, 4,ptr);
   bytesOut += fwrite(&(F.period),1, 4,ptr);
   bytesOut += fwrite(&(F.time_of_pick),1, 4,ptr);
   bytesOut += fwrite(&(F.pick_authority),1, 2,ptr);
   bytesOut += fwrite(&(F.pick_reader),1, 2,ptr);
   if(bytesOut != length) return 0;

   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteFElist(feList *feHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   feList *fe;
   short type = FEATURE;
   int length = FE_LEN;
   FillTagIn(Tag, type, length , 0L);
   fe = feHead;
   while(fe){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteFEstruct(fe->fe, ptr, length) ) return 0;
      fe = fe->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */






static int WriteSCstruct(SUDS_STATIONCOMP *sc, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_STATIONCOMP S;
   if(!sc) return 0;
   S = *sc;
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(S.azim));
     Convert2(&(S.incid));
     Convert8(&(S.st_lat));
     Convert8(&(S.st_long));
     Convert4(&(S.elev));
     Convert2(&(S.rocktype));
     Convert4(&(S.max_gain));
     Convert4(&(S.clip_value));
     Convert4(&(S.con_mvolts));
     Convert2(&(S.channel));
     Convert2(&(S.atod_gain));
     Convert4(&(S.effective));
     Convert4(&(S.clock_correct));
     Convert4(&(S.station_delay));
   }

   bytesOut  = WriteStatIdent(&(S.sc_name), ptr);
   bytesOut += fwrite(&(S.azim),1, 2,ptr);
   bytesOut += fwrite(&(S.incid),1, 2,ptr);
   bytesOut += fwrite(&(S.st_lat),1, 8,ptr);
   bytesOut += fwrite(&(S.st_long),1, 8,ptr);
   bytesOut += fwrite(&(S.elev),1, 4,ptr);
   bytesOut += fwrite(&(S.enclosure),1, 1,ptr);
   bytesOut += fwrite(&(S.annotation),1, 1,ptr);
   bytesOut += fwrite(&(S.recorder),1, 1,ptr);
   bytesOut += fwrite(&(S.rockclass),1, 1,ptr);
   bytesOut += fwrite(&(S.rocktype),1, 2,ptr);
   bytesOut += fwrite(&(S.sitecondition),1, 1,ptr);
   bytesOut += fwrite(&(S.sensor_type),1, 1,ptr);
   bytesOut += fwrite(&(S.data_type),1, 1,ptr);
   bytesOut += fwrite(&(S.data_units),1, 1,ptr);
   bytesOut += fwrite(&(S.polarity),1, 1,ptr);
   bytesOut += fwrite(&(S.st_status),1, 1,ptr);
   bytesOut += fwrite(&(S.max_gain),1, 4,ptr);
   bytesOut += fwrite(&(S.clip_value),1, 4,ptr);
   bytesOut += fwrite(&(S.con_mvolts),1, 4,ptr);
   bytesOut += fwrite(&(S.channel),1, 2,ptr);
   bytesOut += fwrite(&(S.atod_gain),1, 2,ptr);
   bytesOut += fwrite(&(S.effective),1, 4,ptr);
   bytesOut += fwrite(&(S.clock_correct),1, 4,ptr);
   bytesOut += fwrite(&(S.station_delay),1, 4,ptr);
   if(bytesOut != length) return 0;
   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteSClist(scList *scHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   scList *sc;
   short type = STATIONCOMP;
   int length = SC_LEN;
   FillTagIn(Tag, type, length , 0L);
   sc = scHead;
   while(sc){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteSCstruct(sc->sc, ptr, length) ) return 0;
      sc = sc->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */






static int WriteTSstruct(SUDS_TRIGSETTING *ts, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_TRIGSETTING T;

   if(!ts) return 0;
   T = *ts;
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert8(&(T.beginttime));
     Convert2(&(T.const1));
     Convert2(&(T.const2));
     Convert2(&(T.threshold));
     Convert2(&(T.const3));
     Convert2(&(T.const4));
     Convert2(&(T.wav_inc));
     Convert4(&(T.sweep));
     Convert4(&(T.aperture));
     Convert2(&(T.spareI));
   }


   bytesOut  = fwrite(T.netwname,1, 4,ptr);
   bytesOut += fwrite(&(T.beginttime),1, 8,ptr);
   bytesOut += fwrite(&(T.const1),1, 2,ptr);
   bytesOut += fwrite(&(T.const2),1, 2,ptr);
   bytesOut += fwrite(&(T.threshold),1, 2,ptr);
   bytesOut += fwrite(&(T.const3),1, 2,ptr);
   bytesOut += fwrite(&(T.const4),1, 2,ptr);
   bytesOut += fwrite(&(T.wav_inc),1, 2,ptr);
   bytesOut += fwrite(&(T.sweep),1, 4,ptr);
   bytesOut += fwrite(&(T.aperture),1, 4,ptr);
   bytesOut += fwrite(&(T.algorithm),1,1,ptr);
   bytesOut += fwrite(&(T.spareJ),1,1,ptr);
   bytesOut += fwrite(&(T.spareI),1, 2,ptr);
   if(bytesOut != length) return 0;

   return 1;
}
/* ----------------------------------------------------------------------- */







int WriteTSlist(tsList *tsHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   tsList *ts;
   short type = TRIGSETTING;
   int length = TS_LEN;
   FillTagIn(Tag, type, length , 0L);
   ts = tsHead;
   while(ts){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteTSstruct(ts->ts, ptr, length) ) return 0;
      ts = ts->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */



static int WriteDTstruct(SUDS_DESCRIPTRACE *dt, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_DESCRIPTRACE D;

   if(!dt) return 0;
   D = *dt;
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert8(&(D.begintime));
     Convert2(&(D.localtime));
     Convert2(&(D.digi_by));
     Convert2(&(D.processed));
     Convert4(&(D.length));
     Convert4(&(D.rate));
     Convert4(&(D.mindata));
     Convert4(&(D.maxdata));
     Convert4(&(D.avenoise));
     Convert4(&(D.numclip));
     Convert8(&(D.time_correct));
     Convert4(&(D.rate_correct));
   }


   bytesOut  = WriteStatIdent(&(D.dt_name), ptr);
   bytesOut += fwrite(&(D.begintime),1, 8,ptr);
   bytesOut += fwrite(&(D.localtime),1, 2,ptr);
   bytesOut += fwrite(&(D.datatype),1, 1,ptr);
   bytesOut += fwrite(&(D.descriptor),1, 1,ptr);
   bytesOut += fwrite(&(D.digi_by),1, 2,ptr);
   bytesOut += fwrite(&(D.processed),1, 2,ptr);
   bytesOut += fwrite(&(D.length),1, 4,ptr);
   bytesOut += fwrite(&(D.rate),1, 4,ptr);
   bytesOut += fwrite(&(D.mindata),1, 4,ptr);
   bytesOut += fwrite(&(D.maxdata),1, 4,ptr);
   bytesOut += fwrite(&(D.avenoise),1, 4,ptr);
   bytesOut += fwrite(&(D.numclip),1, 4,ptr);
   bytesOut += fwrite(&(D.time_correct),1, 8,ptr);
   bytesOut += fwrite(&(D.rate_correct),1, 4,ptr);
   if(bytesOut != length) return 0;


   return 1;
}
/* ----------------------------------------------------------------------- */





static int WriteDTdata(dtList *dt, FILE *ptr)
{
   short  *i2data;
   int    *i4data;
   float  *r4data;
   double *r8data;
   int bytesOut;
   int j;

   if(dt->dt->length < 1 ) return 1;
   switch (dt->dt->datatype){

   case 's': case 'q': case 'u': case 'i':
      i2data = (short *) malloc( dt->dt->length * sizeof(short) );
      if(!i2data){
         printf("ERROR: Could not allocate data array in FillDescripTraceStruct.\n");
         exit(1);

	 /* force output data to short */
      }
      if(dt->i2data)
         for(j=0;j<dt->dt->length;j++){
            *(i2data + j) = *(dt->i2data + j);
	    if(CheckByteOrder() == ENDIAN_BIG) {
	      Convert2(i2data + j);
	    }
         }
      else if(dt->i4data)
	for(j=0;j<dt->dt->length;j++){
	  *(i2data + j) = *(dt->i4data + j);
	  if(CheckByteOrder() == ENDIAN_BIG) {
            Convert2(i2data + j);
	  }
	}
      else if(dt->r4data)
	for(j=0;j<dt->dt->length;j++){
	  *(i2data + j) = *(dt->r4data + j);
	  if(CheckByteOrder() == ENDIAN_BIG) {
            Convert2(i2data + j);
	  }
	}
      else if(dt->r8data)
	for(j=0;j<dt->dt->length;j++){
	  *(i2data + j) = *(dt->r8data + j);
	  if(CheckByteOrder() == ENDIAN_BIG) {
            Convert2(i2data + j);
	  }
	}

      bytesOut = fwrite(i2data,1 , sizeof(short) * dt->dt->length, ptr);
      if((size_t)bytesOut != sizeof(short) * dt->dt->length){
         printf("ERROR: Expected to write %d bytes but wrote %d instead.\n",
                (int)(sizeof(short) * dt->dt->length), bytesOut);
         exit(1);
      }
      free(i2data);
      return 1;

      case '2': case 'l':
      i4data = (int *) malloc( dt->dt->length * sizeof(int) );
      if(!i4data){
         printf("ERROR: Could not allocate data array in FillDescripTraceStruct.\n");
         exit(1);

	 /* force output data to int */
      }
      if(dt->i2data)
         for(j=0;j<dt->dt->length;j++){
	   *(i4data + j) = *(dt->i2data + j);
	   if(CheckByteOrder() == ENDIAN_BIG) {
	     Convert4(i4data + j);
	   }
         }
      else if(dt->i4data)
	for(j=0;j<dt->dt->length;j++){
	  *(i4data + j) = *(dt->i4data + j);
	  if(CheckByteOrder() == ENDIAN_BIG) {
            Convert4(i4data + j);
	  }
	}
      else if(dt->r4data)
	for(j=0;j<dt->dt->length;j++){
	  *(i4data + j) = *(dt->r4data + j);
	  if(CheckByteOrder() == ENDIAN_BIG) {
            Convert4(i4data + j);
	  }
	}
      else if(dt->r8data)
	for(j=0;j<dt->dt->length;j++){
	  *(i4data + j) = *(dt->r8data + j);
	  if(CheckByteOrder() == ENDIAN_BIG) {
            Convert4(i4data + j);
	  }
	}
      
      bytesOut = fwrite(i4data,1 , sizeof(int) * dt->dt->length, ptr);
      if((size_t)bytesOut != sizeof(int) * dt->dt->length){
         printf("ERROR: Expected to write %d bytes but wrote %d instead.\n",
                (int)(sizeof(int) * dt->dt->length), bytesOut);
         exit(1);
      }
      free(i4data);
      return 1;


      case 'f':
      r4data = (float *) malloc( dt->dt->length * sizeof(float) );
      if(!r4data){
         printf("ERROR: Could not allocate data array in FillDescripTraceStruct.\n");
         exit(1);

	 /* force output data to float */
      }
      if(dt->i2data)
         for(j=0;j<dt->dt->length;j++){
	   *(r4data + j) = *(dt->i2data + j);
	   if(CheckByteOrder() == ENDIAN_BIG) {
	     Convert4(r4data + j);
	   }
         }
      else if(dt->i4data)
	for(j=0;j<dt->dt->length;j++){
	  *(r4data + j) = *(dt->i4data + j);
	  if(CheckByteOrder() == ENDIAN_BIG) {
            Convert4(r4data + j);
	  }
	}
      else if(dt->r4data)
	for(j=0;j<dt->dt->length;j++){
	  *(r4data + j) = *(dt->r4data + j);
	  if(CheckByteOrder() == ENDIAN_BIG) {
            Convert4(r4data + j);
	  }
	}
      else if(dt->r8data)
	for(j=0;j<dt->dt->length;j++){
	  *(r4data + j) = *(dt->r8data + j);
	  if(CheckByteOrder() == ENDIAN_BIG) {
            Convert4(r4data + j);
	  }
	}
      
      bytesOut = fwrite(r4data,1 , sizeof(float) * dt->dt->length, ptr);
      if((size_t)bytesOut != sizeof(float) * dt->dt->length){
         printf("ERROR: Expected to write %d bytes but wrote %d instead.\n",
                (int)(sizeof(float) * dt->dt->length), bytesOut);
         exit(1);
      }
      free(r4data);
      return 1;

      case 'd':
      r8data = (double *) malloc( dt->dt->length * sizeof(double) );
      if(!r8data){
         printf("ERROR: Could not allocate data array in FillDescripTraceStruct.\n");
         exit(1);

	 /* force output data to double */
      }
      if(dt->i2data)
         for(j=0;j<dt->dt->length;j++){
	   *(r8data + j) = *(dt->i2data + j);
	   if(CheckByteOrder() == ENDIAN_BIG) {
	     Convert8(r8data + j);
	   }
         }
      else if(dt->i4data)
	for(j=0;j<dt->dt->length;j++){
	  *(r8data + j) = *(dt->i4data + j);
	  if(CheckByteOrder() == ENDIAN_BIG) {
            Convert8(r8data + j);
	  }
	}
      else if(dt->r4data)
	for(j=0;j<dt->dt->length;j++){
	  *(r8data + j) = *(dt->r4data + j);
	  if(CheckByteOrder() == ENDIAN_BIG) {
            Convert8(r8data + j);
	  }
	}
      else if(dt->r8data)
	for(j=0;j<dt->dt->length;j++){
	  *(r8data + j) = *(dt->r8data + j);
	  if(CheckByteOrder() == ENDIAN_BIG) {
            Convert8(r8data + j);
	  }
	}

      bytesOut = fwrite(r8data,1 , sizeof(double) * dt->dt->length, ptr);
      if((size_t)bytesOut != sizeof(double) * dt->dt->length){
         printf("ERROR: Expected to write %d bytes but wrote %d instead.\n",
                (int)(sizeof(double) * dt->dt->length), bytesOut);
         exit(1);
      }
      free(r8data);
      return 1;
      }

   return 0;
}
/* ----------------------------------------------------------------------- */





int WriteDTlist(dtList *dtHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   dtList *dt;
   char Type[] = "\0\0";
   short type = DESCRIPTRACE;
   int length = DT_LEN;
   int DataLen;
   
   dt = dtHead;
   while(dt){
      switch (dt->dt->datatype){
      case 's': case 'q': case 'u': case 'i':
         DataLen = 2 *dt->dt->length;
         break;
      case '2': case 'l': case 'f':
         DataLen = 4 *dt->dt->length;
         break;
      case 'd':
         DataLen = 8 *dt->dt->length;
         break;
      default:
         Type[0] = dt->dt->datatype;
	 printf("ERROR: Unknown datatype (%s)\n",Type);
         dt = dt->next;
         continue;
      }

      FillTagIn(Tag, type, length , DataLen);
      if (fwrite( Tag, TagLen, 1, ptr) != 1) {
         printf("ERROR: Could not write tag struct.\n");
         exit(1);
      }
      if( !WriteDTstruct(dt->dt, ptr, length) ) {
         printf("ERROR: Could not write trace data struct.\n");
         exit(1);
      }
      if( !WriteDTdata(dt, ptr ) ) {
         printf("ERROR: Could not write trace data.\n");
         exit(1);
      }
      dt = dt->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */







static int WriteVMstruct(SUDS_VELMODEL *vm, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_VELMODEL V;

   if(!vm) return 0;
   V = *vm;
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert8(&(V.latA));
     Convert8(&(V.longA));
     Convert8(&(V.latB));
     Convert8(&(V.longB));
     Convert4(&(V.time_effective));
   }

   bytesOut  = fwrite(V.netname,1, 4,ptr);
   bytesOut += fwrite(V.modelname,1, 6,ptr);
   bytesOut += fwrite(&(V.spareE),1, 1,ptr);
   bytesOut += fwrite(&(V.modeltype),1, 1,ptr);
   bytesOut += fwrite(&(V.latA),1, 8,ptr);
   bytesOut += fwrite(&(V.longA),1, 8,ptr);
   bytesOut += fwrite(&(V.latB),1, 8,ptr);
   bytesOut += fwrite(&(V.longB),1, 8,ptr);
   bytesOut += fwrite(&(V.time_effective),1, 4,ptr);
   if(bytesOut != length) return 0;


   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteVMlist(vmList *vmHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   vmList *vm;
   short type = VELMODEL;
   int length = VM_LEN;
   FillTagIn(Tag, type, length , 0L);
   vm = vmHead;
   while(vm){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteVMstruct(vm->vm, ptr, length) ) return 0;
      vm = vm->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */






static int WriteTCstruct(SUDS_TIMECORRECTION *tc, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_TIMECORRECTION T;

   if(!tc) return 0;
   T = *tc;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert8(&(T.time_correct));
     Convert4(&(T.rate_correct));
     Convert4(&(T.effective_time));
     Convert2(&(T.spareM));
   }



   bytesOut = WriteStatIdent(&(T.tm_name), ptr);
   bytesOut += fwrite(&(T.time_correct),1, 8,ptr);
   bytesOut += fwrite(&(T.rate_correct),1, 4,ptr);
   bytesOut += fwrite(&(T.sync_code),1, 1,ptr);
   bytesOut += fwrite(&(T.program),1, 1,ptr);
   bytesOut += fwrite(&(T.effective_time),1, 4,ptr);
   bytesOut += fwrite(&(T.spareM),1, 2,ptr);
   if(bytesOut != length) return 0;


   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteTClist(tcList *tcHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   tcList *tc;
   short type = TIMECORRECTION;
   int length = TC_LEN;
   FillTagIn(Tag, type, length , 0L);
   tc = tcHead;
   while(tc){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteTCstruct(tc->tc, ptr, length) ) return 0;
      tc = tc->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */






static int WriteTEstruct(SUDS_TERMINATOR *te, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_TERMINATOR T;

   if(!te) return 0;
   T = *te;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(T.structid));
     Convert2(&(T.spareA));
   }

   bytesOut  = fwrite(&(T.structid),1, 2,ptr);
   bytesOut += fwrite(&(T.spareA),1, 2,ptr);
   if(bytesOut != length) return 0;


   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteTElist(teList *teHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   teList *te;
   short type = TERMINATOR;
   int length = TE_LEN;
   FillTagIn(Tag, type, length , 0L);
   te = teHead;
   while(te){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteTEstruct(te->te, ptr, length) ) return 0;
      te = te->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */







static int WriteCAstruct(SUDS_CALIBRATION *ca, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_CALIBRATION C;

   if(!ca) return 0;
   C = *ca;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(C.maxgain));
     Convert4(&(C.normaliz));
     ConvertPZArray(C.cal, NOCALPTS);
     Convert4(&(C.begint));
     Convert4(&(C.endt));
   }

   bytesOut = WriteStatIdent(&(C.ca_name), ptr);
   bytesOut += fwrite(&(C.maxgain),1, 4,ptr);
   bytesOut += fwrite(&(C.normaliz),1, 4,ptr);
   bytesOut += fwrite(C.cal,1, NOCALPTS * sizeof(SUDS_CALIBR),ptr);
   bytesOut += fwrite(&(C.begint),1, 4,ptr);
   bytesOut += fwrite(&(C.endt),1, 4,ptr);
   if(bytesOut != length) return 0;


   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteCAlist(caList *caHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   caList *ca;
   short type = CALIBRATION;
   int length = CA_LEN;
   FillTagIn(Tag, type, length , 0L);
   ca = caHead;
   while(ca){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteCAstruct(ca->ca, ptr, length) ) return 0;
      ca = ca->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */








static int WriteCOstruct(SUDS_COMMENT *co, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_COMMENT C;

   if(!co) return 0;
   C = *co;
   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(C.refer));
     Convert2(&(C.item));
     Convert2(&(C.length));
     Convert2(&(C.unused));
   }


   bytesOut  = fwrite(&(C.refer),1, 2,ptr);
   bytesOut += fwrite(&(C.item),1, 2,ptr);
   bytesOut += fwrite(&(C.length),1, 2,ptr);
   bytesOut += fwrite(&(C.unused),1, 2,ptr);
   if(bytesOut != length) return 0;
   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteCOlist(coList *coHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   coList *co;
   short type = COMMENT;
   int length = CO_LEN;
   
   co = coHead;
   while(co){
      FillTagIn(Tag, type, length , co->co->length);
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteCOstruct(co->co, ptr, length) ) return 0;
      if( fwrite(co->text,1, co->co->length, ptr) != (size_t)co->co->length)return 0;
      co = co->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */





static int WriteEVstruct(SUDS_EVENT *ev, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_EVENT E;

   if(!ev) return 0;
   E = *ev;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(E.authority));
     Convert4(&(E.number));
     Convert2(&(E.felt));
     Convert4(&(E.size));
   }

   bytesOut  = fwrite(&(E.authority),1, 2,ptr);
   bytesOut += fwrite(&(E.number),1, 4,ptr);
   bytesOut += fwrite(&(E.felt),1, 2,ptr);
   bytesOut += fwrite(&(E.mintensity),1, 1,ptr);
   bytesOut += fwrite(&(E.ev_type),1, 1,ptr);
   bytesOut += fwrite(&(E.tectonism),1, 1,ptr);
   bytesOut += fwrite(&(E.waterwave),1, 1,ptr);
   bytesOut += fwrite(&(E.mechanism),1, 1,ptr);
   bytesOut += fwrite(&(E.medium),1, 1,ptr);
   bytesOut += fwrite(&(E.size),1, 4,ptr);
   if(bytesOut != length) return 0;


   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteEVlist(evList *evHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   evList *ev;
   short type = EVENT;
   int length = EV_LEN;
   FillTagIn(Tag, type, length , 0L);
   ev = evHead;
   while(ev){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteEVstruct(ev->ev, ptr, length) ) return 0;
      ev = ev->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */







static int WriteEDstruct(SUDS_EVDESCR *ed, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_EVDESCR E;

   if(!ed) return 0;
   E = *ed;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(E.localtime));
     Convert2(&(E.spareB));
   }


   bytesOut  = fwrite(&(E.eqname),1, 20,ptr);
   bytesOut += fwrite(&(E.country),1, 16,ptr);
   bytesOut += fwrite(&(E.state),1, 16,ptr);
   bytesOut += fwrite(&(E.localtime),1, 2,ptr);
   bytesOut += fwrite(&(E.spareB),1, 2,ptr);
   if(bytesOut != length) return 0;


   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteEDlist(edList *edHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   edList *ed;
   short type = EV_DESCRIPT;
   int length = ED_LEN;
   FillTagIn(Tag, type, length , 0L);
   ed = edHead;
   while(ed){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteEDstruct(ed->ed, ptr, length) ) return 0;
      ed = ed->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */





static int WriteFOstruct(SUDS_FOCALMECH *fo, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_FOCALMECH F;

   if(!fo) return 0;
   F = *fo;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(F.astrike));
     Convert4(&(F.adip));
     Convert4(&(F.arake));
     Convert4(&(F.bstrike));
     Convert4(&(F.bdip));
     Convert4(&(F.brake));
   }


   bytesOut  = fwrite(&(F.astrike),1, 4,ptr);
   bytesOut += fwrite(&(F.adip),1, 4,ptr);
   bytesOut += fwrite(&(F.arake),1, 4,ptr);
   bytesOut += fwrite(&(F.bstrike),1, 4,ptr);
   bytesOut += fwrite(&(F.bdip),1, 4,ptr);
   bytesOut += fwrite(&(F.brake),1, 4,ptr);
   bytesOut += fwrite(&(F.prefplane),1, 1,ptr);
   bytesOut += fwrite(&(F.spareC),1, 3,ptr);
   if(bytesOut != length) return 0;


   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteFOlist(foList *foHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   foList *fo;
   short type = FOCALMECH;
   int length = FO_LEN;
   FillTagIn(Tag, type, length , 0L);
   fo = foHead;
   while(fo){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteFOstruct(fo->fo, ptr, length) ) return 0;
      fo = fo->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */







static int WriteINstruct(SUDS_INSTRUMENT *in, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_INSTRUMENT I;

   if(!in) return 0;
   I = *in;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(I.in_serial));
     Convert2(&(I.comps));
     Convert2(&(I.channel));
     Convert4(&(I.void_samp));
     Convert4(&(I.dig_con));
     Convert4(&(I.aa_corner));
     Convert4(&(I.aa_poles));
     Convert4(&(I.nat_freq));
     Convert4(&(I.damping));
     Convert4(&(I. mot_con));
     Convert4(&(I.gain));
     Convert4(&(I.local_x));
     Convert4(&(I.local_y));
     Convert4(&(I.local_z));
     Convert4(&(I.effective));
     Convert4(&(I.pre_event));
     Convert2(&(I.trig_num));
     Convert2(&(I.sn_serial));
   }


   bytesOut  = WriteStatIdent(&(I.in_name), ptr);
   bytesOut += fwrite(&(I.in_serial),1, 2,ptr);
   bytesOut += fwrite(&(I.comps),1, 2,ptr);
   bytesOut += fwrite(&(I.channel),1, 2,ptr);
   bytesOut += fwrite(&(I.sens_type),1, 1,ptr);
   bytesOut += fwrite(&(I.datatype),1, 1,ptr);
   bytesOut += fwrite(&(I.void_samp),1, 4,ptr);
   bytesOut += fwrite(&(I.dig_con),1, 4,ptr);
   bytesOut += fwrite(&(I.aa_corner),1, 4,ptr);
   bytesOut += fwrite(&(I.aa_poles),1, 4,ptr);
   bytesOut += fwrite(&(I.nat_freq),1, 4,ptr);
   bytesOut += fwrite(&(I.damping),1, 4,ptr);
   bytesOut += fwrite(&(I. mot_con),1, 4,ptr);
   bytesOut += fwrite(&(I.gain),1, 4,ptr);
   bytesOut += fwrite(&(I.local_x),1, 4,ptr);
   bytesOut += fwrite(&(I.local_y),1, 4,ptr);
   bytesOut += fwrite(&(I.local_z),1, 4,ptr);
   bytesOut += fwrite(&(I.effective),1, 4,ptr);
   bytesOut += fwrite(&(I.pre_event),1, 4,ptr);
   bytesOut += fwrite(&(I.trig_num),1, 2,ptr);
   bytesOut += fwrite(in->study,1, 6,ptr);
   bytesOut += fwrite(&(I.sn_serial),1, 2,ptr);
   if(bytesOut != length) return 0;


   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteINlist(inList *inHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   inList *in;
   short type = INSTRUMENT;
   int length = IN_LEN;
   FillTagIn(Tag, type, length , 0L);
   in = inHead;
   while(in){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteINstruct(in->in, ptr, length) ) return 0;
      in = in->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */






static int WriteLAstruct(SUDS_LAYERS *la, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_LAYERS L;

   if(!la) return 0;
   L = *la;


   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(L.thickness));
     Convert4(&(L.pveltop));
     Convert4(&(L.pvelbase));
     Convert4(&(L.sveltop));
     Convert4(&(L.svelbase));
     Convert2(&(L.function));
     Convert2(&(L.spareF));
   }


   bytesOut  = fwrite(&(L.thickness),1, 4,ptr);
   bytesOut += fwrite(&(L.pveltop),1, 4,ptr);
   bytesOut += fwrite(&(L.pvelbase),1, 4,ptr);
   bytesOut += fwrite(&(L.sveltop),1, 4,ptr);
   bytesOut += fwrite(&(L.svelbase),1, 4,ptr);
   bytesOut += fwrite(&(L.function),1, 2,ptr);
   bytesOut += fwrite(&(L.spareF),1, 2,ptr);
   if(bytesOut != length) return 0;

   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteLAlist(laList *laHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   laList *la;
   short type = LAYERS;
   int length = LA_LEN;
   FillTagIn(Tag, type, length , 0L);
   la = laHead;
   while(la){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteLAstruct(la->la, ptr, length) ) return 0;
      la = la->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */






static int WriteLOstruct(SUDS_LOCTRACE *lo, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_LOCTRACE L;

   if(!lo) return 0;
   L = *lo;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(L.beginloc));
   }
   bytesOut  = WriteStatIdent(&(L.lt_name), ptr);
   bytesOut += fwrite(&(L.fileloc),1, 4,ptr);
   bytesOut += fwrite(&(L.tapeloc),1, 4,ptr);
   bytesOut += fwrite(&(L.beginloc),1, 4,ptr);
   if(bytesOut != length) return 0;

   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteLOlist(loList *loHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   loList *lo;
   short type = LOCTRACE;
   int length = LO_LEN;
   FillTagIn(Tag, type, length , 0L);
   lo = loHead;
   while(lo){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteLOstruct(lo->lo, ptr, length) ) return 0;
      lo = lo->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */









static int WriteMOstruct(SUDS_MOMENT *mo, FILE *ptr, int length)
{
   int bytesOut;
   int j;
   SUDS_MOMENT M;

   if(!mo) return 0;
   M = *mo;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(M.sc_moment));
     for(j=0;j<6;j++)
       Convert4(&(M.norm_ten[j]));
   }

   bytesOut  = fwrite(&(M.datatypes),1, 1,ptr);
   bytesOut += fwrite(&(M.constraints),1, 1,ptr);
   bytesOut += fwrite(M.spareD,1, 2,ptr);
   bytesOut += fwrite(&(M.sc_moment),1, 4,ptr);
   bytesOut += fwrite(M.norm_ten,1, 24,ptr);
   if(bytesOut != length) return 0;
   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteMOlist(moList *moHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   moList *mo;
   short type = MOMENT;
   int length = MO_LEN;
   FillTagIn(Tag, type, length , 0L);
   mo = moHead;
   while(mo){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteMOstruct(mo->mo, ptr, length) ) return 0;
      mo = mo->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */







static int WriteORstruct(SUDS_ORIGIN *orig, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_ORIGIN O;

   if(!orig) return 0;
   O = *orig;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(O.number));
     Convert2(&(O.authority));
     Convert4(&(O.region));
     Convert8(&(O.orgtime));
     Convert8(&(O.or_lat));
     Convert8(&(O.or_long));
     Convert4(&(O.depth));
     Convert4(&(O.err_horiz));
     Convert4(&(O.err_depth));
     Convert4(&(O.res_rms));
     Convert2(&(O.gap));
     Convert4(&(O.nearstat));
     Convert2(&(O.num_stats));
     Convert2(&(O.rep_p));
     Convert2(&(O.used_p));
     Convert2(&(O.rep_s));
     Convert2(&(O. used_s));
     Convert2(&(O.mag_type));
     Convert2(&(O.rep_m));
     Convert2(&(O.used_m));
     Convert4(&(O.magnitude));
     Convert4(&(O.weight));
     Convert4(&(O.mag_rms));
     Convert4(&(O.effective));
   }


   bytesOut  = fwrite(&(O.number),1, 4,ptr);
   bytesOut += fwrite(&(O.authority),1, 2,ptr);
   bytesOut += fwrite(&(O.version),1, 1,ptr);
   bytesOut += fwrite(&(O.or_status),1, 1,ptr);
   bytesOut += fwrite(&(O.preferred),1, 1,ptr);
   bytesOut += fwrite(&(O.program),1, 1,ptr);
   bytesOut += fwrite(&(O.depcontrl),1, 1,ptr);
   bytesOut += fwrite(&(O.convergence),1, 1,ptr);
   bytesOut += fwrite(&(O.region),1, 4,ptr);
   bytesOut += fwrite(&(O.orgtime),1, 8,ptr);
   bytesOut += fwrite(&(O.or_lat),1, 8,ptr);
   bytesOut += fwrite(&(O.or_long),1, 8,ptr);
   bytesOut += fwrite(&(O.depth),1, 4,ptr);
   bytesOut += fwrite(&(O.err_horiz),1, 4,ptr);
   bytesOut += fwrite(&(O.err_depth),1, 4,ptr);
   bytesOut += fwrite(&(O.res_rms),1, 4,ptr);
   bytesOut += fwrite(O.crustmodel,1, 6,ptr);
   bytesOut += fwrite(&(O.gap),1, 2,ptr);
   bytesOut += fwrite(&(O.nearstat),1, 4,ptr);
   bytesOut += fwrite(&(O.num_stats),1, 2,ptr);
   bytesOut += fwrite(&(O.rep_p),1, 2,ptr);
   bytesOut += fwrite(&(O.used_p),1, 2,ptr);
   bytesOut += fwrite(&(O.rep_s),1, 2,ptr);
   bytesOut += fwrite(&(O. used_s),1, 2,ptr);
   bytesOut += fwrite(&(O.mag_type),1, 2,ptr);
   bytesOut += fwrite(&(O.rep_m),1, 2,ptr);
   bytesOut += fwrite(&(O.used_m),1, 2,ptr);
   bytesOut += fwrite(&(O.magnitude),1, 4,ptr);
   bytesOut += fwrite(&(O.weight),1, 4,ptr);
   bytesOut += fwrite(&(O.mag_rms),1, 4,ptr);
   bytesOut += fwrite(&(O.effective),1, 4,ptr);
   if(bytesOut != length) return 0;
   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteORlist(orList *orHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   orList *orig;
   short type = ORIGIN;
   int length = OR_LEN;
   FillTagIn(Tag, type, length , 0L);
   orig = orHead;
   while(orig){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteORstruct(orig->orig, ptr, length) ) return 0;
      orig = orig->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */






static int WritePRstruct(SUDS_PROFILE *pr, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_PROFILE P;

   if(!pr) return 0;
   P = *pr;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(P.junk1));
   }

   bytesOut  = fwrite(&(P.junk1),1, 2,ptr);
   if(bytesOut != length) return 0;
   return 1;
}
/* ----------------------------------------------------------------------- */






int WritePRlist(prList *prHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   prList *pr;
   short type = PROFILE;
   int length = PR_LEN;
   FillTagIn(Tag, type, length , 0L);
   pr = prHead;
   while(pr){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WritePRstruct(pr->pr, ptr, length) ) return 0;
      pr = pr->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */








static int WriteREstruct(SUDS_RESIDUAL *re, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_RESIDUAL R;

   if(!re) return 0;
   R = *re;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(R.event_num));
     Convert2(&(R.set_phase));
     Convert4(&(R.residual));
     Convert4(&(R.weight_used));
     Convert4(&(R.delay));
     Convert4(&(R.azimuth));
     Convert4(&(R.distance));
     Convert4(&(R.emergence));
   }

   bytesOut  = fwrite(&(R.event_num),1, 4,ptr);
   bytesOut += WriteStatIdent(&(R.re_name), ptr);
   bytesOut += fwrite(&(R.set_phase),1, 2,ptr);
   bytesOut += fwrite(&(R.set_tim_qual),1, 1,ptr);
   bytesOut += fwrite(&(R.set_amp_qual),1, 1,ptr);
   bytesOut += fwrite(&(R.residual),1, 4,ptr);
   bytesOut += fwrite(&(R.weight_used),1, 4,ptr);
   bytesOut += fwrite(&(R.delay),1, 4,ptr);
   bytesOut += fwrite(&(R.azimuth),1, 4,ptr);
   bytesOut += fwrite(&(R.distance),1, 4,ptr);
   bytesOut += fwrite(&(R.emergence),1, 4,ptr);
   if(bytesOut != length) return 0;
   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteRElist(reList *reHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   reList *re;
   short type = RESIDUAL;
   int length = RE_LEN;
   FillTagIn(Tag, type, length , 0L);
   re = reHead;
   while(re){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteREstruct(re->re, ptr, length) ) return 0;
      re = re->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */







static int WriteSHstruct(SUDS_SHOTGATHER *sh, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_SHOTGATHER S;

   if(!sh) return 0;
   S = *sh;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(S.junk2));
   }

   bytesOut  = fwrite(&(S.junk2),1, 2,ptr);
   if(bytesOut != length) return 0;
   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteSHlist(shList *shHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   shList *sh;
   short type = SHOTGATHER;
   int length = SH_LEN;
   FillTagIn(Tag, type, length , 0L);
   sh = shHead;
   while(sh){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteSHstruct(sh->sh, ptr, length) ) return 0;
      sh = sh->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */





static int WriteERstruct(SUDS_ERROR *er, FILE *ptr, int length)
{
   int bytesOut;
   int j;
   SUDS_ERROR E;

   if(!er) return 0;
   E = *er;
   if(CheckByteOrder() == ENDIAN_BIG) {
     for(j=0;j<10;j++)
       Convert4(&(E.covarr[j]));
   }

   bytesOut  = fwrite(E.covarr,1, length,ptr);
   if(bytesOut != length) return 0;
   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteERlist(erList *erHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   erList *er;
   short type = ERROR;
   int length = ER_LEN;
   FillTagIn(Tag, type, length , 0L);
   er = erHead;
   while(er){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteERstruct(er->er, ptr, length) ) return 0;
      er = er->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */





static int WriteEQstruct(SUDS_EQUIPMENT *eq, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_EQUIPMENT E;

   if(!eq) return 0;
   E = *eq;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(E.model));
     Convert2(&(E.knob1));
     Convert2(&(E.knob2));
     Convert2(&(E.reason));
     Convert4(&(E.frequency));
     Convert4(&(E.effective));
   }

   bytesOut  = WriteStatIdent(&(E.thisSID), ptr);
   bytesOut += WriteStatIdent(&(E.previous), ptr);
   bytesOut += WriteStatIdent(&(E.next), ptr);
   bytesOut += fwrite(E.serial,1, 8,ptr);
   bytesOut += fwrite(&(E.model),1, 2,ptr);
   bytesOut += fwrite(&(E.knob1),1, 2,ptr);
   bytesOut += fwrite(&(E.knob2),1, 2,ptr);
   bytesOut += fwrite(&(E.reason),1, 2,ptr);
   bytesOut += fwrite(&(E.frequency),1, 4,ptr);
   bytesOut += fwrite(&(E.effective),1, 4,ptr);
   if(bytesOut != length) return 0;
   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteEQlist(eqList *eqHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   eqList *eq;
   short type = EQUIPMENT;
   int length = EQ_LEN;
   FillTagIn(Tag, type, length , 0L);
   eq = eqHead;
   while(eq){
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      if( !WriteEQstruct(eq->eq, ptr, length) ) return 0;
      eq = eq->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */







static int WriteCSstruct(SUDS_CHANSET *cs, FILE *ptr, int length)
{
   int bytesOut;
   SUDS_CHANSET C;

   if(!cs) return 0;
   C = *cs;

   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert2(&(C.type));
     Convert2(&(C.entries));
     Convert4(&(C.active));
     Convert4(&(C.inactive));
   }

   bytesOut  = fwrite(&(C.type),1, 2,ptr);
   bytesOut += fwrite(&(C.entries),1, 2,ptr);
   bytesOut += fwrite(C.network,1, 4,ptr);
   bytesOut += fwrite(C.name,1, 5,ptr);
   bytesOut += fwrite(&(C.active),1, 4,ptr);
   bytesOut += fwrite(&(C.inactive),1, 4,ptr);
   if(bytesOut != length) return 0;
   return 1;
}
/* ----------------------------------------------------------------------- */






static int WriteCSEstruct(CHANSETENTRY cse, FILE *ptr, int length)
{
   int bytesOut;


   if(CheckByteOrder() == ENDIAN_BIG) {
     Convert4(&(cse.inst_num));
     Convert2(&(cse.stream_num));
     Convert2(&(cse.chan_num));
   }


   bytesOut  = fwrite(&(cse.inst_num),1, 4,ptr);
   bytesOut += fwrite(&(cse.stream_num),1, 2,ptr);
   bytesOut += fwrite(&(cse.chan_num),1, 2,ptr);
   bytesOut += WriteStatIdent(&(cse.st), ptr);
   if(bytesOut != length) return 0;
   return 1;
}
/* ----------------------------------------------------------------------- */






int WriteCSlist(csList *csHead, SUDS_STRUCTTAG *Tag, FILE *ptr)
{
   csList *cs;
   short type = CHANSET;
   int length = CS_LEN;
   int entries;
   int j;
   
   cs = csHead;
   while(cs){
      Tag->len_data = cs->cs->entries * CSE_LEN;
      FillTagIn(Tag, type, length , Tag->len_data);
      if (fwrite( Tag, TagLen, 1, ptr) != 1) return 0;
      entries = cs->cs->entries;
      if( !WriteCSstruct(cs->cs, ptr, length) ) return 0;
      length = CSE_LEN;
      for(j=0;j<entries;j++) 
         if( !WriteCSEstruct(cs->entry[j], ptr, length) ) return 0;
         
      cs = cs->next;
   }

   return 1;

}
/* ----------------------------------------------------------------------- */
