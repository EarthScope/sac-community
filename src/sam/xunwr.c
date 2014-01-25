
#include <string.h>

#include "sam.h"
#include "amf.h"
#include "dfm.h"
#include "hdr.h"
#include "bool.h"


#include "msg.h"
#include "clf.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xunwr(nerr)
int *nerr;
{
	int lok;
	int int_, j, jdfl, jdfl_, jj, 
	 ndx1, ndx2, ndxaux1, ndxaux2, ndxaux3, ndxold, 
	 nfreq, nlnaux, nlnnew, nlnold, nok, nptsmx, ntused;
	float scalef;

        float *Sacmem1, *Sacmem2;
        char *tmp;

	/*=====================================================================
	 * PURPOSE: To parse and execute the action command UNWRAP.
	 *          This command does a phase unwrapping.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    HDR:     IAMPH
	 *    SAM:     MFFT
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    DFM:     NDXDTA, NLNDTA, NCOMP
	 *    HDR:     NSNPTS, NPTS, IFTYPE, SB, SDELTA, B, DELTA, E
	 *    SAM:     LUNWFZ, NUNWFZ, VUNWIT, VUNWCT
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, LCLOG2, CFMT, CRESP, VFLIST, VFEVEN, VFMAXN,
	 *             GETFIL, ALLAMB, COPY, FILL, RELAMB, UNWRAP, TOAMPH,
	 *             PUTFIL, SETMSG, APIMSG, APCMSG, OUTMSG, NEXT2
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    NPTSMX:  Maximum number of points in files in DFL. [i]
	 *    NDXAUX1: Index in SACMEM array of first scratch array. [i]
	 *    NDXAUX2: Index in SACMEM array of second scratch array. [i]
	 *    NDXAUX3: Index in SACMEM array of third scratch array. [i]
	 *    NLNOLD:  Number of points in current file before transform.. [i]
	 *    NDXOLD:  Index in SACMEM array of time-series before transform. [i]
	 *    NLNNEW:  Number of points in current file after transform. [i]
	 *    NDX1:    Index in SACMEM of first component after transform. [i]
	 *    NDX2:    Index in SACMEM of second component after transform. [i]
	 *    NFREQ:   Number of frequencies in transformed data. [i]
	 *    SCALEF:  Scaling factor applied after transform. [f]
	 * Added for data-set storage.
	 *    NDSFILES: Number of files in a given data-set. [i]
	 *    NDSFLNUM: File number (senquental) in the current data set. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    911015:  Added data-set storage, two sections of code.
	 *    910320:  lok=.false. returned by unwrap changed from WARN to ERR.
	 *    910215:  Debug: remove scaling of phase result (jy)
	 *    870727:  Use memory manager for scratch space rather than fixed
	 *             size arrays inside of unwrap.
	 *    850617:  Major rewrite due to addition of new memory manager.
	 *    820817:  Changed to newest set of parsing and checking functions.
	 *    810806:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850617
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - PARSING PHASE: */

	/* - For each token in command: */

	while ( lcmore( nerr ) ){

	    /* -- "FILL n/OFF/ON":  fill with zeros option. */
	    if( lklogi( "FILL$",6, &cmsam.lunwfz, &int_ ) ){
		int_ = next2( int_ );
		if( int_ <= MFFT )
		    cmsam.nunwfz = int_;

		else{
		    int_ = MFFT;
		    *nerr = 1606;
		    setmsg( "ERROR", *nerr );
		    apimsg( int_ );
		    goto L_8888;
		}
	    }

	    /* -- "INCTHR v":  phase increment threshold parameter. */
	    else if( lkreal( "INTTHR$",8, &cmsam.vunwit ) )
	    { /* do nothing */ }

	    /* -- "CONTHR v":  phase consistency threshold parameter. */
	    else if( lkreal( "PVTHR$",7, &cmsam.vunwct ) )
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

        /* get the maximum number of points in the input files */

	vfmax( &nptsmx, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* EXECUTION PHASE: */

	/* - Allocate three scratch arrays for use by unwrap.
	 *   Each must be equal to the size of the fft being performed. */

	if( cmsam.lunwfz )
	    nlnaux = cmsam.nunwfz;

	else
	    nlnaux = next2( nptsmx );

	allamb( &cmmem, nlnaux, &ndxaux1, nerr );
	if( *nerr != 0 )
	    goto L_8888;
	allamb( &cmmem, nlnaux, &ndxaux2, nerr );
	if( *nerr != 0 )
	    goto L_8888;
	allamb( &cmmem, nlnaux, &ndxaux3, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - For each file in data file list: */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
	    jdfl_ = jdfl - 1;

	    /* -- Get file from memory manager. */
	    getfil( jdfl, TRUE, &nlnold, &ndxold, &ntused, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* - Compute length of data after transform. */
	    Ncomp[jdfl] = 2;
	    if( cmsam.lunwfz ){
		nlnnew = cmsam.nunwfz;
	    }
	    else{
		nlnnew = next2( nlnold );
	    }
	    Nlndta[jdfl] = nlnnew;

	    /* - Allocate memory block for first component. */
	    allamb( &cmmem, nlnnew, &ndx1, nerr );
	    if( *nerr != 0 ){
		/* ***** buffered mode ****** */
	    }
	    cmdfm.ndxdta[jdfl_][0] = ndx1;

	    /* -- Copy time-series data into first block and zero fill. */
	    /* copy( (int*)cmmem.sacmem[ndxold], (int*)cmmem.sacmem[ndx1], nlnold ); */
	    copy_float( cmmem.sacmem[ndxold], cmmem.sacmem[ndx1], nlnold );
	    fill( cmmem.sacmem[ndx1]+nlnold, nlnnew - nlnold, 0. );

	    /* -- Release old data block containing time-series. */
	    relamb( cmmem.sacmem, ndxold, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	    /* -- Allocate memory block for second component and zero fill. */
	    allamb( &cmmem, nlnnew, &ndx2, nerr );
	    if( *nerr != 0 ){
		/* ***** buffered mode ****** */
	    }
	    cmdfm.ndxdta[jdfl_][1] = ndx2;
	    fill( cmmem.sacmem[ndx2], nlnnew, 0. );

	    /* -- Perform phase unwrapping. */
	    unwrap( cmmem.sacmem[ndx1], nlnold, nlnnew, 
              (float)cmsam.vunwct, (float)cmsam.vunwit, 
	     cmmem.sacmem[ndxaux1], cmmem.sacmem[ndxaux2], 
             cmmem.sacmem[ndxaux3], cmmem.sacmem[ndx1], 
	     cmmem.sacmem[ndx2], &nok, &lok );

	    /* -- Check for errors. */
	    if( !lok ){
		*nerr = 1610;
		setmsg( "ERROR", 1610 );
		apimsg( nok );
        tmp = string_list_get(datafiles, jdfl-1);
        apcmsg2(tmp, strlen(tmp)+1);
		outmsg();
		clrmsg();
		goto L_8888;
	    }

	    /* -- Scale the transformed data. */
	    nfreq = nlnnew/2;
	    scalef = *delta;

            Sacmem1 = cmmem.sacmem[ndx1];
            Sacmem2 = cmmem.sacmem[ndx2];
	    *Sacmem1 *= scalef;
	    *(Sacmem1+ nfreq) *= scalef;
	    for( j = 1; j <= (nfreq - 1); j++ ){
		*(Sacmem1+j) *= scalef;
		/*          sacmem(ndx2+j)=scalef*sacmem(ndx2+j) */
		jj = nlnnew - j;
		*(Sacmem1+jj) = *(Sacmem1+j);
		*(Sacmem2+jj) = -*(Sacmem2+j);
	    }

	    /* -- Write DC level to terminal. */
	    setmsg( "OUTPUT", 1607 );
	    if( *(cmmem.sacmem[ndx2]) != 0 ){
		apfmsg( -*(cmmem.sacmem[ndx1]));
	    }
	    else{
		apfmsg( *(cmmem.sacmem[ndx1]) );
	    }
	    outmsg();
	    clrmsg();

	    /* -- Adjust header to reflect new status. */
	    *nsnpts = *npts;
	    *npts = nlnnew;
	    *iftype = *iamph;
	    *sb = *b;
	    *sdelta = *delta;
	    *b = 0.;
	    *delta = 1./(*delta*(float)( *npts ));
	    *e = *b + (float)( nfreq )**delta;

	    /* -- Give file back to memory manager. */
	    putfil( jdfl, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

	/* - Release scratch space. */

	relamb( cmmem.sacmem, ndxaux1, nerr );
	relamb( cmmem.sacmem, ndxaux2, nerr );
	relamb( cmmem.sacmem, ndxaux3, nerr );

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

} /* end of function */

