

#include <config.h>

#ifdef HAVE_MATLAB

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Matlab Specific Header File */
#include <engine.h>

#include "hdr.h"

#include "matStructDefs.h"
#include "matFuncExternal.h"
#include "matStructArrayOps.h"


void matSetDoubleField(mxArray *structPtr, double *dblVal, char *fieldName, int rows, int cols)
{
      mxArray *field_value;
#     include "matFuncInternal.h"
      
      if( *dblVal != SAC_UNDEFINED ){
         field_value = MxCreateDoubleMatrix(rows,cols,mxREAL);
	 *MxGetPr(field_value) = *dblVal;
         MxSetField(structPtr, 0, fieldName, field_value);
      } 
}      


void matUpdateDouble(mxArray *structPtr ,double *dblVal)
{      
#     include "matFuncInternal.h"
     if(structPtr){
        if(MxIsEmpty(structPtr) )
	   *dblVal = SAC_UNDEFINED;
	else
	   *dblVal = MxGetScalar(structPtr);
     }
}

void matUpdateString(mxArray *structPt, char *string)
{
#     include "matFuncInternal.h"
   if(structPt){
      if(MxIsEmpty(structPt) )
	 strncpy(string, SAC_CHAR_UNDEFINED, 6);
      else
         MxGetString(structPt,string, CHAR_FIELD_LEN);
   }
}

mxArray *matSetIntBBval(int value)
{
#     include "matFuncInternal.h"
   double *dblVal;
   mxArray *field_value;
   field_value = MxCreateDoubleMatrix(1,1,mxREAL);
   *MxGetPr(field_value) = value;
   return field_value;
}

mxArray *matSetFloatBBval(float value)
{
#     include "matFuncInternal.h"
   double *dblVal;
   mxArray *field_value;
   field_value = MxCreateDoubleMatrix(1,1,mxREAL);
   *MxGetPr(field_value) = value; 
   return field_value;
}

mxArray *matCreateDepMecStruct(struct hdrDepMec *depmec)
{
#     include "matFuncInternal.h"
 int      ndim = 2, dims[2] = {1, 1};
 int      nFields=9; 
 const char *field_names[] = {"checked", "flipped","signoise","snrfixed",
                              "filtertype","filterorder",
                              "lowerfiltercorner","upperfiltercorner",
                              "mtisotropicfraction"};
 mxArray  *field_value, *structPtr; 
 int j;

    structPtr = MxCreateStructArray(ndim, dims, nFields, field_names);
   if (structPtr == NULL){
     fprintf(stderr,"ERROR: Could not create struct array in matCreateDepMecStruct.\n");
     exit(-1);
   }

      /* Populate the checked field. */
      matSetDoubleField(structPtr, &depmec->checked, "checked", 1, 1); 

      /* Populate the flipped field. */ 
      matSetDoubleField(structPtr, &depmec->flipped, "flipped", 1, 1);

      /* Populate the signoise field. */ 
      matSetDoubleField(structPtr, &depmec->signoise, "signoise", 1, 1);

      /* Populate the snrfixed field. */ 
      matSetDoubleField(structPtr, &depmec->snrfixed, "snrfixed", 1, 1);

      /* Populate the filtertype field. */ 
      matSetDoubleField(structPtr, &depmec->filtertype, "filtertype", 1, 1);

      /* Populate the filterorder field. */ 
      matSetDoubleField(structPtr, &depmec->filterorder, "filterorder", 1, 1);

      /* Populate the lowerfiltercorner field. */ 
      matSetDoubleField(structPtr, &depmec->lowerfiltercorner, "lowerfiltercorner", 1, 1);

      /* Populate the upperfiltercorner field. */ 
      matSetDoubleField(structPtr, &depmec->upperfiltercorner, "upperfiltercorner", 1, 1);

      /* Populate the mtisotropicfraction field. */ 
      matSetDoubleField(structPtr, &depmec->mtisotropicfraction, "mtisotropicfraction", 1, 1);

      return structPtr;
}           

mxArray *matCreateDescripStruct(struct hdrDataDescrip *descrip)
{
#     include "matFuncInternal.h"
 int      ndim = 2, dims[2] = {1, 1};
 int      nFields=10; 
 const char *field_names[] = {"iftype", "idep","iztype","iinst","istreg","ievreg",
 			      "ievtyp","iqual","isynth","filename"};
 mxArray  *field_value, *structPtr; 
 int j;

    structPtr = MxCreateStructArray(ndim, dims, nFields, field_names);
   if (structPtr == NULL){
     fprintf(stderr,"ERROR: Could not create struct array in matCreateDescripStruct.\n");
     exit(-1);
   }

      /* Populate the iftype field. */
      matSetDoubleField(structPtr, &descrip->iftype, "iftype", 1, 1); 

      /* Populate the idep field. */ 
      matSetDoubleField(structPtr, &descrip->idep, "idep", 1, 1);

      /* Populate the iztype field. */ 
      matSetDoubleField(structPtr, &descrip->iztype, "iztype", 1, 1);

      /* Populate the iinst field. */ 
      matSetDoubleField(structPtr, &descrip->iinst, "iinst", 1, 1);

      /* Populate the istreg field. */ 
      matSetDoubleField(structPtr, &descrip->istreg, "istreg", 1, 1);

      /* Populate the ievreg field. */ 
      matSetDoubleField(structPtr, &descrip->ievreg, "ievreg", 1, 1);

      /* Populate the ievtyp field. */ 
      matSetDoubleField(structPtr, &descrip->ievtyp, "ievtyp", 1, 1);

      /* Populate the iqual field. */ 
      matSetDoubleField(structPtr, &descrip->iqual, "iqual", 1, 1);

      /* Populate the isynth field. */ 
      matSetDoubleField(structPtr, &descrip->isynth, "isynth", 1, 1);

      /* Populate the filename field. */
      if(strncmp(descrip->filename,SAC_CHAR_UNDEFINED,6)){
         field_value = MxCreateString(descrip->filename);
         MxSetField(structPtr, 0, "filename", field_value);
      } 

      return structPtr;
}           

mxArray *matCreateUserStructArray(struct hdrUser *user)
{
#     include "matFuncInternal.h"
 int      ndim = 2, dims[2] = {10, 1};
 int      nFields=2; 
 const char *field_names[] = {"data", "label"};
 mxArray  *field_value, *structPtr; 
 int j;


    structPtr = MxCreateStructArray(ndim, dims, nFields, field_names);
   if (structPtr == NULL){
     fprintf(stderr,"ERROR: Could not create struct array in matCreateUserStructArray.\n");
     exit(-1);
   }


   /* within SAC there are only 3 kuser fields, but the user[] array has 10, 7 filled with
      SAC_UNDEFINED values */   
   for(j=0;j<10;j++){
 
     /* Populate the data field. */
      if( user[j].data != SAC_UNDEFINED){
         field_value = MxCreateDoubleMatrix(1,1,mxREAL);
	 *MxGetPr(field_value) = user[j].data;
	 MxSetField(structPtr, j, "data", field_value);
      }


     /* Populate the label field. */
      if(strncmp(user[j].label,SAC_CHAR_UNDEFINED,6)){
         field_value = MxCreateString(user[j].label);
	 MxSetField(structPtr, j, "label", field_value);
      }
   }
   
   return structPtr;
   
}   

mxArray *matCreateEventStruct(struct hdrEvent *event)
{
#     include "matFuncInternal.h"
 int      ndim = 2, dims[2] = {1, 1};
 int      nFields=14; 
 const char *field_names[] = {"evla", "evlo","evel","evdp","nzyear","nzjday","nzhour",
			      "nzmin","nzsec","nzmsec","kevnm","mag","imagtyp","imagsrc"};
 mxArray  *field_value, *structPtr; 
 int j;

    structPtr = MxCreateStructArray(ndim, dims, nFields, field_names);
   if (structPtr == NULL){
     fprintf(stderr,"ERROR: Could not create struct array in matCreateEventStruct.\n");
     exit(-1);
   }


     /* Populate the evla field. */
      matSetDoubleField(structPtr, &event->evla, "evla", 1, 1); 

      /* Populate the evlo field. */ 
      matSetDoubleField(structPtr, &event->evlo, "evlo", 1, 1);

      /* Populate the evel field. */ 
      matSetDoubleField(structPtr, &event->evel, "evel", 1, 1);

      /* Populate the evdp field. */ 
      matSetDoubleField(structPtr, &event->evdp, "evdp", 1, 1);

      /* Populate the nzyear field. */
      matSetDoubleField(structPtr, &event->nzyear, "nzyear", 1, 1); 

      /* Populate the nzjday field. */ 
      matSetDoubleField(structPtr, &event->nzjday, "nzjday", 1, 1);

      /* Populate the nzhour field. */ 
      matSetDoubleField(structPtr, &event->nzhour, "nzhour", 1, 1);

      /* Populate the nzmin field. */ 
      matSetDoubleField(structPtr, &event->nzmin, "nzmin", 1, 1);

      /* Populate the nzsec field. */ 
      matSetDoubleField(structPtr, &event->nzsec, "nzsec", 1, 1);

      /* Populate the nzmsec field. */ 
      matSetDoubleField(structPtr, &event->nzmsec, "nzmsec", 1, 1);

      /* Populate the mag field. */ 
      matSetDoubleField(structPtr, &event->mag, "mag", 1, 1);

      /* Populate the imagtyp field. */ 
      matSetDoubleField(structPtr, &event->imagtyp, "imagtyp", 1, 1);
      
      /* Populate the imagsrc field. */ 
      matSetDoubleField(structPtr, &event->imagsrc, "imagsrc", 1, 1);

      /* Populate the kevnm field. */
      if(strncmp(event->kevnm,SAC_CHAR_UNDEFINED,6)){
         field_value = MxCreateString(event->kevnm);
         MxSetField(structPtr, 0, "kevnm", field_value);
      } 

      return structPtr;
}           

mxArray *matCreateEvstaStruct(struct  hdrEventSta *evsta)
{
#     include "matFuncInternal.h"
 int      ndim = 2, dims[2] = {1, 1};
 int      nFields=4; 
 const char *field_names[] = {"dist", "az","baz","gcarc"};
 mxArray  *field_value, *structPtr; 
 int j;

    structPtr = MxCreateStructArray(ndim, dims, nFields, field_names);
   if (structPtr == NULL){
     fprintf(stderr,"ERROR: Could not create struct array in matCreateEvstaStruct.\n");
     exit(-1);
   }


 /* Populate the dist field. */
      matSetDoubleField(structPtr, &evsta->dist, "dist", 1, 1); 


 /* Populate the az field. */
      matSetDoubleField(structPtr, &evsta->az, "az", 1, 1); 


 /* Populate the baz field. */
      matSetDoubleField(structPtr, &evsta->baz, "baz", 1, 1); 


 /* Populate the gcarc field. */
      matSetDoubleField(structPtr, &evsta->gcarc, "gcarc", 1, 1); 

      return structPtr;
}           

mxArray *matCreateLLNLStruct(struct hdrLLNLextensions *llnl )
{
#     include "matFuncInternal.h"
 int      ndim = 2, dims[2] = {1, 1};
 int      nFields=9; 
 const char *field_names[] = {"xminimum","xmaximum","yminimum","ymaximum",
			      "norid","nevid","nwfid","nxsize","nysize"};
 mxArray  *field_value, *structPtr; 
 int j;

   structPtr = MxCreateStructArray(ndim, dims, nFields, field_names);
   if (structPtr == NULL){
     fprintf(stderr,"ERROR: Could not create struct array in matCreateLLNLStruct.\n");
     exit(-1);
   }


 /* Populate the xminimum field. */
    matSetDoubleField(structPtr, &llnl->xminimum, "xminimum", 1, 1);

 /* Populate the xmaximum field. */
    matSetDoubleField(structPtr, &llnl->xmaximum, "xmaximum", 1, 1);

 /* Populate the yminimum field. */
    matSetDoubleField(structPtr, &llnl->yminimum, "yminimum", 1, 1); 

 /* Populate the ymaximum field. */
    matSetDoubleField(structPtr, &llnl->ymaximum, "ymaximum", 1, 1); 

 /* Populate the norid field. */
    matSetDoubleField(structPtr, &llnl->norid, "norid", 1, 1); 

 /* Populate the nevid field. */
    matSetDoubleField(structPtr, &llnl->nevid, "nevid", 1, 1); 

 /* Populate the nwfid field. */
    matSetDoubleField(structPtr, &llnl->nwfid, "nwfid", 1, 1); 

 /* Populate the nxsize field. */
    matSetDoubleField(structPtr, &llnl->nxsize, "nxsize", 1, 1); 

 /* Populate the nysize field. */
    matSetDoubleField(structPtr, &llnl->nysize, "nysize", 1, 1); 

      return structPtr;
}           

mxArray *matCreateStationStruct(struct hdrStation *station)
{
#     include "matFuncInternal.h"
 int      ndim = 2, dims[2] = {1, 1};
 int      nFields=9; 
 const char *field_names[] = {"stla", "stlo","stel","stdp","cmpaz",
 			      "cmpinc","kstnm","kcmpnm","knetwk"};
 mxArray  *field_value, *structPtr; 
 int j;

    structPtr = MxCreateStructArray(ndim, dims, nFields, field_names);
   if (structPtr == NULL){
     fprintf(stderr,"ERROR: Could not create struct array in matCreateStationStruct.\n");
     exit(-1);
   }


 /* Populate the stla field. */
      matSetDoubleField(structPtr, &station->stla, "stla", 1, 1); 

/* Populate the stlo field. */ 
      matSetDoubleField(structPtr, &station->stlo, "stlo", 1, 1);

/* Populate the stel field. */ 
      matSetDoubleField(structPtr, &station->stel, "stel", 1, 1);

/* Populate the stdp field. */ 
      matSetDoubleField(structPtr, &station->stdp, "stdp", 1, 1);

/* Populate the cmpaz field. */ 
      matSetDoubleField(structPtr, &station->cmpaz, "cmpaz", 1, 1);

/* Populate the cmpinc field. */ 
      matSetDoubleField(structPtr, &station->cmpinc, "cmpinc", 1, 1);

 /* Populate the kstnm field. */ 
     if(strncmp(station->kstnm,SAC_CHAR_UNDEFINED,6)){
        field_value = MxCreateString(station->kstnm);
        MxSetField(structPtr, 0, "kstnm", field_value);
     } 

 /* Populate the kcmpnm field. */ 
      if(strncmp(station->kcmpnm,SAC_CHAR_UNDEFINED,6)){
	 field_value = MxCreateString(station->kcmpnm);
         MxSetField(structPtr, 0, "kcmpnm", field_value);
      } 


 /* Populate the knetwk field. */ 
      if(strncmp(station->knetwk,SAC_CHAR_UNDEFINED,6)){
	 field_value = MxCreateString(station->knetwk);
         MxSetField(structPtr, 0, "knetwk", field_value); 
      }

      return structPtr;
}           

mxArray *matCreateTimesStruct(struct hdrTimes *times)
{
#     include "matFuncInternal.h"
 int      ndim = 2, dims[2] = {1, 1};
 int      nFields=29; 
 const char *field_names[] = {"delta", "b","e","o","a","t0","t1","t2","t3","t4",
			      "t5","t6","t7","t8","t9","f","ko","ka","kt0","kt1",
			      "kt2","kt3","kt4","kt5","kt6","kt7","kt8","kt9","kf"};
 static const char *kt[10];
 mxArray  *field_value, *structPtr; 
 int j;
 char fieldname[4];

    structPtr = MxCreateStructArray(ndim, dims, nFields, field_names);
   if (structPtr == NULL){
     fprintf(stderr,"ERROR: Could not create struct array in matCreateTimesStruct.\n");
     exit(-1);
   }

 /* Populate the delta field. */
      matSetDoubleField(structPtr, &times->delta, "delta", 1, 1); 

/* Populate the b field. */ 
      matSetDoubleField(structPtr, &times->b, "b", 1, 1);

/* Populate the e field. */ 
      matSetDoubleField(structPtr, &times->e, "e", 1, 1);

/* Populate the o field. */ 
      matSetDoubleField(structPtr, &times->o, "o", 1, 1);

/* Populate the a field. */ 
      matSetDoubleField(structPtr, &times->a, "a", 1, 1);

/* Populate the f field. */ 
      matSetDoubleField(structPtr, &times->f, "f", 1, 1);

/* Populate the t0 - t9 fields. */
      for(j=0;j<9;j++){
	 sprintf(fieldname,"t%d",j);
         matSetDoubleField(structPtr, times->t + j, fieldname, 1, 1);
      }


 /* Populate the ko field. */
      if(strncmp(times->ko,SAC_CHAR_UNDEFINED,6)){
         field_value = MxCreateString(times->ko);
         MxSetField(structPtr, 0, "ko", field_value);
      } 

 /* Populate the ka field. */ 
      if(strncmp(times->ka,SAC_CHAR_UNDEFINED,6)){
	 field_value = MxCreateString(times->ka);
         MxSetField(structPtr, 0, "ka", field_value); 
      }

 /* Populate the kt0 - kt9 fields. */
      for(j=0;j<9;j++){
	 sprintf(fieldname,"kt%d",j);
         if(strncmp(times->kt[j],SAC_CHAR_UNDEFINED,6)){
	    field_value = MxCreateString(times->kt[j]);
            MxSetField(structPtr, 0, fieldname, field_value); 
         }
      }

      
 /* Populate the kf field. */ 
      if(strncmp(times->kf,SAC_CHAR_UNDEFINED,6)){
	 field_value = MxCreateString(times->kf);
         MxSetField(structPtr, 0, "kf", field_value); 
      }

      return structPtr;
}           

void matUpdateTimesStruct(mxArray *structPtr,  struct hdrTimes *times )
{
#     include "matFuncInternal.h"
   mxArray  *field_value;
   char buffer[10 * CHAR_FIELD_LEN+1];
   int j,k,index;
   char fieldname[4];

   field_value = MxGetField(structPtr, 0, "delta");
   matUpdateDouble(field_value,&times->delta);
   
   field_value = MxGetField(structPtr, 0, "b");
   matUpdateDouble(field_value,&times->b);

   field_value = MxGetField(structPtr, 0, "e");
   matUpdateDouble(field_value,&times->e);

   field_value = MxGetField(structPtr, 0, "o");
   matUpdateDouble(field_value,&times->o);

   field_value = MxGetField(structPtr, 0, "a");
   matUpdateDouble(field_value,&times->a);

   field_value = MxGetField(structPtr, 0, "f");
   matUpdateDouble(field_value,&times->f);

   for(j=0;j<9;j++){
      sprintf(fieldname,"t%d",j);
      field_value = MxGetField(structPtr, 0, fieldname);
      if(field_value)matUpdateDouble(field_value,&times->t[j]);
   }


   field_value = MxGetField(structPtr, 0, "ko");
   matUpdateString(field_value, times->ko);

   field_value = MxGetField(structPtr, 0, "ka");
   matUpdateString(field_value, times->ka);


   for(j=0;j<9;j++){
      sprintf(fieldname,"kt%d",j);
      field_value = MxGetField(structPtr, 0, fieldname);
      if(field_value){
	 matUpdateString(field_value, times->kt[j]);
         times->kt[j][CHAR_FIELD_LEN-1]='\0';
      }
   }

}

void matUpdateStationStruct(mxArray *structPtr,  struct hdrStation *station )
{
#     include "matFuncInternal.h"
   mxArray  *field_value;

   field_value = MxGetField(structPtr, 0, "stla");
   matUpdateDouble(field_value,&station->stla);
   
   field_value = MxGetField(structPtr, 0, "stlo");
   matUpdateDouble(field_value,&station->stlo);

   field_value = MxGetField(structPtr, 0, "stel");
   matUpdateDouble(field_value,&station->stel);

   field_value = MxGetField(structPtr, 0, "stdp");
   matUpdateDouble(field_value,&station->stdp);

   field_value = MxGetField(structPtr, 0, "cmpaz");
   matUpdateDouble(field_value,&station->cmpaz);

   field_value = MxGetField(structPtr, 0, "cmpinc");
   matUpdateDouble(field_value,&station->cmpinc);

   field_value = MxGetField(structPtr, 0, "kstnm");
   matUpdateString(field_value, station->kstnm);

   field_value = MxGetField(structPtr, 0, "kcmpnm");
   matUpdateString(field_value, station->kcmpnm);

   field_value = MxGetField(structPtr, 0, "knetwk");
   matUpdateString(field_value, station->knetwk);

   /* MxDestroyArray(structPtr); */
}

void matUpdateEventStruct(mxArray *structPtr,  struct  hdrEvent *event )
{
#     include "matFuncInternal.h"
   mxArray  *field_value;

   field_value = MxGetField(structPtr, 0, "evla");
   matUpdateDouble(field_value,&event->evla);
   
   field_value = MxGetField(structPtr, 0, "evlo");
   matUpdateDouble(field_value,&event->evlo);

   field_value = MxGetField(structPtr, 0, "evel");
   matUpdateDouble(field_value,&event->evel);

   field_value = MxGetField(structPtr, 0, "evdp");
   matUpdateDouble(field_value,&event->evdp);

   field_value = MxGetField(structPtr, 0, "nzyear");
   matUpdateDouble(field_value,&event->nzyear);

   field_value = MxGetField(structPtr, 0, "nzjday");
   matUpdateDouble(field_value,&event->nzjday);

   field_value = MxGetField(structPtr, 0, "nzhour");
   matUpdateDouble(field_value, &event->nzhour);

   field_value = MxGetField(structPtr, 0, "nzmin");
   matUpdateDouble(field_value, &event->nzmin);

   field_value = MxGetField(structPtr, 0, "nzsec");
   matUpdateDouble(field_value, &event->nzsec);

   field_value = MxGetField(structPtr, 0, "nzmsec");
   matUpdateDouble(field_value, &event->nzmsec);

   field_value = MxGetField(structPtr, 0, "mag");
   matUpdateDouble(field_value, &event->mag);

   field_value = MxGetField(structPtr, 0, "imagtyp");
   matUpdateDouble(field_value, &event->imagtyp);

   field_value = MxGetField(structPtr, 0, "imagsrc");
   matUpdateDouble(field_value, &event->imagsrc);

   field_value = MxGetField(structPtr, 0, "kevnm");
   matUpdateString(field_value, event->kevnm);

}

void matUpdateUserStruct(mxArray *structPtr, struct hdrUser *user )
{
#     include "matFuncInternal.h"
   int j;
   mxArray  *field_value;

   for(j=0;j<10;j++){
 
     /* Update the data field. */
      field_value = MxGetField(structPtr, j, "data");
      matUpdateDouble(field_value, &user[j].data);


     /* Update the label field. */
      field_value = MxGetField(structPtr, j, "label");
      matUpdateString(field_value, user[j].label);
   }

}

void matUpdateDescripStruct(mxArray *structPtr,  struct hdrDataDescrip *descrip )
{
#     include "matFuncInternal.h"
   mxArray  *field_value;

   field_value = MxGetField(structPtr, 0, "iftype");
   matUpdateDouble(field_value,&descrip->iftype);
   
   field_value = MxGetField(structPtr, 0, "idep");
   matUpdateDouble(field_value,&descrip->idep);

   field_value = MxGetField(structPtr, 0, "iztype");
   matUpdateDouble(field_value,&descrip->iztype);

   field_value = MxGetField(structPtr, 0, "iinst");
   matUpdateDouble(field_value,&descrip->iinst);

   field_value = MxGetField(structPtr, 0, "istreg");
   matUpdateDouble(field_value,&descrip->istreg);

   field_value = MxGetField(structPtr, 0, "ievreg");
   matUpdateDouble(field_value,&descrip->ievreg);

   field_value = MxGetField(structPtr, 0, "iqual");
   matUpdateDouble(field_value, &descrip->iqual);

   field_value = MxGetField(structPtr, 0, "isynth");
   matUpdateDouble(field_value, &descrip->isynth);

   field_value = MxGetField(structPtr, 0, "filename");
   matUpdateString(field_value, descrip->filename);


}

void matUpdateEvstaStruct(mxArray *structPtr, struct hdrEventSta *evsta )
{
#     include "matFuncInternal.h"
   mxArray  *field_value;

   field_value = MxGetField(structPtr, 0, "dist");
   matUpdateDouble(field_value,&evsta->dist);

   field_value = MxGetField(structPtr, 0, "az");
   matUpdateDouble(field_value,&evsta->az);

   field_value = MxGetField(structPtr, 0, "baz");
   matUpdateDouble(field_value,&evsta->baz);

   field_value = MxGetField(structPtr, 0, "gcarc");
   matUpdateDouble(field_value,&evsta->gcarc);

}

void matUpdateLLNLStruct(mxArray *structPtr, struct hdrLLNLextensions *llnl )
{
#     include "matFuncInternal.h"
   mxArray  *field_value;

   field_value = MxGetField(structPtr, 0, "xminimum");
   matUpdateDouble(field_value,&llnl->xminimum);

   field_value = MxGetField(structPtr, 0, "xmaximum");
   matUpdateDouble(field_value,&llnl->xmaximum);

   field_value = MxGetField(structPtr, 0, "yminimum");
   matUpdateDouble(field_value,&llnl->yminimum);

   field_value = MxGetField(structPtr, 0, "ymaximum");
   matUpdateDouble(field_value,&llnl->ymaximum);

   field_value = MxGetField(structPtr, 0, "norid");
   matUpdateDouble(field_value,&llnl->norid);

   field_value = MxGetField(structPtr, 0, "nevid");
   matUpdateDouble(field_value,&llnl->nevid);

   field_value = MxGetField(structPtr, 0, "nwfid");
   matUpdateDouble(field_value,&llnl->nwfid);

   field_value = MxGetField(structPtr, 0, "nxsize");
   matUpdateDouble(field_value,&llnl->nxsize);

   field_value = MxGetField(structPtr, 0, "nysize");
   matUpdateDouble(field_value,&llnl->nysize);

}

void matUpdateDepMecStruct(mxArray *structPtr,  struct hdrDepMec *depmec )
{
#     include "matFuncInternal.h"
   mxArray  *field_value;

   field_value = MxGetField(structPtr, 0, "checked");
   matUpdateDouble(field_value,&depmec->checked);
   
   field_value = MxGetField(structPtr, 0, "flipped");
   matUpdateDouble(field_value,&depmec->flipped);

   field_value = MxGetField(structPtr, 0, "signoise");
   matUpdateDouble(field_value,&depmec->signoise);

   field_value = MxGetField(structPtr, 0, "snrfixed");
   matUpdateDouble(field_value,&depmec->snrfixed);

   field_value = MxGetField(structPtr, 0, "filtertype");
   matUpdateDouble(field_value,&depmec->filtertype);

   field_value = MxGetField(structPtr, 0, "filterorder");
   matUpdateDouble(field_value,&depmec->filterorder);

   field_value = MxGetField(structPtr, 0, "lowerfiltercorner");
   matUpdateDouble(field_value, &depmec->lowerfiltercorner);

   field_value = MxGetField(structPtr, 0, "upperfiltercorner");
   matUpdateDouble(field_value, &depmec->upperfiltercorner);

   field_value = MxGetField(structPtr, 0, "mtisotropicfraction");
   matUpdateDouble(field_value, &depmec->mtisotropicfraction);


}


#endif /* HAVE_MATLAB */

#ifndef HAVE_MATLAB

void __matStructArrayOps_undef_symbol() { }

#endif 
