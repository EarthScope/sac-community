#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "cssListOps/dblPublicDefs.h"
#include "amf.h"
#include "gse.h"
#include "string_utils.h"

#define MAX_LINE 1025


int CheckSumFromIntArray(int *data, int Npts)
{
   int j;
   int Value;
   int modulo;
   int checksum;
   int MODULO_VALUE = 100000000;

   checksum = 0;
   modulo   = MODULO_VALUE;

   for(j=0;j<Npts;j++){
      Value = data[j];
      if(abs(Value) >= modulo) 
         Value -= (Value / modulo ) * modulo;

      checksum += Value;

      if(abs(checksum) >= modulo)
         checksum -= (checksum / modulo) * modulo;
   }

   return abs(checksum);
}
/* ------------------------------------------------------------- */



int ReadINTdata(FILE *ptr, int Nsamp, int *Data)
{
   char line[MAX_LINE];
   int SampsRead = 0;
   char *token;
   char delimit[] = " \t \n";
   int CheckSum, RecordedChkSum;

   while( fgetsp(line, MAX_LINE, ptr) ){
      token = strtok(line, delimit);
      while(token){
         Data[SampsRead++] = atol(token);
	 token = strtok(0, delimit);
         if(SampsRead > Nsamp){
	    printf("ERROR: More samples than listed on WID2 line!\n");
	    return 0;
	 }
      }
      if(SampsRead == Nsamp)break;
   }

   /* Compute checksum and compare to stored checksum... */
   CheckSum =  CheckSumFromIntArray(Data, Nsamp);
   if(!fgetsp(line, MAX_LINE, ptr)){
      printf("WARNING: No checksum found!\n");
      return 1;
   }
   token = strtok(line, delimit);
   if(!token || strcmp(token, "CHK2") ){
      printf("WARNING: No checksum found!\n");
      return 1;
   }
   RecordedChkSum = atol(strtok(0, delimit) );
   if(RecordedChkSum != CheckSum){
      printf("ERROR: Checksums differ!\n");
      return 0;
   }

   return 1;
}
/* -------------------------------------------------------------------------------- */



/* prototypes */
int dcomp6( char *buf, int **data) ;
int dcomp7( char *buf, int **data) ;
int dcomp8( char *buf, int **data) ;
void remdif1( int *data, int npts);


int ReadCompData(FILE *ptr, int Nsamp, int *Data, char *fmt)
{
   char line[MAX_LINE];
   int SampsRead = 0;
   int CheckSum, RecordedChkSum;
   char *cdata, *tmp;
   int *data;
   int BlkSize = 512;
   int ChrArraySize;
   int m, n;

   cdata = (char*) malloc(BlkSize);
   ChrArraySize = BlkSize;
   RecordedChkSum = 0;
   while( fgetsp(line, MAX_LINE, ptr) ){
      if(!strncmp(line, "CHK2", 4)) break;
      m = strlen(line);
      if(line[m-1] == '\n'){
	 line[m-1] = '\0';
	 m--;
      }
      if(m + SampsRead > ChrArraySize - 1){
      	 ChrArraySize += BlkSize;
         tmp = (char *) realloc(cdata, ChrArraySize);
         if(!tmp){
             FREE(cdata);
            printf("ERROR: Could not realloc chr array in ReadCompData!\n");
            return 0;
         }
         cdata = tmp;
      }
      strcpy(cdata+SampsRead, line);
      SampsRead += m;
   }
   if(!SampsRead){
      free(cdata);
      return 0;
   }
   if(!strncmp(line, "CHK2", 4))
      sscanf(line+4, "%d", &RecordedChkSum);
   else
      printf("WARNING: No checksum found!\n");

   if(!strcmp(fmt, "CMP6") || !strcmp(fmt, "CM6"))
      n = dcomp6(cdata, &data);
   else if(!strcmp(fmt, "CMP7"))
      n = dcomp7(cdata, &data);
   else if(!strcmp(fmt, "CMP8"))
      n = dcomp8(cdata, &data);
   else{
      printf("ERROR: Unknown data format: (%s)\n",fmt);
      free(cdata);
      return 0;
   }


   if(n != Nsamp){
      printf("ERROR: Number of samples decompressed != header npts!\n");
      free(cdata);
      free(data);
      return 0;
   }

   free(cdata);


   remdif1(data, Nsamp);
   remdif1(data, Nsamp);



   memcpy(Data, data, Nsamp * sizeof(int) );
   free(data); 


   /* Compute checksum and compare to stored checksum... */
   CheckSum =  CheckSumFromIntArray(Data, Nsamp);
   if(RecordedChkSum != CheckSum){
      printf("ERROR: Checksums differ!\n");
      return 0;
   }


   return 1;
}
/* ------------------------------------------------------------------------ */
