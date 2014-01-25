
#include <stdio.h>
#include <string.h>

#include "sam.h"

#include "string_utils.h"


#include "co.h"
#include "msg.h"
#include "dfm.h"

void /*FUNCTION*/ rfir(knmfir, knmfir_s, mfir, cfir, nfir, desdt, 
	 kidfir, kidfir_s, nerr)
char *knmfir;   int knmfir_s;
int mfir;
float cfir[];
int *nfir;
float *desdt;
char *kidfir;   int kidfir_s;
int *nerr;
{
	char kcard[MCMSG+1];
	int idx, iffir[255], jstart, nleft, nread, numsave;
        FILE *nun;
        int ierr;

	float *const Cfir = &cfir[0] - 1;

	memset ( kcard , ' ' , MCMSG ) ;
	kcard[ MCMSG ] = '\0' ;


	/*=====================================================================
	 * PURPOSE: To read FIR filter coefficient file into memory.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    KNMFIR:  Name of FIR filter file. [c32]
	 *    MFIR:    Maximum length of CFIR array. [i]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    CFIR:    FIR filter coefficients array. [f]
	 *    NFIR:    Number of coefficients in this filter. Length of CFIR. [i]
	 *    DESDT:   Design sampling interval in seconds. [f]
	 *    KIDFIR   Alphanumeric identifier for filter. [c80]
	 *             Characters 1-10 describe the FIR filter class:
	 *             ='FIR-FILT: ' for a filter.
	 *             ='FIR-HILB: ' for a Hilbert transform.
	 *             ='FIR-DIFF: ' for a differentiator.
	 *             Characters 11-80 contains identifying information.
	 *     NERR:   Error return flag.
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  ZGTFUN, ZOPEN, ZCLOSE, GTOUTM, CNVFRE
	 *=====================================================================
	 * MODIFICATION HISTORY:
         *    970129:  Add parameter (1) to cnvfre.  1 means that if a string
         *             of digits is too long, warn user & end command.  maf 
	 *    870727:  Changed to free format for coefficient  file.
	 *    840130:  Changed format of filter coefficient file.
	 *    801121:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870727
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	zopens( &nun, knmfir,knmfir_s, "ROTEXT",7, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Read in header. */

	if(fgetsp( kidfir,kidfir_s,nun)==NULL) goto L_9000;
	if(kidfir[(numsave=strlen(kidfir)-1)] == '\n') kidfir[numsave] = ' ';


	if(fgetsp( kcard,MCMSG+1,nun)==NULL) goto L_9000;
	if(sscanf(kcard," %f %d",desdt,&nread) != 2){
	    printf("error reading desdt,nread-rfir\n");
	    goto L_9000;
	}
        
	/* - Set up loop parameters. */

	if( memcmp(kidfir,"FIR-FILT",8) == 0 ){
	    jstart = 1;
	    *nfir = nread;
	}
	else{
	    Cfir[1] = 0.;
	    jstart = 2;
	    *nfir = nread + 1;
	}

	/* - Make sure coefficient array is large enough. */

	if( *nfir > mfir ){
	    setmsg( "WARNING", 803 );
	    apcmsg( knmfir,knmfir_s );
	    outmsg();
	    *nfir = mfir;
	}

	/* - Initialize to read all fields on a card */

	for( idx = 0; idx < mfir; idx++ ){
	    iffir[idx] = 1;
	}

	/* - Read each of the remaining cards and decode coefficients. */

	nleft = *nfir - jstart + 1;

	while( nleft > 0 ){
	    if(fgetsp( kcard,MCMSG+1,nun)==NULL) goto L_9000;
	    if(kcard[(numsave=strlen(kcard)-1)] == '\n') kcard[numsave] = ' ';

	    cnvfre( kcard , MCMSG+1 , nleft + 1 , &nread , &Cfir[ jstart ] , iffir ,
		    kcard , MCMSG+1 , 1 , nerr ) ;
	    if( *nerr != 0 )
		goto L_9000;
	    jstart = jstart + nread;
	    nleft = nleft - nread;
	}

L_8888:
	zcloses( &nun, &ierr );
	return;

	/* - Error reading filter coefficient file. */

L_9000:
	*nerr = 2011;
	setmsg( "ERROR", *nerr );
	apcmsg( knmfir,knmfir_s );
	goto L_8888;

} /* end of function */

