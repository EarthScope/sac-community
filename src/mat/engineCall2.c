
#include <config.h>

#ifdef HAVE_MATLAB

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>

/* Matlab Specific Header File */
#include <engine.h>

#include "matStructDefs.h"
#include "matFuncExternal.h"

#include "select.h"

#define INBUF_LEN   200
#define BUFFER_LEN  10000

#ifndef TRUE
#   define TRUE 1
#   define FALSE 0
#endif /* TRUE */

#define SAC_MAT_PROMPT "SACMAT>> $"

#include "matHeaderDataManage.h"
#include "matStructArrayOps.h"


static int  matValidatePath(void);
static char *CreateMfilePathString(void);
static char *CreateEnginePathCommand(char *mHome);
static int  ExecuteCommands(Engine *ep, char *ErrorBuffer);
static void FillBlackBoardStruct(mxArray *BlackBoard);
static void UpdateBlackBoardStruct(mxArray *BlackBoard);
static int  ReturnedDataOK(mxArray *SeisData, mxArray *structPtr);
static void UpdateStructArray(mxArray *structPtr);
static void UpdateSeisArray(mxArray *SeisData);
static void CopyDataToMatlabArrays(mxArray *SeisData, mxArray *structPtr);


/* Make sure that the path specified by user (with setmat) exists using system call */
static int matValidatePath(void)
{
   extern char matdir[]; /* If non-NULL, add this directory to matlab search path */
   int j, Nlen;
   Nlen = strlen(matdir);
   for(j = Nlen-1;j >= 0; j--)
      if(matdir[j] == ' ')matdir[j] = '\0';
   Nlen = strlen(matdir);
   if(Nlen < 1)return FALSE;
   if(Nlen == 1 && matdir[0] == '.')return FALSE;
   if(access(matdir, F_OK | X_OK))return FALSE;
   return TRUE;
}

static void RemoveDotMfromName(char *mfile)
{
  char *loc;
  
  if( loc = strstr(mfile,".m") ) *loc = 0;
  return;
}

/* Create default mfile path based on $SACAUX */
static char *CreateMfilePathString(void)
{
   char *temp = 0;
   char *mHome = 0;
   int len;


   if( ( temp = getenv("SACAUX")) == NULL){
       fprintf(stderr, "ERROR: Environmental variable SACAUX not defined.\n");
       exit(1);
   }
   else{
	   
       len = strlen(temp);
       mHome = (char *) calloc(len+5,sizeof(char) );
       strcpy(mHome,temp);
       if(mHome[len-1] == '/')
          strcat(mHome,"mat");
       else
          strcat(mHome,"/mat");
   }
   return mHome;
}

/* Create depmec mfile path based on $SACAUX */
static char *CreateDepMecMfilePathString(void)
{
   char *temp = 0;
   char *mDepMec = 0;
   int len;


   if( ( temp = getenv("SACAUX")) == NULL){
       fprintf(stderr, "ERROR: Environmental variable SACAUX not defined.\n");
       exit(1);
   }
   else{
	   
       len = strlen(temp);
       mDepMec = (char *) calloc(len+15,sizeof(char) );
       strcpy(mDepMec,temp);
       if(mDepMec[len-1] == '/')
          strcat(mDepMec,"mat/sacdepmec");
       else
          strcat(mDepMec,"/mat/sacdepmec");
   }
   return mDepMec;
}

static char *CreateEnginePathCommand(char *base)
{
   int PathLength;
   char *MatlabPath = 0;

 /* This is a matlab command like "path(path,'/us/dodge/data1/sac/aux/mat')" */	
   PathLength = strlen(base);
   MatlabPath = (char *) malloc(PathLength + strlen("path(path,'')") + 1);
   if( !MatlabPath ){
      fprintf(stderr,"ERROR: could not allocate space for string. \n");
      exit(1);
   }
        
   strcpy(MatlabPath,"path(path,'");
   strcat(MatlabPath,base);
   strcat(MatlabPath,"')");
   return MatlabPath;
}


static char *PrependEnginePathCommand(char *base)
{
   int PathLength;
   char *MatlabPath = 0;

 /* This is a matlab command like "path('/us/peterg/sac/aux/mat/sacdepmec',path)" */	
   PathLength = strlen(base);
   MatlabPath = (char *) malloc(PathLength + strlen("path('',path)") + 1);
   if( !MatlabPath ){
      fprintf(stderr,"ERROR: could not allocate space for string. \n");
      exit(1);
   }
        
   strcpy(MatlabPath,"path('"); 
   strcat(MatlabPath,base);
   strcat(MatlabPath,"',path)");
   return MatlabPath;
}

static char *EXIT[] = {  "exitmat",  "exit", "sac" };
static char *QUIT[] = {  "closemat", "quit", "q"   };

static
void process_matlab_line(char *p) {
  select_loop_continue(SELECT_OFF);
  select_loop_message(p, SELECT_MSG_SET);

  rl_callback_handler_remove();  
}

static  int ExecuteCommands(Engine *ep, char *ErrorBuffer)
{
  int i;
    extern int		( * EngEvalString )	() ;
    char *input;  
    int nerr;     


    input = (char *)malloc(sizeof(char) * INBUF_LEN );
    input[0] = '\0';
    printf("     'closemat' or 'quit' or 'q'   to return to SAC and quit MATLAB. \n");
    printf("     'exitmat'  or 'exit' or 'sac' to return to SAC and leave MATLAB engine running. \n");
    printf("     'inspector' to view workspace variables. \n");
    do{
      select_loop(SAC_MAT_PROMPT, strlen(SAC_MAT_PROMPT), 
		  input, INBUF_LEN-1, 
		  NULL, process_matlab_line);

       for(i = 0; i < sizeof(EXIT)/sizeof(char *); i++) {
	 if( strncmp(input, EXIT[i], strlen(EXIT[i])) == 0 ) {
	   fprintf(stderr, "Type 'mat' to return to your Matlab session\n");
	   free(input);
	   input = NULL;
	   return FALSE;
	 }
       }
       for(i = 0; i < sizeof(QUIT)/sizeof(char *); i++) {
	 if( strncmp(input, QUIT[i], strlen(QUIT[i])) == 0 ) {
	   free(input);
	   input = NULL;
	   return TRUE;
	 }
       }
       if( !strcmp(input, "help mat") ){
	 wrhelp("mat", 3, 1, FALSE, &nerr);
	 continue;
       }
       if( !strcmp(input,"") )continue;
       EngEvalString(ep, input);
       if(strlen(ErrorBuffer) > 0) {
	 if(ErrorBuffer[0] == '>' && ErrorBuffer[1] == '>') {
	   printf("%s\n", ErrorBuffer+3);	   
	 } else {
	   printf("%s\n", ErrorBuffer);
	 }
       }
    } while(TRUE);
    free(input);
    input = NULL;
}


static void FillBlackBoardStruct(mxArray *BlackBoard)
{
   int j;
   struct BlackBoardVars *bbPtr;
   int NumBBvars = matNumBlackBoardVars();
   mxArray *value;
#  include "matFuncInternal.h"
   
   for(j=0;j<NumBBvars;j++){
      if( bbPtr = matGetBBPtr(j) ){
	  value = MxCreateString( bbPtr->name );
	  MxSetField(BlackBoard,j,"name", value);

	  switch (bbPtr->type){
	      case INT_BBVAL:
 		 value = matSetIntBBval(bbPtr->intval); 
 		 MxSetField(BlackBoard,j,"value",value); 
		 break;
	      case FLOAT_BBVAL:
		 value = matSetFloatBBval(bbPtr->floatval);
		 MxSetField(BlackBoard,j,"value",value);
		 break;
	      case STRING_BBVAL:
		 value = MxCreateString( bbPtr->stringval );
		 MxSetField(BlackBoard,j,"value",value);
		 break;
	  }
      }
   }
}


static void UpdateBlackBoardStruct(mxArray *BlackBoard)
{
   int j;
   struct BlackBoardVars *bbPtr;
#  include "matFuncInternal.h"
   int Nrows,Ncols;
   mxArray *value;

   Nrows=MxGetM(BlackBoard);
   Ncols=MxGetN(BlackBoard);
   if(Nrows != matNumBlackBoardVars() || Ncols != 1){
       printf(" Blackboard size changed in mat. Will not be updated. \n");
       return;
   }

   for(j=0;j<matNumBlackBoardVars();j++){
       bbPtr = matGetBBPtr(j);
       value = MxGetField(BlackBoard,j,"value" );
       if( !value) continue;
       switch (bbPtr->type){
	  case INT_BBVAL:
	     if( MxIsNumeric(value) )bbPtr->intval = MxGetScalar(value);
	     break;
	  case FLOAT_BBVAL:
	     if( MxIsNumeric(value) )bbPtr->floatval = MxGetScalar(value);
	     break;
	  case STRING_BBVAL:
	     if( MxIsChar(value) )MxGetString(value,bbPtr->stringval,MxGetN(value) +1);
	     break;
       }
   }
}



static int ReturnedDataOK(mxArray *SeisData, mxArray *structPtr)
{
   int nchans = matGetNumChans();
   int MaxDataLen = matGetMaxTraceLen();
   int Nrows, Ncols;
#  include "matFuncInternal.h"

   if(!SeisData){
      printf("SeisData was cleared! No update in SAC.\n");
      return FALSE;
   }
   if(!structPtr){
      printf("structPtr was cleared! No update in SAC.\n");
      return FALSE;
   }

   Nrows=MxGetM(SeisData);
   Ncols=MxGetN(SeisData);
   if(Ncols != nchans || Nrows != MaxDataLen){
      printf("SeisData dimensions changed in Matlab! No update in SAC.\n");
      return FALSE;
   }

   Nrows=MxGetM(structPtr);
   if(Nrows != nchans){
      printf("structPtr dimensions changed in Matlab! No update in SAC.\n");
      return FALSE;
   }

   if(! MxIsDouble(SeisData) ){
      printf("Data type for SeisData was changed in Matlab! No update in SAC.\n");
      return FALSE;
   }


   return TRUE;

}

static void UpdateStructArray(mxArray *structPtr)
{
   int nchans = matGetNumChans();
   mxArray *field_value;
   int j;
#  include "matFuncInternal.h"

   for(j=0;j < nchans;j++){

      /* update times struct  */ 
      field_value = MxGetField(structPtr,j,"times" );  
      if(field_value)matUpdateTimesStruct(field_value,  matGetTimesPtr(j) );  

      /* update station struct */ 
      field_value = MxGetField(structPtr,j,"station" ); 
      if(field_value)matUpdateStationStruct(field_value,  matGetStationPtr(j) ); 

      /* update event struct  */ 
      field_value = MxGetField(structPtr,j,"event" ); 
      if(field_value)matUpdateEventStruct(field_value,  matGetEventPtr(j) ); 
		
      /* update user struct array  */ 
      field_value =  MxGetField(structPtr,j,"user" );  
      if(field_value)matUpdateUserStruct(field_value, matGetUserPtr(j) ); 

      /* update descrip struct  */ 
      field_value = MxGetField(structPtr,j,"descrip" ); 
      if(field_value)matUpdateDescripStruct(field_value, matGetDescripPtr(j) ); 

      /* update evsta struct  */ 
      field_value = MxGetField(structPtr,j,"evsta" ); 
      if(field_value)matUpdateEvstaStruct(field_value, matGetEvstaPtr(j) ); 

      /* update llnl struct */  
      field_value = MxGetField(structPtr,j,"llnl" ); 
      if(field_value)matUpdateLLNLStruct(field_value, matGetLLNLPtr(j) );  
		
      /* update depmec struct */
      field_value = MxGetField(structPtr,j,"depmec" ); 
      if(field_value)matUpdateDepMecStruct(field_value, matGetDepMecPtr(j) ); 
		
      /* update response field */
      field_value =  MxGetField(structPtr,j,"response" ); 
      if(field_value){  
 	 memcpy( (void *) matGetResponsePtr(j), (void *) 
		  MxGetPr(field_value), 10 * sizeof(double) ); 
      }	 

      /* update trcLen field and destroy memory for Matlab copy of struct */
      field_value =  MxGetField(structPtr,j,"trcLen" );  
      if(field_value){ 
 	 memcpy( (void *) matGetTraceLengthPtr(j), (void *) 
		 MxGetPr(field_value), sizeof(double) );  
      }	 

   } 
}


static void UpdateSeisArray(mxArray *SeisData)
{
   int nchans = matGetNumChans();
   int MaxDataLen = matGetMaxTraceLen();
   int Nlen;
   mxArray *field_value;
   int j;
#  include "matFuncInternal.h"


   if(MxIsComplex(SeisData) && !DataIsComplex){
      printf("Domain was changed in Matlab. Trace data not updated. \n");
      return;
   }
   if(!MxIsComplex(SeisData) && DataIsComplex){
      printf("Domain was changed in Matlab. Trace data not updated. \n");
      return;
   }



   for(j=0;j < nchans;j++){
      Nlen = matGetSeisLength(j);
      memcpy((void *) matGetSeisRealPtr(j), (void *) (MxGetPr(SeisData) + 
		      j*MaxDataLen ), Nlen*sizeof(double));
      if( MxIsComplex(SeisData) )
	   memcpy((void *) matGetSeisImagPtr(j), (void *) (MxGetPi(SeisData) + 
			   j*MaxDataLen ), Nlen*sizeof(double));
       
   }
}

static void CopyDataToMatlabArrays(mxArray *SeisData, mxArray *structPtr)
{
   int nchans = matGetNumChans();
   int MaxDataLen = matGetMaxTraceLen();
   int Nlen;
   mxArray *field_value;
   int j, k;
#  include "matFuncInternal.h"

   printf("Copying to Matlab workspace ... \n");
   for(j=0;j < nchans;j++){
	  
      Nlen = matGetSeisLength(j);   
      memcpy( (void *) (MxGetPr(SeisData)+j * MaxDataLen ),  
	       (void *) matGetSeisRealPtr(j), Nlen * sizeof(double));
      if(DataIsComplex){
	 memcpy( (void *) (MxGetPi(SeisData)+j * MaxDataLen ), (void *) 
		 matGetSeisImagPtr(j), Nlen * sizeof(double));
      }

      /* create and fill times struct */
      field_value = matCreateTimesStruct( matGetTimesPtr(j) ); 
      MxSetField(structPtr,j,"times",field_value);
		
      /* create and fill station struct */ 
      field_value = matCreateStationStruct(matGetStationPtr(j) );
      MxSetField(structPtr,j,"station",field_value);
		
      /* create and fill event struct */ 
      field_value = matCreateEventStruct(matGetEventPtr(j) );
      MxSetField(structPtr,j,"event",field_value);
		
      /* create and fill user struct array */ 
      field_value = matCreateUserStructArray(matGetUserPtr(j) );
      MxSetField(structPtr,j,"user",field_value);
	
      /* create and fill descrip struct */ 
      field_value = matCreateDescripStruct(matGetDescripPtr(j) ); 
      MxSetField(structPtr,j,"descrip",field_value);
	
      /* create and fill evsta struct */ 
      field_value = matCreateEvstaStruct(matGetEvstaPtr(j) ); 
      MxSetField(structPtr,j,"evsta",field_value); 
	
      /* create and fill llnl struct */ 
      field_value = matCreateLLNLStruct(matGetLLNLPtr(j) );
      MxSetField(structPtr,j,"llnl",field_value);
		
      /* create and fill depmec struct */
      field_value = matCreateDepMecStruct(matGetDepMecPtr(j) );
      MxSetField(structPtr,j,"depmec",field_value); 
		
      /* create and fill response field */
      field_value = MxCreateDoubleMatrix(10,1,mxREAL);
      for(k=0;k<10;k++)
	 *(MxGetPr(field_value) + k) = *(matGetResponsePtr(j) + k);
      MxSetField(structPtr, j, "response", field_value); 
		
      /* create and fill trcLen field */
      field_value = MxCreateDoubleMatrix(1,1,mxREAL);
      *MxGetPr(field_value) = *(matGetTraceLengthPtr(j) );
      MxSetField(structPtr, j, "trcLen", field_value); 
   } 
}


/* This function passes the SAC data to Matlab and calls a user-defined m-file.
   or else starts an interactive Matlab session.
 
   When the m-file returns, it copies the data from the Matlab workspace back 
   to the SAC workspace. */
   
int matEngineCall2(char *mfile)
{
	/* Engine *ep; declared below as extern. maf 970828 */
	mxArray *SeisData = NULL;	/* Matlab's copy of the trace data. */
	mxArray *structPtr = NULL;
	mxArray *BlackBoard = NULL;

	mxArray *SeisData2 = NULL;	/* Matlab's copy of the trace data. */
	mxArray *structPtr2 = NULL;
	mxArray *BlackBoard2 = NULL;

	char ErrorBuffer[BUFFER_LEN];
        extern char matdir[];           /* If non-NULL, add to matlab search path */
	char *mHome, *mDepMec, *getenv();
	char *MatlabPath;
	int MaxDataLen;
	int NumChans;
	int NumBBvars;                 /* Number of black board variables */
	int status ;
	int CloseOnExit = FALSE;

#	include "matFuncInternal.h"

	/* variables used to create the matlab structure array */
	int      ndim = 2, dims[2] = {1, 1};
	const char *field_names[] = {"times","station","event","user",
				     "descrip","evsta","llnl","depmec",
                                     "response","trcLen"};
	int      nFields=10; 
	const char *bbFieldNames[] = {"name","value"};
	int nBBfields = 2;
	int mHomelen=0;
	struct BlackBoardVars *bbPtr;


        /* Start Matlab computational engine */
	if ( !linkedAndRunning ) {
	    if( (status = matConnect () ) ) return status ;
	}

	/* Create the default sacmat path string and send it to Matlab */
	mHome = CreateMfilePathString();
	MatlabPath = CreateEnginePathCommand(mHome);
	EngEvalString(ep,MatlabPath);
	free(mHome);
	free(MatlabPath);

	/* Add the depmec path if running depmec */
	if ( mfile != NULL ) {
	  if( strcmp(mfile,"sacdepmec") == 0 ) { 
	    mDepMec = CreateDepMecMfilePathString();
	    MatlabPath = PrependEnginePathCommand(mDepMec);
	    EngEvalString(ep,MatlabPath);
	    free(mDepMec);
	    free(MatlabPath);
	  }
	}




	/* Add user-specified matdir to matlab search path (if set) */
	if(matdir && matValidatePath() ){
	     printf("Adding %s to Matlab path. \n",matdir);
	     MatlabPath = CreateEnginePathCommand(matdir);
	     EngEvalString(ep,MatlabPath);
	     free(MatlabPath);
	}


	
	printf("Allocating and filling Matlab workspace arrays... \n");
	NumChans = matGetNumChans();
	MaxDataLen = matGetMaxTraceLen();
	

	/* Create Matlab arrays... */

	/* First the trace-data matrix */
	if(DataIsComplex)
	   SeisData=MxCreateDoubleMatrix(MaxDataLen, NumChans, mxCOMPLEX); 
	else
	   SeisData=MxCreateDoubleMatrix(MaxDataLen, NumChans, mxREAL);
	if(!SeisData){
	   printf("ERROR: Could not allocate Matlab workspace. \n");
	   exit(1);
	}
	MxSetName(SeisData,"SeisData");  


	/* Then the structure array */
	dims[0] = NumChans;
	if(!( structPtr = MxCreateStructArray(ndim,dims,nFields,field_names) ) ){
     	   fprintf(stderr,
           "ERROR: Could not create SACdata struct array in matEngineCall2.\n");
     	   exit(-1);
   	}
	MxSetName(structPtr, "SACdata");



	/* Now copy the data into Matlab workspace */
	CopyDataToMatlabArrays(SeisData,structPtr);

        /* copy any blackboard variables... */
	NumBBvars = matNumBlackBoardVars();
	if (NumBBvars > 0 ){
	   dims[0] = NumBBvars;
	   if( !(BlackBoard = MxCreateStructArray(ndim,dims,nBBfields,bbFieldNames)) ){
     	      fprintf(stderr,
	      "ERROR: Could not create BlackBoard struct array in matEngineCall2.\n");
     	      exit(-1);
   	   }
	   MxSetName(BlackBoard, "BlackBoard");
	   FillBlackBoardStruct(BlackBoard);
	}
 
	printf("Matlab Compuational engine started\n");

	/* Register the arrays with the matlab engine */	   
 	EngPutArray(ep,SeisData);  
	EngPutArray(ep,structPtr);
	if(NumBBvars > 0)EngPutArray(ep,BlackBoard);
	EngEvalString(ep,"global SeisData");
	EngEvalString(ep,"global SACdata");
	if(NumBBvars > 0)EngEvalString(ep,"global BlackBoard");

	/* pass buffer for holding Matlab console output */
        EngOutputBuffer(ep,ErrorBuffer,BUFFER_LEN);	


	/* either run an mfile or a series of commands */
        if(mfile){
	   RemoveDotMfromName(mfile);
           EngEvalString(ep,mfile);
  	   printf("%s\n",ErrorBuffer);
        }   
        else{
	   CloseOnExit = ExecuteCommands(ep, ErrorBuffer);
 	}   
	

	/* copy possibly modified data from matlab storage */	
 	SeisData2 = EngGetArray(ep,"SeisData");  
 	structPtr2 = EngGetArray(ep,"SACdata"); 

	if( ReturnedDataOK(SeisData2, structPtr2) ){
	    UpdateStructArray(structPtr2);
	    UpdateSeisArray(SeisData2);
	}

	/* Update the black board */
	if(NumBBvars > 0){
            BlackBoard2 = EngGetArray(ep,"BlackBoard");
	    if(BlackBoard2)UpdateBlackBoardStruct(BlackBoard2);
	}

 	EngEvalString(ep,"clear; clear global;");
	EngEvalString(ep,"close all hidden;"); /* close any figure windows */



	/* Close engine and release memory used ... */	
	
 	MxDestroyArray(SeisData);
	if(BlackBoard)MxDestroyArray(BlackBoard);
  	MxDestroyArray(SeisData2); 
  	MxDestroyArray(structPtr); 
  	MxDestroyArray(structPtr2); 
 	if(BlackBoard2)MxDestroyArray(BlackBoard2); 

	if(CloseOnExit)matDisconnect () ;
	return 0;
}

#endif /* HAVE_MATLAB */

#ifndef HAVE_MATLAB

void __engineCall2_undef_symbol() { }

#endif 
