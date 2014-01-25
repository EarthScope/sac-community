
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void /*FUNCTION*/ xscallop(nerr)
int *nerr;
{
	int idum, indexdata, indexheader, itemp1, itemp2, itemp3, 
	 jdfl, notused, nptslist[MDFL], numfiles, specindex, speclength, 
	 specsize, specwidth, nchar;
	int lprint = FALSE , ltry = FALSE ;
	float begin, deltalist[MDFL], depmax, depmen, depmin, xmaximum, 
	 xminimum, ymaximum, yminimum;
        float *sdata, *scdata;
	static double window = 2.0;
	static double slice = 1.0;
        float ymin = VLARGE;
        float ymax = -1.0;
        static float fmin = 2.0;
        static float fmax = 6.0;
        static int lmean = TRUE;
	static char type[4] = "mem";
        static char imagetype[6] = "color";
	static int order  = 200;
        static int morder = 100;
        static int lorder = FALSE;
        static int lbinary = FALSE;
        static int lcbar = TRUE;
        static int sfft = 1024;

        static int cnumber = 1;               /* number of correlation windows */
        static int lcnumber = FALSE;          /* was cnumber set by user? */

        static float cwinlength = 0.0;         /* length of correlation window (in seconds)
                                                  should default to the window size of the image (WINDOW) */
        static int lcwinlength = FALSE;       /* was cwinlength set? if not default to value of WINDOW */

        static char cwintype[9] = "HAMMING ";  /* correlation window type */
        int cwindex;

        static char scale[11] = "STOCHASTIC";  /* type of scaling */

	float *const Deltalist = &deltalist[0] - 1;
	int *const Nptslist = &nptslist[0] - 1;

	double ftemp ;	/* added to allow setfhv to pass things by reference. maf 970917 */
  double tmp;

	/*=====================================================================
	 * PURPOSE:  To execute the action command SCALLOP
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
	while( lcmore( nerr ) ){

		/* -- WINDOW v: define window size of image. */
		if( lkreal( "WINDOW$",8, &window ) ){
		}

		else if( lklog( "CBAR$",8, &lcbar ) )
		{ /* do nothing */ }

		/* -- SLICE v:  define slice size for image. */
		else if( lkreal( "SLICE$",7, &slice ) ){
		}
		else if( lkreal( "YMAX#$",7, &tmp ) ) {
      ymax = (float)tmp;
    }
		else if( lkreal( "YMIN#$",7, &tmp ) ) {
      ymin = (float)tmp;
    }

		/* -- CORRELATION v: correlation function */
	        else if(lkchar("COR#RELATION$",14,3,type,4,&nchar))
		{ /* do nothing */ }

		/* -- METHOD v: correlation function */
	        else if(lkchar("M#ETHOD$",9,3,type,4,&nchar))
		{ /* do nothing */ }

          else if( lkreal( "FMIN$",6, &tmp )) {
            fmin = (float) tmp;
          }
          else if( lkreal( "FMAX$",6, &tmp )) { 
            fmax = (float) tmp;
          }

		else if( lclog2( "MEAN$",6, "MEDIAN$",8, &lmean ) )
		{ /* do nothing */ }

		else if( lclog2( "BINARY$",6, "FULL$",8, &lbinary ) )
		{ /* do nothing */ }

		/* -- ORDER v: order of correlation function */
		else if( lkirc( "ORDER$",7, 10, 400, &order ) ){
                        lorder = TRUE; /* user specified order */
		}

                /* number of points in the spectral estimate */
		else if( lkirc( "NUMBER$",8, 512, 2048, &sfft ) ){
			sfft = next2( sfft );
		}

		/* -- Color image    */
                else if(lckey("C#OLOR$",8)) {
                    strcpy(imagetype,"color");
		}

		/* -- Greyscale image */
                else if(lckey("G#REY$",7))  {
                    strcpy(imagetype,"grey");
		}
                else if(lckey("G#RAY$",7))  {
                    strcpy(imagetype,"grey");
		}

                else if(lkreal("CWL#ENGTH$",11,&tmp)){
                    lcwinlength = TRUE;
                    cwinlength = (float) tmp;
		}

                else if(lklogi("CWNUM#BER$",11,&cnumber,&lcnumber))
		{ /* do nothing */ }

                else if(lklist("CWT#YPE&",9, (char *)kmspe.kwintp,9,MWINTP,&cwindex)){
                    strcpy(cwintype,(char *)kmspe.kwintp[cwindex]);
		}

		/* if prewhitening is requested prewhiten all the input traces and replace the
                   data in memory with the prewhitened data first, then call the spectrogram
                   routine with these.  Prewhitening will alter the number of points in the
                   input, so have to be sure to get that right. */
/*              else if("PREW#HITEN&",12,&lprew,&nprew)
		{ * do nothing * }  */

                else if(lckey("STOCH#ASTIC&",13)){
                  strcpy(scale,"STOCHASTIC");
		}

                else if(lckey("TRANS#IENT&",12)){
                  strcpy(scale,"TRANSIENT");
		}

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
	}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

        if( !lorder  && (!strncmp(type,"MEM",3) || !strncmp(type,"MLM",3))) order = morder;

	if( *nerr != 0 ){
           goto L_8888;
	}
	else{
	    if( (window <= 0.0) || (slice <= 0.0) ){
		fprintf( stdout, "Error: WINDOW and SLICE must be positive (xspectrogram).\n" );
		goto L_8888;
	    }
	    else if( slice > window ){
		fprintf( stdout, "Error: SLICE can not be greater than WINDOW(xspectrogram).\n" );
		goto L_8888;
	    }
	    else{
		/* cwinlength defaults to size of window */
		if(!lcwinlength) cwinlength = window;

		/* - Get number of files in data file list. */
		getnfiles( &numfiles );

		/* CHECKING PHASE:
		 * - Check for null data file list. */
		vflist( nerr );
		if( *nerr != 0 ){
		    goto L_8888;
		}
		else{
		    /* - Check to make sure all files are evenly spaced time series files. */
		    vfeven( nerr );
		    if( *nerr != 0 ){
			goto L_8888;
		    }
		    else{
			/* - Perform the requested function on each file in DFL. */
			for( jdfl = 1; jdfl <= numfiles; jdfl++ ){

			    /* -- Get the next file and their lengths in DFL, moving header to CMHDR. */
			    getfil( jdfl, TRUE, &Nptslist[jdfl], &idum, &idum, nerr );
			    if( *nerr != 0 )
				goto L_8888;

			    /* -- Get sampling interval of data. */
			    getfhv( "DELTA", &Deltalist[jdfl], nerr , 5 );
			    if( *nerr != 0 )
				goto L_8888;

			    /* -- Get begin value if first file. */
			    if( jdfl == 1 ){
				getfhv( "B", &begin, nerr , 1 );
				if( *nerr != 0 )
				    goto L_8888;
			    }

			    /* -- Reverse the steps used in getting the next file in DFL. */
			    putfil( jdfl, nerr );
			    if( *nerr != 0 )
				goto L_8888;

			} /* end for */

			/* -- Check if all files have same delta. */
			for( jdfl = 1; jdfl <= numfiles; jdfl++ ){
			    if( Deltalist[1] != Deltalist[jdfl] )
				*nerr = 1;
			}

			/* EXECUTION PHASE: */
			if( *nerr != 0 ){
			    /* 'Sampling intervals of files not equal.' */
			    goto L_8888;
			}
			else{
			    if ( fmax > (1.0/(2.0 * Deltalist[1]))){
				/* fmax should be <= nyquist */
				*nerr = 1;
				printf("Error:  Fmax > Nyquist\n");
				goto L_8888;
			    }

			    if( spectrogram( window, slice, type, &order, 
				numfiles, nptslist, (double)Deltalist[1], &specindex, 
				&specwidth, &speclength, sfft, cwinlength, lcnumber, cnumber,
				cwintype, scale ) != 0 ){
				goto L_8888;
			    }
			}
		    }
		}
	    }
	}

        /* - Do the spectrogram graphical output. */

        /* Flip the x and y storage */
        if((sdata = (float *)malloc(specwidth*speclength*sizeof(float))) == NULL) {
	    printf("error allocating sdata-xspectrogram\n");
	    *nerr = 301;
	    goto L_8888;
	}

        flipdata(cmmem.sacmem[specindex],specwidth,speclength,sdata);
	relamb( cmmem.sacmem, specindex, nerr );
	if( *nerr != 0 )
	    goto L_8888;

        if((scdata = (float *)malloc(specwidth*speclength*sizeof(float))) == NULL) {
	    printf("error allocating scdata-xspectrogram\n");
	    *nerr = 301;
	    goto L_8888;
	}


	xminimum = begin + 0.5*window;
	xmaximum = xminimum + (float)( speclength - 1 )*slice;
        yminimum = 0.0;
        ymaximum =  0.5/Deltalist[1];

        scallop(sdata,specwidth,speclength,ymaximum,fmin,fmax,lmean,scdata,nerr);
        if(*nerr != 0) goto L_8888;


        /* nxsize = speclength, nysize = specwidth */
        if(ymax <= 0.0) ymax = ymaximum;
        if(ymin > 1.e30 ) ymin = yminimum;
        specplot(scdata,speclength,specwidth,xminimum,xmaximum,yminimum,ymaximum,
                 ymin,ymax,imagetype,lbinary,lcbar,lprint,nerr);
        if( *nerr != 0 )
	    goto L_8888;
        
	/* - Replace data in memory with spectrogram. */

	/* -- Clear current data file list. */
	cleardfl( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* -- Create space for a single data file. */
	setnfiles( 1 );
	specsize = specwidth*speclength;
	crsac( 1, 1, specsize, &indexheader, &indexdata, &notused, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	getfil( 1, TRUE, &itemp1, &itemp2, &itemp3, nerr );
	if( *nerr != 0 )
	    goto L_8888;

        /* Store the spectrogram data in sacmem */
        memcpy((char *)cmmem.sacmem[indexdata],(char *)scdata,specsize*sizeof(float));


        free(sdata);
        free(scdata);

	/* -- Store header values. */
	setnhv( "NPTS", &specsize, nerr, 4 );
	if( *nerr != 0 )
	    goto L_8888;
	ftemp = 1.0 ;
	setfhv( "DELTA", (float *)&ftemp, nerr, 5 );
	if( *nerr != 0 )
	    goto L_8888;
	ftemp = 0.0 ;
	setfhv( "B", (float *)&ftemp, nerr, 1 );
	if( *nerr != 0 )
	    goto L_8888;
	ftemp = (double)( specsize - 1 ) ;
	setfhv( "E", (float *)&ftemp, nerr, 1 );
	if( *nerr != 0 )
	    goto L_8888;

	setihv( "IFTYPE", "IXYZ", nerr, 6, 4 );
	if( *nerr != 0 )
	    goto L_8888;
	setnhv( "NXSIZE", &speclength, nerr, 6 );
	if( *nerr != 0 )
	    goto L_8888;
	setnhv( "NYSIZE", &specwidth, nerr, 6 );
	if( *nerr != 0 )
	    goto L_8888;
	ftemp = xminimum ;
	setfhv( "XMINIMUM", (float *)&ftemp, nerr, 8 );
	if( *nerr != 0 )
	    goto L_8888;
	ftemp = xmaximum ;
	setfhv( "XMAXIMUM", (float *)&ftemp, nerr, 8 );
	if( *nerr != 0 )
	    goto L_8888;
	ftemp = yminimum ;
	setfhv( "YMINIMUM", (float *)&ftemp, nerr, 8 );
	if( *nerr != 0 )
	    goto L_8888;
	ftemp = ymaximum ;
	setfhv( "YMAXIMUM", (float *)&ftemp, nerr, 8 );
	if( *nerr != 0 )
	    goto L_8888;

	extrma( cmmem.sacmem[indexdata], 1, specsize, &depmin, &depmax, &depmen );
	ftemp = depmin ;
	setfhv( "DEPMIN", (float *)&ftemp, nerr, 6 );
	if( *nerr != 0 )
	    goto L_8888;
	ftemp = depmax ;
	setfhv( "DEPMAX", (float *)&ftemp, nerr, 6 );
	if( *nerr != 0 )
	    goto L_8888;
	ftemp = depmen ;
	setfhv( "DEPMEN", (float *)&ftemp, nerr, 6 );
	if( *nerr != 0 )
	    goto L_8888;

	/* -- Return file to memory manager. */
	putfil( 1, nerr );

L_8888:

	return;

} /* end of function */






