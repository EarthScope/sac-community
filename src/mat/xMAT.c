
#include <config.h>
#include "debug.h"

#ifdef HAVE_MATLAB

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>

#include "amf.h"
#include "dfm.h"
#include "hdr.h"
#include "com.h"
#include "bool.h"
#include "mat.h"


void xMAT(nerr)
int *nerr;
{
	int lhorz, lnpin, lnpout;
	int ic1a, ic2a,  jdfl,  ndx1, nlen,  ndx2;
        
        char *mfile = NULL;
	char PRS[] = "prs"; 
	char DEPMEC[] = "sacdepmec"; 
        
        int j;

	/*=====================================================================
	 * PURPOSE:  To execute the action command MAT.
	 *           This command passes the SAC workspace to MATLAB.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
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
	

	/* Run MATPRS if mfile is PRS or command is MATPRS */
        if(*nerr == 500){
            mfile = PRS;
	    /*            printf("Running: %s \n",mfile);*/
            *nerr = 0;
	}
	/* Run DEPMEC if mfile is DEPMEC or command is MATDEPMEC */
        else if(*nerr == 600){
            mfile = DEPMEC;
	    /*            printf("Running: %s \n",mfile);*/
            *nerr = 0;
	}
        else{
	   *nerr = 0;
           /* See if an .m file was specified on the command line. It will be assumed to
           be the first argument after the command name. */
        
           if(kmcom.nkargs > 1){
              mfile=kmcom.kargs[1];
              printf("Mfile specified is: %s \n",mfile);
           } 
	}

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		return;



        matGetBlackboardVars();   /* copy into local linked list of structs */


	/* - For each file in DFL copy the header and data into arrays to be passed to matlab engine. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

	   /* -- Get the file, moving header to CMHDR. */

	   getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
              if( *nerr != 0 ){  /* Free allocated resources and return */
                   matDestroySACdataArray();
		   matDestroyBlackboardList();
	  	   *nerr = 1;
                   return;
                }
                

	   /* Set data pointer for this channel to correct place in cmmem.sacmem */
	   if ( !matAddSACdataElement(jdfl -1 ,cmmem.sacmem[ndx1], cmmem.sacmem[ndx2], nlen) ){
	        /* Free allocated resources and return */
	        matDestroySACdataArray();
	        fprintf(stderr,"ERROR: Could not add element to SAC data array in xMAT.");
		*nerr = 1;
		return;
           }
	}

	/* call Matlab engine */
        *nerr = matEngineCall2(mfile);
	if(*nerr){
	   matDestroySACdataArray();
	   matDestroyBlackboardList();
	   return;
	}
	

	/* - Now copy the changed header variables for each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

	   /* -- Get the file, moving header to CMHDR. */

	   getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
           if( *nerr != 0 ){  /* Free allocated resources and return */
               matDestroySACdataArray();
	       matDestroyBlackboardList();
	       *nerr = 1;
               return;
           }
	   if( !matUpdateSACfromSACdataArray(jdfl -1, cmmem.sacmem[ndx1], cmmem.sacmem[ndx2], nlen) ) {
	        matDestroySACdataArray();
		matDestroyBlackboardList();
	        fprintf(stderr,"ERROR: Could not update SAC data from array in xMAT.");
		*nerr = 1;
		return;
           }


 

           
           /* -- Return file to memory manager. */
           putfil( jdfl, nerr );
           if( *nerr != 0 ){  /* Free allocated resources and return */
               matDestroySACdataArray();
	       matDestroyBlackboardList();
	       *nerr = 1;
               return;
           }
           

	}
	matSetBlackboardVars();
	matDestroyBlackboardList();
        matDestroySACdataArray(); 
        return;
        
} 

#endif /* HAVE_MATLAB */

#ifndef HAVE_MATLAB

void matlab_unavailable();

void xMAT(int *err) { UNUSED(err); matlab_unavailable(); }

#endif
