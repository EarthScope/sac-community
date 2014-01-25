#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cssb.h"
#include "timefuncs.h"
#include "cssListOps/cssStrucs.h"
#include "cssListOps/cssListStrucs.h"
#include "cssListOps/dblUserData.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "cssListOps/dblErrors.h"
#include "smDataIO.h"
#include "stringfun.h"
#include "smMemory/smMemory.h"


static int Verbose;
static int FirstInstance;
static int MaxAllowableTraces;
static int TracesInExistingTree;
static int dots;
static double MaxPhysMemToUse;


static void FreeMatrix(ComplexFloat **matrix, int Nrows, int Ncols)
{
   int j;
   if(!matrix || !Nrows || !Ncols)return;
   for(j=0;j<Nrows;j++)
      smFree(matrix[j]);
   smFree(matrix);
}
/* ----------------------------------------------------------------- */




static int WriteAffiliationTable(struct affiliationList *af, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = AFFILIATION;
   tag.len_struct = sizeof(struct affiliation);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( af->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */




static int WriteArrivalTable(struct arrivalList *ar, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = ARRIVAL;
   tag.len_struct = sizeof(struct arrival);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( ar->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */




static int WriteAssocTable(struct assocList *as, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = ASSOC;
   tag.len_struct = sizeof(struct assoc);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( as->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */




static int WriteEventTable(struct eventList *ev, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = EVENT;
   tag.len_struct = sizeof(struct event);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( ev->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */




static int WriteGregionTable(struct gregionList *gr, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = GREGION;
   tag.len_struct = sizeof(struct gregion);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( gr->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */




static int WriteInstrumentTable(struct instrumentList *in, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = INSTRUMENT;
   tag.len_struct = sizeof(struct instrument);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( in->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */




static int WriteOrigerrTable(struct origerrList *oe, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = ORIGERR;
   tag.len_struct = sizeof(struct origerr);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( oe->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */





static int WriteOriginTable(struct originList *orig, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = ORIGIN;
   tag.len_struct = sizeof(struct origin);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( orig->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */






static int WriteRemarkTable(struct remarkList *re, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = REMARK;
   tag.len_struct = sizeof(struct remark);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( re->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */






static int WriteSensorTable(struct sensorList *se, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = SENSOR;
   tag.len_struct = sizeof(struct sensor);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( se->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */





static int WriteSiteTable(struct siteList *si, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = SITE;
   tag.len_struct = sizeof(struct site);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( si->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */





static int WriteSitechanTable(struct sitechanList *sc, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = SITECHAN;
   tag.len_struct = sizeof(struct sitechan);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( sc->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */





static int WriteStassocTable(struct stassocList *sa, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = STASSOC;
   tag.len_struct = sizeof(struct stassoc);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( sa->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */





static int WriteWfdiscTable(struct wfdiscList *w, FILE* ptr)
{
   CSSBTAG tag;
   int Cmplx;
   

   /* Progress indicator in verbose mode. */
   if(Verbose){
      putchar('.');
      dots++;
      if(dots == 80){
         putchar('\n');
         dots = 0;
      }
      fflush(stdout);
   }
   
   
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = WFDISC;
   tag.len_struct = sizeof(struct wfdisc);
   if(w->element->nsamp < 1 || !w->seis)
      tag.len_data   = 0;
   else
      if(w->seis->Cmplx && w->seis->r && w->seis->i){
         Cmplx = 1;
         tag.len_data = 2 * w->element->nsamp * sizeof(float) + sizeof(int);
      }
      else if(w->seis->i){
         Cmplx = 0;
         tag.len_data = w->element->nsamp * sizeof(float) + sizeof(int);
      }
      else
         tag.len_data = 0;
   
      
            
   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( w->element, tag.len_struct, 1, ptr) != 1)return 0;
   if( w->element->nsamp < 1 ) return 1;
   
   if(w->seis && w->seis->Cmplx && w->seis->r && w->seis->i){
      if( fwrite( &Cmplx, sizeof(int), 1, ptr) != 1)return 0;
      if( fwrite( w->seis->r, sizeof(float), w->element->nsamp, ptr) != (size_t)w->element->nsamp)
         return 0;
      if( fwrite( w->seis->i, sizeof(float), w->element->nsamp, ptr) != (size_t)w->element->nsamp)
         return 0;
      return 1;
   }

   if(w->seis && w->seis->i){
      if( fwrite( &Cmplx, sizeof(int), 1, ptr) != 1)return 0;
      if( fwrite( w->seis->i, sizeof(float), w->element->nsamp, ptr) != (size_t)w->element->nsamp)
         return 0;
      return 1;
   }
   
   return 1;
}/* ----------------------------------------------------------------- */







static int WriteWftagTable(struct wftagList *wt, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = WFTAG;
   tag.len_struct = sizeof(struct wftag);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( wt->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */






static int WriteSacdataTable(struct sacdataList *sd, FILE* ptr)
{
   CSSBTAG tag;
   
   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = SACDATA;
   tag.len_struct = sizeof(struct sacdata);
   tag.len_data   = 0;

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( sd->element, tag.len_struct, 1, ptr) != 1)return 0;
   return 1;
}/* ----------------------------------------------------------------- */




static int WriteUserDataComment(char* comment, FILE* ptr)
{
   CSSBTAG tag;
   if(!comment || !strlen(comment) ) return 1;

   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = UDCOMMENT;
   tag.len_struct = 0;
   tag.len_data   = strlen(comment);
   
   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( comment, tag.len_data, 1, ptr) != 1)return 0;
   return 1;
}
/* ----------------------------------------------------------------- */





static int WriteUserDataCommentStruct(ComplexFloat *value, int Npts, char* comment, 
                                      enum dataType Type, int Index, FILE* ptr)
{
   CSSBTAG tag;
   DataCom DC;

   DC.Datalen     = Npts;
   DC.Type        = Type;
   if(comment)DC.CommentLen = strlen(comment);
   DC.Reference   = Index;


   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = DATACOM;
   tag.len_struct = sizeof(DataCom);
   tag.len_data   = 0;
   if(value)tag.len_data += Npts * sizeof(ComplexFloat);
   if(comment)tag.len_data += strlen(comment);

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( &DC,  sizeof(DataCom), 1, ptr) != 1)return 0;
   if(value) 
       if( fwrite(value, sizeof(ComplexFloat), Npts, ptr) != (size_t)Npts)return 0;

   if(comment)
      if( fwrite(comment, strlen(comment), 1, ptr) != 1)return 0;


   return 1;

}
/* ----------------------------------------------------------------- */




static int WriteUserDataMatrix(ComplexFloat **matrix, int Nrows, int Ncols, 
                               char* comment, FILE* ptr)
{

   CSSBTAG tag;
   UDmatrix UD;
   int j;
   int RowsOut = 0;

   UD.nrows       = Nrows;
   UD.ncols       = Ncols;
   if(comment)UD.CommentLen = strlen(comment);


   tag.sync       = 'S';
   tag.machine    = 'X';
   tag.id_struct  = UDMATRIX;
   tag.len_struct = sizeof(UDmatrix);
   tag.len_data   = 0;
   if(matrix)tag.len_data += Nrows * Ncols * sizeof(ComplexFloat);
   if(comment)tag.len_data += strlen(comment);

   if( fwrite( &tag, sizeof(CSSBTAG), 1, ptr) != 1)return 0;
   if( fwrite( &UD,  sizeof(UDmatrix), 1, ptr) != 1)return 0;
   for(j=0;j<Nrows;j++)
      RowsOut += fwrite( matrix[j], Ncols * sizeof(ComplexFloat), 1, ptr);
   if(RowsOut != Nrows) return 0;

   if(comment)
      if( fwrite(comment, strlen(comment), 1, ptr) != 1)return 0;
   return 1;

}
/* ----------------------------------------------------------------- */






int WriteCSSBfile(const char *WorkSetName, const char *fname, int verbose)
{
   FILE *ptr;
   struct affiliationList *af = 0;
   struct arrivalList     *ar = 0;
   struct assocList       *as = 0;
   struct eventList       *ev = 0;
   struct gregionList     *gr = 0;
   struct instrumentList  *in = 0;
   struct origerrList     *oe = 0;
   struct originList      *orig = 0;
   struct remarkList      *re = 0;
   struct sensorList      *se = 0;
   struct siteList        *si = 0;
   struct sitechanList    *sc = 0;
   struct stassocList     *sa = 0;
   struct wfdiscList       *w = 0;
   struct wftagList       *wt = 0;
   struct sacdataList     *sd = 0;
   DBlist tree;
   char *comment;
   DBdataComm DCpntr = 0;
   int Npts;
   ComplexFloat *value;
   enum dataType Type;
   int Index;
   ComplexFloat **matrix;
   int Nrows, Ncols;


   dots = 0;
   Verbose = verbose;

   if(!fname || ! strlen(fname)){
      printf("Invalid file name!\n");
      return 0;
   }


   if(!WorkSetName || !strlen(WorkSetName) ){
      printf("Invalid worksetname! Cannot write CSS binary data to file.\n");
      return 0;
   }

   if(!smChangeDefaultWorksetByName( (char*)WorkSetName )){
      printf("Workset %s is empty or does not exist!\n", WorkSetName);
      return 0;
   }
   tree = smGetDefaultTree();
   if(!tree)return 0;

   if(! (ptr = fopen( fname, "wb" ) ) ){
      printf("ERROR: Unable to open output file (%s)\n",fname);
      return 0;
   }

   do{
      af = (struct affiliationList*)dblNextTableInstance(af, tree, dbl_LIST_AFFILIATION);
      if(af) 
         if(!WriteAffiliationTable(af, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Affiliation structure!\n");
            return 0;
	 }
   }while(af);

   do{
      ar = (struct arrivalList*)dblNextTableInstance(ar, tree, dbl_LIST_ARRIVAL);
      if(ar) 
         if(!WriteArrivalTable(ar, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Arrival structure!\n");
            return 0;
	 }
   }while(ar);

   do{
      as = (struct assocList*)dblNextTableInstance(as, tree, dbl_LIST_ASSOC);
      if(as) 
         if(!WriteAssocTable(as, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Assoc structure!\n");
            return 0;
	 }
   }while(as);

   do{
      ev = (struct eventList*)dblNextTableInstance(ev, tree, dbl_LIST_EVENT);
      if(ev) 
         if(!WriteEventTable(ev, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Event structure!\n");
            return 0;
	 }
   }while(ev);

   do{
      gr = (struct gregionList *)dblNextTableInstance(gr, tree, dbl_LIST_GREGION);
      if(gr) 
         if(!WriteGregionTable(gr, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Gregion structure!\n");
            return 0;
	 }
   }while(gr);

   do{
      in = (struct instrumentList*)dblNextTableInstance(in, tree, dbl_LIST_INSTRUMENT);
      if(in) 
         if(!WriteInstrumentTable(in, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Instrument structure!\n");
            return 0;
	 }
   }while(in);

   do{
      oe = (struct origerrList*)dblNextTableInstance(oe, tree, dbl_LIST_ORIGERR);
      if(oe) 
         if(!WriteOrigerrTable(oe, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Origerr structure!\n");
            return 0;
	 }
   }while(oe);

   do{
      orig = (struct originList*)dblNextTableInstance(orig, tree, dbl_LIST_ORIGIN);
      if(orig) 
         if(!WriteOriginTable(orig, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Origin structure!\n");
            return 0;
	 }
   }while(orig);

   do{
      re = (struct remarkList*)dblNextTableInstance(re, tree, dbl_LIST_REMARK);
      if(re) 
         if(!WriteRemarkTable(re, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Remark structure!\n");
            return 0;
	 }
   }while(re);

   do{
      se = (struct sensorList*)dblNextTableInstance(se, tree, dbl_LIST_SENSOR);
      if(se) 
         if(!WriteSensorTable(se, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Sensor structure!\n");
            return 0;
	 }
   }while(se);

   do{
      si = (struct siteList*)dblNextTableInstance(si, tree, dbl_LIST_SITE);
      if(si) 
         if(!WriteSiteTable(si, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Site structure!\n");
            return 0;
	 }
   }while(si);

   do{
      sc = (struct sitechanList*)dblNextTableInstance(sc, tree, dbl_LIST_SITECHAN);
      if(sc) 
         if(!WriteSitechanTable(sc, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Sitechan structure!\n");
            return 0;
	 }
   }while(sc);

   do{
      sa = (struct stassocList*)dblNextTableInstance(sa, tree, dbl_LIST_STASSOC);
      if(sa) 
         if(!WriteStassocTable(sa, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Stassoc structure!\n");
            return 0;
	 }
   }while(sa);


   do{
      w = (struct wfdiscList*)dblNextTableInstance(w, tree, dbl_LIST_WFDISC);
      if(w) 
         if(!WriteWfdiscTable(w, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Wfdisc structure!\n");
            return 0;
	 }
   }while(w);


   do{
      wt = (struct wftagList*)dblNextTableInstance(wt, tree, dbl_LIST_WFTAG);
      if(wt) 
         if(!WriteWftagTable(wt, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Wftag structure!\n");
            return 0;
	 }
   }while(wt);


   do{
      sd = (struct sacdataList*)dblNextTableInstance(sd, tree, dbl_LIST_SACDATA);
      if(sd) 
         if(!WriteSacdataTable(sd, ptr) ){
            fclose(ptr);
            printf("\nERROR: Failure writing Sacdata structure!\n");
            return 0;
	 }
   }while(sd);


   comment = dblGetUserDataComment( tree );
   if(!WriteUserDataComment(comment, ptr) ){fclose(ptr); return 0;}
   smFree(comment);

   /* Now write out DataComment Structs... */
   do{
      if(!(DCpntr = dblGetNextDataComment(DCpntr, tree) ) )break;
      value   = dblGetUserDataComData(DCpntr, &Npts);
      comment = dblGetUserDataComComment(DCpntr);
      Type    = dblGetDCType(DCpntr);
      Index   = dblGetDCIndex(DCpntr);
      if(!WriteUserDataCommentStruct(value, Npts, comment, Type, Index, ptr)){
         printf("\nFailed to write User-data comment structure!\n");
         fclose(ptr); 
         if(value)  smFree(value);
         if(comment)smFree(comment);
         return 0;
      }
      if(value)  smFree(value);
      if(comment)smFree(comment);
   }while(DCpntr);


   /* Now write out the UserData Matrix struct... */
   Nrows = Ncols = 0;
   matrix = dblGetUserDataMatrixData(tree, &Nrows, &Ncols);
   if(matrix && Nrows && Ncols){
      comment = dblGetUserDataMatrixComment(tree);
      if(!WriteUserDataMatrix(matrix, Nrows, Ncols, comment, ptr)){
         printf("\nFailed writing User-data Matrix!\n");
         fclose(ptr);
         if(comment)smFree(comment);
         FreeMatrix(matrix, Nrows, Ncols); 
         return 0;
      }
      if(comment)smFree(comment);
      FreeMatrix(matrix, Nrows, Ncols); 
   }	 

   fclose(ptr);
   return 1;
}
/* ------------------------------------------------------- */







static int CSSTagFound(FILE *ptr, CSSBTAG *Tag)
{
  /* Read in the tag */
   if( fread( Tag, sizeof(CSSBTAG), 1, ptr ) != 1)
      return 0;
   else{
      return 1;
   }
}
/* ------------------------------------------------------- */




static int AddAffiliationStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct affiliationList *af;
   int StructLen = sizeof(struct affiliation);
   
   af = (struct affiliationList *) dblCreateTableInstance(tree, dbl_LIST_AFFILIATION);
   if(!af)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( af->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( af->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */





static int AddArrivalStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct arrivalList *ar;
   int StructLen = sizeof(struct arrival);
   
   ar = (struct arrivalList *) dblCreateTableInstance(tree, dbl_LIST_ARRIVAL);
   if(!ar)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( ar->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( ar->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */





static int AddAssocStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct assocList *as;
   int StructLen = sizeof(struct assoc);
   
   as = (struct assocList *) dblCreateTableInstance(tree, dbl_LIST_ASSOC);
   if(!as)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( as->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( as->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */





static int AddEventStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct eventList *ev;
   int StructLen = sizeof(struct event);
   
   ev = (struct eventList *) dblCreateTableInstance(tree, dbl_LIST_EVENT);
   if(!ev)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( ev->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( ev->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */





static int AddGregionStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct gregionList *gr;
   int StructLen = sizeof(struct gregion);
   
   gr = (struct gregionList *) dblCreateTableInstance(tree, dbl_LIST_GREGION);
   if(!gr)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( gr->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( gr->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */





static int AddInstrumentStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct instrumentList *in;
   int StructLen = sizeof(struct instrument);
   
   in = (struct instrumentList *) dblCreateTableInstance(tree, dbl_LIST_INSTRUMENT);
   if(!in)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( in->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( in->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */





static int AddOrigerrStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct origerrList *oe;
   int StructLen = sizeof(struct origerr);
   
   oe = (struct origerrList *) dblCreateTableInstance(tree, dbl_LIST_ORIGERR);
   if(!oe)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( oe->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( oe->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */





static int AddOriginStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct originList *orig;
   int StructLen = sizeof(struct origin);
   
   orig = (struct originList *) dblCreateTableInstance(tree, dbl_LIST_ORIGIN);
   if(!orig)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( orig->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( orig->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */






static int AddRemarkStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct remarkList *re;
   int StructLen = sizeof(struct remark);
   
   re = (struct remarkList *) dblCreateTableInstance(tree, dbl_LIST_REMARK);
   if(!re)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( re->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( re->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */






static int AddSensorStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct sensorList *se;
   int StructLen = sizeof(struct sensor);
   
   se = (struct sensorList *) dblCreateTableInstance(tree, dbl_LIST_SENSOR);
   if(!se)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( se->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( se->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */





static int AddSiteStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct siteList *si;
   int StructLen = sizeof(struct site);
   
   si = (struct siteList *) dblCreateTableInstance(tree, dbl_LIST_SITE);
   if(!si)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( si->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( si->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */





static int AddSitechanStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct sitechanList *sc;
   int StructLen = sizeof(struct sitechan);
   
   sc = (struct sitechanList *) dblCreateTableInstance(tree, dbl_LIST_SITECHAN);
   if(!sc)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( sc->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( sc->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */





static int AddStassocStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct stassocList *st;
   int StructLen = sizeof(struct stassoc);
   
   st = (struct stassocList *) dblCreateTableInstance(tree, dbl_LIST_STASSOC);
   if(!st)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( st->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( st->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */





static int AddWfdiscStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   int Cmplx;
   int npts;
   int BytesRead = 0;
   struct wfdiscList *w;
   int StructLen = sizeof(struct wfdisc);
   
   if(dblGetNumWaveformsInMemory(tree) + TracesInExistingTree >= MaxAllowableTraces){
      if(FirstInstance){
         printf("\nThere are now %d traces in memory.\n", 
                dblGetNumWaveformsInMemory(tree) + TracesInExistingTree);
         printf("Remaining traces in file will be skipped.\n");
         FirstInstance = 0;
      }
      
      fseek(ptr, Tag.len_struct + Tag.len_data, SEEK_CUR);
      return 1;
   }
   
   if(smFracPhysMemUsed() > MaxPhysMemToUse){
      if(FirstInstance){
         printf("\nWaveforms in SeisMgr memory are using more than %5.2f%% of physical memory.\n",
                MaxPhysMemToUse * 100);
         printf("No more waveforms will be read.\n");
         printf("To utilize a higher percentage of physical memory use the MAXMEM option.\n\n");
         FirstInstance = 0;
      }                
      fseek(ptr, Tag.len_struct + Tag.len_data, SEEK_CUR);
      return 1;
   }
   
   
   
   /* Progress indicator. */
   if(Verbose){
      putchar('.');
      dots++;
      if(dots == 80){
         putchar('\n');
         dots = 0;
      }
      fflush(stdout);
   }
   
   
  
   w = (struct wfdiscList *) dblCreateTableInstance(tree, dbl_LIST_WFDISC);
   if(!w)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( w->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen, SEEK_CUR);
   }
   else{
      if( fread( w->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, StructLen - Tag.len_struct, SEEK_CUR);
   }

   if(Tag.len_data< 8){ /*Must be at least one data data point plus Cmplx flag. */
      fseek(ptr, Tag.len_data, SEEK_CUR);
      return 1;
   }

   /* struct is read so now read Cmplx flag (should be 1 or 0) and get number of points*/
   if( fread( &Cmplx, sizeof(int), 1, ptr) != 1) return 0;
   BytesRead += sizeof(int);

   w->seis->Cmplx = Cmplx;
   if(Cmplx)
      npts = (Tag.len_data - sizeof(int) ) / sizeof(float) / 2;
   else
      npts = (Tag.len_data - sizeof(int) ) / sizeof(float);

   if(npts < 1){
      fseek(ptr, Tag.len_data - sizeof(int), SEEK_CUR);
      return 1;
   }

   /* Allocate the dependent variable float array... */  
   w->seis->i = (float*) smMalloc(npts * sizeof(float) );
   if(!w->seis->i){
      printf("Allocation error in AddWfdiscStruct!\n");
      return 0;
   }

   if(Cmplx){
      w->seis->r = (float*) smMalloc(npts * sizeof(float) );
      if(!w->seis->r){
         printf("Allocation error in AddWfdiscStruct!\n");
         return 0;
      }
      if(fread(w->seis->r, npts * sizeof(float), 1, ptr) != 1) return 0;
      BytesRead += npts * sizeof(float);
   }

   if(fread(w->seis->i, npts * sizeof(float), 1, ptr) != 1) return 0;
   BytesRead += npts * sizeof(float);

   fseek(ptr, Tag.len_data - BytesRead, SEEK_CUR);

   w->element->nsamp = npts;

   return 1;
}
/* ------------------------------------------------------------------ */







static int AddWftagStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct wftagList *wt;
   int StructLen = sizeof(struct wftag);
   
   wt = (struct wftagList *) dblCreateTableInstance(tree, dbl_LIST_WFTAG);
   if(!wt)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( wt->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( wt->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */






static int AddSacdataStruct( DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   struct sacdataList *sd;
   int StructLen = sizeof(struct sacdata);
   
   sd = (struct sacdataList *) dblCreateTableInstance(tree, dbl_LIST_SACDATA);
   if(!sd)return 0;

   if(StructLen < Tag.len_struct){
      if( fread( sd->element, StructLen, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_struct - StructLen + Tag.len_data, SEEK_CUR);
   }
   else{
      if( fread( sd->element, Tag.len_struct, 1, ptr ) != 1)return 0;
      fseek(ptr, Tag.len_data, SEEK_CUR);
   }

   return 1;
}
/* ------------------------------------------------------------------ */





static int AddDataComment(DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   char* comment;
   
   if(!Tag.len_data)return 1;
   comment = (char*) malloc(Tag.len_data + 1);
   if(fread(comment, Tag.len_data, 1, ptr) != 1) return 0;
   comment[Tag.len_data] = '\0';
   dblSetUserDataComment(tree, comment);
   smFree(comment);
   return 1;
}
/* ------------------------------------------------------------------ */




static int AddDCStruct(DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   DataCom DC;
   char* comment;
   ComplexFloat *data;
   int Expected;
   DBdataComm DCpntr;

   if( fread( &DC, sizeof(DataCom), 1, ptr ) != 1)return 0;
   if(! Tag.len_data) return 1;

   Expected = DC.Datalen * sizeof(ComplexFloat) + DC.CommentLen;
   if(Expected != Tag.len_data){
      fseek(ptr, Tag.len_data, SEEK_CUR);
      return 1;
   }
   data    = (ComplexFloat*) malloc(DC.Datalen * sizeof(ComplexFloat));
   comment = (char*) malloc(DC.CommentLen + 1);

   if( fread(data, DC.Datalen * sizeof(ComplexFloat), 1, ptr ) != 1) goto ERROR;
   if( fread(comment, DC.CommentLen, 1, ptr ) != 1) goto ERROR;
   comment[DC.CommentLen] = '\0';

   DCpntr = dblCreateDataComment(tree);
   dblSetUserDataComComment(DCpntr, comment);
   dblSetDCType(DCpntr, DC.Type);
   dblSetDCIndex(DCpntr, DC.Reference);
   dblSetUserDataComData( DCpntr, data, DC.Datalen);
   smFree(data);
   smFree(comment);

   return 1;
 ERROR:
   smFree(data);
   smFree(comment);
   return 0;
}
/* ------------------------------------------------------------------ */


static int AddUDmatrix(DBlist tree, CSSBTAG Tag, FILE* ptr)
{
   UDmatrix UD;
   char* comment;
   ComplexFloat **matrix;
   int Expected, j;

   comment = NULL;
   matrix = NULL;
   if( fread( &UD, sizeof(UDmatrix), 1, ptr ) != 1)return 0;
   if(! Tag.len_data) return 1;

   Expected = UD.nrows * UD.ncols * sizeof(ComplexFloat) + UD.CommentLen;
   if(Expected != Tag.len_data){
      fseek(ptr, Tag.len_data, SEEK_CUR);
      return 1;
   }
   matrix  = (ComplexFloat**) malloc(UD.nrows * sizeof(ComplexFloat*));

   for(j=0;j<UD.nrows;j++){
       matrix[j] = (ComplexFloat *) malloc(UD.ncols * sizeof(ComplexFloat) );
       if( fread(matrix[j], UD.ncols * sizeof(ComplexFloat), 1, ptr ) != 1)goto ERROR;
   }
   dblSetUserDataMatrixData(tree, UD.nrows, UD.ncols, matrix);
   FreeMatrix(matrix, UD.nrows, UD.ncols);

   comment = (char*) malloc(UD.CommentLen + 1);
   if( fread(comment, UD.CommentLen, 1, ptr ) != 1)goto ERROR;
   comment[UD.CommentLen] = '\0';
   dblSetUserDataMatrixComment(tree, comment);
   smFree(comment);

   return 1;
 ERROR:
   if(comment) {
       smFree(comment);
   }
   if(matrix) {
       FreeMatrix(matrix, UD.nrows, UD.ncols);
   }
   return 0;

}
/* ------------------------------------------------------------------ */




static int AddToTree(DBlist tree, CSSBTAG Tag, FILE* ptr)
{

   switch(Tag.id_struct){
   case AFFILIATION:
      if(!AddAffiliationStruct(tree, Tag, ptr) )return 0;
      return 1;
   case ARRIVAL:
      if(!AddArrivalStruct(tree, Tag, ptr) )return 0;
      return 1;
   case ASSOC:
      if(!AddAssocStruct(tree, Tag, ptr) )return 0;
      return 1;
   case EVENT:
      if(!AddEventStruct(tree, Tag, ptr) )return 0;
      return 1;
   case GREGION:
      if(!AddGregionStruct(tree, Tag, ptr) )return 0;
      return 1;
   case INSTRUMENT:
      if(!AddInstrumentStruct(tree, Tag, ptr) )return 0;
      return 1;
   case ORIGERR:
      if(!AddOrigerrStruct(tree, Tag, ptr) )return 0;
      return 1;
   case ORIGIN:
      if(!AddOriginStruct(tree, Tag, ptr) )return 0;
      return 1;
   case REMARK:
      if(!AddRemarkStruct(tree, Tag, ptr) )return 0;
      return 1;
   case SENSOR:
      if(!AddSensorStruct(tree, Tag, ptr) )return 0;
      return 1;
   case SITE:
      if(!AddSiteStruct(tree, Tag, ptr) )return 0;
      return 1;
   case SITECHAN:
      if(!AddSitechanStruct(tree, Tag, ptr) )return 0;
      return 1;
   case STASSOC:
      if(!AddStassocStruct(tree, Tag, ptr) )return 0;
      return 1;
   case WFDISC:
      if(!AddWfdiscStruct(tree, Tag, ptr) )return 0;
      return 1;
   case WFTAG:
      if(!AddWftagStruct(tree, Tag, ptr) )return 0;
      return 1;
   case SACDATA:
      if(!AddSacdataStruct(tree, Tag, ptr) )return 0;
      return 1;
   case UDCOMMENT:
      if(!AddDataComment(tree, Tag, ptr) )return 0;
      return 1;
   case DATACOM:
      if(!AddDCStruct(tree, Tag, ptr) )return 0;
      return 1;
   case UDMATRIX:
      if(!AddUDmatrix(tree, Tag, ptr) )return 0;
      return 1;
   default:
      fseek(ptr, Tag.len_struct + Tag.len_data, SEEK_CUR);
      return 1;
   }

}
/* ------------------------------------------------------- */






int ReadCSSBfile(const char *fname, const char *WorkSetName, int Replace, 
                 int MaxTraces, int verbose, double MaxPhysMem, int takeEvid )
{

   DBlist tree;
   DBlist tree2;

   FILE *ptr;
   CSSBTAG Tag;
   
   Verbose            = verbose;
   FirstInstance      = 1;
   MaxAllowableTraces = MaxTraces;
   MaxPhysMemToUse    = MaxPhysMem;
   dots               = 0;
   
   
   if(!fname || ! strlen(fname)){
      printf("Invalid file name!\n");
      return 0;
   }


   if(!WorkSetName || !strlen(WorkSetName) ){
      printf("Invalid worksetname! Cannot add CSS binary data to workset.\n");
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
   
   /* Check for too many traces already... */
   TracesInExistingTree = dblGetNumWaveformsInMemory(tree2);
   if(MaxTraces <= TracesInExistingTree){
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




   /* Open the file */
   if(! (ptr = fopen( fname, "rb" ) ) ){
      printf("ERROR: Unable to open file (%s).\n",fname);
      if(Replace)smDeleteWorksetByName( (char*)WorkSetName );
      dblDeleteTree(tree);
      return 0;
   }

   while( CSSTagFound( ptr, &Tag)){
      if(Tag.sync != ST_MAGIC){
         printf("File (%s) is out of sync or is not a CSS binary file!\n",fname);
         if(Replace)smDeleteWorksetByName( (char*)WorkSetName );
         fclose(ptr);
         dblDeleteTree(tree);
         return 0;
      }
      if(Tag.machine != SPARC){
         printf("File (%s) is not a CSS binary file!\n",fname);
         if(Replace)smDeleteWorksetByName( (char*)WorkSetName );
         fclose(ptr);
         dblDeleteTree(tree);
         return 0;
      }
      
      
      
      if(! AddToTree(tree, Tag, ptr) ){
         printf("Error reading data from (%s).\n",fname);
         if(Replace)smDeleteWorksetByName( (char*)WorkSetName );
         fclose(ptr);
         dblDeleteTree(tree);
         return 0;
      }

   }

   tree2 = dblMergeTrees(tree2, tree, takeEvid );
   if(tree2 != tree) {
       dblDeleteTree(tree);
   }

   fclose(ptr);
   return 1;
}
/* ------------------------------------------------------- */


