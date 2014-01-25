
#include <stdio.h>
#include <math.h>

#include "scm.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "ucf.h"
#include "cpf.h"
#include "dff.h"

static double sWinLen=0.5;
static double Thresh2=5.0;
static double MinGlitchAmp=50.0;
int PntsInWin;


float R4Mean(array, Nsamples)
float *array ;
int Nsamples ;
{
   int j;
   float result;
   
   if(Nsamples < 1 )
      return(0.0);
      
   result=0.0;
   for(j = 0; j < Nsamples; j++)
      result += array[j];
   result=result/Nsamples;
   return(result);
}




float VarianceR4(array, Nlen)
float *array ;
int Nlen ;
{
   int j;
   float dot, deviation;
   float mean;

   if(Nlen < 2)
      return(0.0);
      
   mean=R4Mean(array,Nlen);
   
   dot=0.0;
   for(j=0; j < Nlen; j++){
      deviation = array[j]-mean;
      dot += deviation * deviation;
   }
   return(dot/(Nlen-1));
}




void RglitchR4(data, NPTS)
float *data ;
int NPTS ;
{
   int Nwins;
   float dataMean, dataVar;
   int j,lst;
   float deviation,SXX;
   int nglitch=0;
   int Wlength;
   
   Wlength = PntsInWin;
   if(Wlength < 2)
      return;
      
         
      Nwins=NPTS/Wlength;
      if(Nwins < 2)
         return;


/* Initialize mean and variance. */
   dataMean=R4Mean(data+NPTS-Wlength,Wlength);
   dataVar=VarianceR4(data+NPTS-Wlength,Wlength);


   j=NPTS-Wlength-1;
   while (j >= 0){
      deviation=fabs(data[j]-dataMean);
      if( (deviation > Thresh2*sqrt(dataVar)) && (deviation > MinGlitchAmp)){
         data[j]=dataMean;
         nglitch++;
      }
      lst=j+Wlength;
      SXX=(Wlength-1)*dataVar+Wlength*dataMean*dataMean;
      dataMean=(Wlength*dataMean+data[j]-data[lst])/Wlength;
      SXX=SXX+data[j]*data[j]-data[lst]*data[lst];
      dataVar=(SXX-Wlength*dataMean*dataMean)/(Wlength-1);
      j--;
   }
   printf("%d glitches removed. \n",nglitch);
   return;
}



void /*FUNCTION*/ xrglitches(nerr)
int *nerr;
{
	int lthold;
	int jdfl, ndxx, ndxy, nlen, nlnwin, nofwin;
	double twinmn, twinmx;


	/*=====================================================================
	 * PURPOSE: To parse and execute the action command RGLITCHES.
	 *          This command removes glitches from data.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  scm/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    dfm:     ndfl
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    hdr:     depmin, depmax, depmen
	 *    mem:     sacmem
	 *    scm:     lrglwin, krglwin, orglwin
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, cfmt, cresp, lklist, lcchar, vflist, vftime,
	 *             getfil, putfil
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    lgood:   Flag to indicate whether a data point is in a
	 *             good or bad region. [l]
	 *    ibad:    Index of first bad point in window or region. [i]
	 *    nbad:    Number of bad points in data window. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920218:  Added a method to deglithing. MAGNITUDE vs. POWER
	 *    891025:  Added logic so that an active glitch region was 
	 *             terminated by the end of the data window.
	 *    890427:  Added ability to specifiy a time window in
	 *             which to apply glitch removal algorithm.
	 *    870206:  Original version from XSC RGL.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  890427
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;
	lthold = FALSE;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	while( lcmore( nerr ) ){

	    /* -- "THRESHOLD v":  set glitch threshold level. */
	    if( lkreal( "THRESHOLD$",11, &cmscm.thold ) ){
		lthold = TRUE;
	    }

	    /* -- "TYPE type":  select type of smoothing.: */
	    else if( lklist( "TYPE$",6, (char*)kmscm.krgltp,9, MRGLTP, 
		     &cmscm.irgltp ) )
	    { /* do nothing */ }

	    /* -- "WINDOW rtw": define time window in which to remove glitches. */
	    else if( lkrtw( "WINDOW$",8, &cmscm.lrglwin, (char*)kmscm.krglwin
		     ,9, cmscm.orglwin ) )
	    { /* do nothing */ }

	    /* -- "METHOD method":  select method of detecting.: */
	    else if( lklist( "METHOD$",8, (char*)kmscm.krglmt,9, MRGLMT, 
		     &cmscm.irglmt ) )
	    { /* do nothing */ }

	    /* -- Bad syntax. */
			
            else if( lkreal( "SWINLEN$",9, &sWinLen ) )
	    { /* do nothing */ }
			
            else if( lkreal( "THRESH2$",9, &Thresh2 ) )
	    { /* do nothing */ }
			
            else if( lkreal( "MINAMP$",9, &MinGlitchAmp ) )
	    { /* do nothing */ }
			
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	}
	if( ( cmscm.irglmt != 1 ) && !lthold ){
	    cmscm.thold = 0.05;
	}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
	    goto L_8888;

	/* CHECKING PHASE: */

	/* - Test for a non-null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Make sure each file is a time series file. */

	vftime( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* EXECUTION PHASE: */

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

	    /* -- Get next file from the memory manager.
	     *    (Header is moved into common blocks CMHDR and KMHDR.) */
	    getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );
	    if( *nerr != 0 )
		goto L_8888;

            if(cmscm.irglmt == 3){
		PntsInWin = sWinLen / (*delta);
		if( PntsInWin < 10 )
		    PntsInWin = 10;
               
		if( Thresh2 < 4.0)
		    Thresh2=4.0;
               
		RglitchR4(cmmem.sacmem[ndxy], nlen);
            }
            else{
		/* -- Determine time window on which to apply glitch removal. */
		if( cmscm.lrglwin ){
		    getatw( (char*)kmscm.krglwin,9, cmscm.orglwin, &twinmn, 
		     &twinmx, &nofwin, &nlnwin, nerr );
		    if( *nerr != 0 )
			goto L_8888;
		}
		else{
		    nofwin = 0;
		    nlnwin = nlen;
		}

		/* -- Search file for regions outside threshold.
		 *    Perform requested smoothing on data in each region. */
		if( cmscm.irglmt == 1 ){
      xabsgl( cmmem.sacmem[ndxy] + nofwin, nlnwin, (float) cmscm.thold,
		     cmscm.irgltp, nerr );
		}
		else{
		    xpowgl( cmmem.sacmem[ndxy] + nofwin, nlnwin, *delta,
		     cmscm.thold, 0.0, cmscm.irgltp, nerr );
		}
	    }
	    /* -- Update any header fields that may have changed. */
	    extrma( cmmem.sacmem[ndxy], 1, nlen, depmin, depmax, depmen );

	    /* -- Return file to memory manager. */
	    putfil( jdfl, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	} /* end for */

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

} /* end of function */

