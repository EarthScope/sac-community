
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mach.h"
#include "xyz.h"
#include "spe.h"
#include "gem.h"
#include "gdm.h"
#include "amf.h"
#include "bool.h"


#include "bot.h"
#include "ucf.h"
#include "dfm.h"
#include "msg.h"
#include "cpf.h"
#include "dff.h"

void 
xspectrogram(int *nerr)
{
	int idx, idum, indexdata, indexheader, itemp1, itemp2, itemp3, 
	 jdfl, notused, nptslist[MDFL], numfiles, specindex, speclength, 
	 specsize, specwidth, nchar;
	int lprint = FALSE , ltry = FALSE ;
	float begin, deltalist[MDFL], depmax, depmen, depmin, xmaximum, 
	 xminimum, ymaximum, yminimum;
        float *sdata;
        float ymax = -1.0;
        float ymin = VLARGE;
	static double window = 2.0;
	static double slice = 1.0;

        double tmp;
	static char type[4] = "mem";
        static char imagetype[6] = "color";
	static int order  = 200;
        static int morder = 100;
        static int lorder = FALSE;
        static int lcbar  = TRUE;
        static int sfft = 1024;
        static int cnumber = 1;            /* number of correlation windows */
        static int lcnumber = FALSE;       /* was cnumber set by user? */

	/* length of correlation window (in seconds) should default to
	   the window size of the image (WINDOW) */
        static float cwinlength = 0.0;

	/* was cwinlength set? if not default to value of WINDOW */
        static int lcwinlength = FALSE;

        static char cwintype[9] = "HAMMING ";  /* correlation window type */
        int cwindex;
        
        static char scale[11] = "STOCHASTIC";  /* type of scaling */

        static int lsqrt  = FALSE;
        static int llog   = FALSE;
        static int llog10 = FALSE;


	float *const Deltalist = &deltalist[0] - 1;
	int *const Nptslist = &nptslist[0] - 1;

	float ftemp ;	/* let setfhv pass things by reference. maf 970917 */

	/*=====================================================================
	 * PURPOSE:  To execute the action command SPECTROGRAM
	 *           This command computes a spectrogram of data in memory.
	 *           The spectrogram is held in memory.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  0 - no error, .ne. 0 - error
	 *=====================================================================
	 * MODULE/LEVEL:  IMAGING/2
	 *=====================================================================
	 * GLOBAL INPUT: 
	 *  inc/mach:     MDFL
	 *=====================================================================
	 * GLOBAL INPUT: 
	 *  inc/mem:      sacmem
	 *=====================================================================
	 * GLOBAL COUPLING: none
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:  cfmt, cresp, vflist, vfeven, 
	 *          getnfiles, getfil, putfil, gethfv, spectrogram, flipdata,
	 *          cleardfl, setnfiles, crsac, setnfv, setihv, setfhv
	 *=====================================================================
	 * LOCAL VARIABLES: see below
	 *=====================================================================
	 * ASSUMPTIONS: none
	 *=====================================================================
	 * LIMITATIONS: Although this routine is set up to receive correlation
	 *              function as an input, only the 'pds' function is 
	 *              implemented.
	 *=====================================================================
	 * KNOWN ERRORS: none
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900308:  Added coding to replace data in memory with spectrogram.
	 *             Retained WINDOW and SLICE values between executions.
	 *    900129:  Original version by Terri Quinn.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900308
	 *===================================================================== */
	/* LOCAL VARIABLES */
	/* EXTERNALS:  */
	/* PROCEDURE: */
	*nerr = 0;
    begin = 0.0;
    memset(deltalist, 0, sizeof(deltalist));
	/* - Loop on each token in command: */
	while ( lcmore( nerr ) ){

	    /* -- WINDOW v: define window size of image. */
    if( lkreal( "WINDOW$",8, &window ) ) {
    }
	    else if( lklog( "CBAR#$",7, &lcbar ) )
	    { /* do nothing */ }

	    else if( lckey( "SQ#RT$",7 ) )
		lsqrt = TRUE;

	    else if( lckey( "NLOG#$",7 ) )
		llog  = TRUE;

	    else if( lckey( "LOG10#$",8 ) )
		llog10 = TRUE;

	    else if( lckey( "NOSCALING#$",12 ) ){
		lsqrt  = FALSE;
		llog   = FALSE;
		llog10 = FALSE;
	    }

	    /* -- SLICE v:  define slice size for image. */
	    else if( lkreal( "SLICE$",7, &slice ) ) {
      }
            /* -- YMAX specify size of output image in y direction */
	    else if( lkreal( "YMAX#$",7, &tmp ) ) { 
        ymax = (float) tmp;
      }

	    /* -- YMIN specify begining of output image in y direction */
	    else if( lkreal( "YMIN#$",7, &tmp ) ) { 
        ymin = (float) tmp;
      }

	    /* -- CORRELATION v: correlation function */
	    else if(lkchar("COR#RELATION$",14,3,type,4,&nchar))
	    { /* do nothing */ }

	    /* -- METHOD v: correlation function */
	    else if(lkchar("M#ETHOD$",9,3,type,4,&nchar))
	    { /* do nothing */ }

	    /* -- ORDER v: order of correlation function */
	    else if( lkirc( "ORDER$",7, 10, 400, &order ) )
		lorder = TRUE;  /* user specified order */

            /* number of points in the spectral estimate */
	    else if( lkirc( "NUMBER$",8, 512, 2048, &sfft ) )
		sfft = next2( sfft );

	    /* -- Color image    */
            else if(lckey("C#OLOR$",8))
		strcpy(imagetype,"color");

	    /* -- Greyscale image */
            else if(lckey("G#REY$",7))
		strcpy(imagetype,"grey");

	    /* -- Greyscale image */
            else if(lckey("G#RAY$",7))
		strcpy(imagetype,"grey");

            else if(lkreal("CWL#ENGTH$",11,&tmp)) {
              cwinlength = (float) tmp;
              lcwinlength = TRUE;
            }
            else if(lklogi("CWNUM#BER$",11,&cnumber,&lcnumber))
	    { /* do nothing */ }

            else if(lklist("CWT#YPE&",9, (char *)kmspe.kwintp,9,MWINTP,&cwindex))
		strcpy(cwintype,(char *)kmspe.kwintp[cwindex]);

            /*  if prewhitening is requested prewhiten all the input traces and
		replace the data in memory with the prewhitened data first,
		then call the spectrogram routine with these.  Prewhitening
		will alter the number of points in the input, so have to be
		sure to get that right. */
/*          else if ( lklogi ( "PREW#HITEN&" , 12 , &lprew , &nprew ) )
	    { * do nothing * } */

            else if(lckey("STOCH#ASTIC&",13))
		strcpy(scale,"STOCHASTIC");

            else if(lckey("TRANS#IENT&",12))
		strcpy(scale,"TRANSIENT");

            /* if PRINT option is tried, get printer name */
            else if ( ltry ) {
                lcchar ( MAXPRNTRNAMELEN   , kmgem.kptrName ,
                  MAXPRNTRNAMELEN+1 , &notused ) ;
                terminate ( kmgem.kptrName ) ;
                if ( !lprint )
                    kmgem.kptrName[0] = '\0' ;

                ltry = FALSE ;
            }

            /* -- "PRINT":  print the final product */
            else if( lckey( "PRINT#$", 8 ) ) {
                ltry = TRUE ;
                if ( cmgdm.lbegf ) {
                    setmsg ( "WARNING" , 2403 ) ;
                    outmsg () ;
                    clrmsg () ;
                }
                else if ( Lgdon[3] || !Lgdon[2] ) {
                    setmsg ( "WARNING" , 2404 ) ;
                    outmsg () ;
                    clrmsg () ;
                }
                else {
                    lprint = TRUE ;
                }
            }


	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }

	} /* end while */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

        /* if mlm or mem were selected and no order was specified set default for these methods */

        if( !lorder  && (!strncmp(type,"MEM",3) || !strncmp(type,"MLM",3))) order = morder;

	if( *nerr != 0 )
           return ;

	if( (window <= 0.0) || (slice <= 0.0) ){
	    fprintf( stdout, "Error: WINDOW and SLICE must be positive (xspectrogram).\n" );
            return ;
	}

	if( slice > window ){
	    fprintf( stdout, "Error: SLICE can not be greater than WINDOW(xspectrogram).\n" );
            return ;
	}

	/* cwinlength defaults to size of window */
	if(!lcwinlength) cwinlength = window;

	/* - Get number of files in data file list. */
	getnfiles( &numfiles );

	/* CHECKING PHASE:
	 * - Check for null data file list. */
	vflist( nerr );
	if( *nerr != 0 )
	    return ;

	/* - Check to make sure all files are evenly spaced time series files. */
	vfeven( nerr );
	if( *nerr != 0 )
	    return ;

	/* - Perform the requested function on each file in DFL. */
	for( jdfl = 1; jdfl <= numfiles; jdfl++ ){
	    /* -- Get the next file and their lengths in DFL, moving header to CMHDR. */
	    getfil( jdfl, TRUE, &Nptslist[jdfl], &idum, &idum, nerr );
	    if( *nerr != 0 )
		return ;

	    /* -- Get sampling interval of data. */
	    getfhv( "DELTA", &Deltalist[jdfl], nerr, 5 );
	    if( *nerr != 0 )
		return ;

	    /* -- Get begin value if first file. */
	    if( jdfl == 1 ){
		getfhv( "B", &begin, nerr, 1 );
		if( *nerr != 0 )
		    return ;
	    }

	    /* -- Reverse the steps used in getting the next file in DFL. */
	    putfil( jdfl, nerr );
	    if( *nerr != 0 )
		return ;

	} /* end for ( jdfl ) */

	/* -- Check if all files have same delta. */
	for( jdfl = 1; jdfl <= numfiles; jdfl++ ){
	    if( Deltalist[1] != Deltalist[jdfl] )
		*nerr = 1;
	}

	/* EXECUTION PHASE: */

	/* 'Sampling intervals of files not equal.' */
	if( *nerr != 0 )
	    return ;

	if( spectrogram( window, slice, type, &order, numfiles, 
	  nptslist, (double)Deltalist[1], &specindex, &specwidth,
	  &speclength, sfft, cwinlength, lcnumber, cnumber,
	  cwintype, scale ) != 0 )
	    return ;


        /* - Do the spectrogram graphical output. */

        /* Flip the x and y storage */
        if((sdata = (float *)malloc(specwidth*speclength*sizeof(float))) == NULL) {
            printf("error allocating memory-xspectrogram\n");
            *nerr = 301;
            return ;
	}

        flipdata(cmmem.sacmem[specindex],specwidth,speclength,sdata);
	relamb( cmmem.sacmem, specindex, nerr );
	if( *nerr != 0 )
	    return ;

        if( lsqrt ){
            for( idx = 0; idx < (specwidth*speclength); idx++)
		sdata[idx] = sqrt(sdata[idx]);
	}
/*
        if( lexp ){
            for( idx = 0; idx < (specwidth*speclength); idx++)
		sdata[idx] = exp(sdata[idx]);
	}
*/
        if( llog ){
            for( idx = 0; idx < (specwidth*speclength); idx++)
		sdata[idx] = log(sdata[idx]);
	}

        if( llog10 ) {
            for( idx = 0; idx < (specwidth*speclength); idx++)
		sdata[idx] = log10(sdata[idx]);
	}
	xminimum = begin + 0.5*window;
	xmaximum = xminimum + (float)( speclength - 1 )*slice;
        yminimum = 0.0;
        ymaximum =  0.5/Deltalist[1];

        /* nxsize = speclength, nysize = specwidth */
        if(ymax <= 0.0) ymax = ymaximum;
        if(ymin > 1.e30 ) ymin = yminimum;
        specplot(sdata,speclength,specwidth,xminimum,xmaximum,yminimum,ymaximum,
            ymin,ymax,imagetype,FALSE,lcbar,lprint,nerr);
        if( *nerr != 0 ) return ;
        
	/* - Replace data in memory with spectrogram. */

	/* -- Clear current data file list. */
	cleardfl( nerr );
	if( *nerr != 0 )
	    return ;

	/* -- Create space for a single data file. */
	setnfiles( 1 );
	specsize = specwidth*speclength;
	crsac( 1, 1, specsize, &indexheader, &indexdata, &notused, nerr );
	if( *nerr != 0 )
	    return ;

	getfil( 1, TRUE, &itemp1, &itemp2, &itemp3, nerr );
	if( *nerr != 0 )
	    return ;

        /* Store the spectrogram data in sacmem */
        memcpy((char *)cmmem.sacmem[indexdata],(char *)sdata,specsize*sizeof(float));

        free(sdata);

	/* -- Store header values. */
	setnhv( "NPTS", &specsize, nerr, 4 );
	if( *nerr != 0 )
	    return ;
	ftemp = 1.0 ;
	setfhv( "DELTA", &ftemp, nerr, 5 );
	if( *nerr != 0 )
	    return ;
	ftemp = 0.0 ;
	setfhv( "B", &ftemp, nerr, 1 );
	if( *nerr != 0 )
	    return ;
	ftemp = (double)( specsize - 1 ) ;
	setfhv( "E", &ftemp, nerr, 1 );
	if( *nerr != 0 )
	    return ;

	setihv( "IFTYPE", "IXYZ", nerr, 6, 4 );
	if( *nerr != 0 )
	    return ;
	setnhv( "NXSIZE", &speclength, nerr, 6 );
	if( *nerr != 0 )
	    return ;
	setnhv( "NYSIZE", &specwidth, nerr, 6 );
	if( *nerr != 0 )
	    return ;
	ftemp = xminimum ;
	setfhv( "XMINIMUM", &ftemp, nerr, 8 );
	if( *nerr != 0 )
	    return ;
	ftemp = xmaximum ;
	setfhv( "XMAXIMUM", &ftemp, nerr, 8 );
	if( *nerr != 0 )
	    return ;

	ftemp = yminimum ;
	setfhv( "YMINIMUM", &ftemp, nerr, 8 );
	if( *nerr != 0 )
	    return ;
	ftemp = ymaximum ;
	setfhv( "YMAXIMUM", &ftemp, nerr, 8 );
	if( *nerr != 0 )
	    return ;

	extrma( cmmem.sacmem[indexdata], 1, specsize, &depmin, &depmax, &depmen );
	ftemp = depmin ;
	setfhv( "DEPMIN", &ftemp, nerr, 6 );
	if( *nerr != 0 )
	    return ;
	ftemp = depmax ;
	setfhv( "DEPMAX", &ftemp, nerr, 6 );
	if( *nerr != 0 )
	    return ;
	ftemp = depmen ;
	setfhv( "DEPMEN", &ftemp, nerr, 6 );
	if( *nerr != 0 )
	    return ;

	/* -- Return file to memory manager. */
	putfil( 1, nerr );

} /* end of function */

