
#include <config.h>
#include "debug.h"

#ifdef HAVE_MATLAB 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "stationSets.h"
#include "mat.h"

int doAuto = FALSE;        /* if true auto-analyze data */
float winLength = 3.0;
int reference = 1;     /* pick to use for locating auto-proc window A=1, T0=2, ... */


void /*FUNCTION*/ xMAT3C(nerr)
int *nerr;
{
	int lhorz, lnpin, lnpout;
	int ic1a, ic2a,  jdfl,  ndx1, nlen,  notused;
	int NumSets;  /* Number of 3-component sets found in data set. */
        int nvals;



	/*=====================================================================
	 * PURPOSE:  To execute the action command MAT-3C.
	 *           This command launches a MATLAB GUI for 3-component analysis.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  MAT-3C/1
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    hdr:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    scm:     krottp, usraz, usrang, lnpreq
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    970313:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
	/* PROCEDURE: */
	
	*nerr = 0;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){
	   if(lklog("AUTO#$",7, &doAuto ) ){
           }

	   else if(lckey("A#$",3)){
              reference=1;
           }
	   else if(lckey("T0#$",4)){
              reference=2;
           }

	   else if(lckey("T1#$",4)){
              reference=3;
           }

	   else if(lckey("T2#$",4)){
              reference=4;
           }

	   else if(lckey("T3#$",4)){
              reference=5;
           }
	   else if(lckey("T4#$",4)){
              reference=6;
           }

	   else if(lckey("T5#$",4)){
              reference=7;
           }

	   else if(lckey("T6#$",4)){
              reference=8;
           }

	   else if(lckey("T7#$",4)){
              reference=9;
           }

	   else if(lckey("T8#$",4)){
              reference=10;
           }

	   else if(lckey("T9#$",4)){
              reference=11;
           }


	   else if( lkra( "WIN#LEN$",9, 1, 1, &winLength, &nvals ) ){
           }


           goto L_1000;

	}

	/* -  The above loop is over when one of two conditions has been met:
	 *    (1) An error in parsing has occurred.  In this case nerr is > 0 .
	 *    (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
	   return;


	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		return;

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

	   /* -- Get the file, moving header to CMHDR. */

	   getfil( jdfl, TRUE, &nlen, &ndx1, &notused, nerr );
	   if( *nerr != 0 ){
	       matFreeChanSetList();
	       return;
	   }

           matAddToChanSet(jdfl,nlen,cmmem.sacmem[ndx1]);

	}
	matTrimSets();
	NumSets=matGetNumStationSets();
	if(NumSets < 1){
	   printf("No 3-component sets found.\n");
	   return;
	}
	printf("%d station sets found.\n",NumSets );
	matListStationSets();
	if( (*nerr = engineCall()) ){
	   matFreeChanSetList();
	   return;
	}
	

	/* - Now copy the changed header variables for each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

	   /* -- Get the file, moving header to CMHDR. */

	   getfil( jdfl, FALSE, &nlen, &ndx1, &notused, nerr );
              if( *nerr != 0 )
		 break;

           matUpdateFromChanSet(jdfl);
           
                /* -- Return file to memory manager. */
                putfil( jdfl, nerr );
                if( *nerr != 0 )
                   break;
           

	}
	 
	matFreeChanSetList();	


	return;

} 

#endif /* HAVE_MATLAB */

#ifndef HAVE_MATLAB

void matlab_unavailable();

void xMAT3C(int *err) { UNUSED(err); matlab_unavailable(); }

#endif
