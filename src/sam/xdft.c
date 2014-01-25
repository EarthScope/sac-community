
#include <stdio.h>

#include "sam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "msg.h"
#include "icm.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xdft(nerr)
int *nerr;
{
	int jdx, jdfl, jdfl_, jj, 
	 ndx1, ndx2, ndxold, nfreq, nlnnew, nlnold, ntused, ndxscr1,
         ndxscr2;

	float scalef;

        float *Sacmem, *Sacmem2;

        double *re, *im;

	/*=====================================================================
	 * PURPOSE:  To execute the action command DFT.
	 *           This command takes the discrete Fourier transform of data.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    HDR:     DEPMEN, IRLIM, IAMPH
	 *    SAM:     MFFT
	 *    DFM:     data set storage stuff.
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    DFM:     NDXDTA, NLNDTA, NCOMP
	 *    HDR:     NSNPTS, NPTS, IFTYPE, SB, SDELTA, B, DELTA, E
	 *    SAM:     LWMEAN, LRLIM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, LCLOG2, CFMT, CRESP, VFLIST, VFEVEN, VFMAXN,
	 *             GETFIL, ALLAMB, COPY, FILL, RELAMB, CPFT, TOAMPH, PUTFIL,
	 *             SETMSG, APFMSG, OUTMSG, NEXT2
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    NPTSMX:  Maximum number of points in files in DFL. [i]
	 *    NLNOLD:  Number of points in current file before transform.. [i]
	 *    NDXOLD:  Index in SACMEM array of time-series before transform. [i]
	 *    NLNNEW:  Number of points in current file after transform. [i]
	 *    NDX1:    Index in SACMEM of first component after transform. [i]
	 *    NDX2:    Index in SACMEM of second component after transform. [i]
	 *    NFREQ:   Number of frequencies in transformed data. [i]
	 *    SCALEF:  Scaling factor applied after transform. [f]
	 *    NDSFILES: Number of files in a given data-set. [i]
	 *    NDSFLNUM: File number (senquental) in the current data set. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920422:  Added msg about default change in version 10.6e.
	 *    911015:  Added data-set storage, two sections of code.
	 *    850614:  Major rewrite due to addition of new memory manager.
	 *    821122:  Changed maximum DFT to 65536.
	 *             Deleted double precision DFT option.
	 *    820621:  Changed to newest set of parsing and checking functions.
	 *    810414:  Minor changes relating to new CMSAM.
	 *    810120:  Changed to output message retrieval from disk.
	 *    800202:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850617
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	while( lcmore( nerr ) ){

	    /* -- WMEAN/WOMEAN: mean is retained/discarded after transform. */
	    if( lclog2( "WMEAN$",7, "WOMEAN$",8, &cmsam.lwmean ) )
	    { /* do nothing */ }

	    /* -- RLIM/AMPH: output will be in real-imaginary/amplitude-phase format. */
	    else if( lclog2( "RLIM$",6, "AMPH$",6, &cmsam.lrlim ) )
	    { /* do nothing */ }

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }

	} /* end while */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
	    goto L_8888;

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Check to make sure all files are evenly spaced time series files. */

	vfeven( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* EXECUTION PHASE: */

	/* - Added temporary msg that fft default changed in 10.6e 
	if( cmsam.lwmean ){
	    fprintf( MUNOUT, "(10.6e)FFT default change: not removing the mean\n" );
	}
	*/

	/* - For each file in (active working-storage) data file list: */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	    jdfl_ = jdfl - 1;

	    /* -- Get file from file manager. */
	    getfil( jdfl, TRUE, &nlnold, &ndx1, &ntused, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Compute length of data after transform. */
	    Ncomp[jdfl] = 2;
	    nlnnew = next2( nlnold );

	    /* -- If transformed length is greater than current length. */
	    if( nlnnew > nlnold ){

		/* --- Allocate memory block for first component. */
		ndxold = ndx1;
		allamb( &cmmem, nlnnew, &ndx1, nerr );
		if( *nerr != 0 ){
		    /* ***** buffered mode ****** */
		}
		/* --- Update DFM entries. */
		Nlndta[jdfl] = nlnnew;
		cmdfm.ndxdta[jdfl_][0] = ndx1;
		/* --- Copy time-series data into first block and zero fill. */
		/* copy( (int*)cmmem.sacmem[ndxold], (int*)cmmem.sacmem[ndx1], nlnold ); */
		copy_float( cmmem.sacmem[ndxold], cmmem.sacmem[ndx1], nlnold );
		fill( cmmem.sacmem[ndx1]+nlnold, nlnnew - nlnold, 0. );
		/* --- Release old data block containing time-series. */
		relamb( cmmem.sacmem, ndxold, nerr );
		if( *nerr != 0 )
		    goto L_8888;
	    } /* end if( nlnnew > nlnold ) */

	    /* -- Allocate memory block for second component and zero fill. */
	    allamb( &cmmem, nlnnew, &ndx2, nerr );
	    if( *nerr != 0 ){
		return ;
	    }
	    cmdfm.ndxdta[jdfl_][1] = ndx2;
	    fill( cmmem.sacmem[ndx2], nlnnew, 0. );

	    /* -- Remove the mean before transform if requested. */
	    if( !cmsam.lwmean ){
                Sacmem = cmmem.sacmem[ndx1];
		for( jdx = ndx1; jdx <= (ndx1 + nlnold - 1); jdx++ ){
		    *(Sacmem++) -= *depmen;
		}
	    } /* end if( !cmsam.lwmean ) */


            /* allocate memory to store data for call to double precision fft routine */

            allamb( &cmmem, 2*nlnnew, &ndxscr1, nerr );
            if( *nerr != 0 ) {
                printf("error allocating memory-xdft\n");
                goto L_8888;
	    }

            allamb( &cmmem, 2*nlnnew, &ndxscr2, nerr );
            if( *nerr != 0 ) {
                printf("error allocating memory-xdft\n");
                goto L_8888;
	    }

            re = (double *)cmmem.sacmem[ndxscr1];
            im = (double *)cmmem.sacmem[ndxscr2];

            Sacmem = cmmem.sacmem[ndx1];
            Sacmem2 = cmmem.sacmem[ndx2];

            for ( jdx=0; jdx<nlnnew; jdx++){
                *re++ = (double)*Sacmem++;
                *im++ = (double)*Sacmem2++;
	    }

            re = (double *)cmmem.sacmem[ndxscr1];
            im = (double *)cmmem.sacmem[ndxscr2];

	    /* -- Perform FFT. */
            dcpft(re, im, nlnnew, 1, cmsam.ifwd);


/*	    cpft( cmmem.sacmem[ndx1], cmmem.sacmem[ndx2], nlnnew, 1, cmsam.ifwd ); */

            re = (double *)cmmem.sacmem[ndxscr1];
            im = (double *)cmmem.sacmem[ndxscr2];

            Sacmem = cmmem.sacmem[ndx1];
            Sacmem2 = cmmem.sacmem[ndx2];

            for ( jdx=0; jdx<nlnnew; jdx++){
                *Sacmem++ = (float)*re++;
                *Sacmem2++ = (float)*im++;
	    }

            relamb(cmmem.sacmem, ndxscr1, nerr);
            if( *nerr != 0 ) {
                printf("error releasing memory-xdft\n");
                goto L_8888;
	    }

            relamb(cmmem.sacmem, ndxscr2, nerr);
            if( *nerr != 0 ) {
                printf("error releasing memory-xdft\n");
                goto L_8888;
	    }

	    nfreq = nlnnew/2;
	    scalef = *delta;
	    *(cmmem.sacmem[ndx1]) *= scalef;
	    *(cmmem.sacmem[ndx1] + nfreq) *= scalef;
	    for( jdx = 1; jdx <= (nfreq - 1); jdx++ ){
		*(cmmem.sacmem[ndx1] + jdx) *= scalef;
		*(cmmem.sacmem[ndx2] + jdx) *= scalef;
		jj = nlnnew - jdx;
		*(cmmem.sacmem[ndx1] + jj) = *(cmmem.sacmem[ndx1] + jdx);
		*(cmmem.sacmem[ndx2] + jj) = -(*(cmmem.sacmem[ndx2] + jdx));
	    }
	    setmsg( "OUTPUT", 1607 );
	    apfmsg( *cmmem.sacmem[ndx1] );
	    outmsg();

	    /* -- Adjust header to reflect new status. */
	    *nsnpts = *npts;
	    *npts = nlnnew;
	    *iftype = *irlim;
	    *sb = *b;
	    *sdelta = *delta;
	    *b = 0.;
	    *delta = 1./(*delta*(float)( *npts ));
	    *e = *b + (float)( nfreq )**delta;
	    if( !cmsam.lrlim ){
		toamph( cmmem.sacmem[ndx1], cmmem.sacmem[ndx2], *npts, cmmem.sacmem[ndx1], 
			cmmem.sacmem[ndx2] );
		*iftype = *iamph;
	    }

	    /* -- Give file back to memory manager. */
	    putfil( jdfl, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	} /* end for ( jdfl ) */

L_8888:
	return;

} /* end of function */

