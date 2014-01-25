
#include <config.h>

#ifdef HAVE_MATLAB 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stationSets.h"

/* Matlab Specific Header File */
#include <engine.h>

#define _ENGINECALL
#include "matFuncExternal.h"

#define NHEADERVALS 28
#define STRINGLEN 136
#define BUFFER_LEN 2048

extern int doAuto;        /* if true auto-analyze data */
extern int reference;     /* pick to use for locating auto-proc window A=1, T0=2, ... */
extern float winLength;   /* length in seconds of auto-analysis window */


int InvalidMatlabPath(char *mHome)
/* See if mHome is a valid path by trying to open the main .m file (BAZ.m) */
{
   int length=strlen(mHome);
   char baz1[]="BAZ.m";
   char baz2[]="/BAZ.m";
   char *testname;
   
   FILE *testfile;
   
   if(length < 2)
      return TRUE;
      
   testname= (char *) malloc(length+7);
   strcpy(testname,mHome);

/* Allow for paths that end with / or not */      
   if(mHome[length-1] == '/')
      testfile=fopen( strcat(testname,baz1),"r");
   else
      testfile=fopen( strcat(testname,baz2),"r");
   
   if( testfile == NULL){
      free(testname);
      return TRUE;
   }
   else{
      fclose(testfile);
      free(testname);
      return FALSE;
   }

}

/* This function extracts the data required by the Matlab GUI for the 3-comp analysis
   from the list of 3-comp data, starts the Matlab computational engine, passes
   the data to the engine (after conversion to double), launches the GUI and after
   return from the GUI, copies the data back from the Matlab workspace to SAC. */
   
int engineCall()
{
	mxArray *SeisData      = NULL;
	mxArray *HeaderData    = NULL;  /* pointer to header data */
	mxArray *MatlabString  = NULL;  /* Matlab string pointer */
	mxArray *SeisData2     = NULL;
	mxArray *HeaderData2   = NULL;  /* pointer to header data */
	mxArray *MatlabString2 = NULL;  /* Matlab string pointer */
	mxArray *matWinLength  = NULL;  /* window length used in auto analysis */
	mxArray *matReference  = NULL;  /* pick reference used in auto analysis */

	float *dataPntr;		/* pointer to trace data */
	double *DbleData;		/* buffer array for copying to matlab wkspce*/
	double double2[NHEADERVALS];    /* buffer for header data for copying into matlab*/
	char *StringData;		/* Strings to pass to Matlab */
        double dblReference;
	double dblWinLength;
	
	
	char *mHome;
	char *MatlabPath;
	int PathLength;
	int NumSets,j,k,NPTS;
	int MaxDataLen;
	char ErrorBuffer[BUFFER_LEN];
        char *temp;
        int len;
	int status ;

	char initialStatus = linkedAndRunning ;

#include "matFuncInternal.h"

	/* make sure there is a path to the M-files, and if so, start the engine.
	 * The default path is $SACAUX/mat  but user can override using SACMAT variable
	 */
        if( ( temp = getenv("SACMAT") ) ==  NULL){

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
        }
        else{
           len = strlen(temp);
           mHome = (char *) calloc(len+5,sizeof(char) );
           strcpy(mHome,temp);
        }

        if(InvalidMatlabPath(mHome)){
           fprintf(stderr, "Specified M-file path %s is invalid. \n",mHome);
	   free(mHome);
           return;
        }

	/* Start MATLAB engine. */
	if ( !linkedAndRunning ) {
	    if( (status = matConnect ()) ) return status ;
	}


	/* Create the path string and send it to Matlab */	
        PathLength=strlen(mHome);
        MatlabPath= (char *) malloc(PathLength + 22);
        if( MatlabPath == NULL ){
           fprintf(stderr,"ERROR: could not allocate space for string. \n");
           exit(1);
        }
        
        strcpy(MatlabPath,"path(path,'");
        strcat(MatlabPath,mHome);
        strcat(MatlabPath,"')");
	EngEvalString(ep,MatlabPath);
	free(mHome);
	free(MatlabPath);

	/* allocate and fill the arrays to pass to matlab */
	
	printf("Allocating and filling Matlab workspace arrays... \n");
	NumSets=matGetNumStationSets();
	MaxDataLen=matGetMaxDataLen();
	
	/* Everything but strings must be passed to Matlab as double. 
	   DbleData is an array that the seismogram data is copied into
	   before being memcpy'ed to the Matlab storage. */
	   
	DbleData= (double *) malloc(MaxDataLen*sizeof(double));
	if(DbleData == (double *) NULL){
	   fprintf(stderr,"ERROR: Could not allocate memory for buffer array. \n");
	   exit(1);
	}
	SeisData=MxCreateDoubleMatrix(MaxDataLen,NumSets*3,mxREAL); /* seismogram data */
	if(SeisData == (mxArray *) NULL){
	   fprintf(stderr,"ERROR: Could not allocate Matlab workspace. \n");
	   exit(1);
	}
	HeaderData=MxCreateDoubleMatrix(NHEADERVALS,NumSets*3,mxREAL); /* seismogram data */
	if(HeaderData == (mxArray *) NULL){
	   fprintf(stderr,"ERROR: Could not allocate Matlab workspace. \n");
	   exit(1);
	}
	StringData = (char *) malloc(STRINGLEN * 3 * NumSets +1);
	if(StringData == (char *) NULL){
	   fprintf(stderr,"ERROR: Could not allocate memory for string array. \n");
	   exit(1);
	}
	

	/* Now copy the data into Matlab workspace */	
	for(j=1;j<=NumSets;j++){
	
	   /* For HORIZ1 channel (east if not rotated)... */
           /* First copy the seismogram data into matlab array...*/	                                    
	   dataPntr=matGetSeisPntr(HORIZ1,j,&NPTS);
	   if(dataPntr == (float *) NULL){
	      fprintf(stderr,"ERROR: Could not get pointer for HORIZ1-Comp data \n");
	      exit(1);
	   }
	   for(k=0;k<NPTS;k++)
	      DbleData[k]=dataPntr[k];
	   memcpy(MxGetPr(SeisData)+(j-1)*3*MaxDataLen,DbleData,NPTS*sizeof(double));
	   
	   /* Next copy float and int data into 2nd matlab array... */
           matCopyHeaderValues(HORIZ1, j, double2);
	   memcpy(MxGetPr(HeaderData)+(j-1)*3*NHEADERVALS,double2,NHEADERVALS*sizeof(double));

	   /* Next copy string data into StringData array... */
	   matCopyStrings(HORIZ1,j,StringData+(j-1)*3*STRINGLEN);


	                                  /* For HORIZ2 channel (north if not rotated)... */
	   dataPntr=matGetSeisPntr(HORIZ2,j,&NPTS);
	   if(dataPntr == (float *) NULL){
	      fprintf(stderr,"ERROR: Could not get pointer for HORIZ2-Comp data \n");
	      exit(1);
	   }
	   for(k=0;k<NPTS;k++)
	      DbleData[k]=dataPntr[k];
	   memcpy(MxGetPr(SeisData)+(3*j-2)*MaxDataLen,DbleData,NPTS*sizeof(double));
	   
	   /* Next copy float and int data into 2nd matlab array... */
           matCopyHeaderValues(HORIZ2, j, double2);
	   memcpy(MxGetPr(HeaderData)+(3*j-2)*NHEADERVALS,double2,NHEADERVALS*sizeof(double));

	   /* Next copy string data into StringData array... */
	   matCopyStrings(HORIZ2,j,StringData+(3*j-2)*STRINGLEN);
	



	                                    /* For vertical channel ... */
	   dataPntr=matGetSeisPntr(VERTICAL,j,&NPTS);
	   if(dataPntr == (float *) NULL){
	      fprintf(stderr,"ERROR: Could not get pointer for Vert-Comp data \n");
	      exit(1);
	   }
	   for(k=0;k<NPTS;k++)
	      DbleData[k]=dataPntr[k];
	   memcpy(MxGetPr(SeisData)+(3*j-1)*MaxDataLen,DbleData,NPTS*sizeof(double)); 
	   
	   /* Next copy float and int data into 2nd matlab array... */
           matCopyHeaderValues(VERTICAL, j, double2);
	   memcpy(MxGetPr(HeaderData)+(3*j-1)*NHEADERVALS,double2,NHEADERVALS*sizeof(double));

	   /* Next copy string data into StringData array... */
	   matCopyStrings(VERTICAL,j,StringData+(3*j-1)*STRINGLEN);
	
	}

	/* Now copy variables used in auto-mode into Matlab workspace. Whether these are used
	   depends on the state of doAuto... */
	matWinLength = MxCreateDoubleMatrix(1,1,mxREAL);
	dblWinLength = winLength;
	memcpy(MxGetPr(matWinLength), &dblWinLength, sizeof(double));
	MxSetName(matWinLength,"winLength");

	matReference = MxCreateDoubleMatrix(1,1,mxREAL);
	dblReference = reference;
	memcpy(MxGetPr(matReference), &dblReference, sizeof(double));
	MxSetName(matReference, "reference");


	/* copy to Matlab workspace */
	printf("Copying to Matlab workspace ... \n");
	MxSetName(SeisData,"TSeisData");  
	EngPutArray(ep,SeisData);

	MxSetName(HeaderData,"THeaderData");  
	EngPutArray(ep,HeaderData);
	
	StringData[STRINGLEN * 3 * NumSets]='\0';
        MatlabString=MxCreateString(StringData);
	MxSetName(MatlabString,"TMatlabString");
	EngPutArray(ep,MatlabString);
	EngPutArray(ep,matWinLength);
	EngPutArray(ep,matReference);
	EngEvalString(ep,"global THeaderData");
	EngEvalString(ep,"global TSeisData");
	EngEvalString(ep,"global TMatlabString");

	/* pass buffer for holding Matlab console output */
        EngOutputBuffer(ep,ErrorBuffer,BUFFER_LEN);	


	printf("Starting GUI ... \n");

	/* Now decide whether to run in GUI mode or in auto mode and take appropriate action */
	if(doAuto){
	   printf("Auto mode selected. \n");
	   printf("Reference is %d. \n",reference);
	   printf("Window length is %f. \n",winLength);
	   EngEvalString(ep,"BAZauto");
	}
	else
	   EngEvalString(ep,"BAZ"); /* execute the matlab program */

	
	printf("%s \n",ErrorBuffer);
	printf("Updating SAC workspace from Matlab. \n");
	
	/* copy possibly modified data from matlab storage */	
	SeisData2 = EngGetArray(ep,"TSeisData");
	HeaderData2 = EngGetArray(ep,"THeaderData");
	MatlabString2 = EngGetArray(ep,"TMatlabString");
        MxGetString(MatlabString2,StringData,(STRINGLEN * 3 * NumSets) );
 	EngEvalString(ep,"clear; clear global;");

	for(j=1;j<=NumSets;j++){
	
	  /* For HORIZ1 (east) channel ... */
           /* First copy the seismogram data from matlab array...*/
	   dataPntr=matGetSeisPntr(HORIZ1,j,&NPTS);
	   if(dataPntr == (float *) NULL){
	      fprintf(stderr,"ERROR: Could not get pointer for HORIZ1-Comp data \n");
	      exit(1);
	   }
	   if(SeisData2){
	      memcpy(DbleData, MxGetPr(SeisData2)+(j-1)*3*MaxDataLen,NPTS*sizeof(double));
	      for(k=0;k<NPTS;k++)
	         dataPntr[k]=DbleData[k];
	   }
	   
	   /* Then copy float and int data from 2nd matlab array... */
	   if(HeaderData2){
	      memcpy(double2,(MxGetPr(HeaderData2)+(j-1)*3*NHEADERVALS),NHEADERVALS*sizeof(double));
	      matCopyToHeader(HORIZ1,j,double2);
	   }

	   /* Next copy string data from StringData array... */
	   if(MatlabString2)
	      matCopyStringsToHeader(HORIZ1,j,StringData+(j-1)*3*STRINGLEN);
	   
	   
	   /* For HORIZ2 (north) channel ... */
	   dataPntr=matGetSeisPntr(HORIZ2,j,&NPTS);
	   if(dataPntr == (float *) NULL){
	      fprintf(stderr,"ERROR: Could not get pointer for HORIZ2-Comp data \n");
	      exit(1);
	   }
	   if(SeisData2){
	      memcpy(DbleData,MxGetPr(SeisData2)+(3*j-2)*MaxDataLen,NPTS*sizeof(double));
	      for(k=0;k<NPTS;k++)
	         dataPntr[k]=DbleData[k];
	   }
	   
	   /* Copy float and int data into 2nd matlab array... */
	   if(HeaderData2){
	      memcpy(double2,(MxGetPr(HeaderData2)+(3*j-2)*NHEADERVALS),NHEADERVALS*sizeof(double));
              matCopyToHeader(HORIZ2, j, double2);
	   }

	   /* Next copy string data from StringData array... */
	   if(MatlabString2)
	      matCopyStringsToHeader(HORIZ2,j,StringData+(3*j-2)*STRINGLEN);
	   
	   
	   /* For vertical channel ... */
	   
	   /* Copy float and int data into 2nd matlab array... */
	   if(HeaderData2){
	      memcpy(double2,(MxGetPr(HeaderData2)+(3*j-1)*NHEADERVALS),NHEADERVALS*sizeof(double));
              matCopyToHeader(VERTICAL, j, double2);
	   }

	   /* Next copy string data from StringData array... */
	   if(MatlabString2)
	      matCopyStringsToHeader(VERTICAL,j,StringData+(3*j-1)*STRINGLEN);
	   
	}

	/* Close engine and release memory used ... */
	free(DbleData);
	free(StringData);
	MxDestroyArray(SeisData);
	MxDestroyArray(HeaderData);
	MxDestroyArray(MatlabString);
	MxDestroyArray(SeisData2);
	MxDestroyArray(HeaderData2);
	MxDestroyArray(MatlabString2);
	MxDestroyArray(matReference);
	MxDestroyArray(matWinLength);

        if ( !initialStatus )
            matDisconnect ();

	printf("Done\n");
	return 0;
}


#endif /* HAVE_MATLAB */

#ifndef HAVE_MATLAB

void __engineCall_undef_symbol() { }

#endif 
